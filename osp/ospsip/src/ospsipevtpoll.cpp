#include "ospsipevtpoll.h"
#include "ospsipconvert.h"

bool g_bSipPollRun = true;

void FailedFreeSipResource(CKDSipEvent* pSipEvent)
{
	if (!pSipEvent)
	{
		return;
	}
	switch (pSipEvent->GetEvent())
	{
	case KDSIP_EVENT_REGISTER_REQ:	
		{
			//body应该都可以不用了吧？
			CKDSipMsgBody sipMsg;
			sipMsg.SetContentType(KDSIP_CONTENT_XML);
			sipMsg.SetMsgBodyContent(NULL, 0);
			KDSipRegisterRsp(pSipEvent->GetTransID(), KDSIP_RESPONSE_403_FORBIDDEN, &sipMsg);
		}
		break;
	case KDSIP_EVENT_INVITE_REQ:
		KDSipInviteRsp(pSipEvent->GetTransID(), KDSIP_RESPONSE_403_FORBIDDEN);
		break;
	case KDSIP_EVENT_INVITE_BYE_REQ:
		KDSipByeRsp(pSipEvent->GetTransID(), KDSIP_RESPONSE_403_FORBIDDEN);
		break;
	case KDSIP_EVENT_INFO_REQ:
		KDSipInfoRsp(pSipEvent->GetTransID(), KDSIP_RESPONSE_403_FORBIDDEN);		
		break;
	case KDSIP_EVENT_OPTIONS_HB_REQ:
		KDSipOptionsRsp(pSipEvent->GetTransID(), KDSIP_RESPONSE_403_FORBIDDEN);		
		break;
	case KDSIP_EVENT_MESSAGE_REQ:
		KDSipMessageRsp(pSipEvent->GetTransID(), KDSIP_RESPONSE_403_FORBIDDEN);
		break;
	case KDSIP_EVENT_SUBSCRIBE_REQ:
		KDSipSubscribeRsp(pSipEvent->GetTransID(), KDSIP_RESPONSE_403_FORBIDDEN);
		break;
	case KDSIP_EVENT_UNSUBSCRIBE_REQ:
		KDSipUnSubscribeRsp(pSipEvent->GetTransID(), KDSIP_RESPONSE_403_FORBIDDEN);
		break;
	case KDSIP_EVENT_NOTIFY_REQ:
		KDSipNotifyRsp(pSipEvent->GetTransID(), KDSIP_RESPONSE_403_FORBIDDEN);
		break;
	default:
		break;
	}
	
}
void ProcSipInEvent(CKDSipEvent* pSipEvent)
{
    if (pSipEvent != NULL)
    {      
        if (!g_cOspSipConvertor.DoConvert(pSipEvent))
        {
            GLOBELOG(OSPSIP, ERROR_LEV, "osp sip convert fail\n");
			FailedFreeSipResource(pSipEvent);//对非法请求的额外资源释放
			
        }

        KDSipFreeSipEvent(pSipEvent); 
    }
    else
    {
        GLOBELOG(OSPSIP, ERROR_LEV, "siprecv消息指针空\n");
    }
}

CEventDescMap  g_cSipTypeMap;

