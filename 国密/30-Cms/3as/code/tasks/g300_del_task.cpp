#include "g300_del_task.h"
#include "cctrl.h"
#include "redis/fastdbmng.h"

int g_g300_del_task_num=0;

CG300DelTask::CG300DelTask( CInstExt *pcInst ): CSipTask(pcInst)
{
	m_tTransId = INVALID_TRANS_ID;
	m_dwSeqNum = 0;
    ++g_g300_del_task_num;
}

CG300DelTask::~CG300DelTask()
{
    --g_g300_del_task_num;
}

void CG300DelTask::InitStateMachine()
{
	CStateProc cIdleProc;
	cIdleProc.ProcMsg = (CTask::PFProcMsg)&CG300DelTask::OnIdle;
	cIdleProc.ProcErrMsg = (CTask::PFProcMsg)&CG300DelTask::OnIdle;
	cIdleProc.TimerPoll = (CTask::PFTimerPoll)&CG300DelTask::OnIdleTimer;
	AddRuleProc(enIdle, cIdleProc);

	CStateProc cClearDeviceProc;
	cClearDeviceProc.ProcMsg = (CTask::PFProcMsg)&CG300DelTask::OnClearDevice;
	cClearDeviceProc.ProcErrMsg = (CTask::PFProcMsg)&CG300DelTask::OnClearDevice;
	cClearDeviceProc.TimerPoll = (CTask::PFTimerPoll)&CG300DelTask::OnClearDeviceTimer;
	AddRuleProc(enClearDevice, cClearDeviceProc);

	CStateProc cClearModelProc;
	cClearModelProc.ProcMsg = (CTask::PFProcMsg)&CG300DelTask::OnClearModel;
	cClearModelProc.ProcErrMsg = (CTask::PFProcMsg)&CG300DelTask::OnClearModel;
	cClearModelProc.TimerPoll = (CTask::PFTimerPoll)&CG300DelTask::OnClearModelTimer;
	AddRuleProc(enClearModel, cClearModelProc);

	CStateProc cClearGroupProc;
	cClearGroupProc.ProcMsg = (CTask::PFProcMsg)&CG300DelTask::OnClearGroup;
	cClearGroupProc.ProcErrMsg = (CTask::PFProcMsg)&CG300DelTask::OnClearGroup;
	cClearGroupProc.TimerPoll = (CTask::PFTimerPoll)&CG300DelTask::OnClearGroupTimer;
	AddRuleProc(enClearGroup, cClearGroupProc);

	NextState(enIdle);
}

u32 CG300DelTask::OnIdle(CMessage *const pcMsg)
{
	u32 dwRet = PROCMSG_OK;

	COspSipMsg* const pcSipMsg = reinterpret_cast<COspSipMsg* const>(pcMsg->content);

	if (TAS_GATEWAY_DEL_REQ == pcMsg->event)
	{
		CGatewayDelReq cReq;
		pcSipMsg->GetMsgBody(cReq);

		m_cRsp.SetHeadFromReq(cReq);
		m_cRsp.SetUri(cReq.GetUri());

		TSipURI tUri;
		tUri.SetURIString(cReq.GetUri());

		m_strGatewayId = tUri.GetUser();
		m_tTransId     = pcSipMsg->GetSipTransID();

		if (tUri.GetDomain() == CurrentDomainName())
		{
            HandleProgressInRedis(1);
			// 首先查询并处理设备信息
			QueryDevice();
		}
		else
		{
			SendRsp(ERR_TAS_INVALID_OPERATION);

			dwRet = PROCMSG_DEL;
		}
	}
	else
	{
		Log(CRITICAL_LEV, "CG300DelTask::OnIdle recved invalid msg, msgid=%u\n", pcMsg->event);
		
		dwRet = PROCMSG_DEL;
	}

	return dwRet;
}

u32 CG300DelTask::OnIdleTimer()
{
	return TIMERPOLL_DONE;
}

