#ifndef _GB_NA_NAT_AGENT_H_
#define _GB_NA_NAT_AGENT_H_

/*************************************************************************
声明：
严禁在NatAgent的回调函数中调用NA接口，以及做一些延迟操作，会导致NA死锁或Punch超时

修改记录：
2013.4.24 by 梁黎
1，修改TInitInfo初始化接口含义，用户在TInitInfo中指定的是排除IP列表（默认为空），而非可用网段IP列表
   由原先初始化传入网段IP列表改为由NA自己判断在线IP列表，并定期刷新
2，增加NA线程名称，便于定位问题出在哪个线程
3，提供在线IP网段列表接口 NAGetOnlineIPList
4，修改querystun回调逻辑，修改为回调结果始终成功（由用户判断Addr是否有效，NA提供判断接口）
   顾肖康提出即使QueryStun失败，还是可以继续Punch
   QueryStun可以认为是获取可用地址列表，不会失败

2013.4.26 by 梁黎
1，重构代码，在所有网段STUN都不可达情况下，修复SendTask无法销毁的Bug
2，增加SendThread/RecvTask的“呼吸特性”，降低CPU消耗
   原先定义select超时时间为20ms，主要是考虑Task的3秒超时计数，能够迅速回调上层
   但是如果Task任务表为空，没必要那么快的轮询
3，在SelfBind模式下，设置NA内部RecvSock端口复用，防止业务层在回调后bind失败
   注意：根据端口复用规则，业务层的socket也需要设置端口复用，否则仍然bind失败

2013.5.2 by 梁黎
1，增加了一个同步获取na结果接口，方便业务层能够单线程的调用NA接口并获得querystun或者punch结果，而不用考虑多线程回调的问题
   例如业务层在调用NAPunch后，紧接着调用NAGetResult，同步等待后返回和回调一样的结果内容，避免了多线程编程
   同步模式和回调模式不能并存，同步模式会导致业务层阻塞，建议客户端使用，服务器等需要处理大批量请求，不建议使用
2，修复几处日志打印问题

2013.5.5 by liangli
1，将NA的回调线程（sendthread）的线程堆栈设置为1 MB，防止回调线程栈溢出
   NRU出现过在回调函数中定义CMonMsg(27K)导致栈溢出问题，建议业务层不要在回调函数中处理业务

2013.5.7 by Royan
1，添加NAStunServerStat接口，返回StunServer是否可连通。
2，使用ticks计算重传和超时时间，取代以往的循环计数方式。

2013.5.8 by Royan
1，采用Req-Rsp-Ack方式进行Punch判断，UDP版的三次握手。
   收到任何一条Req都回复一条Rsp，仅在收到第一次Rsp时回复Ack，收到Ack不回复。
   以前NA版本不区分Req/Rsp，会把对端Req的SrcAddr作为Punch应答结果，单向路由情况下punch结果错误
   例如单网卡绑定多IP（172,10），RawSocket发送可达（SrcAddr=10，DstAddr=172，按照目的地址下一跳路由能通），但是逆向不可达
   新版本NA只根据Rsp/Ack来更新punch结果
2，添加一些必要的调试打印。

2013.5.10 by Royan
1，只绑定对端地址发送Punch包，RawSocket自动填充本地IP，减少无谓的Punch包。
2，调整NAQueryMappedAddr & NAPunch接口，增加UsedIP。
   如果业务不打算自己指定使用的本地IP，可以填充一个无效IP，NA内部会使用所有在线可用的IP列表工作；
   如果业务打算使用指定的本地IP，请填充有效的本地IP作为UsedIP，NA内部使用该UsedIP工作。
   用以规避如下情况：
   A（192.A & 10.A）和 B（192.B & 10.B）打洞时，遇到以下情况会造成Punch成功，两端却保持不同的Punch通道。
   |10.A  -> 10.B Req  |
   |192.A <- 192.B Req |
   |10.A  <- 10.B Rsp  |
   |192.A -> 192.B Rsp |

2013.5.13 by Royan
1，采用time_t计算每次Punch耗时秒数。
2，区分win/linux下的u64打印。win：%I64u；linux：%llu。
3，u64命名统一为ddw。

2013.5.16 by Royan
1，丰富打印，事务类型和状态改为str打印。
2，新增NACmd，统一调试命令调用接口。配合NAHelp打印中的CmdNum使用。业务调用只需要注册这两个命令（NAHelp & NACmd）即可。
3，回调时打印Punch起始时间。

2013.5.28 by Royan
1，NAInitLib时，增加判断StunServerIP是否有效。有效，在线IP连通stun标记位预设为TRUE；无效，则预设标记位为FALSE，表示StunServer不连通。
2，NAUpdateStunAddr时，判断StunAddr是否有变化，无变化直接return NATAGENT_OK；有变化，更新Stun地址。
   且 增加判断StunServerIP是否有效。有效，在线IP连通stun标记位预设为TRUE；无效，则预设标记位为FALSE，表示StunServer不连通。
3，127.0.0.1也添加到无效IP判断，即natagent认定127.0.0.1为无效IP。

2013.5.30 by Royan
1，鉴于平台升级重启可能导致vtdu调用NAInitLib获取不到在线IP，特修改NAInitLib循环获取在线IP，每次间隔10s，最多获取6次，获取成功立即跳出循环。

2013.7.3
1，鉴于CU每一个kedaplayer都会封装一个na，而且一个na就会绑定3个内部端口。如此，原本分配的3500-3599的100个内部端口段就不满足使用了，故在此基础上，
   把端口段放大到3500-4499共1000个端口。
2，修改na的打印，Stun/Punch起始、结束、耗时时长打印用u64强转，避免打印出错的问题。
3，当stun指定UsedIP时，stun回调没有过滤掉其他去的在线IP。在此，stun回调做过滤，其他的地址更改为0.0.0.0:0。

2013.8.12 by Royan
1，修改2处随机端口。
2，linux下改用epoll模式。
3，新增LogLev：emDebug，Debug调试级别日志。并附增NADebugLogOn & NADebugLogOff打开关闭Debug调试日志。

2013.9.10 by Royan
1，修改STUN探测间隔为1分钟（原10分钟）。
2，在检测在线IP数为0时，STUN探测间隔改为5秒；不为0时，STUN探测间隔为1分钟。

2014.9.29 by Royan
1，修改重传包间隔，50ms起，倍数递增，直至超时。
2，stun超时3s，punch超时延长为10s。
3，调试命令范围内打印改为error级别。
4，依次绑定所有ip发包。

2016.11.9 by Royan
1，解除网卡绑定。


遗留问题：
1，当两个网段物理隔绝时，传入的STUN地址只是其中一个网段的
   必然导致另一个网段querystun一定超时
   还好当前穿NAT（即使多运营商网络）应用必然所有IP都跟某个stun地址可连通
   而网段判断时，即使querystun超时也可以继续punch
   下个版本可以传入stun地址列表，同时stunserver绑定anyaddr
2，当NAInitLib或NAUpdateStunAddr传入的Stun地址为一个有效地址，但此IP上没有运行StunServer时，需要等探测线程超时才会设置探测标记位为FALSE。
   这个过程耗时一个超时时间（3秒）。后续考虑如何优化。
**************************************************************************/

