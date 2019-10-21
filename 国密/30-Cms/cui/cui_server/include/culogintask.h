/*****************************************************************************
   ģ����      : cui_fcgi 
   �ļ���      : cuitask.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: cuitask.h �������е�����
   ����        : liangli
   �汾        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2011/11/9   1.0         liangli        ����
   2013/10/24              huzhiyun       �����ع�
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
    /* ���﷢�͵���������CMsg */
    PostMsgToRecvThread(ptNode, CUI_CU_WEBSERVICE_RSP, strXmlVal.c_str(), strXmlVal.length());
    //ֵ���ݱ�Ϊָ�봫�ݺ����ﲻdelete���ȵ������յ�����ɾ��
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
    /* ���﷢�͵���������CMsg */
    /* ���﷢�͵���������CMsg */
    PostMsgToRecvThread(ptNode, CUI_CU_WEBSERVICE_RSP, strRsp.c_str(), strRsp.length());
    //ֵ���ݱ�Ϊָ�봫�ݺ����ﲻdelete���ȵ������յ�����ɾ��
//     delete ptNode;
//     ptNode = NULL;
#endif
    return PROCMSG_OK;
}

// CU�汾���� [1/22/2015 pangubing]
#define CU_VER_WITHOUT_NATOPT 0x0001       // NATǰ�İ汾 [1/22/2015 pangubing]
#define CU_VER_WITH_NATOPT    0x0002       // NAT�Ż��İ汾 [1/22/2015 pangubing]

