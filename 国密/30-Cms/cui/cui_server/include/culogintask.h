/*****************************************************************************
   模块名      : cui_fcgi 
   文件名      : cuitask.h
   相关文件    : 
   文件实现功能: cuitask.h 定义所有的事务
   作者        : liangli
   版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2011/11/9   1.0         liangli        创建
   2013/10/24              huzhiyun       代码重构
******************************************************************************/
#ifndef _CUI_TASK_H_
#define _CUI_TASK_H_
#include "cms/ospext/ospext.h"
#include "cms/ospsip/ospsip.h"
#include "transthread.h"
#include "cuidata.h"
#include "cuistruct.h"
#include "cuidbg.h"
#include "subscribe_task.h"
class CExportTask;
class CExportTaskEx;
class CImportTaskEx;
class CDpsSearchTask;

template <> struct CHashFunctor<TTvChannel>
{
	size_t operator()(const TTvChannel& key) const
	{
		char szTvId[64] = {0};
		sprintf(szTvId, "%d", key.GetTvId());
		char szTvDivId[64] = {0};
		sprintf(szTvDivId, "%d", key.GetTvDivId());
		string strKey = key.GetTvWallId() + szTvId + szTvDivId;

		return hash_ms(strKey.c_str(), 0);
	}
};

template <> struct CEqualFunctor<TTvChannel>
{
	int operator()(const TTvChannel& key1, const TTvChannel& key2) const
	{
		return (key1.GetTvWallId() == key2.GetTvWallId()
			&& key1.GetTvId() == key2.GetTvId()
			&& key1.GetTvDivId() == key2.GetTvDivId());
	}	
};
typedef TRecvNode* PTRecvNode;
typedef CSipTask* PCSipTask;
typedef CXMap<TTvChannel,CTask*> TRecTvTaskSets;

class CInviteTask;
class CPeInviteTask;
class CPeInviteTaskNatSwitch;
//class CExportTask;
class CModuleListSubscribeTaskBase;

typedef CStrMap<u32> CDevUriToSubscribeTaskMap;
typedef CXMap<s32, u32> CPlayIdToInviteTaskMap;
typedef CXMap<u32, PCSipTask> CTaskNOToSubTaskPtrMap;
typedef CXMap<s32, u32> CPlayIdToCallPuTaskMap;

typedef CStrMap<u32> CPeDevUriToSubscribeTaskMap;
typedef CXMap<s32, u32> CPePlayIdToInviteTaskMap;
typedef CXMap<s32, u32> CPePlayIdToInviteTaskNatSwitchMap;
typedef CXMap<u32, PCSipTask> CPeTaskNOToSubTaskPtrMap;

typedef CStrMap<u32> CFileUrlToExportTaskMap;

typedef CStrMap<u32> CDevUriTypeToSubTaskMap;

typedef queue<u32> COnlineCuAgentQueue;

template<typename CRsp>
u32 SendRspToCu(CRsp &cRsp, PTRecvNode &ptNode)
{
    CMS_ASSERT(NULL != ptNode);
    string strXmlVal = cRsp.ToXml();
#ifdef _UNIT_TEST_
    cout<< "send Rsp to cu " << strXmlVal.c_str() <<endl;
#else
    /* 这里发送的是完整的CMsg */
    PostMsgToRecvThread(ptNode, CUI_CU_WEBSERVICE_RSP, strXmlVal.c_str(), strXmlVal.length());
    //值传递变为指针传递后，这里不delete，等到邮箱收到后再删除
//     delete ptNode;
//     ptNode = NULL;
#endif
    return PROCMSG_OK;
}

inline u32 SendRspToCu(string &strRsp, PTRecvNode &ptNode)
{
    CMS_ASSERT(NULL != ptNode);
#ifdef _UNIT_TEST_
    std::cout<< "send Rsp to cu " << strRsp.c_str() <<std::endl;
#else
    /* 这里发送的是完整的CMsg */
    /* 这里发送的是完整的CMsg */
    PostMsgToRecvThread(ptNode, CUI_CU_WEBSERVICE_RSP, strRsp.c_str(), strRsp.length());
    //值传递变为指针传递后，这里不delete，等到邮箱收到后再删除
//     delete ptNode;
//     ptNode = NULL;
#endif
    return PROCMSG_OK;
}

