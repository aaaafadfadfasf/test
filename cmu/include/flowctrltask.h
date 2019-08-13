
#ifndef _SERIAL_MNG_TASK_H
#define _SERIAL_MNG_TASK_H

#include <queue>
#include <map>
using std::queue;
using std::map;

#include "cms/ospext/osptask.h"
#include "cmuconst.h"

/*
 * CFlowCtrlTask设计说明：
 * 要求：
 *     CFlowCtrlTask不能涉及具体的业务，只是进行流量控制，避免同时处理大量事务造成阻塞，同时要有超时控制，
 *     以避免一个作业超时导致堵塞（故从CTask继承，便于TimerPoll来处理超时）
 *
 * 细节：
 *     1、对外接口尽可能简单（实际只有RegistTask和EndTask）
 *     2、CSerialMngTask尽可能的轻量化（实际针对每个Task只保存了TaskNO）
 *     3、为什么在CSerialMngTask中保存TaskNO而不是CTask指针？因为队列机制下，可能某个Task在排队期间被析构了，如果保存指针，将很容易导致野指针。
 *        保存TaskNO的话，可以避免这个问题，虽然会带来一些性能开销，鉴于FindTask是基于hashmap的查找，暂时忽略这个影响
 *
 */

class CFlowCtrlTask
{
public:
	CFlowCtrlTask()  {}
	~CFlowCtrlTask() {}

	// 纯虚函数，支持流控的Task需要实现该函数（返回true表示任务等待进一步处理，返回false说明任务已经处理结束）
	virtual bool StartWork() = 0;
};


class CFlowCtrlMngTask : public CTask
{
public:
	enum { Working = UNKNOWN_STATE + 1 };

public:
	CFlowCtrlMngTask(CInstExt *pcInst);
	~CFlowCtrlMngTask() {}

	virtual const char* GetObjName() const { return "CFlowCtrlMngTask";   }
	virtual void InitStateMachine();
	virtual const char *GetStateName(u32 dwState) const;
	virtual void PrintData() const;

	u32 OnWorking(CMessage *const pcMsg)   { return PROCMSG_OK;         }
	u32 OnWorkingTimer();

	void Begin();

public:
	// 对外的接口
	void RegistTask(u32 dwTaskNO);			// 登记任务
	void EndTask(u32 dwTaskNO);				// 任务处理结束

private:
	queue<u32> m_cTaskQueue;				// 任务队列
	map<u32, u16> m_cProcessing;			// 处理中任务信息(key为任务TaskNO，value为该任务持续的TimerPoll次数，即秒数)
	u32 m_dwTop;							// 峰值任务数量
    u32 m_dwDrop;                           // 丢弃的任务数量

	static const u32 m_dwMaxParallelNum = 10;
};


#endif  //#ifndef _SERIAL_MNG_TASK_H
