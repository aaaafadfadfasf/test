#include "user_devgrp_vidsrc_query_task.h"
#include "common/ssn_taskbase.h"

CUserDevGrpVidsrcQryTask::CUserDevGrpVidsrcQryTask( CSsnTaskbase* pcSsnTask )
    :CSipTask(pcSsnTask->GetInstance()), m_pcSsnTask(pcSsnTask)
{
    m_tSipUri = m_pcSsnTask->GetClientSipURI();
    m_pcQryRsp = NULL;
    m_strObjName = "CUserDevGrpVidsrcQryTask";
	m_bHasGetLastNtf = false;
	m_bWaitNtfRsp = false;
}

CUserDevGrpVidsrcQryTask::~CUserDevGrpVidsrcQryTask()
{
    while (!m_cQryNtfLst.empty())
    {
        delete m_cQryNtfLst.front();
        m_cQryNtfLst.pop_front();
    }
    if (NULL != m_pcQryRsp)
    {
        delete m_pcQryRsp;
    }

    m_pcSsnTask->ReverseDeleteQueryTaskNoNumRecord(this);
}

const char* CUserDevGrpVidsrcQryTask::GetStateName( u32 dwState ) const
{
    if (dwState == enStateIdle) return "enStateIdle";
    if (dwState == enStateQuery) return "enStateQuery";
    if (dwState == enStateTransNtf) return "enStateTransNtf";
    return "unknown";
}

void CUserDevGrpVidsrcQryTask::InitStateMachine()
{
    DoNextState(enStateIdle);

    CStateProc cProc;

    cProc.ProcMsg = (CTask::PFProcMsg)&CUserDevGrpVidsrcQryTask::OnIdle;
    cProc.ProcErrMsg = cProc.ProcMsg;
    cProc.TimerPoll = &CTask::TimerPollSkip;
    AddRuleProc(enStateIdle, cProc);

    cProc.ProcMsg = (CTask::PFProcMsg)&CUserDevGrpVidsrcQryTask::OnQuery;
    cProc.ProcErrMsg = cProc.ProcMsg;
    cProc.TimerPoll = (CTask::PFTimerPoll)&CUserDevGrpVidsrcQryTask::QueryTimerPoll;
    AddRuleProc(enStateQuery, cProc);
    
	cProc.ProcMsg = (CTask::PFProcMsg)&CUserDevGrpVidsrcQryTask::OnTransNtf;
    cProc.ProcErrMsg = cProc.ProcMsg;
    cProc.TimerPoll = (CTask::PFTimerPoll)&CUserDevGrpVidsrcQryTask::TransNtfTimerPoll;
    AddRuleProc(enStateTransNtf, cProc);
}

