/**
* @file         dataswitch.h
* @brief        data switch
* @details      数据转发
* @author       顾晓康
* @date         2016/06/20
* @version      2016/06/20
* @par Copyright (c):
*    kedacom
* @par History:
*   version:
*/

#ifndef DATASWITCH_H        ///< 头文件宏,防止重包含
#define DATASWITCH_H        ///< 头文件宏,防止重包含

#define DSVERSION       "Dataswitch_64bit v1.1.0 2008.4.24" ///< 版本


#include "osp.h"

/** DataSwitch 句柄 */
#define DSID    u32

/** DataSwitch 返回值*/
#define DSOK    1
#define DSERROR 0

#define INVALID_DSID  0xffffffff   //<无效的句柄值


/**
 * 地址类型结构
 * dwIP : IP地址,网络序
 * wPort : 端口,主机序
 */
typedef struct
{
	u32 dwIP;
	u16 wPort;
}TDSNetAddr;

/**
 * UDP 报文接收回调函数类型
 * pPackBuf : 报文缓存地址
 * wPackLen : 报文长度
 * ptDstAddr : 本地接收地址
 * ptSrcAddr : 报文源地址
 * qwTimeStamp : 时间戳
 * dwContext : 用户回调上下文
 */
typedef void* (*DSUDPPACKCALLBACK)(u8* pPackBuf, u16 wPackLen, TDSNetAddr* ptDstAddr, TDSNetAddr* ptSrcAddr, u64 qwTimeStamp, void* pvContext);


/**
 *@brief 设置接收报文回调
 *@param[in]   const TDSNetAddr* ptRcvAddr 接收地址
 *@param[in]   DSUDPPACKCALLBACK pUdpCallBack 回调函数
 *@param[in]   void* pvContext 回调上下文
 *@return      DSOK:成功 错误码:失败
 *@ref
 *@note
 */
API u32 dsRegRcvChannel(const TDSNetAddr* ptRcvAddr, DSUDPPACKCALLBACK pUdpCallBack, void* pvContext);


 /**
 *@brief 移除接收报文回调
 *@param[in]   const TDSNetAddr* ptRcvAddr 接收地址
 *@param[in]   DSUDPPACKCALLBACK pUdpCallBack 回调函数
 *@param[in]   void* pvContext 回调上下文
 *@return      DSOK:成功 错误码:失败
 *@ref
 *@note
 */
API u32 dsUnRegRcvChannel(const TDSNetAddr* ptRcvAddr, DSUDPPACKCALLBACK pUdpCallBack, void* pvContext);


/**
 *@brief 报文发送函数
 *@param[in]   pPackBuff : 报文缓存
 *@param[in]   dwPackLen : 报文长度
 *@param[in]   ptSrcAddr : 源地址
 *@param[in]   ptDstAddr : 目的地址
 *@return      DSOK:成功 错误码:失败
 *@ref
 *@note
 */
API s32 dsUDPPackSend(u8 *pPackBuff, u32 dwPackLen, TDSNetAddr* ptSrcAddr,TDSNetAddr* ptDstAddr );


/**
 * @func FilterFunc
 * @brief 接收过滤函数
 *
 * 每个接收结点有一个过滤函数，当此结点接收到UDP数据包时执行此函数。
 * 并根据函数的返回值，来动态地决定是否对此数据包转发。
 *
 * @param ptRecvAddr        - 接收地址
 * @param ptSrcAddr         - 源地址
 * @param pData          - [in, out]数据包；可以修改；
 * @param uLen           - [in, out]数据包长度；修改后的长度不可超过最大值
 * @return DSOK, 该数据包有效； 其他值，该数据包无效；
 */
typedef u32 (*FilterFunc)(u32 dwRecvIP, u16 wRecvPort, u32 dwSrcIP, u16 wSrcPort, u8* pData, u32 uLen);


#define SENDMEM_MAX_MODLEN        (u8)32        ///<发送时允许的最大修改长度


/** 发送组定义*/
typedef struct tagNetSndMember
{
	TDSNetAddr  tDstAddr;               //<转发目的地址
    s64         lIdx;                   //<接口索引
    u32         errNum;                 //<出错次数
    s64         errNo;                  //<错误号
    void *      lpuser;                 //<user info
    u16         wOffset;                //<修改部分的起始位置; 目前未使用
    u16         wLen;		            //<修改部分的长度，必须为4的倍数；为零表示不修改
    s8          pNewData[SENDMEM_MAX_MODLEN]; //<用户自定义数据
    void *      pAppData;                     //<用户自定义的回调函数的参数
}TNetSndMember;


