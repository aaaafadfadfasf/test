/*╔══════════════════════════════════════╗
  ║KDDevFinder.h                                                               ║
  ║  基本功能：                                                                ║
  ║     * 通过组播搜索在线设备                                                 ║
  ║     * 设置在线设备网络地址                                                 ║
  ║     * 设置在线设备入网状态                                                 ║
  ║     * 设置在线设备信息                                                     ║
  ║     * 设置在线设备扩展数据                                                 ║
  ║                                                                            ║
  ╠══════════════════════════════════════╣
  ║修订记录                                                                    ║
  ║1.0      发布1.0版本                                    吴彬      2011-07-19║
  ║         增加协议栈接口                                 吴彬      2011-07-27║
  ╚══════════════════════════════════════╝*/
#ifndef _KDDEVFINDER_H_
#define _KDDEVFINDER_H_
#include "kdvtype.h"


#ifdef WIN32

    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib,"Ws2_32.lib")

    #ifdef __cplusplus
        #define KDDEVFINDER_API               extern "C" __declspec(dllexport)
    #else
        #define KDDEVFINDER_API               __declspec(dllexport)
    #endif //__cplusplus
#elif defined _LINUX_
    #ifdef __cplusplus
        #define KDDEVFINDER_API               extern "C"
    #else
        #define KDDEVFINDER_API
    #endif //__cplusplus
#endif


/*╔══════════════════════════════════════╗
  ║公共定义                                                                    ║
  ╚══════════════════════════════════════╝*/
  
#define KDDEVFINDER_VERSION                 "v1.0"
#define KDDEVFINDER_DEFAULT_FIND_ADDRESS    "239.255.255.200"
#define KDDEVFINDER_DEFAULT_FIND_PORT       21000
#define KDDEVFINDER_DEFAULT_UUID_LEN        32
#define KDDEVFINDER_DEFAULT_STRING_LEN      256
#define KDDEVFINDER_DEFAULT_LONG_STRING_LEN 2048
#define KDDEVFINDER_DEFAULT_TIMEOUT         32
#define MAX_SUPPORT_INTERFACE_NUM 32


typedef enum
{
    EKDDEVFINDER_OK             = 0,        // 成功
    EKDDEVFINDER_INVALID_ARG    = 1,        // 参数错误
    EKDDEVFINDER_OUT_OF_MEMORY  = 2,        // 内存溢出
    EKDDEVFINDER_TIMEOUT        = 3,        // 超时
    EKDDEVFINDER_REFUSE         = 4,        // 拒绝（可用于回调，表示拒绝回应）
    EKDDEVFINDER_SYSFAIL        = 5,        // 系统操作失败
}   EKDDEVFINDER_RESULT;				// 返回值

typedef enum
{
    EKDDEVFINDER_EVENT_SEARCH               = 10000,        // 搜索
    EKDDEVFINDER_EVENT_SEARCH_ACK           = 10001,        // 搜索回应
    EKDDEVFINDER_EVENT_SEARCH_FINISH        = 10003,        // 搜索回应
    EKDDEVFINDER_EVENT_TIMEOUT              = 10009,        // 超时
    EKDDEVFINDER_EVENT_CHANGE_IP            = 10010,        // 修改IP
    EKDDEVFINDER_EVENT_CHANGE_IP_ACK        = 10011,        // 修改IP回应
    EKDDEVFINDER_EVENT_CHANGE_INFO          = 10020,        // 修改设备信息
    EKDDEVFINDER_EVENT_CHANGE_INFO_ACK      = 10021,        // 修改设备信息回应
    EKDDEVFINDER_EVENT_CHANGE_CHECKIN       = 10030,        // 设备入网
    EKDDEVFINDER_EVENT_CHANGE_CHECKIN_ACK   = 10031,        // 设备入网回应
    EKDDEVFINDER_EVENT_CHANGE_CHECKOUT      = 10032,        // 设备入网
    EKDDEVFINDER_EVENT_CHANGE_CHECKOUT_ACK  = 10033,        // 设备入网回应
    EKDDEVFINDER_EVENT_CHANGE_DATA          = 10040,        // 修改设备数据
    EKDDEVFINDER_EVENT_CHANGE_DATA_ACK      = 10041,        // 修改设备数据回应
}   EKDDEVFINDER_EVENT;									// 事件

