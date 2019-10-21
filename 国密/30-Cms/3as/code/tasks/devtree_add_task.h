/*
tas��ӷ���,���������£�
1��ϵͳ�豸���½����飻
2���Զ����豸����ק���顣
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
		enDealNoCopy, //�ǿ����豸�������߼�
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
	void SetMapRelation4Grps(); //���ö�Ӧ��ϵ
	
	TKDSIP_TRANS_ID  m_tTransId;               // �����SIP����ID
	string           m_strDevTreeName;         // �豸��ID
	string           m_strRootDevGrpGbid;      // ������gbid
	CDevTreeAddReq   m_cReq;
	CDevTreeAddRsp   m_cRsp;
	u32              m_dwVidSrcGbInfoIndex; //m_vecVidSrcGbInfo��gbid��ǰ����index
	string           m_strCompletingGrpId;  //��ǰ���������豸gbid�ķ���Id
	map<string, vector<TVidSrcGbInfo> > m_mapGrpId2DevGbInfos; //grpId-->��ƵԴ��Ϣ
	bool m_bNormalOver; //�Ƿ���������
	bool m_bHasRsped;   //�Ƿ���Ӧ��
	bool m_bStartQryNewTree;
	bool m_bWaitGbsRsp;
	u32            m_dwGrpGbidSetIndex;
	u32            m_dwMaxFullPathLen;   //���fullpath�ֶγ���
	map<string, string> m_mapOldGrpId2NewGrpId; //keyΪsrcGrpId��valueΪ��Ӧ�·���dstGrpId
	vector<CDeviceGroupInfo> m_vecSrcGrpInfo;
	vector<CDeviceGroupInfo> m_vecNewGrpInfo;
};

#endif // _DEVTREE_ADD_TASK_H_
