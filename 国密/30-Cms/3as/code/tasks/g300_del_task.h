/*
���g300��Ϣ
*/
#ifndef _G300_DEL_TASK_H_
#define _G300_DEL_TASK_H_

#include "inner_common.h"

class CG300DelTask: public CSipTask
{
public:
    enum
    {
		enIdle = UNKNOWN_STATE + 1,
		enClearDevice,
		enClearModel,
		enClearGroup
    };

    CG300DelTask(CInstExt *pcInst);
    ~CG300DelTask();

    virtual const char* GetObjName() const
	{
		return "CG300DelTask";
	}

    virtual const char* GetStateName(u32 dwState) const
	{
		switch (dwState)
		{
		case enIdle:
			return "Idle";
		case enClearDevice:
			return "ClearDevice";
		case enClearModel:
			return "ClearModel";
		case enClearGroup:
			return "ClearGroup";
		default:
			break;
		}
		return "Unknown State";
	}

    virtual void InitStateMachine();

private:

	u32 OnIdle(CMessage *const pcMsg);
	u32 OnIdleTimer();
    u32 OnClearDevice(CMessage *const pcMsg);
	u32 OnClearDeviceTimer();
	u32 OnClearModel(CMessage *const pcMsg);
	u32 OnClearModelTimer();
	u32 OnClearGroup(CMessage *const pcMsg);
	u32 OnClearGroupTimer();

	void QueryDevice();
	void DeleteDevice(const string &strDeviceId, u32 dwSeqNum);
	void QueryModel();
	void DeleteModel(const string &strModelId, u32 dwSeqNum);
	void QueryGroup();
	void DeleteGroup(const string &strGroupId, u32 dwSeqNum);

	void SendRsp(u32 dwErrorCode);
	void SendRsp();
    void HandleProgressInRedis(int progress);
	bool IsNtfMsg(const CTaskMsgTransferBase *pcTrans);

	TKDSIP_TRANS_ID  m_tTransId;               // G300���������SIP����ID
	string           m_strGatewayId;           // ��Ҫ������G300��ID
	set<string>      m_cDeviceId;			   // ��Ҫ�����G300�豸ID
	set<string>      m_cModelId;		       // ��Ҫ�����G300�豸�ͺ�ID
	vector<string>      m_vecGroupId;		       // ��Ҫ�����G300�豸��ID
	map<u32, string> m_cProcessing;            // �����е��豸ID/�豸�ͺ�ID/�豸��ID

	u32              m_dwSeqNum;        // ��ǰ���ڴ�����豸/�豸�ͺ�/�豸�������

	map<string, s32> m_cFailedDeviceMap;       // ����ʧ�ܵ��豸��Ϣ
	map<string, s32> m_cFailedModelMap;        // ɾ��ʧ�ܵ��豸�ͺ���Ϣ
	map<string, s32> m_cFailedGroupMap;        // ɾ��ʧ�ܵ��豸����Ϣ
	static const u32 m_dwMaxParallelProcess = 3;

	CGatewayDelRsp   m_cRsp;
};

#endif // _G300_DEL_TASK_H_