/**
 *@brief  发送数据回调
 *@param[in]  u32 dwRecvIP  接收组ip
 *@param[in]  u16 wRecvPort 接收组端口
 *@param[in]  u32 dwSrcIP   源ip
 *@param[in]  u16 wSrcPort  源端口
 *@param[in]  TNetSndMember *ptSends    转发目标列表
 *@param[in]  u16* pwSendNum            转发目标个数
 *@param[in]  u8* pUdpData      数据报文
 *@param[in]  u32 dwUdpLen      报文长度
 *@return
 *@ref
 *@see
 *@note
 */
typedef void (*SendCallback)(u32 dwRecvIP, u16 wRecvPort, u32 dwSrcIP, u16 wSrcPort, TNetSndMember *ptSends,
                             u16* pwSendNum,
                             u8* pUdpData, u32 dwUdpLen);


/**
 *@brief 设置接口超时时长
 *@param[in]   s32 timeout : 时长,单位:ms
 *@return
 *@ref
 *@note
 */
API void dsSetApiTimeOut(s32 timeout);


/**
 *@brief 配置ds能力集
 *@param[in]   u32 dwMaxRcvGrp  最大接收组数目
 *@param[in]   u32 dwMaxSndMmbPerRcvGrp  单个接收组可以配置的转发组数目
 *@return      DSOK:成功 错误码:失败，
 *@ref
 *@see
 *@note
 */
API u32 dsSetCapacity( u32 dwMaxRcvGrp , u32 dwMaxSndMmbPerRcvGrp );


/**
 *@brief 创建DataSwitch模块
 *@param[in]   u32 num      接口IP数组个数
 *@param[in]   u32 adwIP[]  接口IP数组
 *@param[in]   const char* pszSendIf  指定的发送设备名称
 *@return      DSID:用户id,失败返回INVALID_DSID
 *@ref
 *@see
 *@note
 */
API DSID dsCreate( u32 num, u32 adwIP[], const char* pszSendIf = NULL );


/**
 *@brief 销毁DataSwitch模块
 *@param[in]   dsId  - 用户标识
 *@return
 *@ref
 *@see
 *@note
 */
API void dsDestroy( DSID dsId );


/**
 *@brief 增加转发目标唯一的转发规则
 *@param[in]   dsId         用户标识
 *@param[in]   tRecvAddr    接收地址，即接收数据包的目的IP
 *@param[in]   dwInLocalIP  接收数据包的网络接口
 *@param[in]   tSendToAddr  转发目的地址
 *@param[in]   dwOutLocalIP 转发数据包所用本地IP
 *@return       DSOK: 表示成功 ; DSERROR: 表示失败
 *@ref
 *@see
 *@note 如果已经存在的交换规则中转发目标与当前规则的相同，则先删除这些规则。
 */
API u32 dsAdd(DSID dsId, TDSNetAddr tRecvAddr, u32 dwInLocalIP, TDSNetAddr tSendToAddr, u32 dwOutLocalIP = 0);


/**
 *@brief 删除转发目标为指定地址的转发规则
 *@param[in]   dsId         用户标识
 *@param[in]   tSendToAddr  转发目的地址
 *@return       DSOK: 表示成功 ; DSERROR: 表示失败
 *@ref
 *@see
 *@note
 */
API u32 dsRemove(DSID dsId, TDSNetAddr tSendToAddr);


/**
 *@brief 增加Dump规则
 *@param[in]   dsId          用户标识
 *@param[in]   tRecvAddr     接收地址，即接收数据包的目的IP和Port
 *@param[in]   dwInLocalIP   接收数据包的网络接口
 *@return       DSOK: 表示成功 ; DSERROR: 表示失败
 *@ref
 *@see
 *@note 从指定地址接收的数据包不被转发。如果一个接收地址只有DUMP规则，则接收的数据包不被转发；
 *      如果还有其他转发规则，则按照其他规则转发。
 */
API u32 dsAddDump(DSID dsId, TDSNetAddr tRecvAddr, u32 dwInLocalIP);


