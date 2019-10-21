#include "tas_config_task.h"
#include "cctrl.h"
#include "common/ssn_taskbase.h"
#include "parent_tas_conn_task.h"
#include "sync_analyzer_mng.h"

CTasConfigTask::CTasConfigTask( CInstExt *pcInst )
    : CSipTask(pcInst)
{
	m_tDelChildTasDlgId = INVALID_DIALOG_ID;
	m_bDelChildTasHasRsped = false;
}

CTasConfigTask::~CTasConfigTask()
{

}

const char* CTasConfigTask::GetStateName( u32 dwState ) const
{
    switch(dwState)
    {
    case enStateIdle:
        return "IdleState";
    case enStateConfigQuery:
        return "ConfigQueryState";
    case enStateConfigDomainName:
        return "ConfigDomainNameState";
    case enStateConfigParentDomain:
        return "ConfigParentDomainState";
    case enStateConfigChildDomainAdd:
        return "ConfigChildDomainAddState";
    case enStateConfigChildDomainDel:
        return "ConfigChildDomainDelState";
    default:
        return "UnknowState";
    }
}

void CTasConfigTask::InitStateMachine()
{
    NextState(enStateIdle);

    CStateProc cProc;
    cProc.TimerPoll = &CTask::TimerPollSkip;

    cProc.ProcMsg = (CTask::PFProcMsg)&CTasConfigTask::OnIdle;
    cProc.ProcErrMsg = cProc.ProcMsg;
    AddRuleProc(enStateIdle, cProc);

    cProc.ProcMsg = (CTask::PFProcMsg)&CTasConfigTask::OnConfigQuery;
    cProc.ProcErrMsg = cProc.ProcMsg;
    AddRuleProc(enStateConfigQuery, cProc);

    cProc.ProcMsg = (CTask::PFProcMsg)&CTasConfigTask::OnConfigDomainName;
    cProc.ProcErrMsg = cProc.ProcMsg;
    AddRuleProc(enStateConfigDomainName, cProc);

    cProc.ProcMsg = (CTask::PFProcMsg)&CTasConfigTask::OnConfigParentDomain;
    cProc.ProcErrMsg = cProc.ProcMsg;
    AddRuleProc(enStateConfigParentDomain, cProc);

    cProc.ProcMsg = (CTask::PFProcMsg)&CTasConfigTask::OnConfigChildDomainAdd;
    cProc.ProcErrMsg = cProc.ProcMsg;
    AddRuleProc(enStateConfigChildDomainAdd, cProc);

    cProc.ProcMsg = (CTask::PFProcMsg)&CTasConfigTask::OnConfigChildDomainDel;
    cProc.ProcErrMsg = cProc.ProcMsg;
    AddRuleProc(enStateConfigChildDomainDel, cProc);
}

