/**
* @file     snmp_nms.h
* @brief    �������ϵͳʵ�֣�snmpЭ�����ܲ���ع��� ֧�ַ���get get-next get-bulk ֧�ֽ���trap
* @author   dcg
* @date     2016-11-10
* @version  0.1
* @copyright Copyright(C) 2016 kedacom All rights reserved.
* @History
*    1. 2017/04/19   1.0   dcg    created
*
*/
#ifndef _SNMP_SNMS_H_
#define _SNMP_SNMS_H_

#ifdef  __USE_NMS_STD_TYPE__
typedef short           s16;
typedef unsigned short  u16;
typedef int             s32;
typedef unsigned int    u32;
typedef s32             BOOL32;

#ifndef TRUE
#define TRUE    1
#endif
#ifndef FALSE
#define FALSE   0
#endif

#else
#include "kdvtype.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct tagSnmpPacket;
struct tagSnmpTrap;

/** ����Ӧ����Ϣ�Ļص�*/
typedef void (*RECV_RSP_CALL_BACK)(struct tagSnmpPacket*);
/** ����trap�¼��Ļص�*/
typedef void (*RECV_TRAP_CALL_BACK)(struct tagSnmpTrap*);

typedef struct tagInitConfig
{
	char* szAppName;
	/** ָ����ʼ��mib�ļ�·�� ��·���µ�mib�ļ����ᱻ����
	    �����Ϊ�գ������Ĭ��·��/usr/local/share/snmp/mibs�³��Լ���mib�ļ�
        ʹ��mib�������ܵĻ���SNMPv2-MIB.txt��SNMPv2-SMI.txt�Ǳ���ģ������
        ����ָ���ô��·��
    */
	char* szMibFilePath;

    /**
        ָ����־�ļ���·��
        �����Ϊ�գ���Ĭ���ڵ�ǰ·����
    */
    char* szLogPath;
    
    /**
       getbulkӦ�������ص��б���� Ĭ��20
    */
    u32   dwCountPerGetBulk;

    /**
       �ȴ�Ӧ��ĳ�ʱʱ�� ��λ�� Ĭ��2s
    */
    u32   dwRspOutTime;
    
    /** Ӧ����Ϣ���ջص�*/
	RECV_RSP_CALL_BACK  pfRecvRspCb;
    
	/** ��������trap�¼��Ĺ���*/
	BOOL32              bRecvTrapEnable;
	/** ����trap��Ϣ�Ķ˿� ���Ϊ0 ��Ĭ��ʹ��162*/
	u16                 wRecvTrapPort;
	/** trap�¼����ջص�*/
	RECV_TRAP_CALL_BACK pfRecvTrapCb;
    /** trap������ �յ�trap��Ϣ������ϲ�*/
    void*               pvTrapContext;
}TSnmpInitConfig;

/** ����İ�ȫģʽ*/
typedef enum
{
	SNMP_SEC_MOD_NoAuthNoPriv = 0, /** ����Ȩ������*/
	SNMP_SEC_MOD_AuthNoPriv   = 1,/** ֻ��Ȩ������*/
	SNMP_SEC_MOD_AuthPriv 	  = 2/** ��Ȩͬʱ����*/
}ESnmpSecLevel;
/** ��Ȩ�㷨*/
typedef enum
{
    SNMP_AUTH_NA  = 0,
    SNMP_AUTH_MD5 = 1,
    SNMP_AUTH_SHA = 2,
} ESnmpAuthType;
/** �����㷨*/
typedef enum
{
    SNMP_PRIV_NA  = 0,
    SNMP_PRIV_DES = 1,
    SNMP_PRIV_AES = 2,
} ESnmpPrivType;

/** snmp�汾*/
typedef enum
{
	SNMP_VERSION_V1 	 = 1,
	SNMP_VERSION_V2 	 = 2,
	SNMP_VERSION_V3 	 = 3
}ESnmpVersion;

/**snmp v1/v2�汾�û���Ϣ*/
typedef struct tagSnmpV1V2User
{
	char* szCommunity;
}TKdSnmpV1V2User;

