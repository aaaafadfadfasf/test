#include "devtree_add_task.h"
#include "cctrl.h"
#include "cms/gbs/gbs_proto.h"
#include "cms/commonconfig/cmcfg.h"
#include "db_task_pool.h"
#include "redis/fastdbmng.h"

extern CDbTaskPool g_cDbTaskPool;
CDevtreeAddTask::CDevtreeAddTask( CInstExt *pcInst ): CSipTask(pcInst)
{
	m_tTransId = INVALID_TRANS_ID;
	m_dwVidSrcGbInfoIndex = 0;
	m_dwGrpGbidSetIndex = 0;
	m_bNormalOver = false;
	m_bStartQryNewTree = false;
	m_bHasRsped = false;
	m_bWaitGbsRsp = false;
	m_dwMaxFullPathLen = 0;
}

CDevtreeAddTask::~CDevtreeAddTask()
{
	if (m_bNormalOver)
	{
		SendRsp(CMS_SUCCESS);
		g_cDbTaskPool.AddTransProgress(m_cRsp.GetOperUuid(), 100);
	}
	else
	{
		if (!m_cRsp.GetDevtreeId().empty())
		{
			CTaskMsgTransfer<CDevTreeDelReq> cTrans(GetTaskNO(), TKDSIP_TRANS_ID(), TAS_DEVTREE_DEL_REQ);
			cTrans.GetMsgObj().SetDevtreeId(m_cRsp.GetDevtreeId());
			PostTaskMsgTransfer2DbTaskPool(cTrans);
		}
	}
	
}

void CDevtreeAddTask::PrintData() const
{
	CSipTask::PrintData();
	OspPrintf(1, 0, "m_strGateWayId[%s]---m_strRootDevGrpGbid[%s] \n", m_strDevTreeName.c_str(), m_strRootDevGrpGbid.c_str());
}

void CDevtreeAddTask::InitStateMachine()
{
	CStateProc cIdleProc;
	cIdleProc.ProcMsg = (CTask::PFProcMsg)&CDevtreeAddTask::OnIdle;
	cIdleProc.ProcErrMsg = (CTask::PFProcMsg)&CDevtreeAddTask::OnIdle;
	cIdleProc.TimerPoll = (CTask::PFTimerPoll)&CDevtreeAddTask::OnIdleTimer;
	AddRuleProc(enIdle, cIdleProc);

	CStateProc cAddProc;
	cAddProc.ProcMsg = (CTask::PFProcMsg)&CDevtreeAddTask::OnAdd;
	cAddProc.ProcErrMsg = (CTask::PFProcMsg)&CDevtreeAddTask::OnAdd;
	cAddProc.TimerPoll = (CTask::PFTimerPoll)&CDevtreeAddTask::OnAddTimer;
	AddRuleProc(enAdd, cAddProc);

	CStateProc cQryProc;
	cQryProc.ProcMsg = (CTask::PFProcMsg)&CDevtreeAddTask::OnDealNoCopy;
	cQryProc.ProcErrMsg = (CTask::PFProcMsg)&CDevtreeAddTask::OnDealNoCopy;
	cQryProc.TimerPoll = (CTask::PFTimerPoll)&CDevtreeAddTask::OnDealNoCopyTimer;
	AddRuleProc(enDealNoCopy, cQryProc);

	CStateProc cQrySrcTreeProc;
	cQrySrcTreeProc.ProcMsg = (CTask::PFProcMsg)&CDevtreeAddTask::OnQrySrcTree;
	cQrySrcTreeProc.ProcErrMsg = (CTask::PFProcMsg)&CDevtreeAddTask::OnQrySrcTree;
	cQrySrcTreeProc.TimerPoll = (CTask::PFTimerPoll)&CDevtreeAddTask::OnQrySrcTreeTimer;
	AddRuleProc(enQrySrcTree, cQrySrcTreeProc);

	CStateProc cSetDevGrpGbidProc;
	cSetDevGrpGbidProc.ProcMsg = (CTask::PFProcMsg)&CDevtreeAddTask::OnSetDevGrpGbid;
	cSetDevGrpGbidProc.ProcErrMsg = (CTask::PFProcMsg)&CDevtreeAddTask::OnSetDevGrpGbid;
	cSetDevGrpGbidProc.TimerPoll = (CTask::PFTimerPoll)&CDevtreeAddTask::OnSetDevGrpGbidTimer;
	AddRuleProc(enSetDevGrpGbid, cSetDevGrpGbidProc);

	CStateProc cCompleteDevGbidProc;
	cCompleteDevGbidProc.ProcMsg = (CTask::PFProcMsg)&CDevtreeAddTask::OnCompleteDevGbid;
	cCompleteDevGbidProc.ProcErrMsg = (CTask::PFProcMsg)&CDevtreeAddTask::OnCompleteDevGbid;
	cCompleteDevGbidProc.TimerPoll = (CTask::PFTimerPoll)&CDevtreeAddTask::OnCompleteDevGbidTimer;
	AddRuleProc(enCompleteDevGbid, cCompleteDevGbidProc);

	NextState(enIdle);
}

