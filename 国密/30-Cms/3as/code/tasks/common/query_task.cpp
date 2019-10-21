#include "query_task.h"
#include "ssn_taskbase.h"

CQueryTask::CQueryTask( CSsnTaskbase* pcSsnTask )
    :CSipTask(pcSsnTask->GetInstance()), m_pcSsnTask(pcSsnTask)
{
    m_tSipUri = m_pcSsnTask->GetClientSipURI();
    m_pcQryRsp = NULL;
    m_strObjName = "CQueryTask";
}

CQueryTask::~CQueryTask()
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

    m_pcSsnTask->ReverseDeleteQueryTaskRecord(this);
}

const char* CQueryTask::GetStateName( u32 dwState ) const
{
    if (dwState == enStateIdle) return "enStateIdle";
    if (dwState == enStateQuery) return "enStateQuery";
    if (dwState == enStateTransRsp) return "enStateTransRsp";
    if (dwState == enStateTransNtf) return "enStateTransNtf";
    return "unknown";
}

void CQueryTask::InitStateMachine()
{
    DoNextState(enStateIdle);

    CStateProc cProc;

    cProc.ProcMsg = (CTask::PFProcMsg)&CQueryTask::OnIdle;
    cProc.ProcErrMsg = cProc.ProcMsg;
    cProc.TimerPoll = &CTask::TimerPollSkip;
    AddRuleProc(enStateIdle, cProc);
    cProc.ProcMsg = (CTask::PFProcMsg)&CQueryTask::OnQuery;
    cProc.ProcErrMsg = cProc.ProcMsg;
    cProc.TimerPoll = (CTask::PFTimerPoll)&CQueryTask::QueryTimerPoll;
    AddRuleProc(enStateQuery, cProc);
    cProc.ProcMsg = (CTask::PFProcMsg)&CQueryTask::OnTransRsp;
    cProc.ProcErrMsg = cProc.ProcMsg;
    cProc.TimerPoll = (CTask::PFTimerPoll)&CQueryTask::TransRspTimerPoll;
    AddRuleProc(enStateTransRsp, cProc);
    cProc.ProcMsg = (CTask::PFProcMsg)&CQueryTask::OnTransNtf;
    cProc.ProcErrMsg = cProc.ProcMsg;
    cProc.TimerPoll = (CTask::PFTimerPoll)&CQueryTask::TransNtfTimerPoll;
    AddRuleProc(enStateTransNtf, cProc);
}

u32 CQueryTask::OnIdle( CMessage *const pcMsg )
{
    if (Query(pcMsg))
    {
        DoNextState(enStateQuery);
        return PROCMSG_OK;
    }

    Log(WARNING_LEV, "CQueryTask::OnIdle delete\n");
    return PROCMSG_DEL;
}

u32 CQueryTask::OnQuery( CMessage *const pcMsg )
{
    if (msgTaskMsgTransfer4Db != pcMsg->event)
    {
        CMS_ASSERT(false);
        Log(WARNING_LEV, "CQueryTask::OnQuery delete(invalid message)\n");
        return PROCMSG_DEL;
    }

    DoNextState(enStateQuery); // 收到了查询应答，重新进入状态更新当前状态Holdtime

    ptrint ptr= *reinterpret_cast<ptrint*>(pcMsg->content);
    CTaskMsgTransferBase* pcTrans = reinterpret_cast<CTaskMsgTransferBase*>(ptr);
    
    bool bIsRsp = IsRspType(pcTrans->GetEventId());

    // 结果:Rsp和Ntf
    if (bIsRsp)
    {
        CMS_ASSERT(m_pcQryRsp == NULL);
        m_pcQryRsp = pcTrans;
        CQueryRspBase *pcBase = reinterpret_cast<CQueryRspBase*>(pcTrans->GetMsgObjAddr());
        *reinterpret_cast<ptrint*>(pcMsg->content) = static_cast<ptrint>(0); // 自己保存之后置空，这样CCtrl才不会Delete掉它

        Log(PROGRESS_LEV, "CQueryTask::OnQuery rsp's entryNum:%d, ntf num:%u\n", 
            pcBase->GetEntryNum(), m_cQryNtfLst.size());

        pcBase->SetQueryTag(m_strQueryTag);

        DoNextState(enStateTransRsp);
        if (DoTransfer(true))
        {
            if (pcBase->GetErrorCode() != CMS_SUCCESS || pcBase->GetEntryNum() == 0)
            {
                // 查询出错或者总数为0, 无需发送Ntf（直接删除）
                return PROCMSG_DEL;
            }
            
            DoNextState(enStateTransNtf);
            DoTransfer(false);
            return PROCMSG_OK;
        }
        else
        {
            Log(WARNING_LEV, "CQueryTask::OnQuery delete\n");
            return PROCMSG_DEL;
        }
    }
    else
    {
        CQueryNtfReqBase *pcBase = reinterpret_cast<CQueryNtfReqBase*>(pcTrans->GetMsgObjAddr());
        pcBase->SetQueryTag(m_strQueryTag);
        m_cQryNtfLst.push_back(pcTrans);
        *reinterpret_cast<ptrint*>(pcMsg->content) = static_cast<ptrint>(0); // 自己保存之后置空，这样CCtrl才不会Delete掉它
    }

    return PROCMSG_OK;
}

