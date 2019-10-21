#include "devgrp_dev_add_task.h"
#include "cctrl.h"
#include "gbs/gbs_proto.h"
#include "commonconfig/cmcfg.h"
/*
思路：分为2中情况：
情况一：不用于国标选择推送，直接抛消息到数据库操作线程，处理完毕后直接发送Rsp+Ntf即可结束Task。
情况二：用于国标选择推送，收到请求后先保存---去gbs查询gbid---抛到数据库处理线程---缓存rsp+ntf--之后设置gbid到数据库--
        把gbid设置到
*/
CDevgrpDevAddTask::CDevgrpDevAddTask( CInstExt *pcInst ): CSipTask(pcInst)
{
	m_tTransId = INVALID_TRANS_ID;
}

CDevgrpDevAddTask::~CDevgrpDevAddTask()
{

}

void CDevgrpDevAddTask::PrintData() const
{
	CSipTask::PrintData();
	OspPrintf(1, 0, "CDevgrpDevAddTask[%d] m_cReq[%s]\n",GetTaskNO(), m_cReq.ToXml().c_str());
}

void CDevgrpDevAddTask::InitStateMachine()
{
	CStateProc cIdleProc;
	cIdleProc.ProcMsg = (CTask::PFProcMsg)&CDevgrpDevAddTask::OnIdle;
	cIdleProc.ProcErrMsg = (CTask::PFProcMsg)&CDevgrpDevAddTask::OnIdle;
	cIdleProc.TimerPoll = (CTask::PFTimerPoll)&CDevgrpDevAddTask::OnIdleTimer;
	AddRuleProc(enIdle, cIdleProc);

	CStateProc cQryProc;
	cQryProc.ProcMsg = (CTask::PFProcMsg)&CDevgrpDevAddTask::OnQryTas;
	cQryProc.ProcErrMsg = (CTask::PFProcMsg)&CDevgrpDevAddTask::OnQryTas;
	cQryProc.TimerPoll = (CTask::PFTimerPoll)&CDevgrpDevAddTask::OnQryTasTimer;
	AddRuleProc(enQryTas, cQryProc);

	CStateProc cQryGbsProc;
	cQryGbsProc.ProcMsg = (CTask::PFProcMsg)&CDevgrpDevAddTask::OnQryGbs;
	cQryGbsProc.ProcErrMsg = (CTask::PFProcMsg)&CDevgrpDevAddTask::OnQryGbs;
	cQryGbsProc.TimerPoll = (CTask::PFTimerPoll)&CDevgrpDevAddTask::OnQryGbsTimer;
	AddRuleProc(enQryGbs, cQryGbsProc);

	CStateProc cAddProc;
	cAddProc.ProcMsg = (CTask::PFProcMsg)&CDevgrpDevAddTask::OnAdd;
	cAddProc.ProcErrMsg = (CTask::PFProcMsg)&CDevgrpDevAddTask::OnAdd;
	cAddProc.TimerPoll = (CTask::PFTimerPoll)&CDevgrpDevAddTask::OnAddTimer;
	AddRuleProc(enAdd, cAddProc);

	CStateProc cSetProc;
	cSetProc.ProcMsg = (CTask::PFProcMsg)&CDevgrpDevAddTask::OnSet;
	cSetProc.ProcErrMsg = (CTask::PFProcMsg)&CDevgrpDevAddTask::OnSet;
	cSetProc.TimerPoll = (CTask::PFTimerPoll)&CDevgrpDevAddTask::OnSetTimer;
	AddRuleProc(enSet, cSetProc);

	NextState(enIdle);
}

