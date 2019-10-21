#include "devgrp_mod .h"
#include "common/ssn_taskbase.h"
#include "cms/commonconfig/cmcfg.h"
#include "db_task_pool.h"
/************************************************************************
设备分组修改 wanghui
主要包括分组gbid修改，和对分组进行设备的主辅流切换操作
************************************************************************/
extern CDbTaskPool g_cDbTaskPool;
CDevGrpModTask::CDevGrpModTask( CSsnTaskbase* pcSsnTask )
	:CSipTask(pcSsnTask->GetInstance())
{
	m_dwPriSec = -1;
	m_dwBatchIndex = 0;
	m_tGbsUri = m_tGbsUri.GetGbsURI();
	m_tGbsUri.SetDomain(GetDomainName());
	m_vecDevGrpDevInfos.clear();
	m_vecLastNeedModDevs.clear();
	m_vecBatchGetConditions.clear();
	m_vecBatchOperate.operType = OPERATE_TYPE_NULL;
	m_dwProgress = 0;
}

CDevGrpModTask::~CDevGrpModTask()
{
}

const char* CDevGrpModTask::GetStateName( u32 dwState ) const
{
	switch (dwState)
	{
	case enStateIdle:
		return "enStateIdle";
	case enStateQryTas:
		return "enStateQryTas";
	case enStateQryGbid:
		return "enStateQryGbid";
	case enStateBatchMod:
		return "enStateBatchMod";
	case enStateMod:
		return "enStateMod";
	default:
		return "NA";
	}
}

void CDevGrpModTask::InitStateMachine()
{
	CStateProc cProc;

	cProc.ProcMsg = (CTask::PFProcMsg)&CDevGrpModTask::OnIdle;
	cProc.ProcErrMsg = cProc.ProcMsg;
	cProc.TimerPoll = &CTask::TimerPollSkip;
	AddRuleProc(enStateIdle, cProc);

	cProc.ProcMsg = (CTask::PFProcMsg)&CDevGrpModTask::OnQryTas;
	cProc.ProcErrMsg = cProc.ProcMsg;
	cProc.TimerPoll = (CTask::PFTimerPoll)&CDevGrpModTask::OnQryTasTimePoll;
	AddRuleProc(enStateQryTas, cProc);

	cProc.ProcMsg = (CTask::PFProcMsg)&CDevGrpModTask::OnQryGbid;
	cProc.ProcErrMsg = cProc.ProcMsg;
	cProc.TimerPoll = (CTask::PFTimerPoll)&CDevGrpModTask::OnQryGbidTimePoll;
	AddRuleProc(enStateQryGbid, cProc);

	cProc.ProcMsg = (CTask::PFProcMsg)&CDevGrpModTask::OnBatchMod;
	cProc.ProcErrMsg = cProc.ProcMsg;
	cProc.TimerPoll = &CTask::TimerPollSkip;
	AddRuleProc(enStateBatchMod, cProc);

	cProc.ProcMsg = (CTask::PFProcMsg)&CDevGrpModTask::OnMod;
	cProc.ProcErrMsg = cProc.ProcMsg;
	cProc.TimerPoll = (CTask::PFTimerPoll)&CDevGrpModTask::OnModTimePoll;
	AddRuleProc(enStateMod, cProc);


	NextState(enStateIdle);

}

void CDevGrpModTask::PrintData() const
{
	CSipTask::PrintData();
	OspPrintf(1, 0, "CDevGrpModTask[%d]-batchConditionsSize[%d]-Index[%d]\n",GetTaskNO(),(u32)m_vecBatchGetConditions.size(),m_dwBatchIndex);
}