u32 CUserDevGrpVidsrcQryTask::OnIdle( CMessage *const pcMsg )
{
    if (TAS_CUSER_DEVGRP_VIDSRC_QRY_REQ == pcMsg->event)
    {
		COspSipMsg* const pcSipMsg = reinterpret_cast<COspSipMsg* const>(pcMsg->content);
		CTaskMsgTransfer<CUserDevGrpVidsrcQryReq> cTrans(GetTaskNO(), pcSipMsg->GetSipTransID(), pcMsg->event);
		pcSipMsg->GetMsgBody(cTrans.GetMsgObj());
		m_strQueryTag = cTrans.GetMsgObj().GetQueryTag();

		if (PostTaskMsgTransfer2DbTaskPool(cTrans))
		{
			m_strObjName.append("[");
			m_strObjName.append(OspExtEventDesc(pcMsg->event));
			m_strObjName.append("]");
			DoNextState(enStateQuery);
			return PROCMSG_OK;
		}
		else
		{
			Log(WARNING_LEV, "CUserDevGrpVidsrcQryTask::OnIdle delete because PostTaskMsgTransfer2DbTaskPool fail !!!\n");
			return PROCMSG_DEL;
		}
    }
	else
	{
		Log(WARNING_LEV, "CUserDevGrpVidsrcQryTask::OnIdle delete with wrong-msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
		return PROCMSG_DEL;
	}
}

u32 CUserDevGrpVidsrcQryTask::OnQuery( CMessage *const pcMsg )
{
    if (msgTaskMsgTransfer4Db != pcMsg->event)
    {
        CMS_ASSERT(false);
        Log(WARNING_LEV, "CUserDevGrpVidsrcQryTask::OnQuery delete(invalid message)\n");
        return PROCMSG_DEL;
    }

    DoNextState(enStateQuery); // 收到了查询应答，重新进入状态更新当前状态Holdtime

    ptrint ptr= *reinterpret_cast<ptrint*>(pcMsg->content);
    CTaskMsgTransferBase* pcTrans = reinterpret_cast<CTaskMsgTransferBase*>(ptr);
    
	if (TAS_CUSER_DEVGRP_VIDSRC_QRY_RSP == pcTrans->GetEventId())
	{
		CMS_ASSERT(m_pcQryRsp == NULL);
		m_pcQryRsp = pcTrans;
		CUserDevGrpVidsrcQryRsp *pcRsp = reinterpret_cast<CUserDevGrpVidsrcQryRsp*>(pcTrans->GetMsgObjAddr());
		*reinterpret_cast<ptrint*>(pcMsg->content) = static_cast<ptrint>(0); // 自己保存之后置空，这样CCtrl才不会Delete掉它

		Log(PROGRESS_LEV, "CUserDevGrpVidsrcQryTask::OnQuery rsp's HasRecords:%d, querytag:%s\n", 
			pcRsp->GetHasRecords(), pcRsp->GetQueryTag().c_str());

		DoNextState(enStateQuery);
		if (DoTransfer(true))
		{
			if (!pcRsp->GetHasRecords() || pcRsp->GetErrorCode() != CMS_SUCCESS)
			{
				// 查询出错或者总数为0, 无需发送Ntf（直接删除）
				Log(EVENT_LEV, "CUserDevGrpVidsrcQryTask recv rsp with HasRecords[%d]-Errcode[%d] \n", pcRsp->GetHasRecords(), pcRsp->GetErrorCode());
				return PROCMSG_DEL;
			}

			DoNextState(enStateTransNtf);
			return PROCMSG_OK;
		}
		else
		{
			Log(WARNING_LEV, "CUserDevGrpVidsrcQryTask::OnQuery Send Rsp fail !\n");
			return PROCMSG_DEL;
		}
	}
	else
	{
		Log(WARNING_LEV, "CUserDevGrpVidsrcQryTask::OnQuery recv err-msg[%d-%s] !\n",
			pcTrans->GetEventId(), OspExtEventDesc(pcTrans->GetEventId()).c_str());
		return PROCMSG_DEL;
	}
}

u32 CUserDevGrpVidsrcQryTask::OnTransNtf( CMessage *const pcMsg )
{
    Log(PROGRESS_LEV, "CUserDevGrpVidsrcQryTask::OnTransferNtf recv event[%u-%s]\n", 
        pcMsg->event, OspExtEventDesc(pcMsg->event).c_str());

	switch(pcMsg->event)
	{
	case msgTaskMsgTransfer4Db:
		{
			ptrint ptr= *reinterpret_cast<ptrint*>(pcMsg->content);
			CTaskMsgTransferBase* pcTrans = reinterpret_cast<CTaskMsgTransferBase*>(ptr);
			if (TAS_CUSER_DEVGRP_VIDSRC_QRY_NTF_REQ == pcTrans->GetEventId())
			{
				CUserDevGrpVidsrcQryNtfReq *pcBase = reinterpret_cast<CUserDevGrpVidsrcQryNtfReq*>(pcTrans->GetMsgObjAddr());
				m_cQryNtfLst.push_back(pcTrans);
				if(pcBase->GetLastNtf())
				{
					m_bHasGetLastNtf = true;
				}
				*reinterpret_cast<ptrint*>(pcMsg->content) = static_cast<ptrint>(0); // 自己保存之后置空，这样CCtrl才不会Delete掉它
				if(!m_bWaitNtfRsp)
				{
					DoTransfer(false);
				}
			}
			else
			{
				Log(WARNING_LEV, "CUserDevGrpVidsrcQryTask::OnTransNtf recv unknown msgTaskMsgTransfer4Db[%s-%d] \n",
					OspExtEventDesc(pcTrans->GetEventId()).c_str(), pcTrans->GetEventId());
				return PROCMSG_FAIL;
			}
		}
		break;

	case TAS_CUSER_DEVGRP_VIDSRC_QRY_NTF_RSP:
		{
			// 收到NtfRsp消息, 判断序列号是否为队首消息的应答
			string strSeqNum = GetSipSeqNO(reinterpret_cast<COspSipMsg*>(pcMsg->content)->GetMsgBody());
			const CTaskMsgTransferBase *pcTrans = m_cQryNtfLst.front();
			
			if (reinterpret_cast<CEventReq*>(pcTrans->GetMsgObjAddr())->GetSeqNum()
				== atoi(strSeqNum.c_str()))
			{
				DoNextState(enStateTransNtf); // 收到了NtfReq的应答，重新进入状态更新当前状态Holdtime
				m_bWaitNtfRsp = false;
				delete pcTrans;
				m_cQryNtfLst.pop_front();

				if (m_cQryNtfLst.empty() && m_bHasGetLastNtf)
				{
					Log(PROGRESS_LEV, "CUserDevGrpVidsrcQryTask::OnTransferNtf finished transfer\n");
					return PROCMSG_DEL;
				}

				if (!m_cQryNtfLst.empty())
				{
					DoTransfer(false);
				}
				
			}
		}
		break;

	default:
		{
			return PROCMSG_FAIL;
		}
		break;
	}
    return PROCMSG_OK;
}

u32 CUserDevGrpVidsrcQryTask::TransNtfTimerPoll()
{
    if (GetCurStateHoldTime() < 60*2) 
    {
        return TIMERPOLL_DONE;
    }
    else
    {
        CUserDevGrpVidsrcQryRsp* pcResult = reinterpret_cast<CUserDevGrpVidsrcQryRsp*>(m_pcQryRsp->GetMsgObjAddr());
        Log(WARNING_LEV, "TransNtf timeout, Rsp HasRecords:%u, leave Ntf Num:%u\n", (u32)pcResult->GetHasRecords(), (u32)m_cQryNtfLst.size());
        return TIMERPOLL_DEL;
    }
}

u32 CUserDevGrpVidsrcQryTask::QueryTimerPoll( )
{
    return (GetCurStateHoldTime() > 60*5)? TIMERPOLL_DEL: TIMERPOLL_DONE; // 累计300秒数据库查询不返回就删除任务
}

bool CUserDevGrpVidsrcQryTask::DoTransfer(bool bRsp)
{
    bool bRet;
    string strMsg;
    if (bRsp)
    {
        const CTaskMsgTransferBase *pcTrans = m_pcQryRsp;
        strMsg = reinterpret_cast<CSipEvent*>(pcTrans->GetMsgObjAddr())->ToXml();
        bRet = (PROCMSG_OK == PostMsgRsp(pcTrans->GetSipTransId(), strMsg));
    }
    else
    {
        const CTaskMsgTransferBase *pcTrans = m_cQryNtfLst.front();
		strMsg = reinterpret_cast<CSipEvent*>(pcTrans->GetMsgObjAddr())->ToXml();
		
        bRet = (PROCMSG_OK == PostMsgReq(strMsg, m_tSipUri));
		if (bRet)
		{
			m_bWaitNtfRsp = true;
		}
    }

    if (!bRet)
    {
        Log(WARNING_LEV, "CUserDevGrpVidsrcQryTask[%s-%u] send message failed, queryTag:%s\n", 
            GetObjName(), GetTaskNO(), m_strQueryTag.c_str());
    }
    else
    {
        CQueryRspBase *pcRsp = static_cast<CQueryRspBase*>(m_pcQryRsp->GetMsgObjAddr());
        Log(PROGRESS_LEV, "CUserDevGrpVidsrcQryTask[%s-%u] send message success, queryTag:%s, bRsp:%s, entryNum:%d\n", 
            GetObjName(), GetTaskNO(), m_strQueryTag.c_str(), bRsp? "yes":"no", pcRsp->GetEntryNum());
    }

    return bRet;
}

void CUserDevGrpVidsrcQryTask::DoNextState( u32 enState )
{
    NextState(enState);
}
