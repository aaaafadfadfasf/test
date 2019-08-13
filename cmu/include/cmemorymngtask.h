/*****************************************************************************
模块名      : cmu 
文件名      : cmemorymngtask.h 
相关文件    : 
文件实现功能: 定时释放tcmalloc的空闲内存             
作者        : 朱森勇
版本        : vs_phoenix  Copyright(C) 2009-2019 KDDACOM, All rights reserved.
-----------------------------------------------------------------------------
修改记录:
日  期         版本        修改人      修改内容
2019/01/14     1.0         朱森勇      创建
******************************************************************************/

#ifndef _CMEMORY_MNG_TASK_H
#define _CMEMORY_MNG_TASK_H

#include "cms/ospsip/ospsip.h"


class CMemoryMngTask : public CSipTask
{
public:
	enum
	{
		Service = UNKNOWN_STATE + 1,
	};
public:
	CMemoryMngTask(CInstExt *pcInst);
	~CMemoryMngTask();
	//子类需要重写的方法
	virtual const char* GetObjName() const //必须重载的方法
	{
		return "CMemoryMngTask";
	}

	virtual const char* GetStateName(u32 dwState) const //必须重载的方法
	{
		switch (dwState)
		{
		case Service:
		{
			return "Service";
		}
		break;
		default:
			break;
		}
		return "Unknown State";
	}
	virtual void InitStateMachine();	
public:
	//状态函数
	u32 OnService(CMessage *const pcMsg);
	u32 OnServiceTimer();

};

#endif  //#ifndef _CMU_TASK_H