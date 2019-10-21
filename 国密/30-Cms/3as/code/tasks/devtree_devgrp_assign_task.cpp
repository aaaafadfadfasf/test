/*
思路：若目的树canBeUse4Gb为false则处理逻辑不变，否则需补全
*/
#include "devtree_devgrp_assign_task.h"
#include "gbs/gbs_proto.h"
#include "cms/commonconfig/cmcfg.h"
#include "db_task_pool.h"

extern CDbTaskPool g_cDbTaskPool;
CDevtreeDevgrpAssignTask::CDevtreeDevgrpAssignTask( CInstExt *pcInst ): CSipTask(pcInst)
{
	m_tTransId = INVALID_TRANS_ID;
	m_dwVidSrcGbInfoIndex = 0;
}

CDevtreeDevgrpAssignTask::~CDevtreeDevgrpAssignTask()
{

}

void CDevtreeDevgrpAssignTask::PrintData() const
{
	CTask::PrintData();

	OspPrintf(1, 0, "m_mapGrpId2DevGbInfos.size[%d] -- m_dwDevInfosIndex[%d] \n", m_mapGrpId2DevGbInfos.size(), m_dwVidSrcGbInfoIndex);
	OspPrintf(1, 0, "m_strDevTreeName[%s]--- m_strRootDevGrpGbid[%s] \n", m_strDevTreeName.c_str(), m_strRootDevGrpGbid.c_str());
	OspPrintf(1, 0, "m_cReq[%s] \n", m_cReq.ToXml().c_str());
	OspPrintf(1, 0, "m_cRsp[%s] \n", m_cRsp.ToXml().c_str());
	
}

void CDevtreeDevgrpAssignTask::InitStateMachine()
{
	CStateProc cIdleProc;
	cIdleProc.ProcMsg = (CTask::PFProcMsg)&CDevtreeDevgrpAssignTask::OnIdle;
	cIdleProc.ProcErrMsg = (CTask::PFProcMsg)&CDevtreeDevgrpAssignTask::OnIdle;
	cIdleProc.TimerPoll = (CTask::PFTimerPoll)&CDevtreeDevgrpAssignTask::OnIdleTimer;
	AddRuleProc(enIdle, cIdleProc);

	CStateProc cAssignProc;
	cAssignProc.ProcMsg = (CTask::PFProcMsg)&CDevtreeDevgrpAssignTask::OnAssign;
	cAssignProc.ProcErrMsg = (CTask::PFProcMsg)&CDevtreeDevgrpAssignTask::OnAssign;
	cAssignProc.TimerPoll = (CTask::PFTimerPoll)&CDevtreeDevgrpAssignTask::OnAssignTimer;
	AddRuleProc(enAssign, cAssignProc);

	CStateProc cQryTasProc;
	cQryTasProc.ProcMsg = (CTask::PFProcMsg)&CDevtreeDevgrpAssignTask::OnQryTas;
	cQryTasProc.ProcErrMsg = (CTask::PFProcMsg)&CDevtreeDevgrpAssignTask::OnQryTas;
	cQryTasProc.TimerPoll = (CTask::PFTimerPoll)&CDevtreeDevgrpAssignTask::OnQryTasTimer;
	AddRuleProc(enQryTas, cQryTasProc);

	CStateProc cQryNewGrpIdProc;
	cQryNewGrpIdProc.ProcMsg = (CTask::PFProcMsg)&CDevtreeDevgrpAssignTask::OnQryNewGrpId;
	cQryNewGrpIdProc.ProcErrMsg = (CTask::PFProcMsg)&CDevtreeDevgrpAssignTask::OnQryNewGrpId;
	cQryNewGrpIdProc.TimerPoll = (CTask::PFTimerPoll)&CDevtreeDevgrpAssignTask::OnQryNewGrpIdTimer;
	AddRuleProc(enQryNewGrpId, cQryNewGrpIdProc);

	CStateProc cQryGbsProc;
	cQryGbsProc.ProcMsg = (CTask::PFProcMsg)&CDevtreeDevgrpAssignTask::OnQryGbs;
	cQryGbsProc.ProcErrMsg = (CTask::PFProcMsg)&CDevtreeDevgrpAssignTask::OnQryGbs;
	cQryGbsProc.TimerPoll = (CTask::PFTimerPoll)&CDevtreeDevgrpAssignTask::OnQryGbsTimer;
	AddRuleProc(enSyncDevsGbid, cQryGbsProc);

	NextState(enIdle);
}