void ProcSipOutEvent(TOspSipSendMsg* ptSipSendMsg)
{
    if (ptSipSendMsg != NULL)
    {
        //发送消息
        u8 byLogLev = EVENT_LEV;

        if (ptSipSendMsg->eSipEvent == KDSIP_EVENT_OPTIONS_HB_REQ
            ||ptSipSendMsg->eSipEvent == KDSIP_EVENT_OPTIONS_HB_RSP
            ||ptSipSendMsg->eSipMsgType == SIP_MSG_TYPE_CHECKER)
        {
            byLogLev = TIMER_LEV;
        }

        //ospsip和kdsip基本稳定，降低一些日志的级别，避免日志文件被很快覆盖
        byLogLev = TRIVIAL_LEV;

        GLOBELOG(OSPSIP, byLogLev, "recv sip send msg, sip operate type[%s-%d]\n",
            g_cSipTypeMap.GetEventDesc(ptSipSendMsg->eSipMsgType).c_str(), ptSipSendMsg->eSipMsgType);

        if (ptSipSendMsg->eSipMsgType == SIP_MSG_TYPE_REQ)
        {
            //发送对话外请求
            g_cOspSipConvertor.OspSipPostReq_Inner(ptSipSendMsg->eSipEvent, ptSipSendMsg->strMsgBuf, 
                ptSipSendMsg->tDstURI, ptSipSendMsg->tSrcOspTrans, ptSipSendMsg->strDirectUAAddr,
                ptSipSendMsg->ptSipExtraInfo);
        }
        else if (ptSipSendMsg->eSipMsgType == SIP_MSG_TYPE_REQ_INDLG)
        {
            //发送对话内请求
            g_cOspSipConvertor.OspSipPostInDlgReq_Inner(ptSipSendMsg->eSipEvent, ptSipSendMsg->strMsgBuf,
                ptSipSendMsg->tDstDlgId, ptSipSendMsg->tSrcOspTrans, ptSipSendMsg->ptSipExtraInfo);
        }
        else if (ptSipSendMsg->eSipMsgType == SIP_MSG_TYPE_RSP)
        {
            //发送应答，不区分对话内、对话外
            g_cOspSipConvertor.OspSipPostRsp_Inner(ptSipSendMsg->eSipEvent, ptSipSendMsg->strMsgBuf,
                ptSipSendMsg->tDstTransId, ptSipSendMsg->tSrcOspTrans, ptSipSendMsg->eRspCode,
                ptSipSendMsg->ptSipExtraInfo);
        }
        else if (ptSipSendMsg->eSipMsgType == SIP_MSG_TYPE_CHECKER)
        {
            //处理发送超时事务
            g_cOspSipConvertor.CheckTimeout();
        }
        else if (ptSipSendMsg->eSipMsgType == SIP_MSG_TYPE_RELEASE_DLG)
        {
            //释放SIP对话
            g_cOspSipConvertor.ReleaseDialog_Inner(ptSipSendMsg->tDstDlgId);
        }
        else if (ptSipSendMsg->eSipMsgType == SIP_MSG_UPDATE_ROUTE_INFO)
        {
            //更新路由信息
            g_cOspSipConvertor.UpdateRouteInfo(ptSipSendMsg->strMsgBuf);
        }      
        else
        {
            GLOBELOG(OSPSIP, ERROR_LEV, "unknown sip send msg type[%d]\n", ptSipSendMsg->eSipMsgType);
        }

        delete ptSipSendMsg;
        ptSipSendMsg = NULL;
    }
    else
    {
        GLOBELOG(OSPSIP, ERROR_LEV, "sipsend消息指针空\n");
    }
}

void* SipEventPollThread(void* pArg)
{
    UNREFERENCED_PARAMETER(pArg);

    g_cSipTypeMap.AddEventDesc("SIP_MSG_TYPE_REQ", SIP_MSG_TYPE_REQ);
    g_cSipTypeMap.AddEventDesc("SIP_MSG_TYPE_REQ_INDLG", SIP_MSG_TYPE_REQ_INDLG);
    g_cSipTypeMap.AddEventDesc("SIP_MSG_TYPE_RSP", SIP_MSG_TYPE_RSP);
    g_cSipTypeMap.AddEventDesc("SIP_MSG_TYPE_CHECKER", SIP_MSG_TYPE_CHECKER);
    g_cSipTypeMap.AddEventDesc("SIP_MSG_TYPE_RELEASE_DLG", SIP_MSG_TYPE_RELEASE_DLG);
    g_cSipTypeMap.AddEventDesc("SIP_MSG_UPDATE_ROUTE_INFO", SIP_MSG_UPDATE_ROUTE_INFO);

    while(g_bSipPollRun)
    {
        TOspSipCoreMsg tSipCoreMsg = ReadCoreQue(INFINITE);    //没有消息就阻塞

        if (tSipCoreMsg.eCoreMsgType == OSPSIP_CORE_MSG_TYPE_SEND)
        {
            //发送消息
            TOspSipSendMsg* ptSipSendMsg = (TOspSipSendMsg*)tSipCoreMsg.pvMsg;
            ProcSipOutEvent(ptSipSendMsg);         
        }
        else if (tSipCoreMsg.eCoreMsgType == OSPSIP_CORE_MSG_TYPE_RECV)
        {
            //接收消息
            CKDSipEvent* pSipEvent = (CKDSipEvent*)tSipCoreMsg.pvMsg;
            ProcSipInEvent(pSipEvent);        
        }
        else
        {
            GLOBELOG(OSPSIP, ERROR_LEV, "未知的OSPSIP_CORE_MSG[%d]\n", int(tSipCoreMsg.eCoreMsgType));
        }                   
    }

    return NULL;
}