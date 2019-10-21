/*
tas设备组添加设备
*/
#ifndef _DEVGRP_DEV_ADD_TASK_H_
#define _DEVGRP_DEV_ADD_TASK_H_
#include <queue>
#include "inner_common.h"

class CDevgrpDevAddTask: public CSipTask
{
public:
	enum
	{
		enIdle = UNKNOWN_STATE + 1,
		enAdd,
		enQryTas,
		enQryGbs,
		enSet,
	};

	CDevgrpDevAddTask(CInstExt *pcInst);
	~CDevgrpDevAddTask();

	virtual const char* GetObjName() const
	{
		return "CDevgrpDevAddTask";
	}

	virtual const char* GetStateName(u32 dwState) const
	{
		switch (dwState)
		{
		case enIdle:
			return "Idle";
		case enAdd:
			return "Add";
		case enQryTas:
			return "enQryTas";
		case enQryGbs:
			return "enQryGbs";
		case enSet:
			return "Set";
		default:
			break;
		}
		return "Unknown State";
	}
	virtual void PrintData() const;
	virtual void InitStateMachine();

private:

	u32 OnIdle(CMessage *const pcMsg);
	u32 OnIdleTimer();
	u32 OnAdd(CMessage *const pcMsg);
	u32 OnAddTimer();
	u32 OnQryTas(CMessage *const pcMsg);
	u32 OnQryTasTimer();
	u32 OnQryGbs(CMessage *const pcMsg);
	u32 OnQryGbsTimer();
	u32 OnSet(CMessage *const pcMsg);
	u32 OnSetTimer();

	string GetDevGbid(const CDeviceInfo& devInfo);
	bool IsNeedAddGbInfo(const CDeviceGroupDeviceInfo& cDevGrpDevInfo);
	bool QryGbs();
	void SendRsp(u32 dwErrorCode = CMS_SUCCESS);

	TKDSIP_TRANS_ID  m_tTransId;               // 请求的SIP事务ID
	CDeviceGroupDeviceAddReq   m_cReq;
	CDeviceGroupDeviceAddRsp   m_cRsp;
	vector<CDevTreeInfo>  m_vecDevTrees;
};

#endif // _DEVGRP_DEV_ADD_TASK_H_
