/*****************************************************************************
   模块名      : cui_fcgi 
   文件名      : cuidata.cpp
   相关文件    : 
   文件实现功能: cuidata 定义CUI的全局变量
   作者        : liangli
   版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2011/11/10   1.0         liangli        创建
******************************************************************************/
#ifndef _CUI_DATA_H_
#define _CUI_DATA_H_
#include <string>
#include <list>
#include <set>
#include <queue>
#include "kdvtype.h"
#include "cms/ospext/ospext.h"
#include "cms/ospsip/ospsip.h"
#include "cms/container/xmap.h"
#include "cms/tas/tas_proto.h"
#include "cms/commonconfig/cmcfg.h"
#include "cms/container/xmap.h"
#include "cms/cmu/cmu_struct.h"
#include "cms/cmu/vtdulib.h"
#include "StreamConcurrencyRestrict.h"
using std::string;
using std::list;
using std::set;
using std::queue;
using std::stringstream;

#define EVENT_BEGIN ("<event>")
#define EVENT_END ("</event>")

class CCuAgent;
typedef CCuAgent * PCuAgent;    //CuAgent指针
class CUserMgr;
typedef CUserMgr * PCUserMgr;

#define REDIS_DEF_PORT 6379
#define REDIS_SEPARATOR ":"
#define REDIS_NTF_CHN_NAME_SIZE 1024 //1k字节
#define REDIS_NTF_MSG_SIZE (10*1024*1024) // 10M字节

#define REDIS_MAX_QUERY_DEVICE_INFO_ENTRY_NUM 100//CU单次批量查询设备信息的数量限制 

struct TRedisNtfMsgInfo
{
	TRedisNtfMsgInfo()
	{
		memset(this, 0, sizeof(TRedisNtfMsgInfo));
	}
	char m_szchnName[REDIS_NTF_CHN_NAME_SIZE];
	char m_szMsgData[REDIS_NTF_MSG_SIZE];
	void* m_dbInterface;
	void* m_szUserData;
};


struct TNatPacketInfo
{
	TNatPacketInfo()
	{
		memset(this, 0, sizeof(TNatPacketInfo));
	}
	u32		m_dwSrcIp;		//源IP;
	u16		m_wSrcPort;		//源端口;
	u32		m_dwNatIp;		//NAT IP地址;
	u16		m_wNatPort;		//NAT 端口;
	u32		m_dwPalyID;		//会话ID;
	s8      m_szCuid[32+1];
	u8      m_byReserve;    //保留，参见EMNatPackMediaTransChnType

public:
	bool IsReserveInvalid()
	{
		bool bInvalid = false;
		if (NAT_PACK_TYPE_INVALID == m_byReserve || NAT_PACK_TYPE_NUM <= m_byReserve)
		{
			bInvalid = true;
		}

		return bInvalid;
	}
};


struct TMediaTaskKey
{
private:
	string          cuId;               //cu sessionID
	int             playId;             //cu playID

public:
	TMediaTaskKey()
	{
		Clear();
	}

	~TMediaTaskKey()
	{
		Clear();
	}

	void Clear()
	{
		cuId.clear();
		playId = 0;
	}
public:
	void SetCUId(const string& strCuId)
	{
		cuId = strCuId;
	}
	string& GetCUId()
	{
		return cuId;
	}
	const string& GetCUId() const
	{
		return cuId;
	}

	void SetPlayId(int nPlayId)
	{
		playId = nPlayId;
	}
	int GetPlayId() const
	{
		return playId;
	}
	int GetPlayId()
	{
		return playId;
	}
};

template <> struct CHashFunctor<TMediaTaskKey>
{
	size_t operator()(const TMediaTaskKey& key) const
	{
		char szPlayId[64] = {0};
		sprintf(szPlayId, "%d", key.GetPlayId());
		string strKey = key.GetCUId() + szPlayId;

		return hash_ms(strKey.c_str(), 0);
	}
};
template <> struct CEqualFunctor<TMediaTaskKey>
{
	int operator()(const TMediaTaskKey& key1, const TMediaTaskKey& key2) const
	{
		return (key1.GetCUId() == key2.GetCUId()
			&& key1.GetPlayId() == key2.GetPlayId());
	}	
};

typedef CXMap<TMediaTaskKey,CTask*> TMediaTaskSets;
extern TMediaTaskSets g_tMediaTaskSets;
void PushMediaTask(const string& strSession,const s32& nPlayId,CTask* pTask);
void PopMediaTask(const string& strSession,const s32& nPlayId);
CTask* GetMediaTask(const string& strSession,const s32& nPlayId);

inline string IpToStr(u32 dwIp)
{
	in_addr inAddrIp;
	inAddrIp.s_addr = dwIp;        
	return string(inet_ntoa(inAddrIp));
}
inline u32 StrToIp(const char* szIp)
{
	if (szIp == NULL)
	{
		return 0;
	}

	u32 dwIp = inet_addr(szIp);
	return dwIp;
}

bool ChangeToNatAddr(const string& strCuSession,TNetAddr& tAddr,const u32& dwPlayId);

