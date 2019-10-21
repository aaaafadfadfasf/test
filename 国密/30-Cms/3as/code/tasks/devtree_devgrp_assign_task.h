/*
tas��ק�����Զ����豸����ק���顣
*/
#ifndef _DEVTREE_DEVGRP_ASSIGN_TASK_H_
#define _DEVTREE_DEVGRP_ASSIGN_TASK_H_
#include <queue>
#include "inner_common.h"
#include "gbs/gbs_struct.h"

class CDevtreeDevgrpAssignTask: public CSipTask
{
public:
	enum
	{
		enIdle = UNKNOWN_STATE + 1,
		enAssign,
		enQryTas,
		enQryNewGrpId,
		enSyncDevsGbid,
		enSetGbid,
	};

	CDevtreeDevgrpAssignTask(CInstExt *pcInst);
	~CDevtreeDevgrpAssignTask();

	virtual const char* GetObjName() const
	{
		return "CDevtreeDevgrpAssignTask";
	}

	virtual const char* GetStateName(u32 dwState) const
	{
		switch (dwState)
		{
		case enIdle:
			return "Idle";
		case enAssign:
			return "enAssign";
		case enQryTas:
			return "QryTas";
		case enQryNewGrpId:
			return "QryNewGrpId";
		case enSyncDevsGbid:
			return "enSyncDevsGbid";
		default:
			break;
		}
		return "Unknown State";
	}
	virtual void PrintData() const;
	virtual void InitStateMachine();

private:

	u32 OnIdle(CMessage *const pcMsg);
	u32 OnIdleTimer();
	u32 OnQryTas(CMessage *const pcMsg);
	u32 OnQryTasTimer();
	u32 OnQryNewGrpId(CMessage *const pcMsg);
	u32 OnQryNewGrpIdTimer();
	u32 OnQryGbs(CMessage *const pcMsg);
	u32 OnQryGbsTimer();
	u32 OnAssign(CMessage *const pcMsg);
	u32 OnAssignTimer();

	void SendQry();
	void SendRsp(u32 dwErrorCode = CMS_SUCCESS);
	bool QryGbidFromGbs();
	void SetDevsGbid(const vector<TGbDevGbidBatchGetInfo>& vecGbInfos);

	TKDSIP_TRANS_ID  m_tTransId;               // �����SIP����ID
	string           m_strDevTreeName;         // �豸��ID
	string           m_strRootDevGrpGbid;      // ������gbid
	string           m_strNewDevGrpId;
	string           m_strTransOprId;
	CDevTreeDevGrpAssignReq   m_cReq;
	CDevTreeDevGrpAssignRsp   m_cRsp;
	vector<CDevTreeInfo>  m_vecDevTrees;              
	
	u32              m_dwVidSrcGbInfoIndex; //m_vecVidSrcGbInfo��gbid��ǰ����index
	string           m_strCompletingGrpId;  //��ǰ���������豸gbid�ķ���Id
	map<string, vector<TVidSrcGbInfo> > m_mapGrpId2DevGbInfos; //grpId-->��ƵԴ��Ϣ
	CDeviceGroupInfo m_cSrcDevGrpInfo;
};

#endif // _DEVTREE_DEVGRP_ASSIGN_TASK_H_
