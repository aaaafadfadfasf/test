/*****************************************************************************
ģ����      : cmu 
�ļ���      : pudecinvitetask.h 
����ļ�    : 
�ļ�ʵ�ֹ���: ������invite����Ŀǰ������¼����ǽ
����        : ��ٻٻ
�汾        : vs_phoenix  Copyright(C) 2009-2015 KDDACOM, All rights reserved.
-----------------------------------------------------------------------------
�޸ļ�¼:
��  ��         �汾        �޸���      �޸�����
2015/04/22     1.0        ��ٻٻ        ����
******************************************************************************/

#ifndef _PUDEC_INVITE_TASK_H_
#define _PUDEC_INVITE_TASK_H_

#include "cms/ospsip/ospsip.h"
#include "cms/container/xmap.h"
#include "cms/cms_errorcode.h"

#include "cms/cmu/cmu_struct.h"
#include "cms/cmu/cmu_proto.h"

class CPuiRegTask;
///////////////////////////////////////////////////////////////////////////////////////
//CPuDecInviteTask
///////////////////////////////////////////////////////////////////////////////////////
class CPuDecInviteTask : public CSipTask
{
public:
	enum
	{
		Idle = UNKNOWN_STATE+1,
		WaitRsp,
		WaitAck,
		Service
	};

public:
	CPuDecInviteTask(CInstExt *pcInst);

	virtual ~CPuDecInviteTask();

	/////////////////////////////////////////////////////////////
	//���غ���
public:
	virtual const char* GetObjName() const
	{
		return "CPuDecInviteTask";
	}

	virtual const char* GetStateName(u32 dwState) const
	{
		switch (dwState)
		{
		case Idle:
			return "Idle";
		case WaitRsp:
			return "WaitRsp";
		case WaitAck:
			return "WaitAck";
		case Service:
			return "Service";

		default:
			break;
		}
		return "Unknown State";
	}

	virtual bool IsNotNeedPollState()
	{
		return (GetState() == Service);
	}


	virtual void InitStateMachine();
	void ReleaseResource();

	/////////////////////////////////////////////////////////////
	//״̬������
public:
	u32 OnIdle(CMessage *const pcMsg);
	u32 OnIdleTimer();
	u32 OnWaitRsp(CMessage *const pcMsg);
	u32 OnWaitRspTimer();
	u32 OnWaitAck(CMessage *const pcMsg);
	u32 OnWaitAckTimer();
	u32 OnService(CMessage *const pcMsg);
	u32 OnServiceTimer();

	/////////////////////////////////////////////////////////////
	//ҵ���߼�
public:
	bool SendRsp(u32 dwError = PROCMSG_OK);
	CPuiRegTask* GetPuiRegTask(TSipURI tPuiUri);
	bool IsFromSender(const string& strUri);
private:
	TKDSIP_TRANS_ID m_tSipTransId;
	KDSIP_DIALOG_ID m_tSenderDlgId;
	KDSIP_DIALOG_ID m_tReceiverDlgId;
	CInviteDecoderCallReq m_tReq;
};

#endif  //#ifndef _PUDEC_INVITE_TASK_H_