u32 CDevtreeAddTask::OnIdle( CMessage *const pcMsg )
{
	if (TAS_DEVTREE_ADD_REQ != pcMsg->event)
	{
		Log(ERROR_LEV, "CDevtreeAddTask[%d] recv unknown event[%d-%s] \n", GetTaskNO(), pcMsg->event, OspExtEventDesc(pcMsg->event).c_str());
		return PROCMSG_DEL;
	}

	COspSipMsg* const pcSipMsg = reinterpret_cast<COspSipMsg* const>(pcMsg->content);
	pcSipMsg->GetMsgBody(m_cReq);
	
	m_tTransId = pcSipMsg->GetSipTransID();

	
	//直接丢到数据库操作线程
	CTaskMsgTransfer<CDevTreeAddReq> cTrans(GetTaskNO(), pcSipMsg->GetSipTransID(), pcMsg->event);
	pcSipMsg->GetMsgBody(cTrans.GetMsgObj());
	if(!PostTaskMsgTransfer2DbTaskPool(cTrans))
	{
		SendRsp(ERR_TAS_SYSTEM_BUSY);
		Log(ERROR_LEV, "CDevtreeAddTask[%d]::OnIdle PostTaskMsgTransfer2DbTaskPool fail \n",GetTaskNO());
		return PROCMSG_DEL;
	}
	NextState(enAdd);
	
	return PROCMSG_OK;
}

u32 CDevtreeAddTask::OnIdleTimer()
{
	return TIMERPOLL_DONE;
}

u32 CDevtreeAddTask::OnAdd( CMessage *const pcMsg )
{
	if (msgTaskMsgTransfer4Db == pcMsg->event)
	{
		ptrint ptr = *reinterpret_cast<ptrint*>(pcMsg->content);
		const CTaskMsgTransferBase* pcMsgTrans = reinterpret_cast<const CTaskMsgTransferBase*>(ptr);
		if (TAS_DEVTREE_ADD_RSP == pcMsgTrans->GetEventId())
		{
			CDevTreeAddRsp *pcRsp = reinterpret_cast<CDevTreeAddRsp*>(pcMsgTrans->GetMsgObjAddr());

			m_cRsp.SetDevtreeId(pcRsp->GetDevtreeId());
			m_cRsp.SetOperUuid(pcRsp->GetOperUuid());

			if (CMS_SUCCESS != pcRsp->GetErrorCode())
			{
				Log(ERROR_LEV, "CDevtreeAddTask[%d]::OnAdd recv CDevTreeAddRsp with errCode[%d] \n", 
					GetTaskNO(), pcRsp->GetErrorCode() );
				SendRsp(pcRsp->GetErrorCode());
				return PROCMSG_DEL;
			}
			else
			{
				if (m_cReq.GetCopyFromOtherTree())
				{
					//设备树拷贝（即数据表拷贝）需要时间，且后续的业务依赖拷贝完的数据，故这里需要等待
					TTransProgress tProgress = g_cDbTaskPool.GetTransProgress(m_cRsp.GetOperUuid());
					ECMS_ERRORCODE errCode = g_cDbTaskPool.GetTransProErr(m_cRsp.GetOperUuid());
					if (tProgress.finishNum<15 && CMS_SUCCESS == errCode) //数据库处理未结束
					{
						SendRsp(pcRsp->GetErrorCode());
						return PROCMSG_OK;
					}
					else
					{
						if (CMS_SUCCESS != errCode)
						{
							Log(ERROR_LEV, "CDevtreeAddTask[%d]::OnAdd recv CDevTreeAddRsp with errCode[%d] \n", 
								GetTaskNO(), errCode);
							SendRsp(errCode);
							return PROCMSG_DEL;
						}
					}
				}
				
				if (!m_cReq.GetCanBeUsed4Gb())
				{
					m_bNormalOver = true;
					return PROCMSG_DEL;
				}
				else
				{
					if (!m_cReq.GetCopyFromOtherTree())
					{
						QryGrpGbid(true);
						NextState(enDealNoCopy);
					}
					else
					{
						if (m_cReq.GetSrcTreeId().empty())
						{
							CTaskMsgTransfer<CDeviceGroupQryReq> cTrans(GetTaskNO(), 0, TAS_DEVICE_GROUP_QRY_REQ);
							cTrans.GetMsgObj().GetDeviceGroupInfoQueryCondition().SetDevTreeId(m_cReq.GetSrcTreeId());
							if (!PostTaskMsgTransfer2DbTaskPool(cTrans))
							{
								SendRsp(ERR_TAS_SYSTEM_BUSY);
								Log(ERROR_LEV, "CDevtreeAddTask[%d]::OnIdle 1-PostTaskMsgTransfer2DbTaskPool fail \n",GetTaskNO());
								return PROCMSG_DEL;
							}
							NextState(enSetDevGrpGbid);
						}
						else
						{
							//判断源树是否是国标树，如果是则无需设置任何gbid（因为存储过程已经搞定了）；
							CTaskMsgTransfer<CDevTreeQryReq> cTrans(GetTaskNO(), 0, TAS_DEVTREE_QRY_REQ);
							if(!PostTaskMsgTransfer2DbTaskPool(cTrans))
							{
								SendRsp(ERR_TAS_SYSTEM_BUSY);
								Log(ERROR_LEV, "CDevtreeAddTask[%d]::OnIdle PostTaskMsgTransfer2DbTaskPool fail \n",GetTaskNO());
								return PROCMSG_DEL;
							}
							NextState(enQrySrcTree);
						}
					}
				}
			}

			return PROCMSG_OK;
		}
		else
		{
			Log(ERROR_LEV, "CDevtreeAddTask[%d] OnAdd recv unknown msg[%d-%s] \n", GetTaskNO(), pcMsgTrans->GetEventId(),
				OspExtEventDesc(pcMsgTrans->GetEventId()).c_str());
			return PROCMSG_DEL;
		}
	}
	else
	{
		Log(ERROR_LEV, "CDevtreeAddTask[%d]::OnQry recv unknown msg[%d-%s] \n",
			GetTaskNO(), pcMsg->event, OspExtEventDesc(pcMsg->event).c_str());
		return PROCMSG_FAIL;
	}
}

