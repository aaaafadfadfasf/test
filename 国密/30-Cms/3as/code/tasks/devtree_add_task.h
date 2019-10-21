/*
tas添加分组,处理场景如下：
1、系统设备树新建分组；
2、自定义设备树拖拽分组。
*/
#ifndef _DEVTREE_ADD_TASK_H_
#define _DEVTREE_ADD_TASK_H_
#include <queue>
#include "inner_common.h"
#include "gbs/gbs_struct.h"



class CDevtreeAddTask: public CSipTask
{
public:
	enum
	{
		enIdle = UNKNOWN_STATE + 1,
		enAdd,
		enDealNoCopy, //非拷贝设备树处理逻辑
		enQrySrcTree,
		enSetDevGrpGbid,
		enCompleteDevGbid,
	};

	CDevtreeAddTask(CInstExt *pcInst);
	~CDevtreeAddTask();

	virtual const char* GetObjName() const
	{
		return "CDevtreeAddTask";
	}

	virtual const char* GetStateName(u32 dwState) const
	{
		switch (dwState)
		{
		case enIdle:
			return "Idle";
		case enAdd:
			return "Add";
		case enDealNoCopy:
			return "DealNoCopy";
		case enQrySrcTree:
			return "QrySrcTree";
		case enSetDevGrpGbid:
			return "SetDevGrpGbid";
		case enCompleteDevGbid:
			return "CompleteDevGbid";
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
	u32 OnAdd(CMessage *const pcMsg);
	u32 OnAddTimer();
	u32 OnDealNoCopy(CMessage *const pcMsg);
	u32 OnDealNoCopyTimer();
	u32 OnQrySrcTree(CMessage *const pcMsg);
	u32 OnQrySrcTreeTimer();
	u32 OnSetDevGrpGbid(CMessage *const pcMsg);
	u32 OnSetDevGrpGbidTimer();
	u32 OnCompleteDevGbid(CMessage *const pcMsg);
	u32 OnCompleteDevGbidTimer();

	
	void SendRsp(u32 dwErrorCode = CMS_SUCCESS);
	void QryGrpGbid(bool bRootGrp=false);
	void QrySrcTreeInfo();
	bool QryEmptyGbidDevs();
	bool QryDevGbids();
	bool CompleteDevGbids(vector<TGbDevGbidBatchGetInfo>& vecBatchDevInfo);
	void SetMapRelation4Grps(); //设置对应关系
	
	TKDSIP_TRANS_ID  m_tTransId;               // 请求的SIP事务ID
	string           m_strDevTreeName;         // 设备组ID
	string           m_strRootDevGrpGbid;      // 根分组gbid
	CDevTreeAddReq   m_cReq;
	CDevTreeAddRsp   m_cRsp;
	u32              m_dwVidSrcGbInfoIndex; //m_vecVidSrcGbInfo的gbid当前完善index
	string           m_strCompletingGrpId;  //当前正在完善设备gbid的分组Id
	map<string, vector<TVidSrcGbInfo> > m_mapGrpId2DevGbInfos; //grpId-->视频源信息
	bool m_bNormalOver; //是否正常结束
	bool m_bHasRsped;   //是否已应答
	bool m_bStartQryNewTree;
	bool m_bWaitGbsRsp;
	u32            m_dwGrpGbidSetIndex;
	u32            m_dwMaxFullPathLen;   //最大fullpath字段长度
	map<string, string> m_mapOldGrpId2NewGrpId; //key为srcGrpId，value为对应新分组dstGrpId
	vector<CDeviceGroupInfo> m_vecSrcGrpInfo;
	vector<CDeviceGroupInfo> m_vecNewGrpInfo;
};

#endif // _DEVTREE_ADD_TASK_H_
