/**
* @file     snmp_nms.h
* @brief    网络管理系统实现，snmp协议网管侧相关功能 支持发送get get-next get-bulk 支持接收trap
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

/** 接收应答消息的回调*/
typedef void (*RECV_RSP_CALL_BACK)(struct tagSnmpPacket*);
/** 接收trap事件的回调*/
typedef void (*RECV_TRAP_CALL_BACK)(struct tagSnmpTrap*);

typedef struct tagInitConfig
{
	char* szAppName;
	/** 指定初始的mib文件路径 该路径下的mib文件都会被加载
	    如果设为空，则会在默认路径/usr/local/share/snmp/mibs下尝试加载mib文件
        使用mib解析功能的话，SNMPv2-MIB.txt和SNMPv2-SMI.txt是必须的，最好在
        这里指定好存放路径
    */
	char* szMibFilePath;

    /**
        指定日志文件的路径
        如果设为空，则默认在当前路径下
    */
    char* szLogPath;
    
    /**
       getbulk应答允许返回的列表个数 默认20
    */
    u32   dwCountPerGetBulk;

    /**
       等待应答的超时时间 单位秒 默认2s
    */
    u32   dwRspOutTime;
    
    /** 应答消息接收回调*/
	RECV_RSP_CALL_BACK  pfRecvRspCb;
    
	/** 开启接收trap事件的功能*/
	BOOL32              bRecvTrapEnable;
	/** 接收trap消息的端口 如果为0 则默认使用162*/
	u16                 wRecvTrapPort;
	/** trap事件接收回调*/
	RECV_TRAP_CALL_BACK pfRecvTrapCb;
    /** trap上下文 收到trap消息后带给上层*/
    void*               pvTrapContext;
}TSnmpInitConfig;

/** 允许的安全模式*/
typedef enum
{
	SNMP_SEC_MOD_NoAuthNoPriv = 0, /** 不鉴权不加密*/
	SNMP_SEC_MOD_AuthNoPriv   = 1,/** 只鉴权不加密*/
	SNMP_SEC_MOD_AuthPriv 	  = 2/** 鉴权同时加密*/
}ESnmpSecLevel;
/** 鉴权算法*/
typedef enum
{
    SNMP_AUTH_NA  = 0,
    SNMP_AUTH_MD5 = 1,
    SNMP_AUTH_SHA = 2,
} ESnmpAuthType;
/** 加密算法*/
typedef enum
{
    SNMP_PRIV_NA  = 0,
    SNMP_PRIV_DES = 1,
    SNMP_PRIV_AES = 2,
} ESnmpPrivType;

/** snmp版本*/
typedef enum
{
	SNMP_VERSION_V1 	 = 1,
	SNMP_VERSION_V2 	 = 2,
	SNMP_VERSION_V3 	 = 3
}ESnmpVersion;

/**snmp v1/v2版本用户信息*/
typedef struct tagSnmpV1V2User
{
	char* szCommunity;
}TKdSnmpV1V2User;

/**snmp v3版本用户信息*/
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
	/** 对端ip地址*/
	char* szPeerName;
	/** 对端端口 如果为0 则会默认使用161*/
	u16   wPort;
	/** snmp 版本*/
	ESnmpVersion eSnmpVersion;
	union
	{
		TKdSnmpV1V2User tV12User;
		TKdSnmpV3User   tV3User;
	};
}TKdSnmpSession;

/**Asn中的对象类型*/
typedef enum
{
    SNMP_OBJ_BUTT    = 0,//无效的
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
	/** 格式 .1.3.6.1.2.1.2.2.1.4*/
    char* 		 szObjId;
    /** 发送请求时value可以不填 
     szObjValue可以不是常规意义的字符串，也就是允许中间有0值出现 */
    char* 		 szObjValue;
    /** szObjValue的字节数 不包括'\0'*/
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
	SNMP_MSG_TYPE_REPORT    = 7, //设备有应答 报告失败 v3命令
	SNMP_MSG_TYPE_TIMEOUT   = 8  //设备无应答
}ESnmpMsgType;

/* PDU(Packet的失败状态值) 用于TSnmpPacket::nErrStat */
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

/** 请求或者应答消息*/
typedef struct tagSnmpPacket
{
	/** 消息类型*/
	ESnmpMsgType  eMsgType;
	u32  		  ObjNum;
	TSnmpObject*  ptObjList;

    /** 对端ip地址*/
	char* szPeerName;
	/** 对端端口 如果为0 则会默认使用161*/
	u16   wPort;
	/** snmp 版本*/
	ESnmpVersion eSnmpVersion;
	union
	{
		TKdSnmpV1V2User tV12User;
		TKdSnmpV3User   tV3User;
	};

	/**唯一标识一个请求 用于在对应的应答的回调中带回给上层*/
	u32           dwSn;
	void* 		  pvContext;

    /** 以下部分只适用于应答消息*/
    /**Error status PDU_ERR_NOERROR 没有错误 其他值有错误*/
    s32            nErrStat;
    /**Error index*/
    s32            nErrIndex;
}TSnmpPacket;

