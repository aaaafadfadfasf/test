#include "devgrp2dev_mod.h"
#include "common/ssn_taskbase.h"
#include "cms/commonconfig/cmcfg.h"
#include "cctrl.h"

/************************************************************************
设备的修改 wanghui
增加主流、增加辅流、删除主流、删除辅流
************************************************************************/
CDevGrp2DevModTask::CDevGrp2DevModTask( CSsnTaskbase* pcSsnTask )
	:CSipTask(pcSsnTask->GetInstance())
{
	m_tGbsUri = m_tGbsUri.GetGbsURI();
	m_tGbsUri.SetDomain(GetDomainName());

	m_operateType = TAS_OPERATE_TYPE_NULL;
	
}

CDevGrp2DevModTask::~CDevGrp2DevModTask()
{

}

const char* CDevGrp2DevModTask::GetStateName( u32 dwState ) const
{
	switch (dwState)
	{
	case enStateIdle:
		return "enStateIdle";
	case enStateGetGbid:
		return "enStateGetGbid";
	case enStateMod:
		return "enStateMod";
	default:
		return "NA";
	}
}

void CDevGrp2DevModTask::InitStateMachine()
{
	CStateProc cProc;

	cProc.ProcMsg = (CTask::PFProcMsg)&CDevGrp2DevModTask::OnIdle;
	cProc.ProcErrMsg = cProc.ProcMsg;
	cProc.TimerPoll = &CTask::TimerPollSkip;
	AddRuleProc(enStateIdle, cProc);

	cProc.ProcMsg = (CTask::PFProcMsg)&CDevGrp2DevModTask::OnGetGbid;
	cProc.ProcErrMsg = cProc.ProcMsg;
	cProc.TimerPoll = (CTask::PFTimerPoll)&CDevGrp2DevModTask::OnGetGbidTimePoll;
	AddRuleProc(enStateGetGbid, cProc);

	cProc.ProcMsg = (CTask::PFProcMsg)&CDevGrp2DevModTask::OnMod;
	cProc.ProcErrMsg = cProc.ProcMsg;
	cProc.TimerPoll = (CTask::PFTimerPoll)&CDevGrp2DevModTask::OnGetGbidTimePoll;
	AddRuleProc(enStateMod, cProc);

	NextState(enStateIdle);
}


u32 CDevGrp2DevModTask::OnIdle( CMessage *const pcMsg )
{
	COspSipMsg* const pcSipMsg = reinterpret_cast<COspSipMsg* const>(pcMsg->content);
	if (pcMsg->event == TAS_DEVICE_GROUP_DEVICE_MOD_4GB_REQ)
	{
		pcSipMsg->GetMsgBody(m_cModReq);
		m_operateType = m_cModReq.GetOperatetype();
		m_TransId = pcSipMsg->GetSipTransID();

		AccordingTypeToStart();
	}
	return PROCMSG_OK;
}

u32 CDevGrp2DevModTask::OnGetGbid(CMessage* const pcMsg)
{
	COspSipMsg* const pcSipMsg = reinterpret_cast<COspSipMsg* const>(pcMsg->content);
	if (pcMsg->event == GBS_DEV_GBID_BATCH_GET_RSP)
	{
		CGbsDevGbidBatchGetRsp cRsp;
		pcSipMsg->GetMsgBody(cRsp);

		for (vector<TGbDevGbidBatchGetInfo>::iterator it = cRsp.GetVecBatchDevInfo().begin();
			it != cRsp.GetVecBatchDevInfo().end();it++)
		{
			m_tOperateGbInfo.devGrpId = m_cModReq.GetDeviceGroupID();
			m_tOperateGbInfo.devId = it->GetKdid();
			m_tOperateGbInfo.vidSrcIdx = it->GetVidindex();
			if (m_operateType == TAS_OPERATE_TYPE_ADD_PRIMARY)//增加主流
			{
				m_tOperateGbInfo.operType = OPERATE_TYPE_ADD_PRIMARY;
				m_tOperateGbInfo.gbid = it->GetPrimary();
			}
			if (m_operateType == TAS_OPERATE_TYPE_ADD_SECOND)//增加辅流
			{
				m_tOperateGbInfo.operType = OPERATE_TYPE_ADD_SECOND;
				m_tOperateGbInfo.gbid = it->GetSecond();
			}
		}

		if (m_tOperateGbInfo.gbid.empty())
		{
			SendRsp(CMS_SUCCESS);
			return PROCMSG_DEL;
		}

		NextState(enStateMod);
		if (!SendDevModReq())
		{
			SendRsp(ERR_TAS_RECORD_NOT_EXISTS);
			return PROCMSG_DEL;
		}
		
	}
	return PROCMSG_OK;
}