typedef struct
{
    s8 achUUID[KDDEVFINDER_DEFAULT_UUID_LEN + 1];               // 设备唯一编号 32位UUID
    s8 achName[KDDEVFINDER_DEFAULT_STRING_LEN];             // 设备名称
    s8 achType[KDDEVFINDER_DEFAULT_STRING_LEN];             // 设备类型
    s8 achManufacturer[KDDEVFINDER_DEFAULT_STRING_LEN];     // 设备厂商信息
}   TKDDEVFINDER_DEVICE_INFO;                           // 设备基本信息

typedef struct
{
    s8 achInterface[KDDEVFINDER_DEFAULT_STRING_LEN];    // 网卡接口名
    s8 achIP[KDDEVFINDER_DEFAULT_STRING_LEN];           // IP地址
    s32 nSubmaskLen;                                    // 子网掩码长度
    s8 achGateway[KDDEVFINDER_DEFAULT_STRING_LEN];      // 网关
}   TKDDEVFINDER_DEVICE_IP;                         // 设备IP信息


typedef struct
{
    s8 achName[KDDEVFINDER_DEFAULT_STRING_LEN];         // 扩展数据名称
    s8 achValue[KDDEVFINDER_DEFAULT_LONG_STRING_LEN];   // 扩展数据值
}   TKDDEVFINDER_DEVICE_EXT_DATA;                   // 扩展数据

typedef struct
{
    TKDDEVFINDER_DEVICE_EXT_DATA*   ptExtDataList;                                      // 扩展数据列表                         
    s32                             nExtDataListSize;                                   // 扩展数据列表长度
}   TKDDEVFINDER_DEVICE_DATA;                                                   // 设备数据

typedef struct
{
    s8                              achCheckinServerIP[KDDEVFINDER_DEFAULT_STRING_LEN]; // 入网服务器IP地址
    s32                             nCheckinServerSubmaskLen;                           // 入网服务器子网掩码长度
}   TKDDEVFINDER_DEVICE_CHECKIN;                                                   // 设备入网信息

typedef struct
{
    TKDDEVFINDER_DEVICE_INFO*       ptDeviceInfo;   // 设备基本信息
    TKDDEVFINDER_DEVICE_IP*         ptDeviceIP;     // 设备IP
    TKDDEVFINDER_DEVICE_CHECKIN*    ptCheckin;      // 设备入网信息
    TKDDEVFINDER_DEVICE_DATA*       ptDeviceData;   // 设备数据
    BOOL32                          bEnable;        // 是否启用
    u32                             dwDeviceID;     // 设备编号 对服务端无效
}   TKDDEVFINDER_DEVICE;                     // 设备对象

typedef struct
{
    u32                         dwSN;                               // 事件流水号
    EKDDEVFINDER_RESULT         eSuccess;                           // ACK成功标识，仅对ACK有效
    u32                         dwInterfaceIp;
    u32                         dwRemoteIp;
    union
    {
        TKDDEVFINDER_DEVICE_INFO*   ptSearch;           // EKDDEVFINDER_EVENT_SEARCH            搜索条件
        TKDDEVFINDER_DEVICE*        ptSearchACK;        // EKDDEVFINDER_EVENT_SEARCH_ACK        搜索结果
        s32                         nSearchSum;         // EKDDEVFINDER_EVENT_SEARCH_FINISH     搜索总数
        TKDDEVFINDER_DEVICE*        ptChange;           // EKDDEVFINDER_EVENT_CHANGE_xxx        修改设备
        TKDDEVFINDER_DEVICE*        ptChangeACK;        // EKDDEVFINDER_EVENT_CHANGE_xxx_ACK    修改设备结果
    };
}   TKDDEVFINDER_EVENT_ARG;                     // 事件参数

// 事件处理回调函数定义
typedef EKDDEVFINDER_RESULT (*KDDEVFIDER_EVENT_HANDLER)(EKDDEVFINDER_EVENT eEvent, TKDDEVFINDER_EVENT_ARG* ptArg, void* pvContext);

/*╔══════════════════════════════════════╗
  ║协议栈                                                                      ║
  ╚══════════════════════════════════════╝*/

