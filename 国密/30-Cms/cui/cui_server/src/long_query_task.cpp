#include "long_query_task.h"
#include "culogintask.h"
#include "cuiproto.h"
#include "normalcutask.h"
#include "cuiapp.h"
#include "cms/commonconfig/cmcfg.h"
#include "cms/utility/md5.h"
#include "cms/utility/flow_limit_counter.h"
#include "redisdb.h"
#include "sdk/redissdk/redissdk.h"
#include "StreamDecentrationAdapter.h"

extern CRedisDBInterface g_cRedisDb;
extern CRedisSdk         g_cCuiRedisSdk;

CLongQueryTask::CLongQueryTask( CInstExt *pcInst, CCuAgent *pcCuAgent, PTRecvNode ptRecvNode ) : CSipTask(pcInst)
{
    CMS_ASSERT(pcCuAgent != NULL);
    CMS_ASSERT(ptRecvNode != NULL);
    m_pcCuAgent = pcCuAgent;
    m_ptRecvNode = ptRecvNode;
    m_pRsp = NULL;
    m_pc3acAgent = NULL;
    m_dwReqEvent = 0;
    m_pcCuAgent->AddSubTask(this);
}

CLongQueryTask::~CLongQueryTask()
{
    TerminateTask();
    if ( ! m_strQueryTag.empty() )
    {
        g_cCuiDataMgr.m_mapSessToTaskMap.Erase(m_strQueryTag);
        m_strQueryTag.clear();
    }

    if ( NULL != m_pRsp )
    {
        delete m_pRsp;
        m_pRsp = NULL;
    }
}

void CLongQueryTask::TerminateTask()
{
    m_pcCuAgent->DelSubTask(this);
    while(!m_VecNtfReqList.empty())
    { 
        string strNtfMsg = m_VecNtfReqList.front();
        m_VecNtfReqList.pop_front();
        COspSipMsg *pcOspSipMsg = (COspSipMsg *)strNtfMsg.c_str();
        TASKLOG(NORMAL_TASK, EVENT_LEV, "有未处理的NTF消息[%s],直接回复\n", OspExtEventDesc(pcOspSipMsg->GetOspEventID()).c_str());
        CNtfRsp cRsp;
        cRsp.SetEvent(pcOspSipMsg->GetOspEventID() + 1);
        string strSession = GetSipSessionID(pcOspSipMsg->GetMsgBody());
        cRsp.SetSession(strSession);
        string strSeqNum = GetSipSeqNO(pcOspSipMsg->GetMsgBody());
        int nSeqNum = static_cast<int>(atol(strSeqNum.c_str()));
        cRsp.SetSeqNum(nSeqNum);
        PostMsgRsp(pcOspSipMsg->GetSipTransID(), cRsp);
    }
}

void CLongQueryTask::InitStateMachine()
{
    CStateProc cNotQueryProc;
    cNotQueryProc.ProcMsg = (CSipTask::PFProcMsg)&CLongQueryTask::NotQueryProcMsg;
    cNotQueryProc.TimerPoll = &CSipTask::TimerPollSkip;
    cNotQueryProc.ProcErrMsg = (CSipTask::PFProcMsg)&CLongQueryTask::NotQueryProcMsg;
    AddRuleProc(NOT_QUERY_STATE, cNotQueryProc);

    CStateProc cWaitRspProc;
    cWaitRspProc.ProcMsg = (CSipTask::PFProcMsg)&CLongQueryTask::WaitRspProcMsg;
    cWaitRspProc.TimerPoll = (CSipTask::PFTimerPoll)&CLongQueryTask::WaitRspTimerPoll;
    cWaitRspProc.ProcErrMsg = (CSipTask::PFProcMsg)&CLongQueryTask::WaitRspProcMsg;
    AddRuleProc(WAIT_RESPONSE_STATE, cWaitRspProc);

    CStateProc cWaitNotifyProc;
    cWaitNotifyProc.ProcMsg = (CSipTask::PFProcMsg)&CLongQueryTask::WaitNotifyProcMsg;
    cWaitNotifyProc.TimerPoll = (CSipTask::PFTimerPoll)&CLongQueryTask::WaitNotifyTimerPoll;
    cWaitNotifyProc.ProcErrMsg = (CSipTask::PFProcMsg)&CLongQueryTask::WaitNotifyProcMsg;
    AddRuleProc(WAIT_NOTIFY_STATE, cWaitNotifyProc);

    NextState(NOT_QUERY_STATE);
}