/**
 *@brief 删除Dump规则
 *@param[in]   dsId          用户标识
 *@param[in]   tRecvAddr     接收地址，即接收数据包的目的IP和Port
 *@return       DSOK: 表示成功 ; DSERROR: 表示失败
 *@ref
 *@see
 *@note
 */
API u32 dsRemoveDump(DSID dsId, TDSNetAddr tRecvAddr);


/**
 *@brief 增加多对一的转发规则
 *@param[in]    dsId          用户标识
 *@param[in]    tRecvAddr     接收地址，即接收数据包的目的IP和Port
 *@param[in]    dwInLocalIP   接收数据包的网络接口
 *@param[in]    tSendToAddr   转发目的IP和Port
 *@param[in]    dwOutLocalIP  转发数据包所用本地IP
 *@return       DSOK: 表示成功 ; DSERROR: 表示失败
 *@ref
 *@see
 *@note
 */
API u32 dsAddManyToOne(DSID dsId , TDSNetAddr tRecvAddr, u32  dwInLocalIP, TDSNetAddr tSendToAddr, u32  dwOutLocalIP = 0);


/**
 *@brief 删除所有转发目标为指定地址的多对一规则
 *@param[in]    dsId          用户标识
 *@param[in]    tSendToAddr   转发目的IP和Port
 *@return       DSOK: 表示成功 ; DSERROR: 表示失败
 *@ref
 *@see
 *@note
 */
API u32 dsRemoveAllManyToOne(DSID dsId , TDSNetAddr tSendToAddr);


/**
 *@brief 删除指定的多对一规则
 *@param[in]    dsId          用户标识
 *@param[in]    tRecvAddr     接收地址，即接收数据包的目的IP和Port
 *@param[in]    tSendToAddr   转发目的IP和Port
 *@return       DSOK: 表示成功 ; DSERROR: 表示失败
 *@ref
 *@see
 *@note
 */
API u32 dsRemoveManyToOne(DSID dsId , TDSNetAddr tRecvAddr, TDSNetAddr tSendToAddr);


/**
 *@brief 增加按源转发的规则
 *@param[in]    dsId          用户标识
 *@param[in]    tRecvAddr     接收地址，即接收数据包的目的IP和Port
 *@param[in]    dwInLocalIP   接收数据包的网络接口
 *@param[in]    tSrcAddr      接收数据包的源IP和Port
 *@param[in]    tSendToAddr   转发目的IP和Port
 *@param[in]    dwOutLocalIP  转发数据包所用本地IP
 *@return       DSOK: 表示成功 ; DSERROR: 表示失败
 *@ref
 *@see
 *@note 使用此规则时，接收数据包的源地址必须与规则中的源
 *     地址相等。每次转发时，都要先根据数据包中的源地址查找转发规
 * 则，如果找不到，则使用默认源地址（即0@0）的转发规则。
 * 注意：该接口支持多点到一点。如果dwSrcIP和wSrcPort都为零，
 * 它就完全等同于dsAddManyToOne。
 */
API u32 dsAddSrcSwitch(DSID dsId , TDSNetAddr tRecvAddr, u32  dwInLocalIP, TDSNetAddr tSrcAddr, TDSNetAddr tSendToAddr, u32  dwOutLocalIP = 0);


/**
 *@brief 删除所有指定的按源转发规则
 *@param[in]    dsId          用户标识
 *@param[in]    tRecvAddr     接收地址，即接收数据包的目的IP和Port
 *@param[in]    tSrcAddr      接收数据包的源IP和Port
 *@return       DSOK: 表示成功 ; DSERROR: 表示失败
 *@ref
 *@see
 *@note
 */
API u32 dsRemoveAllSrcSwitch(DSID dsId, TDSNetAddr tRecvAddr, TDSNetAddr tSrcAddr);


/**
 *@brief 删除指定的按源转发的规则
 *@param[in]    dsId          用户标识
 *@param[in]    tRecvAddr     接收地址，即接收数据包的目的IP和Port
 *@param[in]    tSrcAddr      接收数据包的源IP和Port
 *@param[in]    tSendToAddr   转发目的IP和Port
 *@return       DSOK: 表示成功 ; DSERROR: 表示失败
 *@ref
 *@see
 *@note
 */