u32 CQueryTask::OnTransRsp( CMessage *const pcMsg )
{
    // 这个状态没有什么事情需要做
    CMS_ASSERT(false);
    Log(WARNING_LEV, "CQueryTask::OnTransRsp delete\n");
    return PROCMSG_DEL;
}

u32 CQueryTask::OnTransNtf( CMessage *const pcMsg )
{
    Log(PROGRESS_LEV, "CQueryTask::OnTransferNtf recv event[%u-%s]\n", 
        pcMsg->event, OspExtEventDesc(pcMsg->event).c_str());
    if (OSP_SIP_MSG_PROC_FAIL == pcMsg->event)
    {
        // SIP层处理出错简单返回fail
        return PROCMSG_FAIL;
    }

    // 收到NtfRsp消息, 判断序列号是否为队首消息的应答
    string strSeqNum = GetSipSeqNO(reinterpret_cast<COspSipMsg*>(pcMsg->content)->GetMsgBody());
    const CTaskMsgTransferBase *pcTrans = m_cQryNtfLst.front();
    
    if (reinterpret_cast<CEventReq*>(pcTrans->GetMsgObjAddr())->GetSeqNum()
        == atoi(strSeqNum.c_str()))
    {
        DoNextState(enStateTransNtf); // 收到了NtfReq的应答，重新进入状态更新当前状态Holdtime

        delete pcTrans;
        m_cQryNtfLst.pop_front();
        if (m_cQryNtfLst.empty())
        {
            Log(PROGRESS_LEV, "CQueryTask::OnTransferNtf finished transfer\n");
            return PROCMSG_DEL;
        }

        DoTransfer(false);
    }

    return PROCMSG_OK;
}

u32 CQueryTask::TransRspTimerPoll()
{
    if (GetCurStateHoldTime() < 15)
    {
        return TIMERPOLL_DONE;
    }
    else
    {
        Log(WARNING_LEV, "TransRsp timeout");
        return TIMERPOLL_DEL;
    }

    // 禁掉查询任务的重发功能
    //if (++m_dwDelayTimes > 3)
    //{
    //    return TIMERPOLL_DEL; // 累计3次无法传输QryRsp就删除任务
    //}

    //if (DoTransfer(true)) // 重发
    //{
    //    CDbQueryResultBase* pcBase = reinterpret_cast<CDbQueryResultBase*>(m_pcQryRsp);
    //    if (pcBase->GetErrorCode() != CMS_SUCCESS || pcBase->GetTotalEntryNum() == 0)
    //    {
    //        return TIMERPOLL_DEL;
    //    }

    //    DoNextState(enStateTransNtf);
    //    DoTransfer(false);
    //}
    //return TIMERPOLL_DONE;
}

