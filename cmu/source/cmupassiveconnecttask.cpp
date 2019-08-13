
#include "cms/cms_errorcode.h"
#include "cms/cmu/cmu_event.h"
#include "cms/cmu/cmu_struct.h"
#include "cms/cmu/cmu_proto.h"

#include "cmuconst.h"
#include "cmudata.h"
#include "cmucoreapp.h"
#include "cmupassiveconnecttask.h"
#include "cmuconf.h"
#include "cmuconnecttask.h"

extern CCmuConf g_cCmuConf;

CCmuPassiveConnecterTask::CCmuPassiveConnecterTask(CInstExt *pcInst) : CLoginSession(pcInst)
{

}

CCmuPassiveConnecterTask::~CCmuPassiveConnecterTask()
{  
    NotifyCmuState(CMU_CMU_DELETE);

    g_cCmuList.Erase(m_tDstURI.GetURIString(), false);
}

void CCmuPassiveConnecterTask::NotifyCmuState(u32 dwConnResult)
{
    //通知所有子事务cmu的登录状态
    CMessage cOspMsg;
    cOspMsg.event = u16(dwConnResult);

    COspSipMsg cOspSipMsg;
    cOspSipMsg.SetOspEventID(cOspMsg.event);
    cOspSipMsg.SetMsgBody(m_tDstURI.GetURIString().c_str(), u16(strlen(m_tDstURI.GetURIString().c_str())+1));
    cOspMsg.content = const_cast<u8*>(cOspSipMsg.GetMsgBuf());
    cOspMsg.length = cOspSipMsg.GetMsgLen();

    CTask* ptTask = NULL;
    Iterator pPos;
    while (!pPos.End())
    {
        if (IterateChildTasks(pPos, ptTask) && ptTask != NULL)
        {
            if (ptTask->ProcMsg(&cOspMsg) == PROCMSG_DEL)
            {
                delete ptTask;
                ptTask = NULL;
            }
        }
    }
}

void CCmuPassiveConnecterTask::InitStateMachine()
{
    CStateProc cIdleProc;
    cIdleProc.ProcMsg = (CTask::PFProcMsg)&CCmuPassiveConnecterTask::OnIdle;
    cIdleProc.ProcErrMsg = (CTask::PFProcMsg)&CCmuPassiveConnecterTask::OnIdle;
    cIdleProc.TimerPoll = (CTask::PFTimerPoll)&CCmuPassiveConnecterTask::OnIdleTimer;
    AddRuleProc(Idle, cIdleProc);

    CStateProc cServiceProc;
    cServiceProc.ProcMsg = (CTask::PFProcMsg)&CCmuPassiveConnecterTask::OnService;
    cServiceProc.ProcErrMsg = (CTask::PFProcMsg)&CCmuPassiveConnecterTask::OnService;
    cServiceProc.TimerPoll = (CTask::PFTimerPoll)&CCmuPassiveConnecterTask::OnServiceTimer;
    AddRuleProc(Service, cServiceProc);

    NextState(Idle);
}

u32 CCmuPassiveConnecterTask::OnIdle(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_FAIL;
    switch(pcMsg->event)
    {
    case CMU_CONNECT_REQ:
        {
            //异域的cmu发起连接
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

            CCmuConnectReq cReq;
            pcOspSipMsg->GetMsgBody(cReq);
            TSipURI tPeerUri;
            tPeerUri.SetURIString(cReq.GetSrcUri());

			m_tDstURI = tPeerUri;
			m_tPeerIp = cReq.GetLocalIp();
   
            CCmuConnectRsp cRsp;
            cRsp.SetErrorCode(CMS_SUCCESS);
            cRsp.SetSeqNum(cReq.GetSeqNum());
            cRsp.SetSession(cReq.GetSession());
			cRsp.SetLocalIp(g_cCmuConf.m_tLocalAddr.GetIpStr());

            CCommonConfig cConfig = GetCommonConfig();
            if (!cConfig.GetRedisIP().empty())
            {
                cRsp.SetSupportRedis(true);
                cRsp.SetRedisIp(cConfig.GetRedisIP());
                cRsp.SetRedisPort(cConfig.GetRedisPort());
                cRsp.SetRedisPassword(cConfig.GetRedisPassword());
            }

            PostMsgRsp(pcOspSipMsg->GetSipTransID(), cRsp);

            //连接成功
            TASKLOG(CMU_MOD_CMU, CRITICAL_LEV, "异域cmu[%s]连接成功!\n",
                m_tDstURI.GetURIString().c_str());

            //开始心跳
            SetHBParam(m_tDstURI);

            //进入服务态
            NextState(Service);

            g_cCmuList.Insert(m_tDstURI.GetURIString(), this, false);

            dwRet = PROCMSG_OK;
        }
        break;

    default:
        {
            TASKLOG(CMU_MOD_CMU, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }

    return dwRet;
}

u32 CCmuPassiveConnecterTask::OnIdleTimer()
{
    return TIMERPOLL_DONE;
}

u32 CCmuPassiveConnecterTask::OnService(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_OK;
    switch(pcMsg->event)
    {
    case OSP_SIP_DISCONNECT:
        {
            dwRet = DisProc(pcMsg);
        }
        break;

    default:
        {
            TASKLOG(CMU_MOD_CMU, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", 
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

            dwRet = PROCMSG_FAIL;
        }
        break;
    }

    return dwRet;
}

u32 CCmuPassiveConnecterTask::OnServiceTimer()
{
    return TIMERPOLL_DONE;
}

u32 CCmuPassiveConnecterTask::DisProc(CMessage *const pcMsg)
{
    //如果是被连接方需要清除所有事务
    return PROCMSG_DEL;
}
