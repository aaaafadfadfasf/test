#ifndef _NE_AGENT_H_
#define _NE_AGENT_H_

#include "daustruct.h"

#ifdef _MSC_VER
#ifdef __cplusplus
#define NE_API               extern "C" __declspec(dllexport)
#else
#define NE_API               __declspec(dllexport)
#endif //__cplusplus
#else
#ifdef __cplusplus
#define NE_API               extern "C"
#else
#define NE_API
#endif //__cplusplus
#endif

enum EM_NE_ERROR_CODE
{
    ERR_OK                  = 0,
    ERR_INNER_ERROR         = NE_ERROR_BEGIN,   // 设备内部错误
    ERR_PARAM_EXCEPTION,                        // 参数异常
    ERR_MEM_EXCEPTION,                          // 设备内存异常
    ERR_GET_DEVICE_INFO_FAIL,                   // 获取设备信息失败
    ERR_FILE_SIZE_EXCEPTION,                    // 文件大小异常
    ERR_FILE_FORMAT_EXCEPTION,                  // 文件格式异常
    ERR_CONSTRUCT_CFGFILE_FAIL,                 // 构造配置文件失败
};

enum EM_TOPO_TYPE
{
    TOPO_SYNC = 0,	
    TOPO_ADD,		// 新增设备
    TOPO_DEL,		// 删除
    TOPO_MOD		// 设备修改
};

//与网管连上回调函数
typedef void (*CBConnectedNtf)();

//注册响应回调函数
typedef void (*CBRegisterRsp)(int dwSerialNO, TNEReportRegisterRsp tRegisterRsp);

//Get回调函数
typedef void (*CBGetValue)(const char* pszID, int dwSerialNO, const char* pszInfo, int dwInfoLen);

//set回调函数
typedef void (*CBSetValue)(const char* pszID, int dwSerialNO, const char* pszInfo, int dwInfoLen);

//获取topo回调函数
typedef void (*CBGetTopo)(int dwSerialNO, const char* pszNEID);

//与网管断链回调函数
typedef void (*CBDisconnectNtf)();

//网管主动断链NE回调函数
typedef void (*CBDisconnectNENtf)(int dwSerialNO, const char* pszInfo, int dwInfoLen);

//配置filebeat回调函数
typedef void (*CBConfigFileBeatReq)(int dwSerialNO, const char* pszInfo, int dwInfoLen);

//获取升级时参数请求回调
typedef void (*CBGetUpgradeReq)(const char* pszID, int dwSerialNO, u32 dwFileType);

//设置升级参数请求回调
typedef void (*CBSetUpgradeReq)(const char* pszID, int dwSerialNO, TNEUpdateInfo tUpdateInfo);

//导入文件请求回调
typedef void (*CBImportFileReq)(const char* pszID, int dwSerialNO, TNEUpdateInfo tReq);

//导出文件请求回调
typedef void (*CBExportFileReq)(const char* pszID, int dwSerialNO, TNEUpdateInfo tReq);

//重启设备请求回调
typedef void (*CBRebootReq)(const char* pszID);

//获取services回调函数
typedef void (*CBGetServices)(int dwSerialNO, const char* pszNEID);

struct TMainTopo
{
public:
    TMainTopo()
    {
        m_szNEID = "";
        m_szNEName = "";
        m_szDesc = "";
        m_szMainType = "";
        m_szSubType = "";
        m_szManuFactor = "";
    }
public:
    std::string          m_szNEID;
    std::string          m_szNEName;
    std::string          m_szDesc;
    std::string          m_szMainType;
    std::string          m_szSubType;
    std::string          m_szManuFactor;
};

struct TInitParam
{
public:
    TInitParam()
    {
        pfConnectedCB = NULL;
        pfRegisterRspCB = NULL;
        pfGetValueCB = NULL;
        pfSetValueCB = NULL;
        pfGetTopoCB = NULL;
        pfDisconnCB = NULL;
        pfDisconnNECB = NULL;
        pfConfigFileBeatCB = NULL;
        pfGetUpgradeCB = NULL;     //获取升级信息回调
        pfSetUpgradeCB = NULL;     //设置升级信息回调
        pfImportFileCB = NULL;     //导入文件回调
        pfExportFileCB = NULL;     //导出文件回调
        pfRebootCB = NULL;         //重启回调
        pfGetServicesCB = NULL;    //获取服务回调

        m_vecTopoList.clear();
        m_vecCapacity.clear();
        m_strProtocol.clear();
    }
public:
    CBConnectedNtf      pfConnectedCB;      //连接网管成功回调
    CBRegisterRsp       pfRegisterRspCB;    //注册响应回调
    CBGetValue          pfGetValueCB;       //GetValue回调
    CBSetValue          pfSetValueCB;       //SetValue回调
    CBGetTopo           pfGetTopoCB;        //GetTopo回调
    CBDisconnectNtf     pfDisconnCB;        //与网管断链回调
    CBDisconnectNENtf   pfDisconnNECB;      //网管主动断链设备回调
    CBConfigFileBeatReq pfConfigFileBeatCB; //配置filebeat回调
    CBGetUpgradeReq     pfGetUpgradeCB;     //获取升级信息回调
    CBSetUpgradeReq     pfSetUpgradeCB;     //设置升级信息回调
    CBImportFileReq     pfImportFileCB;     //导入文件回调
    CBExportFileReq     pfExportFileCB;     //导出文件回调
    CBRebootReq         pfRebootCB;         //重启回调
    CBGetServices       pfGetServicesCB;    //获取服务回调