// CU版本定义 [1/22/2015 pangubing]
#define CU_VER_WITHOUT_NATOPT 0x0001       // NAT前的版本 [1/22/2015 pangubing]
#define CU_VER_WITH_NATOPT    0x0002       // NAT优化的版本 [1/22/2015 pangubing]

/* CU登录代理，标识一个CU */
class CCuAgent : public CSipTask
{
public:
    enum
    {
        OFFLINE_STATE = UNKNOWN_STATE + 1,
        WAIT_3AS_REG_RSP_STATE,         //此状态下CUI尝试注册3AS, 有超时
        WAIT_3AS_ADD_CU_RSP_STATE,      //此状态下向3AS添加CU子用户，附着于CUI用户
        WAIT_CMU_ADD_CU_RSP_STATE,      //此状态下向CMU添加CU
        ONLINE_STATE,
    };
public:
    CCuAgent(CInstExt *pcInst, const string &strClientType);
    ~CCuAgent();
    void Clear();
    void PrintData() const;

    bool IsHasDevPrivilege(const string &strDevUri, s32 nPrivilege);
    bool IsMobileClientType() const;

	bool CuiTransStream4PlatRec() const;
	bool CuiTransStream4Pu() const;
public:
    //子类需要重写的方法
    virtual const char* GetObjName() const //必须重载的方法
    {
        return "CCuAgent";
    }
    virtual const char* GetStateName(u32 dwState) const //必须重载的方法
    {
        switch (dwState)
        {
        case OFFLINE_STATE:
            {
                return "Offline State";
            }
            break;
        case WAIT_3AS_REG_RSP_STATE:
            {
                return "Wait 3AS Reg Rsp State";
            }
            break;
        case WAIT_3AS_ADD_CU_RSP_STATE:
            {
                return "Wait 3AS Add Cu Rsp State";
            }
            break;
        case WAIT_CMU_ADD_CU_RSP_STATE:
            {
                return "Wait CMU Add Cu Rsp State";
            }
            break;
        case ONLINE_STATE:
            {
                return "Online State";
            }
            break;
        default:
            break;
        }
        return "Unknown State";
    }
    virtual void InitStateMachine();
protected:
    template <typename CReq>
    u32 FakePostMsgReq(CReq &cReq, const TSipURI &tDstURI)
    {
#ifdef _UNIT_TEST_
        g_cTaskNOQue.push(GetTaskNO());
        cout << "Post To " << tDstURI.GetURIString().c_str() << endl;
        cout << cReq.ToXml().c_str() << endl;
        return PROCMSG_OK;
#else
        return PostMsgReq<CReq>(cReq, tDstURI);
#endif
    }
public:
    //状态函数
    u32 OfflineProcMsg(CMessage *const pcMsg);
    u32 Wait3ASRegRspProcMsg(CMessage *const pcMsg);
    u32 Wait3ASRspTimerPoll();
    u32 Wait3ASAddCuRspProcMsg(CMessage *const pcMsg);

    u32 WaitCmuAddCuRspProcMsg(CMessage *const pcMsg);

    void WriteLog( int dwErrorCode, bool bLogin, bool bEnableDefaultDesc = true); 
	void WriteDeviceModLog(int dwErrorCode, bool bDevGroup);
    void GetOptLogDesc(bool bLogin);

