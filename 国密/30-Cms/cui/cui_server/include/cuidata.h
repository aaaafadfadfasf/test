/*****************************************************************************
   ģ����      : cui_fcgi 
   �ļ���      : cuidata.cpp
   ����ļ�    : 
   �ļ�ʵ�ֹ���: cuidata ����CUI��ȫ�ֱ���
   ����        : liangli
   �汾        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2011/11/10   1.0         liangli        ����
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
typedef CCuAgent * PCuAgent;    //CuAgentָ��
class CUserMgr;
typedef CUserMgr * PCUserMgr;

#define REDIS_DEF_PORT 6379
#define REDIS_SEPARATOR ":"
#define REDIS_NTF_CHN_NAME_SIZE 1024 //1k�ֽ�
#define REDIS_NTF_MSG_SIZE (10*1024*1024) // 10M�ֽ�

#define REDIS_MAX_QUERY_DEVICE_INFO_ENTRY_NUM 100//CU����������ѯ�豸��Ϣ���������� 

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
	u32		m_dwSrcIp;		//ԴIP;
	u16		m_wSrcPort;		//Դ�˿�;
	u32		m_dwNatIp;		//NAT IP��ַ;
	u16		m_wNatPort;		//NAT �˿�;
	u32		m_dwPalyID;		//�ỰID;
	s8      m_szCuid[32+1];
	u8      m_byReserve;    //�������μ�EMNatPackMediaTransChnType

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
    //����PTZ��Ȩ�޵ȼ�
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
    string m_tUserID;       //UUID����������
    string m_strUserName;   //�û���, ������
    string m_strPasswd;     //��������
    string m_tUserGroupID;  //�û���ID
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

/*�û������û���ʱȨ�����ݵ�������ݽ������û���*/
class CUserAccount : public CUserAccountBaseInfo
{
private:
    /*
        2012.4.6�޸ģ���ΪTAS�ܶ�ʱ��û��ͬʱ�ṩ�豸ID��DomainID����ô���޷�������ȷ��tDevURI
        Ҳ���޷�����������DevURI��ѯ�豸
        ��ΪtDevUUID�Ǿ���Ψһ�ģ����Կ��Ը���tDevUUID����ѯ
        ����m_mapDevPriList��key��tDevUUID��value��������tDevURI
    */
    list<PCuAgent> m_lstCuAgentList;                  //���˻��µ�ͬ������CU
    PCUserMgr m_pcUserMgr;                            //��������ͬһ��3acAgent�е�UserGroupList

public:
    EUserDevPriListState m_bDevPriState; //��ǻ�ȡ�豸Ȩ���б��״̬�������û���ʱ�ñ�־��Ч
    u32 m_dwDevPriQueryTaskNo;           //��ȡ�豸Ȩ���б��TASK��, ��ֹ���ڶ����ѯTASK
    u32 m_dwStreamConcurrency;
public:
    CUserAccount();
    CUserAccount(PCUserMgr pcUserMgr);
    ~CUserAccount();
    void PrintData(u32 dwOffSet = 0, u32 dwMaxDev =(~(u32)0));

    s32 GetUserType(); // ����û������û����У������������ͣ����򷵻��û��������
	bool IsSuperAdmin();
    bool IncreaseConcurrency();
    void DecreaseConcurrency();

    CPrivilege &GetUserPri();

    list<PCuAgent>& GetCuAgentList();
    bool AddCuAgent(PCuAgent pcCuAgent);
    bool DeleteCuAgent(const PCuAgent pcCuAgent);

	// ֪ͨCU
    bool DelUserDevInfo(const string &strDevUri, bool bIsTvWall); // ����ǵ���ǽ���򲻷�֪ͨ
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
    PCUserMgr m_pcUserMgr;              //��������ͬһ��3acAgent�е�UserGroupList
    /*
        2012.4.6�޸ģ���ΪTAS�ܶ�ʱ��û��ͬʱ�ṩ�豸ID��DomainID����ô���޷�������ȷ��tDevURI
        Ҳ���޷�����������DevURI��ѯ�豸
        ��ΪtDevUUID�Ǿ���Ψһ�ģ����Կ��Ը���tDevUUID����ѯ
        ����m_mapDevPriList��key��tDevUUID��value��������tDevURI
    */
public:
    EUserDevPriListState m_bDevPriState; //��ǻ�ȡ�豸Ȩ���б��״̬
    u32 m_dwDevPriQueryTaskNo;           //��ȡ�豸Ȩ���б��TASK��, ��ֹ���ڶ����ѯTASK

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

