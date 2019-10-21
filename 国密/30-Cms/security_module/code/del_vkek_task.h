#ifndef DEL_VKEK_TASK_H
#define DEL_VKEK_TASK_H

#include "cms/ospsip/ospsip.h"
#include "cms/ospext/ospext.h"
#include "cms/utility/xthread.h"
#include "conf.h"
#include "dbixface.h"


class CDelVkekTask : public CSipTask
{
public:
	enum
	{
		SERVER = UNKNOWN_STATE + 1,
	};

public:
	CDelVkekTask(CInstExt *pcInst);
	~CDelVkekTask();

public:
	virtual const char* GetObjName() const
	{
		return "CDelVkekTask";
	}
	virtual const char* GetStateName(u32 dwState) const
	{
		switch(dwState)
		{
		case SERVER:
			return "SERVER";
		default:
			return "Unknow State";
		}
		return "Uknow State";
	}
	virtual void InitStateMachine();

public:
	//×´Ì¬´¦Àíº¯Êý
	u32 ProcServerMsg(CMessage *const pcMsg);
	u32 ServerTimerPoll();

};

extern CDelVkekTask *g_pcDelVkekTask;

#endif