u32 CDevtreeAddTask::OnAddTimer()
{
	if (m_cReq.GetCopyFromOtherTree())
	{
		//设备树拷贝（即数据表拷贝）需要时间，且后续的业务依赖拷贝完的数据，故这里需要等待
		ECMS_ERRORCODE errCode = g_cDbTaskPool.GetTransProErr(m_cRsp.GetOperUuid());
		if (g_cDbTaskPool.GetTransProgress(m_cRsp.GetOperUuid()).finishNum>=15
			&& CMS_SUCCESS == errCode) //数据库操作线程拷贝完毕后会设置进度为15%
		{
			if (!m_cReq.GetCanBeUsed4Gb())
			{
				m_bNormalOver = true;
				return TIMERPOLL_DEL;
			}
			else
			{
				if (m_cReq.GetSrcTreeId().empty())
				{
					CTaskMsgTransfer<CDeviceGroupQryReq> cTrans(GetTaskNO(), 0, TAS_DEVICE_GROUP_QRY_REQ);
					cTrans.GetMsgObj().GetDeviceGroupInfoQueryCondition().SetDevTreeId(m_cReq.GetSrcTreeId());
					if (!PostTaskMsgTransfer2DbTaskPool(cTrans))
					{
						SendRsp(ERR_TAS_SYSTEM_BUSY);
						Log(ERROR_LEV, "CDevtreeAddTask[%d]::OnIdle 1-PostTaskMsgTransfer2DbTaskPool fail \n",GetTaskNO());
						return TIMERPOLL_DEL;
					}
					NextState(enSetDevGrpGbid);
				}
				else
				{
					//判断源树是否是国标树，如果是则无需设置任何gbid（因为存储过程已经搞定了）；
					CTaskMsgTransfer<CDevTreeQryReq> cTrans(GetTaskNO(), 0, TAS_DEVTREE_QRY_REQ);
					if(!PostTaskMsgTransfer2DbTaskPool(cTrans))
					{
						SendRsp(ERR_TAS_SYSTEM_BUSY);
						Log(ERROR_LEV, "CDevtreeAddTask[%d]::OnIdle PostTaskMsgTransfer2DbTaskPool fail \n",GetTaskNO());
						return TIMERPOLL_DEL;
					}
					NextState(enQrySrcTree);
				}
			}
		}
		else if (CMS_SUCCESS != errCode)
		{
			return TIMERPOLL_DEL;
		}
	}
	return TIMERPOLL_DONE;
}

u32 CDevtreeAddTask::OnDealNoCopy( CMessage *const pcMsg )
{
	switch(pcMsg->event)
	{
	case GBS_DEV_GRP_GBID_BATCH_GET_RSP:
		{
			COspSipMsg *pcSipMsg = reinterpret_cast<COspSipMsg*>(pcMsg->content);
			CGbsDevGrpGbidBatchGetRsp cRsp;
			pcSipMsg->GetMsgBody(cRsp);

			if (cRsp.GetErrorCode() != CMS_SUCCESS)
			{
				Log(ERROR_LEV, "CDevtreeAddTask::OnDealNoCopy recv rsp with errcode[%d] \n", cRsp.GetErrorCode());
				return PROCMSG_DEL;
			}

			if (cRsp.GetVecBatchDevGrpInfo().empty())
			{
				Log(ERROR_LEV, "CDevtreeAddTask::OnDealNoCopy recv GBS_DEVGRP_CIVILCODE_GET_RSP with empty gbid \n");
				return PROCMSG_DEL;
			}

			m_strRootDevGrpGbid = cRsp.GetVecBatchDevGrpInfo()[0].GetGbid();

			//设置到数据库
			CDbMsgTransfer<TSetDevGrpGbidReq> cTrans(MAKEIID(GetAppID(), GetInsID()), GetTaskNO(), msgSetDevGrpGbidReq);
			cTrans.GetMsgObj().devTreeId = m_cRsp.GetDevtreeId();
			cTrans.GetMsgObj().devGrpGbid = m_strRootDevGrpGbid;
			cTrans.GetMsgObj().devGrpId = m_cRsp.GetDevtreeId(); 
			if(!PostDbMsgTransfer2DbTaskPool(cTrans))
			{
				SendRsp(ERR_TAS_SYSTEM_BUSY);
				Log(ERROR_LEV, "CDevtreeAddTask[%d]::OnAdd PostDbMsgTransfer2DbTaskPool fail \n",GetTaskNO());
				return PROCMSG_DEL;
			}
			else
			{
				Log(EVENT_LEV, "CDevtreeAddTask[%d]::OnDealNoCopy set devgrp[%s]'s gbid[%s], tree[%s] \n",
					GetTaskNO(), m_cRsp.GetDevtreeId().c_str(), m_strRootDevGrpGbid.c_str(), m_cRsp.GetDevtreeId().c_str());
			}
			return PROCMSG_OK;
		}
		break;
	case msgDbMsgTransfer:
		{
			CDbMsgTransferBase *pcBase = reinterpret_cast<CDbMsgTransferBase*>(*reinterpret_cast<ptrint*>(pcMsg->content));
			if (pcBase->GetEventId() == msgSetDevGrpGbidRsp)
			{
				CDbMsgTransfer<TSetDevGrpGbidRsp> *pcRsp = static_cast<CDbMsgTransfer<TSetDevGrpGbidRsp>* >(pcBase);
				if (pcRsp->GetMsgObj().GetErrno() != CMS_SUCCESS)
				{
					Log(ERROR_LEV, "CDevtreeAddTask[%d]::OnDealNoCopy set devgrp fail,errCode[%d] \n",
						GetTaskNO(), pcRsp->GetMsgObj().GetErrno());
				}
				m_bNormalOver=true;
				return PROCMSG_DEL;
			}
			else
			{
				Log(ERROR_LEV, "CDevtreeAddTask::OnDealNoCopy recv unknown msg[%d-%s] \n", pcBase->GetEventId(), OspExtEventDesc(pcBase->GetEventId()).c_str());
				return PROCMSG_FAIL;
			}
		}
		break;
	default:
		{
			Log(ERROR_LEV, "CDevtreeAddTask::OnDealNoCopy recv unknown msg[%d-%s] \n", pcMsg->event, OspExtEventDesc(pcMsg->event).c_str());
			return PROCMSG_FAIL;
		}
	}
}

