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
        用户在协议栈初始化开始之前想要完成的调用(由用户自行决定)
        注意函数的原型为int (*fun)(void)
    */
    void* pfInitCB; 
    
    /**
        指定snmpd.conf静态配置文件的路径
        如果设为空，则默认为./conf
    */
    char* szConfPath;

    /**
        指定snmpd.conf持久化配置文件目录
        如果为空 linux 默认为/var/net-snmp window默认为c:/usr/snmp/persist
        注意: szPersistPath和szConfPath不能相同!!!
    */
    char* szPersistPath;
    
	/** 指定初始的mib文件路径 该路径下的mib文件都会被加载
	    如果为空 linux 默认为/usr/local/share/snmp/mibs 
	             window默认为c:/usr/share/snmp/mibs	    
        使用mib解析功能的话，SNMPv2-MIB.txt和SNMPv2-SMI.txt是必须的，最好在
        这里指定好存放路径
    */
	char* szMibFilePath;  

	/** 安全加固配置项,指定是否禁用V1&V2
        如果为true 即禁用V1&V2,从而V1&V2的数据包会被丢弃,无法收到应答.只有V3能收到应答消息
        如果为false 即不禁用V1&V2, V1、V2以及V3的数据包都不会被丢弃，均能收到应答消息
 
        背景:LJ300-B设备安全测试中SNMP存在漏洞和多余的端口（绿盟扫描），所以增加该配置项，
        如果上层启用该选项(设置为true), 那么只有V3能收到应答消息，从而禁用某些snmp的功能交互
    */
    int bDisableV1V2;

}TSnmpAgentConf;

//此两个函数已经废弃 不要调用 
int SnmpdStartup(void);
int SnmpdCleanup(void);

typedef int (*FuncInit)(void);
//pszAgentName参数已经废弃 底层强制设置为"snmpd"
//2017/07/03 这两个函数建议替换为SnmpAgentStartupX()/SnmpAgentCleanupX()
//这里保留仅是为了兼容老的版本
AGENTHANDLE SnmpAgentStartup(char* pszAgentName, FuncInit pfInitAgentModule);
bool SnmpAgentCleanup(AGENTHANDLE hAgentHandle);

/**
* 协议栈是否已经初始化
* @param[in]   无
* @param[out]  无
* @retval      TRUE 完成初始化     FALSE 未初始化
* @note       
* @History
*    1. 2017/07/03  1.0   dcg    created
*/
int SnmpAgentInited();

/**
* 带配置参数的协议栈初始化
* @param[in]   ptInitConf 配置参数
* @param[out]   无
* @retval      0 失败  非0 成功
* @note        
* @History
*    1. 2017/07/03  1.0   dcg    created
*/
AGENTHANDLE SnmpAgentStartupX(TSnmpAgentConf* ptInitConf);
/**
* 协议栈退出清理
* @param[in]   无
* @param[out]  无
* @retval      无
* @note  必须与SnmpAgentStartupX()配对使用      
* @History
*    1. 2017/07/03  1.0   dcg    created
*/
void SnmpAgentCleanupX(AGENTHANDLE hAgentHandle);

/**
* 设置团体名 
* 涉及修改的静态配置文件为"/usr/local/share/snmp/snmpd.conf" linux
*                         "c:/usr/share/snmp/snmpd.conf"     windows
* @param[in]   无
* @param[out]  无
* @retval      0 成功  其他值失败
* @note        必须退出协议栈后进行修改，否则不会生效   
* @History
*    1. 2017/07/03  1.0   dcg    add comment
*/
int SnmpV2SetROCommunity(char* pszCommunity);
int SnmpV2SetRWCommunity(char* pszCommunity);

/**
* 设置用户名及相关加密参数 
* @param[in]   pszCommunity 团体名 
* @param[in]   szFile       涉及修改的静态配置文件全名
* @param[out]  无
* @retval      0 成功  其他值失败
* @note      必须退出协议栈后进行修改，否则不会生效
* @History
*    1. 2017/07/03  1.0   dcg    created
*/
int SnmpV2SetROCommunityX(char* pszCommunity, const char* szFile);
int SnmpV2SetRWCommunityX(char* pszCommunity, const char* szFile);

/**
* V3设置用户名及相关加密参数 
* 涉及修改的静态配置文件为"/usr/local/share/snmp/snmpd.conf" linux
*                         "c:/usr/share/snmp/snmpd.conf"     windows
* 涉及修改的动态配置文件为"/var/net-snmp/snmpd.conf"         linux
*                         "c:/usr/snmp/persist/snmpd.conf"   windows
* @param[in]   无
* @param[out]  无
* @retval      0 成功  其他值失败
* @note      必须退出协议栈后进行修改，否则不会生效     
* @History
*    1. 2017/07/03  1.0   dcg    add comment
*/
int SnmpV3SetROUser(TSnmpV3User tUserInfo);
int SnmpV3SetRWUser(TSnmpV3User tUserInfo);

/**
* V3设置用户名及相关加密参数
* V3的配置文件使用静态配置文件和动态的配置文件, 这个两个文件都是必须的
* 创建v3用户，静态配置文件中增加行: rwuser username  
* 动态的配置文件中增加行: 指定用户名 完整性方式和密码  加密方式和密码
*                         createUser username MD5 "12345678" DES "12345678"
* 当协议栈启动后会将动态配置文件中的createUser行修改为usmUser "username" "username"的形式
* @param[in]   szStaticFile       涉及修改的静态配置文件全名
* @param[in]   szPersistFile       涉及修改的动态配置文件全名
* @param[out]  无
* @retval      0 成功  其他值失败
* @note        必须退出协议栈后进行修改，否则不会生效 
*              szStaticFile和szPersistFile必须都给出全路径, 文件名为snmpd.conf，例如:
* szStaticFile为"/usr/local/share/snmp/snmpd.conf"
* szPersistFile为"/var/net-snmp/snmpd.conf"
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
* description: 修改静态配置文件中的syslocation配置项  
* @param[in]  const char *szStaticFile 静态配置文件路径，包括文件名在内
* @param[in]  const char* szValue      SysLocation配置值
* @retval
* @note  静态配置文件全径应和初始化时的参数szConfPath配置一致，等于szConfPath/snmpd.conf
* @history
	1. 2018年7月3日	  1.0 	deng	   create & comment
*/
int SnmpSetSysLocation(const char* szStaticFile, const char* szValue);

#ifdef __cplusplus
}
#endif

#endif // __SNMP_WRAPPER_H_

