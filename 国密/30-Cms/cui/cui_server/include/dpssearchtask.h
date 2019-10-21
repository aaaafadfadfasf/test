#ifndef _DPS_SEARCH_TASK_H_
#define _DPS_SEARCH_TASK_H_
#include "common.h"
//#include "cms/cmu/cmu_proto.h"
#include "culogintask.h"
#include "cms/dps/dps_proto.h"

class CDpsSearchInterAction : public CSipTask
{
public:
	enum
	{
		Req = UNKNOWN_STATE + 1,
		Rsp,
	};
public:
	CDpsSearchInterAction(CInstExt *pcInst, const string &strCuSession, PTRecvNode ptRecvNode);
	virtual ~CDpsSearchInterAction();
	virtual const char* GetObjName() const //必须重载的方法
	{
		return "CDpsSearchInterAction";
	}
	virtual const char* GetStateName(u32 dwState) const //必须重载的方法
	{
		switch (dwState)
		{
		case Req:
		{
			return "Req";
		}
		break;
		case Rsp:
		{
			return "Rsp";
		}
		break;
		default:
			break;
		}
		return "Unknown State";
	}
	virtual void TerminateTask();
	virtual void InitStateMachine();
public:
	u32 ReqProcMsg(CMessage *const pcMsg);
	u32 ReqTimerPoll();
	u32 RspProcMsg(CMessage *const pcMsg);
	u32 RspTimerPoll();

	CTask *GetDpsSearchTask( u32 dwSearchTaskNo );
public:
	void PostErrorMsg2Cu(u32 dwErrorCode);
	template<typename CRspToCu>
	u32 SendErrorMsg2Cu(u32 dwErrorCode = ERR_CUI_OPR_TIME_OUT)
	{
		CRspToCu cRsp;
		cRsp.SetErrorCode(dwErrorCode);
		if ( m_ptRecvNode )
		{
			return SendRspToCu<CRspToCu>(cRsp, m_ptRecvNode);
		}
	}
private:
	PTRecvNode m_ptRecvNode;
	string m_strCuSession;
	CCuDPSGetResultReq m_cReq;
	CCuDPSGetResultByGroupReq m_cGroupReq;
private:
	u32 m_dwReqEventID;
	u32 m_dwInviteTaskNo;
};


class CDpsSearchTask : public CCallBackTask
{
public:
	enum
	{
		NotReq = UNKNOWN_STATE + 1,      //空闲状态，未发送请求
		WaitRsp,
		WaitGetResultReq,   // 等待请求搜索结果，无超时
		WaitGetResultRsp,   // 查询搜索结果等待
		WaitGetResultNtf,   // 服务状态
		WaitBye,         // 等待挂断
	};
public:
	CDpsSearchTask(CInstExt *pcInst, const string &strCuSession, CSipTask *pcExportActTask, const string &strPeUri);
	~CDpsSearchTask();
public:
	virtual const char* GetObjName() const //必须重载的方法
	{
		return "CDpsSearchTask";
	}
	virtual const char* GetStateName(u32 dwState) const //必须重载的方法
	{
		switch (dwState)
		{
		case NotReq:
			{
				return "Not Req";
			}
			break;
		case WaitRsp:
			{
				return "Wait Rsp";
			}
			break;
		case WaitGetResultReq:
			{
				return "Wait Get Result Req";
			}
			break;
		case WaitGetResultRsp:
			{
				return "Wait Get Result Rsp";
			}
			break;
		case WaitGetResultNtf:
			{
				return "Wait Get Result Ntf";
			}
			break;
		case WaitBye:
			{
				return "Wait Bye";
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
	u32 NotReqProcMsg(CMessage *const pcMsg);
	u32 NotReqTimerPoll();
	u32 WaitRspProcMsg(CMessage *const pcMsg);
	u32 WaitRspTimerPoll();
	u32 WaitGetResultReqProcMsg(CMessage *const pcMsg);
	u32 WaitGetResultReqProcTimerPoll();
	u32 WaitGetResultRspProcMsg(CMessage *const pcMsg);
	u32 WaitGetResultRspProcTimerPoll();
	u32 WaitGetResultNtfProcMsg(CMessage *const pcMsg);
	u32 WaitGetResultNtfProcTimerPoll();
	u32 WaitByeProcMsg(CMessage *const pcMsg);
	u32 WaitByeTimerPoll();

	const CDPSSearchProgressNtfReq &GetProgressNtf();
public:
	u32 StartSendReq(); 
	u32 StartDpsSearchReq(CDPSSearchReq &cReq);
	u32 SendGetResultReq(CDPSGetResultReq &cReq);
	u32 SendGetResultByGroupReq(CDPSGetResultByGroupReq &cReq);
	u32 SendSearchBye();

private:
	u32 SendDpsSearchAck();
	void SendGetResultNtfRsp(CMessage *const pcMsg);
	void ClearQueryMsg();
	
	// 处理进度通知
	void SendProcNtfRsp(CMessage *const pcMsg);
	//void NotifyStop2Cu(s32 dwErrorCode);

public:
	CSipTask *m_pcImportActTask;
	TKDSIP_DIALOG_ID m_tInviteID;
	string m_strExportFileUrl;
	CFileCollectProgressNtfRsp m_tNtfRsp;
public:
	string m_strPeSession;

	CDPSSearchReq m_tReq;
	CDPSSearchRsp m_tRsp;

	CDPSGetResultReq m_tGetResultReq;
	CDPSGetResultRsp m_tGetResultRsp;
	
	CDPSGetResultByGroupReq m_tGetResultByGroupReq;
	CDPSGetResultByGroupRsp m_tGetResultByGroupRsp;

	u32 m_GetResultReqEvent;

	CDPSSearchResultList m_tResultList;

	CDPSSearchProgressNtfReq m_tProgressNtfReq;
};

#endif // _DPS_SEARCH_TASK_H_