u32 CDevgrpDevAddTask::OnIdle( CMessage *const pcMsg )
{
	//分组内增加设备：若分组隶属于系统设备树，则无需查找，否则需先查询视频源主流的gbid。

	if (pcMsg->event != TAS_DEVICE_GROUP_DEVICE_ADD_REQ)
	{
		Log(ERROR_LEV, "CDevgrpDevAddTask[%d]::OnIdle recv unknown msg[%d-%s] \n",
			GetTaskNO(), pcMsg->event, OspExtEventDesc(pcMsg->event).c_str());
		return PROCMSG_DEL;
	}

	COspSipMsg* const pcSipMsg = reinterpret_cast<COspSipMsg* const>(pcMsg->content);
	pcSipMsg->GetMsgBody(m_cReq);

	m_cRsp.SetHeadFromReq(m_cReq);
	m_tTransId = pcSipMsg->GetSipTransID();
	
	//直接丢到数据库操作线程
	CTaskMsgTransfer<CDeviceGroupDeviceAddReq> cTrans(GetTaskNO(), pcSipMsg->GetSipTransID(), pcMsg->event);
	pcSipMsg->GetMsgBody(cTrans.GetMsgObj());
	if(!PostTaskMsgTransfer2DbTaskPool(cTrans))
	{
		SendRsp(ERR_TAS_SYSTEM_BUSY);
		Log(ERROR_LEV, "CDevgrpDevAddTask[%d]::OnIdle PostTaskMsgTransfer2DbTaskPool fail \n",GetTaskNO());
		return PROCMSG_DEL;
	}

	NextState(enAdd);
	return PROCMSG_OK;
}

u32 CDevgrpDevAddTask::OnIdleTimer()
{
	return TIMERPOLL_DONE;
}

u32 CDevgrpDevAddTask::OnAdd( CMessage *const pcMsg )
{
	if (msgTaskMsgTransfer4Db == pcMsg->event)
	{
		ptrint ptr = *reinterpret_cast<ptrint*>(pcMsg->content);
		const CTaskMsgTransferBase* pcMsgTrans = reinterpret_cast<const CTaskMsgTransferBase*>(ptr);
		if (TAS_DEVICE_GROUP_DEVICE_ADD_RSP == pcMsgTrans->GetEventId())
		{
			CDeviceGroupDeviceAddRsp *pcRsp = reinterpret_cast<CDeviceGroupDeviceAddRsp*>(pcMsgTrans->GetMsgObjAddr());

			SendRsp(pcRsp->GetErrorCode());

			if (pcRsp->GetErrorCode() != CMS_SUCCESS)
			{
				Log(ERROR_LEV, "CDevgrpDevAddTask[%d] recv TAS_DEVICE_GROUP_DEVICE_ADD_RSP with errCode[%d] \n",
					GetTaskNO(), pcRsp->GetErrorCode());
				return PROCMSG_DEL;
			}
			else
			{
				Log(EVENT_LEV, "CDevgrpDevAddTask[%d] recv TAS_DEVICE_GROUP_DEVICE_ADD_RSP with errCode[0] \n",
					GetTaskNO(), pcRsp->GetErrorCode());

				if (!m_cReq.GetDeviceGroupDeviceInfo().GetGrpDevTreeId().empty())
				{
					NextState(enQryTas);
					CTaskMsgTransfer<CDevTreeQryReq> cTrans(GetTaskNO(), m_tTransId, TAS_DEVTREE_QRY_REQ);
					//pcSipMsg->GetMsgBody(cTrans.GetMsgObj());
					if(!PostTaskMsgTransfer2DbTaskPool(cTrans))
					{
						Log(ERROR_LEV, "CDevgrpDevAddTask[%d]::OnAdd PostTaskMsgTransfer2DbTaskPool fail \n",GetTaskNO());
						return PROCMSG_DEL;
					}
					return PROCMSG_OK;
				}
				else
				{
					return PROCMSG_OK;
				}
			}
		}
		else if (TAS_DEVICE_GROUP_DEVICE_ADD_NTF_REQ== pcMsgTrans->GetEventId())
		{
			CDeviceGroupDeviceAddNtfReq *pcNtf = reinterpret_cast<CDeviceGroupDeviceAddNtfReq*>(pcMsgTrans->GetMsgObjAddr());

			((CCtrlInst *)GetInstance())->GetAppData().HandleChangeNtfReq(TAS_DEVICE_GROUP_DEVICE_ADD_NTF_REQ, pcNtf->ToXml());

			CMS_ASSERT(m_cReq.GetDeviceGroupDeviceInfo().GetGrpDevTreeId().empty());
			return PROCMSG_DEL;
			
		}
	}

	return PROCMSG_OK;
}

