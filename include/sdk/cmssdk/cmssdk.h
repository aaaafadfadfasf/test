/*****************************************************************************
   模块名      : CMSSDK
   文件名      : cmssdk.h
   相关文件    : 
   文件实现功能: 监控平台2.0接入开发sdk
   作者        : fanxg
   版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2013/09/03  1.0         fanxg        创建
******************************************************************************/

#ifndef __CMS_SDK_H__
#define __CMS_SDK_H__

#include <stdlib.h>
#include <string.h>

#ifdef _MSC_VER
#define CMS_API extern "C"			__declspec(dllexport)
#else
#define CMS_API extern "C"
#endif	// _MSC_VER

#ifdef SetPort
#undef SetPort
#endif

class CCmsMsg;
struct TCmsSdkConf;


/*====================================================================
函数名      : CmsSdk_Init
功能        : 初始化cmssdk
算法实现    : 同步接口，全局只能调用一次
输入参数说明:
             tCfg: CmsSdk初始化参数
返回值说明  : 
             成功:CMS_SDK_SUCCESS，
             失败:错误码
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2013/09/03  0.1         fanxg
====================================================================*/
CMS_API int CmsSdk_Init(const TCmsSdkConf& tCfg);
 

/*====================================================================
函数名      : CmsSdk_Quit
功能        : 退出cmssdk
算法实现    : 同步接口，全局只能调用一次
输入参数说明:
             
返回值说明  : 
             成功:CMS_SDK_SUCCESS，
             失败:错误码
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2013/09/03  0.1         fanxg
====================================================================*/
CMS_API int CmsSdk_Quit();


/*====================================================================
函数名      : CmsSdk_SendMsgToPlat
功能        : G网关向平台发送消息，包括主动的请求、通知消息，被动的应答消息
算法实现    : 异步接口
输入参数说明:
             cMsg：将消息头和消息体包装到CCmsMsg中
             
返回值说明  : 
             成功:CMS_SDK_SUCCESS，
             失败:错误码
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2013/09/03  0.1         fanxg
====================================================================*/
CMS_API int CmsSdk_SendMsgToPlat(const CCmsMsg& cMsg);

/*====================================================================
函数名      : PCmsEventCallBack
功能        : 平台向G网关发送回调消息，包括主动的请求、通知消息，被动的应答消息
算法实现    : 该回调函数应该设计成异步接口，不能阻塞底层的调用线程
引用全局变量:
输入参数说明:
              cMsg：将消息头和消息体包装到CCmsMsg中
返回值说明  : 
             成功:CMS_SDK_SUCCESS，
             失败:错误码
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2013/09/03  0.1         fanxg
====================================================================*/
typedef int (*PCmsEventCallBack)(const CCmsMsg& cMsg);


//动态库接口中不使用stl, 这里实现一个简单的string类
class CCmsString
{
public:
    CCmsString(const char* pStr=0) 
    { 
        CopyStrToSelf(pStr); 
    }

    CCmsString(const CCmsString & other) 
    { 
        CopyStrToSelf(other.m_pData); 
    }

    ~CCmsString() 
    { 
        FreeStrBuffer(m_pData);
    }

    bool operator==(const CCmsString& other) const 
    { 
        return (strcmp(m_pData, other.m_pData) == 0); 
    }

    bool operator==(const char* pStr) const 
    { 
        return (pStr == 0) ? false : (strcmp(m_pData, pStr) == 0); 
    }

    CCmsString& operator=(const CCmsString& other) 
    { 
        return ((this != &other) ? AssignStrToSelf(other.m_pData) : *this); 
    }

    CCmsString& operator=(const char* pStr) 
    { 
        return AssignStrToSelf(pStr); 
    }

    CCmsString& operator+=(const CCmsString& other) 
    { 
        return AddStrToSelf(other.m_pData); 
    }

    CCmsString& operator+=(const char* pStr)
    { 
        return AddStrToSelf(pStr); 
    }

    CCmsString operator+(const CCmsString &other) const 
    { 
        CCmsString newStr; 
        return((newStr += *this) += other); 
    }

    size_t size() const 
    { 
        return strlen(m_pData); 
    }

    const char* c_str() const
    { 
        return m_pData; 
    }

    void clear()
    {
        AssignStrToSelf("");
    }

    bool  empty() const
    {
        return ((m_pData == 0) || operator==(""));
    }

private:
    void CopyStrToSelf(const char* pStr)
    {
        int nLen = (pStr==0) ? 0 : strlen(pStr);
        pStr = (pStr==0) ? "" : pStr;
        m_pData = AllocStrBuffer(nLen+1);
        strcpy(m_pData, pStr);
    }