u32 CQueryTask::TransNtfTimerPoll()
{
    if (GetCurStateHoldTime() < 60*2) 
    {
        return TIMERPOLL_DONE;
    }
    else
    {
        CQueryRspBase* pcResult = reinterpret_cast<CQueryRspBase*>(m_pcQryRsp->GetMsgObjAddr());
        Log(WARNING_LEV, "TransNtf timeout, Rsp EntryNum:%u, leave Ntf Num:%u\n", (u32)pcResult->GetEntryNum(), (u32)m_cQryNtfLst.size());
        return TIMERPOLL_DEL;
    }

    // 禁掉查询任务的重发功能
    //if (++m_dwDelayTimes > 60*2)
    //{
    //    return TIMERPOLL_DEL; // 累计120次传输应答不返回就删除任务
    //}
    //if (GetPollCounter() < 5)
    //{
    //    return TIMERPOLL_DONE; // 轮循5次才尝试重发
    //}
    //else
    //{
    //    ClearPollCounter();
    //}

    //DoTransfer(false); // 重发
    //return TIMERPOLL_DONE; 
}

u32 CQueryTask::QueryTimerPoll( )
{
    return (GetCurStateHoldTime() > 60*5)? TIMERPOLL_DEL: TIMERPOLL_DONE; // 累计300秒数据库查询不返回就删除任务
}

bool CQueryTask::DoTransfer(bool bRsp)
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
       
		// 丑陋的特殊处理，过滤掉设备组名里面的国标编码 [10/16/2015 pangubing]
		if (TAS_DEVICE_GROUP_QRY_NTF_REQ==pcTrans->GetEventId()&&!m_pcSsnTask->GetIsNeedDevGrpGbId())
		{
			Log(EVENT_LEV, "CQueryTask m_tSipUri[%s] need cut group gb id.\n",m_tSipUri.GetURIString().c_str());
			 CDeviceGroupQryNtfReq* pNtfReq = reinterpret_cast<CDeviceGroupQryNtfReq*>(pcTrans->GetMsgObjAddr());
			 vector<CDeviceGroupInfo> &infos=pNtfReq->GetDeviceGroupInfo();
			 vector<CDeviceGroupInfo>::iterator itr=infos.begin();
			 for (;itr!=infos.end();itr++)
			 {
				 itr->GetDeviceGroupName()=CutDevGrpGbId(itr->GetDeviceGroupName());
			 }

			strMsg = pNtfReq->ToXml();
		}
		else
		{
			 strMsg = reinterpret_cast<CSipEvent*>(pcTrans->GetMsgObjAddr())->ToXml();
		}

        bRet = (PROCMSG_OK == PostMsgReq(strMsg, m_tSipUri));
    }

    if (!bRet)
    {
        Log(WARNING_LEV, "CQueryTask[%s-%u] send message failed, queryTag:%s\n", 
            GetObjName(), GetTaskNO(), m_strQueryTag.c_str());
    }
    else
    {
        CQueryRspBase *pcRsp = static_cast<CQueryRspBase*>(m_pcQryRsp->GetMsgObjAddr());
        Log(PROGRESS_LEV, "CQueryTask[%s-%u] send message success, queryTag:%s, bRsp:%s, entryNum:%d\n", 
            GetObjName(), GetTaskNO(), m_strQueryTag.c_str(), bRsp? "yes":"no", pcRsp->GetEntryNum());
    }

    return bRet;
}

void CQueryTask::DoNextState( u32 enState )
{
    NextState(enState);
}

#define DECODE_SIP_AND_POST2DbTaskPool(msgid, msgtype) \
    case msgid:\
{\
    CTaskMsgTransfer<msgtype> cTrans(GetTaskNO(), pcSipMsg->GetSipTransID(), pcMsg->event);\
    pcSipMsg->GetMsgBody(cTrans.GetMsgObj());\
    m_strQueryTag = static_cast<CQueryReqBase*>(cTrans.GetMsgObjAddr())->GetQueryTag(); \
    m_strQueryReq = static_cast<CQueryReqBase*>(cTrans.GetMsgObjAddr())->ToXml(); \
    bRet = PostTaskMsgTransfer2DbTaskPool(cTrans);\
}break

