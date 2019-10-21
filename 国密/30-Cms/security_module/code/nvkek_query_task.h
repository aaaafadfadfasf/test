#ifndef NVKEK_QUERY_TASK_H
#define NVKEK_QUERY_TASK_H


#include "cms/ospsip/ospsip.h"
#include "cms/ospext/ospext.h"
#include "cms/ospext/osptask.h"
#include "cms/dbix/dbresult_callback_imp4osptask.h"
#include "dbixface.h"
#include "cms/security_module/security_module_proto.h"
#include "cms/cms_errorcode.h"
#include "conf.h"

class CNVkekQueryTask:public CSipTask
{
public:
	enum
	{
		REQ = UNKNOWN_STATE + 1,
		RSP,
	};
public:
	CNVkekQueryTask(CInstExt *pcInst);
	~CNVkekQueryTask();
public:
	virtual const char* GetObjName() const
	{
		return "CNVkekQueryTask";
	}
	virtual const char* GetStateName(u32 dwState) const
	{
		switch (dwState)
		{
		case REQ:
			{
				return "REQ State";
			}
			break;
		case RSP:
			{
				return "RSP State";
			}
			break;
		default:
			break;
		}
		return "Unknown State";
	}
	virtual void InitStateMachine();
public:
	u32 HandleQueryProcMsg(CMessage *const pcMsg);
	void QryFromDB(CSecurityModuleNVkekQueryReq &cReq);
	u32 WaitDBIXResultProcMsg(CMessage *const pcMsg);
	u32 ProcQryRsp(CMessage *const pcMsg, u32 dwErrorCode);
	u32 PostErrRsp();
	u32 WaitHandleQueryTimerPoll();
	u32 WaitDBIXResultTimerPoll();
	u32 ProcErrMsg(CMessage *const pcMsg);
private:
	string m_strSession;
	u32 m_dwSeqNum;
	u32 m_dwEvent;
	KDSIP_TRANS_ID m_tTransID;
};


#endif