    std::vector<TMainTopo>   m_vecTopoList;  // 当前topo列表
    std::vector<std::string> m_vecCapacity;  // 当前Agent具有的能力集

    std::string             m_strProtocol;  //协议
};

struct TInitBaseInfo
{
public:
    TInitBaseInfo()
    {
        m_strLocalIpAddr = "127.0.0.1";
        m_strAgentID.clear();
        m_strName.clear();
        m_strNmsIpAddr.clear();
        m_wNmsPort = 0;
        m_strMainType.clear();
        m_strSubType.clear();
    }

public:
    std::string m_strLocalIpAddr;
    std::string m_strAgentID;
    std::string m_strName;
    std::string m_strNmsIpAddr;
    u16         m_wNmsPort;
    std::string m_strMainType;
    std::string m_strSubType;
};

/*====================================================================
函 数 名: InitNeAgent
功    能: 初始化NEAgent，并注册回调函数(一个程序只可调用一次)
算法实现: 
全局变量:
参    数: const TInitParam* ptInitParam   初始化参数，携带多个回调函数

返 回 值: 成功返回 0; 其他 返回非0
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
====================================================================*/
NE_API int InitNeAgent(const TInitParam* ptInitParam);

/*====================================================================
函 数 名: InitNeAgentWithBaseInfo
功    能: 初始化NEAgent，并注册回调函数(一个程序只可调用一次)
算法实现: 
全局变量:
参    数: const TInitParam* ptInitParam   初始化参数，携带多个回调函数
          TInitBaseInfo tInitBaseInfo     携带一些初始化值，原接口这些值是从配置文件获取的

返 回 值: 成功返回 0; 其他 返回非0
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
====================================================================*/
NE_API int InitNeAgentWithBaseInfo(const TInitParam* ptInitParam, TInitBaseInfo tInitBaseInfo);

/*====================================================================
函 数 名: InitFileBeatPath
功    能: 初始化filebeat的安装路径
算法实现: 
全局变量:
参    数: const char *pszPath filebeat的绝对安装路径

返 回 值: 成功返回 0; 其他 返回非0
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
====================================================================*/
NE_API int InitFileBeatPath(const char *pszPath);

/*====================================================================
函 数 名: ResetNmsAddr
功    能: 重设网管地址
算法实现: 重设之后，会和网管断开，重新以新地址和端口去链接
全局变量:
参    数: TInitBaseInfo tInitBaseInfo 同初始化时设置的参数，这里重置这个Base参数

返 回 值: 成功返回 0; 其他 返回非0
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
====================================================================*/
NE_API int ResetAgentBaseInfo(TInitBaseInfo tInitBaseInfo);

/*====================================================================
函 数 名: RegistReq
功    能: 注册请求接口
算法实现: 
全局变量: 
参    数: u32 dwSerialNO              流水号
          TNEReportRegisterReq tReq   注册请求消息体

返 回 值: 成功返回 0; 其他 返回非0
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
====================================================================*/
NE_API int RegistReq(u32 dwSerialNO, TNEReportRegisterReq tReq);

/*====================================================================
函 数 名: SetValueRsp
功    能: SetValue响应接口
算法实现: 
全局变量: 
参    数: const char* pszID       网元ID
          u32 dwSerialNO          流水号 
          u32 dwErrCode           错误码
          const char* pszInfo     响应消息体
          int dwInfoLen           消息体长度

返 回 值: 成功返回 0; 其他 返回非0
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
====================================================================*/
NE_API int SetValueRsp(const char* pszID, u32 dwSerialNO, u32 dwErrCode, const char* pszInfo, int dwInfoLen);

