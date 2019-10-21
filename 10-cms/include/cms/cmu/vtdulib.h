/*****************************************************************************
   模块名      : vtdu 
   文件名      : vtdulib.h
   相关文件    : 
   文件实现功能: vtdulib的对外接口定义
   作者        : 黄至春
   版本        : vs_phoenix  Copyright(C) 2009-2015 KDDACOM, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期         版本        修改人      修改内容
   2012/01/06     1.0        黄至春        创建
******************************************************************************/
#ifndef _VTDULIB_H_
#define _VTDULIB_H_

#include "cms/cms_errorcode.h"
#include "cms/cmu/cmu_struct.h"
#ifdef _USE_NEW_KDM_NETAGENT_
    #include "cbb/kdmnatagent/kdmnatagent.h"
#else
    #include "cms/natagent/natagent.h"
#endif //_USE_NEW_KDM_NETAGENT_


//VTDU默认配置文件路径
#define DEFAULT_PATH_VTDUCFG    "./conf/vtducfg.ini"

#define   KDM_NAT_DEFAULT_PU_SESSID         0xFFFFFFFF 


//VTDU运行模式
enum EMRunMode
{
    RUN_MODE_ATTACH = 1,    //附属模式运行
    RUN_MODE_ABSOLUTE       //独立
};

LPCSTR GetRunModeStr(EMRunMode emValue);

////////////////////////////////////////////////////////////////////////////////////
//数据结构及变量定义
////////////////////////////////////////////////////////////////////////////////////
struct TVtduCfg
{
    CIpList     m_cIpAddrList;
    int         m_nMediaStartPort;      //码流交换起始端口
	int         m_nMediaM2PPort;        //按源交换端口
    int         m_nMaxSwitchNum;        //最大交换路数
    int         m_nMaxRcvGroup;         //最大接收组
    int         m_nMaxSndMmbPerRcvGrp;  //每组最大发送目标数
    bool        m_bIsTravGAP;           //是否穿越网闸
    string      m_strCreditDomainUri;   //信任域
};

enum EMNatPackMediaTransChnType
{
	NAT_PACK_TYPE_INVALID    = 0,

	NAT_PACK_TYPE_VIDEO_RTP  = 1,
	NAT_PACK_TYPE_VIDEO_RTCP = 2,
	NAT_PACK_TYPE_AUDIO_RTP  = 3,
	NAT_PACK_TYPE_AUDIO_RTCP = 4,
	NAT_PACK_TYPE_DATA_RTP   = 5,
	NAT_PACK_TYPE_DATA_RTCP  = 6,
	NAT_PACK_TYPE_DATA_DATA  = 7, //下载时用????

	NAT_PACK_TYPE_NUM,
};

struct TNatData
{
protected:
    u32		m_dwSrcIp;		//源IP;
    u16		m_wSrcPort;		//源端口;
    u32		m_dwNatIp;		//NAT IP地址;
    u16		m_wNatPort;		//NAT 端口;

    string	m_strID;		//设备编码
    u32		m_dwSessID;		//会话ID;
	u8      m_byReserve;    //保留，参照EMNatPackMediaTransChnType

    u16		m_wBindPort;	//绑定端口
    u8		m_byChangeCnt;	//NAT变化次数

	string  m_strRecvedTime;//收到NAT探测包的时间

public:
    TNatData(void);
    u32 GetSrcIp() const;			//获取源IP
    void SetSrcIp(u32 dwSrcIp);		//设置源IP
    u16 GetSrcPort() const;			//获取源端口
    void SetSrcPort(u16 wSrcPort);	//设置源端口
    u32 GetNatIp() const;			//获取NAT IP地址
    void SetNatIp(u32 dwNatIp);		//设置NAT IP地址
    u16 GetNatPort() const;			//获取NAT 端口
    void SetNatPort(u16 wNatPort);	//设置NAT 端口
    const string& GetID() const;	//获取用户或设备编码
    void SetID(const string& tID);	//设置用户或设备编码
    u32 GetSessID() const;			//获取会话ID
    void SetSessID(u32 dwSessID);	//设置会话ID
    u8 GetReserve() const;			//获取保留字段
    void SetReserve(u8 byReserve);  //设置保留字段
    u8 GetChangeCnt(void)const;		//获取NAT变化数
    void AddChangeCnt(u8 byChangeCnt);//增加NAT变化数
	string GetRecvedTime() const;
	void SetRecvedTime(string strRecvedTime);
    void PrintData() const;
};