u32 CDevtreeAddTask::OnDealNoCopyTimer()
{
	if (GetCurStateHoldTime()>5 && m_strRootDevGrpGbid.empty())
	{
	    SendRsp(ERR_TAS_RELYMODULE_DEALTIMEOUT_NOSTART);
		Log(ERROR_LEV, "CDevtreeAddTask[%d]::OnDealNoCopyTimer wait GBS_DEV_GRP_GBID_BATCH_GET_RSP timeout !\n",GetTaskNO());
		return TIMERPOLL_DEL;
	}
	return TIMERPOLL_DONE;
}

u32 CDevtreeAddTask::OnQrySrcTree( CMessage *const pcMsg )
{
	if (msgTaskMsgTransfer4Db == pcMsg->event)
	{
		ptrint ptr = *reinterpret_cast<ptrint*>(pcMsg->content);
		const CTaskMsgTransferBase* pcMsgTrans = reinterpret_cast<const CTaskMsgTransferBase*>(ptr);
		if (TAS_DEVTREE_QRY_RSP == pcMsgTrans->GetEventId())
		{
			CDevTreeQryRsp *pcRsp = reinterpret_cast<CDevTreeQryRsp*>(pcMsgTrans->GetMsgObjAddr());
			if (pcRsp->GetErrorCode() != CMS_SUCCESS)
			{
				Log(ERROR_LEV, "CDevtreeAddTask::OnQrySrcTree recv CDevTreeQryRsp with errcode[%d] \n", pcRsp->GetErrorCode());
				return PROCMSG_DEL;
			}

			if (pcRsp->GetTotalEntryNum() <=0 )
			{
				Log(ERROR_LEV, "CDevtreeAddTask::OnQrySrcTree recv CDevTreeQryRsp with totalnum[%d] \n", pcRsp->GetTotalEntryNum());
				return PROCMSG_DEL;
			}

			return PROCMSG_OK;
		}
		else if(TAS_DEVTREE_QRY_NTF_REQ == pcMsgTrans->GetEventId())
		{
			CDevTreeQryNtfReq *pcNtfReq = reinterpret_cast<CDevTreeQryNtfReq*>(pcMsgTrans->GetMsgObjAddr());
			for (vector<CDevTreeInfo> ::iterator it = pcNtfReq->GetInfos().begin(); 
				it != pcNtfReq->GetInfos().end(); ++it)
			{
				if (it->GetDevtreeId() == m_cReq.GetSrcTreeId())
				{
					if (it->GetCanBeUsed4Gb())
					{
						m_bNormalOver  = true;
						return PROCMSG_DEL;
					}
					else
					{
						//非国标选择推送树（系统默认设备树除外），不支持拷贝为国标选择推送树
						SendRsp(ERR_TAS_INVALID_OPERATION);
						return PROCMSG_DEL;
					}
				}
			}
		}
	}
	else
	{
		Log(ERROR_LEV, "CDevtreeAddTask::OnQrySrcTree recv unknown msg[%d-%s] \n", pcMsg->event, OspExtEventDesc(pcMsg->event).c_str());
		return PROCMSG_FAIL;
	}
	
	return PROCMSG_FAIL;
}

u32 CDevtreeAddTask::OnQrySrcTreeTimer()
{
	if (GetCurStateHoldTime()>5)
	{
		SendRsp(ERR_TAS_RELYMODULE_DEALTIMEOUT_NOSTART);
		Log(ERROR_LEV, "CDevtreeAddTask[%d]::OnQrySrcTreeTimer wait GBS_DEV_GRP_GBID_BATCH_GET_RSP timeout !\n",GetTaskNO());
		return TIMERPOLL_DEL;
	}
	return TIMERPOLL_DONE;
}