    u32 OnlineProcMsg(CMessage *const pcMsg);
    u32 OnlineTimerPoll();
public:
    //提供的对外方法
    /*====================================================================
    函数名      : StartLogin
    功能        : 开始CU登录
    算法实现    :
    引用全局变量:
    输入参数说明: strUserName  用户名, 例如ligang@Nanjing
                 strPasswd    密码
    返回值说明  : 成功:true，失败:false
    ----------------------------------------------------------------------
    修改记录    :
    日  期      版本        修改人        修改内容
    2011/11/10/             liangli
    ====================================================================*/
    u32 StartLogin(const string &strUserName, const string &strPasswd, const string &strVersion);
    u32 CuHBAndGetNotify(PTRecvNode ptNode, s32 nTimeoutSeconds);
    void ClearCuHBCount();
    void ReplyNotifyRsp(PTRecvNode &ptNode);
    void TryReplyNotifyRsp();

    void SetCMUURI(const TSipURI &tURI);
    void ClearAllSubTask();
    CSipTask *GetInviteTaskPtrByPlayId(const int nPlayId);
    CPeInviteTask *GetPeInviteTaskPtrByPlayId( const int nPlayId );
    CSipTask *GetSubTaskPtrByDevUri(const string& strDev);
    CExportTask *GetExportTaskPtrByFileUrl(const string& strFile);
    void ClearDevPermanentTaskRef(const string &strDevId);
    void AddSubTask(CSipTask *pcSubTask);
    void DelSubTask(const CSipTask *pcSubTask);
    void AddRecTvTask(const TTvChannel& tTvChn,CTask *pcTask);
	void DelRecTvTask(const TTvChannel& tTvChn);
	CTask* GetRecTvTask(const TTvChannel& tTvChn);

    bool IsTasOnline();
    bool IsExistDomainInfo(const string& strDomainName);
    /////////////////////////////////////////////////////////////
    //计算网卡号
    int CalcEthNo(const TIpAddr& strIp);
    //是否有nat关系
    int IsHasNat(const TIpAddr& tServiceIp, const TIpAddr& tHostIp);
	string GetHostIp(const string& strHost);

	void OnUpdateCuProperty();

private:
    void FillOnlineReq( CCuOnlineReq &cReq, const string &strSession );

protected:
public:
    //子类自己的成员
    string m_strUserName;
    string m_strPassWord;
    string m_strVersion;
    string m_strSession;            //CMU为CU分配的session
    string m_strCuSessionIn3AS;     //3AS为CU分配的session
    string m_str3asName;
    string m_strClientIp;
	string m_strClientLocalIp;
	string m_strWebCuClientIp;
    string m_strHostIp;
	ENATMode m_ePlatNat;
	ENATMode m_eClientNat;
    bool m_bIsSupportNA;
    string m_strClientType;
    string m_strLogDesc;
    string m_strServerDataVersion;
	s32 m_nPwdLockRemainTime;           //密码被锁定剩余时间，单位：秒               
	s32 m_nPwdExpireRemainTime;         //密码有效期到期剩余时间
	u32 m_nPwdRemainError;              //密码失效错误码
    TSipURI m_tCmuURI;
    PTRecvNode m_ptCuLoginNode;
    PTRecvNode m_ptCuLogoutNode;
    CCuNotifyPool m_cNotifyPool;       //缓存各种平台通知消息
    PTRecvNode m_ptNotifyRecvNode;     //等待通知的RecvNode
    s32 m_nNotifyRecvNodeTimeout;      //等待通知的超时时间
    bool m_bNeedPlatformServiceStatue; //需要平台服务状态
	bool m_bClientSupCuiTransPlatRec;  // 客户端是否支持CUI转发平台录像回放/下载

    CUserAccount *m_pcUserAccount;     //CuAgent保存自己的账户信息
    CPlayIdToInviteTaskMap      m_cInviteTaskSet;    //媒体流事务集合
    //外设模块的子Task复用原有的task存储形式(新增部分)
    CPePlayIdToInviteTaskMap    m_cPeInviteTaskSet;  //媒体流事务集合
    CPlayIdToCallPuTaskMap      m_cCallPuTaskSet;

	set<u32>  m_setDpsSearchTaskNo;