u32 CDevtreeDevgrpAssignTask::OnIdle( CMessage *const pcMsg )
{
	if (pcMsg->event != TAS_DEVTREE_DEVGROUP_ASSIGN_REQ)
	{
		Log(ERROR_LEV, "CDevtreeDevgrpAssignTask[%d]::OnIdle recv unknown msg[%d-%s] \n",
			GetTaskNO(), pcMsg->event, OspExtEventDesc(pcMsg->event).c_str());
		return PROCMSG_DEL;
	}

	COspSipMsg* const pcSipMsg = reinterpret_cast<COspSipMsg* const>(pcMsg->content);
	pcSipMsg->GetMsgBody(m_cReq);

	m_tTransId = pcSipMsg->GetSipTransID();

	//直接丢到数据库操作线程
	CTaskMsgTransfer<CDevTreeDevGrpAssignReq> cTrans(GetTaskNO(),m_tTransId, pcMsg->event);
	pcSipMsg->GetMsgBody(cTrans.GetMsgObj());
	if(!PostTaskMsgTransfer2DbTaskPool(cTrans))
	{
		SendRsp(ERR_TAS_SYSTEM_BUSY);
		Log(ERROR_LEV, "CDevtreeDevgrpAssignTask[%d]::OnIdle PostTaskMsgTransfer2DbTaskPool fail \n",GetTaskNO());
		return PROCMSG_DEL;
	}

	NextState(enAssign);
	return PROCMSG_OK;
}

u32 CDevtreeDevgrpAssignTask::OnIdleTimer()
{
	return TIMERPOLL_DONE;
}

u32 CDevtreeDevgrpAssignTask::OnAssign( CMessage *const pcMsg )
{
	if (msgTaskMsgTransfer4Db != pcMsg->event)
	{
		return PROCMSG_FAIL;
	}

	ptrint ptr = *reinterpret_cast<ptrint*>(pcMsg->content);
	const CTaskMsgTransferBase* pcMsgTrans = reinterpret_cast<const CTaskMsgTransferBase*>(ptr);
	if (TAS_DEVTREE_DEVGROUP_ASSIGN_RSP == pcMsgTrans->GetEventId())
	{
		CDevTreeDevGrpAssignRsp *pcRsp = reinterpret_cast<CDevTreeDevGrpAssignRsp*>(pcMsgTrans->GetMsgObjAddr());
		m_cRsp = *pcRsp;
		SendRsp(pcRsp->GetErrorCode());
		m_strTransOprId = m_cRsp.GetOperUuid();

		if (CMS_SUCCESS != pcRsp->GetErrorCode())
		{
			Log(ERROR_LEV, "CDevtreeDevgrpAssignTask::OnAdd fail,errcode[%d],req[%s] \n",
				pcRsp->GetErrorCode(), m_cReq.ToXml().c_str());
			return PROCMSG_DEL;
		}

		NextState(enQryTas);

		//确认目的设备树是否可用于国标选择推送
		CTaskMsgTransfer<CDevTreeQryReq> cTrans(GetTaskNO(), m_tTransId, TAS_DEVTREE_QRY_REQ);
		if(!PostTaskMsgTransfer2DbTaskPool(cTrans))
		{
			Log(ERROR_LEV, "CDevtreeDevgrpAssignTask[%d]::OnAdd PostTaskMsgTransfer2DbTaskPool fail \n",GetTaskNO());
			return PROCMSG_DEL;
		}

		return PROCMSG_OK;
	}
	
	return PROCMSG_FAIL;
}

u32 CDevtreeDevgrpAssignTask::OnAssignTimer()
{
	return TIMERPOLL_DONE;
}

