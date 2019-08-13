
#ifndef _SERIAL_MNG_TASK_H
#define _SERIAL_MNG_TASK_H

#include <queue>
#include <map>
using std::queue;
using std::map;

#include "cms/ospext/osptask.h"
#include "cmuconst.h"

/*
 * CFlowCtrlTask���˵����
 * Ҫ��
 *     CFlowCtrlTask�����漰�����ҵ��ֻ�ǽ����������ƣ�����ͬʱ��������������������ͬʱҪ�г�ʱ���ƣ�
 *     �Ա���һ����ҵ��ʱ���¶������ʴ�CTask�̳У�����TimerPoll������ʱ��
 *
 * ϸ�ڣ�
 *     1������ӿھ����ܼ򵥣�ʵ��ֻ��RegistTask��EndTask��
 *     2��CSerialMngTask�����ܵ���������ʵ�����ÿ��Taskֻ������TaskNO��
 *     3��Ϊʲô��CSerialMngTask�б���TaskNO������CTaskָ�룿��Ϊ���л����£�����ĳ��Task���Ŷ��ڼ䱻�����ˣ��������ָ�룬�������׵���Ұָ�롣
 *        ����TaskNO�Ļ������Ա���������⣬��Ȼ�����һЩ���ܿ���������FindTask�ǻ���hashmap�Ĳ��ң���ʱ�������Ӱ��
 *
 */

class CFlowCtrlTask
{
public:
	CFlowCtrlTask()  {}
	~CFlowCtrlTask() {}

	// ���麯����֧�����ص�Task��Ҫʵ�ָú���������true��ʾ����ȴ���һ����������false˵�������Ѿ����������
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
	// ����Ľӿ�
	void RegistTask(u32 dwTaskNO);			// �Ǽ�����
	void EndTask(u32 dwTaskNO);				// ���������

private:
	queue<u32> m_cTaskQueue;				// �������
	map<u32, u16> m_cProcessing;			// ������������Ϣ(keyΪ����TaskNO��valueΪ�����������TimerPoll������������)
	u32 m_dwTop;							// ��ֵ��������
    u32 m_dwDrop;                           // ��������������

	static const u32 m_dwMaxParallelNum = 10;
};


#endif  //#ifndef _SERIAL_MNG_TASK_H