u32 CG300DelTask::OnClearDevice(CMessage *const pcMsg)
{
	u32 dwRet = PROCMSG_OK;

	if (msgTaskMsgTransfer4Db == pcMsg->event)
	{
		ptrint ptr = *reinterpret_cast<ptrint*>(pcMsg->content);
		const CTaskMsgTransferBase* pcMsgTrans = reinterpret_cast<const CTaskMsgTransferBase*>(ptr);

		switch (pcMsgTrans->GetEventId())
		{
		case msgQryDevidRsp:
			{
				Log(EVENT_LEV, "CG300DelTask::OnClearDevice receive query rsp, msgid=%u\n", pcMsgTrans->GetEventId());

				const std::pair< int, vector<string> > *pcRsp = static_cast<const std::pair< int, vector<string> >*>(pcMsgTrans->GetMsgObjAddr());
				if (pcRsp->first != CMS_SUCCESS)
				{
                    SendRsp(pcRsp->first);
                    dwRet=PROCMSG_DEL;
                    break;
				}
                if (pcRsp->second.empty()){
                    Log(EVENT_LEV, "CG300DelTask::OnClearDevice G300 设备数量为0，无需清除设备信息!\n");

                    // 处理设备型号信息
                    QueryModel();
				}
                else{
                    HandleProgressInRedis(20);
                    for (vector<string>::const_iterator it=pcRsp->second.begin(); it != pcRsp->second.end(); ++it)
                    {
                        m_cDeviceId.insert(*it);
                    }

                    u32 dwCnt = 0;
                    for (set<string>::iterator it = m_cDeviceId.begin(); 
                        dwCnt < m_dwMaxParallelProcess && m_cDeviceId.end() != it; ++dwCnt)
                    {
                        DeleteDevice(*it, m_dwSeqNum);

                        m_cProcessing.insert(pair<u32, string>(m_dwSeqNum, *it));
                        m_cDeviceId.erase(it++);

                        ++m_dwSeqNum;
                    }
                }
			}
			break;
		case TAS_DEVICE_DEL_RSP:
			{
				const CDeviceDelRsp *pcRsp = static_cast<const CDeviceDelRsp*>(pcMsgTrans->GetMsgObjAddr());

				map<u32, string>::iterator it = m_cProcessing.find(pcRsp->GetSeqNum());
				if (m_cProcessing.end() != it)
				{
					if (CMS_SUCCESS != pcRsp->GetErrorCode())
					{
						m_cFailedDeviceMap.insert(pair<string, s32>(it->second, pcRsp->GetErrorCode()));
					}

					m_cProcessing.erase(it);

					if (!m_cDeviceId.empty())			// 还有尚未处理的设备，则继续处理
					{
						string strDeviceId = *(m_cDeviceId.begin());
						DeleteDevice(strDeviceId, m_dwSeqNum);

						m_cProcessing.insert(pair<u32, string>(m_dwSeqNum, strDeviceId));
						m_cDeviceId.erase(strDeviceId);

						++m_dwSeqNum;
					}
					else			
					{
						if (m_cProcessing.empty())		// 所有的设备已经处理结束，开始处理设备型号
						{
							QueryModel();
                            HandleProgressInRedis(75);
						}
						else
						{
							// 等待正在处理的其他请求的应答
						}
					}
				}
			}
			break;
		default:
			{
				if (IsNtfMsg(pcMsgTrans))
				{
					const CEventReq *pcNtfReq = reinterpret_cast<const CEventReq*>(pcMsgTrans->GetMsgObjAddr());

					((CCtrlInst *)GetInstance())->GetAppData().HandleChangeNtfReq(pcMsgTrans->GetEventId(), pcNtfReq->ToXml());
				}
				else
				{
					Log(CRITICAL_LEV, "CG300DelTask::OnClearDevice recved invalid msg, msgid=%u\n", pcMsgTrans->GetEventId());

					dwRet = PROCMSG_FAIL;
				}
			}
			break;
		}
	}
	else
	{
		Log(CRITICAL_LEV, "CG300DelTask::OnClearDevice recved invalid msg, msgid=%u\n", pcMsg->event);

		dwRet = PROCMSG_FAIL;
	}

	return dwRet;
}

u32 CG300DelTask::OnClearDeviceTimer()
{
	// 超时处理，回超时应答给请求者
	if (15 < GetCurStateHoldTime())
	{
		SendRsp(ERR_TAS_NOT_FINISHED);
	}
    
	return TIMERPOLL_DONE;
}