u32 CDevtreeDevgrpAssignTask::OnQryTas( CMessage *const pcMsg )
{
	if (msgTaskMsgTransfer4Db == pcMsg->event)
	{
		ptrint ptr = *reinterpret_cast<ptrint*>(pcMsg->content);
		const CTaskMsgTransferBase* pcMsgTrans = reinterpret_cast<const CTaskMsgTransferBase*>(ptr);
		switch(pcMsgTrans->GetEventId())
		{
		case TAS_DEVTREE_QRY_RSP:
			{
				CDevTreeQryRsp *pcRsp = reinterpret_cast<CDevTreeQryRsp*>(pcMsgTrans->GetMsgObjAddr());
				if (pcRsp->GetErrorCode() != CMS_SUCCESS)
				{
					Log(ERROR_LEV, "CDevtreeDevgrpAssignTask::OnQryTas Recv TAS_DEVTREE_QRY_RSP with errCode[%d] \n", pcRsp->GetErrorCode());
					return PROCMSG_DEL;
				}

				if (pcRsp->GetTotalEntryNum() <= 0)
				{
					Log(ERROR_LEV, "CDevtreeDevgrpAssignTask::OnQryTas Recv TAS_DEVTREE_QRY_RSP with totalNum[%d] \n", pcRsp->GetTotalEntryNum());
					return PROCMSG_DEL;
				}
			}
			break;
		case TAS_DEVTREE_QRY_NTF_REQ:
			{
				CDevTreeQryNtfReq *pcNtf = reinterpret_cast<CDevTreeQryNtfReq*>(pcMsgTrans->GetMsgObjAddr());
				m_vecDevTrees.insert(m_vecDevTrees.end(), pcNtf->GetInfos().begin(), pcNtf->GetInfos().end());
				if (pcNtf->IsLastNtf())
				{
					vector<CDevTreeInfo> ::iterator it;
					for ( it = m_vecDevTrees.begin(); it != m_vecDevTrees.end(); ++it)
					{
						if (it->GetDevtreeId() == m_cReq.GetDstDevTreeId())
						{
							break;
						}
					}
					if (it == m_vecDevTrees.end())
					{
						Log(ERROR_LEV, "CDevtreeDevgrpAssignTask::OnQryTas qry no devtree[%s] \n", m_cReq.GetDstDevTreeId().c_str());
						return PROCMSG_DEL;
					}

					if (!it->GetCanBeUsed4Gb())
					{
						return PROCMSG_DEL;
					}
					else
					{
						//查询源分组Id
						CTaskMsgTransfer<CDeviceGroupQryReq> cTrans(GetTaskNO(), m_tTransId, TAS_DEVICE_GROUP_QRY_REQ);
						cTrans.GetMsgObj().GetDeviceGroupInfoQueryCondition().SetDevTreeId(m_cReq.GetSrcDevTreeId());
						cTrans.GetMsgObj().GetDeviceGroupInfoQueryCondition().SetDeviceGroupID(m_cReq.GetSrcGroupId());
						if(!PostTaskMsgTransfer2DbTaskPool(cTrans))
						{
							Log(ERROR_LEV, "CDevtreeDevgrpAssignTask[%d]::OnAdd PostTaskMsgTransfer2DbTaskPool fail \n",GetTaskNO());
							return PROCMSG_DEL;
						}
					}
				}
			}
			break;
		case TAS_DEVICE_GROUP_QRY_RSP:
			{
				CDeviceGroupQryRsp *pcRsp = reinterpret_cast<CDeviceGroupQryRsp*>(pcMsgTrans->GetMsgObjAddr());
				if (pcRsp->GetErrorCode() != CMS_SUCCESS || pcRsp->GetTotalEntryNum() <= 0)
				{
					Log(ERROR_LEV, "CDevtreeDevgrpAssignTask::OnQryTas recv TAS_DEVICE_GROUP_QRY_RSP with errcode[%d],totalNum[%d] \n",
						pcRsp->GetErrorCode(), pcRsp->GetTotalEntryNum());
					return PROCMSG_DEL;
				}
			}
			break;
		case TAS_DEVICE_GROUP_QRY_NTF_REQ:
			{
				CDeviceGroupQryNtfReq *pcNtf = reinterpret_cast<CDeviceGroupQryNtfReq*>(pcMsgTrans->GetMsgObjAddr());
				for (vector<CDeviceGroupInfo> ::iterator it = pcNtf->GetDeviceGroupInfo().begin();
					it != pcNtf->GetDeviceGroupInfo().end(); ++it)
				{
					if (it->GetDeviceGroupID() == m_cReq.GetSrcGroupId())
					{
						m_cSrcDevGrpInfo = *it;
						NextState(enQryNewGrpId);

						//查询目的树新分组Id
						CTaskMsgTransfer<CDeviceGroupQryReq> cTrans(GetTaskNO(), m_tTransId, TAS_DEVICE_GROUP_QRY_REQ);
						cTrans.GetMsgObj().GetDeviceGroupInfoQueryCondition().SetDevTreeId(m_cReq.GetDstDevTreeId());
						cTrans.GetMsgObj().GetDeviceGroupInfoQueryCondition().SetDeviceParentGroupID(m_cReq.GetDstParentGrpId());
						cTrans.GetMsgObj().GetDeviceGroupInfoQueryCondition().SetDeviceGroupName(m_cSrcDevGrpInfo.GetDeviceGroupName());
						if(!PostTaskMsgTransfer2DbTaskPool(cTrans))
						{
							Log(ERROR_LEV, "CDevtreeDevgrpAssignTask[%d]::OnAdd PostTaskMsgTransfer2DbTaskPool fail \n",GetTaskNO());
							return PROCMSG_DEL;
						}
					}
				}
			}
			break;
		default:
			{
				Log(ERROR_LEV, "CDevtreeDevgrpAssignTask::OnQryTas Recv Unknown msg[%d-%s] \n",
					pcMsgTrans->GetEventId(), OspExtEventDesc(pcMsgTrans->GetEventId()).c_str());
				return PROCMSG_FAIL;
			}
		};
	}
	else
	{
		Log(ERROR_LEV, "CDevtreeDevgrpAssignTask::OnQryTas Recv Unknown msg[%d-%s] \n",
			pcMsg->event, OspExtEventDesc(pcMsg->event).c_str());
		return PROCMSG_FAIL;
	}

	return PROCMSG_OK;
}

