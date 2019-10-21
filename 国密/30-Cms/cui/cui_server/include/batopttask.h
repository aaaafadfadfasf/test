#ifndef _BAT_OPT_TASK_H_
#define _BAT_OPT_TASK_H_

#include "cms/ospext/ospext.h"
#include "cms/ospsip/ospsip.h"
#include "cuidata.h"
#include "cms/commonconfig/cmcfg.h"
#include "culogintask.h"
#include "cms/utility/digital_string.h"

class CBatOptTask : public CSipTask
{
public:
    enum
    {
        NOT_QUERY_STATE = UNKNOWN_STATE + 1,        // 等待向3A发起查询
        WAIT_RESPONSE_STATE,                        // 等待查询Rsp
        WAIT_NOTIFY_STATE,                          // 等待查询Ntf
		WAIT_OPT_RSP_STATE							// 等待DPS应答
    };
public:
	CBatOptTask(CInstExt *pcInst, const string &strReq);
    ~CBatOptTask();

public:
    //子类需要重写的方法
    virtual const char* GetObjName() const //必须重载的方法
    {
        return "CBatOptTask";
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
		case WAIT_OPT_RSP_STATE:
			{
				return "Wait DPS Rsp State";
			}
			break;
        default:
            break;
        }
        return "Unknown State";
    }
    void TerminateTask();
    virtual void InitStateMachine();
public:
    template <typename CReq>
    u32 FakePostMsgReq(CReq &cReq, const TSipURI &tDstURI)
    {
        TASKLOG(NORMAL_TASK, EVENT_LEV, "CBatOptTask[TaskNO %u]发送消息[%s--%d]给服务器[%s]\n", 
            GetTaskNO(), cReq.GetEventStr().c_str(), cReq.GetEventId(), tDstURI.GetURIString().c_str());
        return PostMsgReq<CReq>(cReq, tDstURI);
    }

	template <typename CReq>
	u32 PostMsgReqTo3AS(CReq &cReq)
	{
		if (m_pc3acAgent == NULL)
		{
			TASKLOG(NORMAL_TASK, EVENT_LEV, "CBatOptTask[TaskNO %u]发送消息[%s--%d]给3a服务器, 3acAgent对象不存在\n",
				GetTaskNO(), cReq.GetEventStr().c_str(), cReq.GetEventId());
			return PROCMSG_DEL;
		}

		if (m_pc3acAgent->GetState() !=  C3acAgent::ONLINE_STATE)
		{
			TASKLOG(NORMAL_TASK, EVENT_LEV, "CBatOptTask[TaskNO %u]发送消息[%s--%d]给3a服务器, 3acAgent不在线\n",
				GetTaskNO(), cReq.GetEventStr().c_str(), cReq.GetEventId());
			return PROCMSG_DEL;
		}

		cReq.SetSession(m_pc3acAgent->m_strSessionID);

		const TSipURI &tDstURI = m_pc3acAgent->m_t3asURI;
		return FakePostMsgReq(cReq, tDstURI);
	}

	template <typename CReq>
	u32 PostMsgReqToCMU(CReq &cReq)
	{
		TSipURI tDstURI(DEV_TYPE_CMU, GetDomainName());
		return FakePostMsgReq(cReq, tDstURI);
	}
public:
    //状态函数
    u32 NotQueryProcMsg(CMessage *const pcMsg);
    u32 WaitRspProcMsg(CMessage *const pcMsg);
    u32 WaitRspTimerPoll();
    u32 WaitNotifyProcMsg(CMessage *const pcMsg);
    u32 ProcNotifyMsg(CMessage *const pcMsg);
	u32 WaitNotifyTimerPoll();
	u32 WaitBatOptRspProcMsg(CMessage *const pcMsg);
	u32 WaitBatOptRspTimerPol();

private:
	string m_strBatOptReq;
	CCuBatOptReq m_cuBatOptReq;
	ECuBatOptType m_eBatOptType;
	CuBatOptList *m_pcCuBatOptList;
	
	C3acAgent *m_pc3acAgent;

	CPrivilege m_userPri;

	u16 m_dwQueryEvent; // ntf回消息用
	string m_strQueryEvent;
	string m_strQryTag;   // 当其不为空时，表面在m_mapSessToTaskMap中登记过

	string m_strSendBatOptReq;	// 记录一下req，失败后用于打印

	int m_nPos, m_nTotal; // 进度预留
	
	list<CUserDevGrpVidSrcInfo> m_allDevVsInGroup; // 缓存的所有NTF
	std::list<string> m_VecNtfReqList; //缓存在rsp消息中接收到的ntf消息

	u16 m_wCurrTimeProcedRecNum; //本次已处理录像数量

private:
	bool GetBatOptType(const string &strBatOptReq, CCuBatOptReq &cuBatOptReq, ECuBatOptType &optType, CuBatOptList* &pcCuBatOptList);

	CPeAgentBase *GetPeAgentInServiceState(const string &strPeName);
	bool Get3aAgentInOnLineStat(C3acAgent* &pc3acAgent);

	bool GetUserPri();
	u32 QueryUserInfo();
	u32 QueryUserDevVsByGrp();

	void SaveVsData(vector<CUserDevGrpVidSrcInfo> &vsData);

	bool SendBatOpt();

	u32 SendOptForPlatRec(const TChannel &tChannel);
	u32 SendOptForPuRec(const TChannel &tChannel, CPrivilege &devPri);
	u32 SendOptForPuPTZ(const TChannel &tChannel, CPrivilege &vsPri);

	CSipTask *GetCallPuTask(CCuAgent *pcCuAgent, int nPlayId);
	u32 SendPotForStopPuCall(const TChannel &tChannel);

	bool GetFrontFromVsData(string &strDevId, string &strDomainId, int &nVsId, CPrivilege &devPri, CPrivilege &vsPri);
	void PopFrontFromNtfData();

	void WriteLog(const string& m_strCuSession, TOperateType enOprType, u32 dwErrorCode, const string& strLogDesc, const string &strDevUri = string());
	void WriteLog(const string& m_strCuSession,const TChannel& tDevChn, const u32 dwEvent, const u32 dwErrorCode);
};

#endif // _BAT_OPT_TASK_H_