/* CU��¼������ʶһ��CU */
class CCuAgent : public CSipTask
{
public:
    enum
    {
        OFFLINE_STATE = UNKNOWN_STATE + 1,
        WAIT_3AS_REG_RSP_STATE,         //��״̬��CUI����ע��3AS, �г�ʱ
        WAIT_3AS_ADD_CU_RSP_STATE,      //��״̬����3AS���CU���û���������CUI�û�
        WAIT_CMU_ADD_CU_RSP_STATE,      //��״̬����CMU���CU
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
    //������Ҫ��д�ķ���
    virtual const char* GetObjName() const //�������صķ���
    {
        return "CCuAgent";
    }
    virtual const char* GetStateName(u32 dwState) const //�������صķ���
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
    //״̬����
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
    //�ṩ�Ķ��ⷽ��
    /*====================================================================
    ������      : StartLogin
    ����        : ��ʼCU��¼
    �㷨ʵ��    :
    ����ȫ�ֱ���:
    �������˵��: strUserName  �û���, ����ligang@Nanjing
                 strPasswd    ����
    ����ֵ˵��  : �ɹ�:true��ʧ��:false
    ----------------------------------------------------------------------
    �޸ļ�¼    :
    ��  ��      �汾        �޸���        �޸�����
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
    //����������
    int CalcEthNo(const TIpAddr& strIp);
    //�Ƿ���nat��ϵ
    int IsHasNat(const TIpAddr& tServiceIp, const TIpAddr& tHostIp);
	string GetHostIp(const string& strHost);

	void OnUpdateCuProperty();

private:
    void FillOnlineReq( CCuOnlineReq &cReq, const string &strSession );

protected:
public:
    //�����Լ��ĳ�Ա
    string m_strUserName;
    string m_strPassWord;
    string m_strVersion;
    string m_strSession;            //CMUΪCU�����session
    string m_strCuSessionIn3AS;     //3ASΪCU�����session
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
	s32 m_nPwdLockRemainTime;           //���뱻����ʣ��ʱ�䣬��λ����               
	s32 m_nPwdExpireRemainTime;         //������Ч�ڵ���ʣ��ʱ��
	u32 m_nPwdRemainError;              //����ʧЧ������
    TSipURI m_tCmuURI;
    PTRecvNode m_ptCuLoginNode;
    PTRecvNode m_ptCuLogoutNode;
    CCuNotifyPool m_cNotifyPool;       //�������ƽ̨֪ͨ��Ϣ
    PTRecvNode m_ptNotifyRecvNode;     //�ȴ�֪ͨ��RecvNode
    s32 m_nNotifyRecvNodeTimeout;      //�ȴ�֪ͨ�ĳ�ʱʱ��
    bool m_bNeedPlatformServiceStatue; //��Ҫƽ̨����״̬
	bool m_bClientSupCuiTransPlatRec;  // �ͻ����Ƿ�֧��CUIת��ƽ̨¼��ط�/����

    CUserAccount *m_pcUserAccount;     //CuAgent�����Լ����˻���Ϣ
    CPlayIdToInviteTaskMap      m_cInviteTaskSet;    //ý�������񼯺�
    //����ģ�����Task����ԭ�е�task�洢��ʽ(��������)
    CPePlayIdToInviteTaskMap    m_cPeInviteTaskSet;  //ý�������񼯺�
    CPlayIdToCallPuTaskMap      m_cCallPuTaskSet;

	set<u32>  m_setDpsSearchTaskNo;

    set<CModuleListSubscribeTaskBase*> m_setModuleListSubTask;
    CDevUriTypeToSubTaskMap m_cDevSubTaskSet;     // ��������
    CFileUrlToExportTaskMap m_cFileExportTaskSet; //����task
    //todo ��ʱ������Ӧ��ȥ���ˣ���Щ��ʱ����ǳ��ã���������
    CTaskNOToSubTaskPtrMap      m_cSubTaskSet;          //������ʱ����

    TNatInfo m_tNatInfo;
	u32 m_dwClientVer;  // �ͻ��˰汾 [1/22/2015 pangubing]
	TRecTvTaskSets m_tRecordTaskSets;   //��¼¼����ǽ����
 };
s32 GetMobileClientNumber();
bool IsMobileClientType(const string &strClientType);

/* TAS��¼������ʶһ��TAS */
class C3acAgent : public CSipTask
{
public:
    enum
    {
        OFFLINE_STATE = UNKNOWN_STATE + 1,
        ONLINE_STATE,
        RELOGIN_STATE   //��TAS�ϻ�������CUʱ����Ҫ���Բ�������TAS���Ա㾡��ظ�CU��TAS����
    };
#define TACAGENT_EMPTY_USERACCOUNT_CHECK_COUNT 5
public:
    C3acAgent(CInstExt *pcInst);
    ~C3acAgent();
public:
    //������Ҫ��д�ķ���
    virtual const char* GetObjName() const //�������صķ���
    {
        return "C3acAgent";
    }
    virtual const char* GetStateName(u32 dwState) const  //�������صķ���
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
    //״̬����
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
    //�����Լ��ĳ�Ա
    TSipURI m_t3asURI;
    string m_strSessionID;  //CUI��Ϊ�ͻ�����3AS�ĻỰ
    string m_str3ASDomainName;
    string m_t3ASDomainID;
    /*
        Ϊ����UserAccount�����ݱ�������
        ��һ��UserAccount�˻������һ��CU�˳�ʱ��ɾ��UserAccount��
        ��һ��UserGroup�����һ��UserAccount��ɾ��ʱ��ɾ��UserGroup��
        ��һ��3asAgent��UserAccount�б��UserGroup�б����ʱ��ɾ��3asAgent

        ��3AS��TimerPoll�ж��ڼ����������
        Ϊ�˷�ֹ��ʱ��ǡ����CU��¼��������м�鲢ɾ����3ASAgent����������һ�����ü���
        ֻ�е����ü���Ϊ 0 ʱ�ż��

        Ҫ��CuAgent�ڿ�ʼ��3AS����ʱ����BeginRefCount()������ʱ����EndRefCount()
    */
    //u32 m_dwRefCounter;         //����CU��3AS�Ĳ�������
    CUserMgr m_cUserMgr;
    vector<string> m_vctTmpQueryTag;        //��ʱ��¼��ѯ��Tag,���ڶ�������Դ����
    CDomainInfoMap m_mapDomainInfo;
	bool m_bGetMapDomainInfoFinish;
	CCuPtzLockInfoList m_cPtzlockInfoList;
};

/* �����¼������ʶһ������ */
class CPeAgentBase : public CSipTask //����ģ��Ļ���task�������Զ���������task���Լ��Զ�ά������
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
    //�����Լ��ĳ�Ա
    TSipURI m_tPeURI;       //����ģ���URI
    string m_strReqMsg;
    string m_strSessionID;  //CUI��Ϊ�ͻ���������ĻỰID

