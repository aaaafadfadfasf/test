#ifndef _GETOFFLINEDEVTASK_H_
#define _GETOFFLINEDEVTASK_H_

/********************************************************************
	filename: getofflinedevtask.h
	purpose : ͳ�Ʋ�������ƵԴ��Ϣ
	author  : pangubing@kedacom.com
    copyright:<Copyright(C) 2003-2013 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-----------------------------------------------------------------------
�޸ļ�¼��
�� ��          �汾     �޸���   �޸ļ�¼
2015/02/10     V1.0     �˹ű�     ����
*********************************************************************/
#include "cms/ospsip/ospsip.h"
#include "cmuconst.h"
#include "cmudata.h"
#include "cmuconnecttask.h"
#include "cmucoreapp.h"
class CGetOfflineDevTask : public CSipTask
{
public:
	enum
	{
		Req = UNKNOWN_STATE+1,
		WaitRsp,
		Ntf,
	};
public:
	CGetOfflineDevTask(CInstExt* pInst);

	~CGetOfflineDevTask();

	void ReleaseResource();

public:
	virtual const char* GetObjName() const
	{
		return "CGetOfflineDevTask";
	}

	virtual const char* GetStateName(u32 dwState) const
	{
		switch (dwState)
		{
		case Req:
			return "Req";
		case WaitRsp:
			return "WaitRsp";
		case Ntf:
			return "Ntf";
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
	u32 OnReq(CMessage *const pcMsg);
	u32 OnReqTimer();

	u32 OnWaitRsp(CMessage *const pcMsg);
	u32 OnWaitRspTimer();

	u32 OnNtf(CMessage *const pcMsg);
	u32 OnNtfTimer();

private:
	u32 SendReq(const COffLineDevQryReq& cReq, const TSipURI& tDstUri);
	u32 SendRsp(u32 dwError);
	u32 SendRsp(const COffLineDevQryRsp& cRsp);
	u32 SendNtfReq();
public: 
	const TKDSIP_TRANS_ID GetSipTransID() const
	{
		return m_tSipTransId;
	}
	void SetSipTransID(const TKDSIP_TRANS_ID tSipTransID)
	{
		m_tSipTransId = tSipTransID;
	}

private:
	TKDSIP_TRANS_ID m_tSipTransId;   //���ڲ�ѯӦ��·��
	TSipURI m_tFromUri;              //���ڲ�ѯ֪ͨ·��
	COffLineDevQryReq m_tReq;
	TSipURI m_tCmuUri;
	TDeviceVideoSrcIdList m_tDevOffLinelist;
	u32 m_dwSendIndex;
	u32 m_dwOnlineVidSrcNum;

    static const u32 s_dwMaxCompatibleNum = 1000;
};

#endif //_GETOFFLINEDEVTASK_H_