u32 CDevtreeAddTask::OnSetDevGrpGbid(CMessage *const pcMsg)
{
	if (msgTaskMsgTransfer4Db == pcMsg->event)
	{
		ptrint ptr = *reinterpret_cast<ptrint*>(pcMsg->content);
		const CTaskMsgTransferBase* pcMsgTrans = reinterpret_cast<const CTaskMsgTransferBase*>(ptr);

		if (TAS_DEVICE_GROUP_QRY_RSP == pcMsgTrans->GetEventId())
		{
			NextState(enSetDevGrpGbid);
			CDeviceGroupQryRsp *pcRsp = reinterpret_cast<CDeviceGroupQryRsp*>(pcMsgTrans->GetMsgObjAddr());
			if (pcRsp->GetErrorCode() != CMS_SUCCESS)
			{
				SendRsp(pcRsp->GetErrorCode());
				return PROCMSG_DEL;
			}
			else if (pcRsp->GetTotalEntryNum() <= 0)
			{
				Log(ERROR_LEV, "CDevtreeAddTask::OnSetDevGrpGbid find no grp in srctree \n");
				return PROCMSG_DEL;
			}
		}
		else if (TAS_DEVICE_GROUP_QRY_NTF_REQ == pcMsgTrans->GetEventId())
		{
			NextState(enSetDevGrpGbid);
			u32 dwfullpathlen = 0;
			CDeviceGroupQryNtfReq *pcNtf = reinterpret_cast<CDeviceGroupQryNtfReq*>(pcMsgTrans->GetMsgObjAddr());
			for (vector<CDeviceGroupInfo> ::iterator it = pcNtf->GetDeviceGroupInfo().begin(); 
				it != pcNtf->GetDeviceGroupInfo().end(); ++it)
			{
				if (m_bStartQryNewTree)
				{
					m_vecNewGrpInfo.push_back(*it);
					dwfullpathlen=it->GetFullpath().size();
					if ( dwfullpathlen > m_dwMaxFullPathLen)
					{
						m_dwMaxFullPathLen = dwfullpathlen;
					}
				}
				else
				{
					m_vecSrcGrpInfo.push_back(*it);
				}

			}
			if (m_bStartQryNewTree)
			{
				if (pcNtf->IsLastNtf())
				{
					if (m_vecSrcGrpInfo.size() != m_vecNewGrpInfo.size())
					{
						Log(ERROR_LEV, "CDevtreeAddTask::OnQrySrcTree copy fail, 2 trees has diff num grps \n");
						SendRsp(ERR_TAS_DBERROR);
						return PROCMSG_DEL;
					}

					SetMapRelation4Grps();
					//根据srcgrpId去gbs中查询相应的分组civilcode
					if (m_dwGrpGbidSetIndex < m_vecSrcGrpInfo.size())
					{
						QryGrpGbid();
						m_bWaitGbsRsp = true;
					}
					else
					{
						Log(ERROR_LEV, "CDevtreeAddTask::OnQrySrcTree copy fail,srctreegrp num[%d] \n",
							m_vecSrcGrpInfo.size());
						return PROCMSG_DEL;
					}
				}
			}
			else
			{
				if (pcNtf->IsLastNtf())
				{
					m_bStartQryNewTree = true;
					NextState(enSetDevGrpGbid);
					CTaskMsgTransfer<CDeviceGroupQryReq> cTrans(GetTaskNO(), 0, TAS_DEVICE_GROUP_QRY_REQ);
					cTrans.GetMsgObj().GetDeviceGroupInfoQueryCondition().SetDevTreeId(m_cRsp.GetDevtreeId());
					if (!PostTaskMsgTransfer2DbTaskPool(cTrans))
					{
						SendRsp(ERR_TAS_SYSTEM_BUSY);
						Log(ERROR_LEV, "CDevtreeAddTask[%d]::OnIdle 1-PostTaskMsgTransfer2DbTaskPool fail \n",GetTaskNO());
						return PROCMSG_DEL;
					}
				}
			}

			return PROCMSG_OK;
		}
	}
	else if (GBS_DEV_GRP_GBID_BATCH_GET_RSP == pcMsg->event)
	{
		NextState(enSetDevGrpGbid);
		m_bWaitGbsRsp = false;
		COspSipMsg* pcSipMsg = reinterpret_cast<COspSipMsg*>(pcMsg->content);
		CGbsDevGrpGbidBatchGetRsp cRsp;
		pcSipMsg->GetMsgBody(cRsp);

		if (cRsp.GetErrorCode() != CMS_SUCCESS)
		{
			Log(ERROR_LEV, "CDevtreeAddTask::OnSetDevGrpGbid recv GBS_DEV_GRP_GBID_BATCH_GET_RSP with errCode[%d] \n",
				cRsp.GetErrorCode());
			return PROCMSG_DEL;
		}

		if (cRsp.GetVecBatchDevGrpInfo().empty())
		{
			Log(ERROR_LEV, "CDevtreeAddTask::OnSetDevGrpGbid recv GBS_DEV_GRP_GBID_BATCH_GET_RSP with empty grpInfo \n");
			return PROCMSG_DEL;
		}

		if (m_vecNewGrpInfo[m_dwGrpGbidSetIndex].GetDeviceGroupID() == m_cRsp.GetDevtreeId()) //根分组进行特别判定
		{
			if (cRsp.GetVecBatchDevGrpInfo()[0].gbid.size()/2 > GetMaxCivilcodeLev())
			{
				SendRsp(ERR_TAS_SRCTREE_HIERARCHY_TOO_DEEP);
				return PROCMSG_DEL;
			}
		}

		map<string, string> ::iterator it = m_mapOldGrpId2NewGrpId.find(cRsp.GetVecBatchDevGrpInfo()[0].groupkdid);
		if (it != m_mapOldGrpId2NewGrpId.end())
		{
			CDbMsgTransfer<TSetDevGrpGbidReq> cTrans(MAKEIID(GetAppID(), GetInsID()), GetTaskNO(), msgSetDevGrpGbidReq);
			cTrans.GetMsgObj().devTreeId = m_cRsp.GetDevtreeId();
			cTrans.GetMsgObj().devGrpGbid = cRsp.GetVecBatchDevGrpInfo()[0].gbid;
			cTrans.GetMsgObj().devGrpId = it->second;
			if(!PostDbMsgTransfer2DbTaskPool(cTrans))
			{
				Log(ERROR_LEV, "CDevtreeAddTask[%d]::OnSetDevGrpGbid PostTaskMsgTransfer2DbTaskPool-3 fail \n",GetTaskNO());
				return PROCMSG_DEL;
			}
		}
		else
		{
			Log(ERROR_LEV, "CDevtreeAddTask[%d]::OnSetDevGrpGbid can't find map relation for devgrp \n",GetTaskNO());
			return PROCMSG_DEL;
		}
	}
	else if (msgDbMsgTransfer == pcMsg->event)
	{
		NextState(enSetDevGrpGbid);
		CDbMsgTransferBase *pcBase = reinterpret_cast<CDbMsgTransferBase*>(*reinterpret_cast<ptrint*>(pcMsg->content));
		if (pcBase->GetEventId() == msgSetDevGrpGbidRsp)
		{
			CDbMsgTransfer<TSetDevGrpGbidRsp> *pcRsp = static_cast<CDbMsgTransfer<TSetDevGrpGbidRsp>* >(pcBase);
			if (pcRsp->GetMsgObj().GetErrno() != CMS_SUCCESS)
			{
				Log(ERROR_LEV, "CDevtreeAddTask[%d]::OnSetDevGrpGbid set devgrp fail,errCode[%d] \n",
					GetTaskNO(), pcRsp->GetMsgObj().GetErrno());
				g_cDbTaskPool.AddTransProgress(m_cRsp.GetOperUuid(), -1, (ECMS_ERRORCODE)pcRsp->GetMsgObj().GetErrno());
				return PROCMSG_DEL;
			}
			else
			{
				if (++m_dwGrpGbidSetIndex < m_vecSrcGrpInfo.size())
				{
					QryGrpGbid();
					m_bWaitGbsRsp = true;
				}
				else
				{
					//分组搞定了，该搞设备了
					g_cDbTaskPool.AddTransProgress(m_cRsp.GetOperUuid(), 30);
					//查出所有gbid字段为空的设备，之后去gbs查得相应的gbid，并且设置完毕。
					if (!QryEmptyGbidDevs())
					{
						return PROCMSG_DEL;
					}
				}

				return PROCMSG_OK;
			}
		}
		else
		{
			Log(ERROR_LEV, "CDevtreeAddTask::OnQrySrcTree recv unknown msg[%d-%s] \n", pcBase->GetEventId(), OspExtEventDesc(pcBase->GetEventId()).c_str());
			return PROCMSG_FAIL;
		}
	}
	return PROCMSG_OK;
}