struct TVtduPunchChn
{
    u16                 m_wLocalPort;
    TMediaTransChannel  m_tMediaTransChn;
};

typedef vector<TVtduPunchChn> TVtduPunchChnList;


//VTDU_QUERY_MAPPED_ADDR_NTF
struct TVtduStunResult
{
    EmResult        m_emResult;
    u32             m_dwResultNum;
    TStunResult     m_atResultList[NATAGENT_MAX_PORT_NUM];
};

//VTDU_PUNCH_RESULT_NTF
struct TVtduPunchResult
{
    EmResult        m_emResult;
    u32             m_dwResultNum;
    TPunchResult    m_atResultList[NATAGENT_MAX_PORT_NUM];
};


//内部发消息，可以不用转网络序
struct TVtduCBMsg
{
public:
	TVtduCBMsg()
	{
		memset(this, 0, sizeof(TVtduCBMsg));
	}

	u32 GetEvent() const
	{
		return m_nEvent;
	}
	void SetEvent(u32 var)
	{
		m_nEvent = var;
	}

	u32 GetSeqNO() const
	{
		return m_dwSeqNO;
	}
	void SetSeqNO(u32 var)
	{
		m_dwSeqNO = var;
	}

	u32 GetContext() const
	{
		return m_dwContext;
	}
	void SetContext(u32 var)
	{
		m_dwContext = var;
	}

	const u8* GetMsgBody() const
	{
		return m_abyMsgBody;
	}
	void SetMsgBody(void* pbyBuf, u32 dwBufLen);

	void CatMsgBody(void* pbyBuf, u32 dwBufLen);

	u32 GetMsgBodyLen() const
	{
		return m_dwMsgBodyLen;
	}

	u32 GetSize() const
	{
		return sizeof(TVtduCBMsg) - sizeof(m_abyMsgBody) + m_dwMsgBodyLen;
	}

private:
	s32 m_nEvent;
	u32 m_dwSeqNO;
	u32 m_dwContext;	//通常是填TaskNo
	u32 m_dwMsgBodyLen;
	u8  m_abyMsgBody[50*1024];
};

typedef void (*pNATDETECTPACKET_CB)(TNatData tNatData);

struct TVtduLibConf
{
	TVtduLibConf()
	{
		m_emRunMode = RUN_MODE_ABSOLUTE;
		m_dwDefaultOspIID = 0;
		m_fpNatDetectCB = NULL;
	}

	void PrintData() const
	{
		OspPrintf(TRUE, FALSE, "  VTDU运行模式   :[%s]\n", GetRunModeStr(m_emRunMode));
		OspPrintf(TRUE, FALSE, "  DefaultOspIID  :[%lu]\n", m_dwDefaultOspIID);
		OspPrintf(TRUE, FALSE, "  ConfigPath     :[%s]\n", m_strConfigPath.c_str());
	}

	EMRunMode m_emRunMode;
	u32 m_dwDefaultOspIID;	//VtduLib投递消息的OspIID
	string m_strConfigPath;	//空，则使用DEFAULT_PATH_VTDUCFG
	pNATDETECTPACKET_CB m_fpNatDetectCB;
};
/*====================================================================
函数名      : VtduStart
功能        : 初始化VTDU
算法实现    : 
引用全局变量: 
输入参数说明: 
EMRunMode emRunMode --[in]vtdu的运行模式
LPSTR lpsConfigPath --[in]vtdulib配置文件路径。值为NULL时，使用默认路径DEFAULT_PATH_VTDUCFG。
返回值说明  : TRUE -  成功
             FALSE -  失败
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人          修改内容
2012/01/06     1.0        黄至春        创建
====================================================================*/
BOOL32 VtduLibStart( const TVtduLibConf& tVtduLibConf);