/*
如不做特殊说明，本接口中所有端口都为主机序存在，ip都为字符串存在
*/

#include "kdvtype.h"

#if defined _WIN32 || defined __CYGWIN__
    #ifdef __GNUC__
      #define DLL_PUBLIC __attribute__ ((dllexport))
    #else
      #define DLL_PUBLIC __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
    #endif
  #define DLL_LOCAL
#else
  #if __GNUC__ >= 4
    #define DLL_PUBLIC __attribute__ ((visibility ("default")))
    #define DLL_LOCAL  __attribute__ ((visibility ("hidden")))
  #else
    #define DLL_PUBLIC
    #define DLL_LOCAL
  #endif
#endif


#ifdef WIN32
#ifdef __cplusplus
#define NATAGENT_API               extern "C" __declspec(dllexport)
#else
#define NATAGENT_API               __declspec(dllexport)
#endif //__cplusplus
#else
#ifdef __cplusplus
#define NATAGENT_API               extern "C" DLL_PUBLIC
#else
#define NATAGENT_API               DLL_PUBLIC
#endif //__cplusplus
#endif

/************************************************************************/
/*          宏定义                                                      */
/************************************************************************/
#define NATAGENT_MAX_IP_LEN            (32)               //IP最大长度为32
#define NATAGENT_MAX_NETSEG            (4)                //最大网段数量，即一个主机上最多4个IP
#define NATAGENT_MAX_IP_NUM            (20)               //本机最多IP个数(参考OSP定义)
#define NATAGENT_MAX_KEY_LEN           (32+1)             //Punch Key最大长度为32，再加上终结符
#define NATAGENT_MAX_PORT_NUM          (4)                //最多支持一次4个端口
#define NATAGENT_MAX_ID_LEN            (128)
/*
注意：部分系统的inet_addr("")的返回值不一致，所以请不要填入空串作为IP参数
为此NatAgent库提供了相关结构体的初始化函数 NASetNatAddrToInvalid
*/
#define NATAGENT_INVALID_IP_STR        ("0.0.0.0")        //无效IP地址"0.0.0.0"
#define NATAGENT_INVALID_IP_ANY        ((unsigned long)0x00000000)
#define NATAGENT_INVALID_IP_NONE       (0xffffffff)