/**snmp v3�汾�û���Ϣ*/
typedef struct tagSnmpV3User
{
	ESnmpSecLevel eSecLevel;
	ESnmpAuthType eAuthType;
	ESnmpPrivType ePrivType;
	char* 		  szAuthPass;
    char* 		  szPrivPass;

    char* 		  szUserName;
    /** context snmpEngineID */
    char*         szContextEngineID;
    /** authoritative contextName */
    char*         szContextName;
}TKdSnmpV3User;

typedef struct tagSnmpSession
{
	/** udp:0 tcp:1*/
	char  chAddrType;
	/** �Զ�ip��ַ*/
	char* szPeerName;
	/** �Զ˶˿� ���Ϊ0 ���Ĭ��ʹ��161*/
	u16   wPort;
	/** snmp �汾*/
	ESnmpVersion eSnmpVersion;
	union
	{
		TKdSnmpV1V2User tV12User;
		TKdSnmpV3User   tV3User;
	};
}TKdSnmpSession;

/**Asn�еĶ�������*/
typedef enum
{
    SNMP_OBJ_BUTT    = 0,//��Ч��
    SNMP_OBJ_BOOLEAN = 1,
    SNMP_OBJ_INTEGER = 2,
    SNMP_OBJ_BIT_STR = 3,
    SNMP_OBJ_OCTET_STR = 4,
    SNMP_OBJ_NULL      = 5,
    SNMP_OBJ_OBJECT_ID = 6,
    SNMP_OBJ_IPADDRESS = 7,
    SNMP_OBJ_UNSIGNED  = 8,
    SNMP_OBJ_TIMETICKS = 9,

    SNMP_OBJ_NOSUCHOBJECT   = 128,
    SNMP_OBJ_NOSUCHINSTANCE = 129,
    SNMP_OBJ_ENDOFMIBVIEW   = 130

} ESnmpObjType;

typedef struct tagSnmpObject
{
	ESnmpObjType eObjType;
	/** ��ʽ .1.3.6.1.2.1.2.2.1.4*/
    char* 		 szObjId;
    /** ��������ʱvalue���Բ��� 
     szObjValue���Բ��ǳ���������ַ�����Ҳ���������м���0ֵ���� */
    char* 		 szObjValue;
    /** szObjValue���ֽ��� ������'\0'*/
    u32 		 dwValLen;
} TSnmpObject;

typedef enum
{
    SNMP_MSG_TYPE_BUTT 	    = 0,
	SNMP_MSG_TYPE_GET 	    = 1,
	SNMP_MSG_TYPE_GET_NEXT  = 2,
	SNMP_MSG_TYPE_GET_BULK  = 3,
	SNMP_MSG_TYPE_SET       = 4,
	SNMP_MSG_TYPE_RESPONSE  = 5,
	SNMP_MSG_TYPE_TRAP      = 6,
	SNMP_MSG_TYPE_REPORT    = 7, //�豸��Ӧ�� ����ʧ�� v3����
	SNMP_MSG_TYPE_TIMEOUT   = 8  //�豸��Ӧ��
}ESnmpMsgType;

/* PDU(Packet��ʧ��״ֵ̬) ����TSnmpPacket::nErrStat */
#define PDU_ERR_NOERROR                 (0)     
#define PDU_ERR_TOOBIG	                (1)
#define PDU_ERR_NOSUCHNAME              (2)
#define PDU_ERR_BADVALUE                (3)
#define PDU_ERR_READONLY                (4)
#define PDU_ERR_GENERR	                (5)
#define PDU_ERR_NOACCESS		        (6)
#define PDU_ERR_WRONGTYPE		        (7)
#define PDU_ERR_WRONGLENGTH		        (8)
#define PDU_ERR_WRONGENCODING		    (9)
#define PDU_ERR_WRONGVALUE		        (10)
#define PDU_ERR_NOCREATION		        (11)
#define PDU_ERR_INCONSISTENTVALUE	    (12)
#define PDU_ERR_RESOURCEUNAVAILABLE	    (13)
#define PDU_ERR_COMMITFAILED		    (14)
#define PDU_ERR_UNDOFAILED		        (15)
#define PDU_ERR_AUTHORIZATIONERROR	    (16)
#define PDU_ERR_NOTWRITABLE		        (17)
#define PDU_ERR_INCONSISTENTNAME	    (18)
#define MAX_PDU_ERR	18