u32 CG300DelTask::OnClearModel(CMessage *const pcMsg)
{
	u32 dwRet = PROCMSG_OK;

	if (msgTaskMsgTransfer4Db == pcMsg->event)
	{
		ptrint ptr = *reinterpret_cast<ptrint*>(pcMsg->content);
		const CTaskMsgTransferBase* pcMsgTrans = reinterpret_cast<const CTaskMsgTransferBase*>(ptr);

		switch (pcMsgTrans->GetEventId())
		{
		case TAS_DEVICE_MODEL_QRY_RSP:
			{
				Log(EVENT_LEV, "CG300DelTask::OnClearModel receive query rsp, msgid=%u\n", pcMsgTrans->GetEventId());

				const CDeviceModelQryRsp *pcRsp = static_cast<const CDeviceModelQryRsp*>(pcMsgTrans->GetMsgObjAddr());
				if (0 == pcRsp->GetEntryNum())
				{
					Log(EVENT_LEV, "CG300DelTask::OnClearModel G300 设备型号数量为0，无需清除设备型号信息!\n");

					// 处理设备组信息
					QueryGroup();
				}
			}
			break;
		case TAS_DEVICE_MODEL_QRY_NTF_REQ:
			{
				const CDeviceModelQryNtfReq *pcNtf = static_cast<const CDeviceModelQryNtfReq*>(pcMsgTrans->GetMsgObjAddr());
				const vector<CDeviceModelInfo> &cInfo = pcNtf->GetDeviceModelInfo();
				for (vector<CDeviceModelInfo>::const_iterator it = cInfo.begin(); cInfo.end() != it; ++it)
				{
					m_cModelId.insert(it->GetDeviceModelId());
				}

				if (pcNtf->IsLastNtf())		// 数据获取完毕，开始处理
				{
					u32 dwCnt = 0;
					for (set<string>::iterator it = m_cModelId.begin(); 
						dwCnt < m_dwMaxParallelProcess && m_cModelId.end() != it; ++dwCnt)
					{
						DeleteModel(*it, m_dwSeqNum);

						m_cProcessing.insert(pair<u32, string>(m_dwSeqNum, *it));
						m_cModelId.erase(it++);

						++m_dwSeqNum;
					}
				}
			}
			break;
		case TAS_DEVICE_MODEL_DEL_RSP:
			{
				const CDeviceModelDelRsp *pcRsp = static_cast<const CDeviceModelDelRsp*>(pcMsgTrans->GetMsgObjAddr());

				map<u32, string>::iterator it = m_cProcessing.find(pcRsp->GetSeqNum());
				if (m_cProcessing.end() != it)
				{
					u32 dwErrorCode = pcRsp->GetErrorCode();
					if (CMS_SUCCESS != dwErrorCode && ERR_TAS_INVALID_OPERATION != dwErrorCode)
					{
						m_cFailedModelMap.insert(pair<string, s32>(it->second, dwErrorCode));
					}

					m_cProcessing.erase(it);

					if (!m_cModelId.empty())			// 还有尚未处理的设备型号，则继续处理
					{
						string strModelId = *(m_cModelId.begin());
						DeleteDevice(strModelId, m_dwSeqNum);

						m_cProcessing.insert(pair<u32, string>(m_dwSeqNum, strModelId));
						m_cModelId.erase(strModelId);

						++m_dwSeqNum;
					}
					else			
					{
						if (m_cProcessing.empty())		// 所有的设备型号已经处理结束，开始处理设备组
						{
							QueryGroup();
                            HandleProgressInRedis(80);
						}
						else
						{
							// 等待正在处理的其他请求的应答
						}
					}
				}
			}
			break;
		default:
			{
				if (IsNtfMsg(pcMsgTrans))
				{
					const CEventReq *pcNtfReq = reinterpret_cast<const CEventReq*>(pcMsgTrans->GetMsgObjAddr());

					((CCtrlInst *)GetInstance())->GetAppData().HandleChangeNtfReq(pcMsgTrans->GetEventId(), pcNtfReq->ToXml());
				}
				else
				{
					Log(CRITICAL_LEV, "CG300DelTask::OnClearModel recved invalid msg, msgid=%u\n", pcMsgTrans->GetEventId());

					dwRet = PROCMSG_FAIL;
				}
			}
			break;
		}
	}
	else
	{
		Log(CRITICAL_LEV, "CG300DelTask::OnClearModel recved invalid msg, msgid=%u\n", pcMsg->event);

		dwRet = PROCMSG_FAIL;
	}

	return dwRet;
}

u32 CG300DelTask::OnClearModelTimer()
{
	// 超时处理，回超时应答给请求者
	if (5 < GetCurStateHoldTime())
	{
		SendRsp(ERR_TAS_NOT_FINISHED);
	}

	return TIMERPOLL_DONE;
}