#define NA_OSP_ETHERNET_ADAPTER_NAME_LENGTH (64)
#define NA_OSP_ETHERNET_ADAPTER_MAC_LENGTH  (6)
#define NA_OSP_ETHERNET_ADAPTER_MAX_IP      (16)
#define NA_OSP_ETHERNET_ADAPTER_MAX_NUM     (32)

//网卡状态,对应于TOSPEthernetAdapterInfo 结构的nState字段
#define NA_OSP_ETHERNET_ADAPTER_UNKNOWN (0)
#define NA_OSP_ETHERNET_ADAPTER_UP      (1)
#define NA_OSP_ETHERNET_ADAPTER_DOWN    (2)

/* 网卡信息 */
typedef struct {
	s8  achName[NA_OSP_ETHERNET_ADAPTER_NAME_LENGTH];/*网卡名*/
	s8  achIp[NATAGENT_MAX_IP_LEN];        /*地址*/
	s8  achSubNetMask[NATAGENT_MAX_IP_LEN];/*子网掩码*/
}TNANicInfo;

/* 多个网卡信息 */
typedef struct {
	u16 wNum; 											/*网卡数量*/
	TNANicInfo atNicInfo[NA_OSP_ETHERNET_ADAPTER_MAX_NUM];/*网卡信息*/
}TNANicInfoAll;

/************************************************************************/
/*          枚举定义                                                    */
/************************************************************************/
//执行结果类型
typedef enum
{
    NATAGENT_OK             = 0,        //操作成功
    NATAGENT_FAIL           = 1         //操作失败
}ENATAGENT_OPR_RESULT;

//日志级别类型
typedef enum 
{
	emError                 = 0,        //错误级别
	emWarning               = 1,        //警告级别
	emInfo                  = 2,        //信息（普通）级别
	emDebug                 = 3         //DEBUG级别
}EmLogLev;

//结果类型
typedef enum
{
    emStopped               = 0,        //当用户主动终止打洞时
    emOK                    = 1,        //成功
    emTimeout               = 2,        //超时
    emSysErr                = 3         //套接字错误
}EmResult;

//网络类型
typedef enum
{
    emSelfBind              = 0,        //由NatAgent来绑定并监听端口
    emDataSwitchBind        = 1         //由用户在DataSwitch上设置过滤函数
}EmNetWorkType;

/************************************************************************/
/*          结构体和回调接口定义                                         */
/************************************************************************/
//nat地址结构
typedef struct tagNatAddr
{
    s8                   m_achIP[NATAGENT_MAX_IP_LEN];      //IP地址
    u16                  m_wPort;                           //主机序Port
}TNatAddr;