class CPrivilege
{
public:
    void SetPri(const int nPri);
    void Clear();
    void SetPri(const CBitsContainer &cPri);
    //对于PTZ的权限等级
    int GetPtzPriLevel();
    string ToString();
    bool IsEmpty() const;
    bool HasPri(const int nPri);
private:
    set<int> m_setPri;
};

class CUserAccountBaseInfo
{
public:
    string m_tUserID;       //UUID，不带域名
    string m_strUserName;   //用户名, 带域名
    string m_strPasswd;     //密文密码
    string m_tUserGroupID;  //用户组ID
    CPrivilege m_tUserPri;
    CStreamConcurrencyRestrict m_cStrmRestrict;
    int    m_nUserType;
public:
    CUserAccountBaseInfo()
    {
        m_nUserType = 0;
    }
public:
    void SetUserAccountBaseInfo(CUserInfo &cUserInfo, const string &strUserName);
    bool InGroup();
};

enum EUserDevPriListState
{
    DEVPRI_HAS_FINISHED = 0,
    DEVPRI_NOT_START,
    DEVPRI_IS_RECVING
};

/*用户处于用户组时权限数据等相关数据将来自用户组*/
class CUserAccount : public CUserAccountBaseInfo
{
private:
    /*
        2012.4.6修改，因为TAS很多时候没有同时提供设备ID和DomainID，那么就无法构建正确的tDevURI
        也就无法根据完整的DevURI查询设备
        因为tDevUUID是绝对唯一的，所以可以根据tDevUUID来查询
        所以m_mapDevPriList的key是tDevUUID，value的内容是tDevURI
    */
    list<PCuAgent> m_lstCuAgentList;                  //此账户下的同名在线CU
    PCUserMgr m_pcUserMgr;                            //方便引用同一个3acAgent中的UserGroupList

public:
    EUserDevPriListState m_bDevPriState; //标记获取设备权限列表的状态，处于用户组时该标志无效
    u32 m_dwDevPriQueryTaskNo;           //获取设备权限列表的TASK号, 防止存在多个查询TASK
    u32 m_dwStreamConcurrency;
public:
    CUserAccount();
    CUserAccount(PCUserMgr pcUserMgr);
    ~CUserAccount();
    void PrintData(u32 dwOffSet = 0, u32 dwMaxDev =(~(u32)0));

    s32 GetUserType(); // 如果用户不在用户组中，返回自身类型，否则返回用户组的类型
	bool IsSuperAdmin();
    bool IncreaseConcurrency();
    void DecreaseConcurrency();

    CPrivilege &GetUserPri();

    list<PCuAgent>& GetCuAgentList();
    bool AddCuAgent(PCuAgent pcCuAgent);
    bool DeleteCuAgent(const PCuAgent pcCuAgent);

	// 通知CU
    bool DelUserDevInfo(const string &strDevUri, bool bIsTvWall); // 如果是电视墙，则不发通知
};
typedef CStrMap<CUserAccount> CUserAccountList;

class CUserGroupBaseInfo
{
public:
    string m_tGroupID;
    string m_strGroupName;
    CPrivilege m_tUserGroupPri;
    int m_nGroupType;
    CStreamConcurrencyRestrict m_cStrmRestrict;
public:
    CUserGroupBaseInfo()
    {
        m_nGroupType = 0;
    }
    void SetUserGroupBaseInfo(CUserGroupInfo &cUserGrpInfo);
};

class CUserGroup : public CUserGroupBaseInfo
{
private:
    PCUserMgr m_pcUserMgr;              //方便引用同一个3acAgent中的UserGroupList
    /*
        2012.4.6修改，因为TAS很多时候没有同时提供设备ID和DomainID，那么就无法构建正确的tDevURI
        也就无法根据完整的DevURI查询设备
        因为tDevUUID是绝对唯一的，所以可以根据tDevUUID来查询
        所以m_mapDevPriList的key是tDevUUID，value的内容是tDevURI
    */
public:
    EUserDevPriListState m_bDevPriState; //标记获取设备权限列表的状态
    u32 m_dwDevPriQueryTaskNo;           //获取设备权限列表的TASK号, 防止存在多个查询TASK

public:
    CUserGroup()
    {
        m_pcUserMgr = NULL;
        m_bDevPriState = DEVPRI_NOT_START;
    }
    CUserGroup(PCUserMgr pcUserMgr)
    {
        m_pcUserMgr = pcUserMgr;
        m_bDevPriState = DEVPRI_NOT_START;
    }
	bool DelUserDevInfo(const string &strDevUri);
public:
    void PrintData(u32 dwOffSet = 0, u32 dwMaxDev =(~(u32)0));
};

class CCuPtzLockInfo
{
public:
	CCuPtzLockInfo();

	// 是否有效
	bool IsValid() const ;
	
	// 是否过期
	bool IsExpired() const;

	void Clear();

	void PrintData() const;
public:
	void SetDeviceId(const string& strDeviceId);
	string& GetDeviceId();
	const string& GetDeviceId() const;

