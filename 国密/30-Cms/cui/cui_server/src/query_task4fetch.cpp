#include "query_task4fetch.h"

CQueryTask4Fetch::CQueryTask4Fetch( CInstExt *pcInst, CCuAgent *pcCuAgent, PTRecvNode ptRecvNode )
    :CSipTask(pcInst), m_pcCuAgent(pcCuAgent), m_ptRecvNode(ptRecvNode), m_strQueryTag(CUUID()), m_nNtfNumPerCuRsp(10)
{
    m_bNtfRecvEnded = false;
    m_pcCuAgent->AddSubTask(this);
    m_nErrorCode = CMS_SUCCESS;

    g_cCuiDataMgr.m_mapSessToTaskMap[m_strQueryTag] = GetTaskNO();
}

CQueryTask4Fetch::~CQueryTask4Fetch()
{
    m_pcCuAgent->DelSubTask(this);

    g_cCuiDataMgr.m_mapSessToTaskMap.Erase(m_strQueryTag);
}

void CQueryTask4Fetch::InitStateMachine()
{
    CStateProc cProc;
    cProc.ProcMsg = (CSipTask::PFProcMsg)&CQueryTask4Fetch::IdleProcMsg;
    cProc.TimerPoll = (CSipTask::PFTimerPoll)&CQueryTask4Fetch::IdleTimerPoll;
    cProc.ProcErrMsg = (CSipTask::PFProcMsg)&CQueryTask4Fetch::IdleProcMsg;
    AddRuleProc(IDLE_STATE, cProc);

    cProc.ProcMsg = (CSipTask::PFProcMsg)&CQueryTask4Fetch::ServerProcMsg;
    cProc.TimerPoll = (CSipTask::PFTimerPoll)&CQueryTask4Fetch::ServerTimerPoll;
    cProc.ProcErrMsg = (CSipTask::PFProcMsg)&CQueryTask4Fetch::ServerProcMsg;
    AddRuleProc(SERVER_STATE, cProc);

    NextState(IDLE_STATE);
}

u32 CQueryTask4Fetch::IdleProcMsg( CMessage *const pcMsg )
{
    if (m_strReqMsg.empty())
    {
        m_wReqMsg = pcMsg->event;
        m_strReqMsg = OspExtEventDesc(m_wReqMsg);
    }

    switch(pcMsg->event)
    {
    case CU_CUI_GET_USER_VIDEOSRC_LIST_REQ:
        {
            CCuUserDevGrpVidsrcQryReq *pcReq = (CCuUserDevGrpVidsrcQryReq *)pcMsg->content;
            CUserDevGrpVidsrcQryReq cReq = *pcReq;
            cReq.SetEvent(TAS_CUSER_DEVGRP_VIDSRC_QRY_REQ);
            cReq.SetQueryTag(m_strQueryTag);
            if (cReq.GetCuserId().empty())
            {
                cReq.SetCuserId(m_pcCuAgent->m_pcUserAccount->m_tUserID);
            }

            if (m_pcCuAgent->IsTasOnline())
            {
                C3acAgent *pc3acAgent = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(
                    m_pcCuAgent->m_str3asName, GetInstance());
                PostMsgReq(cReq.ToXml(), pc3acAgent->m_t3asURI);
                NextState(SERVER_STATE);
            }
            else
            {
                TASKLOG(WARNING_LEV, "tas is offline.\n");
                return PROCMSG_DEL;
            }
        }
        break;
    default:
        break;
    }

    return PROCMSG_OK;
}

u32 CQueryTask4Fetch::IdleTimerPoll()
{
    return TIMERPOLL_DONE;
}

u32 CQueryTask4Fetch::ServerProcMsg( CMessage *const pcMsg )
{
    switch(pcMsg->event)
    {
    case TAS_CUSER_DEVGRP_VIDSRC_QRY_RSP:
        {
            CUserDevGrpVidsrcQryRsp cRsp;
            COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
            pcOspSipMsg->GetMsgBody(cRsp);

            m_nErrorCode = cRsp.GetErrorCode();
            if (!cRsp.GetHasRecords() || m_nErrorCode != CMS_SUCCESS)
            {
                m_bNtfRecvEnded = true;

                PostRsp2Cu(m_ptRecvNode);
                m_ptRecvNode = NULL;

                delete this;
                return PROCMSG_DEL;
            }
        }
        break;
    case TAS_CUSER_DEVGRP_VIDSRC_QRY_NTF_REQ:
        {
            NextState(SERVER_STATE);

            CUserDevGrpVidsrcQryNtfReq cReq;
            COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
            pcOspSipMsg->GetMsgBody(cReq);

            CNtfRsp cRsp;
            cRsp.SetEvent(cReq.GetEventId()+1);
            cRsp.SetSeqNum(cReq.GetSeqNum());
            cRsp.SetSession(cReq.GetSession());
            PostMsgRsp(pcOspSipMsg->GetSipTransID(), cRsp);

            if (cReq.GetLastNtf())
            {
                m_bNtfRecvEnded = true;
            }

            m_vecNtfReqList.push_back(cReq.ToXml());
            if (m_ptRecvNode != NULL && (m_bNtfRecvEnded || m_vecNtfReqList.size() >= m_nNtfNumPerCuRsp))
            {
                if (PostRsp2Cu(m_ptRecvNode))
                {
                    m_ptRecvNode = NULL;
                }
                else
                {
                    delete this;
                    return PROCMSG_DEL;
                }
            }
        }
        break;
    }

    return PROCMSG_OK;
}