//内外网地址对
typedef struct tagNatAddrPair
{
	TNatAddr             m_tLocalAddr;                      //本机地址
	TNatAddr             m_tMappedAddr;                     //本机地址的外网地址
}TNatAddrPair;

typedef enum
{
    NA_KDNA_MODE = 0,
    NA_KDGB_MODE = 1,
} ePunchMode;

typedef struct tagGBIdInfomation
{
    s8                   m_achLocalID[NATAGENT_MAX_ID_LEN];               //输入
    s8                   m_achRemoteID[NATAGENT_MAX_ID_LEN];               //输入
    s8                   m_achSessionID[NATAGENT_MAX_ID_LEN];		//输入
    TNatAddrPair         m_tReq;                                    //输出
    TNatAddrPair         m_tRsp;								    //输出
} TGBInfo;

//Punch Key
typedef struct tagPunchKey
{
    ePunchMode           m_eMode;                           //
    s8                   m_achKey[NATAGENT_MAX_KEY_LEN];    //Key
    u16                  m_wLen;                            //实际长度
    TGBInfo              m_tGBInfo;                         //国标Punch模式下使用
}TPunchKey;

/*
Punch对应信息结构。
考虑到需要一次打多个洞，故创建一个struct来标示一个本地端口对应的所需要的打洞信息。
@Param  m_wLocalPort  本地端口。
@Param  m_tPunchKey  key。
@Param  m_wRemoteAddrsNum  远端地址数，包括远端内外网。
@Param  m_ptRemoteAddrList  远端地址列表。
*/
typedef struct tagPunchInfo
{
	u16                  m_wLocalPort;                      //主机序
	TPunchKey            m_tPunchKey;                       //key
	u16                  m_wRemoteAddrsNum;                 //远端地址数
	TNatAddr            *m_ptRemoteAddrList;                //远端地址列表
}TPunchInfo;

/*
Stun查询外网地址结果。
每一个本地端口对应一个返回结果，每个结果就是一个内外网地址对列表，内网地址的端口唯一。
@Param  m_wAddrPairNum    地址对数目。
@Param  m_ptAddrPairList  地址对列表。
*/
typedef struct tagStunResult
{
	u32                  m_dwAddrPairNum;                    //地址对数目
	TNatAddrPair         m_atAddrPair[NATAGENT_MAX_NETSEG];  //地址对列表
}TStunResult;

/*
Punch结果。
每一个Key（每一个本地端口）对应一组打通的地址对。
@Param m_tPunchKey 就是key，一个本地端口对应一个key。
@Param m_tLocalAddr 本地ip、port
@Param m_tRemoteAddr 远端ip、port
m_tLocalAddr和m_tRemoteAddr是m_tPunchKey对应打通的第一对地址对。
*/
typedef struct tagPunchResult
{
	TPunchKey            m_tPunchKey;                       //key
	TNatAddr             m_tLocalAddr;                      //本地地址
	TNatAddr             m_tRemoteAddr;                     //远端地址
}TPunchResult;

/******************************************************************************
定义写日志的回调函数。
@Param emLogLev 日志级别。
@Param pchLog   日志内容。
******************************************************************************/
typedef void (*CBLogPrinter)(EmLogLev emLogLev, const s8 *pchLog);

/******************************************************************************
定义获取对端地址对的回调函数。
@Param emResult         查询结果，有可能超时。
@Param dwStunResultNum  stun回调结果数，等同于本地端口数，最大NATAGENT_MAX_PORT_NUM。
@Param ptStunResultList 结果列表。
@Param dwSeqNum         序列号，保证事务一致性，与对应的NAQueryMappedAddr序列号相等。
@Param context          上下文。
******************************************************************************/
typedef void (*CBQueryMappedAddr)(
	EmResult            emResult,
	u32                 dwStunResultNum,
	TStunResult        *ptStunResultList,
	u32                 dwSeqNum,
	void               *context);