	// �Ƿ���Ч
	bool IsValid() const ;
	
	// �Ƿ����
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
	string m_strDeviceId;      // �豸id
	int m_nVideoSrcId;       // ��ƵԴid
	string m_strBeginTime;     // ������ʼʱ��.
	string m_strEndTime;       // �Զ�����ʱ��.
	string m_strUserId;        // �����û�
	string m_strUserName;      // �û���		
	string m_strPassWord;      // �ֶ���������
	int m_nUserPtzLevel;   // �û�PTZ�ȼ�
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

	// �����ƵԴ��PTZ����ָ���û���ӵģ���ô��ɾ��
	bool DelCuPtzLockInfoByUserWithVsChn(const string &strUserId, const TChannel &devVsChn);

	// ͨ����ƵԴIDɾ����
	void DelCuPtzLockInfoByVs(const TChannel &devVsChn);

	// ͨ���û�IDɾ����
	void DelCuPtzLockInfoByUserId(const string &strUserId);

	// ͨ���豸IDɾ����
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
    CUI�е��û������Ϊ���㣺1��3AC�˻���2���û���/�û��˻���3��CuAgent
    3AC�˻�������һ����������û����û���
    �����û��˻���Ӧ���CuAgent��ÿ��CuAgentΨһ��Ӧһ��CU�ͻ���
    ����û������ڣ��������û�����Ϣ
*/
class CUserMgr
{
public:
    CUserMgr(){}
    ~CUserMgr();

    CStrMap<string> m_mapUserID2UserName;                   //Ϊ�˷������UserID����UserAccount
    CStrMap<CUserAccount*> m_mapUserName2PCuserAccount;     //�û��˻��б��û����������У���Ҫ����userGrpId�ж�
    CStrMap<CUserGroup> m_mapGroupID2UserGroup;             //��������û�, ��Щ�û��������û���һ��
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
    //cmu��cui�����session
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
	enBatOptTypeInvalid = -1,	// ��Чֵ
	enBatOptStartPlatRecord = 0,	// ��������ƽ̨¼��
	enBatOptStopPlatRecord = 1,	// ����ֹͣƽ̨¼��
	enBatOptStartPuRecord = 2,	// ��������ǰ��¼��
	enBatOptStopPuRecrod = 3,	// ����ֹͣǰ��¼��
	enBatOptStartWiper = 4,	// ����������ˢ
	enBatOptStopWiper = 5,	// ����ֹͣ��ˢ
	enBatOptStopPuCall = 6,	// ����ֹͣ��Ƶ����
	enBatOptStartPresetCall=7 //��������Ԥ��λ

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

	// ֱ��ɾ����һ����¼
	void PopFront();
	// ɾ����һ����¼
	bool PopFrontByBatOptInfo(TBatOptInfo &frontBatOptInfo);

	// ��ʼ��һ���������� ����TaskNo
	bool SetFrontBatOptTaskNo(const string strReq, const int &nTaskNo);

	bool GetFrontBatOptInfo(TBatOptInfo &frontBatOptInfo);

	// �����������
	void PushBackBatOptStrReq(const string &strReq);

	// ���õ�һ����������Ľ���
	bool SetFrontBatOptPos(const TBatOptInfo &frontBatOptInfo);
	
private:
	list<TBatOptInfo> m_batOptReqInfolist; // �����Ⱥ�˳���¼���������������
};

class CCuiDataMgr
{
public:
    CSessionToTaskMap m_mapSessToTaskMap;       //�û��ỰID��CuAgent�Ķ�Ӧ��ϵ������QueryTag��TAS QueryTask��Ӧ��ϵ
    C3ASNameToTaskMap m_map3asURITo3asAgentMap; //������3asAgent�Ķ�Ӧ��ϵ
    //CDomainInfoMap m_mapDomainInfo;  //��������ϵͳ��CUI��¼��3AS�����б�
    CPeNameToPeTaskMap m_mapPeNameToPeTaskMap;  //����������CPeTask�Ķ�Ӧ��ϵ(��������)
    //////////////////////////////////////////////////////////
    CDevUriTypeToSubTaskMap m_mapDevUriTypeToSubTaskMap;
    //////////////////////////////////////////////////////////
    CCuiState m_cCuiState;
	//////////////////////////////////////////////////////////
	// ���û��CU��½��3A�����󲻻�������������������ȫ��ȡ��
	// CMU�������ͷ�����task��ȡ��������������
	// rcs���������������ȡ����������
	CuBatOptList m_cPuBatOptRequestList;
	CuBatOptList m_cPlatBatOptRequestList;
	u32 m_dwBatOptCheckTaskNo;

public:
    CCuiDataMgr();
    void ClearAllData();
    void PrintData(const CInstExt *pcInst, u32 dwOffSet = 0);
    void SetCuiBeginRunTime();
    void SetCuiStateHappenTime();