u32 CG300DelTask::OnClearGroup(CMessage *const pcMsg)
{
	u32 dwRet = PROCMSG_OK;

	if (msgTaskMsgTransfer4Db == pcMsg->event)
	{
		ptrint ptr = *reinterpret_cast<ptrint*>(pcMsg->content);
		const CTaskMsgTransferBase* pcMsgTrans = reinterpret_cast<const CTaskMsgTransferBase*>(ptr);

		switch (pcMsgTrans->GetEventId())
		{
		case TAS_DEVICE_GROUP_QRY_RSP:
			{
				Log(EVENT_LEV, "CG300DelTask::OnClearGroup receive query rsp, msgid=%u\n", pcMsgTrans->GetEventId());

				const CDeviceGroupQryRsp *pcRsp = static_cast<const CDeviceGroupQryRsp*>(pcMsgTrans->GetMsgObjAddr());
				if (0 == pcRsp->GetEntryNum())
				{
					Log(EVENT_LEV, "CG300DelTask::OnClearGroup G300 设备组数量为0，无需清除设备组信息!\n");

					SendRsp();
					dwRet = PROCMSG_DEL;
				}
			}
			break;
		case TAS_DEVICE_GROUP_QRY_NTF_REQ:
			{
				const CDeviceGroupQryNtfReq *pcNtf = static_cast<const CDeviceGroupQryNtfReq*>(pcMsgTrans->GetMsgObjAddr());
				const vector<CDeviceGroupInfo> &cInfo = pcNtf->GetDeviceGroupInfo();
				const vector<bool> &cHasDevice = pcNtf->GetDeviceGroupHasDevice();
				CMS_ASSERT(cInfo.size() == cHasDevice.size() && "cInfo.size() != cHasDevice.size()!!!");
				for (u32 i = 0; i < cInfo.size(); ++i)
				{
					if (!cHasDevice[i])		// 只需要清除里面没有设备的设备组
					{
						m_vecGroupId.push_back(cInfo[i].GetDeviceGroupID());
					}
				}

				if (pcNtf->IsLastNtf())		// 数据获取完毕，开始处理
				{
					if (!m_vecGroupId.empty())			// 还有尚未处理的设备组，则继续处理
					{
						string strGroupId = m_vecGroupId.back();
						DeleteGroup(strGroupId, m_dwSeqNum);

						m_cProcessing.insert(pair<u32, string>(m_dwSeqNum, strGroupId));
						m_vecGroupId.pop_back();

						++m_dwSeqNum;
					}
					else
					{
						SendRsp();
						Log(WARNING_LEV, "CG300DelTask::OnClearGroup no devgrp can be deleted \n");
						return PROCMSG_DEL;
					}
				}
			}
			break;
		case TAS_DEVICE_GROUP_DEL_RSP:
			{
				const CDeviceGroupDelRsp *pcRsp = static_cast<const CDeviceGroupDelRsp*>(pcMsgTrans->GetMsgObjAddr());

				map<u32, string>::iterator it = m_cProcessing.find(pcRsp->GetSeqNum());
				if (m_cProcessing.end() != it)
				{
					if (CMS_SUCCESS != pcRsp->GetErrorCode()
						&& ERR_TAS_RECORD_NOT_EXISTS != pcRsp->GetErrorCode()) //父分组被删除时子分组会被递归删除，不认为是报错
					{
						m_cFailedGroupMap.insert(pair<string, s32>(it->second, pcRsp->GetErrorCode()));
					}

					m_cProcessing.erase(it);

					if (!m_vecGroupId.empty())			// 还有尚未处理的设备组，则继续处理
					{
						string strGroupId = m_vecGroupId.back();
						DeleteGroup(strGroupId, m_dwSeqNum);

						m_cProcessing.insert(pair<u32, string>(m_dwSeqNum, strGroupId));
						m_vecGroupId.pop_back();

						++m_dwSeqNum;
					}
					else			
					{
						if (m_cProcessing.empty())		// 所有的设备组已经处理结束，所有数据清除完毕
						{
							SendRsp();
							dwRet = PROCMSG_DEL;
						}
						else
						{
							// 等待正在处理的其他请求的应答
						}
					}
				}
			}
			break;
		default:
			{
				if (IsNtfMsg(pcMsgTrans))
				{
					const CEventReq *pcNtfReq = reinterpret_cast<const CEventReq*>(pcMsgTrans->GetMsgObjAddr());

					((CCtrlInst *)GetInstance())->GetAppData().HandleChangeNtfReq(pcMsgTrans->GetEventId(), pcNtfReq->ToXml());
				}
				else
				{
					Log(CRITICAL_LEV, "CG300DelTask::OnClearGroup recved invalid msg, msgid=%u\n", pcMsgTrans->GetEventId());

					dwRet = PROCMSG_FAIL;
				}
			}
			break;
		}
	}
	else
	{
		Log(CRITICAL_LEV, "CG300DelTask::OnClearGroup recved invalid msg, msgid=%u\n", pcMsg->event);

		dwRet = PROCMSG_FAIL;
	}

	return dwRet;
}