	void SetVideoSrcId(int nVideoSrcId);
	int GetVideoSrcId() const;

	void SetBeginTime(const string& strBeginTime);
	string& GetBeginTime();
	const string& GetBeginTime() const;

	void SetEndTime(const string& strEndTime);
	string& GetEndTime();
	const string& GetEndTime() const;

	void SetUserId(const string& strUserId);
	string& GetUserId();
	const string& GetUserId() const;

	void SetUserName(const string& strUserName);
	string& GetUserName();
	const string& GetUserName() const;

	void SetPassword(const string& strPassword);
	string& GetPassword();
	const string& GetPassword() const;

	void SetUserPtzLevel(int nUserPtzLevel);
	int GetUserPtzLevel() const;
private:
	string m_strDeviceId;      // 设备id
	int m_nVideoSrcId;       // 视频源id
	string m_strBeginTime;     // 锁定开始时间.
	string m_strEndTime;       // 自动解锁时间.
	string m_strUserId;        // 加锁用户
	string m_strUserName;      // 用户名		
	string m_strPassWord;      // 手动解锁密码
	int m_nUserPtzLevel;   // 用户PTZ等级
};

class CCuPtzLockInfoList
{
public:
	CCuPtzLockInfoList();;
	~CCuPtzLockInfoList();;

	void Clear();

	bool IsCuPtzLockedByUser(const TChannel &tDevVsChn, const string &userId);

	bool GetCuPtzLockInfoByVs(const TChannel &tDevVsChn, CCuPtzLockInfo &ptzLockInfo);

	bool AddCuPtzLockInfo(const CCuPtzLockInfo &ptzLockInfo);

	// 如果视频源的PTZ锁是指定用户添加的，那么就删除
	bool DelCuPtzLockInfoByUserWithVsChn(const string &strUserId, const TChannel &devVsChn);

	// 通过视频源ID删除锁
	void DelCuPtzLockInfoByVs(const TChannel &devVsChn);

	// 通过用户ID删除锁
	void DelCuPtzLockInfoByUserId(const string &strUserId);

	// 通过设备ID删除锁
	void DelCuPtzLockInfoByDevId(const string &strDevId);

	void PrintData();
	void PrintDataByDevVsChn(const TChannel &tDevVsChn);
	void PrintDataByUserId(const string &strUserId);
	void PrintDataByDevId(const string &strdevId);
private:
	std::map <string, TChannelList> m_mapUserIdToVsChnList;
	std::map <TChannel, CCuPtzLockInfo> m_mapPtzLockInfo;
	std::map <string, TChannelList> m_mapDevToChnList;
};

/*
    CUI中的用户管理分为三层：1，3AC账户；2，用户组/用户账户；3，CuAgent
    3AC账户代表了一个域的所有用户或用户组
    其中用户账户对应多个CuAgent，每个CuAgent唯一对应一个CU客户端
    如果用户在组内，就属于用户组信息
*/
class CUserMgr
{
public:
    CUserMgr(){}
    ~CUserMgr();

    CStrMap<string> m_mapUserID2UserName;                   //为了方便根据UserID查找UserAccount
    CStrMap<CUserAccount*> m_mapUserName2PCuserAccount;     //用户账户列表，用户可能在组中，需要根据userGrpId判断
    CStrMap<CUserGroup> m_mapGroupID2UserGroup;             //属于组的用户, 这些用户表现与用户组一致
public:
    CUserAccount *FindUserAccountByUserName(const string &strUserName);
    CUserAccount *FindUserAccountByUserID(const string &tUserID);
    CUserGroup *FindUserGroupByGroupID(const string &strGroupID);
};

class CCuiState
{
public:
    CCuiState()
    {
        m_bConnectProxy = false;
    }
public:
    CCMSTime m_tCuiBeginRun;
    CCMSTime m_tCuiStateTime;
    bool m_bConnectProxy;
    //cmu给cui分配的session
    string m_strSession;
};

typedef CStrMap<u32> CSessionToTaskMap;
typedef CStrMap<u32> C3ASNameToTaskMap;
//typedef CStrMap<string> CDomainIdToDomainNameMap;
typedef CStrMap<u32> CPeNameToPeTaskMap;
typedef CStrMap<u32> CDevUriToPuConfigTaskMap;
typedef CStrMap<u32> CDevUriToPuAlarmTaskMap;
typedef CStrMap<u32> CDevUriToPuStatusTaskMap;
typedef CStrMap<u32> CDevUriToPuServiceStatusTaskMap;
typedef CStrMap<u32> CDevUriTypeToSubTaskMap;
class CDomainInfoMap
{
public:
    CDomainInfoMap(){};
    ~CDomainInfoMap();
public:
    void SetDomainInfo(const CDomainRelationInfo& cInfo);
    void DelDomainInfo(const string& strDomainId);
    void ModDomainInfo(const CDomainRelationInfo& cInfo);
    CDomainRelationInfo* FindDomainInfoById(const string& strDomainId);
    CDomainRelationInfo* FindDomainInfoByName(const string& strDomainName);
    string GetNextDomainNameTo(const string& strDstDomainName);
    string DomainIdToName(const string& strDomainId);
    string DomainNameToId(const string& strDomainName);
    s32 GetSize() const
    {
        return m_cDomainNameMap.GetSize();
    }
    void PrintData();
    const string GetParentDomainId() const
    {
        return m_strParentDomainId;
    }
    void SetParentDomainId(const string& strDomainId)
    {
        m_strParentDomainId = strDomainId;
    }
	void GetDomainNameList(std::vector<string> &domainNameList)
	{
		Iterator iter;
		iter.SetToBegin();
		std::string key;
		CDomainRelationInfo *pDomainInfo = NULL;

		while( !iter.End() )
		{
			if ( m_cDomainNameMap.Iterate(iter, key, pDomainInfo) )
			{
				domainNameList.push_back(pDomainInfo->GetDomainName());
			}
		}
	}
private:
    CStrMap<CDomainRelationInfo*>   m_cDomainIdMap;
    CStrMap<CDomainRelationInfo*>   m_cDomainNameMap;
    string m_strParentDomainId;
};