u32 CDevtreeDevgrpAssignTask::OnQryTasTimer()
{
	return TIMERPOLL_DONE;
}

u32 CDevtreeDevgrpAssignTask::OnQryNewGrpId( CMessage *const pcMsg )
{
	if (msgTaskMsgTransfer4Db == pcMsg->event)
	{
		ptrint ptr = *reinterpret_cast<ptrint*>(pcMsg->content);
		const CTaskMsgTransferBase* pcMsgTrans = reinterpret_cast<const CTaskMsgTransferBase*>(ptr);
		switch(pcMsgTrans->GetEventId())
		{
		case TAS_DEVICE_GROUP_QRY_RSP:
			{
				CDeviceGroupQryRsp *pcRsp = reinterpret_cast<CDeviceGroupQryRsp*>(pcMsgTrans->GetMsgObjAddr());
				if (pcRsp->GetErrorCode() != CMS_SUCCESS || pcRsp->GetTotalEntryNum() <= 0)
				{
					Log(ERROR_LEV, "CDevtreeDevgrpAssignTask::OnQryNewGrpId recv TAS_DEVICE_GROUP_QRY_RSP with errcode[%d],totalNum[%d] \n",
						pcRsp->GetErrorCode(), pcRsp->GetTotalEntryNum());
					return PROCMSG_DEL;
				}
			}
			break;
		case TAS_DEVICE_GROUP_QRY_NTF_REQ:
			{
				CDeviceGroupQryNtfReq *pcNtf = reinterpret_cast<CDeviceGroupQryNtfReq*>(pcMsgTrans->GetMsgObjAddr());
				for (vector<CDeviceGroupInfo> ::iterator it = pcNtf->GetDeviceGroupInfo().begin();
					it != pcNtf->GetDeviceGroupInfo().end(); ++it)
				{
					if (it->GetDevtreeId() == m_cReq.GetDstDevTreeId()
						&& it->GetDeviceParentGroupID() == m_cReq.GetDstParentGrpId()
						&& it->GetDeviceGroupName() == m_cSrcDevGrpInfo.GetDeviceGroupName())
					{
						m_strNewDevGrpId = it->GetDeviceGroupID();
						
						TTransProgress tTProgress = g_cDbTaskPool.GetTransProgress(m_strTransOprId);
						if (tTProgress.finishNum >= tTProgress.totalNum)
						{
							m_strTransOprId.clear(); 

							//查询指定设备树指定分组下设备的gbid
							CTaskMsgTransfer<CDeviceGroupChildrenQryReq> cTrans(GetTaskNO(), m_tTransId, TAS_DEVICE_GROUP_CHILDREN_QRY_REQ);
							cTrans.GetMsgObj().SetDevTreeId(m_cReq.GetDstDevTreeId());
							cTrans.GetMsgObj().SetDevGrpId(it->GetDeviceGroupID());
							if(!PostTaskMsgTransfer2DbTaskPool(cTrans))
							{
								Log(ERROR_LEV, "CDevtreeDevgrpAssignTask[%d]::OnAdd PostTaskMsgTransfer2DbTaskPool fail \n",GetTaskNO());
								return PROCMSG_DEL;
							}
						}
					}
				}
			}
			break;
		case TAS_DEVICE_GROUP_CHILDREN_QRY_RSP:
			{
				CDeviceGroupChildrenQryRsp *pcRsp = reinterpret_cast<CDeviceGroupChildrenQryRsp*>(pcMsgTrans->GetMsgObjAddr());
				if (pcRsp->GetErrorCode() != CMS_SUCCESS)
				{
					Log(ERROR_LEV, "CDevtreeDevgrpAssignTask::OnQryTas Recv TAS_DEVICE_GROUP_CHILDREN_QRY_RSP with errCode[%d] \n", pcRsp->GetErrorCode());
					return PROCMSG_DEL;
				}

				if (pcRsp->GetTotalEntryNum() <= 0)
				{
					Log(ERROR_LEV, "CDevtreeDevgrpAssignTask::OnQryTas Recv TAS_DEVICE_GROUP_CHILDREN_QRY_RSP with totalNum[%d] \n", pcRsp->GetTotalEntryNum());
					return PROCMSG_DEL;
				}
			}
			break;
		case TAS_DEVICE_GROUP_CHILDREN_QRY_NTF_REQ:
			{
				CDeviceGroupChildrenQryNtfReq *pcNtf = reinterpret_cast<CDeviceGroupChildrenQryNtfReq*>(pcMsgTrans->GetMsgObjAddr());
				if (!pcNtf->GetDevInfos().empty())
				{
					vector<TDeviceInfo4Gb> ::iterator it2;
					map<string, vector<TVidSrcGbInfo> > ::iterator it3;
					TVidSrcGbInfo tVidSrcGbInfo;
					vector<int> vecVidSrcIdxs;
					for (vector<CDeviceGroupDeviceInfo4Gb> ::iterator it = pcNtf->GetDevInfos().begin(); it != pcNtf->GetDevInfos().end(); ++it)
					{
						vecVidSrcIdxs.clear();
						//需要建立分组--视频源的映射关系才好处理
						for (it2 = it->GetDevinfo().begin(); it2 != it->GetDevinfo().end(); ++it2)
						{
							if (it2->GetGbid().GetNewprimarygbid().empty())
							{
								vecVidSrcIdxs.push_back(it2->GetVidIndex());
							}
						}
						if (!vecVidSrcIdxs.empty())
						{
							tVidSrcGbInfo.devGrpId = it->GetDevGrpId();
							tVidSrcGbInfo.devId = it->GetDeviceId();
							//tVidSrcGbInfo.vidIndex = it
							it3 = m_mapGrpId2DevGbInfos.find(it->GetDevGrpId());
							if (it3 != m_mapGrpId2DevGbInfos.end())
							{
								for (int i = 0; i < vecVidSrcIdxs.size(); ++i)
								{
									tVidSrcGbInfo.vidIndex=vecVidSrcIdxs[i];
									it3->second.push_back(tVidSrcGbInfo);
								}
							}
							else
							{
								vector<TVidSrcGbInfo> vecTmp;
								for (int i = 0; i<vecVidSrcIdxs.size(); ++i)
								{
									tVidSrcGbInfo.vidIndex=vecVidSrcIdxs[i];
									vecTmp.push_back(tVidSrcGbInfo);
								}

								m_mapGrpId2DevGbInfos.insert(std::pair<string, vector<TVidSrcGbInfo> >(it->GetDevGrpId(), vecTmp));
							}
						}
					}
				}

				if (pcNtf->IsLastNtf())
				{
					if (m_mapGrpId2DevGbInfos.empty())
					{
						return PROCMSG_DEL;
					}
					else
					{
						if(!QryGbidFromGbs())
						{
							return PROCMSG_DEL;
						}
						NextState(enSyncDevsGbid);
					}
				}

				return PROCMSG_OK;
			}
			break;
		}
	}

	return PROCMSG_FAIL;
}

