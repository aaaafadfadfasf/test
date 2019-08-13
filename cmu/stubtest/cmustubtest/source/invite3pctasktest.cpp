
#include "cms/cms_errorcode.h"


#include "cmustubtestdata.h"
#include "cmustubtestconst.h"
#include "culogintasktest.h"
#include "cmustubtestcoreapp.h"
#include "invite3pctasktest.h"
#include "cmustubtestconf.h"

void CInvite3PCTask::InitStateMachine()
{
    CStateProc cReqProc;
    cReqProc.ProcMsg = (CTask::PFProcMsg)&CInvite3PCTask::OnReq;
    cReqProc.ProcErrMsg = (CTask::PFProcMsg)&CInvite3PCTask::OnReq;
    cReqProc.TimerPoll = (CTask::PFTimerPoll)&CInvite3PCTask::OnReqTimer;
    AddRuleProc(Req, cReqProc);

    CStateProc cWaitRspProc;
    cWaitRspProc.ProcMsg = (CTask::PFProcMsg)&CInvite3PCTask::OnWaitRsp;
    cWaitRspProc.ProcErrMsg = (CTask::PFProcMsg)&CInvite3PCTask::OnWaitRsp;
    cWaitRspProc.TimerPoll = (CTask::PFTimerPoll)&CInvite3PCTask::OnWaitRspTimer;
    AddRuleProc(WaitRsp, cWaitRspProc);

    CStateProc cHoldProc;
    cHoldProc.ProcMsg = (CTask::PFProcMsg)&CInvite3PCTask::OnAck;
    cHoldProc.ProcErrMsg = (CTask::PFProcMsg)&CInvite3PCTask::OnAck;
    cHoldProc.TimerPoll = (CTask::PFTimerPoll)&CInvite3PCTask::OnAckTimer;
    AddRuleProc(Ack, cHoldProc);

    NextState(Req);
}

u32 CInvite3PCTask::OnReq(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_OK;

    TInviteReqMsg* ptInviteReqMsg = (TInviteReqMsg*)pcMsg->content;
    s32 nPuChn = ptInviteReqMsg->m_nSrcChn;
    s32 nCuChn = ptInviteReqMsg->m_nDstChn;

	//设备Uri
    string strDevUri;
    if (strlen(ptInviteReqMsg->m_szSrcUri) == 0)
    {
        strDevUri = TEST_PU_URI;
    }
    else
    {
        strDevUri = ptInviteReqMsg->m_szSrcUri;
    }

	//用户Session
    CCuLoginTask* ptCuLoginTask = NULL;

    Iterator pPos;
    string strSession;
    CLoginSession* pTask = NULL;
    while (!pPos.End())
    {
        if (g_cCuList.Iterate(pPos, strSession, pTask) && pTask != NULL)
        {
            ptCuLoginTask = (CCuLoginTask*)(pTask);
            break;
        }
    }

    if (ptCuLoginTask == NULL)
    {
        TASKLOG(ERROR_LEV, "没有cu用户登录\n");
        dwRet = PROCMSG_DEL;
        return dwRet;
    }

	//设备信息
    CPuData* ptPuData = g_cPuList.FindPuData(strDevUri);
    if (ptPuData == NULL)
    {
        TASKLOG(ERROR_LEV, "设备[%s]不存在\n",
            strDevUri.c_str());
        dwRet = PROCMSG_DEL;
        return dwRet;
    }

	//构造请求
    m_tReq.SetSession(ptCuLoginTask->GetSession());

    TChannel tSrcChn;    //源通道
    tSrcChn.SetDevUri(strDevUri);
    tSrcChn.SetChnNO(nPuChn);

    TChannel tDstChn;    //目的设备通道
    tDstChn.SetDevUri(ptInviteReqMsg->m_szDstUri);
    tDstChn.SetChnNO(nCuChn);

	m_tReq.SetSrcChn(tSrcChn);
	m_tReq.SetDstChn(tDstChn);
	
    TSdp tSdp;
	TNetAddr tNetAddr(g_cCmuConf.m_tLocalAddr.tIpAddr, 60128+nCuChn*4);
	TNetAddrList tAddrList;
	tAddrList.push_back(tNetAddr);

	TMediaDesc tVideoMediaDesc;
	tVideoMediaDesc.SetMediaType(MEDIA_TYPE_VIDEO);
	tVideoMediaDesc.AddMediaTransChn(TMediaTransChannel(MEDIA_TRANS_CHN_RTP, tAddrList));

	tAddrList[0].SetNetPort(tAddrList[0].GetNetPort()+1);
	tVideoMediaDesc.AddMediaTransChn(TMediaTransChannel(MEDIA_TRANS_CHN_RTCP, tAddrList));
    tSdp.AddMediaDesc(tVideoMediaDesc);


	TMediaDesc tAudioMediaDesc;
	tAudioMediaDesc.SetMediaType(MEDIA_TYPE_AUDIO);
	tAddrList[0].SetNetPort(tAddrList[0].GetNetPort()+2);
	tAudioMediaDesc.AddMediaTransChn(TMediaTransChannel(MEDIA_TRANS_CHN_RTP, tAddrList));

	tAddrList[0].SetNetPort(tAddrList[0].GetNetPort()+3);
	tAudioMediaDesc.AddMediaTransChn(TMediaTransChannel(MEDIA_TRANS_CHN_RTCP, tAddrList));
	tSdp.AddMediaDesc(tAudioMediaDesc);

    m_tReq.SetSdp(tSdp);

    //转发消息
    PostReq(KDSIP_EVENT_INVITE_REQ, m_tReq, GetCmuUri());

    //等待应答
    NextState(WaitRsp);

    return dwRet;
}

