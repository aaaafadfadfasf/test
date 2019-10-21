#ifndef _KDM_MCUSDK_INCLUDE_
#define _KDM_MCUSDK_INCLUDE_

#include "McuSdkType.h"

#ifdef KDM_MCUSDK_EXPORTS
#define KDM_MCUSDK_API __declspec(dllexport)
#else
#define KDM_MCUSDK_API __declspec(dllimport)
#endif

#if defined (_MAC_IOS_) ||defined (_KDM_LINUX_) || defined (_ANDROID_)
#define STDCALL 
#define KDM_MCUSDK_API 
#else
#define STDCALL __stdcall
#endif

/**********************************************************************************
                                 KEDACOM
/**********************************************************************************

在使用接口的过程中需要注意几点概念：
一是设备ID及域ID，设备ID是指设备在平台中的唯一的编码序列，由平台负责生成，客户端即
SDK只是去使用该设备ID进行操作，而域ID是指平台本身标示的编码序列，用以区分不同的平台

二是设备的视频源和编码通道，视频源就是视频的来源，如一个NVR设备上接的有四个IPC，那
就是说该NVR设备有四个视频源分别对应4个IPC，但该NVR可能有八个编码通道，分别可以编出
不同格式的码流，如每个IPC可以出1080P及720P的码流，这样就是有主辅流的区别了，所以可
以说该NVR设备有4个视频源8个编码通道

三是目前在浏览等操作中，视频源号值与通道号值通常填相等的值
/**********************************************************************************
**********************************************************************************/