u32 CDevtreeDevgrpAssignTask::OnQryNewGrpIdTimer()
{
	if (!m_strNewDevGrpId.empty() && !m_strTransOprId.empty())
	{
		TTransProgress tTProgress = g_cDbTaskPool.GetTransProgress(m_strTransOprId);
		if (tTProgress.finishNum >= tTProgress.totalNum)
		{
			//查询指定设备树指定分组下设备的gbid
			CTaskMsgTransfer<CDeviceGroupChildrenQryReq> cTrans(GetTaskNO(), m_tTransId, TAS_DEVICE_GROUP_CHILDREN_QRY_REQ);
			cTrans.GetMsgObj().SetDevTreeId(m_cReq.GetDstDevTreeId());
			cTrans.GetMsgObj().SetDevGrpId(m_strNewDevGrpId);
			if(!PostTaskMsgTransfer2DbTaskPool(cTrans))
			{
				Log(ERROR_LEV, "CDevtreeDevgrpAssignTask[%d]::OnAdd PostTaskMsgTransfer2DbTaskPool fail \n",GetTaskNO());
				return PROCMSG_DEL;
			}
		}
	}
	return TIMERPOLL_DONE;
}

u32 CDevtreeDevgrpAssignTask::OnQryGbs( CMessage *const pcMsg )
{
	switch(pcMsg->event)
	{
	case GBS_DEV_GBID_BATCH_GET_RSP:
		{
			COspSipMsg* pcSipMsg = reinterpret_cast<COspSipMsg*>(pcMsg->content);
			CGbsDevGbidBatchGetRsp cRsp;
			pcSipMsg->GetMsgBody(cRsp);
			if (cRsp.GetErrorCode() != CMS_SUCCESS)
			{
				Log(ERROR_LEV, "CDevtreeDevgrpAssignTask::OnQryGbs recv GBS_DEV_GBID_BATCH_GET_RSP with errCode[%d] \n",
					cRsp.GetErrorCode());
				return PROCMSG_DEL;
			}

			SetDevsGbid(cRsp.GetVecBatchDevInfo());
		}
		break;
	case msgDbMsgTransfer:
		{
			ptrint ptr = *reinterpret_cast<ptrint*>(pcMsg->content);
			const CDbMsgTransferBase* pcMsgTrans = reinterpret_cast<const CDbMsgTransferBase*>(ptr);
			if (msgSetDevGbidRsp == pcMsgTrans->GetEventId())
			{
				TSetDevGbidRsp * pRsp = reinterpret_cast<TSetDevGbidRsp*>(pcMsgTrans->GetMsgObjAddr());
				if (pRsp->GetErrno() != CMS_SUCCESS)
				{
					Log(ERROR_LEV, "CDevtreeDevgrpAssignTask set trees[%s]'s dev's gbid fail!!! \n",
						m_cReq.GetDstDevTreeId().c_str());
					return PROCMSG_DEL;
				}

				if (!QryGbidFromGbs())
				{

					return PROCMSG_DEL;
				}
				else
				{
					return PROCMSG_OK;
				}
			}
		}
		break;
	default:
		{
			Log(ERROR_LEV, "CDevtreeDevgrpAssignTask recv unknown msg[%d-%s]! \n",
				pcMsg->event, OspExtEventDesc(pcMsg->event).c_str());
			return PROCMSG_FAIL;
		}
	}
	return PROCMSG_OK;
	
}