/* ==========================================================
   函数名: KDDEVFIDER_InitStack
   功能:  初始化协议栈资源
   算法实现: 
   参数说明: 
   返回值说明: 
   ------------------------------------------------------------
   修改记录:
   日 期      版本  修改人  走读人  修改记录
   2012/07/27  1.0  吴彬              创建
   ===========================================================*/
KDDEVFINDER_API EKDDEVFINDER_RESULT KDDEVFIDER_InitStack();

/* ==========================================================
   函数名: KDDEVFIDER_UninitStack
   功能:  释放协议栈资源
   算法实现: 
   参数说明: 
   返回值说明: 
   ------------------------------------------------------------
   修改记录:
   日 期      版本  修改人  走读人  修改记录
   2012/07/27  1.0  吴彬              创建
   ===========================================================*/
KDDEVFINDER_API void KDDEVFIDER_UninitStack();

/*╔══════════════════════════════════════╗
  ║服务端                                                                      ║
  ╚══════════════════════════════════════╝*/

/* ==========================================================
   函数名: KDDEVFIDER_StartServer
   功能: 开始搜索服务
   算法实现: 
   参数说明: KDDEVFIDER_EVENT_HANDLER pfHandler,  [IN] 事件处理函数回调
             TKDDEVFINDER_DEVICE* ptDevice        [IN] 设备初始信息
   返回值说明: 
   ------------------------------------------------------------
   修改记录:
   日 期      版本  修改人  走读人  修改记录
   2012/07/19  1.0  吴彬              创建
   2012/07/23  1.0  吴彬              添加pvContext
   ===========================================================*/
KDDEVFINDER_API EKDDEVFINDER_RESULT KDDEVFIDER_StartServer(KDDEVFIDER_EVENT_HANDLER pfHandler, void* pvContext , const TKDDEVFINDER_DEVICE* ptDevice);


/* ==========================================================
   函数名: KDDEVFIDER_GetCurrentDevice
   功能:  获取当前设备对象的副本
   算法实现: 
   参数说明: TKDDEVFINDER_DEVICE* ptDevice [OUT] 设备对象副本
   返回值说明: 
   ------------------------------------------------------------
   修改记录:
   日 期      版本  修改人  走读人  修改记录
   2012/07/19  1.0  吴彬              创建
   ===========================================================*/
KDDEVFINDER_API EKDDEVFINDER_RESULT KDDEVFIDER_GetCurrentDevice(TKDDEVFINDER_DEVICE* ptDevice);

/* ==========================================================
   函数名: KDDEVFIDER_FreeCurrentDevice
   功能:  销毁KDDEVFIDER_GetCurrentDevice获得的设备对象副本
   算法实现: 
   参数说明: TKDDEVFINDER_DEVICE* ptDevice [IN] 设备对象副本
   返回值说明: 
   ------------------------------------------------------------
   修改记录:
   日 期      版本  修改人  走读人  修改记录
   2012/07/19  1.0  吴彬              创建
   ===========================================================*/
KDDEVFINDER_API void KDDEVFIDER_FreeCurrentDevice(TKDDEVFINDER_DEVICE* ptDevice);

/* ==========================================================
   函数名: KDDEVFIDER_UpdateCurrentDevice
   功能: 重新设置设备参数
   算法实现: 
   参数说明: TKDDEVFINDER_DEVICE* ptDevice 设备对象
   返回值说明: 
   ------------------------------------------------------------
   修改记录:
   日 期      版本  修改人  走读人  修改记录
   2012/07/19  1.0  吴彬              创建
   ===========================================================*/
KDDEVFINDER_API EKDDEVFINDER_RESULT KDDEVFIDER_UpdateDevice(TKDDEVFINDER_DEVICE* ptDevice);


/* ==========================================================
   函数名: KDDEVFIDER_StopServer
   功能:  停止搜索服务
   算法实现: 
   参数说明: 
   返回值说明: 
   ------------------------------------------------------------
   修改记录:
   日 期      版本  修改人  走读人  修改记录
   2012/07/19  1.0  吴彬              创建
   ===========================================================*/
KDDEVFINDER_API void KDDEVFIDER_StopServer();


/*╔══════════════════════════════════════╗
  ║客户端                                                                      ║
  ╚══════════════════════════════════════╝*/