u32 CDevgrpDevAddTask::OnAddTimer()
{
	return TIMERPOLL_DONE;
}

u32 CDevgrpDevAddTask::OnQryTas( CMessage *const pcMsg )
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
					Log(ERROR_LEV, "CDevgrpDevAddTask::OnQryTas Recv TAS_DEVTREE_QRY_RSP with errCode[%d] \n", pcRsp->GetErrorCode());
					return PROCMSG_DEL;
				}

				if (pcRsp->GetTotalEntryNum() <= 0)
				{
					Log(ERROR_LEV, "CDevgrpDevAddTask::OnQryTas Recv TAS_DEVTREE_QRY_RSP with totalNum[%d] \n", pcRsp->GetTotalEntryNum());
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
						if (it->GetDevtreeId() == m_cReq.GetDeviceGroupDeviceInfo().GetGrpDevTreeId())
						{
							break;
						}
					}
					if (it == m_vecDevTrees.end())
					{
						Log(ERROR_LEV, "CDevgrpDevAddTask::OnQryTas qry no devtree[%s] \n", m_cReq.GetDeviceGroupDeviceInfo().GetGrpDevTreeId().c_str());
						return PROCMSG_DEL;
					}

					if (!it->GetCanBeUsed4Gb())
					{
						return PROCMSG_DEL;
					}
					else
					{
						CTaskMsgTransfer<CDeviceQryReq> cTrans(GetTaskNO(), m_tTransId, TAS_DEVICE_QRY_REQ);
						cTrans.GetMsgObj().GetDeviceInfoQueryCondition().SetDeviceUUID(m_cReq.GetDeviceGroupDeviceInfo().GetDeviceID());
						if(!PostTaskMsgTransfer2DbTaskPool(cTrans))
						{
							Log(ERROR_LEV, "CDevgrpDevAddTask[%d]::OnAdd PostTaskMsgTransfer2DbTaskPool-1 fail \n",GetTaskNO());
							return PROCMSG_DEL;
						}
					}
				}
			}
			break;
		case TAS_DEVICE_QRY_RSP:
			{
				CDeviceQryRsp *  pcRsp = reinterpret_cast<CDeviceQryRsp*>(pcMsgTrans->GetMsgObjAddr());
				if (pcRsp->GetErrorCode() != CMS_SUCCESS)
				{
					Log(ERROR_LEV, "CDevgrpDevAddTask::OnQryTas recv msgQryDevGbidRsp with errCode[%d]!!! \n", pcRsp->GetErrorCode());
					return PROCMSG_DEL;
				}
				if (pcRsp->GetTotalEntryNum() <= 0)
				{
					Log(ERROR_LEV, "CDevgrpDevAddTask::OnQryTas dev[%s] is not exist !!! \n", m_cReq.GetDeviceGroupDeviceInfo().GetDeviceID().c_str());
					return PROCMSG_DEL;
				}
			}
			break;
		case TAS_DEVICE_QRY_NTF_REQ:
			{
				CDeviceQryNtfReq*  pcNtf = reinterpret_cast<CDeviceQryNtfReq*>(pcMsgTrans->GetMsgObjAddr());
				if (pcNtf->GetDeviceInfo().size() != 1)
				{
					return PROCMSG_DEL;
				}
				string strDevGbid = GetDevGbid(pcNtf->GetDeviceInfo()[0]);
				if (!strDevGbid.empty())
				{
					if (strDevGbid.substr(11,3)== "132"
						|| strDevGbid.substr(11,3)== "131")
					{
						NextState(enSet);

						CDbMsgTransfer<TSetDevGbidReq> cTrans(MAKEIID(GetAppID(), GetInsID()), GetTaskNO(), msgSetDevGbidReq);
						cTrans.GetMsgObj().devTreeId = m_cReq.GetDeviceGroupDeviceInfo().GetGrpDevTreeId();
						cTrans.GetMsgObj().devGrpId = m_cReq.GetDeviceGroupDeviceInfo().GetDeviceGroupID();
						TDevGbId tDevGbid;
						tDevGbid.devId = m_cReq.GetDeviceGroupDeviceInfo().GetDeviceID();
						tDevGbid.vidSrcIdx = 0;
						tDevGbid.operType = 0;
						tDevGbid.priGbid = strDevGbid;
						cTrans.GetMsgObj().DevGbids.push_back(tDevGbid);
						if(!PostDbMsgTransfer2DbTaskPool(cTrans))
						{
							Log(ERROR_LEV, "CDevgrpDevAddTask[%d]::OnAdd PostTaskMsgTransfer2DbTaskPool-2 fail \n",GetTaskNO());
							return PROCMSG_DEL;
						}

						return PROCMSG_OK;
					}
				}

				NextState(enQryGbs);
				if(!QryGbs())
				{
					Log(ERROR_LEV, "CDevgrpDevAddTask QryGbs fail,dev[%s] \n", m_cReq.GetDeviceGroupDeviceInfo().GetDeviceID().c_str());
					return PROCMSG_DEL;
				}
			}
			break;
		default:
			{
				Log(ERROR_LEV, "CDevgrpDevAddTask::OnQryTas recv unknown msg[%d-%s] \n", pcMsgTrans->GetEventId(), OspExtEventDesc(pcMsgTrans->GetEventId()).c_str());
				return PROCMSG_FAIL;
			}
		};
	}
	else
	{
		Log(ERROR_LEV, "CDevgrpDevAddTask::OnQryTas recv unknown msg[%d-%s] \n", pcMsg->event, OspExtEventDesc(pcMsg->event).c_str());
		return PROCMSG_FAIL;
	}

	return PROCMSG_OK;
}