u32 CTasConfigTask::OnIdle( CMessage *const pcMsg )
{
    COspSipMsg* const pcSipMsg = reinterpret_cast<COspSipMsg* const>(pcMsg->content);

    if (TAS_CONFIG_TAS_GET_REQ == pcMsg->event)
    {
        NextState(enStateConfigQuery);
        CTaskMsgTransfer<CConfigTasGetReq> cTrans(GetTaskNO(), pcSipMsg->GetSipTransID(), pcMsg->event);
        pcSipMsg->GetMsgBody(cTrans.GetMsgObj());

        if (!PostTaskMsgTransfer2DbTaskPool(cTrans)) return PROCMSG_DEL;
    }
    else if (TAS_CONFIG_TAS_SET_REQ == pcMsg->event)
    {
        CConfigTasSetReq cReq;
        pcSipMsg->GetMsgBody(cReq);
        
        string strNewDomainName;
		TDomainExtData tNewDomainAlias;
        string strParentDomainId;
        if (cReq.GetDomainName(strNewDomainName) && cReq.GetParentDomainId(strParentDomainId))
        {
            Log(CRITICAL_LEV, "config the two items in CConfigTasSetReq at the same");
            return PROCMSG_DEL;
        }

        if (cReq.GetDomainName(strNewDomainName) || cReq.GetDomainAlias(tNewDomainAlias))
        {
            NextState(enStateConfigDomainName);

            CTaskMsgTransfer<CConfigTasSetReq> cTrans(GetTaskNO(), pcSipMsg->GetSipTransID(), pcMsg->event);
            cTrans.GetMsgObj() = cReq;

            if (!PostTaskMsgTransfer2DbTaskPool(cTrans)) return PROCMSG_DEL;
        }
        else if (cReq.GetParentDomainId(strParentDomainId))
        {
            NextState(enStateConfigParentDomain);

            cReq.GetParentDomainId(m_tParentDomainId);
            cReq.GetParentDomainName(m_strParentDomainName);

            CTaskMsgTransfer<CConfigTasSetReq> cTrans(GetTaskNO(), pcSipMsg->GetSipTransID(), pcMsg->event);
            cTrans.GetMsgObj() = cReq;

            if (PostTaskMsgTransfer2DbTaskPool(cTrans))
            {
                // 先删除老的父域连接任务
                // (删除上级域时本来就需要删除，添加新上级时必须先删除老的，故此处统一删除)
                DelOldParentTasConnTask();
            }
            else
            {
                return PROCMSG_DEL;
            }
        }
        else
        {
            Log(CRITICAL_LEV, "no item is configed in CConfigTasSetReq");
            return PROCMSG_DEL;
        }
    }
    else if (TAS_CHILDTAS_ADD_REQ == pcMsg->event)
    {
        NextState(enStateConfigChildDomainAdd);

        CTaskMsgTransfer<CChildtasAddReq> cTrans(GetTaskNO(), pcSipMsg->GetSipTransID(), pcMsg->event);
        pcSipMsg->GetMsgBody(cTrans.GetMsgObj());

        m_tSubDomainId = cTrans.GetMsgObj().GetChildDomainId();

        if (!PostTaskMsgTransfer2DbTaskPool(cTrans)) return PROCMSG_DEL;
    }
    else if (TAS_CHILDTAS_DEL_REQ == pcMsg->event)
    {
        NextState(enStateConfigChildDomainDel);

        CTaskMsgTransfer<CChildtasDelReq> cTrans(GetTaskNO(), pcSipMsg->GetSipTransID(), pcMsg->event);
        pcSipMsg->GetMsgBody(cTrans.GetMsgObj());

        m_tSubDomainId = cTrans.GetMsgObj().GetChildDomainId();

        if (!PostTaskMsgTransfer2DbTaskPool(cTrans)) return PROCMSG_DEL;
    }
    else
    {
        CMS_ASSERT(false && "CTasConfigTask::OnIdle recved invalid msg");
        return PROCMSG_FAIL;
    }

    return PROCMSG_OK;
}

u32 CTasConfigTask::OnConfigQuery( CMessage *const pcMsg )
{
    if (msgTaskMsgTransfer4Db != pcMsg->event)
        return PROCMSG_FAIL;

    ptrint ptr = *reinterpret_cast<ptrint*>(pcMsg->content);
    const CTaskMsgTransferBase* pcMsgTrans = reinterpret_cast<const CTaskMsgTransferBase*>(ptr);

    if (TAS_CONFIG_TAS_GET_RSP == pcMsgTrans->GetEventId())
    {
        const CConfigTasGetRsp *pcRsp = static_cast<const CConfigTasGetRsp*>(pcMsgTrans->GetMsgObjAddr());
        PostMsgRsp(pcMsgTrans->GetSipTransId(), *pcRsp);
        return PROCMSG_DEL;
    }
    else
    {
        Log(CRITICAL_LEV, "CTasConfigTask::OnConfigQuery recved invalid msg, msgid=%u\n", pcMsgTrans->GetEventId());
        return PROCMSG_FAIL;
    }
}

u32 CTasConfigTask::OnConfigDomainName( CMessage *const pcMsg )
{
    if (msgTaskMsgTransfer4Db != pcMsg->event)
        return PROCMSG_FAIL;

    ptrint ptr = *reinterpret_cast<ptrint*>(pcMsg->content);
    const CTaskMsgTransferBase* pcMsgTrans = reinterpret_cast<const CTaskMsgTransferBase*>(ptr);
    if (TAS_DOMAIN_RELATION_MOD_NTF_REQ == pcMsgTrans->GetEventId())
    {
        const CDomainRelationModNtfReq* pcModNtfReq = static_cast<const CDomainRelationModNtfReq*>(pcMsgTrans->GetMsgObjAddr());
        static_cast<CCtrlInst*>(GetInstance())->GetAppData().HandleChangeNtfReq(TAS_DOMAIN_RELATION_MOD_NTF_REQ, pcModNtfReq->ToXml());

        UpdateCurrentDomainName(pcModNtfReq->GetDomainRelation().GetDomainName());

        return PROCMSG_OK;
    }
    else if (TAS_CONFIG_TAS_SET_RSP == pcMsgTrans->GetEventId())
    {
        const CConfigTasSetRsp *pcRsp = static_cast<const CConfigTasSetRsp*>(pcMsgTrans->GetMsgObjAddr());
        PostMsgRsp(pcMsgTrans->GetSipTransId(), *pcRsp);
        return PROCMSG_DEL; // 回复应答后tas还需要一些时间动态通知其它模块域关系变更，tas不能立即被kill
    }
    else
    {
        Log(CRITICAL_LEV, "CTasConfigTask::OnConfigDomainName recved invalid msg, msgid=%u\n", pcMsgTrans->GetEventId());
        return PROCMSG_FAIL;
    }
}