/* ==========================================================
   函数名: KDDEVFIDER_StartClient
   功能: 开始搜索服务
   算法实现: 
   参数说明: KDDEVFIDER_EVENT_HANDLER pfHandler,  [IN] 事件处理函数回调
             TKDDEVFINDER_DEVICE* ptDevice        [IN] 设备初始信息
   返回值说明: 
   ------------------------------------------------------------
   修改记录:
   日 期      版本  修改人  走读人  修改记录
   2012/07/19  1.0  吴彬              创建
   2012/07/23  1.0  吴彬              添加pvContext
   ===========================================================*/
KDDEVFINDER_API EKDDEVFINDER_RESULT KDDEVFIDER_StartClient(u32 dwInterfaceNum, u32* pdwInterfaceList);


/* ==========================================================
   函数名: KDDEVFIDER_Search
   功能:  搜索设备 结果通过异步回调
   算法实现: 
   参数说明: TKDDEVFINDER_DEVICE_INFO* ptDeviceInfo, [IN] 设备对象，用于作为搜索条件 NULL表示无条件
             KDDEVFIDER_EVENT_HANDLER pfHandler,     [IN] 结果处理回调
             void* pvContext,                        [IN] 回调上下文
             u32 dwTimeoutS                          [IN] 搜索超时 单位秒
             u32* pdwSN                              [OUT]流水号
   返回值说明: 
   ------------------------------------------------------------
   修改记录:
   日 期      版本  修改人  走读人  修改记录
   2012/07/19  1.0  吴彬              创建
   ===========================================================*/
KDDEVFINDER_API EKDDEVFINDER_RESULT KDDEVFIDER_Search(const u32 dwInterfaceIp, const TKDDEVFINDER_DEVICE_INFO* ptDeviceInfo, KDDEVFIDER_EVENT_HANDLER pfHandler, void* pvContext, u32 dwTimeoutS, u32* pdwSN);

/* ==========================================================
   函数名: KDDEVFIDER_ChangeIP
   功能:  修改设备IP
   算法实现: 
   参数说明: TKDDEVFINDER_DEVICE* ptDevice,         [IN] 设备对象，需要填写设备ID 以及设备IP
             KDDEVFIDER_EVENT_HANDLER pfHandler,    [IN] 结果处理回调
             void* pvContext,                       [IN] 回调上下文
             u32 dwTimeoutS                         [IN] 搜索超时 单位秒
             u32* pdwSN                             [OUT]流水号
   返回值说明: 
   ------------------------------------------------------------
   修改记录:
   日 期      版本  修改人  走读人  修改记录
   2012/07/19  1.0  吴彬              创建
   ===========================================================*/
KDDEVFINDER_API EKDDEVFINDER_RESULT KDDEVFIDER_ChangeIP(const u32 dwInterfaceIp, const TKDDEVFINDER_DEVICE* ptDevice, KDDEVFIDER_EVENT_HANDLER pfHandler, void* pvContext, u32 dwTimeoutS, u32* pdwSN);

/* ==========================================================
   函数名: KDDEVFIDER_ChangeInfo
   功能:  修改设备基本信息
   算法实现: 
   参数说明: TKDDEVFINDER_DEVICE* ptDevice,         [IN] 设备对象，需要填写设备ID 以及设备基本信息
             KDDEVFIDER_EVENT_HANDLER pfHandler,    [IN] 结果处理回调
             void* pvContext,                       [IN] 回调上下文
             u32 dwTimeoutS                         [IN] 搜索超时 单位秒
             u32* pdwSN                             [OUT]流水号 
   返回值说明: 
   ------------------------------------------------------------
   修改记录:
   日 期      版本  修改人  走读人  修改记录
   2012/07/19  1.0  吴彬              创建
   ===========================================================*/
KDDEVFINDER_API EKDDEVFINDER_RESULT KDDEVFIDER_ChangeInfo(const u32 dwInterfaceIp, const TKDDEVFINDER_DEVICE* ptDevice, KDDEVFIDER_EVENT_HANDLER pfHandler, void* pvContext, u32 dwTimeoutS, u32* pdwSN);

/* ==========================================================
   函数名: KDDEVFIDER_Checkin
   功能:  设备入网
   算法实现: 
   参数说明: TKDDEVFINDER_DEVICE* ptDevice,         [IN] 设备对象，需要填写设备ID 以及入网信息
             KDDEVFIDER_EVENT_HANDLER pfHandler,    [IN] 结果处理回调
             void* pvContext,                       [IN] 回调上下文
             u32 dwTimeoutS                         [IN] 搜索超时 单位秒
             u32* pdwSN                             [OUT]流水号 
   返回值说明: 
   ------------------------------------------------------------
   修改记录:
   日 期      版本  修改人  走读人  修改记录
   2012/07/19  1.0  吴彬              创建
   ===========================================================*/