u32 CInvite3PCTask::OnReqTimer()
{
    return TIMERPOLL_DONE;
}

u32 CInvite3PCTask::OnWaitRsp(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_DEL;

    COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
    if (pcOspSipMsg == NULL)
    {
        TASKLOG(ERROR_LEV, "ospsip 消息为空\n");
        return dwRet;
    }

    CRsp cRsp;
    pcOspSipMsg->GetMsgBody(cRsp);
	m_tSipDlgId = pcOspSipMsg->GetSipDlgID();

    if (cRsp.GetErrorCode() == CMS_SUCCESS)
    {
        TASKLOG(CRITICAL_LEV, "对设备[%s]Invite到[%s]成功\n", 
            m_tReq.GetSrcChn().ToString().c_str(), m_tReq.GetDstChn().ToString().c_str());

		CInviteACK cInviteAck;
		cInviteAck.SetSession(m_tReq.GetSession());
		if(PROCMSG_OK!= PostInDlgReq(KDSIP_EVENT_INVITE_ACK, cInviteAck, m_tSipDlgId))
		{
			TASKLOG(ERROR_LEV, "发送KDSIP_EVENT_INVITE_ACK失败！\n");
		}

		NextState(Ack);
        return PROCMSG_OK;
    }
    else
    {
        TASKLOG(ERROR_LEV, "对设备[%s]Invite到[%s]失败，错误码[%d]\n", 
            m_tReq.GetSrcChn().ToString().c_str(), m_tReq.GetDstChn().ToString().c_str(), 
			cRsp.GetErrorCode());

        return PROCMSG_DEL;
    }
}

u32 CInvite3PCTask::OnWaitRspTimer()
{
    //应答超时
    if (GetCurStateHoldTime() > CMU_REQ_RSP_TASK_TIMEOUT)
    {
        TASKLOG(ERROR_LEV, "Dev[%s]应答超时, Msg[%s-%d]请求失败\n", 
            m_tReq.GetSrcChn().ToString().c_str(), m_tReq.GetEventStr().c_str(), m_tReq.GetEventId());

        return TIMERPOLL_DEL;
    }

    return TIMERPOLL_DONE;
}

u32 CInvite3PCTask::OnAck(CMessage *const pcMsg)
{
    if (pcMsg->event == BYE_REQ)
    {
        TASKLOG(ERROR_LEV, "bye devDev[%s], dlg[%d]\n", 
            m_tReq.GetSrcChn().ToString().c_str(), m_tSipDlgId);

        string strNull;
        PostInDlgReq(KDSIP_EVENT_INVITE_BYE_REQ, strNull, m_tSipDlgId);
        return PROCMSG_DEL;
    }
    else
    {
        TASKLOG(ERROR_LEV, "not BYE_REQ\n");
    }

    return PROCMSG_OK;
}

u32 CInvite3PCTask::OnAckTimer()
{
    return TIMERPOLL_DONE;
}