u32 CLongQueryTask::NotQueryProcMsg( CMessage *const pcMsg )
{
    TASKLOG(NORMAL_TASK, EVENT_LEV, "[CLongQueryTask]收到CU用户[%s]会话[%s]的请求[%s--%hu]\n",
        m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

    switch (pcMsg->event)
    {
    case CU_CUI_PU_REC_QRY_REQ:
        {
            CCuPuRecQryReq *pcReq = (CCuPuRecQryReq *)pcMsg->content;
            CPuRecQryReq cReq = *pcReq;
            cReq.SetEvent(PU_REC_QRY_REQ);

			//modified by zzc
			//EFDB_ErrorCode eRet = g_cRedisDb.GetDevInfo(TSipURI(cReq.GetDevUri()).GetUser(), TPuConfig_puiId, strPuiId);
            
            TSipURI tDstUri;
            TSipURI tDefUri;
            //通用部分暂时保留
            bool bFound = false; //CStreamDecentrationAdapter::FindPuiUri( tDstUri, cReq.GetDevUri(), tDefUri );

            EServerType eSendToDst = bFound? SERVER_PUI : SERVER_CMU;
            OnNotQueryMsg<CPuRecQryReq, CCuPuRecQryRsp>(cReq, tDstUri.GetUser(), eSendToDst);
        }
        break;
    default:
        {
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"收到未知消息");
        }
        break;
    }
    return PROCMSG_OK;
}

u32 CLongQueryTask::WaitRspProcMsg( CMessage *const pcMsg )
{
    COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
    TASKLOG(NORMAL_TASK, EVENT_LEV, "CLongQueryTask[TaskNO %u]收到服务器[%s]的应答消息[%s--%hu]\n",
        GetTaskNO(), pcOspSipMsg->GetSipToUri().GetURIString().c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

    switch (pcMsg->event)
    {
    case OSP_SIP_MSG_PROC_FAIL:
        {
            TASKLOG(NORMAL_TASK, EVENT_LEV, "收到[OSP_SIP_MSG_PROC_FAIL]消息\n");
            CMS_ASSERT(NULL != m_ptRecvNode);
            m_pRsp->SetErrorCode(ERR_CUI_SIP_MSG_FAIL);
            m_strRspOfErr = m_pRsp->ToXml();
            SendRspToCu(m_strRspOfErr, m_ptRecvNode);
            NextState(NOT_QUERY_STATE);
            delete this;
        }
        break;
    case OSP_SIP_DIALOG_TERMINATE:
        {
            CMS_ASSERT( 0 && "CLongQueryTask::WaitRspProcMsg不可能出现会话内消息" );
        }
        break;
    case PU_REC_QRY_RSP:
        {
            OnWaitRspMsg<CPuRecQryRsp, CCuPuRecQryRsp>(pcMsg);
        }
        break;
    case PU_REC_QRY_NTF_REQ:
        {
            if((m_dwReqEvent+2) == pcMsg->event)
            {
                if(pcMsg->content != NULL)
                {
                    string strContect((char *)pcMsg->content, pcMsg->length);
                    m_VecNtfReqList.push_back(strContect);
                    TASKLOG(NORMAL_TASK, CRITICAL_LEV, "在[Wait Rsp State]收到相应的Ntf消息[%s--%hu]\n",
                        OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
                }              
            }
            else
            {
                TASKLOG(NORMAL_TASK, CRITICAL_LEV, "在[Wait Rsp State]收到不对应的Ntf消息[%s--%hu]\n",
                    OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            }
        }
        break;
    default:
        {
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"收到未知消息");
        }
        break;
    }
    return PROCMSG_OK;
}

u32 CLongQueryTask::WaitRspTimerPoll()
{
    if ( GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout() )
    {
        TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户[%s]会话[%s]等待应答消息[%s--%d]超时:SeqNum[%d]\n", 
            m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
            m_pRsp->GetEventStr().c_str(), m_pRsp->GetEventId(), m_pRsp->GetSeqNum());
        CMS_ASSERT(NULL != m_ptRecvNode);
        m_pRsp->SetErrorCode(ERR_CUI_OPR_TIME_OUT);
        m_strRspOfErr = m_pRsp->ToXml();
        SendRspToCu(m_strRspOfErr, m_ptRecvNode);
        NextState(NOT_QUERY_STATE);
        return TIMERPOLL_DEL;
    }
    return TIMERPOLL_DONE;
}

u32 CLongQueryTask::WaitNotifyProcMsg( CMessage *const pcMsg )
{
    switch(pcMsg->event)
    {
    case PU_REC_QRY_NTF_REQ:
        {
            while(!m_VecNtfReqList.empty())
            {
                CMessage cMsg;
                cMsg.event = m_dwReqEvent+2;
                string content = m_VecNtfReqList.front();
                m_VecNtfReqList.pop_front();
                cMsg.content = (u8 *)content.c_str();

                ProcNotifyMsg(&cMsg);
            }

            ProcNotifyMsg(pcMsg);
        }
        break;
    default:
        {
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"收到未知消息");
        }
        break;
    }

    return PROCMSG_OK;
}