/** �������Ӧ����Ϣ*/
typedef struct tagSnmpPacket
{
	/** ��Ϣ����*/
	ESnmpMsgType  eMsgType;
	u32  		  ObjNum;
	TSnmpObject*  ptObjList;

    /** �Զ�ip��ַ*/
	char* szPeerName;
	/** �Զ˶˿� ���Ϊ0 ���Ĭ��ʹ��161*/
	u16   wPort;
	/** snmp �汾*/
	ESnmpVersion eSnmpVersion;
	union
	{
		TKdSnmpV1V2User tV12User;
		TKdSnmpV3User   tV3User;
	};

	/**Ψһ��ʶһ������ �����ڶ�Ӧ��Ӧ��Ļص��д��ظ��ϲ�*/
	u32           dwSn;
	void* 		  pvContext;

    /** ���²���ֻ������Ӧ����Ϣ*/
    /**Error status PDU_ERR_NOERROR û�д��� ����ֵ�д���*/
    s32            nErrStat;
    /**Error index*/
    s32            nErrIndex;
}TSnmpPacket;

/**
  �˽ṹ�еĲ���������tagInitConfig�ṹ�ж�Ӧ�Ĳ���
*/
typedef struct tagCtlPara
{
	/**
       getbulkӦ�������ص���Ŀ���� 
    */
    u32   dwCountPerGetBulk;

    /**
       �ȴ�Ӧ��ĳ�ʱʱ�� ��λ��
    */
    u32   dwRspOutTime;
}TCtlPara;

typedef struct tagSnmpTrap
{
    /** �Զ�ip��ַ*/
	char*         szPeerName;
	/** �Զ˶˿�*/
	u16           wPort;
	char* 		  szCommunity;    
	char* 		  szTrapOid;
	u32  		  ObjNum;
	TSnmpObject*  ptObjList;
    /**Э��ջ��ʼ��ʱ�����pvTrapContext*/
    void* 		  pvContext;
}TSnmpTrap;

#ifndef SNMP_OK
#define SNMP_OK     0
#endif

#ifndef SNMP_TRUE
#define SNMP_TRUE   1
#endif

#ifndef SNMP_FALSE
#define SNMP_FALSE  0
#endif

/**
* snmp����Э��ջ��ʼ��
* @param[in]   ptConfig ���ò���.
* @retval      SNMP_OK �ɹ�  ����ֵ ʧ��
* @note
* @History
*    1. 2017/04/19   1.0   dcg    created
*/
s32 SnmpNmsStartup(TSnmpInitConfig *ptConfig);

/**
* * snmp����Э��ջ�˳� ���������Դ
* @retval      ��
* @note
* @History
*    1. 2017/04/19   1.0   dcg    created
*/
void SnmpNmsCleanup();

/**
* snmp����Э��ջ��ʼ��
* @retval      SNMP_TRUE �Ѿ���ʼ��  SNMP_FALSE δ����ʼ��
* @note
* @History
*    1. 2017/04/19   1.0   dcg    created
*/
s32 SnmpNmsIfInited();

/**
* �첽ģʽ��������
* @param[in]   ptPacket     ����������.
* @retval      SNMP_OK �ɹ�  ����ֵ ʧ��
* @note        ��ر���TSnmpPacket�ṹ�еĳ�Ա�ֶγ���δ��ʼ�������.
               �����TSnmpPacket�����������������memset����.               
* @History
*    1. 2017/04/19   1.0   dcg    created
*/
s32 SnmpPost(TSnmpPacket* ptPacket);