    set<CModuleListSubscribeTaskBase*> m_setModuleListSubTask;
    CDevUriTypeToSubTaskMap m_cDevSubTaskSet;     // 订阅任务
    CFileUrlToExportTaskMap m_cFileExportTaskSet; //导出task
    //todo 临时子任务应该去掉了，这些临时任务非长久，自生自灭
    CTaskNOToSubTaskPtrMap      m_cSubTaskSet;          //其他临时任务

    TNatInfo m_tNatInfo;
	u32 m_dwClientVer;  // 客户端版本 [1/22/2015 pangubing]
	TRecTvTaskSets m_tRecordTaskSets;   //记录录像上墙任务
 };
s32 GetMobileClientNumber();
bool IsMobileClientType(const string &strClientType);

/* TAS登录代理，标识一个TAS */
class C3acAgent : public CSipTask
{
public:
    enum
    {
        OFFLINE_STATE = UNKNOWN_STATE + 1,
        ONLINE_STATE,
        RELOGIN_STATE   //当TAS上还有在线CU时，需要尝试不断重连TAS，以便尽快回复CU的TAS操作
    };
#define TACAGENT_EMPTY_USERACCOUNT_CHECK_COUNT 5
public:
    C3acAgent(CInstExt *pcInst);
    ~C3acAgent();
public:
    //子类需要重写的方法
    virtual const char* GetObjName() const //必须重载的方法
    {
        return "C3acAgent";
    }
    virtual const char* GetStateName(u32 dwState) const  //必须重载的方法
    {
        switch (dwState)
        {
        case OFFLINE_STATE:
            {
                return "Offline State";
            }
            break;
        case ONLINE_STATE:
            {
                return "Online State";
            }
            break;
        case RELOGIN_STATE:
            {
                return "Relogin State";
            }
            break;
        default:
            break;
        }
        return "Unknown State";
    }
    void TerminateTask();
    virtual void InitStateMachine();
protected:
    template <typename CReq>
    u32 FakePostMsgReq(CReq &cReq, const TSipURI &tDstURI)
    {
#ifdef _UNIT_TEST_
        g_cTaskNOQue.push(GetTaskNO());
        cout << "Post To " << tDstURI.GetURIString().c_str() << endl;
        cout << cReq.ToXml().c_str() << endl;
        return PROCMSG_OK;
#else
        return PostMsgReq<CReq>(cReq, tDstURI);
#endif
    }
    bool FakeSetHBParam(const TSipURI &tDstURI)
    {
#ifdef _UNIT_TEST_
        return true;
#else
        return SetHBParam(tDstURI);
#endif
    }
    bool FakeStopHB(const TSipURI& tHbDst)
    {
#ifdef _UNIT_TEST_
        return true;
#else
        return OspSipStopHB(tHbDst);
#endif
    }
public:
    //状态函数
    u32 OfflineProcMsg(CMessage *const pcMsg);
    u32 OnlineProcMsg(CMessage *const pcMsg);
    u32 OnlineTimerPoll();
    u32 CheckUserOnline();
    u32 ReloginProcMsg(CMessage *const pcMsg);
    u32 ReloginTimerPoll();
public:
    COnlineCuAgentQueue GetCuAgentList();
    void ClearDataWhenTASOfflineOrDisconnect();
    void EnterWaitReloginState();
    void FlashTimePoll();
public:
    string GetDomainNameByDomainID(const string& strDomainID);
    void ClearDomainResource(const string& strDomainName);
    bool IsExistDomainInfo(const string& strDomainName);