u32 CDevgrpDevAddTask::OnQryTasTimer()
{
	return TIMERPOLL_DONE;
}


u32 CDevgrpDevAddTask::OnQryGbs( CMessage *const pcMsg )
{
	if (pcMsg->event != GBS_DEV_GBID_BATCH_GET_RSP)
	{
		Log(ERROR_LEV, "CDevgrpDevAddTask::OnQryGbs recv unknown msg[%d-%s] ! \n", pcMsg->event, OspExtEventDesc(pcMsg->event).c_str());
		return PROCMSG_FAIL;
	}
   
	COspSipMsg* pcOspMsg = reinterpret_cast<COspSipMsg*>(pcMsg->content);
	CGbsDevGbidBatchGetRsp cRsp;
	pcOspMsg->GetMsgBody(cRsp);

	if(CMS_SUCCESS != cRsp.GetErrorCode())
	{
		Log(ERROR_LEV, "CDevgrpDevAddTask::OnQryGbs recv GBS_DEV_GBID_BATCH_GET_RSP with errCode[%d],dev[%s] !!! \n", 
			cRsp.GetErrorCode(), m_cReq.GetDeviceGroupDeviceInfo().GetDeviceID().c_str());
		return PROCMSG_DEL;
	}


	NextState(enSet);
	map<bool,string> ::iterator itPri;
	CDbMsgTransfer<TSetDevGbidReq> cTrans(MAKEIID(GetAppID(), GetInsID()), GetTaskNO(), msgSetDevGbidReq);
	cTrans.GetMsgObj().devTreeId = m_cReq.GetDeviceGroupDeviceInfo().GetGrpDevTreeId();
	cTrans.GetMsgObj().devGrpId = m_cReq.GetDeviceGroupDeviceInfo().GetDeviceGroupID();
	TDevGbId tDevGbid;
	tDevGbid.devId = m_cReq.GetDeviceGroupDeviceInfo().GetDeviceID();
	for (vector<TGbDevGbidBatchGetInfo> ::const_iterator it = cRsp.GetVecBatchDevInfo().begin(); 
		it != cRsp.GetVecBatchDevInfo().end(); ++it)
	{
		tDevGbid.operType = 0;
		tDevGbid.vidSrcIdx = it->GetVidindex();
		tDevGbid.priGbid = it->GetPrimary();
		
		cTrans.GetMsgObj().DevGbids.push_back(tDevGbid);
	}
	if (cTrans.GetMsgObj().DevGbids.empty())
	{
		Log(ERROR_LEV, "CDevgrpDevAddTask::OnQryGbs qry gbids fail !!!");
		return PROCMSG_DEL;
	}
	
	if(!PostDbMsgTransfer2DbTaskPool(cTrans))
	{
		Log(ERROR_LEV, "CDevgrpDevAddTask[%d]::OnAdd PostTaskMsgTransfer2DbTaskPool-2 fail \n",GetTaskNO());
		return PROCMSG_DEL;
	}


	return PROCMSG_OK;
}