    CCmsString& AssignStrToSelf(const char* pStr)
    {
        FreeStrBuffer(m_pData);
        CopyStrToSelf(pStr);
        return *this;
    }

    CCmsString& AddStrToSelf(const char* pStr)
    {
        if (pStr)
        {
            char* pNewBuf = AllocStrBuffer(strlen(m_pData) + strlen(pStr) + 1);
            strcpy(pNewBuf, m_pData);
            strcat(pNewBuf, pStr);
            FreeStrBuffer(m_pData);
            m_pData = pNewBuf;
        }
        return *this;
    }

    char* AllocStrBuffer(size_t nSize)
    {
        return (char*)(malloc(nSize));
    }

    void FreeStrBuffer(void* pStr)
    {
        free(pStr);
    }

private:
    char* m_pData;
};

class CCmsNetAddr
{
private:
    CCmsString ip;            
    int        port;                  

public:
    CCmsNetAddr() 
    {
        Clear();
    }

    void Clear()
    {
        ip.clear();
        port = 0;
    }
public:
    void SetIp(const CCmsString& strIp)
    {
        ip = strIp;
    }
    CCmsString& GetIp()
    {
        return ip;
    }
    const CCmsString& GetIp() const
    {
        return ip;
    }

    void SetPort(int nPort)
    {
        port = nPort;
    }
    int GetPort() const
    {
        return port;
    }
};

// 网关类型
typedef CCmsString TGateWayType;
#define CMS_DEV_TYPE_GBS    "gbs"
#define CMS_DEV_TYPE_CUI1   "cui1"
#define CMS_DEV_TYPE_GXX    "gxx"

//设备变更通知类型
#define CmsEmpty						0x0000		// 不处理任何变更通知
#define CmsGeneralDev					0x0002      // 通用设备(不包括特殊处理的设备类型，比如电视墙设备)
#define CmsTvwallDev					0x0004      // 电视墙设备

#define CmsDevCapIndexName				0x0010		// 设备能力索引名称
#define CmsDevGrp						0x0020      // 设备分组
#define CmsDevGrp2Dev					0x0040      // 设备分组设备划归
#define CmsDomainRelation				0x0080		// 域关系
#define CmsCUserInfo				    0x0100		// 用户信息
#define CmsIgnoreGbChildDomainDevChg    0x100000  // 忽略国标子域的设备变更0x0001 0000  

typedef unsigned char TLogLev;
#define LOGLEV_NOLOG_LEV                0
#define LOGLEV_ERROR_LEV                1
#define LOGLEV_WARNING_LEV              2
#define LOGLEV_CRITICAL_LEV             3
#define LOGLEV_EVENT_LEV                4
#define LOGLEV_PROGRESS_LEV             5
#define LOGLEV_TIMER_LEV                6
#define LOGLEV_ALL_LEV                  7
#define LOGLEV_TRIVIAL_LEV              255


//这个结构体中的字段需要从G网关的配置文件中读取
struct TCmsSdkConf
{
    TCmsSdkConf()
    {
        ptCmsEventCB = NULL;
        strLogDir = "./";
        bIsSupportNA = false;
		nInterestChangeNtf = CmsEmpty;

		nMaxRegTime  = 1;
		nMaxMediaTransNum = 2048;
		nMaxSubsDevNum    = 4096;
        nMaxNtfPerSs      = 5;
		nMaxTransNum      = 4096;
		bDisableCheckSum  = false;
		bSpeedPriority    = true;
        bIsLanFileExsit   = false;
		bNeedGrpGbid      = false;
        bIsSupportRedis   = false;

		tRunlogLevel = LOGLEV_NOLOG_LEV;	//默认关闭运行日志
		nRunlogFileSize_KB = 1024;			//默认运行日志大小为1M
		nRunlogFileNum     = 10;			//默认开启10个运行日志
		nErrlogFileSize_KB = 1024;			//默认错误日志大小为1M
		nErrlogFileNum     = 5;				//默认开启5个错误日志
    }
    TGateWayType strGateWayType;    //网关设备类型
    CCmsString strGateWayUUID;      //网关设备的唯一标识，不超过32个字节
    CCmsNetAddr tGateWayAddr;       //网关地址
    CCmsString strPlatDomainName;   //监控平台域名
    CCmsNetAddr tPlatAddr;          //监控平台地址
    CCmsString strLogDir;           //cmssdk日志目录
    bool bIsSupportNA;              //是否支持NA
	int nInterestChangeNtf;			//需要处理的设备变更通知类型，该值为CmsGeneralDev、CmsTvwallDev等及其按位或的结果，如CmsGeneralDev | CmsTvwallDev
	bool bNeedGrpGbid;				//是否需要国标编码