u32 CTasConfigTask::OnConfigParentDomain( CMessage *const pcMsg )
{
    if (msgTaskMsgTransfer4Db != pcMsg->event)
        return PROCMSG_FAIL;

    ptrint ptr = *reinterpret_cast<ptrint*>(pcMsg->content);
    const CTaskMsgTransferBase* pcMsgTrans = reinterpret_cast<const CTaskMsgTransferBase*>(ptr);

    if (TAS_DOMAIN_RELATION_MOD_NTF_REQ == pcMsgTrans->GetEventId())
    {
        static_cast<CCtrlInst*>(GetInstance())->GetAppData().HandleChangeNtfReq(
            TAS_DOMAIN_RELATION_MOD_NTF_REQ, 
            static_cast<const CDomainRelationModNtfReq*>(pcMsgTrans->GetMsgObjAddr())->ToXml());

        return PROCMSG_OK;
    }
    else if (TAS_CONFIG_TAS_SET_RSP == pcMsgTrans->GetEventId())
    {
        const CConfigTasSetRsp *pcRsp = static_cast<const CConfigTasSetRsp*>(pcMsgTrans->GetMsgObjAddr());
        PostMsgRsp(pcMsgTrans->GetSipTransId(), *pcRsp);

        if (pcRsp->GetErrorCode() == CMS_SUCCESS)
        {
            // 添加新的父域连接任务
            if (!m_tParentDomainId.empty())
            {
                AddNewParentTasConnTask();
            }
        }
        else
        {
            // 恢复老的父域连接任务
            if (!m_tOldParentDomainId.empty())
            {
                ResumeOldParentTasConnTask();
            }
        }

        return PROCMSG_DEL;
    }
    else
    {
        Log(CRITICAL_LEV, "CTasConfigTask::OnConfigParentDomain recved invalid msg, msgid=%u\n", pcMsgTrans->GetEventId());
        return PROCMSG_FAIL;
    }
}

u32 CTasConfigTask::OnConfigChildDomainAdd( CMessage *const pcMsg )
{
    if (msgTaskMsgTransfer4Db != pcMsg->event)
        return PROCMSG_FAIL;

    ptrint ptr = *reinterpret_cast<ptrint*>(pcMsg->content);
    const CTaskMsgTransferBase* pcMsgTrans = reinterpret_cast<const CTaskMsgTransferBase*>(ptr);

    if (TAS_CHILDTAS_ADD_RSP == pcMsgTrans->GetEventId())
    {
        const CChildtasAddRsp *pcRsp = static_cast<const CChildtasAddRsp*>(pcMsgTrans->GetMsgObjAddr());

        if (pcRsp->GetErrorCode() == CMS_SUCCESS)
        {
            OspPost(MAKEIID(AID_SyncAnalyze, CInstance::DAEMON), msgSyncMngSubdomainAddNtf, 
                m_tSubDomainId.c_str(), strlen(m_tSubDomainId.c_str())+1);
        }

        PostMsgRsp(pcMsgTrans->GetSipTransId(), *pcRsp);
        return PROCMSG_DEL;
    }
    else if (TAS_DOMAIN_RELATION_ADD_NTF_REQ == pcMsgTrans->GetEventId())
    {
        const CDomainRelationAddNtfReq *pcNtfReq = static_cast<const CDomainRelationAddNtfReq*>(pcMsgTrans->GetMsgObjAddr());
        static_cast<CCtrlInst*>(GetInstance())->GetAppData().HandleChangeNtfReq(pcNtfReq->GetEventId(), pcNtfReq->ToXml());
        return PROCMSG_OK;
    }
    else
    {
        Log(CRITICAL_LEV, "CTasConfigTask::OnConfigChildDomainAdd recved invalid msg, msgid=%u\n", pcMsgTrans->GetEventId());
        return PROCMSG_FAIL;
    }
}