/**
* �첽ģʽ�������������ָ��Ի�����
* @param[in]   ptPacket     ����������.
* @param[in]   ptCtlPara    ��Ա��η��͵Ŀ��Ʋ���
*              ptCtlParaΪ��, ���ܵ�ͬ��SnmpPost()
* @retval      SNMP_OK �ɹ�  ����ֵ ʧ��
* @note        ��ر���TSnmpPacket�ṹ�еĳ�Ա�ֶγ���δ��ʼ�������.
               �����TSnmpPacket�����������������memset����.
* @History
*    1. 2017/04/19   1.0   dcg    created
*/
s32 SnmpPostX(TSnmpPacket* ptPacket, TCtlPara* ptCtlPara);

/**
* �������ֻ������ּ����ʽ���Ҷ�Ӧ��objid, ����
* .ipInDiscards��Ӧ          .1.3.6.1.2.1.4.8
* .ip.ipInDiscards��Ӧ       .1.3.6.1.2.1.4.8
* .iso.org.dod.internet��Ӧ  .1.3.6.1
* .1.3.6.1��Ӧ              .1.3.6.1
* .1.3.6.dod.internet��Ӧ   .1.3.6.1
* iso.org.6.1��Ӧ           .1.3.6.1  
* @param[in]   szDotName ��������
* @param[in]   nSize   ����Ļ���������ֽ���
* @param[out]  szObjId �����������ʽ��oid
* @retval      -1 buffer�ռ�̫С  0 δ�ҵ�  ����0 �ɹ�
* @note        DotName��ʽ������Ӣ����ĸ�������֣�Ӣ����ĸ�����ֻ���
* @History
*    1. 2017/04/19   1.0   dcg    created
*/
s32 SnmpDotName2ObjId(char* szDotName, s32 nSize, char* szObjId);


/**
* �������ֲ��Ҷ�Ӧ��objid, ����
* ifTable��Ӧ����       .1.3.6.1.2.1.2.2
* interfaces.ifTable��Ӧ.1.3.6.1.2.1.2.2
  szLableֻ�����ַ�����ʽinterfaces.ifTable ���ָ�ʽ�Ĳ鲻�����
* @param[in]   szLable �������� ��Ӣ����ĸ��.���
* @param[in]   nSize   ����Ļ���������ֽ���
* @param[out]  szObjId �����������ʽ��oid
* @retval      -1 buffer�ռ�̫С  0 δ�ҵ�  ����0 �ɹ�
* @note        �˺����Ĺ���SnmpDotName2ObjId�Ĺ�����ͬ������Ч�ʸ���Щ
*              ��β�����.��ͷ
* @History
*    1. 2017/04/19   1.0   dcg    created
*/
s32 SnmpLabel2ObjId(char* szLable, s32 nSize, char* szObjId);

/**
* ���ּ����ʽ�õ���Ӧ��������������ʽ,��
* .1.3.6.1.2.1.4.8��Ӧ���.iso.org.dod.internet.mgmt.mib-2.ip.ipInDiscards
* @param[in]   szObjId ������ʽ��oid ��.1.3.6.1.2.1.4.8
* @param[in]   nSize   ����Ļ���������ֽ���
* @param[out]  szDotLabel  ����Ľ��
* @retval      -1 buffer�ռ�̫С  0 δ�ҵ�  ����0 �ɹ�
* @note        DotLabel��ʽ������.�ָ��Ĵ�Ӣ����ĸ
* @History
*    1. 2017/04/19   1.0   dcg    created
*/
s32 SnmpObjId2DotLabel(char* szObjId, s32 nSize, char* szDotLabel);

/**
* ���ּ����ʽ�õ���Ӧ�����ֱ�ǩ,��.1.3.6.1.2.1.4.8�õ�ipInDiscards
* @param[in]   szObjId ������ʽ��oid ��.1.3.6.1.2.1.4.8
* @param[in]   nSize   ����Ļ���������ֽ���
* @param[out]  szLabel ����Ľ��
* @retval      -1 buffer�ռ�̫С  0 δ�ҵ�  ����0 �ɹ�
* @note        Label��ʽ��ӦӢ������
* @History
*    1. 2017/04/19   1.0   dcg    created
*/
s32 SnmpObjId2Label(char* szObjId, s32 nSize, char* szLabel);