/******************************************************************************
定义处理打洞结果的回调函数。
@Param emResult          Punch结果。失败则其他参数无效。
@Param dwPunchResultNum  Punch回调结果数，等同于打洞数目，最大NATAGENT_MAX_PORT_NUM。
@Param ptPunchResultList 打洞结果信息列表(数组)。数组元素信息包括：key，打通的本地和远端地址对。
@Param dwSeqNum          序列号，保证事务一致性，与对应的NAPunch序列号相等。
@Param context           上下文。
******************************************************************************/
typedef void (*CBPunchResult)(
	EmResult            emResult,
	u32                 dwPunchResultNum,
	TPunchResult       *ptPunchResultList,
	u32                 dwSeqNum,
	void               *context);

/*在线可用的网段IP列表*/
/*
实现算法：
NatAgent内部会获取本机所有地址列表，然后定期判断哪些网口在线
再排除指定的IP，返回业务层可用的在线网段IP列表
每次调用NAGetOnlineIPList的结果可能不一样（网线插拔），建议即用即取
*/
typedef struct tagOnlineIPList
{
    u32                 m_dwOnlineIPNum;                                         //本地IP数目
    s8                  m_achOnlineIP[NATAGENT_MAX_NETSEG][NATAGENT_MAX_IP_LEN]; //本地IP列表
}TOnlineIPList;

/*
初始化信息结构，用户传入
1，不启用IP列表：
特定应用场景下业务层会把本机某些地址不作为码流交换地址
因此m_achUnusedIP列表可以告诉NA排除这些地址，不作为网段
NA不会在stun或punch过程中使用这些网段，即使IP在线
默认用户可以不必填写m_achUnusedIP列表
2，STUN地址
即使stun地址无效，NA也可以初始化成功
因为NA是由2部分组成，querystun和punch
有可能用户在局域网内，不穿NAT根本不用stun，但是要用punch
后续需要修改为STUN地址列表
3，Stun回调结果
NAQueryMappedAddr接口一次最多可以查询4个端口
组合本机最多4个网段IP，再加上LocalAddr和MappedAddr，一共有32个Addr
    //只要有一个Addr收到STUNServer应答，回调结果为emOK
    //如果所有地址都和STUNServer无法ping通，回调结果为emTimeout
现在已经修改为无论如何都成功，返回的都是本机可用IP列表
4，Punch回调结果
NAPunch接口一次最多查询4个端口，分别绑定本机4个Addr，朝对端最多32个地址（对端也是4 x 4 x 2）发送punch包
只有当本机4个端口全部成功，，回调结果为emOK
否则回调结果为emTimeout
5，严禁在回调函数中调用NA接口，会导致死锁（二次Lock(g_hCBMutex);）
6，如果m_bSyncMode = TRUE；NA不会触发回调，会将回调结果通过NAGetResult接口抛给业务层
*/
typedef struct tagInitInfo
{
    u32                 m_dwUnusedIPNum;               //不启用IP数目
    s8                  m_achUnusedIP[NATAGENT_MAX_NETSEG][NATAGENT_MAX_IP_LEN]; //不启用IP列表
    TNatAddr            m_tSTUNAddr;                   //Stun Server地址（主机序） 
	CBLogPrinter        m_fLogPrinter;                 //日志回调接口
    CBQueryMappedAddr   m_fQueryMappedAddr;            //获取外网地址结果回调接口
    CBPunchResult       m_fPunchResult;                //打洞结果回调接口
    BOOL32              m_bSyncMode;                   //同步获取回调结果模式
}TInitInfo;

/*
为了避免用户直接memset(ptInitInfo, 0, sizeof(TInitInfo))，特添加此函数
该函数不负责分配对象内存，只是帮助用户初始化所有数据成员
@Param  输入要初始化的结构体地址
@Return 仍然返回该结构体地址
*/
NATAGENT_API TInitInfo* NASetInitInfoToNULL(TInitInfo *ptInitInfo);
/* 同上 */
NATAGENT_API TNatAddr* NASetNatAddrToInvalid(TNatAddr *ptNatAddr);