u32 CDevtreeDevgrpAssignTask::OnQryGbsTimer()
{
	return TIMERPOLL_DONE;
}

void CDevtreeDevgrpAssignTask::SendRsp( u32 dwErrorCode /*= CMS_SUCCESS*/ )
{
	m_cRsp.SetErrorCode(dwErrorCode);
	PostMsgRsp(m_tTransId, m_cRsp);
}

void CDevtreeDevgrpAssignTask::SetDevsGbid( const vector<TGbDevGbidBatchGetInfo>& vecGbInfos )
{
	//直接丢到数据库操作线程
	TDevGbId tDevIds;
	tDevIds.operType = 0;
	CDbMsgTransfer<TSetDevGbidReq> cTrans(MAKEIID(GetAppID(), GetInsID()), GetTaskNO(), msgSetDevGbidReq);
	cTrans.GetMsgObj().devTreeId  = m_cReq.GetDstDevTreeId();
	cTrans.GetMsgObj().devGrpId = m_strCompletingGrpId;
	for (vector<TGbDevGbidBatchGetInfo> ::const_iterator it = vecGbInfos.begin(); it != vecGbInfos.end(); ++it)
	{
		tDevIds.devId = it->GetKdid();
		tDevIds.priGbid = it->GetPrimary();
		tDevIds.vidSrcIdx = it->GetVidindex();

		cTrans.GetMsgObj().DevGbids.push_back(tDevIds);
	}

	if(!PostDbMsgTransfer2DbTaskPool(cTrans))
	{
		Log(ERROR_LEV, "CDevtreeDevgrpAssignTask[%d]::OnIdle PostTaskMsgTransfer2DbTaskPool fail \n",GetTaskNO());
	}
}

