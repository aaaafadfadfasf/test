#ifndef __SNMP_WRAPPER_H_
#define __SNMP_WRAPPER_H_

#ifndef __cplusplus
#define true 1
#define false 0
typedef int bool;
#endif

#ifdef __cplusplus
extern          "C" {
#endif

typedef void* AGENTHANDLE;

typedef enum emAuthorityType
{
    emAuthority_NA,
    emAuthority_MD5,
    emAuthority_SHA,
} EAuthorityType;

typedef enum emPrivacyType
{
    emPrivacy_NA,
    emPrivacy_DES,
    emPrivacy_AES,
} EPrivacyType;

typedef enum emObjectType
{
    emObjectType_INTEGER,
    emObjectType_UNSIGNED,
    emObjectType_COUNTER32,
    emObjectType_STRING,
    emObjectType_HEX_STRING,
    emObjectType_DECIMAL_STRING,
    emObjectType_NULLOBJ,
    emObjectType_OBJID,
    emObjectType_TIMETICKS,
    emObjectType_IPADDRESS,
    emObjectType_BITS,
} EObjectType;

typedef struct tagSnmpV3User
{
    bool bIsNoAuthNoPriv;
    EAuthorityType eAuthType;
    EPrivacyType ePrivType;
    char achAuthPass[100];
    char achPrivPass[100];
    char achUsername[100];
} TSnmpV3User;

typedef struct tagSnmpTrapObject
{
    char achObjectOID[100];
    char achObjectValue[100];
    EObjectType eObjectType;
} TSnmpTrapObject;

typedef struct TSnmpAgentConf
{	
    /**
        �û���Э��ջ��ʼ����ʼ֮ǰ��Ҫ��ɵĵ���(���û����о���)
        ע�⺯����ԭ��Ϊint (*fun)(void)
    */
    void* pfInitCB; 
    
    /**
        ָ��snmpd.conf��̬�����ļ���·��
        �����Ϊ�գ���Ĭ��Ϊ./conf
    */
    char* szConfPath;

    /**
        ָ��snmpd.conf�־û������ļ�Ŀ¼
        ���Ϊ�� linux Ĭ��Ϊ/var/net-snmp windowĬ��Ϊc:/usr/snmp/persist
        ע��: szPersistPath��szConfPath������ͬ!!!
    */
    char* szPersistPath;
    
	/** ָ����ʼ��mib�ļ�·�� ��·���µ�mib�ļ����ᱻ����
	    ���Ϊ�� linux Ĭ��Ϊ/usr/local/share/snmp/mibs 
	             windowĬ��Ϊc:/usr/share/snmp/mibs	    
        ʹ��mib�������ܵĻ���SNMPv2-MIB.txt��SNMPv2-SMI.txt�Ǳ���ģ������
        ����ָ���ô��·��
    */
	char* szMibFilePath;  

	/** ��ȫ�ӹ�������,ָ���Ƿ����V1&V2
        ���Ϊtrue ������V1&V2,�Ӷ�V1&V2�����ݰ��ᱻ����,�޷��յ�Ӧ��.ֻ��V3���յ�Ӧ����Ϣ
        ���Ϊfalse ��������V1&V2, V1��V2�Լ�V3�����ݰ������ᱻ�����������յ�Ӧ����Ϣ
 
        ����:LJ300-B�豸��ȫ������SNMP����©���Ͷ���Ķ˿ڣ�����ɨ�裩���������Ӹ������
        ����ϲ����ø�ѡ��(����Ϊtrue), ��ôֻ��V3���յ�Ӧ����Ϣ���Ӷ�����ĳЩsnmp�Ĺ��ܽ���
    */
    int bDisableV1V2;

}TSnmpAgentConf;

//�����������Ѿ����� ��Ҫ���� 
int SnmpdStartup(void);
int SnmpdCleanup(void);

typedef int (*FuncInit)(void);
//pszAgentName�����Ѿ����� �ײ�ǿ������Ϊ"snmpd"
//2017/07/03 ���������������滻ΪSnmpAgentStartupX()/SnmpAgentCleanupX()
//���ﱣ������Ϊ�˼����ϵİ汾
AGENTHANDLE SnmpAgentStartup(char* pszAgentName, FuncInit pfInitAgentModule);
bool SnmpAgentCleanup(AGENTHANDLE hAgentHandle);

/**
* Э��ջ�Ƿ��Ѿ���ʼ��
* @param[in]   ��
* @param[out]  ��
* @retval      TRUE ��ɳ�ʼ��     FALSE δ��ʼ��
* @note       
* @History
*    1. 2017/07/03  1.0   dcg    created
*/
int SnmpAgentInited();

/**
* �����ò�����Э��ջ��ʼ��
* @param[in]   ptInitConf ���ò���
* @param[out]   ��
* @retval      0 ʧ��  ��0 �ɹ�
* @note        
* @History
*    1. 2017/07/03  1.0   dcg    created
*/
AGENTHANDLE SnmpAgentStartupX(TSnmpAgentConf* ptInitConf);
/**
* Э��ջ�˳�����
* @param[in]   ��
* @param[out]  ��
* @retval      ��
* @note  ������SnmpAgentStartupX()���ʹ��      
* @History
*    1. 2017/07/03  1.0   dcg    created
*/
void SnmpAgentCleanupX(AGENTHANDLE hAgentHandle);

/**
* ���������� 
* �漰�޸ĵľ�̬�����ļ�Ϊ"/usr/local/share/snmp/snmpd.conf" linux
*                         "c:/usr/share/snmp/snmpd.conf"     windows
* @param[in]   ��
* @param[out]  ��
* @retval      0 �ɹ�  ����ֵʧ��
* @note        �����˳�Э��ջ������޸ģ����򲻻���Ч   
* @History
*    1. 2017/07/03  1.0   dcg    add comment
*/
int SnmpV2SetROCommunity(char* pszCommunity);
int SnmpV2SetRWCommunity(char* pszCommunity);

/**
* �����û�������ؼ��ܲ��� 
* @param[in]   pszCommunity ������ 
* @param[in]   szFile       �漰�޸ĵľ�̬�����ļ�ȫ��
* @param[out]  ��
* @retval      0 �ɹ�  ����ֵʧ��
* @note      �����˳�Э��ջ������޸ģ����򲻻���Ч
* @History
*    1. 2017/07/03  1.0   dcg    created
*/
int SnmpV2SetROCommunityX(char* pszCommunity, const char* szFile);
int SnmpV2SetRWCommunityX(char* pszCommunity, const char* szFile);

/**
* V3�����û�������ؼ��ܲ��� 
* �漰�޸ĵľ�̬�����ļ�Ϊ"/usr/local/share/snmp/snmpd.conf" linux
*                         "c:/usr/share/snmp/snmpd.conf"     windows
* �漰�޸ĵĶ�̬�����ļ�Ϊ"/var/net-snmp/snmpd.conf"         linux
*                         "c:/usr/snmp/persist/snmpd.conf"   windows
* @param[in]   ��
* @param[out]  ��
* @retval      0 �ɹ�  ����ֵʧ��
* @note      �����˳�Э��ջ������޸ģ����򲻻���Ч     
* @History
*    1. 2017/07/03  1.0   dcg    add comment
*/
int SnmpV3SetROUser(TSnmpV3User tUserInfo);
int SnmpV3SetRWUser(TSnmpV3User tUserInfo);

/**
* V3�����û�������ؼ��ܲ���
* V3�������ļ�ʹ�þ�̬�����ļ��Ͷ�̬�������ļ�, ��������ļ����Ǳ����
* ����v3�û�����̬�����ļ���������: rwuser username  
* ��̬�������ļ���������: ָ���û��� �����Է�ʽ������  ���ܷ�ʽ������
*                         createUser username MD5 "12345678" DES "12345678"
* ��Э��ջ������Ὣ��̬�����ļ��е�createUser���޸�ΪusmUser "username" "username"����ʽ
* @param[in]   szStaticFile       �漰�޸ĵľ�̬�����ļ�ȫ��
* @param[in]   szPersistFile       �漰�޸ĵĶ�̬�����ļ�ȫ��
* @param[out]  ��
* @retval      0 �ɹ�  ����ֵʧ��
* @note        �����˳�Э��ջ������޸ģ����򲻻���Ч 
*              szStaticFile��szPersistFile���붼����ȫ·��, �ļ���Ϊsnmpd.conf������:
* szStaticFileΪ"/usr/local/share/snmp/snmpd.conf"
* szPersistFileΪ"/var/net-snmp/snmpd.conf"
* @History
*    1. 2017/10/11  1.0   dcg    added
*/
int SnmpV3SetROUserX(TSnmpV3User *ptUserInfo, 
        const char *szStaticFile, const char *szPersistFile);
int SnmpV3SetRWUserX(TSnmpV3User *ptUserInfo, 
        const char *szStaticFile, const char *szPersistFile);

int SnmpV2Trap(char* szTrapMgrIpAddr, char* szTrapCommunity, char* szTrapOID, 
    int nObjectNum, TSnmpTrapObject* patTrapObjArray);
int SnmpV3Trap(char* szTrapMgrIpAddr, TSnmpV3User tUserInfo, char* szTrapOID, 
    int nObjectNum, TSnmpTrapObject* patTrapObjArray);

/**
* description: �޸ľ�̬�����ļ��е�syslocation������  
* @param[in]  const char *szStaticFile ��̬�����ļ�·���������ļ�������
* @param[in]  const char* szValue      SysLocation����ֵ
* @retval
* @note  ��̬�����ļ�ȫ��Ӧ�ͳ�ʼ��ʱ�Ĳ���szConfPath����һ�£�����szConfPath/snmpd.conf
* @history
	1. 2018��7��3��	  1.0 	deng	   create & comment
*/
int SnmpSetSysLocation(const char* szStaticFile, const char* szValue);

#ifdef __cplusplus
}
#endif

#endif // __SNMP_WRAPPER_H_