u32 CDevtreeAddTask::OnSetDevGrpGbidTimer()
{
	if (GetCurStateHoldTime()>5 && m_bWaitGbsRsp)
	{
		SendRsp(ERR_TAS_RELYMODULE_DEALTIMEOUT_NOSTART);
		Log(ERROR_LEV, "CDevtreeAddTask[%d]::OnSetDevGrpGbidTimer wait GBS_DEV_GRP_GBID_BATCH_GET_RSP timeout !\n",GetTaskNO());
		return TIMERPOLL_DEL;
	}
	return TIMERPOLL_DONE;
}

u32 CDevtreeAddTask::OnCompleteDevGbid(CMessage *const pcMsg)
{
	switch(pcMsg->event)
	{
	case msgDbMsgTransfer:
		{
			CDbMsgTransferBase *pcBase = reinterpret_cast<CDbMsgTransferBase*>(*reinterpret_cast<ptrint*>(pcMsg->content));
			if (pcBase->GetEventId() == msgQryGbidEmptyDevsRsp)
			{
				CDbMsgTransfer<TQryGbidEmptyDevsRsp> *pcRsp = static_cast<CDbMsgTransfer<TQryGbidEmptyDevsRsp>* >(pcBase);
				if (pcRsp->GetMsgObj().GetErrno() != CMS_SUCCESS)
				{
					Log(ERROR_LEV, "CDevtreeAddTask[%d]::OnCompleteDevGbid qry emptygbid devs fail,errCode[%d] \n",
						GetTaskNO(), pcRsp->GetMsgObj().GetErrno());
					return PROCMSG_DEL;
				}

				if (pcRsp->GetMsgObj().totalNum < 0)
				{
					Log(ERROR_LEV, "CDevtreeAddTask[%d]::OnCompleteDevGbid qry emptygbid devs with totalnum[%d] \n",
						GetTaskNO(), pcRsp->GetMsgObj().totalNum);
					return PROCMSG_DEL;
				}

				if (pcRsp->GetMsgObj().totalNum == 0)
				{
					m_bNormalOver = true;
					return PROCMSG_DEL;
				}

				return PROCMSG_OK;
			}
			else if (pcBase->GetEventId() == msgQryGbidEmptyDevsNtf)
			{
				CDbMsgTransfer<TQryGbidEmptyDevsNtf> *pcNtf = static_cast<CDbMsgTransfer<TQryGbidEmptyDevsNtf>* >(pcBase);
				map<string, vector<TVidSrcGbInfo> > ::iterator it2;
				
				for (vector<TVidSrcGbInfo>::iterator it = pcNtf->GetMsgObj().vidSrcGbInfos.begin();
					it != pcNtf->GetMsgObj().vidSrcGbInfos.end(); ++it)
				{
					it2 = m_mapGrpId2DevGbInfos.find(it->devGrpId);
					if (it2 != m_mapGrpId2DevGbInfos.end())
					{
						it2->second.push_back(*it);
					}
					else
					{
						vector<TVidSrcGbInfo> vecTmp;
						vecTmp.push_back(*it);
						m_mapGrpId2DevGbInfos.insert(std::pair<string, vector<TVidSrcGbInfo> >(it->devGrpId, vecTmp));
					}
				}
				
				if (pcNtf->GetMsgObj().lastNtf)
				{
					if (m_mapGrpId2DevGbInfos.empty())
					{
						m_bNormalOver = true;
						return PROCMSG_DEL;
					}
					else
					{
						g_cDbTaskPool.AddTransProgress(m_cRsp.GetOperUuid(), 60);
						if(!QryDevGbids())
						{
							return PROCMSG_DEL;
						}
					}
				}
			}
			else if (pcBase->GetEventId() == msgSetDevGbidRsp)
			{
				CDbMsgTransfer<TSetDevGbidRsp> *pcRsp = static_cast<CDbMsgTransfer<TSetDevGbidRsp>* >(pcBase);
				if (pcRsp->GetMsgObj().GetErrno() != CMS_SUCCESS)
				{
					Log(ERROR_LEV, "CDevtreeAddTask::OnCompleteDevGbid recv msgSetDevGrpGbidRsp with errCode[%d] \n", pcRsp->GetMsgObj().GetErrno());
					return PROCMSG_DEL;
				}

				if (!QryDevGbids())
				{
					m_bNormalOver = true;
					return PROCMSG_DEL;
				}
				TTransProgress tProgress = g_cDbTaskPool.GetTransProgress(m_cRsp.GetDevtreeId());
				if (tProgress.finishNum + 1>=tProgress.totalNum)
				{
					g_cDbTaskPool.AddTransProgress(m_cRsp.GetOperUuid(), tProgress.finishNum);
				}
				else
				{
					g_cDbTaskPool.AddTransProgress(m_cRsp.GetOperUuid(), tProgress.finishNum+1);
				}
			}
			else
			{
				Log(ERROR_LEV, "CDevtreeAddTask:::OnCompleteDevGbid recv unknown msg[%d-%s] \n", pcBase->GetEventId(), OspExtEventDesc(pcBase->GetEventId()).c_str());
				return PROCMSG_FAIL;
			}
		}
		break;
	case GBS_DEV_GBID_BATCH_GET_RSP:
		{
			COspSipMsg* pcSipMsg = reinterpret_cast<COspSipMsg*>(pcMsg->content);
			CGbsDevGbidBatchGetRsp cRsp;
			pcSipMsg->GetMsgBody(cRsp);

			if (cRsp.GetErrorCode() != CMS_SUCCESS)
			{
				Log(ERROR_LEV, "CDevtreeAddTask::OnCompleteDevGbid recv GBS_DEV_GBID_BATCH_GET_RSP with errCode[%d] \n", cRsp.GetErrorCode());
				return PROCMSG_DEL;
			}
			
			if(!CompleteDevGbids(cRsp.GetVecBatchDevInfo()))
			{
				return PROCMSG_DEL;
			}
		}
		break;
	default:
		{
			Log(ERROR_LEV, "CDevtreeAddTask::OnCompleteDevGbid recv unknonwn msg[%d-%s] \n", pcMsg->event, OspExtEventDesc(pcMsg->event).c_str());
			return PROCMSG_FAIL;
		}
		break;
	}

	return PROCMSG_OK;
}