u32 CLongQueryTask::ProcNotifyMsg(CMessage *const pcMsg)
{
    switch (pcMsg->event)
    {
    case OSP_SIP_MSG_PROC_FAIL:
    case OSP_SIP_DIALOG_TERMINATE:
        {
            CMS_ASSERT( 0 && "CLongQueryTask::WaitNotifyProcMsg不可能出现SIP异常" );
        }
        break;
    case PU_REC_QRY_NTF_REQ:
        {
            OnWaitNotifyMsg<CPuRecQryNtfReq, CPuRecQryNtfRsp, CCuPuRecQryRsp>(pcMsg);
        }
        break;
    default:
        {
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"收到未知消息");
        }
        break;
    }
    return PROCMSG_OK;
}

u32 CLongQueryTask::WaitNotifyTimerPoll()
{
    if ( GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout() )
    {
        TASKLOG(NORMAL_TASK, ERROR_LEV, "CLongQueryTask[NO: %u]等待NTF消息超时SeqNum[%d]，直接向CU回复应答消息[%s--%d]. m_ptRecvNode is %s\n",
            GetTaskNO(), m_pRsp->GetSeqNum(), m_pRsp->GetEventStr().c_str(), m_pRsp->GetEventId(), (NULL == m_ptRecvNode)? "NULL":"NotNULL");
        m_pRsp->SetErrorCode(ERR_CUI_OPR_TIME_OUT);
        m_strRspOfErr = m_pRsp->ToXml();
        if (NULL != m_ptRecvNode) SendRspToCu(m_strRspOfErr, m_ptRecvNode);
        g_cCuiDataMgr.m_mapSessToTaskMap.Erase(m_strQueryTag);
        m_strQueryTag.clear();
        NextState(NOT_QUERY_STATE);
        return TIMERPOLL_DEL;
    }

    return TIMERPOLL_DONE;
}

void CLongQueryTask::Fetch( PTRecvNode ptRecvNode )
{
    if (GetState() == WAIT_NOTIFY_STATE && m_ptRecvNode == NULL)
    {
        NextState(WAIT_NOTIFY_STATE);

        if (m_pRsp->GetHasLeft())
        {
            m_ptRecvNode = ptRecvNode;
            return;
        }

        if (m_pRsp->GetActualResultNum() == 0)
        {
            m_ptRecvNode = ptRecvNode;
            return;
        }

        bool bRet;
        if (CU_CUI_PU_REC_QRY_RSP == m_pRsp->GetEventId())
        {
            bRet = SendLeftCuRsp(CCuPuRecQryRsp(), ptRecvNode);
        }
        else
        {
            CMS_ASSERT(false && "please implement");
            bRet = true;
        }

        if (bRet)
        {
            delete this;
        }
    }
    else
    {
        CMS_ASSERT(false);
    }
}

bool CLongQueryTask::SendLeftCuRsp( const CCuPuRecQryRsp&, PTRecvNode ptNode )
{
    CCuPuRecQryRsp *pcRspToCu = (CCuPuRecQryRsp *)m_pRsp;

    u32 dwParts = m_pRsp->ToXml().size() / MAX_RECV_MSG_LEN + 1;
    
    if (dwParts == 1)
    {
        SendRspToCu<CCuPuRecQryRsp>(*pcRspToCu, ptNode);

        pcRspToCu->SetActualResultNum(0);
        pcRspToCu->GetRecInfoList().clear();

        return !m_pRsp->GetHasLeft();
    }
    else
    {
        u32 dwResultNum = pcRspToCu->GetRecInfoList().size()/dwParts;
        CCuPuRecQryRsp cRsp = *pcRspToCu;
        cRsp.SetActualResultNum(dwResultNum);
        CPuRecInfoList::iterator it = cRsp.GetRecInfoList().begin();
        for (u32 i = 0; i < dwResultNum; ++i, ++it)
        {
        }
        cRsp.GetRecInfoList().erase(it, cRsp.GetRecInfoList().end());
        SendRspToCu<CCuPuRecQryRsp>(cRsp, ptNode);


        CPuRecInfoList::iterator it2 = pcRspToCu->GetRecInfoList().begin();
        for (u32 i = 0; i < dwResultNum; ++i, ++it2)
        {
        }
        pcRspToCu->GetRecInfoList().erase(pcRspToCu->GetRecInfoList().begin(), it2);

        return false;
    }
}