/*
判断IP是否合法
非法IP字符串输入参数包括
1, a.b.c.d 任何一段超过255
2，0.0.0.0
3，NULL
*/
NATAGENT_API BOOL32 NAIsIpAddrInvalid(u32 dwIp);
NATAGENT_API BOOL32 NAIsIpStrInvalid(const char *lpIPStr);

/************************************************************************/
/*          接口定义                                                    */
/************************************************************************/
/*
判断处理接收包。返回数据包类型值。DataSwitch模式，将此函数设为Filter函数来收包。
输入参数：按照NatAgent（科达代码规范）约定，所有IP地址为网络序，端口为主机序，其他为主机序
返回InvalidPacket表示不是punch or stun包，
返回StunPacket表示是stun包，返回PunchPacket表示是punch包
注：NAOnAck本打算改成NAOnRecv，考虑接口修改的影响，就只在此备注一下！！！
*/
NATAGENT_API u32 NAOnAck(u32 dwRecvIP, u16 wRecvPort, u32 dwSrcIP, u16 wSrcPort, u8* pData, u32 dwLen);

//NatAgent初始化
NATAGENT_API ENATAGENT_OPR_RESULT NAInitLib(TInitInfo *ptInitInfo);

/******************************************************************************
更新Stun Server地址。
@Param ptStunAddr      Stun Server地址。
******************************************************************************/
NATAGENT_API ENATAGENT_OPR_RESULT NAUpdateStunAddr(TNatAddr *ptStunAddr);

/******************************************************************************
获取在线网段地址列表
在线网段地址列表是本机地址列表中，插上网线的哪些IP，并且排除用户指定不启用IP
@Param ptOnlineIPList  [OUT]在线IP地址列表
******************************************************************************/
NATAGENT_API ENATAGENT_OPR_RESULT NAGetOnlineIPList(TOnlineIPList *ptOnlineIPList);

/******************************************************************************
查询外网地址。
@Param eNetworkType    [IN]网络类型。
@Param wLocalPortNum   [IN]端口数目，不得大于NATAGENT_MAX_PORT_NUM。
@Param pwLocalPortList [IN]端口列表(数组)，元素数目等于wLocalPortNum。
@Param achUsedIP       [IN]指定本地使用IP（IN）。
                       如果业务不打算自己指定使用的本地IP，可以填充一个无效IP，NA内部会使用所有在线可用的IP列表工作；
                       如果业务打算使用指定的本地IP，请填充有效的本地IP作为UsedIP，NA内部使用该UsedIP工作。
@Param dwSeqNum        [OUT]序列号，保证事务一致性，与对应的CBQueryMappedAddr序列号相等。
@Param context         [IN]上下文指针。
******************************************************************************/
NATAGENT_API ENATAGENT_OPR_RESULT NAQueryMappedAddr(
	EmNetWorkType        eNetworkType,
	u32                  dwLocalPortNum,
	const u16           *pwLocalPortList,
	s8                   achUsedIP[NATAGENT_MAX_IP_LEN],
	u32                 *pdwSeqNum,
	void                *context);

/******************************************************************************
打洞。
@Param eNetworkType    [IN]网络类型。
@Param dwPunchInfoNum  [IN]Punch信息数，等于本地端口数，最大NATAGENT_MAX_PORT_NUM。
@Param ptPunchInfoList [IN]Punch信息列表(数组)，元素数目等于dwPunchInfoNum。
                       每一条打洞信息都包括（本地端口，远端的内外地址，内网在前外网在后）。
@Param achUsedIP       [IN]指定本地使用IP（IN）。
					   如果业务不打算自己指定使用的本地IP，可以填充一个无效IP，NA内部会使用所有在线可用的IP列表工作；
					   如果业务打算使用指定的本地IP，请填充有效的本地IP作为UsedIP，NA内部使用该UsedIP工作。
@Param dwSeqNum        [OUT]序列号，保证事务一致性，与对应的CBQueryMappedAddr序列号相等。
@Param context         [IN]上下文指针。
******************************************************************************/
NATAGENT_API ENATAGENT_OPR_RESULT NAPunch(
	EmNetWorkType        eNetworkType,
	u32                  dwPunchInfoNum,
	const TPunchInfo    *ptPunchInfoList,
	s8                   achUsedIP[NATAGENT_MAX_IP_LEN],
	u32                 *pdwSeqNum,
	void                *context);