u32 CDevtreeAddTask::OnCompleteDevGbidTimer()
{
	return TIMERPOLL_DONE;
}

void CDevtreeAddTask::SendRsp( u32 dwErrorCode )
{
	if (dwErrorCode != CMS_SUCCESS)
	{
		g_cDbTaskPool.AddTransProgress(m_cRsp.GetOperUuid(),-1, (ECMS_ERRORCODE)dwErrorCode );
	}
	if (m_bHasRsped)
	{
		return;
	}
	m_bHasRsped = true;
	m_cRsp.SetErrorCode(dwErrorCode);
	PostMsgRsp(m_tTransId, m_cRsp);
}

void CDevtreeAddTask::QryGrpGbid(bool bRootGrp)
{
	CGbsDevGrpGbidBatchGetReq cQryReq;
	cQryReq.SetSeqNum(GetTaskNO());
	if (bRootGrp)
	{
		cQryReq.GetVecGroupkdid().push_back(GetDomainUUID());
	}
	else
	{
		cQryReq.GetVecGroupkdid().push_back(m_vecSrcGrpInfo[m_dwGrpGbidSetIndex].GetDeviceGroupID());
	}
	TSipURI tUri;
	tUri.SetDomain(GetDomainName());
	PostReq(KDSIP_EVENT_MESSAGE_REQ, cQryReq, tUri.GetGbsURI());
}