KDDEVFINDER_API EKDDEVFINDER_RESULT KDDEVFIDER_Checkin(const u32 dwInterfaceIp, const TKDDEVFINDER_DEVICE* ptDevice, KDDEVFIDER_EVENT_HANDLER pfHandler, void* pvContext, u32 dwTimeoutS, u32* pdwSN);

/* ==========================================================
   函数名: KDDEVFIDER_Checkout
   功能: 设备退网
   算法实现: 
   参数说明: TKDDEVFINDER_DEVICE* ptDevice,         [IN] 设备对象，需要填写设备ID
             KDDEVFIDER_EVENT_HANDLER pfHandler,    [IN] 结果处理回调
             void* pvContext,                       [IN] 回调上下文
             u32 dwTimeoutS                         [IN] 搜索超时 单位秒
             u32* pdwSN                             [OUT]流水号 
   返回值说明: 
   ------------------------------------------------------------
   修改记录:
   日 期      版本  修改人  走读人  修改记录
   2012/07/19  1.0  吴彬              创建
   ===========================================================*/
KDDEVFINDER_API EKDDEVFINDER_RESULT KDDEVFIDER_Checkout(const u32 dwInterfaceIp, const TKDDEVFINDER_DEVICE* ptDevice, KDDEVFIDER_EVENT_HANDLER pfHandler, void* pvContext, u32 dwTimeoutS, u32* pdwSN);

/* ==========================================================
   函数名: KDDEVFIDER_ChangeData
   功能: 设备数据修改
   算法实现: 
   参数说明: TKDDEVFINDER_DEVICE* ptDevice,         [IN] 设备对象，需要填写设备ID以及扩展数据
             KDDEVFIDER_EVENT_HANDLER pfHandler,    [IN] 结果处理回调
             void* pvContext,                       [IN] 回调上下文
             u32 dwTimeoutS                         [IN] 搜索超时 单位秒
             u32* pdwSN                             [OUT]流水号
   返回值说明: 
   ------------------------------------------------------------
   修改记录:
   日 期      版本  修改人  走读人  修改记录
   2012/07/19  1.0  吴彬              创建
   ===========================================================*/
KDDEVFINDER_API EKDDEVFINDER_RESULT KDDEVFIDER_ChangeData(const u32 dwInterfaceIp, const TKDDEVFINDER_DEVICE* ptDevice, KDDEVFIDER_EVENT_HANDLER pfHandler, void* pvContext, u32 dwTimeoutS, u32* pdwSN);


/* ==========================================================
   函数名: KDDEVFIDER_CleanSearch
   功能:  清除上次搜索申请的资源，可在业务结束之前调用
   算法实现: 
   参数说明: 
   返回值说明: 
   ------------------------------------------------------------
   修改记录:
   日 期      版本  修改人  走读人  修改记录
   2012/07/19  1.0  吴彬              创建
   ===========================================================*/
KDDEVFINDER_API void KDDEVFIDER_CleanSearch();

/* ==========================================================
   函数名: KDDEVFIDER_StopClient
   功能:  停止搜索服务
   算法实现: 
   参数说明: 
   返回值说明: 
   ------------------------------------------------------------
   修改记录:
   日 期      版本  修改人  走读人  修改记录
   2012/07/19  1.0  吴彬              创建
   ===========================================================*/
KDDEVFINDER_API void KDDEVFIDER_StopClient();

/* ==========================================================
   函数名: KDDEVFIDER_SetLogCB
   功能:  设置日志回调
   算法实现: 
   参数说明: 
   返回值说明: 
   ------------------------------------------------------------
   修改记录:
   日 期      版本  修改人  走读人  修改记录
   2012/07/19  1.0  吴彬              创建
   ===========================================================*/
typedef void (*KDDEVFINDER_LOG)(const char* szLog);
KDDEVFINDER_API void KDDEVFIDER_SetLogCB(KDDEVFINDER_LOG pfLogCB);

#endif //_KDDEVFINDER_H_  