enum ECuBatOptType
{
	enBatOptTypeInvalid = -1,	// 无效值
	enBatOptStartPlatRecord = 0,	// 批量开启平台录像
	enBatOptStopPlatRecord = 1,	// 批量停止平台录像
	enBatOptStartPuRecord = 2,	// 批量开启前端录像
	enBatOptStopPuRecrod = 3,	// 批量停止前端录像
	enBatOptStartWiper = 4,	// 批量开启雨刷
	enBatOptStopWiper = 5,	// 批量停止雨刷
	enBatOptStopPuCall = 6,	// 批量停止音频呼叫
	enBatOptStartPresetCall=7 //批量载入预置位

};

struct TBatOptInfo
{
public:
	TBatOptInfo()
	{
		m_strBatOptReq.clear();
		m_nPos = 0;
		m_total = 0;
		m_nTaskNo = INVALID_TASKNO;
	}

	string m_strBatOptReq;
	int m_nPos;
	int m_total;
	u32 m_nTaskNo;
};

class CuBatOptList
{
public:
	CuBatOptList()
	{
		Clear();
	};
	~CuBatOptList()
	{
		Clear();
	}

	bool Clear();;
	bool IsEmpty();
	void PrintData();

	// 直接删除第一条记录
	void PopFront();
	// 删除第一条记录
	bool PopFrontByBatOptInfo(TBatOptInfo &frontBatOptInfo);

	// 开始第一个批量任务， 设置TaskNo
	bool SetFrontBatOptTaskNo(const string strReq, const int &nTaskNo);

	bool GetFrontBatOptInfo(TBatOptInfo &frontBatOptInfo);

	// 添加批量任务
	void PushBackBatOptStrReq(const string &strReq);

	// 设置第一条批量任务的进度
	bool SetFrontBatOptPos(const TBatOptInfo &frontBatOptInfo);
	
private:
	list<TBatOptInfo> m_batOptReqInfolist; // 按照先后顺序记录待处理的批量操作
};

class CCuiDataMgr
{
public:
    CSessionToTaskMap m_mapSessToTaskMap;       //用户会话ID与CuAgent的对应关系，或者QueryTag与TAS QueryTask对应关系
    C3ASNameToTaskMap m_map3asURITo3asAgentMap; //域名和3asAgent的对应关系
    //CDomainInfoMap m_mapDomainInfo;  //保存整个系统中CUI登录的3AS的域列表
    CPeNameToPeTaskMap m_mapPeNameToPeTaskMap;  //外设名字与CPeTask的对应关系(新增部分)
    //////////////////////////////////////////////////////////
    CDevUriTypeToSubTaskMap m_mapDevUriTypeToSubTaskMap;
    //////////////////////////////////////////////////////////
    CCuiState m_cCuiState;
	//////////////////////////////////////////////////////////
	// 如果没有CU登陆，3A断链后不会重连，所以两个任务都全部取消
	// CMU断链后释放所有task，取消所有批量任务
	// rcs断链后会重连，不取消批量任务
	CuBatOptList m_cPuBatOptRequestList;
	CuBatOptList m_cPlatBatOptRequestList;
	u32 m_dwBatOptCheckTaskNo;

public:
    CCuiDataMgr();
    void ClearAllData();
    void PrintData(const CInstExt *pcInst, u32 dwOffSet = 0);
    void SetCuiBeginRunTime();
    void SetCuiStateHappenTime();

    // pcAgent为空时刷状态给所有的CU, tPeUri为空时刷新所有的模块
    void FlushPlatformServiceStatus(const CInstExt *pcInst, CCuAgent *pcAgent = NULL, const TSipURI &tPeUri=TSipURI()); 
public:
    //全局数据接口
    
