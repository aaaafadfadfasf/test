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
        NOT_QUERY_STATE = UNKNOWN_STATE + 1,        // �ȴ���3A�����ѯ
        WAIT_RESPONSE_STATE,                        // �ȴ���ѯRsp
        WAIT_NOTIFY_STATE,                          // �ȴ���ѯNtf
		WAIT_OPT_RSP_STATE							// �ȴ�DPSӦ��
    };
public:
	CBatOptTask(CInstExt *pcInst, const string &strReq);
    ~CBatOptTask();

public:
    //������Ҫ��д�ķ���
    virtual const char* GetObjName() const //�������صķ���
    {
        return "CBatOptTask";
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
        TASKLOG(NORMAL_TASK, EVENT_LEV, "CBatOptTask[TaskNO %u]������Ϣ[%s--%d]��������[%s]\n", 
            GetTaskNO(), cReq.GetEventStr().c_str(), cReq.GetEventId(), tDstURI.GetURIString().c_str());
        return PostMsgReq<CReq>(cReq, tDstURI);
    }

	template <typename CReq>
	u32 PostMsgReqTo3AS(CReq &cReq)
	{
		if (m_pc3acAgent == NULL)
		{
			TASKLOG(NORMAL_TASK, EVENT_LEV, "CBatOptTask[TaskNO %u]������Ϣ[%s--%d]��3a������, 3acAgent���󲻴���\n",
				GetTaskNO(), cReq.GetEventStr().c_str(), cReq.GetEventId());
			return PROCMSG_DEL;
		}

		if (m_pc3acAgent->GetState() !=  C3acAgent::ONLINE_STATE)
		{
			TASKLOG(NORMAL_TASK, EVENT_LEV, "CBatOptTask[TaskNO %u]������Ϣ[%s--%d]��3a������, 3acAgent������\n",
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
    //״̬����
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

	u16 m_dwQueryEvent; // ntf����Ϣ��
	string m_strQueryEvent;
	string m_strQryTag;   // ���䲻Ϊ��ʱ��������m_mapSessToTaskMap�еǼǹ�

	string m_strSendBatOptReq;	// ��¼һ��req��ʧ�ܺ����ڴ�ӡ

	int m_nPos, m_nTotal; // ����Ԥ��
	
	list<CUserDevGrpVidSrcInfo> m_allDevVsInGroup; // ���������NTF
	std::list<string> m_VecNtfReqList; //������rsp��Ϣ�н��յ���ntf��Ϣ

	u16 m_wCurrTimeProcedRecNum; //�����Ѵ���¼������

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
