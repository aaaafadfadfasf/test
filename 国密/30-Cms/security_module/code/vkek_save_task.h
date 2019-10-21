#ifndef SAVE_VKEK_TASK_H
#define SAVE_VKEK_TASK_H


#include "cms/ospsip/ospsip.h"
#include "cms/ospext/ospext.h"
#include "cms/ospext/osptask.h"
#include "cms/dbix/dbresult_callback_imp4osptask.h"
#include "cms/security_module/security_module_proto.h"
#include "cms/cms_errorcode.h"
#include "cms/utility/ende_crypt.h"

class CVkekSaveTask:public CSipTask
{
public:
	enum
	{
		REQ = UNKNOWN_STATE + 1,	//处理请求状态
		OPT_RSP,	                //等待操作结果
	};
public:
	CVkekSaveTask(CInstExt *pcInst);
	~CVkekSaveTask();
public:
	virtual const char* GetObjName() const
	{
		return "CVkekSaveTask";
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
		return "Unknown State";
	}
	virtual void InitStateMachine();
public:
	u32 ProcReqMsg(CMessage *const pcMsg);
	u32 ProcReqMsgTimerPoll();
	u32 ProcOptRspMsg(CMessage *const pcMsg);
	u32 ProcOptRspMsgTimerPoll();
	u32 ProcErrMsg(CMessage *const pcMsg);
	void AddVkekReq(const CSecurityModuleVkekSaveReq &cReq);
	u32 PostRspToClient(u32 dwErrorCode);
protected:
private:
	u32 m_dwSeqNum;
	u32 m_dwEvent;
	string m_strSession;
	TKDSIP_TRANS_ID m_tTransId;
	string m_strReqMsg;	//存储请求消息的ToXml串
};



















#endif