	void Del3APtzLockInfoByVs(const TChannel &tVsChn);
	void GetMapDomainInfo();

public:
    //子类自己的成员
    TSipURI m_t3asURI;
    string m_strSessionID;  //CUI作为客户端在3AS的会话
    string m_str3ASDomainName;
    string m_t3ASDomainID;
    /*
        为了让UserAccount等数据保持最新
        当一个UserAccount账户的最后一个CU退出时，删除UserAccount。
        当一个UserGroup的最后一个UserAccount被删除时，删除UserGroup。
        当一个3asAgent的UserAccount列表和UserGroup列表都清空时，删除3asAgent

        在3AS的TimerPoll中定期检查以上数据
        为了防止定时器恰好在CU登录请求过程中检查并删除了3ASAgent，所以设置一个引用计数
        只有当引用计数为 0 时才检查

        要求CuAgent在开始与3AS交互时调用BeginRefCount()，结束时调用EndRefCount()
    */
    //u32 m_dwRefCounter;         //其他CU对3AS的操作引用
    CUserMgr m_cUserMgr;
    vector<string> m_vctTmpQueryTag;        //临时记录查询的Tag,用于断链的资源清理
    CDomainInfoMap m_mapDomainInfo;
	bool m_bGetMapDomainInfoFinish;
	CCuPtzLockInfoList m_cPtzlockInfoList;
};

/* 外设登录代理，标识一个外设 */
class CPeAgentBase : public CSipTask //外设模块的基类task，用于自动管理子类task表以及自动维护心跳
{
public:
    typedef vector<u32> TCallBackTaskList;

    CPeAgentBase(CInstExt *pcInst);
    ~CPeAgentBase();
    void PrintData() const;
public:
	virtual const char* GetObjName() const
	{
		return "CPeAgentBase";
	}

	virtual const char* GetStateName(u32 dwState) const
	{
		return "CPeAgentBaseUnknown";
	}
protected:
    template <typename CReq>
    u32 FakePostMsgReq(CReq &cReq, const TSipURI &tDstURI)
    {
#ifdef _UNIT_TEST_
        g_cTaskNOQue.push(GetTaskNO());
        cout << "Post To " << tDstURI.GetURIString().c_str() << endl;
        cout << cReq.ToXml().c_str() << endl;
        return PROCMSG_OK;
#else
        return PostMsgReq<CReq>(cReq, tDstURI);
#endif
    }
    bool FakeSetHBParam(const TSipURI &tDstURI)
    {
#ifdef _UNIT_TEST_
        return true;
#else
        return SetHBParam(tDstURI);
#endif
    }
    bool FakeStopHB(const TSipURI& tHbDst)
    {
#ifdef _UNIT_TEST_
        return true;
#else
        return OspSipStopHB(tHbDst);
#endif
    }
public:
    virtual u32 StartLogin() = 0;
    virtual u32 StartLogOut() = 0;
    virtual void TerminateTask() = 0;
    void ClearDataWhenPeOfflineOrDisconnect();
    void CallBackTasks();
    bool IsEmpty();
    CSipTask * GetSubTaskPtrByDevURI(const string &strDevURI);
    //void ReplaceReqSession();
public:
    //子类自己的成员
    TSipURI m_tPeURI;       //外设模块的URI
    string m_strReqMsg;
    string m_strSessionID;  //CUI作为客户端在外设的会话ID

    //只缓存持久事务，瞬时查询task等无需缓存
    //暂时用不到
    CDevUriToSubscribeTaskMap   m_cSubscribeTaskSet;    //订阅设备事务集合，基于设备订阅，每个devURI对应一个SubscribeTask, 包含最多4中订阅类型
    CModuleListSubscribeTaskBase *m_pcModuleListSubscribeTask;
    CPlayIdToInviteTaskMap      m_cInviteTaskSet;       //媒体流事务集合
    CFileUrlToExportTaskMap     m_cFileExportTaskSet;

	set<u32>  m_setDpsSearchTaskNo;

    TCallBackTaskList m_tCallBackTaskList; //需要激活的task表
};
class CPeAgent : public CPeAgentBase
{
public:
    enum
    {
        Idle = UNKNOWN_STATE + 1,
        WaitRegRsp,
        Service,
    };
public:
    CPeAgent(CInstExt *pcInst);
    virtual ~CPeAgent();
    void PrintData() const;
public:
    virtual const char* GetObjName() const
    {
        return "CPeAgent";
    }