    /*
        目前只有用户会话ID与CuAgent的对应关系，或者QueryTag与TAS QueryTask对应关系
    */
    CSipTask * GetTaskPtrBySession(const string &strSession, const CInstExt *pcInst);
    CSipTask * Get3asAgentTaskPtrBy3asName(const string &str3asName, const CInstExt *pcInst);
    CSipTask * GetPeAgentPtrByPeName(const string &strPeName, const CInstExt *pcInst);
    CSipTask * GetSubTaskPtrByDevUri(const string& strDevUri, const CInstExt *pcInst);

    bool IsHasDevPrivilege(const string &strCuSession, const CInstExt *pcInst, const string &strDevUri, s32 nPrivilege);
    const string& GetCuserIdByCuserSession(const string &strSession, const CInstExt *pcInst);
//     CSipTask * GetPuStatusSubPtrByDevURI(const string &strDevURI, CInstExt *pcInst);
//     CSipTask * GetPuAlarmSubPtrByDevURI(const string &strDevURI, CInstExt *pcInst);
//     CSipTask * GetPuConfigSubPtrByDevURI(const string &strDevURI, CInstExt *pcInst);
//     CSipTask * GetPuServiceStatusSubPtrByDevURI(const string &strDevURI, CInstExt *pcInst);
private:
    u32 GetTaskIDBySession(const string &strSession);
    u32 Get3asAgentTaskIDBy3asName(const string &str3asName);
    u32 GetPeAgentTaskIDByPeName(const string &strPeName);
    u32 GetSubTaskIDByDevUri(const string& strURI);
//     u32 GetPuStatusSubIDByDevURI(const string &strURI);
//     u32 GetPuAlarmSubIDByDevURI(const string &strURI);
//     u32 GetPuConfigSubIDByDevURI(const string &strURI);
//     u32 GetPuServiceStatusSubIDByDevURI(const string &strURI);
};


#ifdef WIN32
#define CONFIG_FILE_NAME ".\\conf\\cuicfg.xml"
#define EXT_CFG_FILE_NAME ".\\conf\\cui.ini"
#else
#define CONFIG_FILE_NAME "./conf/cuicfg.xml"
#define EXT_CFG_FILE_NAME "./conf/cui.ini"
#endif

#define CUI_CONFIG_ROOT "cuiconfig"
#define SECTION_CUI_LOCAL_INFO "cui_local_info"

#define KEY_LOCAL_IP    "local_sip_ip"
#define KEY_LOCAL_PORT  "local_sip_port"
#define KEY_CUI_UUID    "cui_uuid"
#define KEY_TCP_PORT    "tcp_listen_port"
#define KEY_TCP_TIMEOUT "tcp_timeout"       //给cui.fcgi使用的超时
#define KEY_SIP_RSP_TIMEOUT "sip_rsp_timeout"   //等待SIP应答超时
#define KEY_DLG_RSP_TIMEOUT "dlg_rsp_timeout"   //等待SIP应答超时
#define KEY_CU_HB_TIMEOUT   "cu_hb_timeout" //CU心跳超时
#define KEY_APP_QUEUE_SIZE "app_queue_size"
#define KEY_CU_MAX_NUM "cu_max_num" //CU最大接入数
#define KEY_SUBSCRIBE_FLOW_LIMIT "subscribe_flow_limit"
#define KEY_UNSUBSCRIBE_FLOW_LIMIT "unsubscribe_flow_limit"
#define KEY_ROUTEMODE "RouteMode"
#define KEY_CU_OPERATELOG "cu_operate_log"
#define KEY_MOBILE_CLIENT_LIMIT "mobile_client_limit"
#define KEY_NAT_PORT "natport"
#define KEY_MAPED_NAT_IP "mapednatip"
#define KEY_MAPED_NAT_PORT "mapednatport"
#define KEY_BIND_MAPED_NAT_ADDR "bind_maped_nat_addr"
#define KEY_UPDATE_NETSEG_INTERVAL "update_netseg_interval"
#define KEY_NEED_CUI_SWITCH "need_cui_switch"
#define KEY_BATTASK_RECNUM_PERTIME   "bat_task_rec_num_pertime"
#define KEY_BATTASK_INTERVAL_PERTIME "bat_task_interval_pertime"
#define KEY_CHECK_CU_VERSION "check_cu_version"

#define SECTION_CUI_SEGMENT "cuisegment"
#define KEY_SEGMENT_NUM "segmentnum"

//kdsip配置
#define KEY_MAX_TRANS                 "max_trans"
#define KEY_MAX_REG_DLG               "max_reg_dlg"
#define KEY_MAX_INVITE_DLG            "max_invite_dlg"
#define KEY_MAX_SUBSCRIBE_DLG         "max_subscribe_dlg"
#define KEY_MAX_DISABLE_CHECK_SUM     "max_disable_check_sum"
#define KEY_MAX_NOTIFY_PERSUB         "max_notify_persub"
#define KEY_MAX_CU_NTF_SIZE           "max_cu_notify_size"