bool CDevtreeDevgrpAssignTask::QryGbidFromGbs()
{
	CGbsDevGbidBatchGetReq cReq;
	TGbDevGbidBatchGetCondition condition;

	if (m_mapGrpId2DevGbInfos.empty())
	{
		return false;
	}
	map<string, vector<TVidSrcGbInfo> > ::iterator it; 
	if (!m_strCompletingGrpId.empty())
	{
		it = m_mapGrpId2DevGbInfos.find(m_strCompletingGrpId);
		if (it != m_mapGrpId2DevGbInfos.end())
		{
			if (m_dwVidSrcGbInfoIndex >= it->second.size())
			{
				m_mapGrpId2DevGbInfos.erase(it);
				if (m_mapGrpId2DevGbInfos.empty())
				{
					return false;
				}
				it = m_mapGrpId2DevGbInfos.begin();
				m_dwVidSrcGbInfoIndex = 0;
				m_strCompletingGrpId = it->first;
			}
			else
			{

			}
		}
		else
		{
			return false;
		}
	}
	else
	{
		it = m_mapGrpId2DevGbInfos.begin();
		m_strCompletingGrpId = it->first;
		m_dwVidSrcGbInfoIndex = 0;
	}

	for (int i = m_dwVidSrcGbInfoIndex; i < it->second.size(); ++i)
	{
		condition.SetKdid(it->second[i].devId);
		condition.GetVidindexs().push_back(it->second[i].vidIndex);
		condition.GetPrisec().push_back(0);

		cReq.GetVecConditions().push_back(condition);
		++m_dwVidSrcGbInfoIndex;

		if (cReq.GetVecConditions().size()>20)
		{
			break;
		}
		condition.GetVidindexs().clear();
		condition.GetPrisec().clear();
	}
	if (!cReq.GetVecConditions().empty())
	{
		TSipURI tUri;
		tUri.SetDomain(GetDomainName());
		if(PROCMSG_OK != PostReq(KDSIP_EVENT_MESSAGE_REQ, cReq, tUri.GetGbsURI()))
		{
			return false;
		}
	}

	return true;
}