u32 CDevGrpModTask::OnIdle( CMessage *const pcMsg )
{
	COspSipMsg* const pcSipMsg = reinterpret_cast<COspSipMsg* const>(pcMsg->content);
	if (pcMsg->event == TAS_DEVICE_GROUP_MOD_4GB_REQ)
	{
		CDeviceGroupMod4GbReq cReq;
		pcSipMsg->GetMsgBody(cReq);
		m_TransId = pcSipMsg->GetSipTransID();
		m_strTreeId = cReq.GetDevTreeId();
		m_strGrpId = cReq.GetDevGrpId();
		m_strGrpGbid = cReq.GetDevGroupGbid();
		m_dwPriSec = cReq.GetPrisec();
	
		if (m_strTreeId.empty() || m_strGrpId.empty())
		{
			//错误，给错误rsp
			SendRsp(ERR_TAS_INVALID_PARM);
			return PROCMSG_DEL;
		}
		//1.修改gbid
		if (!m_strGrpGbid.empty())
		{
			NextState(enStateMod);
			CDbMsgTransfer<CDevGrpGbidOperateReq> cTrans(MAKEIID(GetAppID(), GetInsID()), GetTaskNO(), msgDevGrpGbidOperateReq);
			cTrans.GetMsgObj().devTreeId = m_strTreeId;
			cTrans.GetMsgObj().devGrpId = m_strGrpId;
			cTrans.GetMsgObj().devGrpGbid = m_strGrpGbid;
			if (!PostDbMsgTransfer2DbTaskPool(cTrans)) 
			{
				TASKLOG(ERROR_LEV,"CDevGrpModTask PostDbMsgTransfer2DbTaskPool fail!\n");
				SendRsp(ERR_POST_MSG_FAILED);
				return PROCMSG_DEL;
			}
			m_dwProgress = 5;SetProgress(m_dwProgress);
		}
		//2.选择主辅流
		else if ( m_dwPriSec != 0)
		{
			//查询该分组下的子分组和所有设备
			NextState(enStateQryTas);

			CDbMsgTransfer<CDevGrpAllChildDevQryReq> cTrans(MAKEIID(GetAppID(), GetInsID()), GetTaskNO(), msgDevGrpAllChildDevQryReq);
			cTrans.GetMsgObj().devTreeId = m_strTreeId;
			cTrans.GetMsgObj().devGrpId = m_strGrpId;
			if (!PostDbMsgTransfer2DbTaskPool(cTrans)) 
			{
				TASKLOG(ERROR_LEV,"CDevGrpModTask PostDbMsgTransfer2DbTaskPool fail!\n");
				SendRsp(ERR_POST_MSG_FAILED);
				return PROCMSG_DEL;
			}
			m_dwProgress = 5;SetProgress(m_dwProgress);
			SendRsp(CMS_SUCCESS);
		}
	}
	
	return PROCMSG_OK;
}

u32 CDevGrpModTask::OnQryTas(CMessage* const pcMsg)
{
	if (msgDbMsgTransfer != pcMsg->event)
		return PROCMSG_FAIL;

	ptrint ptr = *reinterpret_cast<ptrint*>(pcMsg->content);
	const CDbMsgTransferBase* pcMsgTrans = reinterpret_cast<const CDbMsgTransferBase*>(ptr);

	if ( msgDevGrpAllChildDevQryRsp == pcMsgTrans->GetEventId() )
	{
		const CDevGrpAllChildDevQryRsp *pcRsp = static_cast<const CDevGrpAllChildDevQryRsp*>(pcMsgTrans->GetMsgObjAddr());

		m_vecDevGrpDevInfos.insert(m_vecDevGrpDevInfos.end(),pcRsp->devInfos.begin(),pcRsp->devInfos.end());

		NextState(enStateQryGbid);
		m_dwProgress = 10;SetProgress(m_dwProgress);
		ConstructGbidGetReq();
		//没有需要向gbs获取gbid的
		if (!SendGbidBatchGetReq())
		{
			//可能存在需要直接修改tas的
			NextState(enStateMod);
			m_dwProgress = 25;SetProgress(m_dwProgress);
			if (!SendDevModReq())
			{
				m_dwProgress = 100;SetProgress(m_dwProgress);
				return PROCMSG_DEL;
			}
		}
	}
	return PROCMSG_OK;
}

u32 CDevGrpModTask::OnQryTasTimePoll()
{
	return TIMERPOLL_DONE;
}