/**
  此结构中的参数优先于tagInitConfig结构中对应的参数
*/
typedef struct tagCtlPara
{
	/**
       getbulk应答允许返回的项目个数 
    */
    u32   dwCountPerGetBulk;

    /**
       等待应答的超时时间 单位秒
    */
    u32   dwRspOutTime;
}TCtlPara;

typedef struct tagSnmpTrap
{
    /** 对端ip地址*/
	char*         szPeerName;
	/** 对端端口*/
	u16           wPort;
	char* 		  szCommunity;    
	char* 		  szTrapOid;
	u32  		  ObjNum;
	TSnmpObject*  ptObjList;
    /**协议栈初始化时填入的pvTrapContext*/
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
* snmp网管协议栈初始化
* @param[in]   ptConfig 配置参数.
* @retval      SNMP_OK 成功  其他值 失败
* @note
* @History
*    1. 2017/04/19   1.0   dcg    created
*/
s32 SnmpNmsStartup(TSnmpInitConfig *ptConfig);

/**
* * snmp网管协议栈退出 清理相关资源
* @retval      无
* @note
* @History
*    1. 2017/04/19   1.0   dcg    created
*/
void SnmpNmsCleanup();

/**
* snmp网管协议栈初始化
* @retval      SNMP_TRUE 已经初始化  SNMP_FALSE 未经初始化
* @note
* @History
*    1. 2017/04/19   1.0   dcg    created
*/
s32 SnmpNmsIfInited();

/**
* 异步模式发送请求
* @param[in]   ptPacket     待发送数据.
* @retval      SNMP_OK 成功  其他值 失败
* @note        务必避免TSnmpPacket结构中的成员字段出现未初始化的情况.
               建议对TSnmpPacket做好清零操作，例如memset清零.               
* @History
*    1. 2017/04/19   1.0   dcg    created
*/
s32 SnmpPost(TSnmpPacket* ptPacket);

/**
* 异步模式发送请求，允许部分个性化配置
* @param[in]   ptPacket     待发送数据.
* @param[in]   ptCtlPara    针对本次发送的控制参数
*              ptCtlPara为空, 则功能等同于SnmpPost()
* @retval      SNMP_OK 成功  其他值 失败
* @note        务必避免TSnmpPacket结构中的成员字段出现未初始化的情况.
               建议对TSnmpPacket做好清零操作，例如memset清零.
* @History
*    1. 2017/04/19   1.0   dcg    created
*/
s32 SnmpPostX(TSnmpPacket* ptPacket, TCtlPara* ptCtlPara);

/**
* 根据名字或者数字间隔形式查找对应的objid, 例如
* .ipInDiscards对应          .1.3.6.1.2.1.4.8
* .ip.ipInDiscards对应       .1.3.6.1.2.1.4.8
* .iso.org.dod.internet对应  .1.3.6.1
* .1.3.6.1对应              .1.3.6.1
* .1.3.6.dod.internet对应   .1.3.6.1
* iso.org.6.1对应           .1.3.6.1  
* @param[in]   szDotName 输入名字
* @param[in]   nSize   输出的缓冲区最大字节数
* @param[out]  szObjId 输出的数字形式的oid
* @retval      -1 buffer空间太小  0 未找到  大于0 成功
* @note        DotName形式包括纯英文字母，纯数字，英文字母和数字混用
* @History
*    1. 2017/04/19   1.0   dcg    created
*/
s32 SnmpDotName2ObjId(char* szDotName, s32 nSize, char* szObjId);


/**
* 根据名字查找对应的objid, 例如
* ifTable对应的是       .1.3.6.1.2.1.2.2
* interfaces.ifTable对应.1.3.6.1.2.1.2.2
  szLable只能是字符串格式interfaces.ifTable 数字格式的查不到结果
* @param[in]   szLable 输入名字 纯英文字母和.组成
* @param[in]   nSize   输出的缓冲区最大字节数
* @param[out]  szObjId 输出的数字形式的oid
* @retval      -1 buffer空间太小  0 未找到  大于0 成功
* @note        此函数的功能SnmpDotName2ObjId的功能类同，但是效率更高些
*              入参不能以.开头
* @History
*    1. 2017/04/19   1.0   dcg    created
*/
s32 SnmpLabel2ObjId(char* szLable, s32 nSize, char* szObjId);

/**
* 数字间隔形式得到对应的完整的名字形式,如
* .1.3.6.1.2.1.4.8对应结果.iso.org.dod.internet.mgmt.mib-2.ip.ipInDiscards
* @param[in]   szObjId 数字形式的oid 如.1.3.6.1.2.1.4.8
* @param[in]   nSize   输出的缓冲区最大字节数
* @param[out]  szDotLabel  输出的结果
* @retval      -1 buffer空间太小  0 未找到  大于0 成功
* @note        DotLabel形式仅限于.分隔的纯英文字母
* @History
*    1. 2017/04/19   1.0   dcg    created
*/
s32 SnmpObjId2DotLabel(char* szObjId, s32 nSize, char* szDotLabel);

/**
* 数字间隔形式得到对应的名字标签,如.1.3.6.1.2.1.4.8得到ipInDiscards
* @param[in]   szObjId 数字形式的oid 如.1.3.6.1.2.1.4.8
* @param[in]   nSize   输出的缓冲区最大字节数
* @param[out]  szLabel 输出的结果
* @retval      -1 buffer空间太小  0 未找到  大于0 成功
* @note        Label形式对应英文名字
* @History
*    1. 2017/04/19   1.0   dcg    created
*/
s32 SnmpObjId2Label(char* szObjId, s32 nSize, char* szLabel);

/**
* 加载指定的MIB文件
* @param[in]   szMibPath mib文件路径 多个文件用:分隔
               如/usr/local/test.mib:./kedacomvideo.mib
* @retval      SNMP_OK 成功 其他 失败
* @note        如果A文件依赖于B文件，则需要将B写在前，A写在后，以确保B文件先加载
* @History
*    1. 2017/04/19   1.0   dcg    created
*/
s32 SnmpLoadMibFile(char* szMibPath);

/**
* 获取节点的类型
* @param[in]   szObjId 数字形式的oid 如.1.3.6.1.4.1.24398.2.1
* @retval      该节点的类型 返回SNMP_OBJ_BUTT说明oid不存在或者该节点类型未知
* @note   
* @History
*    1. 2017/04/19   1.0   dcg    created
*/
ESnmpObjType SnmpGetNodeType(char* szObjId);

/**
* 获取oid节点的儿子节点
* @param[in]   szObjId 数字形式的oid 如.1.3.6.1.4.1.24398.2.1
* @param[out]  *pnCount 获取的孩子个数 其中:
*               -1 报错; ==0 该节点为叶子节点，孩子个数为0; >0 返回孩子个数
* @retval      孩子们的id列表   
* @note   例如，如果节点.1.3.6.1.4.1.24398.2.1有两个儿子.1和.2，则*pnCount为2 
* 返回字符串列表".1.3.6.1.4.1.24398.2.1.1", ".1.3.6.1.4.1.24398.2.1.2"
* @History
*    1. 2017/04/19   1.0   dcg    created
*/
char** SnmpGetChildList(char* szObjId, s32* pnCount);

/**
* 释放列表
* @param[in]   pszChildList 调用SnmpGetChildList()的返回结构
* @param[in]   nListNum       子节点个数
* @retval      无
* @note   
* @History
*    1. 2017/04/19   1.0   dcg    created
*/
void SnmpFreeChildList(char** pszChildList, s32 nCount);

typedef struct tagOidNode
{
    char*         szObjId;    //节点的完整oid 纯数字形式的
    char*         szLable;    //节点名字
    ESnmpObjType  eType;      //节点的数据类型
    BOOL32        bIsTable;   //该节点名字是否已"Table"结尾
    BOOL32        bIsEntry;   //该节点名字是否已"Entry"结尾
    s32           nChildCount;//子节点的个数
    struct tagOidNode* ptChildList;//子节点列表   
}TOidNode;

/**
* 获取节点的属性以及儿子节点列表 
* 例如，假设输入节点.1.3.6.1.4.1.24398.2.1有两个儿子.1和.2， 
* 则返回的儿子们的oid为".1.3.6.1.4.1.24398.2.1.1", ".1.3.6.1.4.1.24398.2.1.2"
* @param[in]   szObjId 数字形式的oid 如.1.3.6.1.4.1.24398.2.1
* @retval      节点列表  其中 NULL表示szObjId节点不存在  
* @note        仅获取父节点的儿子节点，非递归
*              返回值不为NULL时 须调用SnmpFreeNodeDetail()释放对应的内存
* @History
*    1. 2017/04/19   1.0   dcg    created
*/
TOidNode* SnmpGetNodeDetail(char* szObjId);

/**
* 释放节点的内存
* @param[in]   ptNode SnmpGetNodeDetail()返回的结果
* @retval      无 
* @note        与SnmpGetNodeDetail()配对使用
* @History
*    1. 2017/04/19   1.0   dcg    created
*/
void SnmpFreeNodeDetail(TOidNode* ptNode);

#ifdef __cplusplus
}
#endif

#endif
