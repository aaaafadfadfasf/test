#ifndef CREATE_VKEK_TASK_H
#define CREATE_VKEK_TASK_H

#include "cms/ospsip/ospsip.h"
#include "cms/ospext/ospext.h"
#include "cms/security_module/security_module_event.h"
#include "cms/security_module/security_module_proto.h"
#include "cms/cms_errorcode.h"
#include "cms/utility/cmstime.h"
#include "dbixface.h"
#include "cms/utility/md5.h"
#include "cms/utility/ende_crypt.h"

class CVkekCreateTask:public CSipTask
{
public:
	enum
	{
		REQ = UNKNOWN_STATE + 1,
		OPT_RSP,
	};
public:
	CVkekCreateTask(CInstExt *pcInst);
	~CVkekCreateTask();
public:
	virtual const char* GetObjName() const
	{
		return "CVkekCreateTask";
	}
	virtual const char* GetStateName(u32 dwState) const
	{
		switch(dwState)
		{
		case REQ:
			return "Req State";
		case OPT_RSP:
			return "Opt Rsp State";
		default:
			return "Unknown State";
		}
	}
	virtual void InitStateMachine();
public:
	 u32 ProcReqMsg(CMessage *const pcMsg);
	 u32 ProcReqMsgTimerPoll();
	 u32 ProcOptRspMsg(CMessage *const pcMsg);
	 u32 ProcOptRspMsgTimerPoll();

	 u32 PostRspToClient(u32 dwErrorCode);
	 void AddVkekReq(const CSecurityModuleNewVkekReq &cReq);
private:
	u32 m_dwSeqNum;
	u32 m_dwEvent;
	string m_strSession;
	TKDSIP_TRANS_ID m_tTransId;
	string m_strReqMsg;	//存储请求消息的ToXml串
	TSecurityModuleVkek m_tVkek;
};







#endif