bool CQueryTask::Query( CMessage *const pcMsg )
{
    bool bRet = false;
    COspSipMsg* const pcSipMsg = reinterpret_cast<COspSipMsg* const>(pcMsg->content);
    switch(pcMsg->event)
    {
        DECODE_SIP_AND_POST2DbTaskPool(TAS_DEVICE_QRY_REQ, CDeviceQryReq);
        DECODE_SIP_AND_POST2DbTaskPool(TAS_TVWALL_QRY_REQ, CTvwallQryReq);
        DECODE_SIP_AND_POST2DbTaskPool(TAS_DEVICE_MODEL_QRY_REQ, CDeviceModelQryReq);
        DECODE_SIP_AND_POST2DbTaskPool(TAS_DEVICE_CAP_INDEX_NAME_QRY_REQ, CDeviceCapIndexNameQryReq);
        DECODE_SIP_AND_POST2DbTaskPool(TAS_DEVICE_MANUFACTURER_QRY_REQ, CDeviceManufacturerQryReq);
        DECODE_SIP_AND_POST2DbTaskPool(TAS_CUSER_DEVICEFULLINFO_AND_PRIVILEGE_QRY_REQ, CCuserDevicefullinfoAndPrivilegeQryReq);
        DECODE_SIP_AND_POST2DbTaskPool(TAS_CUSER_QRY_REQ, CUserQryReq);
        DECODE_SIP_AND_POST2DbTaskPool(TAS_CUSER_DEVICE_QRY_REQ, CUserDeviceQryReq);
        DECODE_SIP_AND_POST2DbTaskPool(TAS_DEVICE_GROUP_QRY_REQ, CDeviceGroupQryReq);
        DECODE_SIP_AND_POST2DbTaskPool(TAS_DEVICE_GROUP_DEVICE_QRY_REQ, CDeviceGroupDeviceQryReq);
        DECODE_SIP_AND_POST2DbTaskPool(TAS_CUSER_GROUP_QRY_REQ, CUserGroupQryReq);
        DECODE_SIP_AND_POST2DbTaskPool(TAS_CUSER_GROUP_DEVICE_QRY_REQ, CUserGroupDeviceQryReq);
        DECODE_SIP_AND_POST2DbTaskPool(TAS_DOMAIN_RELATION_QRY_REQ, CDomainRelationQryReq);
		DECODE_SIP_AND_POST2DbTaskPool(TAS_DEVICE_AUTO_ASSIGN_TO_USER_QRY_REQ, CDeviceAutoAssignToUserQryReq);
		DECODE_SIP_AND_POST2DbTaskPool(TAS_DEVICE_AUTO_ASSIGN_TO_USER_GROUP_QRY_REQ, CDeviceAutoAssignToUserGroupQryReq);
		DECODE_SIP_AND_POST2DbTaskPool(TAS_DEVTREE_QRY_REQ, CDevTreeQryReq);
		DECODE_SIP_AND_POST2DbTaskPool(TAS_DEVICE_GROUPPATH_QRY_REQ, CDeviceGrpPathQryReq);
		DECODE_SIP_AND_POST2DbTaskPool(TAS_PTZLOCK_QRY_REQ, CPtzLockQryReq);
		DECODE_SIP_AND_POST2DbTaskPool(TAS_VIDSRC_INFO_QRY_REQ, CVidsrcInfoQryReq);
		DECODE_SIP_AND_POST2DbTaskPool(TAS_DEVICE_GROUP_DEVICE_QRY_4GB_REQ,CDeviceGroupDeviceQry4GbReq);//设备查询4gb
		DECODE_SIP_AND_POST2DbTaskPool(TAS_DEVGRP_GBINFO_QRY_REQ,CDevgrpGbinfoQryReq);//根据gbid查询分组信息
		DECODE_SIP_AND_POST2DbTaskPool(TAS_DEVICE_GBINFO_QRY_REQ,CDeviceGbinfoQryReq);//根据gbid查询设备信息
    default:
        {
            Log(ERROR_LEV, "CQueryTask::Query recv unknow message, [%u-%s]\n", static_cast<u32>(pcMsg->event),
                OspExtEventDesc(pcMsg->event).c_str());
        }
        break;
    }
    if (bRet)
    {
        m_strObjName.append("[");
        m_strObjName.append(OspExtEventDesc(pcMsg->event));
        m_strObjName.append("]");
    }

    return bRet;
}

void CQueryTask::PrintData() const
{
    CSipTask::PrintData();

    OspPrintf(TRUE, FALSE, "m_cQryNtfLst size:%d, m_strQueryTag:%s, m_strQueryReq:%s, m_tSipUri:%s\n",
        (s32)m_cQryNtfLst.size(), m_strQueryTag.c_str(), m_strQueryReq.c_str(), 
        m_tSipUri.GetURIString().c_str());
}