API u32 dsRemoveSrcSwitch(DSID dsId, TDSNetAddr tRecvAddr,  TDSNetAddr tSrcAddr,  TDSNetAddr tSendToAddr);


/**
 *@brief 设置用户数据
 *@param[in]    dsId          用户标识
 *@param[in]    tLocalAddr    接收地址，即接收数据包的目的IP和Port
 *@param[in]    dwInLocalIP    接收数据包的网络接口
 *@param[in]    tSrcAddr      接收数据包的源IP和Port
 *@param[in]    tDstAddr      目的IP和Port
 *@param[in]    dwOutLocalIP  转发数据包所用本地IP
 *@param[in]    bSend       是否需要发送
 *@param[in]    pbyUserData   用户数据buffer
 *@param[in]    nUserLen      用户数据长度
 *@return       DSOK: 表示成功 ; DSERROR: 表示失败
 *@ref
 *@see
 *@note
 */
API u32 dsSetUserData(DSID dsId,
					  TDSNetAddr tLocalAddr,
					  u32        dwLocalIfIp,
					  TDSNetAddr tSrcAddr,
					  TDSNetAddr tDstAddr,
					  u32        dwDstOutIfIP,
					  BOOL32      bSend,
                      u8* pbyUserData,
                      s32 nUserLen);


/**
 *@brief 设置过滤函数
 *@param[in]    dsId          用户标识
 *@param[in]    tRecvAddr     接收地址，即接收数据包的目的IP和Port
 *@param[in]    ptFunc        过滤函数指针
 *@return       DSOK: 表示成功 ; DSERROR: 表示失败
 *@ref
 *@see
 *@note 函数指针可以为空，表示清除原有的设置；另外，由于
 * DataSwitch是在另外的线程中调用此函数的，要设置的函数必
 * 须是全局函数；设置的参数，也必须是全局有效的。
 */
API u32 dsSetFilterFunc(DSID dsId, TDSNetAddr tRecvAddr, FilterFunc ptFunc);


/**
 *@brief 删除所有的转发规则
 *@param[in]    dsId          用户标识
 *@param[in]    bKeepDump     是否保留所有的dump规则，默认不保留
 *@return       DSOK: 表示成功 ; DSERROR: 表示失败
 *@ref
 *@see
 *@note
 */
API u32 dsRemoveAll( DSID dsId, BOOL32 bKeepDump = FALSE );


/**
 *@brief 为指定接收地址设置转发数据包所填充的源地址(基于接收组伪装转发源)
 *@param[in]    dsId          用户标识
 *@param[in]    tOrigAddr     原始IP和Port
 *@param[in]    tMappedAddr   映射IP和Port
 *@return       DSOK: 表示成功 ; DSERROR: 表示失败
 *@ref
 *@see
 *@note
 */
API u32 dsSpecifyFwdSrc(DSID dsId, TDSNetAddr tOrigAddr, TDSNetAddr tMappedAddr);


/**
 *@brief 恢复指定地址转发数据包的源地址
 *@param[in]    dsId         用户标识
 *@param[in]    tOrigAddr    原始IP和Port
 *@return       DSOK: 表示成功 ; DSERROR: 表示失败
 *@ref
 *@see
 *@note
 */
API u32 dsResetFwdSrc(DSID dsId, TDSNetAddr tOrigAddr);


/**
 *@brief 根据数据报文目的地址和接收地址填充转发的源地址(基于某一转发通道伪装转发源)
 *@param[in]    dsId         用户标识
 *@param[in]    tRcvAddr    接收的IP和Port
 *@param[in]    tDstAddr    目标IP和Port
 *@param[in]    tRawAddr    映射的IP和Port
 *@return       DSOK: 表示成功 ; DSERROR: 表示失败
 *@ref
 *@see
 *@note
 */
API u32 dsSetSrcAddrByDst(DSID dsId, TDSNetAddr tRcvAddr, TDSNetAddr tDstAddr, TDSNetAddr tRawAddr);

/**
 *@brief 恢复基于接收地址和目的地址的数据的源地址伪装
 *@param[in]    dsId         用户标识
 *@param[in]    tRcvAddr    接收的IP和Port
 *@param[in]    tDstAddr    目标IP和Port
 *@return       DSOK: 表示成功 ; DSERROR: 表示失败
 *@ref
 *@see
 *@note
 */