u32 CDevGrpModTask::OnQryGbid(CMessage* const pcMsg)
{
	COspSipMsg* const pcSipMsg = reinterpret_cast<COspSipMsg* const>(pcMsg->content);
	switch (pcMsg->event)
	{
	case GBS_DEV_GBID_BATCH_GET_RSP:
		{
			CGbsDevGbidBatchGetRsp cRsp;
			pcSipMsg->GetMsgBody(cRsp);
			TOperateGbDevInfo tDevModGbid;
			
			for (vector<TGbDevGbidBatchGetInfo>::iterator it = cRsp.GetVecBatchDevInfo().begin();
				it != cRsp.GetVecBatchDevInfo().end();it++)
			{
				tDevModGbid.devGrpId = it->GetDevGrpId(); 
				tDevModGbid.devId = it->GetKdid();
				tDevModGbid.vidSrcIdx = it->GetVidindex();
				if (it->prisec == 0) 
				{
					tDevModGbid.operType = OPERATE_TYPE_ADD_PRIMARY;
					tDevModGbid.gbid = it->GetPrimary();
				}
				else if (it->prisec == 1) 
				{
					tDevModGbid.operType = OPERATE_TYPE_ADD_SECOND;
					tDevModGbid.gbid = it->GetSecond();
				}
				else if (it->prisec == 2)
				{
					tDevModGbid.operType = OPERATE_TYPE_ADD_PRIMARY;
					tDevModGbid.gbid = it->GetPrimary();
					m_vecLastNeedModDevs.push_back(tDevModGbid);

					tDevModGbid.operType = OPERATE_TYPE_ADD_SECOND;
					tDevModGbid.gbid = it->GetSecond();
				}
				m_vecLastNeedModDevs.push_back(tDevModGbid);
			}
		
			if (m_dwBatchIndex < m_vecBatchGetConditions.size())
			{
				NextState(enStateQryGbid);
				if (!SendGbidBatchGetReq())
				{
					m_dwProgress = 20;SetProgress(m_dwProgress);
					NextState(enStateBatchMod);
					if (!SendBatchOperate())
					{
						m_dwProgress = 25;SetProgress(m_dwProgress);
						NextState(enStateMod);
						if (!SendDevModReq())
						{
							m_dwProgress = 100;SetProgress(m_dwProgress);
							return PROCMSG_DEL;
						}
					}
				}
			}
			else
			{
				m_dwProgress = 20;SetProgress(m_dwProgress);
				NextState(enStateBatchMod);
				if (!SendBatchOperate())
				{
					m_dwProgress = 25;SetProgress(m_dwProgress);
					NextState(enStateMod);
					if (!SendDevModReq())
					{
						m_dwProgress = 100;SetProgress(m_dwProgress);
						return PROCMSG_DEL;
					}	
				}

			}
		}
		break;
	}

	return PROCMSG_OK;
}

u32 CDevGrpModTask::OnQryGbidTimePoll()
{
	return TIMERPOLL_DONE;
}

u32 CDevGrpModTask::OnBatchMod(CMessage* const pcMsg)
{
	if (msgDbMsgTransfer != pcMsg->event)
		return PROCMSG_FAIL;

	ptrint ptr = *reinterpret_cast<ptrint*>(pcMsg->content);
	const CDbMsgTransferBase* pcMsgTrans = reinterpret_cast<const CDbMsgTransferBase*>(ptr);

	if ( msgDevGrp2DevGbidOperateRsp == pcMsgTrans->GetEventId())
	{
		const CDevGrp2DevGbidOperateRsp *pcRsp = static_cast<const CDevGrp2DevGbidOperateRsp*>(pcMsgTrans->GetMsgObjAddr());
		if (pcRsp->GetErrno() == CMS_SUCCESS)
		{
			NextState(enStateMod);
			if (!SendDevModReq())
			{
				m_dwProgress = 100;SetProgress(m_dwProgress);
				return PROCMSG_DEL;
			}
		}
		else
		{
			m_dwProgress = 0;SetProgress(m_dwProgress);
			return PROCMSG_DEL;
		}
	}
	return TIMERPOLL_DONE;
}