    //ֻ����־�����˲ʱ��ѯtask�����軺��
    //��ʱ�ò���
    CDevUriToSubscribeTaskMap   m_cSubscribeTaskSet;    //�����豸���񼯺ϣ������豸���ģ�ÿ��devURI��Ӧһ��SubscribeTask, �������4�ж�������
    CModuleListSubscribeTaskBase *m_pcModuleListSubscribeTask;
    CPlayIdToInviteTaskMap      m_cInviteTaskSet;       //ý�������񼯺�
    CFileUrlToExportTaskMap     m_cFileExportTaskSet;

	set<u32>  m_setDpsSearchTaskNo;

    TCallBackTaskList m_tCallBackTaskList; //��Ҫ�����task��
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
    //״̬����
    u32 OnIdle(CMessage *const pcMsg);
    u32 OnIdleTimer();

    u32 OnWaitRegRsp(CMessage *const pcMsg);
    u32 OnWaitRegRspTimer();

    u32 OnService(CMessage *const pcMsg);
    u32 OnServiceTimer();
    void DisProc(CMessage *const pcMsg);
public:
    u32 StartLogin();  //�鷽������ʵ�ָ�������ģ��ĵ�½
    u32 StartLogOut();
};

/* CU��¼�����л�ȡ�û���Ϣ���豸Ȩ����Ϣ���û�������ʱ�Զ���ȡ�û������Ϣ��CGetUserInfoΪ���task */
class CGetUserInfo : public CSipTask
{
public:
    enum
    {
        NOT_QUERY_STATE = UNKNOWN_STATE + 1,        //����״̬��δ���Ͳ�ѯ����
        WAIT_RESPONSE_STATE,                        //�û���Ϣ����Ӧ
        WAIT_NOTIFY_STATE,                          //�û���Ϣ֪ͨ��Ϣ
    };
public:
    CGetUserInfo(CInstExt *pcInst, const string& str3asName, const string& strUserName, const string& strSession);
    ~CGetUserInfo();
public:
    //������Ҫ��д�ķ���
    virtual const char* GetObjName() const //�������صķ���
    {
        return "CGetUserInfo";
    }
    virtual const char* GetStateName(u32 dwState) const //�������صķ���
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
    string m_strQueryTag;       //���䲻Ϊ��ʱ��������m_mapSessToTaskMap�еǼǹ�
    std::list<string> m_VecNtfReqList; //������rsp��Ϣ�н��ܵ���ntf��Ϣ

private:
    u32 DoStartQryUserInfo();
};

class CGetGroupInfo : public CSipTask
{
public:
    enum
    {
        NOT_QUERY_STATE = UNKNOWN_STATE + 1,        //����״̬��δ���Ͳ�ѯ����
        WAIT_RESPONSE_STATE,                        //�û���Ϣ����Ӧ
        WAIT_NOTIFY_STATE,                          //�û���Ϣ֪ͨ��Ϣ
    };
public:
    CGetGroupInfo(CInstExt *pcInst, const string& str3asName, const string& strGroupID);
    ~CGetGroupInfo();
public:
    //������Ҫ��д�ķ���
    virtual const char* GetObjName() const //�������صķ���
    {
        return "CGetGroupInfo";
    }
    virtual const char* GetStateName(u32 dwState) const //�������صķ���
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
    string m_strQueryTag;       //���䲻Ϊ��ʱ��������m_mapSessToTaskMap�еǼǹ�
    std::list<string> m_VecNtfReqList; //������rsp��Ϣ�н��ܵ���ntf��Ϣ

private:
    u32 DoStartQryGroupInfo();
};

/* TAS��������TASK�������߼����£�
�ж��Ƿ���CuAgent���ߣ�
1���������CuAgent�е�Session
2������һ����task CReLoginTasTask������3AS DRI��������CuAgent��taskid��������ָ�룩
3������ע��3AS�����ע��ɹ��򷵻���һ״̬��
4������ע��ÿ��CuAgent��ע��ɹ��͸���CuAgent��Session
5���������CuAgent��ע��ɹ�����ɾ������

���е���TAS�����󣬶�Ҫ�ж�Session�Ƿ�Ϊ��
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
    //������Ҫ��д�ķ���
    virtual const char* GetObjName() const //�������صķ���
    {
        return "CReLoginTasTask";
    }
    virtual const char* GetStateName(u32 dwState) const  //�������صķ���
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