/*====================================================================
函 数 名: GetValueRsp
功    能: GetValue响应接口
算法实现: 
全局变量: 
参    数: const char* pszID       网元ID
          u32 dwSerialNO          流水号 
          u32 dwErrCode           错误码
          const char* pszInfo     响应消息体
          int dwInfoLen           消息体长度

返 回 值: 成功返回 0; 其他 返回非0
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
====================================================================*/
NE_API int GetValueRsp(const char* pszID, u32 dwSerialNO, u32 dwErrCode, const char* pszInfo, int dwInfoLen);

/*====================================================================
函 数 名: AlarmReport
功    能: 告警上报接口
算法实现: 
全局变量: 
参    数: const char* pszID                 网元ID
          u32 dwSerialNO                    流水号 
          TNEAlarmReport tAlarmReport       消息体

返 回 值: 成功返回 0; 其他 返回非0
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
====================================================================*/
NE_API int AlarmReport(const char* pszID, u32 dwSerialNO, TNEAlarmReport tAlarmReport);

/*====================================================================
函 数 名: GetTopoRsp
功    能: GetTopo响应接口
算法实现: 
全局变量: 
参    数: const char* pszID                 网元ID
          u32 dwSerialNO                    流水号 
          TNEResult tResult                 消息体

返 回 值: 成功返回 0; 其他 返回非0
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
====================================================================*/
NE_API int GetTopoRsp(const char* pszID, u32 dwSerialNO, TNEResult tResult);

/*====================================================================
函 数 名: TopoReport
功    能: topo上报接口
算法实现: 
全局变量: 
参    数: const char* pszID                 网元ID
          u32 dwSerialNO                    流水号 
          EM_TOPO_TYPE emTopoType           上报类型
          TNEReportTopoInfoNty tTopoInfoNty 消息体

返 回 值: 成功返回 0; 其他 返回非0
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
====================================================================*/
NE_API int TopoReport(const char* pszID, u32 dwSerialNO, EM_TOPO_TYPE emTopoType, TNEReportTopoInfoNty tTopoInfoNty);

/*====================================================================
函 数 名: ConfigFileBeatRsp
功    能: 配置filebeat响应
算法实现: 
全局变量: 
参    数: u32 dwSerialNO                    流水号 
          u32 dwErrCode           错误码

返 回 值: 成功返回 0; 其他 返回非0
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
====================================================================*/
NE_API int ConfigFileBeatRsp(int dwSerialNO, u32 dwErrCode);

/*====================================================================
函 数 名: GetUpgradeRsp
功    能: 获取升级参数响应接口
算法实现: 
全局变量: 
参    数: const char* pszID       网元ID
          u32 dwSerialNO          流水号 
          u32 dwErrCode           错误码
          TNEUpdateInfo tUpdateInfo 响应消息体

返 回 值: 成功返回 0; 其他 返回非0
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
====================================================================*/
NE_API int GetUpgradeRsp(const char* pszID, u32 dwSerialNO, u32 dwErrCode, TNEUpdateInfo tUpdateInfo);

/*====================================================================
函 数 名: SetUpgradeRsp
功    能: 设置升级参数响应接口
算法实现: 
全局变量: 
参    数: const char* pszID       网元ID
          u32 dwSerialNO          流水号 
          u32 dwErrCode           错误码

返 回 值: 成功返回 0; 其他 返回非0
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
====================================================================*/
NE_API int SetUpgradeRsp(const char* pszID, u32 dwSerialNO, u32 dwErrCode);

/*====================================================================
函 数 名: ImportFileRsp
功    能: 导入文件响应接口
算法实现: 
全局变量: 
参    数: const char* pszID       网元ID
          u32 dwSerialNO          流水号 
          u32 dwErrCode           错误码

返 回 值: 成功返回 0; 其他 返回非0
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
====================================================================*/
NE_API int ImportFileRsp(const char* pszID, u32 dwSerialNO, u32 dwErrCode, TNEUpdateInfo tRsp);

/*====================================================================
函 数 名: ExportFileRsp
功    能: 导出文件响应接口
算法实现: 
全局变量: 
参    数: const char* pszID       网元ID
          u32 dwSerialNO          流水号 
          u32 dwErrCode           错误码

返 回 值: 成功返回 0; 其他 返回非0
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
====================================================================*/
NE_API int ExportFileRsp(const char* pszID, u32 dwSerialNO, u32 dwErrCode, TNEUpdateInfo tRsp);

/*====================================================================
函 数 名: GetServicesRsp
功    能: GetServices响应接口
算法实现: 
全局变量: 
参    数: const char* pszID                 网元ID
          u32 dwSerialNO                    流水号 
          TNEResult tResult                 消息体

返 回 值: 成功返回 0; 其他 返回非0
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
====================================================================*/
NE_API int GetServicesRsp(const char* pszID, u32 dwSerialNO, u32 dwErrCode, TServicesList tServicesList);


#endif //_NE_AGENT_H