u32 CDevGrpModTask::OnMod(CMessage* const pcMsg)
{
	if (msgDbMsgTransfer != pcMsg->event)
		return PROCMSG_FAIL;

	ptrint ptr = *reinterpret_cast<ptrint*>(pcMsg->content);
	const CDbMsgTransferBase* pcMsgTrans = reinterpret_cast<const CDbMsgTransferBase*>(ptr);

	if ( msgDevGrp2DevGbidOperateRsp == pcMsgTrans->GetEventId())
	{
		const CDevGrp2DevGbidOperateRsp *pcRsp = static_cast<const CDevGrp2DevGbidOperateRsp*>(pcMsgTrans->GetMsgObjAddr());
		if (pcRsp->GetErrno() == CMS_SUCCESS)
		{
			if (!SendDevModReq())
			{
				m_dwProgress = 100;SetProgress(m_dwProgress);
				return PROCMSG_DEL;
			}
		}
		else
		{
			m_dwProgress = 0;SetProgress(m_dwProgress);
		}
	}

	if (msgDevGrpGbidOperateRsp == pcMsgTrans->GetEventId())
	{
		const CDevGrpGbidOperateRsp *pcRsp = static_cast<const CDevGrpGbidOperateRsp*>(pcMsgTrans->GetMsgObjAddr());
		if (pcRsp->GetErrno() == CMS_SUCCESS)
		{
			m_dwProgress = 100;SetProgress(m_dwProgress);
		}
		else 
		{
			m_dwProgress = 0;SetProgress(m_dwProgress);
		}
		SendRsp(pcRsp->GetErrno());
		return PROCMSG_DEL;
	}

	return PROCMSG_OK;
}

u32 CDevGrpModTask::OnModTimePoll()
{
	if (GetCurStateHoldTime() % 3 == 0)
	{
		m_dwProgress++;
		if (m_dwProgress>=99)
			m_dwProgress = 99;
		SetProgress(m_dwProgress);
	}
	return TIMERPOLL_DONE;
}

bool CDevGrpModTask::SendBatchOperate()
{
	if (m_vecBatchOperate.operType == OPERATE_TYPE_NULL)
	{
		return false;
	}
	CDbMsgTransfer<CDevGrp2DevGbidOperateReq> cTrans(MAKEIID(GetAppID(), GetInsID()), GetTaskNO(), msgDevGrp2DevGbidOperateReq);
	cTrans.GetMsgObj().devTreeId = m_strTreeId;
	cTrans.GetMsgObj().devs.push_back(m_vecBatchOperate);
	if (!PostDbMsgTransfer2DbTaskPool(cTrans)) 
	{
		TASKLOG(ERROR_LEV,"CDevGrpModTask PostDbMsgTransfer2DbTaskPool fail!\n");
		return false;
	}
	return true;
}

bool CDevGrpModTask::SendDevModReq()
{
	if (m_vecLastNeedModDevs.empty())
	{
		return false;
	}

	CDbMsgTransfer<CDevGrp2DevGbidOperateReq> cTrans(MAKEIID(GetAppID(), GetInsID()), GetTaskNO(), msgDevGrp2DevGbidOperateReq);
	cTrans.GetMsgObj().devTreeId = m_strTreeId;
	//一次性只要线程池处理100个数据
	while (!m_vecLastNeedModDevs.empty() && cTrans.GetMsgObj().devs.size() <= 100)
	{
		cTrans.GetMsgObj().devs.push_back(m_vecLastNeedModDevs.back());
		m_vecLastNeedModDevs.pop_back();
	}
	
	if (!PostDbMsgTransfer2DbTaskPool(cTrans)) 
	{
		TASKLOG(ERROR_LEV,"CDevGrpModTask PostDbMsgTransfer2DbTaskPool fail!\n");
		return false;
	}
	return true;	
}