u32 CTasConfigTask::OnConfigChildDomainDel( CMessage *const pcMsg )
{
    // 下级配置上级时需要重启, 此处只需要简单回复应答即可
	if (msgTaskMsgTransfer4Db == pcMsg->event)
	{
		ptrint ptr = *reinterpret_cast<ptrint*>(pcMsg->content);
		const CTaskMsgTransferBase* pcMsgTrans = reinterpret_cast<const CTaskMsgTransferBase*>(ptr);

		if (TAS_CHILDTAS_DEL_RSP == pcMsgTrans->GetEventId())
		{
			const CChildtasDelRsp *pcRsp = static_cast<const CChildtasDelRsp*>(pcMsgTrans->GetMsgObjAddr());

			if (pcRsp->GetErrorCode() != CMS_SUCCESS )
			{
				Log(ERROR_LEV, "CTasConfigTask[%d]::OnConfigChildDomainDel[%s] recv Rsp with errCode[%d] \n",
					GetTaskNO(), m_tSubDomainId.c_str(), pcRsp->GetErrorCode());
			}
			
			if(!m_bDelChildTasHasRsped)
			{
				PostRsp(KDSIP_EVENT_INVITE_RSP, pcMsgTrans->GetSipTransId(), *pcRsp, KDSIP_RESPONSE_200_OK, NULL);
				m_bDelChildTasHasRsped = true;
			}
			
			if (pcRsp->GetErrorCode() != CMS_SUCCESS)
			{
				PostInDlgReq(KDSIP_EVENT_INVITE_BYE_REQ,string(),m_tDelChildTasDlgId,NULL);
				return PROCMSG_DEL;
			}

			return PROCMSG_OK;
		}
		else if (TAS_DOMAIN_RELATION_DEL_NTF_REQ == pcMsgTrans->GetEventId())
		{
			const CDomainRelationDelNtfReq *pcNtfReq = static_cast<const CDomainRelationDelNtfReq*>(pcMsgTrans->GetMsgObjAddr());
			static_cast<CCtrlInst*>(GetInstance())->GetAppData().HandleChangeNtfReq(pcNtfReq->GetEventId(), pcNtfReq->ToXml());
			if (pcNtfReq->GetDomainId() == m_tSubDomainId)
			{
				OspPost(MAKEIID(AID_SyncAnalyze, CInstance::DAEMON), msgSyncMngSubdomainDelNtf, 
					m_tSubDomainId.c_str(), strlen(m_tSubDomainId.c_str())+1);

				PostInDlgReq(KDSIP_EVENT_INVITE_BYE_REQ,string(),m_tDelChildTasDlgId,NULL);
				return PROCMSG_DEL;
			}
			else
			{
				return PROCMSG_OK;
			}
		}
		else
		{
			Log(CRITICAL_LEV, "CTasConfigTask::OnConfigChildDomainDel recved invalid msg, msgid=%u\n", pcMsgTrans->GetEventId());
			return PROCMSG_FAIL;
		}
	}
	else if (TAS_CHILDTAS_DEL_ACK == pcMsg->event)
	{
		COspSipMsg* const pcSipMsg = reinterpret_cast<COspSipMsg* const>(pcMsg->content);
		m_tDelChildTasDlgId = pcSipMsg->GetSipDlgID();


		return PROCMSG_OK;
	}
	else
	{
		Log(ERROR_LEV, "CTasConfigTask[%d]::OnConfigChildDomainDel recv unknown msg[%d-%s] \n",
			GetTaskNO(), pcMsg->event, OspExtEventDesc(pcMsg->event).c_str());
		return PROCMSG_FAIL;
	}
}

void CTasConfigTask::DelOldParentTasConnTask()
{
    set<CConnTaskbase*> cConns = g_cCtrlApp.GetAppData().GetTasConnTaskSet();

    for (set<CConnTaskbase*>::const_iterator it = cConns.begin(); it != cConns.end(); ++it)
    {
        CConnTaskbase* pcConn = *it;
        if (0 == strcmp(pcConn->GetObjName(), ParentTasConnTaskName))
        {
            m_tOldParentDomainId = pcConn->GetPeerDomainId();
            m_strOldParentDomainName = pcConn->GetPeerUri().GetDomain();

            delete pcConn;
        }
    }

    // 同时删除父域同步分析器
    DelParentAnalyzer();
}

void CTasConfigTask::AddNewParentTasConnTask()
{
    CMS_ASSERT(!m_tParentDomainId.empty() && !m_strParentDomainName.empty());

    CParentTasConnTask *pcTask = new CParentTasConnTask(GetInstance(), m_tParentDomainId, m_strParentDomainName);
    pcTask->InitStateMachine();

    AddParentAnalyzer(m_tParentDomainId);
}

void CTasConfigTask::ResumeOldParentTasConnTask()
{
    CMS_ASSERT(!m_tOldParentDomainId.empty() && !m_strOldParentDomainName.empty());

    CParentTasConnTask *pcTask = new CParentTasConnTask(GetInstance(), m_tOldParentDomainId, m_strOldParentDomainName);
    pcTask->InitStateMachine();

    AddParentAnalyzer(m_tOldParentDomainId);
}