/*====================================================================
函数名      : AddNatRcvPort
功能        : 添加需要监听Nat包的端口
算法实现    : 
引用全局变量: 
输入参数说明: u16 wPort --[in]监听Nat数据包端口
返回值说明  : TRUE -  成功
             FALSE -  失败
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人          修改内容
2012/01/06     1.0        黄至春        创建
====================================================================*/
BOOL32 AddNatRcvPort( u16 wPort);

/*====================================================================
函数名      : RemoveNatRcvPort
功能        : 添加需要监听Nat包的端口
算法实现    : 
引用全局变量: 
输入参数说明: u16 wPort --[in]监听Nat数据包端口
返回值说明  : TRUE -  成功
             FALSE -  失败
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人          修改内容
2013/03/07     1.0        黄至春        创建
====================================================================*/
BOOL32 RemoveNatRcvPort( u16 wPort);

/*====================================================================
// 函数名:    Remove
// 功能:      删除指定设备的NatData
// 算法实现:  因为现在TNatData::m_strID不完整，所以最好输入IP精确匹配，如果dwIP为0，则不去匹配
// 访问权限:  public 
// 参数说明:  const string & strDevId--设备ID不是Uri
// 参数说明:  u32 dwIP
// 返回值:    void
//-----------------------------------------------------------
// 修改记录:
// 日      期     修改人        走读人          修改记录
// 2013/03/07     huangzhichun huangzhichun    创建
====================================================================*/
void RemoveNatData(const string& strDevId, u32 dwIP);
void RemoveAllNatData();

//******************************************************************************
// 函数名:    FindNatData
// 功能:      
// 算法实现:  
// 访问权限:  public 
// 参数说明:  const string & strDevId --设备ID不是Uri
// 参数说明:  u16 wSrcPort
// 返回值:    const TNatData*
//-----------------------------------------------------------
// 修改记录:
// 日      期     修改人        走读人          修改记录
// 2013/04/09     huangzhichun huangzhichun    创建
//******************************************************************************
const TNatData* FindNatData(const string& strDevId, u16 wSrcPort, u32 dwSessionId = KDM_NAT_DEFAULT_PU_SESSID );

/*====================================================================
函 数 名：  NatPkgCallback
功    能：  使用探测包的回调函数
算法实现：
全局变量：
参    数：u32 dwRecvIP,   探测包接收地址信息
u16 wRecvPort,  探测包接收地址信息
u32 dwSrcIP     探测包源地址信息
u16 wSrcPort    探测包源地址信息
u8* pData       探测包消息内容
u32 uLen        探测包消息长度
返 回 值： 0, 该数据包有效； 其他值，该数据包无效；
==============================================================*/
//typedef u32 (*VtduNatPkgCB)(u32 dwRecvIP, u16 wRecvPort, u32 dwSrcIP, u16 wSrcPort, u8* pData, u32 uLen);

/*====================================================================
函数名      : UpdateStun
功能        : 更新Stun地址
算法实现    : 
引用全局变量: 
输入参数说明: const TNetAddr& tStunAddr --[in]交换信息

返回值说明  : CMS_SUCCESS - 成功
             其他值 - 失败
----------------------------------------------------------------------
修改记录    :
日期        版本       修改人       修改内容
2013/05/17  1.0       黄至春        创建
====================================================================*/
BOOL32 UpdateStun(const TNetAddr& tStunAddr);