u32 CQueryTask4Fetch::ServerTimerPoll()
{
    if (GetCurStateHoldTime() > g_cCuiConfig.GetDbQueryTimeout())
    {
        TASKLOG(WARNING_LEV, "CQueryTask4Fetch timeout, m_strReqMsg=%s, m_vecNtfReqList size:%d, m_bNtfRecvEnded:%d.\n", 
            m_strReqMsg.c_str(), (s32)m_vecNtfReqList.size(), (s32)m_bNtfRecvEnded);
        return TIMERPOLL_DEL;
    }

    return TIMERPOLL_DONE;
}

bool CQueryTask4Fetch::PostRsp2Cu( PTRecvNode ptRecvNode )
{
    switch(m_wReqMsg)
    {
    case CU_CUI_GET_USER_VIDEOSRC_LIST_REQ:
        {
            CCuUserDevGrpVidsrcQryRsp cRsp;
            cRsp.SetErrorCode(m_nErrorCode);
            cRsp.SetQueryTag(m_strQueryTag);

            C3acAgent *pc3acAgent;
            if (m_pcCuAgent->IsTasOnline())
            {
                pc3acAgent = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(
                    m_pcCuAgent->m_str3asName, GetInstance());
            }
            else
            {
                TASKLOG(WARNING_LEV, "tas is offline.\n");
                return false;
            }

            CUserDevGrpVidsrcQryNtfReq cNtfReq;
            s32 nNtfCount = 0;
            for (std::list<string>::iterator it = m_vecNtfReqList.begin(); nNtfCount<m_nNtfNumPerCuRsp && it != m_vecNtfReqList.end();)
            {
                ++nNtfCount;

                cNtfReq.FromXml(*it);
                for (vector<CUserDevGrpVidSrcInfo>::const_iterator it2 = cNtfReq.GetInfos().begin();
                    it2 != cNtfReq.GetInfos().end(); ++it2)
                {
                    cRsp.GetInfos().push_back(*it2);
                }

                m_vecNtfReqList.erase(it++);
            }
            for (vector<CUserDevGrpVidSrcInfo>::iterator it = cRsp.GetInfos().begin();
                it != cRsp.GetInfos().end(); ++it)
            {
                it->SetDevDomainID(pc3acAgent->GetDomainNameByDomainID(it->GetDevDomainID()));
                if (it->GetDevDomainID().empty())
                {
                    TASKLOG(WARNING_LEV, "not found domainId's domainName.\n");
                    return false;
                }
            }
            cRsp.SetLastNtf(m_bNtfRecvEnded && m_vecNtfReqList.empty());

            string strRsp = cRsp.ToXml();
            if (!PostMsgToRecvThread(ptRecvNode, CUI_CU_WEBSERVICE_RSP, strRsp.c_str(), strRsp.length()))
            {
                TASKLOG(WARNING_LEV, "CQueryTask4Fetch::PostRsp2Cu PostMsgToRecvThread failed. m_strReqMsg=%s\n", m_strReqMsg.c_str());
                return false;
            }

            if (cRsp.GetLastNtf())
            {
                return false;
            }
        }
        break;
    default:
        TASKLOG(ERROR_LEV, "CQueryTask4Fetch::PostRsp2Cu assert.\n");
        break;
    }
    
    return true;
}



bool CuFetchQueryTask( const string &QueryTag, PTRecvNode ptRecvNode, const CInstExt *pcInst )
{
    CSipTask *pcTask = g_cCuiDataMgr.GetTaskPtrBySession(QueryTag, pcInst);
    if (NULL != pcTask)
    {
        if (!(dynamic_cast<CQueryTask4Fetch*>(pcTask)->PostRsp2Cu(ptRecvNode)))
            delete pcTask;
        return true;
    }
    else
    {
        return false;
    }
}