u32 CG300DelTask::OnClearGroupTimer()
{
	// 超时处理，回超时应答给请求者
	if (5 < GetCurStateHoldTime())
	{
		SendRsp(ERR_TAS_NOT_FINISHED);
	}

	return TIMERPOLL_DONE;
}

void CG300DelTask::QueryDevice()
{
	set<TCreater> tCreaters;
	tCreaters.insert(CREATE_BY_CMSSDK);
	tCreaters.insert(CREATE_BY_GBS_FOREIGN_DOMAIN);

	string strLocalDomain = CurrentDomainId();

	CDeviceQryReq cReq;
	CDeviceInfoQueryCondition &cCond = cReq.GetDeviceInfoQueryCondition();
	cCond.SetOwner(m_strGatewayId);
	cCond.SetCreaters(tCreaters);
	cCond.SetDomainId(strLocalDomain);

	CTaskMsgTransfer<CDeviceQryReq> cTrans(GetTaskNO(), INVALID_TRANS_ID, msgQryDevidReq);
	cTrans.SetMsgObj(cReq);

	// 查询G300的设备
	PostTaskMsgTransfer2DbTaskPool(cTrans);

	// 每次重新查询都将原查询结果和正在处理的数据清空
	m_cProcessing.clear();
	m_cDeviceId.clear();

	NextState(enClearDevice);
}

void CG300DelTask::DeleteDevice(const string &strDeviceId, u32 dwSeqNum)
{
	CDeviceDelReq cReq;
	cReq.SetSeqNum(dwSeqNum);
	cReq.SetDeviceUUID(strDeviceId);

	CTaskMsgTransfer<CDeviceDelReq> cTrans(GetTaskNO(), INVALID_TRANS_ID, TAS_DEVICE_DEL_REQ);
	cTrans.SetMsgObj(cReq);

	// 删除G300的设备
	PostTaskMsgTransfer2DbTaskPool(cTrans);
}

void CG300DelTask::QueryModel()
{
	TCreater tCreater = CREATE_BY_CMSSDK;

	string strLocalDomain = CurrentDomainId();

	CDeviceModelQryReq cReq;
	CDeviceModelQueryCondition &cCond = cReq.GetDeviceModelQryCondition();
	cCond.SetOwner(m_strGatewayId);
	cCond.SetCreater(tCreater);
	cCond.SetDomainId(strLocalDomain);

	CTaskMsgTransfer<CDeviceModelQryReq> cTrans(GetTaskNO(), INVALID_TRANS_ID, TAS_DEVICE_MODEL_QRY_REQ);
	cTrans.SetMsgObj(cReq);

	// 查询G300的设备型号
	PostTaskMsgTransfer2DbTaskPool(cTrans);

	// 每次重新查询都将原查询结果和正在处理的数据清空
	m_cProcessing.clear();
	m_cModelId.clear();

	NextState(enClearModel);
}

void CG300DelTask::DeleteModel(const string &strModelId, u32 dwSeqNum)
{
	CDeviceModelDelReq cReq;
	cReq.SetSeqNum(dwSeqNum);
	cReq.SetDeviceModelId(strModelId);

	CTaskMsgTransfer<CDeviceModelDelReq> cTrans(GetTaskNO(), INVALID_TRANS_ID, TAS_DEVICE_MODEL_DEL_REQ);
	cTrans.SetMsgObj(cReq);

	// 删除G300的设备型号
	PostTaskMsgTransfer2DbTaskPool(cTrans);
}