void CDevtreeAddTask::QrySrcTreeInfo()
{
	CTaskMsgTransfer<CDevTreeQryReq> cTrans(GetTaskNO(), 0, TAS_DEVTREE_QRY_REQ);
	if(!PostTaskMsgTransfer2DbTaskPool(cTrans))
	{
		SendRsp(ERR_TAS_SYSTEM_BUSY);
		Log(ERROR_LEV, "CDevtreeAddTask[%d]::OnIdle PostTaskMsgTransfer2DbTaskPool fail \n",GetTaskNO());
	}
}

bool CDevtreeAddTask::QryEmptyGbidDevs()
{
	CDbMsgTransfer<TQryGbidEmptyDevsReq> cTrans(MAKEIID(GetAppID(), GetInsID()), GetTaskNO(), msgQryGbidEmptyDevsReq);
	cTrans.GetMsgObj().devTreeId = m_cRsp.GetDevtreeId();
	if(!PostDbMsgTransfer2DbTaskPool(cTrans))
	{
		Log(ERROR_LEV, "CDevtreeAddTask[%d]::OnQrySrcTree PostTaskMsgTransfer2DbTaskPool-2 fail \n",GetTaskNO());
		return false;
	}

	NextState(enCompleteDevGbid);
	return true;
}

bool CDevtreeAddTask::QryDevGbids()
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

bool CDevtreeAddTask::CompleteDevGbids( vector<TGbDevGbidBatchGetInfo>& vecBatchDevInfo )
{
	//根据根分组的gbid,匹配所有子分组的gbid。
	CDbMsgTransfer<TSetDevGbidReq> cTrans(MAKEIID(GetAppID(), GetInsID()), GetTaskNO(), msgSetDevGbidReq);
	cTrans.GetMsgObj().devTreeId = m_cRsp.GetDevtreeId();
	cTrans.GetMsgObj().devGrpId = m_strCompletingGrpId; //需设置好grpId;
	TDevGbId tGbId;
	tGbId.operType = 0;
	for(vector<TGbDevGbidBatchGetInfo> ::iterator it = vecBatchDevInfo.begin(); it != vecBatchDevInfo.end(); ++it)
	{
		tGbId.devId = it->GetKdid();
		tGbId.vidSrcIdx = it->GetVidindex();
		tGbId.priGbid = it->GetPrimary();

		cTrans.GetMsgObj().DevGbids.push_back(tGbId);
	}
	if (cTrans.GetMsgObj().DevGbids.empty())
	{
		return false;
	}
	if(!PostDbMsgTransfer2DbTaskPool(cTrans))
	{
		Log(ERROR_LEV, "CDevtreeAddTask[%d]::OnQrySrcTree PostTaskMsgTransfer2DbTaskPool-2 fail \n",GetTaskNO());
		return false;
	}

	return true;
}

void CDevtreeAddTask::SetMapRelation4Grps()
{
	//先把根分组信息写入
	if (m_cReq.GetSrcTreeId().empty())
	{
		m_mapOldGrpId2NewGrpId.insert(std::pair<string, string>(GetDomainUUID(), m_cRsp.GetDevtreeId()));
	}
	else
	{
		m_mapOldGrpId2NewGrpId.insert(std::pair<string, string>(m_cReq.GetSrcTreeId(), m_cRsp.GetDevtreeId()));
	}
	map<string, string> ::iterator it;
	u32 dwTotalNum = m_vecSrcGrpInfo.size();
	for (int i = 0; i<dwTotalNum; ++i) //外层循环为srcgrpId循环
	{
		if (m_vecSrcGrpInfo[i].GetDeviceGroupID() == GetDomainUUID()
			|| m_vecSrcGrpInfo[i].GetDeviceGroupID() == m_cReq.GetSrcTreeId())
		{
			continue;
		}
		for (int j = 0; j<dwTotalNum; ++j) //内存循环为dstgrpId循环
		{
			if (m_vecSrcGrpInfo[i].GetFullpath().size() == m_vecNewGrpInfo[j].GetFullpath().size()
				&& FLOAT_EQU(m_vecSrcGrpInfo[i].GetGrpseq(), m_vecNewGrpInfo[j].GetGrpseq())
				&& m_vecSrcGrpInfo[i].GetDeviceGroupName().find(m_vecNewGrpInfo[j].GetDeviceGroupName()) != string::npos)
			{
				it = m_mapOldGrpId2NewGrpId.find(m_vecSrcGrpInfo[i].GetDeviceParentGroupID());
				if (it != m_mapOldGrpId2NewGrpId.end()
					&& it->second != m_vecNewGrpInfo[j].GetDeviceParentGroupID())
				{
					continue;
				}
				m_mapOldGrpId2NewGrpId.insert(std::pair<string, string>(m_vecSrcGrpInfo[i].GetDeviceGroupID(), m_vecNewGrpInfo[j].GetDeviceGroupID()));
				Log(EVENT_LEV, "srcgrpId[%s]---newgrid[%s] \n", m_vecSrcGrpInfo[i].GetDeviceGroupID().c_str(), m_vecNewGrpInfo[j].GetDeviceGroupID().c_str());
				break;
			}
		}
	}
}