void CDevGrpModTask::ConstructGbidGetReq()
{
	TGbDevGbidBatchGetCondition tCondition;
	vector<int> vidindexs,prisec;
	if (m_dwPriSec == 1)//主流
	{
		//批量去辅流
		m_vecBatchOperate.devGrpId = m_strGrpId;
		m_vecBatchOperate.operType = OPERATE_TYPE_BATCH_DEL_SECOND;

		for (vector<CDeviceGroupDeviceInfo4Gb>::iterator it = m_vecDevGrpDevInfos.begin();
			it != m_vecDevGrpDevInfos.end();it++)
		{
			tCondition.SetDevGrpId(it->GetDevGrpId());
			tCondition.SetKdid(it->GetDeviceId());
			tCondition.SetVidnum(0);
			for (vector<TDeviceInfo4Gb>::iterator it4 = it->GetDevinfo().begin();
				it4 != it->GetDevinfo().end();it4++)
			{
				//没有主流，加主流
				if (it4->GetGbid().GetNewprimarygbid().empty())
				{
					vidindexs.push_back(it4->GetVidIndex());
					prisec.push_back(0);//主流
				}
			}
			tCondition.SetVidindexs(vidindexs);
			tCondition.SetPrisec(prisec);
			vidindexs.clear();
			prisec.clear();
			m_vecBatchGetConditions.push_back(tCondition);
		}
	}
	else if (m_dwPriSec == 2)//辅流
	{
		//批量去主流
		m_vecBatchOperate.devGrpId = m_strGrpId;
		m_vecBatchOperate.operType = OPERATE_TYPE_BATCH_DEL_PRIMARY;

		for (vector<CDeviceGroupDeviceInfo4Gb>::iterator it = m_vecDevGrpDevInfos.begin();
			it != m_vecDevGrpDevInfos.end();it++)
		{
			tCondition.SetDevGrpId(it->GetDevGrpId());
			tCondition.SetKdid(it->GetDeviceId());
			tCondition.SetVidnum(0);
			for (vector<TDeviceInfo4Gb>::iterator it4 = it->GetDevinfo().begin();
				it4 != it->GetDevinfo().end();it4++)
			{
				//没有辅流，加辅流
				if (it4->GetGbid().GetNewsecondarygbid().empty())
				{
					vidindexs.push_back(it4->GetVidIndex());
					prisec.push_back(1);//辅流
				}
			}
			tCondition.SetVidindexs(vidindexs);
			tCondition.SetPrisec(prisec);
			vidindexs.clear();
			prisec.clear();
			m_vecBatchGetConditions.push_back(tCondition);
		}
	}
	else if (m_dwPriSec == 3)//全部
	{
		for (vector<CDeviceGroupDeviceInfo4Gb>::iterator it = m_vecDevGrpDevInfos.begin();
			it != m_vecDevGrpDevInfos.end();it++)
		{
			tCondition.SetDevGrpId(it->GetDevGrpId());
			tCondition.SetKdid(it->GetDeviceId());
			tCondition.SetVidnum(0);
			for (vector<TDeviceInfo4Gb>::iterator it4 = it->GetDevinfo().begin();
				it4 != it->GetDevinfo().end();it4++)
			{
				if (it4->GetGbid().GetNewprimarygbid().empty() && !it4->GetGbid().GetNewsecondarygbid().empty())
				{
					vidindexs.push_back(it4->GetVidIndex());
					prisec.push_back(0);//只要获取主流
				}
				else if (!it4->GetGbid().GetNewprimarygbid().empty() && it4->GetGbid().GetNewsecondarygbid().empty())
				{
					vidindexs.push_back(it4->GetVidIndex());
					prisec.push_back(1);//只要获取辅流
				}
				else if (it4->GetGbid().GetNewprimarygbid().empty() && it4->GetGbid().GetNewsecondarygbid().empty())
				{
					vidindexs.push_back(it4->GetVidIndex());
					prisec.push_back(2);//全部
				}
			}
			tCondition.SetVidindexs(vidindexs);
			tCondition.SetPrisec(prisec);
			vidindexs.clear();
			prisec.clear();
			m_vecBatchGetConditions.push_back(tCondition);
		}
	}
}

bool CDevGrpModTask::SendGbidBatchGetReq()
{
	CGbsDevGbidBatchGetReq cBatchReq;
	vector<TGbDevGbidBatchGetCondition> vecConditions;
	vecConditions.clear();
	int indexTmp = m_dwBatchIndex;
	if (!m_vecBatchGetConditions.empty())
	{
		while (m_dwBatchIndex < m_vecBatchGetConditions.size() && (m_dwBatchIndex-indexTmp) < MAX_BATCH_DEAL_NUM )
		{	
			vecConditions.push_back(m_vecBatchGetConditions[m_dwBatchIndex]);
			cBatchReq.SetVecConditions(vecConditions);
			m_dwBatchIndex++;
		}
	}

	if (indexTmp == m_dwBatchIndex)//一包都没有装下
	{
		return false;
	}

	PostMsgReq(cBatchReq,m_tGbsUri);
	return true;
}

void CDevGrpModTask::SendRsp(s32 ErrNo)
{
	CDeviceGroupMod4GbRsp cRsp;
	cRsp.SetErrorCode(ErrNo);
	cRsp.SetOperUuid(m_operUuid);
	PostMsgRsp(m_TransId,cRsp);
}

void CDevGrpModTask::SetProgress(int progress)
{
	g_cDbTaskPool.AddTransProgress(m_operUuid,100,progress);
}