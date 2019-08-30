
#include "cms/cms_errorcode.h"
#include "cms/cmu/cmu_event.h"
#include "cms/cmu/cmu_struct.h"
#include "cms/cmu/cmu_proto.h"

#include "ospsiproutessertask.h"
#include "ospsipapp.h"
#include "ospsipconvert.h"

extern TOspSipConf g_tOspSipConf;

COspsipRouteSserTask::COspsipRouteSserTask(CInstExt *pcInst) : CSipTask(pcInst)
{
    m_tSsDlgId = INVALID_DIALOG_ID;
}

COspsipRouteSserTask::~COspsipRouteSserTask()
{
    ReleaseResource();
}

void COspsipRouteSserTask::InitStateMachine()
{
    CStateProc cWaitSsProc;
    cWaitSsProc.ProcMsg = (CTask::PFProcMsg)&COspsipRouteSserTask::OnWaitSs;
    cWaitSsProc.ProcErrMsg = (CTask::PFProcMsg)&COspsipRouteSserTask::OnWaitSs;
    cWaitSsProc.TimerPoll = (CTask::PFTimerPoll)&COspsipRouteSserTask::OnWaitSsTimer;
    AddRuleProc(WaitSs, cWaitSsProc);

    CStateProc cServiceProc;
    cServiceProc.ProcMsg = (CTask::PFProcMsg)&COspsipRouteSserTask::OnService;
    cServiceProc.ProcErrMsg = (CTask::PFProcMsg)&COspsipRouteSserTask::OnService;
    cServiceProc.TimerPoll = (CTask::PFTimerPoll)&COspsipRouteSserTask::OnServiceTimer;
    AddRuleProc(Service, cServiceProc);

    NextState(WaitSs);
}

void COspsipRouteSserTask::PrintData() const
{
    CTask::PrintData();

    OspPrintf(TRUE, FALSE, "\n  sser[%s][%s]  SIP-dlg[%u]\n",
        m_strdevUri.c_str(), m_tSsDlgId);

    OspPrintf(TRUE, FALSE, "  ua list:\n");
    typedef set<string>::const_iterator SIT;
    for (SIT sIt = m_tUaList.begin(); sIt != m_tUaList.end(); ++sIt)
    {
        OspPrintf(TRUE, FALSE, "    [%s]\n", sIt->c_str());
    }

    OspPrintf(TRUE, FALSE, "\n");
}

void COspsipRouteSserTask::ReleaseResource()
{
    switch(GetState())
    {
    case WaitSs:
        {
            if (m_tSsDlgId != INVALID_DIALOG_ID)
            {
                OspSipReleaseDlg(m_tSsDlgId);
                m_tSsDlgId = INVALID_DIALOG_ID;
            }
        }
        break;

    case Service:
        {
            if (m_tSsDlgId != INVALID_DIALOG_ID)
            {
                OspSipReleaseDlg(m_tSsDlgId);
                m_tSsDlgId = INVALID_DIALOG_ID;
            }
        }
        break;

    default:
        {
            TASKLOG(OSPSIP, ERROR_LEV, "δ֪����״̬[%d]\n", GetState());
        }
        break;
    }
}