/**
* ����ָ����MIB�ļ�
* @param[in]   szMibPath mib�ļ�·�� ����ļ���:�ָ�
               ��/usr/local/test.mib:./kedacomvideo.mib
* @retval      SNMP_OK �ɹ� ���� ʧ��
* @note        ���A�ļ�������B�ļ�������Ҫ��Bд��ǰ��Aд�ں���ȷ��B�ļ��ȼ���
* @History
*    1. 2017/04/19   1.0   dcg    created
*/
s32 SnmpLoadMibFile(char* szMibPath);

/**
* ��ȡ�ڵ������
* @param[in]   szObjId ������ʽ��oid ��.1.3.6.1.4.1.24398.2.1
* @retval      �ýڵ������ ����SNMP_OBJ_BUTT˵��oid�����ڻ��߸ýڵ�����δ֪
* @note   
* @History
*    1. 2017/04/19   1.0   dcg    created
*/
ESnmpObjType SnmpGetNodeType(char* szObjId);

/**
* ��ȡoid�ڵ�Ķ��ӽڵ�
* @param[in]   szObjId ������ʽ��oid ��.1.3.6.1.4.1.24398.2.1
* @param[out]  *pnCount ��ȡ�ĺ��Ӹ��� ����:
*               -1 ����; ==0 �ýڵ�ΪҶ�ӽڵ㣬���Ӹ���Ϊ0; >0 ���غ��Ӹ���
* @retval      �����ǵ�id�б�   
* @note   ���磬����ڵ�.1.3.6.1.4.1.24398.2.1����������.1��.2����*pnCountΪ2 
* �����ַ����б�".1.3.6.1.4.1.24398.2.1.1", ".1.3.6.1.4.1.24398.2.1.2"
* @History
*    1. 2017/04/19   1.0   dcg    created
*/
char** SnmpGetChildList(char* szObjId, s32* pnCount);

/**
* �ͷ��б�
* @param[in]   pszChildList ����SnmpGetChildList()�ķ��ؽṹ
* @param[in]   nListNum       �ӽڵ����
* @retval      ��
* @note   
* @History
*    1. 2017/04/19   1.0   dcg    created
*/
void SnmpFreeChildList(char** pszChildList, s32 nCount);

typedef struct tagOidNode
{
    char*         szObjId;    //�ڵ������oid ��������ʽ��
    char*         szLable;    //�ڵ�����
    ESnmpObjType  eType;      //�ڵ����������
    BOOL32        bIsTable;   //�ýڵ������Ƿ���"Table"��β
    BOOL32        bIsEntry;   //�ýڵ������Ƿ���"Entry"��β
    s32           nChildCount;//�ӽڵ�ĸ���
    struct tagOidNode* ptChildList;//�ӽڵ��б�   
}TOidNode;

/**
* ��ȡ�ڵ�������Լ����ӽڵ��б� 
* ���磬��������ڵ�.1.3.6.1.4.1.24398.2.1����������.1��.2�� 
* �򷵻صĶ����ǵ�oidΪ".1.3.6.1.4.1.24398.2.1.1", ".1.3.6.1.4.1.24398.2.1.2"
* @param[in]   szObjId ������ʽ��oid ��.1.3.6.1.4.1.24398.2.1
* @retval      �ڵ��б�  ���� NULL��ʾszObjId�ڵ㲻����  
* @note        ����ȡ���ڵ�Ķ��ӽڵ㣬�ǵݹ�
*              ����ֵ��ΪNULLʱ �����SnmpFreeNodeDetail()�ͷŶ�Ӧ���ڴ�
* @History
*    1. 2017/04/19   1.0   dcg    created
*/
TOidNode* SnmpGetNodeDetail(char* szObjId);

/**
* �ͷŽڵ���ڴ�
* @param[in]   ptNode SnmpGetNodeDetail()���صĽ��
* @retval      �� 
* @note        ��SnmpGetNodeDetail()���ʹ��
* @History
*    1. 2017/04/19   1.0   dcg    created
*/
void SnmpFreeNodeDetail(TOidNode* ptNode);

#ifdef __cplusplus
}
#endif

#endif