API u32 dsResetSrcAddrByDst(DSID dsId, TDSNetAddr tRcvAddr, TDSNetAddr tDstAddr);


/**
 *@brief 设置发送回调函数(基于接收组)
 *@param[in]    dsId         用户标识
 *@param[in]    tRcvAddr    接收的IP和Port
 *@param[in]    tSrcAddr    源目的IP和Port
 *@param[in]    pfCallback  回调函数
 *@return       DSOK: 表示成功 ; DSERROR: 表示失败
 *@ref
 *@see
 *@note
 */
API u32 dsSetSendCallback( DSID dsId, TDSNetAddr tRcvAddr, TDSNetAddr tSrcAddr, SendCallback pfCallback);


/**
 *@brief 为发送目标设置一个自定义的指针
 *@param[in]    dsId         用户标识
 *@param[in]    tRcvAddr    接收的IP和Port
 *@param[in]    tSrcAddr    源目的IP和Port
 *@param[in]    tDstAddr    转发目的IP地址和Port
 *@param[in]    pAppData    自定义指针
 *@return       DSOK: 表示成功 ; DSERROR: 表示失败
 *@ref
 *@see
 *@note
 */
API u32 dsSetAppDataForSend( DSID dsId, TDSNetAddr tRcvAddr, TDSNetAddr tSrcAddr, TDSNetAddr tDstAddr, void * pAppData);


/**
 *@brief 查询接收总包数
 *@param[in]    dsId            用户标识
 *@param[in]    tRcvAddr        接收的IP和Port
 *@param[in]    tSrcAddr        源目的IP和Port
 *@param[in]    dwRecvPktCount  接收总包数
 *@return       DSOK: 表示成功 ; DSERROR: 表示失败
 *@ref
 *@see
 *@note
 */
API u32 dsGetRecvPktCount( DSID dsId , TDSNetAddr tRcvAddr, TDSNetAddr tSrcAddr, u32& dwRecvPktCount );


/**
 *@brief  查询发送总包数
 *@param[in]    dsId            用户标识
 *@param[in]    tRcvAddr        接收的IP和Port
 *@param[in]    tSendToAddr     转发目的IP地址和Port
 *@param[in]    dwSendPktCount  发送总包数
 *@return       DSOK: 表示成功 ; DSERROR: 表示失败
 *@ref
 *@see
 *@note
 */
API u32 dsGetSendPktCount( DSID dsId, TDSNetAddr tRcvAddr, TDSNetAddr tSrcAddr, TDSNetAddr tSendToAddr, u32& dwSendPktCount);


/**
 *@brief  查询接收总字节数
 *@return       返回接收总字节数
 *@ref
 *@see
 *@note
 */
API s64 dsGetRecvBytesCount( );


/**
 *@brief  查询发送总字节数
 *@return       返回发送总字节数
 *@ref
 *@see
 *@note
 */
API s64 dsGetSendBytesCount( );


/**
 *@brief  显示所有的转发规则，及所有正在监听的端口
 *@return
 *@ref
 *@see
 *@note
 */
API void dsinfo();


/**
 *@brief  显示Dataswitch的版本信息
 *@return
 *@ref
 *@see
 *@note
 */
API void dsver();


/**
 *@brief  显示Dataswitch所提供的命令的帮助信息
 *@return
 *@ref
 *@see
 *@note
 */
API void dshelp();


/**
 *@brief        打开/关闭调试信息
 *@param[IN]    op    指明是设置还是清除：set, clear
 *@return
 *@ref
 *@see
 *@note
 */
API void dsdebug(const char* op = NULL);


/**
 *@brief        打开/关闭更高一级调试信息
 *@return
 *@ref
 *@see
 *@note 单独设置此函数是因为调试信息会影响整体性能
 */
API void dsdebug2();


/**
 *@brief  跟踪某一接收组调用
 *@param[in]    dsId            用户标识
 *@param[in]    pszLocalIP      接收的IP
 *@param[in]    wLocalPort      接收的port
 *@param[in]    dwTraceNum      跟中调度次数
 *@return
 *@ref
 *@see
 *@note
 */
API void dstrace(DSID dsId, s8* pszLocalIP, u16 wLocalPort, u32 dwTraceNum);

#endif
