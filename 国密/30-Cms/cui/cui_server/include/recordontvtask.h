#include "cuiapp.h"

class CRecordOnTvTask : public CSipTask
{
public:
	enum
	{
		Wait = UNKNOWN_STATE+1,
		Service,
	};

public:
	CRecordOnTvTask(CInstExt* pInst,CCuAgent *pcCuAgent,PTRecvNode ptRecvNode);
	~CRecordOnTvTask();

public:
	virtual const char* GetObjName() const
	{
		return "CRecordOnTvTask";
	}

	virtual const char* GetStateName(u32 dwState) const
	{
		switch (dwState)
		{
		case Wait:
			return "Wait";
		case Service:
			return "Service";
		default:
			break;
		}
		return "Unknown State";
	}


	virtual void InitStateMachine();

	virtual void PrintData() const
	{
		CTask::PrintData();

		//ֱ�Ӵ�ӡxml����
		OspPrintf(TRUE, FALSE, "  %s\n", m_tReq.ToXml().c_str());
	}

public:
	//״̬����
	u32 OnWait(CMessage *const pcMsg);
	u32 OnWaitTimer();

	u32 OnService(CMessage *const pcMsg);
	u32 OnServiceTimer();

public:
	string GetTvsSession();   //��ȡ��¼tvsSession
	void SetRecvNode(PTRecvNode ptRecvNode)
	{
		m_ptRecvNode = ptRecvNode;
	}
private:
	CRecordOnTvReq m_tReq;
	CCuAgent *m_pcCuAgent;
	TSipURI m_tTvsUri;
	TTvChannel m_tTvChn;
    PTRecvNode m_ptRecvNode;
};