    virtual const char* GetStateName(u32 dwState) const
    {
        switch (dwState)
        {
        case Idle:
            return "Idle";
        case WaitRegRsp:
            return "WaitRegRsp";
        case Service:
            return "Service";
        default:
            break;
        }
        return "Unknown State";
    }
    void TerminateTask();
    virtual void InitStateMachine();
public:
    //状态处理
    u32 OnIdle(CMessage *const pcMsg);
    u32 OnIdleTimer();

    u32 OnWaitRegRsp(CMessage *const pcMsg);
    u32 OnWaitRegRspTimer();

    u32 OnService(CMessage *const pcMsg);
    u32 OnServiceTimer();
    void DisProc(CMessage *const pcMsg);
public:
    u32 StartLogin();  //虚方法用于实现各种外设模块的登陆
    u32 StartLogOut();
};

/* CU登录过程中获取用户信息和设备权限信息，用户属于组时自动获取用户组的信息。CGetUserInfo为入口task */
class CGetUserInfo : public CSipTask
{
public:
    enum
    {
        NOT_QUERY_STATE = UNKNOWN_STATE + 1,        //空闲状态，未发送查询请求
        WAIT_RESPONSE_STATE,                        //用户信息回响应
        WAIT_NOTIFY_STATE,                          //用户信息通知消息
    };
public:
    CGetUserInfo(CInstExt *pcInst, const string& str3asName, const string& strUserName, const string& strSession);
    ~CGetUserInfo();
public:
    //子类需要重写的方法
    virtual const char* GetObjName() const //必须重载的方法
    {
        return "CGetUserInfo";
    }
    virtual const char* GetStateName(u32 dwState) const //必须重载的方法
    {
        switch (dwState)
        {
        case NOT_QUERY_STATE:
            {
                return "Not Query State";
            }
            break;
        case WAIT_RESPONSE_STATE:
            {
                return "Wait Response State";
            }
            break;
        case WAIT_NOTIFY_STATE:
            {
                return "Wait Notify State";
            }
            break;
        default:
            break;
        }
        return "Unknown State";
    }
    virtual void InitStateMachine();
protected:
    template <typename CReq>
    u32 FakePostMsgReq(CReq &cReq, const TSipURI &tDstURI)
    {
#ifdef _UNIT_TEST_
        g_cTaskNOQue.push(GetTaskNO());
        cout << "Post To " << tDstURI.GetURIString().c_str() << endl;
        cout << cReq.ToXml().c_str() << endl;
        return PROCMSG_OK;
#else
        return PostMsgReq<CReq>(cReq, tDstURI);
#endif
    }
public:
    u32 StartQryUserInfo();
    u32 NotQueryProcMsg(CMessage *const pcMsg);
    u32 WaitRspProcMsg(CMessage *const pcMsg);
    u32 WaitNotifyProcMsg( CMessage *const pcMsg );
    u32 WaitRspTimerPoll();
    u32 WaitNotifyTimerPoll();
    u32 ProcNotifyMsg(CMessage *const pcMsg);
public:
    void WriteLog(const string& strLogDesc);
    string GetOptLogDesc(const CUserAccount* pcUserAccount);
public:
    string m_strSession;
    string m_str3asName;
    string m_strUserName;
    string m_strQueryTag;       //当其不为空时，表明在m_mapSessToTaskMap中登记过
    std::list<string> m_VecNtfReqList; //缓存在rsp消息中接受到得ntf消息

private:
    u32 DoStartQryUserInfo();
};