/******************************************************************************
定义同步获取回调结果函数
@Param dwSeqNum     [IN]回调的序列号
@Param pemType      [OUT]回调的操作类型
@Param pemResult    [OUT]回调结果
@Param pdwResultNum [OUT]结果个数
@Param ptResultList [OUT]回调信息数组（注意，根据具体应用场景决定传入TStunResult *或者 TPunchResult *）
@Param ppContent    [OUT]回调上下文指针，注意，传入的参数是指针的指针
@Param dwTimeout    [IN]接口超时时间，单位ms，推荐 3500
返回值：如果成功获取回调信息返回NATAGENT_OK，否则返回NATAGENT_FAIL
******************************************************************************/
NATAGENT_API ENATAGENT_OPR_RESULT NAGetResult(u32 dwSeqNum, EmResult *pemResult, u32 *pdwResultNum, 
    void *ptResultList, void **ppContent, u32 dwTimeout);

/******************************************************************************
开启UDP洞保活。
@Param ptLocalNatAddr      [IN]UDP洞对应的本地地址。
@Param ptRemoteNatAddr     [IN]UDP洞对应的远端地址。
@Param dwKeepAliveInterval [IN]保活间隔，单位：秒，推荐 20。
******************************************************************************/
NATAGENT_API ENATAGENT_OPR_RESULT NAStartKeepAlive(TPunchKey tPunchKey,
	const TNatAddr      *ptLocalNatAddr,
	const TNatAddr      *ptRemoteNatAddr,
	u32                  dwKeepAliveInterval);

/******************************************************************************
停止UDP洞保活。
@Param ptLocalNatAddr  [IN]UDP洞对应的本地地址。
@Param ptRemoteNatAddr [IN]UDP洞对应的远端地址。
******************************************************************************/
NATAGENT_API ENATAGENT_OPR_RESULT NAStopKeepAlive(
	const TNatAddr      *ptLocalNatAddr,
	const TNatAddr      *ptRemoteNatAddr);

//退出NatAgentLib
NATAGENT_API ENATAGENT_OPR_RESULT NAQuitLib();

//返回StunServer连通状态，TRUE连通，FALSE不连通。
NATAGENT_API BOOL32 NAStunServerStat();

/************************************************************************/
/*          调试接口定义                                                 */
/* NatAgent库被设定不依赖于OSP库，所以如果需要在telnet执行，由上层用户注册  */
/* 建议注册OSP命令时，将函数字符串换成小写                                 */
/************************************************************************/
//帮助信息
NATAGENT_API void NAHelp();

//打印全局信息
NATAGENT_API void NAGlobalInfo();

//打印回调事务链表
NATAGENT_API void NACBList();

//打印接收事务链表
NATAGENT_API void NARecvList();

//打印发送事务链表
NATAGENT_API void NASendList();

//打印保活事务链表
NATAGENT_API void NAKAList();

//打开Debug调试日志
NATAGENT_API void NADebugLogOn();

//关闭Debug调试日志
NATAGENT_API void NADebugLogOff();

//NA命令调用汇总接口
NATAGENT_API void NACmd(u16 wCmdNum);

//for test
NATAGENT_API u32 NAGetKAListSize();
NATAGENT_API u32 NAGetCBListSize();
NATAGENT_API u32 NAGetSendListSize();
NATAGENT_API u32 NAGetRecvListSize();

NATAGENT_API BOOL32 NAGetOnlineNicList(TNANicInfoAll* ptOnlineNicList);
NATAGENT_API BOOL32 NAGetAllNicList(TNANicInfoAll* ptAllNicList);

NATAGENT_API u32 ConstructGBPunchReq(u8 byPunchType,u8* pbyPunchReq, const TPunchKey *ptPunchKey, s8* pchLocalIp, u16 wLocalPort, s8* pchRemoteIp, u16 wRemotePort);


#endif /*_GB_NA_NAT_AGENT_H_*/


