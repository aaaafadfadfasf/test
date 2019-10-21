#ifndef VKEK_QUERY_TASK_H
#define VKEK_QUERY_TASK_H


#include "cms/ospsip/ospsip.h"
#include "cms/ospext/ospext.h"
#include "cms/ospext/osptask.h"
#include "cms/dbix/dbresult_callback_imp4osptask.h"
#include "dbixface.h"
#include "cms/security_module/security_module_proto.h"
#include "cms/cms_errorcode.h"
#include "conf.h"
#include "cms/utility/ende_crypt.h"

class CVkekQueryTask:public CSipTask
{
public:
	enum
	{
		REQ = UNKNOWN_STATE + 1,
		RSP,
	};
public:
	CVkekQueryTask(CInstExt *pcInst);
	~CVkekQueryTask();
public:
	virtual const char* GetObjName() const
	{
		return "CVkekQueryTask";
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
	void QryFromDB();
	void QryFromDB(string strTmFrom);
	u32 WaitDBIXResultProcMsg(CMessage *const pcMsg);
	u32 ProcQryRsp(CMessage *const pcMsg, u32 dwErrorCode);
	u32 PostErrRsp();
	u32 WaitHandleQueryTimerPoll();
	u32 WaitDBIXResultTimerPoll();
	u32 ProcErrMsg(CMessage *const pcMsg);
protected:
private:
	string m_strSession;
	u32 m_dwSeqNum;
	u32 m_dwEvent;
	KDSIP_TRANS_ID m_tTransID;
	CSecurityModuleVkekQueryReq m_cReq;
	bool m_bQryFlag;
};


#endif