u32 CDevGrp2DevModTask::OnGetGbidTimePoll()
{
	return TIMERPOLL_DONE;
}

u32 CDevGrp2DevModTask::OnMod(CMessage* const pcMsg)
{
	if (msgDbMsgTransfer != pcMsg->event)
		return PROCMSG_FAIL;

	ptrint ptr = *reinterpret_cast<ptrint*>(pcMsg->content);
	const CDbMsgTransferBase* pcMsgTrans = reinterpret_cast<const CDbMsgTransferBase*>(ptr);

	if ( msgDevGrp2DevGbidOperateRsp == pcMsgTrans->GetEventId() )
	{
		const CDevGrp2DevGbidOperateRsp *pcRsp = static_cast<const CDevGrp2DevGbidOperateRsp*>(pcMsgTrans->GetMsgObjAddr());

		SendRsp(pcRsp->GetErrno(),pcRsp->gbids.front());

		return PROCMSG_DEL;
	}

	return PROCMSG_OK;
}

u32 CDevGrp2DevModTask::OnModTimePoll()
{
	return TIMERPOLL_DONE;
}


void CDevGrp2DevModTask::SendGetGbid()
{
	CGbsDevGbidBatchGetReq cBatchReq;

	cBatchReq.GetVecConditions().push_back(m_BatchGetCondition);

	if (cBatchReq.ToXml().empty())
	{
		return;
	}

	PostMsgReq(cBatchReq,m_tGbsUri);
}

bool CDevGrp2DevModTask::SendDevModReq()
{
	CDbMsgTransfer<CDevGrp2DevGbidOperateReq> cTrans(MAKEIID(GetAppID(), GetInsID()), GetTaskNO(), msgDevGrp2DevGbidOperateReq);
	cTrans.GetMsgObj().devTreeId = m_cModReq.GetDevTreeId();
	cTrans.GetMsgObj().devs.push_back(m_tOperateGbInfo);
	//cTrans.GetMsgObj() = m_cGbidOperateReq;

	if (!PostDbMsgTransfer2DbTaskPool(cTrans))
	{
		TASKLOG(ERROR_LEV,"PostDbMsgTransfer2DbTaskPool fail!!\n");
		return false;
	}
	return true;	
}