/*====================================================================
函数名      : StartPunch
功能        : 开始Punch
算法实现    : 
引用全局变量: 
输入参数说明: const TSwitchChannel& tSwitchChn --[in]交换信息

返回值说明  : CMS_SUCCESS - 成功
             其他值 - 失败
----------------------------------------------------------------------
修改记录    :
日期        版本       修改人       修改内容
2013/02/01  1.0        黄至春        创建
====================================================================*/
CMS_RESULT StartPunch(const TVtduPunchChnList& tVtduPunchChnList, const TIpAddr& tLocalIp, u32 dwContext, u32 *pdwSeqNum);

/*====================================================================
函数名      : AddSwitch
功能        : 添加交换
算法实现    : 
引用全局变量: 
输入参数说明: const TSwitchChannel& tSwitchChn --[in]交换信息

返回值说明  : CMS_SUCCESS - 成功
             其他值 - 失败
----------------------------------------------------------------------
修改记录    :
日期        版本       修改人       修改内容
2012/01/06  1.0        黄至春        创建
====================================================================*/
CMS_RESULT AddSwitch( const TSwitchChannel& tSwitchChn);

/*====================================================================
函数名      : RemoveSwitch
功能        : 删除交换
算法实现    : 
引用全局变量: 
输入参数说明: const TSwitchSsnID& tSwitchSsnID --[in]交换任务号

返回值说明  : CMS_RESULT -- CMS_SUCCESS，为成功；其他值，为失败
----------------------------------------------------------------------
修改记录    :
日期        版本       修改人       修改内容
2012/01/06  1.0        黄至春        创建
====================================================================*/
CMS_RESULT RemoveSwitch( const TSwitchSsnID& tSwitchSsnID);

/*====================================================================
函数名      : VtduLibQuit
功能        : 退出VTDU
算法实现    : 
引用全局变量: 
输入参数说明: 无
返回值说明  : 无
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人          修改内容
2012/01/06     1.0        黄至春        创建
====================================================================*/
void VtduLibQuit();

/*====================================================================
函数名      : GetVtduCfg
功能        : 获取vtdu模块的配置信息
算法实现    : 
引用全局变量: 
输入参数说明: 空

返回值说明  : TVtduCfg
----------------------------------------------------------------------
修改记录    :
日期        版本       修改人       修改内容
2012/04/18  1.0        黄至春        创建
====================================================================*/
TVtduCfg GetVtduCfg();

extern TVtduLibConf g_tVtduLibConf;


/*=============================调试命令(开始)===============================*/
//---------NAT探测包命令
API void vtdunatid(LPCSTR pszDevId);
API void vtdunatlist();

//---------Filter相关命令
API void pflist(u32 dwPort = 0);
API void vtduportfilterlist(u32 dwPort = 0);

//---------配置相关命令
API void vtduloadcfg();

//---------日志级别相关命令
//显示log参数信息
API void vtdulogpara();
//设置所有模块的打印级别
API void vtdulogall(u8 byLev);
//关闭所有打印，相当与logall(0)
API void vtdulogoff();

//---------事务相关命令 
//打印所有事务
API void vtdutasklist(LPCSTR szTaskType);
//打印指定事务信息
API void vtdutask(u32 dwTaskNO);
//杀死指定僵尸事务，用于故障恢复或者测试，故意使用恶心的名称避免用户随意调用
API void vtduIWillKillZombieTask(u32 dwTaskNO);
//根据事务号打印指定事务的子事务
API void vtdupnst(u32 dwTaskNO);
//根据事务号打印指定事务的子事务，等同于pnst
API void vtdutaskson(u32 dwTaskNO);
//根据sesskon号打印指定事务的子事务
API void vtdupsst(LPCSTR szSession);
//trace 指定事务
API void vtdutracet(u32 dwTaskNO);
//trace 下一个新建的事务
API void vtdutracent();

//打印redissdk信息
API void redissdk();
/*=============================调试命令(结束)===============================*/


#endif     //#ifndef _VTDULIB_H_

/////////////////////////////END OF THIS FILE///////////////////////////////
