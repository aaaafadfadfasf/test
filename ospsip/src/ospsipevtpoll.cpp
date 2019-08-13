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
			//bodyӦ�ö����Բ����˰ɣ�
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
			FailedFreeSipResource(pSipEvent);//�ԷǷ�����Ķ�����Դ�ͷ�
			
        }

        KDSipFreeSipEvent(pSipEvent); 
    }
    else
    {
        GLOBELOG(OSPSIP, ERROR_LEV, "siprecv��Ϣָ���\n");
    }
}

CEventDescMap  g_cSipTypeMap;

void ProcSipOutEvent(TOspSipSendMsg* ptSipSendMsg)
{
    if (ptSipSendMsg != NULL)
    {
        //������Ϣ
        u8 byLogLev = EVENT_LEV;

        if (ptSipSendMsg->eSipEvent == KDSIP_EVENT_OPTIONS_HB_REQ
            ||ptSipSendMsg->eSipEvent == KDSIP_EVENT_OPTIONS_HB_RSP
            ||ptSipSendMsg->eSipMsgType == SIP_MSG_TYPE_CHECKER)
        {
            byLogLev = TIMER_LEV;
        }

        //ospsip��kdsip�����ȶ�������һЩ��־�ļ��𣬱�����־�ļ����ܿ츲��
        byLogLev = TRIVIAL_LEV;

        GLOBELOG(OSPSIP, byLogLev, "recv sip send msg, sip operate type[%s-%d]\n",
            g_cSipTypeMap.GetEventDesc(ptSipSendMsg->eSipMsgType).c_str(), ptSipSendMsg->eSipMsgType);

        if (ptSipSendMsg->eSipMsgType == SIP_MSG_TYPE_REQ)
        {
            //���ͶԻ�������
            g_cOspSipConvertor.OspSipPostReq_Inner(ptSipSendMsg->eSipEvent, ptSipSendMsg->strMsgBuf, 
                ptSipSendMsg->tDstURI, ptSipSendMsg->tSrcOspTrans, ptSipSendMsg->strDirectUAAddr,
                ptSipSendMsg->ptSipExtraInfo);
        }
        else if (ptSipSendMsg->eSipMsgType == SIP_MSG_TYPE_REQ_INDLG)
        {
            //���ͶԻ�������
            g_cOspSipConvertor.OspSipPostInDlgReq_Inner(ptSipSendMsg->eSipEvent, ptSipSendMsg->strMsgBuf,
                ptSipSendMsg->tDstDlgId, ptSipSendMsg->tSrcOspTrans, ptSipSendMsg->ptSipExtraInfo);
        }
        else if (ptSipSendMsg->eSipMsgType == SIP_MSG_TYPE_RSP)
        {
            //����Ӧ�𣬲����ֶԻ��ڡ��Ի���
            g_cOspSipConvertor.OspSipPostRsp_Inner(ptSipSendMsg->eSipEvent, ptSipSendMsg->strMsgBuf,
                ptSipSendMsg->tDstTransId, ptSipSendMsg->tSrcOspTrans, ptSipSendMsg->eRspCode,
                ptSipSendMsg->ptSipExtraInfo);
        }
        else if (ptSipSendMsg->eSipMsgType == SIP_MSG_TYPE_CHECKER)
        {
            //�����ͳ�ʱ����
            g_cOspSipConvertor.CheckTimeout();
        }
        else if (ptSipSendMsg->eSipMsgType == SIP_MSG_TYPE_RELEASE_DLG)
        {
            //�ͷ�SIP�Ի�
            g_cOspSipConvertor.ReleaseDialog_Inner(ptSipSendMsg->tDstDlgId);
        }
        else if (ptSipSendMsg->eSipMsgType == SIP_MSG_UPDATE_ROUTE_INFO)
        {
            //����·����Ϣ
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
        GLOBELOG(OSPSIP, ERROR_LEV, "sipsend��Ϣָ���\n");
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
        TOspSipCoreMsg tSipCoreMsg = ReadCoreQue(INFINITE);    //û����Ϣ������

        if (tSipCoreMsg.eCoreMsgType == OSPSIP_CORE_MSG_TYPE_SEND)
        {
            //������Ϣ
            TOspSipSendMsg* ptSipSendMsg = (TOspSipSendMsg*)tSipCoreMsg.pvMsg;
            ProcSipOutEvent(ptSipSendMsg);         
        }
        else if (tSipCoreMsg.eCoreMsgType == OSPSIP_CORE_MSG_TYPE_RECV)
        {
            //������Ϣ
            CKDSipEvent* pSipEvent = (CKDSipEvent*)tSipCoreMsg.pvMsg;
            ProcSipInEvent(pSipEvent);        
        }
        else
        {
            GLOBELOG(OSPSIP, ERROR_LEV, "δ֪��OSPSIP_CORE_MSG[%d]\n", int(tSipCoreMsg.eCoreMsgType));
        }                   
    }

    return NULL;
}