class CGetGroupInfo : public CSipTask
{
public:
    enum
    {
        NOT_QUERY_STATE = UNKNOWN_STATE + 1,        //空闲状态，未发送查询请求
        WAIT_RESPONSE_STATE,                        //用户信息回响应
        WAIT_NOTIFY_STATE,                          //用户信息通知消息
    };
public:
    CGetGroupInfo(CInstExt *pcInst, const string& str3asName, const string& strGroupID);
    ~CGetGroupInfo();
public:
    //子类需要重写的方法
    virtual const char* GetObjName() const //必须重载的方法
    {
        return "CGetGroupInfo";
    }
    virtual const char* GetStateName(u32 dwState) const //必须重载的方法
    {
        switch (dwState)
        {
        case NOT_QUERY_STATE:
            {
                return "Not Query State";
            }
            break;
        case WAIT_RESPONSE_STATE:
            {
                return "Wait Response State";
            }
            break;
        case WAIT_NOTIFY_STATE:
            {
                return "Wait Notify State";
            }
            break;
        default:
            break;
        }
        return "Unknown State";
    }
    virtual void InitStateMachine();
protected:
    template <typename CReq>
    u32 FakePostMsgReq(CReq &cReq, const TSipURI &tDstURI)
    {
#ifdef _UNIT_TEST_
        g_cTaskNOQue.push(GetTaskNO());
        cout << "Post To " << tDstURI.GetURIString().c_str() << endl;
        cout << cReq.ToXml().c_str() << endl;
        return PROCMSG_OK;
#else
        return PostMsgReq<CReq>(cReq, tDstURI);
#endif
    }
public:
    u32 StartQryGroupInfo();
    u32 NotQueryProcMsg(CMessage *const pcMsg);
    u32 WaitRspProcMsg(CMessage *const pcMsg);
    u32 WaitNotifyProcMsg( CMessage *const pcMsg );
    u32 WaitRspTimerPoll();
    u32 WaitNotifyTimerPoll();
    u32 ProcNotifyMsg(CMessage *const pcMsg);
public:
    string m_str3asName;
    string m_strGroupID;
    string m_strQueryTag;       //当其不为空时，表面在m_mapSessToTaskMap中登记过
    std::list<string> m_VecNtfReqList; //缓存在rsp消息中接受到得ntf消息

private:
    u32 DoStartQryGroupInfo();
};

/* TAS断链处理TASK，处理逻辑如下：
判断是否有CuAgent在线，
1，首先清除CuAgent中的Session
2，建立一个新task CReLoginTasTask，保存3AS DRI，并保存CuAgent的taskid（而不是指针）
3，定期注册3AS，如果注册成功则返回下一状态，
4，尝试注册每个CuAgent，注册成功就更新CuAgent的Session
5，如果所有CuAgent都注册成功，则删除任务

所有的向TAS的请求，都要判断Session是否为空
*/
class CReLoginTasTask : public CSipTask
{
public:
    enum
    {
        LOGIN_TAS_STATE = UNKNOWN_STATE + 1,
        LOGIN_CU_STATE,
    };
    CReLoginTasTask(CInstExt *pcInst);
    ~CReLoginTasTask();
public:
    //子类需要重写的方法
    virtual const char* GetObjName() const //必须重载的方法
    {
        return "CReLoginTasTask";
    }
    virtual const char* GetStateName(u32 dwState) const  //必须重载的方法
    {
        switch (dwState)
        {
        case LOGIN_TAS_STATE:
            {
                return "Login TAS State";
            }
            break;
        case LOGIN_CU_STATE:
            {
                return "Login CU State";
            }
            break;
        default:
            break;
        }
        return "Unknown State";
    }
    virtual void InitStateMachine();
public:
    template <typename CReq>
    u32 FakePostMsgReq(CReq &cReq, const TSipURI &tDstURI)
    {
#ifdef _UNIT_TEST_
        g_cTaskNOQue.push(GetTaskNO());
        cout << "Post To " << tDstURI.GetURIString().c_str() << endl;
        cout << cReq.ToXml().c_str() << endl;
        return PROCMSG_OK;
#else
        return PostMsgReq<CReq>(cReq, tDstURI);
#endif
    }
    u32 LoginTasProcMsg(CMessage *const pcMsg);
    u32 LoginCuProcMsg(CMessage *const pcMsg);
    u32 LoginTasTimerPoll();
    u32 LoginCuTimerPoll();
public:
    u32 LoginCuReq();
    u32 LoginTasReq();
public:
    TSipURI m_t3asURI;
    u32 m_dw3acAgentTask;
    COnlineCuAgentQueue m_tCuAgentIds;
    int m_nReqSeqNum;
};

#endif