	int nMaxRegTime;                //向注册服务器注册次数，无特殊需求，默认为1
	int nMaxMediaTransNum;          //最大媒体转发路数，即最大浏览/放像/下载路数，默认2048
	int nMaxSubsDevNum;             //最大订阅设备信息次数，默认4096
    int nMaxNtfPerSs;               //订阅对话内最大ntf队列大小，默认5
	int nMaxTransNum;               //最大并发事务数，默认4096
	bool bDisableCheckSum;          //禁用校验和，默认为false，路由器有问题时设置为true
    bool bSpeedPriority;            //速度优先，默认true(开启)
    bool bIsLanFileExsit;
    bool bIsSupportRedis;

	TLogLev tRunlogLevel;			//运行日志的级别
	int nRunlogFileSize_KB;		//单个运行日志的文件大小，单位为KB
	int nRunlogFileNum;			//运行日志的数量（达到上限后覆盖最早的日志）
	int nErrlogFileSize_KB;		//单个错误日志的文件大小，单位为KB
	int nErrlogFileNum;			//错误日志的数量（达到上限后覆盖最早的日志）

    PCmsEventCallBack ptCmsEventCB; //事件回调
};

#define  CMS_SDK_INVALID_TASK_NO      (0)
#define  CMS_SDK_INVALID_EVENT        (0)

struct TCmsMsgHead
{
public:
    TCmsMsgHead()
    {
        Clear();
    }

    ~TCmsMsgHead()
    {
        Clear();
    }

    void Clear()
    {      
        m_wEventId = CMS_SDK_INVALID_EVENT;
        m_dwSdkTask = CMS_SDK_INVALID_TASK_NO;
        m_dwGwTask = CMS_SDK_INVALID_TASK_NO;
    }

public:
    
    unsigned short m_wEventId;    //事件id
    unsigned long  m_dwGwTask;    //gw的taskNO
    unsigned long  m_dwSdkTask;   //sdk的taskNO
};


/*
  CCmsMsg注意事项：
  1. 在不同线程间传递该消息时使用指针，并且整个消息是动态内存分配。
  2. new/delete在各自的模块代码中进行。
*/
class CCmsMsg
{
public:
    CCmsMsg() {}

public:

    TCmsMsgHead& GetMsgHead()
    {
        return m_tMsgHeader;
    }
    const TCmsMsgHead& GetMsgHead() const
    {
        return m_tMsgHeader;
    }
    void SetMsgHead(const TCmsMsgHead& tMsgHeader)
    {
        m_tMsgHeader = tMsgHeader;
    }

    CCmsString& GetMsgBody()
    {
        return m_strMsgBody;
    }
    const CCmsString& GetMsgBody() const
    {
        return m_strMsgBody;
    }
    void SetMsgBody(const CCmsString& strMsgBody)
    {
        m_strMsgBody = strMsgBody;
    }
    void ClearMsgBody(void)
    {
        m_strMsgBody.clear();
    }
    size_t GetMsgBodySize() const
    {
        return m_strMsgBody.size();
    }

    unsigned short GetEventID() const
    {
        return m_tMsgHeader.m_wEventId;
    }
    void SetEventID(unsigned short wEventID)
    {
        m_tMsgHeader.m_wEventId = wEventID;
    }

    unsigned long GetSdkTask() const
    {
        return m_tMsgHeader.m_dwSdkTask;
    }
    void SetSdkTask(unsigned long dwTaskNO)
    {
        m_tMsgHeader.m_dwSdkTask = dwTaskNO;
    }

    unsigned long GetGwTask() const
    {
        return m_tMsgHeader.m_dwGwTask;
    }
    void SetGwTask(unsigned long dwTaskNO)
    {
        m_tMsgHeader.m_dwGwTask = dwTaskNO;
    }

    template<class CMsg>
    void GetMsgBody(CMsg& rMsg) const
    {
        rMsg.FromXml(m_strMsgBody.c_str());
    }

protected:
    TCmsMsgHead m_tMsgHeader;
    CCmsString m_strMsgBody;
};


#endif // __CMS_SDK_H__
