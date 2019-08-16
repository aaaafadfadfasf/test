#ifndef _USER_REG_TASK_H
#define _USER_REG_TASK_H

#include "../include/loginsession.h"
#include "democonf.h"
#include "demomsgmanage.h"

class CUserRegTask : public CLoginSession
{
public:
	enum
	{
		WaitReg = UNKNOWN_STATE + 1,
		Service,
	};

public:
	CUserRegTask(CInstExt *pcInst);
	virtual ~CUserRegTask();

public:
	virtual const char* GetObjName() const
	{
		return "CUserRegTask";
	}

	virtual const char* GetStateName(u32 dwState) const
	{
		switch (dwState)
		{
		case WaitReg:
			return "WaitReg";
		case Service:
			return "Service";
		default:
			break;
		}
		return "Unknown State";
	}

public:
	virtual void InitStateMachine();

public:
	u32 OnWaitReg(CMessage *const pcMsg);
	u32 OnWaitRegTimer();

	u32 OnService(CMessage *const pcMsg);
	u32 OnServiceTimer();

	//注册信息记录在redis中
	void RecordRegInfoToRedis();



private:

};






#endif