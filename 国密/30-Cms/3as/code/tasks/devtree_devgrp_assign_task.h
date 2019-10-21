/*
tas拖拽分组自定义设备树拖拽分组。
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

	TKDSIP_TRANS_ID  m_tTransId;               // 请求的SIP事务ID
	string           m_strDevTreeName;         // 设备组ID
	string           m_strRootDevGrpGbid;      // 根分组gbid
	string           m_strNewDevGrpId;
	string           m_strTransOprId;
	CDevTreeDevGrpAssignReq   m_cReq;
	CDevTreeDevGrpAssignRsp   m_cRsp;
	vector<CDevTreeInfo>  m_vecDevTrees;              
	
	u32              m_dwVidSrcGbInfoIndex; //m_vecVidSrcGbInfo的gbid当前完善index
	string           m_strCompletingGrpId;  //当前正在完善设备gbid的分组Id
	map<string, vector<TVidSrcGbInfo> > m_mapGrpId2DevGbInfos; //grpId-->视频源信息
	CDeviceGroupInfo m_cSrcDevGrpInfo;
};

#endif // _DEVTREE_DEVGRP_ASSIGN_TASK_H_