#define DEFAULT_IP      "127.0.0.1"
#define DEFAULT_PORT    0
#define DEFAULT_CU_MAX_NUM 100
#define DEFAULT_SUBSCRIBE_FLOW_LIMIT 30
#define DEFAULT_UNSUBSCRIBE_FLOW_LIMIT 1
#define DEFAULT_UUID    ""
#define DEFAULT_TIMEOUT 5
#define DEFAULT_APP_QUEUE_SIZE 10000
#define DEFAULT_ROUTEMODE 0
#define NTF_TRY_SEND_NUM              (3)         //3次
#define DEFAULT_TIMEOUT_RES_TO_CU    (18)        // 从收到CU的请求到对CU的应答，最长总时间

#define DEFAULT_MAX_TRANS            (4096)
#define DEFAULT_MAX_REG_DLG          (2)
#define DEFAULT_MAX_INVITE_DLG       (1024)
#define DEFAULT_MAX_SUBSCRIBE_DLG    (1024)
#define DEFAULT_DISABLE_CHECK_SUM    (0)
#define DEFAULT_MAX_NTF_PERSUB       (5)
#define DEFAULT_UPDATE_TIMER         (60)
#define DEFAULT_CUI_NAT_PORT         (8001)       //NAT探测包接收端口默认8001
#define DEFAULT_BATTASK_RECNUM_PERTIME       (200)
#define DEFAULT_BATTASK_INTERVAL_PERTIME     (1000)
#define DEFAULT_BATTASK_RECNUM_PERTIME_MIN   (10)
#define DEFAULT_BATTASK_INTERVAL_PERTIME_MIN (10)

#define DEFAULT_MAX_CU_NTF_SIZE   MAX_CUI_SEQMARKS_SIZE // CU通知消息队列最大限制 2147483647
#define DEFAULT_MIN_CU_NTF_SIZE   MIN_CUI_SEQMARKS_SIZE // CU通知消息队列最小限制

#define DEF_TASK_TYPE_FILE_EXPORT_TASK_EX	"CExportTaskEx"  // 暂定用 task name
#define DEF_TASK_TYPE_DPS_SEARCH_TASK		"CDpsSearchTask" //

struct TCuiStat
{
	void PrintData()
	{
		OspPrintf(TRUE, FALSE, "  连接Redis状态 :[%s]\n", strRedisStat.c_str());
		OspPrintf(TRUE, FALSE, "  注册3AS状态   :[%s]\n", strTasState.c_str());
		OspPrintf(TRUE, FALSE, "  注册TVS状态   :[%s]\n", strTvsState.c_str());
		OspPrintf(TRUE, FALSE, "  注册ALS状态   :[%s]\n", strAlsState.c_str());
		OspPrintf(TRUE, FALSE, "  注册RCS状态   :[%s]\n", strRcsState.c_str());
		OspPrintf(TRUE, FALSE, "  注册UAS状态   :[%s]\n", strUasState.c_str());
		OspPrintf(TRUE, FALSE, "  注册MPS状态   :[%s]\n", strMpsState.c_str());
		OspPrintf(TRUE, FALSE, "  注册GBS状态   :[%s]\n", strGbsState.c_str());
		OspPrintf(TRUE, FALSE, "  注册DPS状态   :[%s]\n", strDpsState.c_str());

		OspPrintf(TRUE, FALSE, "------------------------------------------------------------------------------\n");
	}

	string strRedisStat;
	string strTasState;
	string strCmuState;
	string strTvsState;
	string strAlsState;
	string strRcsState;
	string strUasState;
	string strMpsState;
	string strGbsState;
	string strDpsState;
};

class CConfig
{
public:
    CConfig();
    bool ReadConfig(LPCSTR lpFileName);
    TSipURI &GetCmuURI();
	TSipURI &GetSecureModuleURI();
    TSipURI &GetLocalURI();
    //string &GetLocalIP();
    u16 GetLocalPort();
    u16 GetTcpListenPort();
    u16 GetCuMaxNum();
    u16 GetSubscribeFlowLimit();
    u16 GetUnsubscribeFlowLimit();
    u32 GetSipWaitRspTimeout();
    u32 GetDbQueryTimeout(){return GetSipWaitRspTimeout()*10;}
    u32 GetDlgWaitRspTimeout();
    u32 GetCuHbTimeout();
    u32 GetAppQueueSize();
    s32 GetRouteMode();
    //kdsip配置相关
    u32 GetMaxTrans();
    u32 GetMaxRegDlg();
    u32 GetMaxInviteDlg();
    u32 GetMaxSubscribeDlg();
	u32 GetMaxNtfPerSub();
    BOOL32 GetDisableCheckSum();
    void SetSipKeyCfg(TKdsipKeyCfg& tKdsipKeyCfg);
    bool IsCuOperateLogEnable(s32 nOperateType);
    s32 GetMobileClientLimit() const;
	string& GetLocalIp() { return m_tLocalIP; }
	u16& GetNatPort() {return m_wNatPort;}
	string& GetMapedNatIP() { return m_tMapedNatIP; }
	u16& GetMapedNatPort() { return m_wMapedNatPort; }