void CG300DelTask::QueryGroup()
{
	set<TCreater> tCreaters;
	tCreaters.insert(CREATE_BY_CMSSDK);
	tCreaters.insert(CREATE_BY_GBS_FOREIGN_DOMAIN);
	string strLocalDomain = CurrentDomainId();

	CDeviceGroupQryReq cReq;
	CDeviceGroupInfoQueryCondition &cCond =  cReq.GetDeviceGroupInfoQueryCondition();
	cCond.SetOwner(m_strGatewayId);
	cCond.SetCreaters(tCreaters);
	cCond.SetDomainUUID(strLocalDomain);

	CTaskMsgTransfer<CDeviceGroupQryReq> cTrans(GetTaskNO(), INVALID_TRANS_ID, TAS_DEVICE_GROUP_QRY_REQ);
	cTrans.SetMsgObj(cReq);

	// 查询G300的设备组
	PostTaskMsgTransfer2DbTaskPool(cTrans);

	// 每次重新查询都将原查询结果和正在处理的数据清空
	m_cProcessing.clear();
	m_vecGroupId.clear();

	NextState(enClearGroup);
}

void CG300DelTask::DeleteGroup(const string &strGroupId, u32 dwSeqNum)
{
	CDeviceGroupDelReq cReq;
	cReq.SetSeqNum(dwSeqNum);
	cReq.GetDeviceGroupInfoKey().SetDeviceGroupID(strGroupId);

	CTaskMsgTransfer<CDeviceGroupDelReq> cTrans(GetTaskNO(), INVALID_TRANS_ID, TAS_DEVICE_GROUP_DEL_REQ);
	cTrans.SetMsgObj(cReq);

	// 删除G300的设备组
	PostTaskMsgTransfer2DbTaskPool(cTrans);
}

void CG300DelTask::SendRsp(u32 dwErrorCode)
{
    if(m_tTransId!=INVALID_TRANS_ID){
        m_cRsp.SetErrorCode(dwErrorCode);
        PostMsgRsp(m_tTransId, m_cRsp);
        m_tTransId=INVALID_TRANS_ID;
    }

    if (dwErrorCode==CMS_SUCCESS)
    {
        HandleProgressInRedis(100);
    }
}

void CG300DelTask::SendRsp()
{
	if (!m_cFailedDeviceMap.empty() || !m_cFailedGroupMap.empty())
	{
		m_cRsp.SetErrorCode(ERR_TAS_OP_PART_FINISH);

		// 将清除失败的信息记录下来
		for (map<string, s32>::const_iterator it = m_cFailedDeviceMap.begin(); m_cFailedDeviceMap.end() != it; ++it)
		{
			Log(WARNING_LEV, "CG300DelTask::Failed Device[%s - %d]\n", it->first.c_str(), it->second);
		}

		for (map<string, s32>::const_iterator it = m_cFailedGroupMap.begin(); m_cFailedGroupMap.end() != it; ++it)
		{
			Log(WARNING_LEV, "CG300DelTask::Failed Group[%s - %d]\n", it->first.c_str(), it->second);
		}
	}
	else
	{
		m_cRsp.SetErrorCode(CMS_SUCCESS);
		if (!m_cFailedModelMap.empty())
		{
			map<string, s32>::const_iterator it = m_cFailedModelMap.begin();
			for (; m_cFailedModelMap.end() != it; ++it)
			{
				Log(EVENT_LEV, "CG300DelTask::Failed del Model[%s - %d] \n", it->first.c_str(), it->second);
				if (it->second != ERR_TAS_DEVICE_MODEL_USING)
				{
					break;
				}
			}
			if (it != m_cFailedModelMap.end())
			{
				m_cRsp.SetErrorCode(ERR_TAS_OP_PART_FINISH);
			}
		}
	}
	
    if(m_tTransId!=INVALID_TRANS_ID){
	    PostMsgRsp(m_tTransId, m_cRsp);
        m_tTransId=INVALID_TRANS_ID;
    }
    HandleProgressInRedis(100);
}

bool CG300DelTask::IsNtfMsg(const CTaskMsgTransferBase *pcTrans)
{
	u16 wEvent = pcTrans->GetEventId();
	if (TAS_DEVICE_DEL_NTF_REQ == pcTrans->GetEventId()
		|| TAS_DEVICE_MODEL_DEL_NTF_REQ == pcTrans->GetEventId()
		|| TAS_DEVICE_GROUP_DEL_NTF_REQ == pcTrans->GetEventId())
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CG300DelTask::HandleProgressInRedis( int progress )
{
    RedisSetProcessValue4Tas(enTasGatewayDevDelProcess, m_strGatewayId, progress);
}