u32 COspsipRouteSserTask::OnWaitSs(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_OK;
    switch(pcMsg->event)
    {
    case UA_ROUTER_SS_RSP:
        {           
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            m_tSsDlgId = pcOspSipMsg->GetSipDlgID();

            CUaRouterSsRsp cRsp;
            pcOspSipMsg->GetMsgBody(cRsp);
            int dwErrorCode = cRsp.GetErrorCode();

            if (dwErrorCode == CMS_SUCCESS)
            {
                TASKLOG(OSPSIP, EVENT_LEV, "��proxy����·����Ϣ�ɹ�\n");

                NextState(Service);
            }
            else
            {
                TASKLOG(OSPSIP, ERROR_LEV, "��proxy����·����Ϣʧ�ܣ�������[%d]\n", dwErrorCode);

                dwRet = PROCMSG_DEL; //liangli ����ɾ������֮����ٲ��ܿ������ˣ��Ƿ����ԣ�
            }      
        }
        break;

    case OSP_SIP_MSG_PROC_FAIL:
        {
            //����������յ�SIP��Ĵ���Ӧ��
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

            TASKLOG(OSPSIP, ERROR_LEV, "��proxy����·����Ϣʧ��, ����SIP�����, �ȴ���ʱ���ط���sip status code[%d]\n",
                pcOspSipMsg->GetSipErrorCode());

            dwRet = PROCMSG_OK;
        }
        break;

    case OSP_SIP_DIALOG_TERMINATE:
        {
            //SIP��Dialog�쳣��ֹ֪ͨ
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

            TASKLOG(OSPSIP, ERROR_LEV, "SIP��Dialog�쳣��ֹ, �ȴ���ʱ���ط���sip status code[%d]\n",
                pcOspSipMsg->GetSipErrorCode());

            //�ͷ�ospsip��Դ
            if (m_tSsDlgId != INVALID_DIALOG_ID)
            {
                OspSipReleaseDlg(m_tSsDlgId);
                m_tSsDlgId = INVALID_DIALOG_ID;
            }

            dwRet = PROCMSG_OK;
        }
        break;

    default:
        {
            TASKLOG(OSPSIP, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            dwRet = PROCMSG_FAIL;
        }
        break;
    }

    return dwRet;
}

u32 COspsipRouteSserTask::OnWaitSsTimer()
{
    if (GetCurStateHoldTime() > CMS_MSG_TIMEOUT)
    {
        TASKLOG(OSPSIP, WARNING_LEV, "����·����Ϣ��ʱ�����·�����\n");

        //���·�����
        StartSs();
    }

    return TIMERPOLL_DONE;
}

u32 COspsipRouteSserTask::OnService(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_OK;
    switch(pcMsg->event)
    {
    case UA_ROUTER_NTF_REQ:
        {
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

            CUaRouterNtfReq cReq;
            pcOspSipMsg->GetMsgBody(cReq);
            SendRouteTableToConvertor(cReq.ToXml());
            
            CUaRouterNtfRsp cRsp;
            cRsp.SetEvent(UA_ROUTER_NTF_RSP);
            cRsp.SetErrorCode(CMS_SUCCESS);
            cRsp.SetSeqNum(cReq.GetSeqNum());
            cRsp.SetSession(cReq.GetSession());
            PostRsp(KDSIP_EVENT_NOTIFY_RSP, pcOspSipMsg->GetSipTransID(), cRsp);
        }
        break;
    
    case OSP_SIP_DIALOG_TERMINATE:
    case OSP_PROXY_DISCONNECT:
    case OSP_SIP_DISCONNECT:
        {
            //�ͷ�ospsip��Դ
            if (m_tSsDlgId != INVALID_DIALOG_ID)
            {
                OspSipReleaseDlg(m_tSsDlgId);
                m_tSsDlgId = INVALID_DIALOG_ID;
            }

            //���³��Զ���
            StartSs();
        }
        break;

    default:
        {
            TASKLOG(OSPSIP, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            dwRet = PROCMSG_FAIL;
        }
        break;
    }

    return dwRet;
}

u32 COspsipRouteSserTask::OnServiceTimer()
{
    return TIMERPOLL_DONE;
}

void COspsipRouteSserTask::StartSs()
{
    NextState(WaitSs);

    COspSipInst* pInst = (COspSipInst*)GetInstance();
    if (pInst->CurState() == COspSipInst::DaemonService)
    {
        SendSsReq();
    }
    else
    {
        TASKLOG(OSPSIP, ERROR_LEV, "ospsipδע��proxy���Ժ�����...\n");
    }
}

void COspsipRouteSserTask::SendSsReq()
{
    CUaRouterSsReq cReq;
    cReq.SetSession(m_strdevUri);
    cReq.SetDevUri(m_strdevUri);

    TSipURI tProxyURI;
    if (g_tOspSipConf.tProxyURI.IsNull())  //liangli �ܶ�ط�������������߼����ܷ��һ�£��ڵ�һ�����õ�ʱ��ͱ�֤g_tOspSipConf.tProxyURI��Ϊ��
    {
        tProxyURI.SetUser(UNIQUE_SIP_UA_PROXY);
        tProxyURI.SetDomain(g_tOspSipConf.tLocalURI.GetDomain());
    }
    else
    {
        tProxyURI = g_tOspSipConf.tProxyURI;
    }


    PostReq(KDSIP_EVENT_SUBSCRIBE_REQ, cReq, tProxyURI);
}

void COspsipRouteSserTask::SendRouteTableToConvertor(const string& strRouteTable)
{
    TOspSipSendMsg* ptSipSendMsg = new TOspSipSendMsg;
    ptSipSendMsg->eSipMsgType = SIP_MSG_UPDATE_ROUTE_INFO;
    ptSipSendMsg->strMsgBuf = strRouteTable;

    TOspSipCoreMsg tCoreMsg;
    tCoreMsg.eCoreMsgType = OSPSIP_CORE_MSG_TYPE_SEND;
    tCoreMsg.pvMsg = ptSipSendMsg;

    WriteCoreQue(tCoreMsg);
}