	BOOL32 IsEnableMapedNatAddr(){ return m_bEnableMapedNatAddr; }
	u32 GetUpdateNetInterval(){ return m_dwUpdateNetSegInterval; }
	void UpdateNatMap(u16 wCuiNatPort);
	bool GetNatMapAddr(const string& strSrcIp,const int& nSrcPort,string& strMapIp,int& nMapPort);
	bool IsNatRevPortMaped();
	void UpdateMapedNatAddr();

	// NAT探测包方式建交换，是否需要CUI转码
	bool IsNeedCUISwitch() const;

	u16 GetBatTaskRecNumPerTime() const;
	u16 GetBatTaskIntervalPerTime() const;

	s32 GetMaxCuNtfSize() const;
	void SetMaxCuNtfSize(s32 CuNtfSize);

	bool IsCheckCuVersion(){return m_bCheckCuVersion;};

    /************************************/
    void PrintData(u32 dwOffSet = 0);
	CCommonConfig m_cCmCfg;
protected:
    string  m_tLocalIP;     //SIP地址
    u16     m_wLocalPort;
    u16     m_wListenPort;  //TCP监听地址
    u16     m_wCuMaxNum;    //允许接入的cu最大数
    u16     m_wSubscribeFlowLimit;
    u16     m_wUnsubscribeFlowLimit;
    //以下超时单位皆为秒
    u32     m_dwSipWaitRspTimeout;  //SIP消息等待应答超时时间
    u32     m_dwDlgWaitRspTimeout;  //DLG消息等待应答超时时间
    u32     m_dwCuHbTimeout;        //CU心跳超时
    u32     m_dwGetAppQueueSize;

    s32     m_nRouteMode;           //路由模式

    TSipURI m_tLocalURI;    //CUI的设备UUID@domain name，如果为空则自动生成
    TSipURI m_tCmuURI;      //由cmcfg的domainName拼凑出来，不写入配置文件
	TSipURI m_tSecureModuleURI;

    set<s32> m_setCuOperateLog; // 允许记录日志的CU操作日志类型
    s32     m_nMobileClientLimit;
    
	u16     m_wNatPort;          //NAT探测接收端口
	string  m_tMapedNatIP;       //NAT探测包接收IP（平台NAT外地址）
	u16     m_wMapedNatPort;     //NAT探测包接收端口（平台NAT外端口）

	BOOL32  m_bEnableMapedNatAddr;       //是否是否映射后的NAT外地址
	u32     m_dwUpdateNetSegInterval;    //定时更新网段信息时间间隔

	BOOL32  m_bNeedCUISwitch;    // 是否需要CUI建立CU--VTDU的交换

	BOOL32  m_wBatTaskRecNumPerTime;       //批量任务之单次启停录像数量，最小单次启停数量10
	BOOL32  m_wBatTaskIntervalMsPerTime;   //批量任务之间隔，单位：毫秒，最小间隔10毫秒
	bool    m_bCheckCuVersion;   // 是否校验CU版本

	s32     m_nMaxCuNtfSize;     // CU通知消息队列最大限制
	//kdsip相关
    s32 m_nMaxTrans;
    s32 m_nMaxRegDlg;
    s32 m_nMaxInviteDlg;
    s32 m_nMaxSubscribeDlg;
	s32 m_nMaxNtfPerSub;
    BOOL32 m_bDisableCheckSum;

private:
};

class CElapsedTime
{
public:
    CElapsedTime(const string& strEvent, const TMsTime& dwTime)
    {
        m_strEvent = strEvent;
        m_tReqTime = dwTime;
        CountTime(dwTime);
    }

    const std::string& GetEventStr() const 
    {
        return m_strEvent;
    }
    const int GetEventId() const
    {
        return OspExtEventID(m_strEvent.c_str());
    }
    void SetEvent(const std::string strEvent)
    {
        m_strEvent = strEvent;
    }
    void SetEvent(const u16 wEvent)
    {
        m_strEvent = OspExtEventDesc(wEvent);
    }

    u32 GetElapsedTime()
    {
        return m_dwTotalTime;
    }

    const u32 GetElapsedTime() const
    {
        return m_dwTotalTime;
    }
    void SetElapsedTime(const u32 dwTime)
    {
        m_dwTotalTime = dwTime;
    }

    const TMsTime& GetReqTime() const
    {
        return m_tReqTime;
    }
    void SetReqTime(const TMsTime& dwTime)
    {
        m_tReqTime = dwTime;
    }

    void CountTime(const TMsTime& dwTime)
    {
        TMsTime tCurTime = OspGetMsTime();
        if(tCurTime.tSecond < dwTime.tSecond || 
            (tCurTime.tSecond == dwTime.tSecond && tCurTime.tMilliSecond < dwTime.tMilliSecond))
        {
            m_dwTotalTime = 0;
        }
        else
        {
            m_dwTotalTime = OspGetDiffMs(tCurTime, dwTime);
        }
    }

    bool operator >= ( const CElapsedTime& tOther ) const
    {
       return (m_dwTotalTime >= tOther.m_dwTotalTime);
    }

    bool operator < ( const CElapsedTime& tOther ) const
    {
        return (m_dwTotalTime < tOther.m_dwTotalTime);
    }

