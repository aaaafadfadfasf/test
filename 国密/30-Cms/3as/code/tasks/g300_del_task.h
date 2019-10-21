/*
清除g300信息
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

	TKDSIP_TRANS_ID  m_tTransId;               // G300退网请求的SIP事务ID
	string           m_strGatewayId;           // 需要退网的G300的ID
	set<string>      m_cDeviceId;			   // 需要清除的G300设备ID
	set<string>      m_cModelId;		       // 需要清除的G300设备型号ID
	vector<string>      m_vecGroupId;		       // 需要清除的G300设备组ID
	map<u32, string> m_cProcessing;            // 处理中的设备ID/设备型号ID/设备组ID

	u32              m_dwSeqNum;        // 当前正在处理的设备/设备型号/设备组的索引

	map<string, s32> m_cFailedDeviceMap;       // 退网失败的设备信息
	map<string, s32> m_cFailedModelMap;        // 删除失败的设备型号信息
	map<string, s32> m_cFailedGroupMap;        // 删除失败的设备组信息
	static const u32 m_dwMaxParallelProcess = 3;

	CGatewayDelRsp   m_cRsp;
};

#endif // _G300_DEL_TASK_H_