void CDevGrp2DevModTask::AccordingTypeToStart()
{
	m_BatchGetCondition.SetKdid(m_cModReq.GetDeviceId());
	m_BatchGetCondition.GetVidindexs().push_back(m_cModReq.GetVidIndex());
	m_BatchGetCondition.SetVidnum(0);
	switch (m_operateType)
	{
	case TAS_OPERATE_TYPE_ADD_PRIMARY://增加主流
		{
			m_BatchGetCondition.GetPrisec().push_back(0);
			NextState(enStateGetGbid);
			SendGetGbid();
		}
		break;
	case TAS_OPERATE_TYPE_ADD_SECOND://增加辅流
		{
			m_BatchGetCondition.GetPrisec().push_back(1);
			NextState(enStateGetGbid);
			SendGetGbid();
		}
		break;
	case TAS_OPERATE_TYPE_DEL_PRIMARY://删除主流
		{
			m_tOperateGbInfo.devGrpId = m_cModReq.GetDeviceGroupID();
			m_tOperateGbInfo.devId = m_cModReq.GetDeviceId();
			m_tOperateGbInfo.vidSrcIdx = m_cModReq.GetVidIndex();
			m_tOperateGbInfo.operType = OPERATE_TYPE_DEL_PRIMARY;
			m_tOperateGbInfo.gbid = "";
			NextState(enStateMod);
			SendDevModReq();
		}
		break;
	case TAS_OPERATE_TYPE_DEL_SECOND://删除辅流
		{
			m_tOperateGbInfo.devGrpId = m_cModReq.GetDeviceGroupID();
			m_tOperateGbInfo.devId = m_cModReq.GetDeviceId();
			m_tOperateGbInfo.vidSrcIdx = m_cModReq.GetVidIndex();
			m_tOperateGbInfo.operType = OPERATE_TYPE_DEL_SECOND;
			m_tOperateGbInfo.gbid = "";

			NextState(enStateMod);
			SendDevModReq();
		}
		break;
	case TAS_OPERATE_TYPE_MOD_PRIMARY://修改主流
		{
			m_tOperateGbInfo.devGrpId = m_cModReq.GetDeviceGroupID();
			m_tOperateGbInfo.devId = m_cModReq.GetDeviceId();
			m_tOperateGbInfo.vidSrcIdx = m_cModReq.GetVidIndex();
			m_tOperateGbInfo.operType = OPERATE_TYPE_MOD_PRIMARY;
			m_tOperateGbInfo.gbid = m_cModReq.GetGbid();

			NextState(enStateMod);
			SendDevModReq();
		}
		break;
	case TAS_OPERATE_TYPE_MOD_SECOND://修改辅流
		{
			m_tOperateGbInfo.devGrpId = m_cModReq.GetDeviceGroupID();
			m_tOperateGbInfo.devId = m_cModReq.GetDeviceId();
			m_tOperateGbInfo.vidSrcIdx = m_cModReq.GetVidIndex();
			m_tOperateGbInfo.operType = OPERATE_TYPE_MOD_SECOND;
			m_tOperateGbInfo.gbid = m_cModReq.GetGbid();

			NextState(enStateMod);
			SendDevModReq();
		}
		break;
	}
}

void CDevGrp2DevModTask::SendRsp(s32 ErrNo,const string& strGbid)
{
	CDeviceGroupDeviceMod4GbRsp cRsp;
	cRsp.SetDevtreeId(m_cModReq.GetDevTreeId());
	cRsp.SetDevgroupId(m_cModReq.GetDeviceGroupID());
	cRsp.SetDeviceId(m_cModReq.GetDeviceId());
	cRsp.SetVidIndex(m_cModReq.GetVidIndex());
	switch (m_operateType)
	{
	case TAS_OPERATE_TYPE_ADD_PRIMARY:
		{
			cRsp.GetGbid().SetOldprimarygbid(m_tOperateGbInfo.gbid);
			cRsp.GetGbid().SetNewprimarygbid(strGbid);
		}
		break;
	case TAS_OPERATE_TYPE_ADD_SECOND:
		{
			cRsp.GetGbid().SetOldsecondarygbid(m_tOperateGbInfo.gbid);
			cRsp.GetGbid().SetNewsecondarygbid(strGbid);
		}
		break;
	case TAS_OPERATE_TYPE_MOD_PRIMARY:
		{
			cRsp.GetGbid().SetNewprimarygbid(strGbid);
		}
		break;
	case TAS_OPERATE_TYPE_MOD_SECOND:
		{
			cRsp.GetGbid().SetNewsecondarygbid(strGbid);
		}
		break;
	case TAS_OPERATE_TYPE_DEL_PRIMARY:
		{
			cRsp.GetGbid().SetOldprimarygbid("");
			cRsp.GetGbid().SetNewprimarygbid("");
		}
		break;
	case TAS_OPERATE_TYPE_DEL_SECOND:
		{
			cRsp.GetGbid().SetOldsecondarygbid("");
			cRsp.GetGbid().SetNewsecondarygbid("");
		}
		break;
	}

	cRsp.SetErrorCode(ErrNo);
	PostMsgRsp(m_TransId,cRsp);
}