    // pcAgentΪ��ʱˢ״̬�����е�CU, tPeUriΪ��ʱˢ�����е�ģ��
    void FlushPlatformServiceStatus(const CInstExt *pcInst, CCuAgent *pcAgent = NULL, const TSipURI &tPeUri=TSipURI()); 
public:
    //ȫ�����ݽӿ�
    
    /*
        Ŀǰֻ���û��ỰID��CuAgent�Ķ�Ӧ��ϵ������QueryTag��TAS QueryTask��Ӧ��ϵ
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
#define KEY_TCP_TIMEOUT "tcp_timeout"       //��cui.fcgiʹ�õĳ�ʱ
#define KEY_SIP_RSP_TIMEOUT "sip_rsp_timeout"   //�ȴ�SIPӦ��ʱ
#define KEY_DLG_RSP_TIMEOUT "dlg_rsp_timeout"   //�ȴ�SIPӦ��ʱ
#define KEY_CU_HB_TIMEOUT   "cu_hb_timeout" //CU������ʱ
#define KEY_APP_QUEUE_SIZE "app_queue_size"
#define KEY_CU_MAX_NUM "cu_max_num" //CU��������
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

//kdsip����
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
#define NTF_TRY_SEND_NUM              (3)         //3��
#define DEFAULT_TIMEOUT_RES_TO_CU    (18)        // ���յ�CU�����󵽶�CU��Ӧ�����ʱ��

#define DEFAULT_MAX_TRANS            (4096)
#define DEFAULT_MAX_REG_DLG          (2)
#define DEFAULT_MAX_INVITE_DLG       (1024)
#define DEFAULT_MAX_SUBSCRIBE_DLG    (1024)
#define DEFAULT_DISABLE_CHECK_SUM    (0)
#define DEFAULT_MAX_NTF_PERSUB       (5)
#define DEFAULT_UPDATE_TIMER         (60)
#define DEFAULT_CUI_NAT_PORT         (8001)       //NAT̽������ն˿�Ĭ��8001
#define DEFAULT_BATTASK_RECNUM_PERTIME       (200)
#define DEFAULT_BATTASK_INTERVAL_PERTIME     (1000)
#define DEFAULT_BATTASK_RECNUM_PERTIME_MIN   (10)
#define DEFAULT_BATTASK_INTERVAL_PERTIME_MIN (10)

#define DEFAULT_MAX_CU_NTF_SIZE   MAX_CUI_SEQMARKS_SIZE // CU֪ͨ��Ϣ����������� 2147483647
#define DEFAULT_MIN_CU_NTF_SIZE   MIN_CUI_SEQMARKS_SIZE // CU֪ͨ��Ϣ������С����

#define DEF_TASK_TYPE_FILE_EXPORT_TASK_EX	"CExportTaskEx"  // �ݶ��� task name
#define DEF_TASK_TYPE_DPS_SEARCH_TASK		"CDpsSearchTask" //

struct TCuiStat
{
	void PrintData()
	{
		OspPrintf(TRUE, FALSE, "  ����Redis״̬ :[%s]\n", strRedisStat.c_str());
		OspPrintf(TRUE, FALSE, "  ע��3AS״̬   :[%s]\n", strTasState.c_str());
		OspPrintf(TRUE, FALSE, "  ע��TVS״̬   :[%s]\n", strTvsState.c_str());
		OspPrintf(TRUE, FALSE, "  ע��ALS״̬   :[%s]\n", strAlsState.c_str());
		OspPrintf(TRUE, FALSE, "  ע��RCS״̬   :[%s]\n", strRcsState.c_str());
		OspPrintf(TRUE, FALSE, "  ע��UAS״̬   :[%s]\n", strUasState.c_str());
		OspPrintf(TRUE, FALSE, "  ע��MPS״̬   :[%s]\n", strMpsState.c_str());
		OspPrintf(TRUE, FALSE, "  ע��GBS״̬   :[%s]\n", strGbsState.c_str());
		OspPrintf(TRUE, FALSE, "  ע��DPS״̬   :[%s]\n", strDpsState.c_str());

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
    //kdsip�������
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

	// NAT̽�����ʽ���������Ƿ���ҪCUIת��
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
    string  m_tLocalIP;     //SIP��ַ
    u16     m_wLocalPort;
    u16     m_wListenPort;  //TCP������ַ
    u16     m_wCuMaxNum;    //��������cu�����
    u16     m_wSubscribeFlowLimit;
    u16     m_wUnsubscribeFlowLimit;
    //���³�ʱ��λ��Ϊ��
    u32     m_dwSipWaitRspTimeout;  //SIP��Ϣ�ȴ�Ӧ��ʱʱ��
    u32     m_dwDlgWaitRspTimeout;  //DLG��Ϣ�ȴ�Ӧ��ʱʱ��
    u32     m_dwCuHbTimeout;        //CU������ʱ
    u32     m_dwGetAppQueueSize;

    s32     m_nRouteMode;           //·��ģʽ

    TSipURI m_tLocalURI;    //CUI���豸UUID@domain name�����Ϊ�����Զ�����
    TSipURI m_tCmuURI;      //��cmcfg��domainNameƴ�ճ�������д�������ļ�
	TSipURI m_tSecureModuleURI;

    set<s32> m_setCuOperateLog; // �����¼��־��CU������־����
    s32     m_nMobileClientLimit;
    
	u16     m_wNatPort;          //NAT̽����ն˿�
	string  m_tMapedNatIP;       //NAT̽�������IP��ƽ̨NAT���ַ��
	u16     m_wMapedNatPort;     //NAT̽������ն˿ڣ�ƽ̨NAT��˿ڣ�

	BOOL32  m_bEnableMapedNatAddr;       //�Ƿ��Ƿ�ӳ����NAT���ַ
	u32     m_dwUpdateNetSegInterval;    //��ʱ����������Ϣʱ����

	BOOL32  m_bNeedCUISwitch;    // �Ƿ���ҪCUI����CU--VTDU�Ľ���

	BOOL32  m_wBatTaskRecNumPerTime;       //��������֮������ͣ¼����������С������ͣ����10
	BOOL32  m_wBatTaskIntervalMsPerTime;   //��������֮�������λ�����룬��С���10����
	bool    m_bCheckCuVersion;   // �Ƿ�У��CU�汾

	s32     m_nMaxCuNtfSize;     // CU֪ͨ��Ϣ�����������
	//kdsip���
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

//�洢һЩȫ������
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
	TVideoSrcStatList m_videoSrcStatList;	// ��ƵԴ״̬�б�

	int m_onlineDeviceNum;	// ��������
	int m_idleDeviceNum;	// δ��������
	int m_offlineDeviceNum;	// ��������

	int m_errorCode;	// ������

	string m_cuQueryTag;	// cu��ѯ��ʶ
	string m_taskQueryTag;	// taskQueryTag
	string m_taskNo;		// taskNO.
	time_t m_createTime;	// �б���ʱ��
	time_t m_finishTime;	// �б�������ɵ�ʱ��
	time_t m_cuUsingLastTime;	// CUʹ�õ����ʱ�䣬����ʱЧ��ɾ��
};

// map:<domain, VideoSrcStatList> ��ͬ�����µ���ƵԴ״̬�б�
typedef std::map<string, CVideoSrcStatList> CMapVideoSrcStatList;

// map:<CuSession, TMapVideoSrcStatList> ��ͬ�û�����ƵԴ״̬�б�
typedef std::map<string, CMapVideoSrcStatList> CMapCuVideoSrcStatList;

extern CMapCuVideoSrcStatList g_mapDevVideoSrcStatList;

extern TMappingTable g_tMappingTable;

// �豸����ƵԴ����Ȩ����Ϣ��ӡת��(�����ڵ���ʱ�Ĵ�ӡ)
string GetPriString(const CEncoderPrivilege &cPri);
string GetPriString(const CVideosourcePrivilege &cPri);
string GetPriString(const CUserPrivilege &cPri);
#endif