#ifdef __cplusplus
extern "C"
{
#endif
    
    /******************************************************************************
    函 数 名：  Kdm_CreateMcuSdk
    功    能：  创建McuSDK实例，要在所有SDK接口中首先被调用，并且在除此接口以外的
                其他接口中，需要用到该接口的返回值
    参    数：  无
    返 回 值：  非空指针，指向sdk实例:成功，NULL:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API void* STDCALL Kdm_CreateMcuSdk();



    /******************************************************************************
    函 数 名：  Kdm_DestroyMcuSdk
    功    能：  销毁McuSDK实例，要在所有操作结束以后调用
    参    数：  pMcuHandle    调用Kdm_CreateMcuSdk接口返回的非空handle
    返 回 值：  无
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API void STDCALL Kdm_DestroyMcuSdk(IN void *pMcuHandle);
    


    /******************************************************************************
    函 数 名：  Kdm_Init
    功    能：  初始化McuSDK实例，创建SDK结束后需要调用该接口对SDK实例进行初始化操作
    参    数：  pMcuHandle    调用Kdm_CreateMcuSdk接口返回的非空handle
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_Init(IN void *pMcuHandle);



    /******************************************************************************
    函 数 名：  Kdm_Cleanup
    功    能：  清理McuSDK实例，在销毁SDK之前需要调用该接口进行相关的清理操作
    参    数：  pMcuHandle    调用Kdm_CreateMcuSdk接口返回的非空handle
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_Cleanup(IN void *pMcuHandle);
    


    /******************************************************************************
    函 数 名：  Kdm_PlatTypeDetect
    功    能：  检测登陆平台的类型，登陆之前可以调用该接口检测平台类型
                如果已经能够确认平台的类型，可以不调用该接口以加快速度
    参    数：  pMcuHandle    调用Kdm_CreateMcuSdk接口返回的非空handle
                strnsAddr     平台的ip地址
                pErrorCode    调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  emBussinessModUnable:    不可知的平台类型，通常意味着要终止操作
                emPlat1BS:    平台1.0
                emPlat2BS:    平台2.0
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API EBussinessMod STDCALL Kdm_PlatTypeDetect(IN void *pMcuHandle,
      IN const char *strnsAddr, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_ModualSelect
    功    能：  选择SDK操作的平台类型模块、码流模块、解码模块
    参    数：  pMcuHandle  调用Kdm_CreateMcuSdk接口返回的非空handle
                emBuss      平台类型，调用Kdm_PlatTypeDetect接口的返回值
                emStream    码流使用模块，其值为枚举类型EStreamMod，具体参见
                            EStreamMod的定义，通常取emG900
                emDec       是否需要SDK解码模块，其值为枚举类型EDecoderMod，具体参
                            见EDecoderMod的定义，通常取emBaseDec
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_ModualSelect(IN void *pMcuHandle, 
      IN EBussinessMod emBuss, IN EStreamMod emStream, IN EDecoderMod emDec);
    
    
    
    /******************************************************************************
    函 数 名：  Kdm_SetSaveLogFile/Kdm_SetSaveLogFile_V2
    功    能：  设置开启或关闭保存SDK的日志文件
    参    数：  pMcuHandle     调用Kdm_CreateMcuSdk接口返回的非空handle
                dwLogFlag      对于Kdm_SetSaveLogFile，是否保存日志文件的标志，1表示
                               保存，0表示不保存
                               对于Kdm_SetSaveLogFile_V2，根据eLogLevel中定义值或操
                               作组合，emLevAll表示开启所有打印类型
                SaveLogFileDirectName    保存日志文件的路径，注意该值是路径名称，而
                                         不是文件名称，如“C:\\LOG”
                SaveLogFileName          保存日志文件的名称，NULL时默认为进程名
                dwMaxSizeKB              单个日志文件的大小，0时默认为50MB
                dwMaxFiles               日志文件的最大数量，0时默认为32个。
    返 回 值：  TRUE:成功，FALSE:失败
    备    注：  注意在同一个进程内使用的日志接口应保持一致，不可混用。dwMaxSizeKB值
                适用于mcusdk和urlplaysdk模块，dwMaxFiles值适用于mcusdk、urlplaysdk
                和uniplay模块。
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2018/03/20  2.0     yss                 创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetSaveLogFile(IN void *pMcuHandle,
      IN u32 dwLogFlag, IN const char* SaveLogFileDirectName );

    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetSaveLogFile_V2(IN void *pMcuHandle,
      IN u32 dwLogFlag, IN const char* SaveLogFileDirectName, 
      IN const char* SaveLogFileName, IN u32 dwMaxSizeKB, IN u32 dwMaxFiles, 
      OUT u32 *pErrorCode );



    /******************************************************************************
    函 数 名：  Kdm_SetScreenShowLog
    功    能：  设置开启或关闭屏幕打印
    参    数：  pMcuHandle    调用Kdm_CreateMcuSdk接口返回的非空handle
                dwShowLogLev  写到屏幕上的日志等级，根据eLogLevel中定义值或操作组合
                              ，emLevAll表示开启所有打印类型
    返 回 值：  TRUE:成功，FALSE:失败
    备    注：  支持printf、__android_log_print和telnet
                telnet端口一般是3010，也可能会被修改
                telnet命令 on开启telnet打印，off关闭telnet打印，help查看其它命令
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2018/03/20  2.0     yss                 创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetScreenShowLog(IN void *pMcuHandle,
      IN u32 dwShowLogLev);



    /******************************************************************************
    函 数 名：  Kdm_GetSdkVersion_V2
    功    能：  获取SDK的版本信息及更新CU的url地址
    参    数：  pMcuHandle    调用Kdm_CreateMcuSdk接口返回的非空handle
                versionInfo   SDK的版本信息结构体指针，外部创建内存
                nBufSize      传入外部创建结构体的大小
                pErrorCode    调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetSdkVersion_V2(IN void *pMcuHandle, 
      OUT McusdkVersionInfo *versionInfo, IN int nBufSize, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_GetPlatDomainInfo
    功    能：  获取当前登录平台的DomainID域信息，DomainID域信息会在浏览时使用
    参    数：  pMcuHandle   调用Kdm_CreateMcuSdk接口返回的非空handle
                tPlatDomain  平台的DomainID域信息，通常是一个32位的字符串
                pErrorCode   调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetPlatDomainInfo(IN void* pMcuHandle,
      OUT DomainID& tPlatDomain, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_GetDomainListTotal
    功    能：  获取平台私有域信息总数
    参    数：  pMcuHandle    调用Kdm_CreateMcuSdk接口返回的非空handle
                tDomainNum    域的总数目
                pErrorCode    调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetDomainListTotal(IN void *pMcuHandle,
      OUT int& nDomainNum, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_GetDomainList
    功    能：  根据域索引号获取平台私有域信息，需要先调用Kdm_GetDomainListTotal
    参    数：  pMcuHandle  调用Kdm_CreateMcuSdk接口返回的非空handle
                DomainIdx   域索引号，0 - (tDomainNum-1)
                tDomainIfno 域信息，包括域ID、域名称、上级平台域ID
                pErrorCode  调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetDomainList(IN void *pMcuHandle,
      IN int nDomainIdx, OUT TDomainInfo& tDomainInfo, OUT u32 *pErrorCode);
    


	/******************************************************************************
    函 数 名：  Kdm_GetChildrenGBDomainListTotal
    功    能：  获取平台国标子域信息总数
    参    数：  pMcuHandle    调用Kdm_CreateMcuSdk接口返回的非空handle
                tGBDomainNum  域的总数目
                pErrorCode    调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetChildrenGBDomainListTotal(
      IN void *pMcuHandle, OUT int& nGBDomainNum, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_GetChildrenGBDomainList
    功    能：  根据域索引号获取平台国标子域信息，需要先
                调用Kdm_GetChildrenGBDomainListTotal
    参    数：  pMcuHandle    调用Kdm_CreateMcuSdk接口返回的非空handle
                nGBDomainIdx  域索引号，0 - (tGBDomainNum-1)
                tGBDomainIfno 域信息，包括域ID、域名称、域状态等
                pErrorCode    调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetChildrenGBDomainList(IN void *pMcuHandle,
      IN int nGBDomainIdx, OUT TChildrenGBDomainInfo& tGBDomainInfo, 
      OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_SetSDKEventCallback/Kdm_SetSDKEventCallback_V2
    功    能：  设置事件回调函数，在各种操作时，通常都有对应的事件回调上来，如操作
                成功、操作失败等
    参    数：  pMcuHandle  调用Kdm_CreateMcuSdk接口返回的非空handle
                EventCBFunc 由使用者提供实现的回调函数，SDK在事件发生时调用该函数，
                            具体响应实现由使用者来决策
                dwUserData  用户回调的自定义数据
    返 回 值：  1:成功，0:失败
    备    注:   支持64bit版本使用Kdm_SetSDKEventCallback_V2
                回调函数内不要调用Mcusdk的其它接口，也不要过长时间的阻塞
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API u32 STDCALL Kdm_SetSDKEventCallback(IN void *pMcuHandle, 
      IN McuSdkEvent_Callback EventCBFunc, IN u32 dwUserData);

    KDM_MCUSDK_API u32 STDCALL Kdm_SetSDKEventCallback_V2(IN void *pMcuHandle,
      IN McuSdkEvent_Callback EventCBFunc, IN KDM_PARAM dwUserData);
    


    /******************************************************************************
    函 数 名：  Kdm_Login
    功    能：  登录平台，获取设备、浏览、下载等操作都需要先登陆平台，每个SDK实例中
                只能登陆一个平台
    参    数：  pMcuHandle    调用Kdm_CreateMcuSdk接口返回的非空handle
                strUser       用户名
                strPassword   用户密码
                strnsAddr     平台地址，通常只填ip，修改了平台登陆端口时，需填写
                              "ip:port"的形式
                strClientType 客户端类型，有"Windows"、"WindowsOCX"、"Linux"、
                              "IOS"、"Android" 几个类型
                pErrorCode    调用失败时带出的错误码，通常成功调用后错误码为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_Login(IN void *pMcuHandle, 
      IN const char *strUser, IN const char *strPassword, 
      IN const char *strnsAddr, IN const char *strClientType, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_GetSession
    功    能：  用于获取登陆平台后获得的seesion号
    参    数：  pMcuHandle    调用Kdm_CreateMcuSdk接口返回的非空handle
                pSesinInfo    用于接收Session信息的已经分配空间的SessionInfo指针
                pErrorCode    调用失败时带出的错误码，通常成功调用后错误码为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2017/04/17  1.0     zsy                 创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetSession(IN void *pMcuHandle,
      OUT SessionInfo *pSessionInfo, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_LoginWithSession
    功    能：  用于传入session号完成SDK登录，获取设备、浏览、下载等操作都需要先
                登陆平台
    参    数：  pMcuHandle  调用Kdm_CreateMcuSdk接口返回的非空handle
                SessionInfo 登陆平台获取到的seesion号
                bUseInsideHeartbeat     是否使用SDK内部的心跳，通常为TRUE
                pErrorCode  调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_LoginWithSession(IN void *pMcuHandle, 
      IN SessionInfo *pSesinInfo, IN BOOL32 bUseInsideHeartbeat,
      OUT u32 *pErrorCode);
    


    /******************************************************************************
    函 数 名：  Kdm_Logout
    功    能：  登出平台
    参    数：  pMcuHandle    调用Kdm_CreateMcuSdk接口返回的非空handle
                pErrorCode    调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_Logout(IN void *pMcuHandle, OUT u32 *pErrorCode);
    


    /******************************************************************************
    函 数 名：  Kdm_GetGroupByGroup
    功    能：  获取平台设备的分组信息，在Kdm_GetGroupNext之前调用
    参    数：  pMcuHandle    调用Kdm_CreateMcuSdk接口返回的非空handle
                groupid       分组的groupid，由该groupid来创建获取分组信息的任务，
                              根分组的groupid为空（即""）
                pErrorCode    调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  返回获取分组的任务id，用于Kdm_GetGroupNext接口，当Kdm_GetGroupNext
                接口获取完成以后，内部销毁该任务id
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API u32 STDCALL Kdm_GetGroupByGroup(IN void *pMcuHandle, 
      IN GroupID groupid, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_GetGroupNext
    功    能：  循环获取平台的设备分组信息，当返回失败的时候表示分组信息获取完成，
                注意子分组的获取，如果分组存在子分组还需要嵌套获取子分组的信息，
                获取子分组同样要创建任务Kdm_GetGroupByGroup进行获取，创建任务中的
                groupid即为父分组信息中的groupid：groupInfo->groupID
    参    数：  pMcuHandle 调用Kdm_CreateMcuSdk接口返回的非空handle
                dwTaskID   获取分组的任务id，调用Kdm_GetGroupByGroup接口返回
                groupInfo  分组的信息，具体定义参见GROUPINFO结构
                pErrorCode 调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetGroupNext(IN void *pMcuHandle, 
      IN u32 dwTaskID, OUT GROUPINFO* groupInfo, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_GetDeviceByGroup
    功    能：  获取分组下面的设备，在Kdm_GetDeviceNext之前调用
    参    数：  pMcuHandle    调用Kdm_CreateMcuSdk接口返回的非空handle
                gourpid       分组的groupid，表示要获取的哪个分组下的设备
                pErrorCode    调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  返回获取分组下设备的任务id，用于Kdm_GetDeviceNext接口，
                当Kdm_GetDeviceNext接口获取完成以后，内部销毁该任务id
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API u32 STDCALL Kdm_GetDeviceByGroup(IN void *pMcuHandle,
      IN GroupID gourpid, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_GetDeviceNext
    功    能：  循环获取分组下的设备，在Kdm_GetDeviceByIndex或Kdm_GetDeviceByGroup
                接口调用后调用，调用失败表示获取结束
    参    数：  pMcuHandle    调用Kdm_CreateMcuSdk接口返回的非空handle
                dwTaskID      获取设备过程的任务id，由接口Kdm_GetDeviceByIndex
                              或者接口Kdm_GetDeviceByGroup调用返回
                deviceInfo    带出的设备信息
                pErrorCode    调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetDeviceNext(IN void *pMcuHandle, 
      IN u32 dwTaskID, OUT DEVICEINFO* deviceInfo, OUT u32 *pErrorCode);
        


    /******************************************************************************
    函 数 名：  Kdm_GetDeviceByIndex
    功    能：  按序号获取分组下面的设备，配合Kdm_GetDeviceNext一起使用
    参    数：  pMcuHandle 调用Kdm_CreateMcuSdk接口返回的非空handle
                gourpid    分组的groupid，表示要获取的哪个分组下的设备
                INdex      获取的设备在分组中的序号，表示从该设备组的第index个设备
                           开始获取，当index的值为0时该接口与Kdm_GetDeviceByGroup
                           接口的功能完全一样
                pErrorCode 调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  返回获取分组下设备的任务id，使用于Kdm_GetDeviceNext接口，当
                Kdm_GetDeviceNext接口获取完成以后，内部销毁该任务id
    支持平台：  平台2.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API u32 STDCALL Kdm_GetDeviceByIndex(IN void *pMcuHandle, 
      IN GroupID gourpid, IN u32 index, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_GetMultiViewDeviceTreeList/Kdm_GetMultiViewDeviceTreeList_V2
    功    能：  获取多视图设备树列表
    参    数：  pMcuHandle 调用Kdm_CreateMcuSdk接口返回的非空handle
                pTreeList  设备的多视图树列表，包括视图个数及具体视图信息等
                pErrorCode 调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    备    注:   Kdm_GetMultiViewDeviceTreeList_V2支持获取用户收藏夹，详见结构体定义
    支持平台：  平台2.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetMultiViewDeviceTreeList(IN void *pMcuHandle,
      OUT MultiViewDeviceTreeList *pTreeList, OUT u32 *pErrorCode);

    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetMultiViewDeviceTreeList_V2(IN void *pMcuHandle,
      OUT MultiViewDeviceTreeList_V2 *pTreeList, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_SelectDeviceTreeID
    功    能：  选择当前操作的设备树
    参    数：  pMcuHandle 调用Kdm_CreateMcuSdk接口返回的非空handle
                treeId     设备树ID
                pErrorCode 调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SelectDeviceTreeID(IN void *pMcuHandle, 
      IN const char *treeId, OUT u32 *pErrorCode);
    
    
    
    /******************************************************************************
    函 数 名：  Kdm_SetDevStatusCallback/Kdm_SetDevStatusCallback_V2
    功    能：  设置设备状态回调函数，用以接收订阅设备状态后SDK的返回信息，需要先设
                置该回调函数，然后再订阅设备状态
    参    数：  pMcuHandle 调用Kdm_CreateMcuSdk接口返回的非空handle
                cbFunc     接收SDK返回设备状态信息的回调函数
                deUserData 用户自定义的数据，在回调函数中使用
    返 回 值：  TRUE:成功，FALSE:失败
    备    注:   支持64bit版本使用Kdm_SetDevStatusCallback_V2
                回调函数内不要调用Mcusdk的其它接口，也不要过长时间的阻塞
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetDevStatusCallback(IN void *pMcuHandle,
      IN DevStatus_Callback cbFunc, IN u32 deUserData);

    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetDevStatusCallback_V2(IN void *pMcuHandle,
      IN DevStatus_Callback cbFunc, IN KDM_PARAM deUserData);



    /******************************************************************************
    函 数 名：  Kdm_SubscriptDeviceStatus
    功    能：  订阅设备的状态信息，只有订阅之后平台才会将该设备的状态信息通过回调
                函数返回给使用者，可以多次连续调用以订阅不同设备的状态信息，一次调
                用最多能够订阅MAX_REQ_DEV_SUBS_NUM个设备的信息
    参    数：  pMcuHandle  调用Kdm_CreateMcuSdk接口返回的非空handle
                vctDeviceID TSUBSDEVS类型结构，内部包含要订阅的设备的个数及
                            各个设备的设备ID，该结构中最多只能包括
                            MAX_REQ_DEV_SUBS_NUM个设备
                emSbs       订阅的设备状态的类型，具体定义参见枚举ESubscriptInfo
                            ，需要注意的是电视墙状态需要单独订阅即不能以或（|）的
                            方式去订阅电视墙的状态信息
                pErrorCode  调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SubscriptDeviceStatus(IN void *pMcuHandle, 
      IN TSUBSDEVS vctDeviceID, IN ESubscriptInfo emSbs, OUT u32 *pErrorCode);
    
    
    
    /******************************************************************************
    函 数 名：  Kdm_UnSubscriptDeviceStatus
    功    能：  取消订阅设备的状态信息，可以多次连续调用以取消订阅不同设备的状态
                信息，一次调用最多能够取消订阅MAX_REQ_DEV_SUBS_NUM个设备的信息
    参    数：  pMcuHandle  调用Kdm_CreateMcuSdk接口返回的非空handle
                vctDeviceID TSUBSDEVS类型结构，内部包含要取消订阅的设备的个数及
                            各个设备的设备ID，该结构中最多只能包括
                            MAX_REQ_DEV_SUBS_NUM个设备
                emSbs       取消订阅的设备状态的类型，定义参见枚举ESubscriptInfo
                pErrorCode  调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_UnSubscriptDeviceStatus(IN void *pMcuHandle,
      IN TSUBSDEVS vctDeviceID, IN ESubscriptInfo emSbs, OUT u32 *pErrorCode);
    
    

    /******************************************************************************
    函 数 名：  Kdm_GetDevSrcStatus
    功    能：  获取设备的视频源状态，平台2.0在收到设备的状态回调类型为emDevConfig
                时调用，平台1.0在收到状态回调类型为emDevOnline调用
    参    数：  pMcuHandle 调用Kdm_CreateMcuSdk接口返回的非空handle
                tDevSrc    设备的相关的信息，包括域id、设备id、通道号及视频源号
                srcStatus  设备的视频源状态信息，包括是否启用、是否在线
                pErrorCode 调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetDevSrcStatus(IN void *pMcuHandle, 
      IN DEVCHN tDevSrc, OUT DEVSRC_ST* srcStatus, OUT u32 *pErrorCode);

    
    
    /******************************************************************************
    函 数 名：  Kdm_GetDeviceGBID
    功    能：  由设备ID获取设备的国标ID，与Kdm_GetDeviceIDByGBID接口功能相反
    参    数：  pMcuHandle 调用Kdm_CreateMcuSdk接口返回的非空handle
                tDevChn    设备的相关的信息，包括域id、设备id、通道号及视频源号
                tDevGbID   获取的设备的国标id
                pErrorCode 调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetDeviceGBID(IN void *pMcuHandle, 
      IN const DEVCHN tDevChn, OUT DEVCHN &tDevGbID, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_GetDeviceIDByGBID/Kdm_GetDeviceChnByGBID
    功    能：  由设备国标ID获取设备ID，与Kdm_GetDeviceGBID接口功能相反
    参    数：  pMcuHandle 调用Kdm_CreateMcuSdk接口返回的非空handle
                tDevGbID   设备的国标ID
                tDevUUID/tDevChn  获取的设备的id/获取的设备id和视频源通道
                pErrorCode 调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetDeviceIDByGBID(IN void *pMcuHandle, 
      IN const DeviceID tDevGbID, OUT DeviceID &tDevUUID, OUT u32 *pErrorCode);

    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetDeviceChnByGBID(IN void *pMcuHandle,
      IN const DeviceID tDevGbID, OUT DEVCHN &tDevChn, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_GetDevicekdmNO
    功    能：  获取设备的kdmNo，即科达编号，该编号同样是唯一的
    参    数：  pMcuHandle 调用Kdm_CreateMcuSdk接口返回的非空handle
                tDevChn    设备的相关的信息，包括域id、设备id、通道号及视频源号
                tDevkdmNo  获取的设备科达编号
                pErrorCode 调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetDevicekdmNO(IN void *pMcuHandle, 
      IN const DEVCHN tDevChn, OUT DeviceKDMNO& tDevkdmNo, OUT u32 *pErrorCode);
    
    
    
    /******************************************************************************
    函 数 名：  Kdm_GetDeviceChannelName
    功    能：  获取设备的所有通道的名称
    参    数：  pMcuHandle  调用Kdm_CreateMcuSdk接口返回的非空handle
                tDeviceId   输入的设备ID
                tDevAllChnIdName  返回的该结构中包含设备所有通道的名称
                pErrorCode  调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetDeviceChannelName(IN void *pMcuHandle, 
      IN const DeviceID tDeviceId, OUT TDevAllChnIdName& tDevAllChnIdName, 
      OUT u32 *pErrorCode);
    


    /******************************************************************************
    函 数 名：  Kdm_SetStreamPattern
    功    能：  设置码流传输的方式
    参    数：  pMcuHandle        调用Kdm_CreateMcuSdk接口返回的非空handle
                eStreamPattern    码流的传输方式，类型参见枚举EStreamFlowPattern的定义
                pErrorCode        调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetStreamPattern(IN void *pMcuHandle,
      IN EStreamFlowPattern eStreamPattern, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_StartRealPlay_V2/Kdm_StartRealPlay_V3
    功    能：  开始实时码流的浏览操作
    参    数：  pMcuHandle   调用Kdm_CreateMcuSdk接口返回的非空handle
                tDevChn      设备的相关的信息，包括域id、设备id、通道号及视频源号
                tStreamParam 码流的相关属性，最常用的是其显示窗口字段m_pDrawWnd，
                             V2接口参见结构体SPARAM，V3接口参加结构体RealplayParam
                tCbSdkMedia  回调函数结构体及用户数据，包括码流回调、YUV数据回调、
                             URL回调，设置了相应的回调后可以在浏览成功后接收到相应
                             的数据，注意URL回调用了提供给用户选择码流的格式，可以
                             不设置这些回调函数，使用SDK内默认的处理方式
                pErrorCode   调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  播放成功后返回的一个从0开始的非65535的playID:成功，65535:播放失败
    备    注:   Kdm_StartRealPlay_V3接口精简tStreamParam结构体字段，同时支持设置码流
                直传，需特定的UDP环境，详见结构体定义
                回调函数内不要调用Mcusdk的其它接口，也不要过长时间的阻塞
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2018/03/20  3.0     zm                  创建
    ******************************************************************************/
    KDM_MCUSDK_API u32 STDCALL Kdm_StartRealPlay_V2(IN void *pMcuHandle,
      IN DEVCHN tDevChn, IN SPARAM tStreamParam, IN CB_SDKMEDIA_V2 tCbSdkMedia, 
      OUT u32 *pErrorCode);
      
    KDM_MCUSDK_API u32 STDCALL Kdm_StartRealPlay_V3(IN void *pMcuHandle,
      IN DEVCHN tDevChn, IN RealplayParam tStreamParam, IN CB_SDKMEDIA_V2 tCbSdkMedia, 
      OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_StartRealPlayByGBID
    功    能：  通过设备国标ID进行实时浏览
    参    数：  pMcuHandle   调用Kdm_CreateMcuSdk接口返回的非空handle
                tDevChnGBID  设备通道的国标ID，GBID填入到DEVCHN::deviceID成员
                tStreamParam 码流的相关属性，最常用的是其显示窗口字段m_pDrawWnd，
                             具体内容参见结构体SPARAM
                tCbSdkMedia  回调函数结构体及用户数据，包括码流回调、YUV数据回调、
                             URL回调，设置了相应的回调后可以在浏览成功后接收到相应
                             的数据，注意URL回调用了提供给用户选择码流的格式，可以
                             不设置这些回调函数，使用SDK内默认的处理方式
                pErrorCode   调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  播放成功后返回的一个从0开始的非65535的playID:成功，65535:播放失败
    备    注：  回调函数内不要调用Mcusdk的其它接口，也不要过长时间的阻塞
    支持平台：  平台2.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2017/03/24  1.0     zsy                 创建
    ******************************************************************************/
    KDM_MCUSDK_API u32 STDCALL Kdm_StartRealPlayByGBID(IN void *pMcuHandle,
      IN DEVCHN tDevChnGBID, IN SPARAM tStreamParam, 
      IN CB_SDKMEDIA_V2 tCbSdkMedia, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_StopRealPlay
    功    能：  根据dwPlayID来停止相应的实时浏览的操作
    参    数：  pMcuHandle  调用Kdm_CreateMcuSdk接口返回的非空handle
                dwPlayID    调用Kdm_StartRealPlay返回的播放ID
                pErrorCode  调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:停止播放成功，FALSE:停止播放失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_StopRealPlay(IN void *pMcuHandle, 
      IN u32 dwPlayID, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_SendPtzControl
    功    能：  向（支持PTZ操作的）设备发送PTZ控制指令
    参    数：  pMcuHandle 调用Kdm_CreateMcuSdk接口返回的非空handle
                tDevChn    设备的相关的信息，包括域id、设备id、通道号及视频源号
                tPtzCmd    PTZ操作的类型结构，包括命令类型、命令的执行速度、命令的
                           等级、命令的持续时间，具体参见结构PTZCMD，命令类型参见
                           枚举ePtzCmd定义
                pErrorCode 调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SendPtzControl(IN void *pMcuHandle, 
      IN DEVCHN tDevChn, IN PTZCMD tPtzCmd, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_SendPointMoveControl
    功    能：  向（支持PTZ操作的）设备发送定点移动指令
    参    数：  pMcuHandle 调用Kdm_CreateMcuSdk接口返回的非空handle
                tDevChn    设备的相关的信息，包括域id、设备id、通道号及视频源号
                tPointCmd  定点移动的操作信息
                pErrorCode 调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SendPointMoveControl(IN void *pMcuHandle, 
      IN DEVCHN tDevChn, IN POINTMOVE tPointCmd, OUT u32 *pErrorCode );



    /******************************************************************************
    函 数 名：  Kdm_SendPtzPreSet/Kdm_LoadPtzPreSet
    功    能：  向设备发送预置位调用指令，两个接口功能相同
    参    数：  pMcuHandle  调用Kdm_CreateMcuSdk接口返回的非空handle
                tDevChn     设备的相关的信息，包括域id、设备id、通道号及视频源号
                tPreSetPosition              预置位编号
                tPtzAuthority/pPtzAuthority  属性信息，速度、等级、持续时间，可传空
                pErrorCode  调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SendPtzPreSet(IN void *pMcuHandle, 
      IN DEVCHN tDevChn, IN int tPreSetPosition, IN TPTZAUTHORITY tPtzAuthority, 
      OUT u32 *pErrCode);
    
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_LoadPtzPreSet(IN void *pMcuHandle, 
      IN DEVCHN tDevChn, IN int tPreSetPosition, IN TPTZAUTHORITY *pPtzAuthority, 
      OUT u32 *pErrCode);    



    /******************************************************************************
    函 数 名：  Kdm_SavePtzPreSet
    功    能：  设置设备的预置位
    参    数：  pMcuHandle  调用Kdm_CreateMcuSdk接口返回的非空handle
                tDevChn     设备的相关的信息，包括域id、设备id、通道号及视频源号
                tPreSetPosition  预置位编号
                pPtzAuthority    属性信息，包含速度、等级、持续时间，可传空
                pErrorCode  调用失败时带出的错误码，通常成功调用后错误码为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SavePtzPreSet(IN void *pMcuHandle, 
      IN DEVCHN tDevChn, IN int tPreSetPosition, IN TPTZAUTHORITY *pPtzAuthority, 
      OUT u32 *pErrCode);



    /******************************************************************************
    函 数 名：  Kdm_TaskWatcher
    功    能：  开启任务守望
    参    数：  pMcuHandle 调用Kdm_CreateMcuSdk接口返回的非空handle
                tDevChn    设备的相关的信息，包括域id、设备id、通道号及视频源号
                tWatchTask 任务守望的信息，包括是否启用、延时时间、持续时间等
                pErrorCode 调用失败时带出来的错误码，通常成功调用后错误码为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_TaskWatcher(IN void *pMcuHandle,
      IN DEVCHN tDevChn, IN TWatchTask tWatchTask, OUT u32 *pErrCode);



    /******************************************************************************
    函 数 名：  Kdm_PrePosWatcher
    功    能：  预置位巡视
    参    数：  pMcuHandle   调用Kdm_CreateMcuSdk接口返回的非空handle
                tDevChn      设备的相关的信息，包括域id、设备id、通道号及视频源号
                tWatchPrePos 巡视的相关信息，包括是否启用、延迟时间、预置位编号等
                pErrorCode   调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_PrePosWatcher(IN void *pMcuHandle,
      IN DEVCHN tDevChn, IN TWatchPrePos tWatchPrePos, OUT u32 *pErrCode);



    /******************************************************************************
    函 数 名：  Kdm_SendPtzBrightnessUp
    功    能：  增加设备的亮度，可配合Kdm_SendPtzBrightnessStop使用
    参    数：  pMcuHandle    调用Kdm_CreateMcuSdk接口返回的非空handle
                tDevChn       设备的相关的信息，包括域id、设备id、通道号及视频源号
                pPtzAuthority 属性信息，包括速度、级别、持续时间长度，可传空
                pErrorCode    调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SendPtzBrightnessUp(IN void *pMcuHandle, 
      IN DEVCHN tDevChn, IN TPTZAUTHORITY *pPtzAuthority, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_SendPtzBrightnessDown
    功    能：  减小设备的亮度，可配合Kdm_SendPtzBrightnessStop使用
    参    数：  pMcuHandle    调用Kdm_CreateMcuSdk接口返回的非空handle
                tDevChn       设备的相关的信息，包括域id、设备id、通道号及视频源号
                pPtzAuthority 属性信息，包括速度、级别、持续时间长度，可传空
                pErrorCode    调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SendPtzBrightnessDown(IN void *pMcuHandle,
      IN DEVCHN tDevChn, IN TPTZAUTHORITY *pPtzAuthority, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_SendPtzBrightnessStop
    功    能：  停止增加或减小设备的亮度的命令
    参    数：  pMcuHandle    调用Kdm_CreateMcuSdk接口返回的非空handle
                tDevChn       设备的相关的信息，包括域id、设备id、通道号及视频源号
                pPtzAuthority 属性信息，包括速度、级别、持续时间长度，可传空
                pErrorCode    调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SendPtzBrightnessStop(IN void *pMcuHandle, 
      IN DEVCHN tDevChn, IN TPTZAUTHORITY *pPtzAuthority, OUT u32 *pErrorCode);
    


    /******************************************************************************
    函 数 名：  Kdm_SendPtzFocusFar
    功    能：  设置设备的远聚焦，可配合Kdm_SendPtzFocusStop使用
    参    数：  pMcuHandle    调用Kdm_CreateMcuSdk接口返回的非空handle
                tDevChn       设备的相关的信息，包括域id、设备id、通道号及视频源号
                pPtzAuthority 属性信息，包括速度、级别、持续时间长度，可传空
                pErrorCode    调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SendPtzFocusFar(IN void *pMcuHandle,
      IN DEVCHN tDevChn, IN TPTZAUTHORITY *pPtzAuthority, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_SendPtzFocusNear
    功    能：  设置设备的近聚焦，可配合Kdm_SendPtzFocusStop使用
    参    数：  pMcuHandle    调用Kdm_CreateMcuSdk接口返回的非空handle
                tDevChn       设备的相关的信息，包括域id、设备id、通道号及视频源号
                pPtzAuthority 属性信息，包括速度、级别、持续时间长度，可传空
                pErrorCode    调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SendPtzFocusNear(IN void *pMcuHandle, 
      IN DEVCHN tDevChn, IN TPTZAUTHORITY *pPtzAuthority, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_SendPtzFocusAuto
    功    能：  设置设备的自动聚焦
    参    数：  pMcuHandle    调用Kdm_CreateMcuSdk接口返回的非空handle
                tDevChn       设备的相关的信息，包括域id、设备id、通道号及视频源号
                pPtzAuthority 属性信息，包括速度、级别、持续时间长度，可传空
                pErrorCode    调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SendPtzFocusAuto(IN void *pMcuHandle, 
      IN DEVCHN tDevChn, IN TPTZAUTHORITY *pPtzAuthority, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_SendPtzFocusStop
    功    能：  停止设备的聚焦
    参    数：  pMcuHandle    调用Kdm_CreateMcuSdk接口返回的非空handle
                tDevChn       设备的相关的信息，包括域id、设备id、通道号及视频源号
                pPtzAuthority 属性信息，包括速度、级别、持续时间长度，可传空
                pErrorCode    调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SendPtzFocusStop(IN void *pMcuHandle, 
      IN DEVCHN tDevChn, IN TPTZAUTHORITY *pPtzAuthority, OUT u32 *pErrorCode);    



    /******************************************************************************
    函 数 名：  Kdm_SetUpStreamTrans
    功    能：  设置码流转发的相关属性，将平台设备码流直接传给某个IP地址端口
    参    数：  pMcuHandle      调用Kdm_CreateMcuSdk接口返回的非空handle
                tDevChn         设备的相关的信息，包括域id、设备id、通道号及视频源号
                tStrTransLoc    本地属性，包括音视频的RTP、RTCP端口、接收码流的IP地址
                tStrTransRemote 远端的相关属性，具体参见TStreamTransRemote结构定义
                pErrorCode      调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  设置成功后返回的一个从0开始的非65535的直传ID:成功，65535:设置失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API u32 STDCALL Kdm_SetUpStreamTrans(IN void *pMcuHandle,
      IN DEVCHN tDevChn, IN TStreamTransLocal tStrTransLoc,
      OUT TStreamTransRemote& tStrTransRemote, OUT u32 *pErrorCode);


    
    /******************************************************************************
    函 数 名：  Kdm_StartStreamTrans
    功    能：  根据直传StrTransId开始码流的直传操作
    参    数：  pMcuHandle    调用Kdm_CreateMcuSdk接口返回的非空handle
                StrTransId    调用Kdm_SetUpStreamTrans接口返回的直传ID
                pErrorCode    调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_StartStreamTrans(IN void *pMcuHandle,
      IN u32 StrTransId, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_StopStreamTrans
    功    能：  根据直传StrTransId停止码流的直传操作
    参    数：  pMcuHandle    调用Kdm_CreateMcuSdk接口返回的非空handle
                StrTransId    调用Kdm_SetUpStreamTrans接口返回的直传ID
                pErrorCode    调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_StopStreamTrans(IN void *pMcuHandle,
      IN u32 StrTransId, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_CreateRecordTask/Kdm_CreateRecordTask_V2
    功    能：  创建录像任务，返回TaskID用于查询录像、录像回放、录像下载等操作
    参    数：  pMcuHandle 调用Kdm_CreateMcuSdk接口返回的非空handle
                tDevChn    设备的相关的信息，包括域id、设备id、通道号及视频源号
                domainName V2接口中域名参数，表示查询该设备在指定域上的录像，而不是
                           查询当前用户登录域上的录像，注意domainName不是域ID而是域
                           名就是设备ID"@"后面的那部分
                pErrorCode 调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  返回非-1的TaskID值:成功，-1:失败
    备    注：  Kdm_CreateRecordTask_V2支持按域创建录像任务，用于查询该设备在指定域
                上的录像
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2018/03/20  2.0     zm                  创建
    ******************************************************************************/
    KDM_MCUSDK_API u32 STDCALL Kdm_CreateRecordTask(IN void *pMcuHandle, 
      IN DEVCHN tDevChn, OUT u32 *pErrorCode);

    KDM_MCUSDK_API u32 STDCALL Kdm_CreateRecordTask_V2(IN void *pMcuHandle,
      IN DEVCHN tDevChn, IN const char *domainName, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_CreateRecordTaskByGBID
    功    能：  按域通过设备国标ID创建录像任务，返回TaskID用于查询录像、录像回放、
                录像下载等操作
    参    数：  pMcuHandle  调用Kdm_CreateMcuSdk接口返回的非空handle
                tDevChnGBID 设备的国标ID
                domainName  设备坐在域的域名称
                pErrorCode  调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  返回非-1的TaskID值:成功，-1:失败
    支持平台：  平台2.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2017/03/24  1.0        zsy                 创建
    ******************************************************************************/
    KDM_MCUSDK_API u32 STDCALL Kdm_CreateRecordTaskByGBID(IN void *pMcuHandle,
      IN DEVCHN tDevChnGBID, IN const char *domainName, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_PreLoadRecord
    功    能：  预加载录像记录，用于查询录像记录，在Kdm_GetRecordNext前调用
    参    数：  pMcuHandle   调用Kdm_CreateMcuSdk接口返回的非空handle
                dwTaskID     调用Kdm_CreateRecordTask返回的任务ID
                tPreLoadTime 预加载录像的起始时间段
                ERecType     录像类型，如平台录像与前端录像，具体参见eRecordType
                pErrorCode   调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_PreLoadRecord(IN void *pMcuHandle, 
      IN u32 dwTaskID, IN TPeriod tPreLoadTime, IN eRecordType ERecType,
      OUT u32 *pErrorCode);   

	KDM_MCUSDK_API BOOL32 STDCALL Kdm_PreLoadRecord_V2(IN void *pMcuHandle,
		IN u32 dwTaskID, IN TPeriod tPreLoadTime, IN eRecordType ERecType, IN emRecordEventType ERecEvent,
		OUT u32 *pErrorCode);
		
		
    
    /******************************************************************************
    函 数 名：  Kdm_GetRecordNext
    功    能：  循环获取录像记录
    参    数：  pMcuHandle  调用Kdm_CreateMcuSdk接口返回的非空handle
                dwTaskID    调用Kdm_CreateRecordTask返回的任务ID
                dwSeekTime  获取录像记录的起始时间，获得其后一条录像记录
                pRecordInfo 获取的录像信息，具体参见TRecordInfo
                pErrorCode  调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetRecordNext(IN void *pMcuHandle, 
      IN u32 dwTaskID, IN u32 dwSeekTime, OUT TRecordInfo *pRecordInfo, 
      OUT u32 *pErrorCode);
	KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetRecordNext_V2(IN void *pMcuHandle,
		IN u32 dwTaskID, IN u32 dwSeekTime, OUT TRecordInfo_V2 *pRecordInfo,
		OUT u32 *pErrorCode);


    /******************************************************************************
    函 数 名：  Kdm_DestroyRecordTask
    功    能：  销毁录像任务
    参    数：  pMcuHandle  调用Kdm_CreateMcuSdk接口返回的非空handle
                dwTaskID    需要销毁的录像任务ID
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_DestroyRecordTask(IN void *pMcuHandle, 
      IN u32 dwTaskID);

    
    
    /******************************************************************************
    函 数 名：  Kdm_QueryRecordInDays/Kdm_QueryRecordInDays_V2
    功    能：  查询录像日历（哪天有录像的标记）
    参    数：  pMcuHandle 调用Kdm_CreateMcuSdk接口返回的非空handle
                recType    录像类型，前端录像或者平台录像
                tDevChn    设备的相关的信息，包括域id、设备id、通道号及视频源号
                domainName V2接口中域名称参数，查询设备在该域上的录像日历，而不是当
                           前用户登录域，不是域ID而是域名，就是设备ID"@"后面的那部分
                period     需要查询的时间区域，传递的时间点需对齐到日的边界(零点时刻)
                           ，时间跨度需为一个月间隔
                bHasRecFlags  用于接收每日录像标记的数组
                flagCnt    数组可以接收标记的个数
                pErrorCode 调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    备    注：  Kdm_QueryRecordInDays_V2支持按域查询录像日历
    支持平台：  平台2.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2018/03/20  2.0     zm                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_QueryRecordInDays(IN void *pMcuHandle, 
      IN eRecordType recType, IN const DEVCHN &tDevChn, IN const TPeriod &period, 
      OUT bool *bHasRecFlags, IN int flagCnt, OUT u32* pErrorCode);

    KDM_MCUSDK_API BOOL32 STDCALL Kdm_QueryRecordInDays_V2(IN void *pMcuHandle,
      IN eRecordType recType, IN const DEVCHN &tDevChn, IN const char *domainName, 
      IN const TPeriod &period, OUT bool *bHasRecFlags, IN int flagCnt, 
      OUT u32* pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_StartRecordPlay/Kdm_StartRecordPlay_V2
    功    能：  开始录像回放
    参    数：  pMcuHandle   调用Kdm_CreateMcuSdk接口返回的非空handle
                dwTaskID     调用Kdm_CreateRecordTask接口返回的任务ID
                dwPlayTime   播放录像的开始时间
                stSparam     码流的相关属性，最常用的是其显示窗口字段m_pDrawWnd，
                             具体内容参见结构体SPARAM
                tCbSdkMedia  回调函数结构体及用户数据，包括码流回调、YUV数据回调，
                             设置了相应的回调后可以在浏览成功后接收到相应的数据，
                             录像回放URL回调无效，填空
                pErrorCode   调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  播放成功后返回的一个从0开始的非65535的playID:成功，65535:播放失败
    备    注:   支持64bit版本使用Kdm_StartRecordPlay_V2
                回调函数内不要调用Mcusdk的其它接口，也不要过长时间的阻塞
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2018/03/20  2.0     zm                  创建
    ******************************************************************************/
    KDM_MCUSDK_API u32 STDCALL Kdm_StartRecordPlay(IN void *pMcuHandle, 
      IN u32 dwTaskID, IN u32 dwPlayTime, IN SPARAM stSparam,
      IN CB_SDKMEDIA tCbSdkMedia, OUT u32 *pErrorCode);  

    KDM_MCUSDK_API u32 STDCALL Kdm_StartRecordPlay_V2(IN void *pMcuHandle,
      IN u32 dwTaskID, IN u32 dwPlayTime, IN SPARAM stSparam,
      IN CB_SDKMEDIA_V2 tCbSdkMedia, OUT u32 *pErrorCode);

	//注意 Kdm_StartRecordPlay_V3 只适用于平台2.0，平台1.0由于平台特殊的机制不支持这个接口
	KDM_MCUSDK_API u32 STDCALL Kdm_StartRecordPlay_V3(IN void *pMcuHandle,
		IN TRecordInfo * recInfo, IN u32 dwPlayTime, IN RecordplayParam *ptSparam,
		IN CB_SDKMEDIA_V2 tCbSdkMedia, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_RecordPlayBySETime/Kdm_RecordPlayBySETime_V2
    功    能：  根据时间段来播放录像
    参    数：  pMcuHandle  调用Kdm_CreateMcuSdk接口返回的非空handle
                dwTaskID    调用Kdm_CreateRecordTask接口返回的任务ID
                tRecordPlayInfo  录像记录的相关信息，包括开始结束时间及渲染窗口等
                tCbSdkMedia 回调函数结构体及用户数据，包括码流回调、YUV数据回调、
                            设置了相应的回调后可以在浏览成功后接收到相应的数据，
                            录像回放URL回调无效，填空
                pErrorCode  调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  播放成功后返回的一个从0开始的非65535的playID:成功，65535:播放失败
    备    注:   支持64bit版本使用Kdm_RecordPlayBySETime_V2
                回调函数内不要调用Mcusdk的其它接口，也不要过长时间的阻塞
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2018/03/20  2.0     zm                  创建
    ******************************************************************************/
    KDM_MCUSDK_API u32 STDCALL Kdm_RecordPlayBySETime(IN void *pMcuHandle, 
      IN u32 dwTaskID, IN TRecordPlayInfo tRecordPlayInfo, 
      IN CB_SDKMEDIA tCbSdkMedia, OUT u32 *pErrorCode);

    KDM_MCUSDK_API u32 STDCALL Kdm_RecordPlayBySETime_V2(IN void *pMcuHandle, 
      IN u32 dwTaskID, IN TRecordPlayInfo tRecordPlayInfo, 
      IN CB_SDKMEDIA_V2 tCbSdkMedia, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_StopRecordPlay
    功    能：  停止录像回放
    参    数：  pMcuHandle 调用Kdm_CreateMcuSdk接口返回的非空handle
                dwPlayID   调用Kdm_StartRecordPlay/Kdm_RecordPlayBySETime接口返回
                           的播放ID
                pErrorCode 调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_StopRecordPlay(IN void *pMcuHandle, 
      IN u32 dwPlayID, OUT u32 *pErrorCode); 



    /******************************************************************************
    函 数 名：  Kdm_SetUpRecordStreamTrans
    功    能：  设置录像码流转发的相关属性，将平台设备录像码流直接传给某个IP地址端口
    参    数：  pMcuHandle      调用Kdm_CreateMcuSdk接口返回的非空handle
                pRecordInfo     录像信息，具体参见TRecordInfo
                tStrTransLoc    本地属性，包括音视频的RTP、RTCP端口、接收码流的IP地址
                tStrTransRemote 远端的相关属性，具体参见TStreamTransRemote结构定义
                pErrorCode      调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  设置成功后返回的一个从0开始的非65535的直传ID:成功，65535:设置失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API u32 STDCALL Kdm_SetUpRecordStreamTrans(IN void *pMcuHandle,
      IN const TRecordInfo &tRecordInfo, IN const TStreamTransLocal &tStrTransLoc, 
	  OUT TStreamTransRemote& tStrTransRemote, OUT u32 *pErrorCode);


    
    /******************************************************************************
    函 数 名：  Kdm_StartRecordStreamTrans
    功    能：  根据直传dwTransId开始录像码流的直传操作
    参    数：  pMcuHandle    调用Kdm_CreateMcuSdk接口返回的非空handle
                dwTransId     调用Kdm_SetUpRecordStreamTrans接口返回的直传ID
                pErrorCode    调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2018/04/28  1.0     yss                 创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_StartRecordStreamTrans(IN void *pMcuHandle,
      IN u32 dwTransId, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_StopRecordStreamTrans
    功    能：  根据直传dwTransId停止录像码流的直传操作
    参    数：  pMcuHandle    调用Kdm_CreateMcuSdk接口返回的非空handle
                dwTransId     调用Kdm_SetUpRecordStreamTrans接口返回的直传ID
                pErrorCode    调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_StopRecordStreamTrans(IN void *pMcuHandle,
      IN u32 dwTransId, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_RecordPlayCtrl
    功    能：  录像播放过程中的VCR控制
    参    数：  pMcuHandle   调用Kdm_CreateMcuSdk接口返回的非空handle
                stRecPlyInfo VCR控制的类型及对应的属性值,具体参见TREPCTLINFO，关
                             于其字段类型的取值参见枚举RecordPlayCtrlType的定义
                pErrorCode   调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    备    注:   Kdm_RecordPlayCtrl_V2支持设置快放时是否启用快解，详见TREPCTLINFO_V2
                结构体定义
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2018/03/20  2.0     zm                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_RecordPlayCtrl(IN void *pMcuHandle,
      IN TREPCTLINFO stRecPlyInfo, OUT u32 *pErrorCode);
    
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_RecordPlayCtrl_V2(IN void *pMcuHandle,
      IN TREPCTLINFO_V2 stRecPlyInfo, OUT u32 *pErrorCode);  
   


    /******************************************************************************
    函 数 名：  Kdm_StartRecordDownLoad/Kdm_StartRecordDownLoad_V2
    功    能：  开始录像下载
    参    数：  pMcuHandle     调用Kdm_CreateMcuSdk接口返回的非空handle
                dwTaskID       调用Kdm_CreateRecordTask接口返回的任务ID
                tDownloadInfo  录像下载相关信息，包括录像文件名称、起始时间、录像
                               类型、下载倍速等
                pErrorCode     调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  开始成功后返回的一个从0开始的非65535的下载ID:成功，65535:下载失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2018/03/20  2.0     zm                  创建
    ******************************************************************************/
    KDM_MCUSDK_API u32 STDCALL Kdm_StartRecordDownLoad(IN void *pMcuHandle,
      IN u32 dwTaskID, IN TRecordDownloadInfo tDownloadInfo, OUT u32 *pErrorCode);

    KDM_MCUSDK_API u32 STDCALL Kdm_StartRecordDownLoad_V2(IN void *pMcuHandle,
      IN u32 dwTaskID, IN TRecordDownloadInfo_V2 tDownloadInfo,
	  OUT u32 *pErrorCode);

	

    /******************************************************************************
    函 数 名：  Kdm_StopRecordDownLoad
    功    能：  停止录像下载
    参    数：  pMcuHandle 调用Kdm_CreateMcuSdk接口返回的非空handle
                dwPlayID   调用Kdm_StartRecordDownLoad/Kdm_StartRecordDownLoad_V2
                           返回的下载ID
                pErrorCode 调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:下载失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_StopRecordDownLoad(IN void *pMcuHandle,
      IN u32 dwPlayID, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_StartLocalRecord/Kdm_StartLocalRecord_V2
    功    能：  开启本地录像，录像文件保存于本地，此接口调用前需保证窗口正常浏览
    参    数：  pMcuHandle  调用Kdm_CreateMcuSdk接口返回的非空handle
                dwPlayID    实时浏览或录像回放时返回的播放ID
                strFileName 带完整路径和文件后缀的录像文件名称
                emFileType  录像的类型，包括mp4、asf，具体参见ELocalRecType
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2018/03/20  2.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_StartLocalRecord(IN void *pMcuHandle, 
      IN u32 dwPlayID, IN const char *strFileName, IN ELocalRecType emFileType);

    KDM_MCUSDK_API BOOL32 STDCALL Kdm_StartLocalRecord_V2(IN void *pMcuHandle,
      IN u32 dwPlayID, IN const char *strFileName, IN ELocalRecType emFileType, 
      OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_StopLocalRecord/Kdm_StopLocalRecord_V2
    功    能：  停止本地录像
    参    数：  pMcuHandle 调用Kdm_CreateMcuSdk接口返回的非空handle
                dwPlayID   实时浏览或录像回放时返回的播放ID
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2018/03/20  2.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_StopLocalRecord(IN void *pMcuHandle, 
      IN u32 dwPlayID);

    KDM_MCUSDK_API BOOL32 STDCALL Kdm_StopLocalRecord_V2(IN void *pMcuHandle,
      IN u32 dwPlayID, OUT u32 *pErrorCode);
    
    
    
    /******************************************************************************
    函 数 名：  Kdm_SaveSnapshot
    功    能：  图片抓拍
    参    数：  pMcuHandle 调用Kdm_CreateMcuSdk接口返回的非空handle
                dwPlayID   实时浏览或录像回放时返回的播放ID
                strPicName 带完整路径和文件后缀的图片名称
                emPicType  抓拍图片保存的格式，具体参见枚举EPictureType
    返 回 值：  TRUE:成功，FALSE:失败
    备    注:   抓拍结果通过事件回调emSnapshot类型来返回，0为成功，非0为错误码
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SaveSnapshot(IN void *pMcuHandle, 
      IN u32 dwPlayID, IN const char *strPicName, IN EPictureType emPicType);
    
    

    /******************************************************************************
    函 数 名：  Kdm_RefreshPlayWnd
    功    能：  刷新播放窗口
    参    数：  pMcuHandle 调用Kdm_CreateMcuSdk接口返回的非空handle
                dwPlayID   实时浏览或录像回放时返回的播放ID
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_RefreshPlayWnd(IN void *pMcuHandle,
      IN u32 dwPlayID);



    /******************************************************************************
    函 数 名：  Kdm_DrawOneFrame
    功    能：  绘制每帧图像接口，仅IOS下使用
    参    数：  pMcuHandle 调用Kdm_CreateMcuSdk接口返回的非空handle
                dwPlayID   实时浏览或录像回放时返回的播放ID
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
#ifdef _MAC_IOS_
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_DrawOneFrame(IN void *pMcuHandle,
      IN u32 dwPlayID);
#endif
    
    
    /******************************************************************************
    函 数 名：  Kdm_SetAudioEnable
    功    能：  开启音频，默认情况下SDK播放是静音，需调用该接口开启音频
    参    数：  pMcuHandle 调用Kdm_CreateMcuSdk接口返回的非空handle
                dwPlayID   实时浏览或录像回放时返回的播放ID
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetAudioEnable(IN void *pMcuHandle,
      IN u32 dwPlayID);



    /******************************************************************************
    函 数 名：  Kdm_SetAudioDisable
    功    能：  关闭音频，与Kdm_SetAudioEnable功能相反
    参    数：  pMcuHandle 调用Kdm_CreateMcuSdk接口返回的非空handle
                dwPlayID   实时浏览或录像回放时返回的播放ID
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetAudioDisable(IN void *pMcuHandle, 
      IN u32 dwPlayID);
    
    

    /******************************************************************************
    函 数 名：  Kdm_SetAudioVolume
    功    能：  设置音量值，范围是0--100
    参    数：  pMcuHandle 调用Kdm_CreateMcuSdk接口返回的非空handle
                dwPlayID   实时浏览或录像回放时返回的播放ID
                nVolume    要设置的音量值大小
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetAudioVolume(IN void *pMcuHandle, 
      IN u32 dwPlayID, IN s32 nVolume);
    


    /******************************************************************************
    函 数 名：  Kdm_GetAudioVolume
    功    能：  获取音量值，范围是0--100
    参    数：  pMcuHandle 调用Kdm_CreateMcuSdk接口返回的非空handle
                dwPlayID   实时浏览或录像回放时返回的播放ID
    返 回 值：  0--100:成功，-1:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API s32 STDCALL Kdm_GetAudioVolume(IN void *pMcuHandle, 
      IN u32 dwPlayID);



    /******************************************************************************
    函 数 名：  Kdm_DropAudio
    功    能：  丢弃浏览码流的音频包
    参    数：  pMcuHandle 调用Kdm_CreateMcuSdk接口返回的非空handle
                dwPlayID   开始实时浏览或者录像回放时返回的ID
                pErrorCode 调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/10/21  1.0     zsy                 创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_DropAudio(IN void *pMcuHandle, IN u32 dwPlayID, 
      IN BOOL32 bDrop, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_SetAudCapVolume/Kdm_GetAudCapVolume
    功    能：  设置/获取采集音量，本地只有一路采集，所以直接设置音量
    参    数：  pMcuHandle   调用Kdm_CreateMcuSdk接口返回的非空handle
                dwVolume/pdwVolume 音量值 0 - 100
                pErrorCode   调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:设置成功，FALSE:设置失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/10/21  1.0     wxh                 创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetAudCapVolume(IN void *pMcuHandle,
      IN int dwVolume, OUT u32 *pErrorCode);

    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetAudCapVolume(IN void *pMcuHandle, 
      OUT int* pdwVolume, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_SetAudCapMute/Kdm_GetAudCapMute
    功    能：  设置/获取采集音量哑音标记
    参    数：  pMcuHandle 调用Kdm_CreateMcuSdk接口返回的非空handle
                bMute/pbMute 1：哑音，0：有音
                pErrorCode 调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2017/03/23  1.0     zsy                 创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetAudCapMute(IN void *pMcuHandle, 
      IN int bMute, OUT u32 *pErrorCode);

    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetAudCapMute(IN void *pMcuHandle, 
      OUT int* pbMute, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_SetInputAudioPowerCB
    功    能：  设置语音呼叫时，获取音频功率的回调函数          
    参    数：  pMcuHandle   调用Kdm_CreateMcuSdk接口返回的非空handle
                pGetInputAudioPowerCB  音频功率回调函数
                pvContext    用户自定义数据，会在回调函数中带出该数据 
    返 回 值：  TRUE:成功，FALSE:失败
    备    注：  Kdm_Init成功之后，开启语音呼叫之前调用
                回调函数内不要调用Mcusdk的其它接口，也不要过长时间的阻塞    
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/10/08  1.0     wxh                 创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetInputAudioPowerCB(IN void *pMcuHandle, 
      IN PMCUSDK_GETAUDIOPOWER pGetInputAudioPowerCB, IN void* pvContext);

    

    /******************************************************************************
    函 数 名：  Kdm_SetVideoOSD
    功    能：  设置字幕信息
    参    数：  pMcuHandle 调用Kdm_CreateMcuSdk接口返回的非空handle
                tDevChn    设备的相关的信息，包括域id、设备id、通道号及视频源号
                tInfo      字幕信息，包括内容及坐标值
                pErrorCode 调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetVideoOSD(IN void *pMcuHandle, 
      IN DEVCHN tDevChn, IN TVidOSD tInfo, OUT u32* pErrorCode);


    
    /******************************************************************************
    函 数 名：  Kdm_GetVideoOSD
    功    能：  获取字幕信息
    参    数：  pMcuHandle 调用Kdm_CreateMcuSdk接口返回的非空handle
                tDevChn    设备的相关的信息，包括域id、设备id、通道号及视频源号
                ptInfo     预先分配好空间的 TVidOSD 结构体，用来接收返回信息
                pErrorCode 调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2017/03/23  1.0        zsy                 创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetVideoOSD(IN void *pMcuHandle,
      IN DEVCHN tDevChn, OUT TVidOSD *ptInfo, OUT u32* pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_StartPlatRec
    功    能：  开启平台录像，录像文件存储在平台上
    参    数：  pMcuHandle 调用Kdm_CreateMcuSdk接口返回的非空handle
                tDevChn    设备的相关的信息，包括域id、设备id、通道号及视频源号
                pErrorCode 调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_StartPlatRec(IN void *pMcuHandle, 
      IN DEVCHN tDevChn, OUT u32* pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_StopPlatRec
    功    能：  停止平台录像
    参    数：  pMcuHandle 调用Kdm_CreateMcuSdk接口返回的非空handle
                tDevChn    设备的相关的信息，包括域id、设备id、通道号及视频源号
                pErrorCode 调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_StopPlatRec(IN void *pMcuHandle,
      IN DEVCHN tDevChn, OUT u32* pErrorCode);
    
    

    /******************************************************************************
    函 数 名：  Kdm_StartPuRec
    功    能：  开启前端录像，录像文件存储在前端设备上
    参    数：  pMcuHandle 调用Kdm_CreateMcuSdk接口返回的非空handle
                tDevChn    设备的相关的信息，包括域id、设备id、通道号及视频源号
                pErrorCode 调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_StartPuRec(IN void *pMcuHandle,
      IN DEVCHN tDevChn, OUT u32* pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_StopPuRec
    功    能：  停止前端录像
    参    数：  pMcuHandle 调用Kdm_CreateMcuSdk接口返回的非空handle
                tDevChn    设备的相关的信息，包括域id、设备id、通道号及视频源号
                pErrorCode 调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_StopPuRec(IN void *pMcuHandle,
      IN DEVCHN tDevChn, OUT u32* pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_GetDevRecordStatus
    功    能：  查询设备录像状态，设备需要先订阅emRecStatus，设备状态回调类型为
                emDevRecStatus时调用该接口
    参    数：  pMcuHandle   调用Kdm_CreateMcuSdk接口返回的非空handle
                tDevChn      设备的相关的信息，包括域id、设备id、通道号及视频源号
                tDevSrcStat  录像的具体信息，包括类型及状态，参见TDevRecordStatus
                pErrorCode   调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetDevRecordStatus(IN void *pMcuHandle, 
      IN DeviceID tDevChn, OUT TDevRecordStatus& tDevSrcStat, 
      OUT u32* pErrorCode);
    
    
    
    /******************************************************************************
    函 数 名：  Kdm_SendKeyFrame
    功    能：  强制发送关键帧请求
    参    数：  pMcuHandle    调用Kdm_CreateMcuSdk接口返回的非空handle
                tDevChn       设备的相关的信息，包括域id、设备id、通道号及视频源号
                pErrorCode    调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SendKeyFrame(IN void *pMcuHandle,
      IN DEVCHN tDevChn, OUT u32 *pErrorCode);

    
    
    /******************************************************************************
    函 数 名：  Kdm_SendTransData_V3
    功    能：  发送透明数据
    参    数：  pMcuHandle    调用Kdm_CreateMcuSdk接口返回的非空handle
                tDevChn       设备的相关的信息，包括域id、设备id、通道号及视频源号
                pTransData    透明数据信息，包括内容长度等信息，参见TTransChnData_V3
                              结构体定义
                pTransDataRsp 透明数据应答信息，参见TTransChnDataRsp结构体定义
                pErrorCode    调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    备    注：  统一使用Kdm_SendTransData_V3接口，发送支持平台的最大buf长度，同时同
                步返回前端应答的数据
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2018/03/20  3.0     zm                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SendTransData_V3(IN void *pMcuHandle, 
      IN DEVCHN tDevChn, IN TTransChnData_V3 *pTransData, 
      OUT TTransChnDataRsp *pTransDataRsp, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_GetTransData_V2
    功    能：  获取前端设备的透明数据，设备需要先订阅emRecvTransData，在设备状态
                回调函数中回调类型为emDevTransDataNtf的时候调用该接口
    参    数：  pMcuHandle    调用Kdm_CreateMcuSdk接口返回的非空handle
                tDevChn       设备的相关的信息，包括域id、设备id、通道号及视频源号
                tTransData    透明数据信息，包括内容长度等信息，参见TTransChnData_V2
                pErrorCode    调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetTransData_V2(IN void *pMcuHandle, 
      IN DEVCHN tDevChn, OUT TTransChnData_V2& tTransData, OUT u32* pErrorCode);

    
    
	/******************************************************************************
    函 数 名：  Kdm_SendTransDataForGB
    功    能：  向国标设备发送透明数据，需入网和平台间级联全为国标协议
    参    数：  pMcuHandle    调用Kdm_CreateMcuSdk接口返回的非空handle
                tDevChn       设备的相关的信息，包括域id、设备id、通道号及视频源号
                pTransData    透明数据发送信息，包括内容及长度等信息
                pTransDataRsp 透明数据同步应答信息，参见TTransChnDataRspGB结构体定义
                pErrorCode    调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2018/03/05  1.0     yss                  创建
    ******************************************************************************/
	KDM_MCUSDK_API BOOL32 STDCALL Kdm_SendTransDataForGB(IN void *pMcuHandle, 
      IN DEVCHN tDevChn, IN TTransChnDataGB *pTransData,
      OUT TTransChnDataRspGB *pTransDataRsp, OUT u32 *pErrorCode);



	/******************************************************************************
    函 数 名：  Kdm_GetTransDataForGB
    功    能：  获取前端国标设备的透明数据，设备需要先订阅emRecvTransData，在设备状态
                回调函数中回调类型为emDevTransDataNtfGB的时候调用该接口
    参    数：  pMcuHandle    调用Kdm_CreateMcuSdk接口返回的非空handle
                tDevChn       设备的相关的信息，包括域id、设备id、通道号及视频源号
                tTransData    透明数据通知信息，包括内容及长度等信息
                pErrorCode    调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetTransDataForGB(IN void *pMcuHandle, 
      IN DEVCHN tDevChn, OUT TTransChnDataGB& tTransData, OUT u32* pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_GetPuEncoderParam_V2
    功    能：  获取前端设备的编码参数
    参    数：  pMcuHandle    调用Kdm_CreateMcuSdk接口返回的非空handle
                tDevChn       设备的相关的信息，包括域id、设备id、通道号及视频源号
                pEncoderParamTotal    编码参数的相关信息，参见TEncoderParamTotal_V2
                pErrorCode    调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetPuEncoderParam_V2(IN void *pMcuHandle, 
      IN DEVCHN tDevChn, OUT TEncoderParamTotal_V2& pEncoderParamTotal,
      OUT u32* pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_SetPuEncoderParam
    功    能：  设置前端设备的编码参数
    参    数：  pMcuHandle    调用Kdm_CreateMcuSdk接口返回的非空handle
                tDevChn       设备的相关的信息，包括域id、设备id、通道号及视频源号
                pEncoderParam 编码参数的相关信息，参见TEncoderParam
                pErrorCode    调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetPuEncoderParam(IN void *pMcuHandle,
      IN DEVCHN tDevChn, IN TEncoderParam *pEncoderParam,OUT u32* pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_GetPuAudioParams
    功    能：  获取前端设备音频的参数
    参    数：  pMcuHandle 调用Kdm_CreateMcuSdk接口返回的非空handle
                tDevChn    设备的相关的信息，包括域id、设备id、通道号及视频源号
                tAudParam  音频参数的相关信息，参见TAudioParams
                pErrorCode 调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetPuAudioParams(IN void *pMcuHandle, 
      IN DEVCHN tDevChn, OUT TAudioParams& tAudParam, OUT u32* pErrorCode);

    

    /******************************************************************************
    函 数 名：  Kdm_GetDevGpsInfo_V2
    功    能：  获取设备的GPS信息，需要先订阅设备的GPS状态，然后在设备状态回调函数
                中收到类型为emDevGpsInfo时调用此接口获取GPS信息
    参    数：  pMcuHandle    调用Kdm_CreateMcuSdk接口返回的非空handle
                tDevChn       设备的相关的信息，包括域id、设备id、通道号及视频源号
                tDevGpsInfo   设备GPS信息，包括纠偏前、纠偏后的经纬度信息等，参见
                              DeviceGPSInfo_V2结构体定义
                pErrorCode    调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetDevGpsInfo_V2(IN void *pMcuHandle,
      IN DEVCHN tDevChn, OUT DeviceGPSInfo_V2& tDevGpsInfo, OUT u32* pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_GetDevLatiLongTude
    功    能：  获取设备经纬度信息。
    参    数：  pMcuHandle    调用Kdm_CreateMcuSdk接口返回的非空handle
                tDevChn       设备的相关的信息，包括域id、设备id、通道号及视频源号
                tDevLatiLongtude  设备经纬度信息，具体参见TDevLatiLongTude的定义
                pErrorCode    调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetDevLatiLongTude(IN void *pMcuHandle,
      IN DEVCHN tDevChn, OUT TDevLatiLongTude& tDevLatiLongtude, 
      OUT u32* pErrorCode);
    
    
    
    /******************************************************************************
    函 数 名：  Kdm_StartVoiceCall
    功    能：  单路语音呼叫，内部采集发送本地音频数据，停止呼叫使用Kdm_StopVoiceCall
    参    数：  pMcuHandle    调用Kdm_CreateMcuSdk接口返回的非空handle
                temDevchn     设备的相关的信息，包括域id、设备id、通道号及视频源号
                temAudioEncType    前端音频编码类型，具体参见枚举EAudioEncType的定义
                pErrorCode    调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  音频呼叫ID 0-(MAX_AUDIO_PLAYID_NUM-1):成功，-1或者65535:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API u32 STDCALL Kdm_StartVoiceCall(IN void *pMcuHandle,
      IN DEVCHN temDevchn, IN EAudioEncType temAudioEncType, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_StopVoiceCall
    功    能：  根据音频呼叫ID停止语音呼叫，用于Kdm_StartVoiceCall语音呼叫
    参    数：  pMcuHandle  调用Kdm_CreateMcuSdk接口返回的非空handle
                VoiceCallId 调用Kdm_StartVoiceCall接口返回的音频呼叫的ID
                pErrorCode  调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_StopVoiceCall(IN void *pMcuHandle,
      IN u32 VoiceCallId, OUT u32 *pErrorCode);
      
      

    /******************************************************************************
    函 数 名：  Kdm_StartVoiceCall_V2
    功    能：  支持同时呼叫多个设备的语音呼叫接口，停止呼叫使用Kdm_StopVoiceCall_V2
    参    数：  pMcuHandle    调用Kdm_CreateMcuSdk接口返回的非空handle
                temDevchn     设备的相关的信息，包括域id、设备id、通道号及视频源号
                temAudioEncType    参数无效，设为emInvailed
                pErrorCode    调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    备    注:   该接口为了支持多个设备呼叫，内部没有缓存信息，返回结果为TRUE或FALSE
                开始呼叫和停止呼叫会在内部计数，暂时不考虑按组id呼叫造成的计数不一致
                停止时对应调用StopVoiceCall_V2接口
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/10/12  1.0     wxh                 创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_StartVoiceCall_V2(IN void *pMcuHandle, 
      IN DEVCHN temDevchn, IN EAudioEncType temAudioEncType, OUT u32 *pErrorCode);

   
   
    /******************************************************************************
    函 数 名：  Kdm_StopVoiceCall_V2
    功    能：  根据设备编号和通道停止语音呼叫，用于Kdm_StartVoiceCall_V2语音呼叫
    参    数：  pMcuHandle    调用Kdm_CreateMcuSdk接口返回的非空handle
                temDevchn     设备的相关的信息，包括域id、设备id、通道号及视频源号
                pErrorCode    调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/10/12  1.0     wxh                 创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_StopVoiceCall_V2(IN void *pMcuHandle, 
      IN DEVCHN temDevchn, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_StartPuCall
    功    能：  开启本地不采集发送码流的语音呼叫
    参    数：  pMcuHandle    调用Kdm_CreateMcuSdk接口返回的非空handle
                tDevChn       设备的相关的信息，包括域id、设备id、通道号及视频源号
                senderAddr    外部将要发送给平台音频码流的发送者的地址信息
                puCallInfo    返回的平台接受码流的信息，和senderAddr的副本
                pErrorCode    调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_StartPuCall(IN void *pMcuHandle, 
      IN const DEVCHN &tDevChn, IN const TMediaAddrInfo &senderAddr, 
      OUT TPuCallInfo &puCallInfo, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_StopPuCall
    功    能：  停止本地不采集发送码流的语音呼叫
    参    数：  pMcuHandle    调用Kdm_CreateMcuSdk接口返回的非空handle
                tDevChn       设备的相关的信息，包括域id、设备id、通道号及视频源号
                pErrorCode    调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_StopPuCall(IN void *pMcuHandle,
      IN const DEVCHN &tDevChn, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_GetTVWallTotal
    功    能：  获取电视墙的总数
    参    数：  pMcuHandle   调用Kdm_CreateMcuSdk接口返回的非空handle
                tvWallTotal  返回的电视墙的总数目
                pErrorCode   调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetTVWallTotal(IN void *pMcuHandle,
      OUT int& tvWallTotal, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_GetTVWall
    功    能：  根据电视墙索引值来获取电视墙的信息，需要先调用Kdm_GetTVWallTotal
    参    数：  pMcuHandle   调用Kdm_CreateMcuSdk接口返回的非空handle
                tvWallIdx    电视墙信息的索引值：0-(tvWallTotal-1)
                tTvWallInfo  返回的电视墙信息，具体参见TTvWallInfoData的定义
                pErrorCode   调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetTVWall(IN void *pMcuHandle,
      IN u32 tvWallIdx, OUT TTvWallInfoData& tTvWallInfo, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_CreateTVWall
    功    能：  创建电视墙
    参    数：  pMcuHandle      调用Kdm_CreateMcuSdk接口返回的非空handle
                CreatTvWallReq  新建的电视墙信息，具体参见TTvWallInfoData
                pErrorCode  调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  电视墙ID:成功，NULL:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API char* STDCALL Kdm_CreateTVWall(IN void *pMcuHandle,
      IN TTvWallInfoData CreatTvWallReq, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_ModifyTVWall
    功    能：  修改电视墙
    参    数：  pMcuHandle  调用Kdm_CreateMcuSdk接口返回的非空handle
                tTvWallInfo 要修改的信息，具体参见TTvWallInfoData
                pErrorCode  调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_ModifyTVWall(IN void *pMcuHandle,
      IN TTvWallInfoData tTvWallInfo, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_DelTVWall
    功    能：  删除电视墙
    参    数：  pMcuHandle    调用Kdm_CreateMcuSdk接口返回的非空handle
                tvWallId      要删除的电视墙ID
                tvWallName    要删除的电视墙的名称
                pErrorCode    调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_DelTVWall(IN void *pMcuHandle,
      IN char* tvWallId, IN char* tvWallName, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_GetTVWallById
    功    能：  根据电视墙ID获取电视墙的信息
    参    数：  pMcuHandle    调用Kdm_CreateMcuSdk接口返回的非空handle
                tvWallId      电视墙ID
                tTvWallInfo   获取的电视墙的相关信息，具体参见TTvWallInfoData
                pErrorCode    调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetTVWallById(IN void *pMcuHandle,
      IN char* tvWallId, OUT TTvWallInfoData& tTvWallInfo, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_GetTVDivNum
    功    能：  根据电视墙ID及电视机序号获取电视墙中电视机的画面数
    参    数：  pMcuHandle    调用Kdm_CreateMcuSdk接口返回的非空handle
                tvWallId      电视墙ID
                tvId          电视墙中电视机的序号
                pErrorCode    调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  大于0的自然数:成功，0:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API u32 STDCALL Kdm_GetTVDivNum(IN void *pMcuHandle,
      IN char* tvWallId, IN int tvId, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_SetTVDivNum
    功    能：  设置电视墙的某电视机的画面数
    参    数：  pMcuHandle    调用Kdm_CreateMcuSdk接口返回的非空handle
                tvSetDivNumData   电视墙ID、电视机ID、画面数，参见结构TTvDivNumData
                pErrorCode    调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetTVDivNum(IN void *pMcuHandle,
      IN TTvDivNumData tvSetDivNumData, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_GetTVDivState
    功    能：  获取电视机的分画面状态
    参    数：  pMcuHandle    调用Kdm_CreateMcuSdk接口返回的非空handle
                tTvWallDivInfo  电视墙ID、电视机ID、分画面ID，参见TTvWallCommonData
                ptTWDivState  返回的分画面状态，参见结构TTVWallTVDivStateNtf
                pErrorCode    调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    备    注:   需要先订阅电视墙状态，在收到emTvWallStaChgNtf状态通知时，循环调用
                获得相应分画面的状态
    支持平台：  平台2.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2018/03/20  1.0     zm                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetTVDivState(IN void *pMcuHandle,
        IN TTvWallCommonData tTvWallDivInfo,OUT TTVWallTVDivStateNtf* ptTWDivState,
        OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_TVWallStartPlay
    功    能：  实时流上电视墙
    参    数：  pMcuHandle            调用Kdm_CreateMcuSdk接口返回的非空handle
                tvWallStartPlyData    电视墙信息及浏览设备信息，参见TTvWallPlayData
                pErrorCode            调用失败时带出来的错误码，通常成功调用后错
                                      误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_TVWallStartPlay(IN void *pMcuHandle,
      IN TTvWallPlayData tvWallStartPlyData, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_TVWallStopPlay
    功    能：  停止实时流上电视墙
    参    数：  pMcuHandle            调用Kdm_CreateMcuSdk接口返回的非空handle
                tvWallStopPlyData     电视墙信息及前端设备信息，参见TTvWallPlayData
                pErrorCode            调用失败时带出来的错误码，通常成功调用后错
                                      误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_TVWallStopPlay(IN void *pMcuHandle,
      IN TTvWallPlayData tvWallStopPlyData, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_TVWallStartPlayRecord
    功    能：  录像回放上电视墙，上墙前必须先进行录像回放操作
    参    数：  pMcuHandle       调用Kdm_CreateMcuSdk接口返回的非空handle
                dwPlayID         录像回放时的播放ID
                tvWallComData    电视墙ID、电视机ID、电视机分画面ID
                pErrorCode       调用失败时带出来的错误码，通常成功调用后错误码为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_TVWallStartPlayRecord(IN void *pMcuHandle,
      IN u32 dwPlayID, IN TTvWallCommonData tvWallComData, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_TVWallStopPlayRecord
    功    能：  停止录像回放上电视墙
    参    数：  pMcuHandle       调用Kdm_CreateMcuSdk接口返回的非空handle
                dwPlayID         录像回放时的播放ID
                tvWallComData    电视墙ID、电视机ID、电视机分画面ID
                pErrorCode       调用失败时带出来的错误码，通常成功调用后错误码为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_TVWallStopPlayRecord(IN void *pMcuHandle,
      IN u32 dwPlayID, IN TTvWallCommonData tvWallComData, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_TVWallQuerySchemeTotal
    功    能：  查询电视墙预案总数
    参    数：  pMcuHandle   调用Kdm_CreateMcuSdk接口返回的非空handle
                tvWallId     电视墙ID
                tTvWallSchemeTotal  预案的总数目
                pErrorCode   调用失败时带出来的错误码，通常成功调用后错误码
                             的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_TVWallQuerySchemeTotal(IN void *pMcuHandle,
      IN char* tvWallId, OUT int& tTvWallSchemeTotal, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_TVWallQueryScheme
    功    能：  根据预案索引号获取预案信息，需要先调用Kdm_TVWallQuerySchemeTotal
    参    数：  pMcuHandle    调用Kdm_CreateMcuSdk接口返回的非空handle
                SchIndx       预案索引号，0-(tTvWallSchemeTotal-1)
                tvWallScheme  预案信息，具体参见结构TTvWallScheme的定义
                pErrorCode    调用失败时带出来的错误码，通常成功调用后错误码为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_TVWallQueryScheme(IN void *pMcuHandle,
      IN int SchIndx, OUT TTvWallScheme& tvWallScheme, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_TVWallCreatScheme
    功    能：  创建预案
    参    数：  pMcuHandle    调用Kdm_CreateMcuSdk接口返回的非空handle
                tvWallScheme  预案信息，具体参见结构TTvWallScheme的定义
                pErrorCode    调用失败时带出来的错误码，通常成功调用后错误码为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_TVWallCreatScheme(IN void *pMcuHandle,
      IN TTvWallScheme tvWallScheme, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_TVWallModifyScheme
    功    能：  修改预案
    参    数：  pMcuHandle    调用Kdm_CreateMcuSdk接口返回的非空handle
                tvWallScheme  预案信息，具体参见结构TTvWallScheme的定义
                pErrorCode    调用失败时带出来的错误码，通常成功调用后错误码为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_TVWallModifyScheme(IN void *pMcuHandle,
      IN TTvWallScheme tvWallScheme, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_TVWallDelScheme
    功    能：  删除预案
    参    数：  pMcuHandle  调用Kdm_CreateMcuSdk接口返回的非空handle
                tvWallId    电视墙ID
                SchemeName  预案名称
                pErrorCode  调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_TVWallDelScheme(IN void *pMcuHandle,
      IN char* tvWallId, IN char* SchemeName, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_TVWallLoadScheme
    功    能：  载入预案
    参    数：  pMcuHandle    调用Kdm_CreateMcuSdk接口返回的非空handle
                tvWallId      电视墙ID
                SchemeName    预案名称
                bLoadWithSave 是否保存载入的预案，TRUE表示保存，FALSE表示不保存
                              目前该参数未使用，可以忽略
                pErrorCode    调用失败时带出来的错误码，通常成功调用后错误码为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_TVWallLoadScheme(IN void *pMcuHandle,
      IN char* tvWallId,IN char* SchemeName, IN BOOL32 bLoadWithSave, 
      OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_TVWallGetSchemePollTotal
    功    能：  获取电视墙预案轮巡总数
    参    数：  pMcuHandle      调用Kdm_CreateMcuSdk接口返回的非空handle
                tSchPollStepReq 电视墙预案轮巡信息，平台2.0只需要填写电视墙ID，平台
                                1.0只需要填写解码器通道，具体参见TSchPollStepReq
                SchemePollTotal 轮巡总数
                pErrorCode      调用失败时带出来的错误码，通常成功调用后错误码为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_TVWallGetSchemePollTotal(IN void *pMcuHandle,
      IN TSchPollStepReq tSchPollStepReq, OUT int& SchemePollTotal, 
      OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_TVWallGetSchemePoll
    功    能：  根据预案轮巡索引号获取电视墙预案轮巡信息，需要先调用
                Kdm_TVWallGetSchemePollTotal
    参    数：  pMcuHandle      调用Kdm_CreateMcuSdk接口返回的非空handle
                SchemePollIndx  预案轮巡索引号，0-(SchemePollTotal-1)
                tvWallSchePoll  轮巡信息，具体参见tagTVWallSchemePollStep的定义
                pErrorCode      调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_TVWallGetSchemePoll(IN void *pMcuHandle,
      IN int SchemePollIndx,OUT tagTVWallSchemePollStep& tvWallSchePoll, 
      OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_TVWallSetSchemePoll
    功    能：  设置预案轮巡信息
    参    数：  pMcuHandle          调用Kdm_CreateMcuSdk接口返回的非空handle
                tSchPollStepReq     预案相关信息，平台2.0 只需要填写电视墙ID
                                    平台1.0只需要填写解码器通道
                tvWallSchePollTotal 所有的轮巡信息及预案的数目
                pErrorCode          调用失败时带出来的错误码，通常成功调用后错
                                    误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_TVWallSetSchemePoll(IN void *pMcuHandle, 
      IN TSchPollStepReq tSchPollStepReq, 
      IN TTvWallSchemePollStepTotal tvWallSchePollTotal, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_TVWallSchemePollControl
    功    能：  预案轮巡控制
    参    数：  pMcuHandle       调用Kdm_CreateMcuSdk接口返回的非空handle
                tSchPollStepReq  预案相关信息，平台2.0只需要填写电视墙ID
                                 平台1.0只需要填写解码器通道
                etvWSPSCmd       控制指令，具体参见emETVWallSCHEPOLLSTEPCMD的定义
                tvWallSchePoll   所有的轮巡信息及预案的数目，平台1.0需设置
                pErrorCode       调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_TVWallSchemePollControl(IN void *pMcuHandle,
      IN TSchPollStepReq tSchPollStepReq, IN emETVWallSCHEPOLLSTEPCMD etvWSPSCmd, 
      IN TTvWallSchemePollStepTotal& tvWallSchePoll, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_TVWallClearAlarm
    功    能：  电视墙分画面消警
    参    数：  pMcuHandle      调用Kdm_CreateMcuSdk接口返回的非空handle
                tvWallId        电视墙ID    
                tvId            电视机ID
                tvDivId         电视机上的分画面ID
                pErrorCode      调用失败时带出来的错误码，通常成功调用后错误码为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2017/04/25  1.0     zsy                 创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_TVWallClearAlarm(IN void *pMcuHandle,
      IN char *tvWallId, IN int tvId, IN int tvDivId, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_GetNruListTotal
    功    能：  获取NRU信息总数
    参    数：  pMcuHandle 调用Kdm_CreateMcuSdk接口返回的非空handle
                beginIdx   开始序号
                num        请求获取的数目个数
                tNruNum    实际获取的数目
                pErrorCode 调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetNruListTotal(IN void *pMcuHandle,
      IN int beginIdx, IN int num, OUT int& tNruNum, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_GetNruList
    功    能：  根据NRU索引号获取NRU信息，需要先调用Kdm_GetNruListTotal
    参    数：  pMcuHandle 调用Kdm_CreateMcuSdk接口返回的非空handle
                NruIdx     NRU索引号
                tNruIfno   NRU信息，包括设备信息、设备网卡信息，具体参见TCNruInfo
                pErrorCode 调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetNruList(IN void *pMcuHandle,
      IN int NruIdx, OUT TCNruInfo& tNruIfno, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_SetRCSSysParam
    功    能：  设置磁盘覆盖策略
    参    数：  pMcuHandle   调用Kdm_CreateMcuSdk接口返回的非空handle
                temRCSSParam 磁盘的覆盖策略，具体参见枚举emCoverPolicy的定义
                pErrorCode   调用失败时带出来的错误码，通常成功调用后错误码为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetRCSSysParam(IN void *pMcuHandle,
      IN emCoverPolicy temRCSSParam, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_GetRCSSysParam
    功    能：  获取磁盘覆盖策略
    参    数：  pMcuHandle      调用Kdm_CreateMcuSdk接口返回的非空handle
                temRCSSParam    磁盘的覆盖策略，具体参见枚举emCoverPolicy的定义
                pErrorCode      调用失败时带出来的错误码，通常成功调用后错误码为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetRCSSysParam(IN void *pMcuHandle,
      OUT emCoverPolicy& temRCSSParam, OUT u32 *pErrorCode);


		
    /******************************************************************************
    函 数 名：  Kdm_SetTimeSyncServer
    功    能：  同步服务器时间，设置后会通过事件回调给使用者，回调类型为emSyncTime
                m_dwReserve1 为UTC时间低32位，m_dwReserve2 为UTC时间高32位
    参    数：  pMcuHandle   调用Kdm_CreateMcuSdk接口返回的非空handle
                bSynctimeServer    是否同步服务器时间
                bSendCmd     是否在返回后直接同步
                pErrorCode   调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetTimeSyncServer(IN void *pMcuHandle, 
      IN BOOL32 bSynctimeServer, IN BOOL32 bSendCmd, OUT u32* pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_SetDirectTransMode
    功    能：  设置码流直传模式
    参    数：  pMcuHandle        调用Kdm_CreateMcuSdk接口返回的非空handle
                bIsDirectTrans    是否启用直传模式，true表示是，false表示不是
    返 回 值：  无
    支持平台：  平台2.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API void STDCALL Kdm_SetDirectTransMode(IN void *pMcuHandle,
      IN bool bIsDirectTrans);



    /******************************************************************************
    函 数 名：  Kdm_SetRecordFileMaxSizeLimit
    功    能：  限制录像下载单个文件的大小，超过此大小的录像文件会被自动切分,
                单位是GB(仅windows有效),默认是3,取值范围[1,0x40000000)
                命名规则：若第一个文件filename.mp4，切分之后文件命名为filename-x.mp4
                x是从1逐渐累加
    参    数：  pMcuHandle    调用Kdm_CreateMcuSdk接口返回的非空handle
                nFileSizeGB   限制的大小值，单位为GB
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetRecordFileMaxSizeLimit(IN void *pMcuHandle, 
      IN int nFileSizeGB);
    


    /******************************************************************************
    函 数 名：  Kdm_ChangePassword
    功    能：  修改平台的登录密码
    参    数：  pMcuHandle    调用Kdm_CreateMcuSdk接口返回的非空handle
                pOldPWD       旧密码
                pNewPWD       新密码
                pErrorCode    调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20    1.0            xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_ChangePassword(IN void *pMcuHandle,
      IN const char *pOldPWD, IN const char *pNewPWD, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_GetCivilcode
    功    能：  获取设备组的行政区划编码（Civilcode）
    参    数：  pMcuHandle     调用Kdm_CreateMcuSdk接口返回的非空handle
                deviceGroupId  设备组ID, 格式 uuid@域名
                pCivilcode     指向存放civilcode的内存
                bufLen         存放civilcode的内存长度
                pErrorCode     调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetCivilcode( IN void* pMcuHandle, 
      IN const GroupID deviceGroupId, OUT char* pCivilcode, IN int bufLen, 
      OUT u32* pErrorCode );            



    /******************************************************************************
    函 数 名：  Kdm_StartPlay/Kdm_StartPlay_V2
    功    能：  使用url进行播放, 目前支持TCP，UDP，rtsp，文件 
                文件播放时url举例:["file://D:/recfile/part1/3.mp4?aa=xx&bb=yy"]
    参    数：  pMcuHandle     调用Kdm_CreateMcuSdk接口返回的非空handle
                utf8_url       进行直接播放使用的URL
                tStreamParam   码流的相关属性，最常用的是其显示窗口字段m_pDrawWnd，
                               具体内容参见结构体SPARAM
                tCbSdkMedia    回调函数结构体及用户数据，包括码流回调、YUV数据回调，
                               URL回调不可用，设置为空
                pErrorCode     调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  播放ID 129-65534:成功，65535:失败
    备    注:   支持64bit版本使用Kdm_StartPlay_V2
                回调函数内不要调用Mcusdk的其它接口，也不要过长时间的阻塞
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API u32 STDCALL Kdm_StartPlay(IN void *pMcuHandle, 
      IN const char *utf8_url, IN SPARAM tStreamParam, 
      IN CB_SDKMEDIA CbSdkMedia, OUT u32 *pErrorCode);

    KDM_MCUSDK_API u32 STDCALL Kdm_StartPlay_V2(IN void *pMcuHandle,
      IN const char *utf8_url, IN SPARAM tStreamParam,
      IN CB_SDKMEDIA_V2 CbSdkMedia, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_StopPlay
    功    能：  停止播放
    参    数：  pMcuHandle    调用Kdm_CreateMcuSdk接口返回的非空handle
                dwPlayID      成功播放时返回的播放ID
                pErrorCode    调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_StopPlay(IN void *pMcuHandle, 
      IN u32 dwPlayID, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_GetRecordFileTimeLen
    功    能：  获取录像文件的总时长（单位毫秒），用于Kdm_StartPlay播放录像文件
    参    数：  pMcuHandle    调用Kdm_CreateMcuSdk接口返回的非空handle
                dwPlayID      成功播放时返回的播放ID
                pErrorCode    调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  大于0的自然数:成功，0:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API u32 STDCALL Kdm_GetRecordFileTimeLen(IN void *pMcuHandle,
      IN u32 dwPlayID, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_GetRecordPlayedTime
    功    能：  获取录像文件当前已经播放的时长（单位毫秒），用于Kdm_StartPlay播放
                录像文件
    参    数：  pMcuHandle    调用Kdm_CreateMcuSdk接口返回的非空handle
                dwPlayID      成功播放时返回的播放ID
                pErrorCode    调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  大于0的自然数:成功，0:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API u32 STDCALL Kdm_GetRecordPlayedTime(IN void *pMcuHandle,
      IN u32 dwPlayID, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_SetRecordPlayedTime
    功    能：  设置录像文件当前已经播放的时长（单位毫秒），也就是录像文件的Seek功
                能，用于Kdm_StartPlay播放录像文件
    参    数：  pMcuHandle    调用Kdm_CreateMcuSdk接口返回的非空handle
                dwPlayID      成功播放时返回的播放ID
                dwPlayedTime  设置的播放时间值
                pErrorCode    调用失败时带出来的错误码，通常成功调用后错误码为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetRecordPlayedTime(IN void *pMcuHandle,
      IN u32 dwPlayID, IN u32 dwPlayedTime, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_PauseRecordPlay
    功    能：  暂停或恢复录像播放，用于Kdm_StartPlay播放录像文件
    参    数：  pMcuHandle 调用Kdm_CreateMcuSdk接口返回的非空handle
                dwPlayID   成功播放时返回的播放ID
                bPause     是否暂停，true表示暂停，false表示播放
                pErrorCode 调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_PauseRecordPlay(IN void *pMcuHandle,
      IN u32 dwPlayID, IN bool bPause, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_GetDeviceStreamUrl
    功    能：  查询指定设备的rtsp等类型的链接地址信息
    参    数：  pMcuHandle 调用Kdm_CreateMcuSdk接口返回的非空handle
                tDevChn    设备的相关的信息，包括域id、设备id、通道号及视频源号
                urlType    URL的类型，具体参加枚举emUrlType的定义
                urlList    返回的URL列表，内含有URL的数量及每个URL的信息
                pErrorCode 调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetDeviceStreamUrl(IN void *pMcuHandle, 
      IN const DEVCHN &tDevChn, IN emUrlType urlType, 
      OUT Stream_UrlList &urlList, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_CreateSearchTask
    功    能：  创建设备搜索任务，返回搜索设备的任务ID
    参    数：  pMcuHandle     调用Kdm_CreateMcuSdk接口返回的非空handle
                queryKey       搜索条件
                maxResultItem  最大的结果集数目
                timeoutMSec    搜索的超时时间
                bNeedNotify    是否启用搜索通知。设置为true，后台搜索完成后会在事件
                               回调函数里回调emSearchResult类型的通知。设置为false，
                               没有事件通知,此时调用Kdm_IsSearchFinished查询搜索进度
                pErrorCode     调用失败时带出来的错误码，通常成功调用后错误码为0
    返 回 值：  大于0的自然数:成功，0:失败
    支持平台：  平台2.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API u32 STDCALL Kdm_CreateSearchTask(IN void *pMcuHandle, 
      IN const char* queryKey,IN u32 maxResultItem, IN u32 timeoutMSec, 
      IN BOOL32 bNotifyResult, OUT u32 *pErrorCode);

    

    /******************************************************************************
    函 数 名：  Kdm_IsSearchFinished
    功    能：  实时判断搜索是否已经完成
    参    数：  pMcuHandle    调用Kdm_CreateMcuSdk接口返回的非空handle
                dwTaskID      Kdm_CreateSearchTask 创建的搜索任务ID
                curPercent    返回当前已经完成的进度值
                totalPercent  返回总进度值
                pErrorCode    调用失败时带出来的错误码，通常成功调用后错误码为0
    返 回 值：  返回TRUE表示搜索已经完成。返回FALSE：如果pErrorCode错误码为0则表示
                搜索还未完成，如果pErrorCode错误码不为0，则表示函数本身调用失败了
    备    注：  本接口是供用户实时查询搜索是否完成用的，另外我们的事件回调里也会有
                专门的事件异步通知上层搜索是否完成
    支持平台：  平台2.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/24  1.0     zsy                 创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_IsSearchFinished(IN void *pMcuHandle, 
      IN u32 dwTaskID, OUT int *curPercent, OUT int *totalPercent, 
      OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_GetSearchResult
    功    能：  根据序号获取搜索的设备信息
    参    数：  pMcuHandle 调用Kdm_CreateMcuSdk接口返回的非空handle
                dwTaskID   任务ID
                startIndex 起始序号
                maxNum     期望得到的数目
                item       存放返回值的数组,大小不小于maxNum
                curNum     本次获取返回的数目
                totalNum   结果总数(搜索未完成时可变)
                lastPage   是否是最后一页
                pErrorCode 调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetSearchResult(IN void *pMcuHandle, 
      IN u32 dwTaskID, IN const u32 startIndex, IN const u32 maxNum, 
      OUT tagTSearchResultItem* item, OUT u32 *curNum, OUT u32 *totalNum,    
      OUT BOOL32* lastPage, OUT u32 *pErrorCode );



    /******************************************************************************
    函 数 名：  Kdm_DestroySearchTask
    功    能：  销毁搜索设备的任务
    参    数：  pMcuHandle 调用Kdm_CreateMcuSdk接口返回的非空handle
                dwTaskID   任务ID
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_DestroySearchTask(IN void *pMcuHandle,
      IN u32 dwTaskID);



    /******************************************************************************
    函 数 名：  Kdm_IsCapbilityEnable
    功    能：  判断平台或SDK是否支持某个能力，某些接口需要平台或SDK具有这个能力才能
                正常使用
    参    数：  pMcuHandle 调用Kdm_CreateMcuSdk接口返回的非空handle
                capType    需要查询的能力类型
    返 回 值：  TRUE:支持，FALSE:不支持
    备    注：  该接口必须在成功登录平台后再调用，否则只会返回FALSE
    支持平台：  平台2.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/05/20  1.0     xw                  创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_IsCapbilityEnable(IN void *pMcuHandle, 
      IN eCapType capType);



    /******************************************************************************
    函 数 名：  Kdm_GetDeviceExtraInfoInt/Kdm_GetDeviceExtraInfoString
    功    能：  按类型获取设备额外信息，数值型参数/字符串型参数
    参    数：  pMcuHandle   调用Kdm_CreateMcuSdk接口返回的非空handle
                grpId        设备的组ID，可传空
                devId        设备ID
                type         获取设备额外信息的类型
                pValue/pBuf  返回数值/字符串的预先分配的内存指针
                bufSize      对应pBuf的大小
                pErrorCode   调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    备    注：  该接口是直接向平台获取设备完整信息后再提取所需信息，以应对不获取列
                表直接传设备ID进行的操作
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/06/23  1.0     zsy                 创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetDeviceExtraInfoInt(IN void *pMcuHandle, 
      IN const GroupID grpId, IN const DeviceID devId, IN EDeviceExtraInfoType type, 
      OUT int *pValue, OUT u32 *pErrorCode);

    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetDeviceExtraInfoString(IN void *pMcuHandle, 
      IN const GroupID grpId, IN const DeviceID devId, IN EDeviceExtraInfoType type, 
      OUT char *pBuf, OUT int *bufSize, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_GetDevGroupExtraInfoInt/Kdm_GetDevGroupExtraInfoString
    功    能：  按类型获取设备组额外信息，数值型参数/字符串型参数。
    参    数：  pMcuHandle   调用Kdm_CreateMcuSdk接口返回的非空handle
                grpId        设备的组ID
                type         获取设备组额外信息的类型
                pValue/pBuf  返回数值/字符串的预先分配的内存指针
                bufSize      对应pBuf的大小
                pErrorCode   调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2017/03/23  1.0     zsy                 创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetDevGroupExtraInfoInt(IN void *pMcuHandle, 
      IN const GroupID grpId, IN EDevGroupExtraInfoType type, OUT int *pValue, 
      OUT u32 *pErrorCode);

    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetDevGroupExtraInfoString(IN void *pMcuHandle, 
      IN const GroupID grpId, IN EDevGroupExtraInfoType type, OUT char *pBuf, 
      OUT int *bufSize, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_GetVideoSrcExtraInfoInt/Kdm_GetVideoSrcExtraInfoString
    功    能：  按类型获取设备视频源额外信息，数值型参数/字符串型参数。
    参    数：  pMcuHandle   调用Kdm_CreateMcuSdk接口返回的非空handle
                grpId        视频源组ID，可传空
                devSrc       设备视频源信息
                type         获取视频源额外信息的类型
                pValue/pBuf  返回数值/字符串的预先分配的内存指针
                bufSize      对应pBuf的大小
                pErrorCode   调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2017/03/23  1.0     zsy                 创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetVideoSrcExtraInfoInt(IN void *pMcuHandle, 
      IN const GroupID grpId, IN const DEVCHN devSrc, 
      IN EVideoSrcExtraInfoType type, OUT int *pValue, OUT u32 *pErrorCode);

    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetVideoSrcExtraInfoString(IN void *pMcuHandle, 
      IN const GroupID grpId, IN const DEVCHN devSrc, IN EVideoSrcExtraInfoType type, 
      OUT char *pBuf, OUT int *bufSize, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_GetDevAzimuthalAngle
    功    能：  获取设备的方位角信息
    参    数：  pMcuHandle 调用Kdm_CreateMcuSdk接口返回的非空handle
                dev        设备相关信息
                pBuf       用来存放方位角信息的缓存
                bufSize    指定pBuf的大小以及返回实际填充pBuf的大小
    返 回 值：  TRUE:成功，FALSE:失败
    备    注：  本函数不会解析返回的方位角信息的内容，解析由调用者自己进行。此缓存
                信息由前端设备定义，SDK只透传给上层。考虑到前端设备的产品迭代对此定
                义的影响，解析缓存时需与前端设备协商确定。
    支持平台：  平台2.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2017/03/23  1.0     zsy                 创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetDevAzimuthalAngle(IN void *pMcuHandle, 
      IN const DEVCHN dev, OUT char *pBuf, OUT int *bufSize, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_SetDecodeMode
    功    能：  设置是否开启硬件解码，全局设置，浏览前调用
    参    数：  pMcuHandle 调用Kdm_CreateMcuSdk接口返回的非空handle
                decMode    设置硬解或软解的模式    
    返 回 值：  无
    备    注：  默认是软解码，开启硬解需设备支持，硬解开关只对设置后的浏览请求生效
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2017/03/23  1.0     zsy                 创建
    ******************************************************************************/
    KDM_MCUSDK_API void STDCALL Kdm_SetDecodeMode(IN void *pMcuHandle, 
      IN EDecodeMode decMode);



    /******************************************************************************
    函 数 名：  Kdm_GetStreamSampleRate
    功    能：  获取码流采样率
    参    数：  pMcuHandle 调用Kdm_CreateMcuSdk接口返回的非空handle
                dwPlayID   浏览播放成功时的播放ID
    返 回 值：  TRUE:成功，FALSE:失败
    备    注：  只支持UDP模式
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2017/03/23  1.0     zsy                 创建
    ******************************************************************************/    
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetStreamSampleRate(IN void *pMcuHandle, 
      IN u32 dwPlayID, OUT int *pVideoSampleRate, OUT int *pAudioSampleRate, 
      OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_SaveVideoBufToFile
    功    能：  将yuv数据保存为本地的bmp文件或者jpg文件
    参    数：  pMcuHandle  调用Kdm_CreateMcuSdk接口返回的非空handle
                nFileType   保存的文件类型，见格式EPictureType，0为bmp32，1为jpg，其
                            他格式暂不支持
                pBuf        保存视频yuv数据的指针
                nSize       buf的长度
                nWidth      yuv图像的宽
                nHeight     yuv图像的高
                nVideoType  pBuf保存的视频数据的类型，见格式MCUSDK_FRAME_DATA_FORMAT，
                            目前只支持NV12和I420，使用SDK的YUV回调格式是I420
                sFileName   目标文件路径
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/09/20  1.0     wxh                 创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SaveVideoBufToFile(IN void *pMcuHandle, 
      IN int nFileType, IN u8* pBuf, IN int nSize, IN int nWidth, IN int nHeight, 
      IN long nVideoType, IN char* sFileName, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_ChangePlayWnd
    功    能：  改变图像的播放窗口        
    参    数：  pMcuHandle  调用Kdm_CreateMcuSdk接口返回的非空handle
                dwPlayID    浏览的播放ID
                nRegionNum  区域索引信息,ID=0的是主的全屏的显示区域
                pSrcRect    视频的区域,根据码流的绝对位置，后期改变窗口大小，都不会
                            影响这个函数的效果。填NULL，就是全屏,显示宽高必须大于0
                hDestWnd    区域视频的显示句柄
                bEnable     是否启用区域，为false，这个窗口句柄将不会显示图像
    返 回 值：  TRUE:成功，FALSE:失败
    备    注：  nRegionNum是用来指定窗口索引，当把某一路图像显示到多个hwnd的时候，
                这个值才会大于0一般改变图像的缩放区域，修改图像的显示句柄，将
                nRegionNum设置为0即可
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/09/20  1.0     wxh                 创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_ChangePlayWnd(IN void *pMcuHandle, 
      IN u32 dwPlayID, IN u32 nRegionNum, IN MCUSDK_KDRECT* pSrcRect, 
      IN void* hDestWnd, IN BOOL32 bEnable, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_SetLocalIp
    功    能：  设置本地IP地址         
    参    数：  pMcuHandle  调用Kdm_CreateMcuSdk接口返回的非空handle
                strLocalIp  本地的ip地址，字符串最长为128byte
    返 回 值：  TRUE:成功，FALSE:失败
    备    注：  这个函数一般不需要设置，提供给CU使用，不设置的情况下，sdk内部会自动
                获取本地ip地址，在Kdm_Init之后，Kdm_Login之前调用
    支持平台：  平台2.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/09/27  1.0     wxh                 创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetLocalIp(IN void *pMcuHandle, 
      IN const char* strLocalIp, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_SetRecvRsParam
    功    能：  设置重传参数和是否开启重传         
    参    数：  pMcuHandle  调用Kdm_CreateMcuSdk接口返回的非空handle
                tRsParam    重传参数
                bRsFlag     是否开启重传
    返 回 值：  TRUE:成功，FALSE:失败
    备    注：  只支持UDP模式
                一般情形下，sdk会设置一个重传级别，默认是开启重传的
                SDK参照CU客户端的级别设置level-1 level-2 level-3,默认为level-2
                TRSParam tRsPara = {80, 120, 160, 200};//level-1
                TRSParam tRsPara = {80, 200, 480, 720};//level-2
                TRSParam tRsPara = {80, 280, 600, 920};//level-3
                Kdm_Init之后，UDP播放之前调用
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/09/27  1.0     wxh                 创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetRecvRsParam(IN void *pMcuHandle, 
      IN MCUSDK_TRSParam tRsParam, IN BOOL32 bRsFlag, OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_EnableNATDetection
    功    能：  开启或关闭发送NAT探测包
    参    数：  pMcuHandle   调用Kdm_CreateMcuSdk接口返回的非空handle
                nEnableFlag  开启或关闭标记，0表示由SDK内部决定是否启用NAT探测包，
                             1表示强制启用NAT探测，2表示强制不启用NAT探测
                nInterval    表示开启NAT探测时探测包发送的时间间隔，单位为秒
    返 回 值：  TRUE:成功，FALSE:失败
    备    注：  当客户端相对于平台处于NAT内，又需要浏览UDP码流时，就需要开启NAT探测，
                以便UDP码流能够到达客户端。本接口仅设置开启或关闭状态，探测包会在后
                台定时发送。发送成功与否，受网络状况或操作系统当前登录用户的权限等多
                方面因素决定
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/10/11  1.0     zsy                 创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_EnableNATDetection(IN void *pMcuHandle, 
      IN int nEnableFlag, IN int nInterval);



    /******************************************************************************
    函 数 名：  Kdm_SetPlatNATMode
    功    能：  指定平台的NAT类型
    参    数：  pMcuHandle  调用Kdm_CreateMcuSdk接口返回的非空handle    
                nNATMode    指定平台的NAT类型,未知请选择默认自动 cusdk__NATModeAuto
                            cusdk__NATModeAuto = 0, cusdk__NATModeInner = 1, 
                            cusdk__NATModeOutside = 2
    返 回 值：  TRUE:成功，FALSE:失败
    备    注：  一般只在非常特殊的情况下才需要调用这个接口,并且需要在登录平台之前调
                用正常情况下不需要调用此接口
    支持平台：  平台2.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2016/11/9   1.0     zsy                 创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetPlatNATMode(IN void *pMcuHandle, 
      IN int nNATMode);



    /******************************************************************************
    函 数 名：  Kdm_SetSignalTimeout
    功    能：  设置信令超时时间函数
    参    数：  pMcuHandle  调用Kdm_CreateMcuSdk接口返回的非空handle
                nRcvTimeout 接收数据的超时时间，单位为秒
    返 回 值：  TRUE:成功，FALSE:失败
    备    注：  此接口应该在Kdm_Init之后、Kdm_Login之前调用
    支持平台：  平台2.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2018/01/30  1.0     yss                 创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetSignalTimeout(IN void *pMcuHandle, 
      IN int nRcvTimeout);



    /******************************************************************************
    函 数 名：  Kdm_SetDrawCallBack
    功    能：  设置播放码流时的绘图回调函数
    参    数：  pMcuHandle 调用Kdm_CreateMcuSdk接口返回的非空handle
                dwPlayID   成功播放返回的播放ID
                pfunc      回调函数
                userdata   回调函数里的用户数据
                pErrorCode 调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    备    注：  回调函数内不要调用Mcusdk的其它接口，也不要过长时间的阻塞
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2017/03/24  1.0     zsy                 创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetDrawCallBack(IN void *pMcuHandle, 
      IN u32 dwPlayID, IN McusdkDraw_Callback pfunc, IN KDM_PARAM userdata, 
      OUT u32 *pErrorCode);



    /******************************************************************************
    函 数 名：  Kdm_QueryCurrentUserInfoInt/Kdm_QueryCurrentUserInfoString
    功    能：  查询当前用户的信息和相关权限，数值型参数/字符串型参数
    参    数：  pMcuHandle    调用Kdm_CreateMcuSdk接口返回的非空handle    
                nQueryType    参考ECurrUserInfoType
                pValue/pBuf   返回数值/字符串的预先分配的内存指针
                bufSize       对应pBuf的大小
                pErrorCode    调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2017/04/17  1.0     wxh                 创建
    ******************************************************************************/    
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_QueryCurrentUserInfoInt(IN void *pMcuHandle, 
      IN ECurrUserInfoType nQeuryType, OUT int* pValue, OUT u32 *pErrorCode);
      
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_QueryCurrentUserInfoString(IN void *pMcuHandle, 
      IN ECurrUserInfoType nQeuryType, OUT char *pBuf, OUT int *bufSize, 
      OUT u32 *pErrorCode);
      
      
      
    /******************************************************************************
    函 数 名：  Kdm_SetAudioEncParam/Kdm_GetAudioEncParam
    功    能：  设置/获取语音呼叫音频编码参数
    参    数：  pMcuHandle   调用Kdm_CreateMcuSdk接口返回的非空handle
                param        音频编码参数，参见McusdkAudioEncodParam结构体
                pErrorCode   调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2017/04/17  1.0     wxh                 创建   
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetAudioEncParam(IN void *pMcuHandle, 
      IN McusdkAudioEncodParam param, OUT u32 *pErrorCode);
    
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetAudioEncParam(IN void *pMcuHandle, 
      OUT McusdkAudioEncodParam* param, OUT u32 *pErrorCode);
    


    /******************************************************************************
    函 数 名：  Kdm_QueryLockedRecord
    功    能：  查询锁定的平台录像
    参    数：  pMcuHandle   调用Kdm_CreateMcuSdk接口返回的非空handle
                tQryLockedRec查询条件，一次查询最多返回MAX_REQ_DEV_SUBS_NUM条结果
                ptQryResult  查询到的结果
                pErrorCode   调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2018/04/17  1.0     wxh                 创建   
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_QueryLockedRecord(IN void *pMcuHandle, 
	  IN McusdkQueryLockedRecord tQryLockedRec, OUT McusdkQryLockedRecResult* ptQryResult,
	  OUT u32 *pErrorCode);

    

	/******************************************************************************
    函 数 名：  Kdm_LockRecord
    功    能：  根据录像信息锁定或解锁平台录像，可通过Kdm_PreLoadRecord/Kdm_GetRecordNext
                获取平台录像信息
    参    数：  pMcuHandle   调用Kdm_CreateMcuSdk接口返回的非空handle
                ptMcuLockRec 录像信息，一次最多操作MAX_REQ_DEV_SUBS_NUM条录像信息
                bLock        传递TRUE时为锁定，传递FALSE时为解锁
                pErrorCode   调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2018/04/17  1.0     wxh                 创建   
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_LockRecord(IN void *pMcuHandle, 
	  IN McusdkLockRecord* ptMcuLockRec, IN BOOL32 bLock, OUT u32 *pErrorCode);



	/******************************************************************************
    函 数 名：  Kdm_LockRecordBatch
    功    能：  根据时间段锁定平台录像, 保证录像记录不被覆盖
    参    数：  pMcuHandle   调用Kdm_CreateMcuSdk接口返回的非空handle
                devChn       锁定录像的监控点
                tRange       锁定时间段
                pErrorCode   调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2018/04/17  1.0     wxh                 创建   
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_LockRecordBatch(IN void *pMcuHandle, 
	  IN DEVCHN devChn, IN TPeriod tRange, OUT u32 *pErrorCode);



	/******************************************************************************
    函 数 名：  Kdm_UnLockRecordBatch
    功    能：  根据时间段解锁平台录像
    参    数：  pMcuHandle      调用Kdm_CreateMcuSdk接口返回的非空handle
                devChn          锁定录像的监控点. 其中设备ID可以为空, 表示搜索所有锁定录像
                tRange          锁定时间段
                enableTimeRange 是否使用时间段参数. 不启用的话搜索所有时间段的锁定录像
                pErrorCode      调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2018/04/17  1.0     wxh                 创建   
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_UnLockRecordBatch(IN void *pMcuHandle, 
	  IN DEVCHN devChn, IN TPeriod tRange, IN BOOL32 enableTimeRange, OUT u32 *pErrorCode);



	/******************************************************************************
    函 数 名：  Kdm_BatchOperate
    功    能：  分组操作接口，支持平台录像、前端录像、雨刷、停止语音呼叫、
                前端载入预置位、锁定平台录像
    参    数：  pMcuHandle      调用Kdm_CreateMcuSdk接口返回的非空handle
                ptBatchOperate  批量操作类型以及相关参数
                pErrorCode      调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    支持平台：  平台2.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2018/04/17  1.0     wxh                 创建   
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_BatchOperate(IN void *pMcuHandle, 
	  IN McusdkBatchOperation* ptBatchOperate, OUT u32 *pErrorCode);
    


	/******************************************************************************
	函 数 名：  Kdm_SelectAudioChannel
	功    能：  选择启用或停用双路音频中的某一路
	参    数：  pMcuHandle 调用Kdm_CreateMcuSdk接口返回的非空handle
                dwPlayID   开始实时浏览时返回的播放ID
                audioChnID 选择的音频通道ID
                bEnable    是否启用
                pErrorCode 调用失败时带出来的错误码，通常成功调用后错误码的值为0
	返 回 值：  TRUE : 成功，FALSE : 失败
	备    注：  仅国标接入支持双路音频，码流是否包含双路音频会通过事件回调通知类型
	            emDoubleAudioChannel上报，之后可进行选择
	支持平台：  平台2.0
	-------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
	2017/08/17  1.0     zsy                 创建
	******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SelectAudioChannel(IN void *pMcuHandle, 
      IN u32 dwPlayID, IN u32 audioChnID, IN BOOL32 bEnable, OUT u32 *pErrorCode);


	
    /******************************************************************************
    函 数 名：  Kdm_GetDeviceInfobyDeviceId
    功    能：  获取单个设备完整信息
    参    数：  pMcuHandle  调用Kdm_CreateMcuSdk接口返回的非空handle
                devId       设备ID
                pDeviceInfo 预分配用于存储返回的设备完整信息
                pErrorCode  调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    备    注：  较老平台版本不支持该功能
    支持平台：  平台2.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2017/08/17  1.0     zsy                 创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetDeviceInfobyDeviceId(IN void *pMcuHandle, 
      IN const DeviceID devId, OUT DEVICEINFO* pDeviceInfo, OUT u32 *pErrorCode);
	  
	  
	  
	/******************************************************************************
    函 数 名：  Kdm_GetGroupInfo
    功    能：  获取单个组完整信息
    参    数：  pMcuHandle  调用Kdm_CreateMcuSdk接口返回的非空handle
                tGroupID    组ID
                tGroupInfo  存储返回的组完整信息
                pErrorCode  调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    备    注：  较老平台版本不支持该功能
    支持平台：  平台2.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2017/08/17  1.0     zsy                 创建
    ******************************************************************************/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetGroupInfo(IN void *pMcuHandle,
      IN GroupID tGroupID, OUT GROUPINFO &tGroupInfo, OUT u32* pErrorCode); 



    /******************************************************************************
    函 数 名：  Kdm_SetLogLevel
    功    能：  在Kdm_SetSaveLogFile_V2成功开启SDK日志后，可以调用此接口重设SDK内各
                个模块的日志等级，日志等级根据eLogLevel中定义值或操作组合，emLevAll
                表示开启所有打印类型
    参    数：  pMcuHandle      调用Kdm_CreateMcuSdk接口返回的非空handle
                dwMcusdkLev     重设mcusdk模块的日志等级
                dwUrlplaysdkLev 重设urlplaysdk模块的日志等级
                dwUniplayLev    重设uniplay模块的日志等级
                pErrorCode  调用失败时带出来的错误码，通常成功调用后错误码的值为0
    返 回 值：  TRUE:成功，FALSE:失败
    备    注：  无
    支持平台：  平台2.0、平台1.0
    -------------------------------------------------------------------------------
    修改记录：
    日  期      版本    修改人    走读人    修改内容
    2018/05/2   2.0     yss                 创建
    ******************************************************************************/
	KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetLogLevel(IN void *pMcuHandle,
		IN u32 dwMcusdkLev, IN u32 dwUrlplaysdkLev, IN u32 dwUniplayLev, 
		OUT u32 *pErrorCode );
    


    /******************************************************************************
          以下接口仅供内部CU2.0使用，接口文档中无接口说明，外部客户不建议使用
    ******************************************************************************/

    /*仅供平台内部使用
    开始和停止音频编码发送，CU在收到平台notify之后调用相应接口*/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_StartAudioEncSend(IN void *pMcuHandle, 
      IN VoiceCallNatEx tCallEx, IN EAudioEncType temAudioEncType, 
      OUT u32 *pErrorCode);
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_StopAudioEncSend(IN void *pMcuHandle, 
      OUT u32 *pErrorCode);
    
    /*仅供平台内部使用
    获取解码统计信息*/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetDecStatis(IN void *pMcuHandle, 
      IN u32 dwPlayID, OUT DecStatis *pDecStatis, OUT u32 *pErrorCode);

    /*仅供平台内部使用
    设置YUV信息回调。和YUV数据回调的区别在于返回结构体里的buf是空的
    只有YUV信息,没有YUV数据*/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetYUVInfoCallBack(IN void *pMcuHandle, 
      IN u32 dwPlayID, IN StreamYuv_Callback pfunc, IN void *userdata, 
      OUT u32 *pErrorCode);

    /*仅供平台内部使用
    设置浏览时是否支持大帧,默认不支持,设置后支持大于512KB的帧,但会消耗更多内存*/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetSupportLargeFrame(IN void *pMcuHandle, 
      IN BOOL32 bSupport, OUT u32 *pErrorCode);

    /*仅供平台内部使用
    可带多个参数的PTZ控制命令, 命令值将默认和平台2.0的PTZ控制命令保持一致
    参数个数不限制, 由 pPtzParam 和 nParamNum决定,每个参数都是一个int型*/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_PtzControl(IN void *pMcuHandle, 
      IN DEVCHN tDevChn, IN EMcusdkPtzCmd ptzCmd, IN int *pPtzParam, 
      IN int nParamNum, OUT McusdkPtzLockInfo *pLockInfo, OUT u32 *pErrorCode);

    /*仅供平台内部使用
    暂停解码*/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_PauseDecode(IN void *pMcuHandle, 
      IN u32 dwPlayID, IN BOOL32 bPause, OUT u32 *pErrorCode);

	/*仅供平台内部使用
    CU指定InternalPlayID的播放接口*/
	KDM_MCUSDK_API u32 STDCALL Kdm_StartRealPlay_V4(IN void *pMcuHandle,
		IN DEVCHN tDevChn, IN RealplayParam tStreamParam, IN CB_SDKMEDIA_V2 tCbSdkMedia, 
		IN int nInternalPlayID, OUT u32 *pErrorCode);

    /*仅供平台内部使用
    获取一路播放的内部播放ID*/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetInternalPlayID(IN void *pMcuHandle, 
      IN u32 dwPlayID, OUT int *pnInternalPlayID, OUT u32 *pErrorCode);

    /*仅供平台内部使用
    出现进程异常退出时使用内部播放ID断开平台码流
    如果创建这路播放的进程还存在，禁止使用该接口来断开码流*/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_StopPlayByInternalPlayID(IN void *pMcuHandle, 
      IN int nInternalPlayID, OUT u32 *pErrorCode);

    /*仅供平台内部使用
    获取本路实时播放码流的详细调试信息打印文本，返回实际所需要的字节数*/
    KDM_MCUSDK_API int STDCALL Kdm_GetStreamDebugString(IN void *pMcuHandle, 
      IN u32 dwPlayID, OUT char *pDebugString, IN int nBufSize, OUT u32 *pErrorCode);
    
    /*仅供平台内部使用
    获取任务守望*/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetTaskWatcher(IN void *pMcuHandle,
      IN DEVCHN tDevChn, OUT TWatchTask *ptWatchTask, OUT u32 *pErrCode);
      
    /*仅供平台内部使用
    获取预置位巡视*/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetPrePosWatcher(IN void *pMcuHandle,
      IN DEVCHN tDevChn, OUT TWatchPrePos *ptWatchPrePos, OUT u32 *pErrCode);

    /*仅供平台内部使用
    获取预置位别名
    itemCount表示预置位别名数组的项数,presetNames预置位别名数组（使用UTF8编码）
    返回值为实际填入presetNames的别名个数，-1表示出错*/
    KDM_MCUSDK_API int STDCALL Kdm_GetPtzPresetName(IN void *pMcuHandle, 
      IN DEVCHN devChn, OUT McusdkPresetName *presetNames, IN int itemCount, 
      OUT u32 *pErrorCode);

    /*仅供平台内部使用
    设置预置位别名，presetName 预置位别名（使用UTF8编码）*/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetPtzPresetName(IN void *pMcuHandle, 
      IN DEVCHN devChn, IN McusdkPresetName presetName, OUT u32 *pErrorCode);

    /*仅供平台内部使用
    删除预置位别名，nPresetId预置位Id号*/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_DelPtzPresetName(IN void *pMcuHandle, 
      IN DEVCHN devChn, IN int nPresetId, OUT u32 *pErrorCode);

    /*仅供平台内部使用
    设置实时码流GPS信息回调函数，用于提取码流中的GPS数据，非平台信令获取*/	    
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetGPSDataCallBack(IN void *pMcuHandle, 
      IN PMCUSDK_GPS_CALLBACK pfCB, IN void *pUserData, OUT u32 *pErrorCode);
    
    /*仅供平台内部使用
    创建查询告警日志的任务*/
    KDM_MCUSDK_API u32 STDCALL Kdm_CreateQueryAlarmLogTask(IN void *pMcuHandle, 
      IN McusdkAlarmlogReq tAlarmReq, OUT u32* pdwTotal, OUT u32 *pErrorCode);
    
    /*仅供平台内部使用
    获取告警日志内容*/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetAlarmLogNext(IN void *pMcuHandle, 
      IN u32 dwTaskID, OUT McusdkAlarmLogItem* ptLogItem, OUT u32 *pErrorCode);
      
    /*仅供平台内部使用
    销毁查询告警日志的任务*/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_DestroyQueryAlarmLogTask(IN void *pMcuHandle, 
      IN u32 dwTaskID, OUT u32 *pErrorCode);
      
    /*仅供平台内部使用
    开启即时内存录像
    nReserve 保留字段用来设置内存录像时长（单位秒），目前暂时填0，内部默认30秒
    内存录像会在后台一直进行，内存录像总是从实时浏览的当前时间点回退30秒的内容*/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_StartMemoryRecord(IN void *pMcuHandle, 
      IN u32 dwPlayID, IN int nReserve, OUT u32 *pErrorCode);
      
    /*仅供平台内部使用
    将内存录像存盘
    outFullFileName将当前的内存录像存盘成一个磁盘录像文件的文件名
    如果开启内存录像的时间还没有达到或超过30秒，那么保存下来的内存录像也将不足30秒
    如果已达到或经超过30秒了，那么存下来的内存录像将会是当前时间点回退30秒的录像内容
    当保存好内存录像后，后台的内存录像还是会继续进行的，可以下次再次将内存录像存盘*/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SaveMemoryRecord(IN void *pMcuHandle, 
      IN u32 dwPlayID, IN char *outFullFileName, OUT u32 *pErrorCode);
      
    /*仅供平台内部使用
    停止即时内存录像接口，由于底层库并未提供该接口，所以目前调用此接口无效*/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_StopMemoryRecord(IN void *pMcuHandle, 
      IN u32 dwPlayID, OUT u32 *pErrorCode);
      
   	/*仅供平台内部使用
    返回内存录像已经缓存的视频帧数,数值型，表示已经缓存的视频帧数，返回-1表示出错*/
    KDM_MCUSDK_API int STDCALL Kdm_GetMemoryRecordBufferSizeInFrames(IN void *pMcuHandle, 
      IN u32 dwPlayID, OUT u32 *pErrorCode);

    /*仅供平台内部使用
    丢弃ADPCM混音，这样客户端就听不到混音内容，但是不影响本地录像里的混音内容*/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_DropADPCMAudio(IN void *pMcuHandle, 
      IN u32 dwPlayID, IN BOOL32 bDrop, OUT u32 *pErrorCode);
      
    /*仅供平台内部使用
    获取制定厂商名的插件版本号, szVersion要求大小至少为32字节*/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetDecoderVersion(IN void *pMcuHandle, 
      IN char *manufactor, OUT char *szVersion, IN int nBufSize, OUT u32 *pErrorCode);

    /*仅供平台内部使用
    PTZ锁定和解锁接口
    锁定接口如果平台返回失败，失败码通过 McusdkPTZLockRsp返回，函数本身还是返回成功*/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_LockPTZ(IN void *pMcuHandle, IN McusdkPTZLockReq req, 
      OUT McusdkPTZLockRsp *pRsp, OUT u32 *pErrorCode);
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_UnLockPTZ(IN void *pMcuHandle, 
      IN McusdkPTZUnLockReq req, OUT McusdkPTZUnLockRsp *pRsp, OUT u32 *pErrorCode);
      
    /*仅供平台内部使用
    开启解码垂直同步*/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_EnableVerticalSync(IN void *pMcuHandle, 
      IN u32 dwPlayID, IN BOOL32 bEnable, OUT u32 *pErrorCode);
      
    /*仅供平台内部使用
    获取告警类型信息*/  
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetAlarmTypesInfo(IN void *pMcuHandle, 
      OUT McusdkAlarmTypesInfo* pAlarmInfo, OUT u32 *pErrorCode);
      
    /*仅供平台内部使用
    获取告警联动配置信息*/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetAlarmLinkConfig(IN void *pMcuHandle,
      IN McusdkAlarmSource tAlarmsource, OUT McusdkAlarmLinkConfig* ptAlarmLinkConfig, 
      OUT u32 *pErrorCode);
	  
	//按组手工 开启/停止 平台录像
	KDM_MCUSDK_API BOOL32 STDCALL Kdm_StartGroupPlatRec(IN void *pMcuHandle, IN GroupID grpId, 
	  IN bool bRecursive, OUT u32* pErrorCode);
	KDM_MCUSDK_API BOOL32 STDCALL Kdm_StopGroupPlatRec(IN void *pMcuHandle, IN GroupID grpId, 
	  IN bool bRecursive, OUT u32* pErrorCode);
	//按组手工 开启/停止 前端录像
	KDM_MCUSDK_API BOOL32 STDCALL Kdm_StartGroupPuRec(IN void *pMcuHandle, IN GroupID grpId, 
	  IN bool bRecursive, OUT u32* pErrorCode);
	KDM_MCUSDK_API BOOL32 STDCALL Kdm_StopGroupPuRec(IN void *pMcuHandle, IN GroupID grpId, 
	  IN bool bRecursive, OUT u32* pErrorCode);

	//获取/设置前端录像计划参数
	KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetRecordPlan(IN void *pMcuHandle, IN DEVCHN devChn, 
	  OUT McusdkRecordPlan *plan, OUT u32* pErrorCode);
	KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetRecordPlan(IN void *pMcuHandle, IN DEVCHN devChn, 
	  IN McusdkRecordPlan *plan, OUT u32* pErrorCode);
	
	//获取NRU列表
	/*参看
	Kdm_GetNruList
	Kdm_GetNruListTotal
	*/
	//获取NRU磁盘列表
	KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetDiskListTotal(IN void *pMcuHandle, IN const char *nruId, 
	  OUT int *pOutNum, OUT u32* pErrorCode);
	//pnNum 填入需要获取的项数，返回实际写入ppNruInfo的项数
	KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetDiskList(IN void *pMcuHandle, IN const char *nruId, 
	  IN McusdkDIskInfo *ppDiskInfo, IN int nStartIndex, IN OUT int *pnNum, OUT u32* pErrorCode);
	
	//添加删除分区绑定关系
	KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetDeviceDiskBindInfoList(IN void *pMcuHandle,
		IN const char *nruId, IN const char *diskMountPath,	//nruid和磁盘挂载路径
		IN int nStartIndex,		//分页查询的起始索引
		//pnNum:输入表示本次查询的分页数同时也表示ppDevChns指向的DEVCHN项数
		//输出表示实际填入ppDevChns的DEVCHN项数
		IN OUT int *pnNum,  OUT DEVCHN *ppDevChns,	
		
		OUT int *pTotalNum,		//总数
		OUT u32* pErrorCode);

	KDM_MCUSDK_API BOOL32 STDCALL Kdm_BindDeviceDisk(IN void *pMcuHandle,
		IN const char *nruId, IN const char *diskMountPath,	//nruid和磁盘挂载路径		
		IN DEVCHN *ppDevChns,	IN int nChnNum,		//需要绑定到上面磁盘路径的通道列表
		OUT u32* pErrorCode);

	KDM_MCUSDK_API BOOL32 STDCALL Kdm_UnBindDeviceDisk(IN void *pMcuHandle,
		IN const char *nruId, IN const char *diskMountPath,	//nruid和磁盘挂载路径		
		IN DEVCHN *ppDevChns, IN int nChnNum,		//需要绑定到上面磁盘路径的通道列表
		OUT u32* pErrorCode);

	//区分时区和夏令时的同步录像查询接口。（主要给cu使用）	
	//pNum,入：表示records里已分配的项数，出，表示实际查询到的总数。
	KDM_MCUSDK_API BOOL32 STDCALL Kdm_RecordQuery(IN void *pMcuHandle, 
	  IN McusdkQueryRecordReq *recQueryReq, OUT TRecordInfo *records, 
	  IN OUT int *pNum, OUT u32* pErrorCode);

	//设置本地码流加流水印接口
	KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetWaterMark(IN void *pMcuHandle, IN u32 dwPlayID, 
	  IN McusdkWaterMark *pWaterMarkInfo, OUT u32* pErrorCode);

	//获取当前用户的码流水印参数，单纯获取是否启用水印功能
	//也可以用枚举 em_UserWaterMarkEnable 调用 Kdm_QueryCurrentUserInfoInt 接口查询
	KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetUserWaterMark(IN void *pMcuHandle, 
	  OUT McusdkUserWaterMark *pWaterMarkInfo, OUT u32* pErrorCode);

	//支持RPCTRL下载，该功能仅支持科达自己的录像
	//给rpctrl下载设置超时检测参数
	KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetRpctrlTimeOutParam(IN void *pMcuHandle, 
	  IN u32 dwInterval, IN u32 dwRepeatCount, OUT u32* pErrorCode);
	KDM_MCUSDK_API u32 STDCALL Kdm_StartRpctrlDownLoad(IN void *pMcuHandle, 
	  IN TRecordInfo *recInfo, IN const char *localfile, IN RpctrlParam *ptParam, OUT u32* pErrorCode);
	KDM_MCUSDK_API BOOL32 STDCALL Kdm_StopRpctrlDownLoad(IN void *pMcuHandle, 
	  IN u32 dwPlayID, OUT u32* pErrorCode);
	//获取下载进度，返回pCurTime已经下载的录像时长毫秒数，pTotalTime本次下载整段录像的时长毫秒数
	//当pCurTime == pTotalTime时表示下载完成
	//如果发生了错误，函数返回FALSE， 错误码请看pErrorCode
	KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetRpctrlDownLoadProgress(IN void *pMcuHandle, 
	  IN u32 dwPlayID, OUT int *pCurTime, OUT int *pTotalTime, OUT u32* pErrorCode);

	//本地录像播放设置是否关键帧解码
	KDM_MCUSDK_API BOOL32 STDCALL Kdm_EnableKeyFramePlay(IN void *pMcuHandle, IN u32 dwPlayID, 
	  IN BOOL32 bEnable, OUT u32* pErrorCode);
	//本地录像文件，获取音频轨道数
	KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetAudioTrackNum(IN void *pMcuHandle, IN u32 dwPlayID, 
	  OUT int *pTrackNum, OUT u32* pErrorCode);
	
	//开启ssl安全模式, 
	//bSafeSignal指是否启用信令加密
	//ptSafeInfo指定是否启用码流加密，为将来预留需要进行双向认证的时候扩充用
	KDM_MCUSDK_API BOOL32 STDCALL Kdm_EnableSafeMode(IN void *pMcuHandle, IN BOOL32 bSafeSignal, IN McusdkSafeModeInfo *ptSafeInfo, OUT u32 *pErrorCode);
	//开启或停止前端设备的码流签名控制.
	KDM_MCUSDK_API BOOL32 STDCALL Kdm_EnableDeviceSignatrue(IN void *pMcuHandle, IN const DeviceID devId, IN bool bEnable, OUT u32 *pErrorCode);
	//开启或停止前端设备的码流加密控制.
	KDM_MCUSDK_API BOOL32 STDCALL Kdm_EnableDeviceEncryption(IN void *pMcuHandle, IN const DeviceID devId, IN bool bEnable, OUT u32 *pErrorCode);

	
	//获取当前播放录像的时长范围（边录边放的时候用于修正录像文件范围用）
	//如果是倒放，请不要调用该接口，该接口不支持倒放
	//如果倒放仍然强制调用此接口获取参数，那么参数的含义我们无法给出。
	KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetRecordRange(IN void *pMcuHandle, IN u32 dwPlayID, 
	  OUT TPeriod *prange, OUT u32 *pcurTime, OUT u32 *pErrorCode);
	  
	KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetRCSSysParam_V2(IN void *pMcuHandle,
      IN McusdkRCSSysParam tRcsParam, OUT u32 *pErrorCode);
	  
	KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetRCSSysParam_V2(IN void *pMcuHandle,
      OUT McusdkRCSSysParam* pRcsParam, OUT u32 *pErrorCode);  
    
    //直接指定录像文件进行下载, nSpeed表示下载倍速，0表示默认，其它可取值2,4,8,16,
	KDM_MCUSDK_API u32 STDCALL Kdm_StartRecordDownLoad_V3(IN void *pMcuHandle,
      IN TRecordInfo *recInfo, IN TPeriod *downloadRange,  IN const char *localfile,
      IN int nSpeed, OUT u32 *pErrorCode);

	//合并多个录像到一个录像，该接口不支持多实例也不支持多线程，同一进程同一时间只能有一份
	//仅windows下有效
	KDM_MCUSDK_API BOOL32 STDCALL Kdm_CombineRecordFiles(IN void *pMcuHandle,
		IN const char **inputFileList, IN int nFileNum, IN const char *outputFile, 
		IN Mcusdk_Combine_CallBack pfn, IN void *userData, OUT u32 *pErrorCode);

	KDM_MCUSDK_API void STDCALL Kdm_CancelCombineRecordFiles(IN void *pMcuHandle);	
      
	//批量获取设备的国标ID
	KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetDeviceGBIDBatch(IN void *pMcuHandle, IN OUT DeviceID2GBIDItem *devIds, IN int devNum/*<=100*/,  OUT u32 *pErrorCode);

    /******************************************************************************
                         以下接口废弃，无特殊需要请勿使用
    ******************************************************************************/

    /*该接口废弃*/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetHighDefinitionValue(IN void *pMcuHandle);

    /*该接口废弃 可使用浏览接口中的m_wHighDefinition字段或URL回调*/
    KDM_MCUSDK_API void STDCALL Kdm_SetHighDefinitionValue(IN void *pMcuHandle,
      IN BOOL32 temHighDefinition);

    /*该接口废弃*/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetFeatureCode(IN void *pMcuHandle, 
      IN const char* pchFeatureCode);

    /*该接口废弃，可使用新搜索接口*/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SearchDvcReq(IN void *pMcuHandle, 
      IN const char* SearchDvcName,IN int SearchResLimit, OUT u32* pErrorCode);

    /*该接口废弃，可使用新搜索接口*/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetSearchDvcInfo(IN void *pMcuHandle,
      OUT StSearchDvcRspInfo &temSearchDvcInfo,OUT u32* pErrorCode);

    /*该接口废弃，获取音视频采样率使用Kdm_GetStreamSampleRate接口*/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetPlayingChnInfo(IN void *pMcuHandle,
      IN u32 dwPlayID, OUT TTPlyingChnAVInfo& tPlayingChnInfo, OUT u32* pErrorCode);

    /*该接口废弃*/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SetDecodeColor(IN void *pMcuHandle,
      IN u32 dwPlayID, IN int nRegionNum, IN int nBrightness,
      IN int nContrast, IN int nSaturation, IN int nHue, OUT u32 *pErrorCode); 
      
    /*该接口废弃  可使用Kdm_StartRealPlay_V2 Kdm_StartRealPlay_V3*/
    KDM_MCUSDK_API u32 STDCALL Kdm_StartRealPlay(IN void *pMcuHandle, 
      IN DEVCHN tDevChn, IN SPARAM tStreamParam, IN CB_SDKMEDIA tCbSdkMedia, 
      OUT u32 *pErrorCode);   
    
    /*该接口废弃*/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetGbidByGbid(IN void *pMcuHandle,  
      IN DEVCHN tGbid, OUT DEVCHN* ptOldGbid, OUT u32 *pErrorCode);

    /*该接口废弃*/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetNewGbidByOldGbid(IN void *pMcuHandle,  
        IN DEVCHN tOldGbid, OUT DEVCHN* ptNewGbid, OUT u32 *pErrorCode);
    
    /*该接口废弃*/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_DeleteRecord(IN void *pMcuHandle, 
      IN const char *domainName, IN const DEVCHN chn, IN const TPeriod timeRange, 
      OUT u32 *pErrorCode);
      
    /*该接口废弃  可使用Kdm_GetSdkVersion_V2*/  
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetSdkVersion(IN void *pMcuHandle, 
      OUT char* pchVersion, OUT char *pchUpdateurl);
    
    /*该接口废弃  可使用Kdm_SendTransData_V3*/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SendTransData(IN void *pMcuHandle, 
      IN DEVCHN tDevChn, IN TTransChnData *pTransData, OUT u32 *pErrorCode);
    
    /*该接口废弃  可使用Kdm_SendTransData_V3*/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_SendTransData_V2(IN void *pMcuHandle, 
      IN DEVCHN tDevChn, IN TTransChnData_V2 *pTransData, OUT u32 *pErrorCode);
    
    /*该接口废弃  可使用Kdm_GetPuEncoderParam_V2*/
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetPuEncoderParam(IN void *pMcuHandle, 
      IN DEVCHN tDevChn, OUT TEncoderParamTotal& pEncoderParamTotal,
      OUT u32* pErrorCode);
    
    /*该接口废弃  可使用Kdm_GetDevGpsInfo_V2*/ 
    KDM_MCUSDK_API BOOL32 STDCALL Kdm_GetDevGpsInfo(IN void *pMcuHandle,
      IN DEVCHN tDevChn, OUT DeviceGPSInfo& tDevGpsInfo, OUT u32* pErrorCode);
      

#ifdef __cplusplus
}
#endif



#endif
