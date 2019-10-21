#ifndef _IMPORT_TASK_H_
#define _IMPORT_TASK_H_
#include "common.h"
#include "cms/cmu/cmu_proto.h"
#include "culogintask.h"

class CImportTaskExInterAction : public CSipTask
{
public:
	enum
	{
		Req = UNKNOWN_STATE + 1,
		Rsp,
	};
public:
	CImportTaskExInterAction(CInstExt *pcInst, const string &strCuSession, PTRecvNode ptRecvNode);
	virtual ~CImportTaskExInterAction();
	virtual const char* GetObjName() const //必须重载的方法
	{
		return "CImportTaskExInterAction";
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
	u32 ReqProcMsg( CMessage *const pcMsg );
	u32 ReqTimerPoll();
	u32 RspProcMsg( CMessage *const pcMsg );
	u32 RspTimerPoll();
public:
	CTask *GetImportTask(const string& strFileUrl);
public:
	void PostErrorMsg2Cu(u32 dwErrorCode);
	template<typename CRspToCu>
	u32 SendErrorMsg2Cu(u32 dwErrorCode = ERR_CUI_OPR_TIME_OUT)
	{
		CRspToCu cRsp;
		cRsp.SetErrorCode(dwErrorCode);
		return SendRspToCu<CRspToCu>(cRsp, m_ptRecvNode);
	}
private:
	PTRecvNode m_ptRecvNode;
	string m_strCuSession;
	string m_stFileUrl;
private:
	u32 m_dwReqEventID;
	u32 m_dwInviteReqTaskNo;
};

/* 文件导入类业务 */
class CImportTaskEx : public CCallBackTask
{
public:
	enum
	{
		NotReq = UNKNOWN_STATE + 1,      //空闲状态，未发送请求
		WaitRsp,
		WaitAck,   //此状态没有应答
		WaitProcess,	// 等待CU发送进度
		WaitBye,
	};
public:
	CImportTaskEx(CInstExt *pcInst, const string &strCuSession, CSipTask *pcExportActTask, const string &strPeUri);
	~CImportTaskEx();
public:
	virtual const char* GetObjName() const //必须重载的方法
	{
		return "CImportTaskEx";
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
		case WaitAck:
			{
				return "Wait Ack";
			}
			break;
		case WaitProcess:
			{
				return "Wait Process";
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
	u32 WaitAckProcMsg(CMessage *const pcMsg);
	u32 WaitAckTimerPoll();
	u32 WaitProcessProcMsg(CMessage *const pcMsg);
	u32 WaitProcessTimerPoll();
	u32 WaitByeProcMsg(CMessage *const pcMsg);
	u32 WaitByeTimerPoll();
public:
	u32 StartSendReq();
	u32 StartSendImportReq(CFileImportReq &cReq);
	u32 StartSendImportAck();
	u32 SendImportProcessReq(CCuFileImportSetProcessExReq &cReq);
	u32 SendImportBye();
	u32 SendImportEnd();
public:
	void NotifyStop2Cu(s32 dwErrorCode);
public:
	//CFileCollectProgressNtfReq &GetProcessNtfReq();
public:
	CSipTask *m_pcImportActTask;
	TKDSIP_DIALOG_ID m_tInviteID;
	string m_strExportFileUrl;
	CFileCollectProgressNtfRsp m_tNtfRsp;
public:
	string m_strPeSession;
public:
	CFileImportReq m_tReq;
	CFileImportRsp m_tRsp;

	CCuFileImportSetProcessExReq m_tSetProcessReq;
};

#endif // _IMPORT_TASK_H_