u32 CDevgrpDevAddTask::OnQryGbsTimer()
{
	return TIMERPOLL_DONE;
}

u32 CDevgrpDevAddTask::OnSet( CMessage *const pcMsg )
{
	if (msgDbMsgTransfer == pcMsg->event)
	{
		ptrint ptr = *reinterpret_cast<ptrint*>(pcMsg->content);
		const CDbMsgTransferBase* pcMsgTrans = reinterpret_cast<const CDbMsgTransferBase*>(ptr);
		if (msgSetDevGbidRsp == pcMsgTrans->GetEventId())
		{
			TSetDevGbidRsp * pRsp = reinterpret_cast<TSetDevGbidRsp*>(pcMsgTrans->GetMsgObjAddr());
			if (pRsp->GetErrno() != CMS_SUCCESS)
			{
				Log(ERROR_LEV, "CDevgrpDevAddTask set trees[%s]'s dev[%s]'s gbid fail!!! \n",
					m_cReq.GetDeviceGroupDeviceInfo().GetGrpDevTreeId().c_str(), m_cReq.GetDeviceGroupDeviceInfo().GetDeviceID().c_str());
			}
			return PROCMSG_DEL;
		}
		else
		{
			Log(ERROR_LEV, "recv unknown msg[%d-%s] \n", pcMsgTrans->GetEventId(), OspExtEventDesc(pcMsgTrans->GetEventId()).c_str());
			return PROCMSG_FAIL;
		}
	}
	
	Log(ERROR_LEV, "recv unknown msg[%d-%s] \n", pcMsg->event, OspExtEventDesc(pcMsg->event).c_str());
	return PROCMSG_FAIL;
}

u32 CDevgrpDevAddTask::OnSetTimer()
{
	return TIMERPOLL_DONE;
}

void CDevgrpDevAddTask::SendRsp( u32 dwErrorCode /*= CMS_SUCCESS*/ )
{
	m_cRsp.SetErrorCode(dwErrorCode);
	PostMsgRsp(m_tTransId, m_cRsp);
}

bool CDevgrpDevAddTask::QryGbs()
{
	//从gbs查询指定设备指定视频源（对应通道）的gbid
	CGbsDevGbidBatchGetReq cGbReq;

	TGbDevGbidBatchGetCondition tCond;
	tCond.SetKdid(m_cReq.GetDeviceGroupDeviceInfo().GetDeviceID());
	for (vector<TDeviceCapIndexs> ::iterator it = m_cReq.GetDeviceGroupDeviceInfo().GetDeviceCapIndexs().begin();
		it != m_cReq.GetDeviceGroupDeviceInfo().GetDeviceCapIndexs().end(); ++it)
	{
		if (it->id == DEVICE_CAP_VIDEOSOURCE)
		{
			tCond.SetVidindexs(it->indexs);
		}
	}
	if (tCond.GetVidindexs().empty())
	{
		return false;
	}
	for (int i = 0; i<tCond.GetVidindexs().size(); ++i)
	{
		tCond.GetPrisec().push_back(0);
	}
	cGbReq.GetVecConditions().push_back(tCond);
	TSipURI tDstUri = tDstUri.GetGbsURI();
	tDstUri.SetDomain(GetDomainName());
	if(PROCMSG_OK != PostMsgReq(cGbReq, tDstUri))
	{
		return false;
	}

	return true;
}

string CDevgrpDevAddTask::GetDevGbid( const CDeviceInfo& devInfo )
{
	map<TDevExtDataType, string> ::const_iterator it = devInfo.GetDevData().GetExtData().find(DEV_EXT_GBID);
	if (it == devInfo.GetDevData().GetExtData().end())
	{
		return string();
	}
	return it->second;
}