    bool operator == ( const CElapsedTime& tOther ) const
    {
       return (m_dwTotalTime == tOther.m_dwTotalTime);
    }

    void PrintData()
    {
        COspTime time(m_tReqTime.tSecond);
        OspPrintf(TRUE, FALSE, "Event[%s] begin time is [%s-%u],elapses time is [%u(ms)]\n", m_strEvent.c_str(),
            time.GetString().c_str(), m_tReqTime.tMilliSecond, m_dwTotalTime);
    }
private:
    string m_strEvent;
    TMsTime m_tReqTime;
    u32 m_dwTotalTime;
};

#define TOP_NUM (10)

class CElapsedTimeList
{
public:
    CElapsedTimeList()
    {
        m_dwMaxNum = TOP_NUM;
    }
public:
    void InsertElapsedTime(const CElapsedTime& time)
    {
        m_tTimeList.push_back(time);
        m_tTimeList.sort();
        while(m_tTimeList.size() > m_dwMaxNum)
        {
            m_tTimeList.pop_front();
        }
    }

    void PrintData()
    {
        int dwElapsedTime = 1;
//         list<CElapsedTime>::iterator itor = m_tTimeList.end();
//         while(itor != m_tTimeList.begin())
//         {
//             OspPrintf(TRUE, FALSE, "\n\t%4d.", dwElapsedTime);
//             itor->PrintData();
//             dwElapsedTime++;
//             itor--;
//         }
        list<CElapsedTime>::reverse_iterator iter = m_tTimeList.rbegin();
        for( ; iter != m_tTimeList.rend(); iter++)
        {
            OspPrintf(TRUE, FALSE, "\n\t%4d.", dwElapsedTime);
            iter->PrintData();
            dwElapsedTime++;
        }
    }

    void Clear()
    {
        m_tTimeList.clear();
    }

    void SetMaxNum(u32 dwNum)
    {
        m_dwMaxNum = dwNum;
    }
private:
    list<CElapsedTime> m_tTimeList;
    u32 m_dwMaxNum;
};
extern CElapsedTimeList g_cElapsedTimeList;

//存储一些全局数据
extern CCuiDataMgr g_cCuiDataMgr;
extern CConfig g_cCuiConfig;
//extern CTaskNOQueue g_cTaskNOQue;

typedef TDeviceVideoSrcIdList TVideoSrcStatList;
class CVideoSrcStatList
{
public:
	CVideoSrcStatList();

public:
	bool IsFinished() const;
	void SetFinished();
	
	void SetCreating();

	int GetOnlineDeviceNum() const;
	void SetOnlineDeviceNum(int onlineDeviceNum);

	int GetIdleDeviceNum() const;
	void SetIdleDeviceNum(int idleDeviceNum);

	int GetOfflineDeviceNum() const;
	void SetOfflineDeviceNum(int offlineDeviceNum);

	int GetErrorCode() const;
	void SetErrorCode(int errorCode);

	time_t GetUsingLastTime() const;
	void SetUsingLastTime();

	string GetCuQueryTag() const;
	void SetCuQueryTag(string cuQueryTag);

	string GetTaskQueryTag() const;
	void SetTaskQueryTag(string taskQueryTag);

	string GetTaskNo() const;
	void SetTaskNo(string taskNo);

	// devNum
	u32 GetVideoSrcStatListSize();

	bool GetVideoSrcStatList( int beginIndex, int entryNum, TDeviceVideoSrcIdList &devVideoSrcIdList);

	void AddVideoSrcStatList( const TVideoSrcStatList& devVideoIdList, bool isFinised = false);

	void PrintData();

	time_t GetCurTime() const;

	void Clear();

private:
	TVideoSrcStatList m_videoSrcStatList;	// 视频源状态列表

	int m_onlineDeviceNum;	// 在线数量
	int m_idleDeviceNum;	// 未启用数量
	int m_offlineDeviceNum;	// 离线数量

	int m_errorCode;	// 错误码

	string m_cuQueryTag;	// cu查询标识
	string m_taskQueryTag;	// taskQueryTag
	string m_taskNo;		// taskNO.
	time_t m_createTime;	// 列表创建时间
	time_t m_finishTime;	// 列表生成完成的时间
	time_t m_cuUsingLastTime;	// CU使用的最后时间，超过时效就删除
};

// map:<domain, VideoSrcStatList> 不同域名下的视频源状态列表
typedef std::map<string, CVideoSrcStatList> CMapVideoSrcStatList;

// map:<CuSession, TMapVideoSrcStatList> 不同用户的视频源状态列表
typedef std::map<string, CMapVideoSrcStatList> CMapCuVideoSrcStatList;

extern CMapCuVideoSrcStatList g_mapDevVideoSrcStatList;

extern TMappingTable g_tMappingTable;

// 设备、视频源、等权限信息打印转换(仅用于调试时的打印)
string GetPriString(const CEncoderPrivilege &cPri);
string GetPriString(const CVideosourcePrivilege &cPri);
string GetPriString(const CUserPrivilege &cPri);
#endif


