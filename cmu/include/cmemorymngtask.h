/*****************************************************************************
ģ����      : cmu 
�ļ���      : cmemorymngtask.h 
����ļ�    : 
�ļ�ʵ�ֹ���: ��ʱ�ͷ�tcmalloc�Ŀ����ڴ�             
����        : ��ɭ��
�汾        : vs_phoenix  Copyright(C) 2009-2019 KDDACOM, All rights reserved.
-----------------------------------------------------------------------------
�޸ļ�¼:
��  ��         �汾        �޸���      �޸�����
2019/01/14     1.0         ��ɭ��      ����
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
	//������Ҫ��д�ķ���
	virtual const char* GetObjName() const //�������صķ���
	{
		return "CMemoryMngTask";
	}

	virtual const char* GetStateName(u32 dwState) const //�������صķ���
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
	//״̬����
	u32 OnService(CMessage *const pcMsg);
	u32 OnServiceTimer();

};

#endif  //#ifndef _CMU_TASK_H