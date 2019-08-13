
#include "cms/cmu/cmu_proto.h"
#include "ospsipapp.h"
#include "ospsiphb.h"
#include "ospsipconvert.h"
#include "ospsiproutessertask.h"


extern TOspSipConf g_tOspSipConf;

s32 g_nRegProxyExpires = DEFAULT_SIP_REG_EXPIRE;

COspSipApp g_cOspSipApp;

COspSipInst::COspSipInst() : CInstExt()
{
    ClearInst();
}

COspSipInst::~COspSipInst()
{
    ClearInst();
}

void COspSipInst::ClearInst()
{
    m_tHbDst.Clear();
    m_tHbSrc = TOspTransID();
    m_wCurrHbCnt = 0;
    m_wHbTime = SIP_HB_INTERVAL;
    m_wHbCnt = SIP_HB_COUNT;
    m_strDirectUAAddr.clear();
}

void COspSipInst::DisProcess()
{
    if (CurState() != Service)
    {
        INSTLOG(OSPSIP, WARNING_LEV, "��service״̬��������SIP����!\n");
        return;
    }

    INSTLOG(OSPSIP, ERROR_LEV, "SIP�豸[%s]����!!!\n", m_tHbDst.GetURIString().c_str());
    
    //֪ͨ�ϲ�
    COspSipMsg cMsg;
    if ( m_tHbDst.GetUser() == UNIQUE_SIP_UA_PROXY )
    {
        cMsg.SetOspEventID(OSP_PROXY_DISCONNECT);
        //����Proxy����������Ϣ����ת����Daemonʵ����������ٻص����ϲ�
    }
    else
    {
        cMsg.SetOspEventID(OSP_SIP_DISCONNECT);
    }
    cMsg.SetOspIID(m_tHbSrc.dwIID);
    cMsg.SetOspTaskNO(m_tHbSrc.dwTaskID);
    cMsg.SetOspSeqNO(m_tHbSrc.dwSeqNO);
    cMsg.SetMsgBody(m_tHbDst.GetURIString().c_str(), u16(strlen(m_tHbDst.GetURIString().c_str())+1));
    SendMsgToOspApp(cMsg); 

    //ɾ���������
    g_cHbNodeList.EraseHbNode(m_tHbDst);

    //ɾ���ý���Ӧ�����񡢶Ի��󶨹�ϵ
    //�˴������κδ���
    //һ���Ե�������Ϣ���ɶ�ʱ����ʱ����
    //���ڻỰ����OSP_SIP_DISCONNECT�ص����ϲ���������

    //��������
    ClearInst();
    NextState(Idle);
}

void COspSipInst::HeartBeat()
{
    //���ڵĶ�ʱ��osp���Զ�ɾ�����������ﲻ��Ҫ�ٵ���KillTimer()
    //KillTimer(SIP_HB_TIMER);

    if (m_wCurrHbCnt >= m_wHbCnt)
    {
        DisProcess();
        return;
    }

    m_wCurrHbCnt++;

    TOspTransation tOspTrans;
    tOspTrans.dwIID = MAKEIID(GetAppID(), GetInsID());

    LPCSTR szDirectUAAddr = NULL;
    if (!m_strDirectUAAddr.empty())
    {
        szDirectUAAddr = m_strDirectUAAddr.c_str();
    }
    bool bRet = OspSipHeartbeatReq(m_tHbDst, tOspTrans, szDirectUAAddr);
    if (!bRet)
    {
        INSTLOG(OSPSIP, ERROR_LEV, "OspSipHeartbeatReq fail\n");    
    }

    SetTimer(SIP_HB_TIMER, m_wHbTime * 1000);    
}

void COspSipInst::RegProxy()
{
    KillTimer(SIP_REG_PROXY_TIMEOUT);

    SendRegProxyReq();
    
    SetTimer(SIP_REG_PROXY_TIMEOUT, SIP_REG_TIMEOUT_INTERVAL);
}

void COspSipInst::RegProxyUpdate()
{
    KillTimer(SIP_REG_PROXY_UPDATE_TIMER);

    static s32 s_nCurrExpires = g_nRegProxyExpires;
    s_nCurrExpires -= SIP_REG_UPDATE_INTERVAL;

    if (s_nCurrExpires < 2 * SIP_REG_UPDATE_INTERVAL)
    {
        //ע���Ҫ���ڣ���ʼ����
        INSTLOG(OSPSIP, EVENT_LEV, "ע��proxy[%s]���ڣ���ʼ����\n", g_tOspSipConf.tProxyAddr.GetSipAddrStr().c_str());

        SendRegProxyReq();

        s_nCurrExpires = g_nRegProxyExpires;
    }

    SetTimer(SIP_REG_PROXY_UPDATE_TIMER, SIP_REG_UPDATE_INTERVAL * 1000);
}

void COspSipInst::SendRegProxyReq()
{
    TOspTransID tOspTrans;
    tOspTrans.dwIID = MAKEIID(GetAppID(), GetInsID());

    TSipURI tProxyURI;
    if (g_tOspSipConf.tProxyURI.IsNull())
    {
        tProxyURI.SetUser(UNIQUE_SIP_UA_PROXY);
        tProxyURI.SetDomain(g_tOspSipConf.tLocalURI.GetDomain());
    }
    else
    {
        tProxyURI = g_tOspSipConf.tProxyURI;
    }

    TOspSipSendMsg* ptSipSendMsg = new TOspSipSendMsg;
    ptSipSendMsg->eSipMsgType = SIP_MSG_TYPE_REQ;
    ptSipSendMsg->eSipEvent = KDSIP_EVENT_REGISTER_REQ;
    ptSipSendMsg->tDstURI = tProxyURI;
    ptSipSendMsg->tSrcOspTrans = tOspTrans;

    //��ȡ���ص�������Ϣ������ע����Ϣ��
    CUaRouterRegReq cReq;
    cReq.SetDevUri(g_tOspSipConf.tLocalURI.GetURIString());
    cReq.SetUAType(g_tOspSipConf.tUAType);

    //U2Uģʽ��Ҫ�����ص�������Ϣ���͸�proxy
    if (g_tOspSipConf.nUaRouteMode == UA_ROUTE_MODE_U2U)
    {
        TNetIfParamList tNetIfParamList = GetNetIfParamList();
        TNetIfParamList::const_iterator itParam = tNetIfParamList.begin();
        for (; itParam != tNetIfParamList.end(); ++itParam)
        {
            TNetParam tNetParam;
            tNetParam.SetIfName(itParam->strIfName);
            tNetParam.SetNetMask(itParam->strNetMask);
            tNetParam.GetNetAddr().SetNetIp(itParam->strNetAddr);
            tNetParam.GetNetAddr().SetNetPort(g_tOspSipConf.tLocalAddr.wPort);
            cReq.GetNetParamList().push_back(tNetParam);
        }
    }

    ptSipSendMsg->strMsgBuf = cReq.ToXml();
    
    TOspSipCoreMsg tCoreMsg;
    tCoreMsg.eCoreMsgType = OSPSIP_CORE_MSG_TYPE_SEND;
    tCoreMsg.pvMsg = ptSipSendMsg;

    WriteCoreQue(tCoreMsg);
}

void COspSipInst::SendRouteSsReq()
{
    vector<CTask*> tSsTaskList = GetTasksArray(TASK_TYPE_OSPSIP_ROUTE_SSER);
    if (tSsTaskList.size() == 0)
    {
        COspsipRouteSserTask* pcSstask = new COspsipRouteSserTask(this);
        pcSstask->InitStateMachine();
        pcSstask->SetDevUri(g_tOspSipConf.tLocalURI.GetURIString());
        pcSstask->StartSs();
    }
    else if (tSsTaskList.size() == 1)
    {
        //���������Ѿ����ڣ�����Ķ�ʱ�����Լ���������
    }
    else
    {
        //�������񲻿��ܶ���һ��
        INSTLOG(OSPSIP, ERROR_LEV, "ospsip��[%u]��·�ɶ������񣬳������\n", tSsTaskList.size());
    }
}

void COspSipInst::ResetRouteSsTask()
{
    vector<CTask*> tSsTaskList = GetTasksArray(TASK_TYPE_OSPSIP_ROUTE_SSER);
    if (tSsTaskList.size() == 1)
    {
        COspsipRouteSserTask* pcSstask = (COspsipRouteSserTask*)tSsTaskList[0];
        if (pcSstask != NULL && pcSstask->GetState() == COspsipRouteSserTask::Service)
        {
            string strNullRoute;
            pcSstask->SendRouteTableToConvertor(strNullRoute);
            CMessage cOspMsg;
            cOspMsg.event = OSP_PROXY_DISCONNECT;
            pcSstask->ProcMsg(&cOspMsg);
        }
    }
}

void COspSipInst::CheckSipTrans()
{
    KillTimer(SIP_TRANS_CHECK_TIMER);

    TOspSipSendMsg* ptCheckMsg = new TOspSipSendMsg;
    ptCheckMsg->eSipMsgType = SIP_MSG_TYPE_CHECKER;

    TOspSipCoreMsg tCoreMsg;
    tCoreMsg.eCoreMsgType = OSPSIP_CORE_MSG_TYPE_SEND;
    tCoreMsg.pvMsg = ptCheckMsg;

    WriteCoreQue(tCoreMsg);
    
    SetTimer(SIP_TRANS_CHECK_TIMER, SIP_TRANS_CHECK_INTERVAL);
}

LPCSTR COspSipInst::GetStrState() const
{
    if ( GetInsID() == CInstance::DAEMON )
    {
        switch(CurState())
        {
        case DaemonIdle:
            return "DaemonIdle";
        case DaemonWaitReg:
            return "DaemonWaitReg";
        case DaemonService:
            return "DaemonService";
        default:
            break;
        }
    }
    else
    {
        switch(CurState())
        {
        case Idle:
            return "Idle";
        case Waiting:
            return "Waiting";
        case Service:
            return "Service";
        default:
            break;
        }
    }

    return "δ֪״̬";
}

void COspSipInst::ProcTaskMsg(CTask* pTask, CMessage* const pcMsg)
{
    if (pTask == NULL)
    {
        INSTLOG(OSPSIP, ERROR_LEV, "pTask == NULL---msg[%s-%u]\n", 
            OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        return;
    }

    u32 dwRet = pTask->ProcMsg(pcMsg);
    if (dwRet == PROCMSG_DEL)
    {
        INSTLOG(OSPSIP, ALL_LEV, "����[%s-%u]�������Ҫɾ��---msg[%s-%u]\n", 
            pTask->GetObjName(), pTask->GetTaskNO(),
            OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

        delete pTask;
        pTask = NULL;
    }
    else if (dwRet == PROCMSG_FAIL)
    {
        INSTLOG(OSPSIP, ERROR_LEV, "����[%s-%u]����ʧ��---msg[%s-%u]\n", 
            pTask->GetObjName(), pTask->GetTaskNO(),
            OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
    }
    else if (dwRet == PROCMSG_OK)
    {
        INSTLOG(OSPSIP, ALL_LEV, "����[%s-%u]����ɹ�---msg[%s-%u]\n", 
            pTask->GetObjName(), pTask->GetTaskNO(),
            OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
    }
    else
    {
        INSTLOG(OSPSIP, ERROR_LEV, "����[%s-%u]������δ֪---msg[%s-%u]\n", 
            pTask->GetObjName(), pTask->GetTaskNO(),
            OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
    }
}

void COspSipInst::DaemonEntry(CMessage *const pcMsg)
{
    u8 byLogLev = EVENT_LEV;
    if (pcMsg->event == SIP_HB_REQ
        || pcMsg->event == SIP_HB_RSP
        || pcMsg->event == SIP_HB_TIMER
        || pcMsg->event == SIP_REG_PROXY_UPDATE_TIMER
        || pcMsg->event == SIP_TRANS_CHECK_TIMER)
    {
        byLogLev = TIMER_LEV;
    }

    INSTLOG(OSPSIP, byLogLev, "Receive Msg[%s-%hu]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

    switch(CurState())
    {
    case DaemonIdle:
        {
            OnDaemonIdle(pcMsg);
            break;
        }

    case DaemonWaitReg:
        {
            OnDaemonWaitReg(pcMsg);
            break;
        }

    case DaemonService:
        {
            OnDaemonService(pcMsg);
            break;
        }

    default:
        {            
            INSTLOG(OSPSIP, WARNING_LEV, "unkown osp state recv Msg[%s-%hu]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }        
}

//DaemonIdle״̬��ʵ��������
void COspSipInst::OnDaemonIdle(CMessage* const pcMsg)
{
    switch(pcMsg->event)
    {
    case SIP_REG_PROXY_REQ:
        {
            s32 nExpires = *(s32*)pcMsg->content;
            g_nRegProxyExpires = nExpires;
            RegProxy();
            NextState(DaemonWaitReg);
        }
        break;

    default:
        {
            INSTLOG(OSPSIP, WARNING_LEV, "Receive unkown Msg[%s-%hu]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }
}

//DaemonWaitReg״̬��ʵ��������
void COspSipInst::OnDaemonWaitReg(CMessage* const pcMsg)
{
    switch(pcMsg->event)
    {
    case SIP_REG_PROXY_RSP:
        {
            //ע��ɹ�

            //��������������ʱ��
            SetTimer(SIP_REG_PROXY_UPDATE_TIMER, 1);

            //֪ͨ�ϲ�ҵ��ע��proxy�ɹ�
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            COspSipMsg cMsg;
            cMsg.SetOspEventID(OSP_PROXY_CONNECT);
            cMsg.SetOspIID(g_tOspSipConf.dwDefaultOspIID);
            cMsg.SetOspTaskNO(INVALID_TASKNO);
            cMsg.SetOspSeqNO(INVALID_SEQ_NO);

            //ע��ɹ�֪ͨ����д��UA��PROXYʹ�õ������ַ
            cMsg.SetSipLocalIp(pcOspSipMsg->GetSipLocalIp().c_str());
            cMsg.SetSipLocalPort(pcOspSipMsg->GetSipLocalPort());
            cMsg.SetSipRemoteIp(pcOspSipMsg->GetSipRemoteIp().c_str());
            cMsg.SetSipRemotePort(pcOspSipMsg->GetSipRemotePort());
            SendMsgToOspApp(cMsg);

            //ɾ��ע�ᶨʱ��
            KillTimer(SIP_REG_PROXY_TIMEOUT);    

            NextState(DaemonService);
            g_cHbNodeList.SetRegSipProxy(true);

            //����·����Ϣ
            if (g_tOspSipConf.nUaRouteMode == UA_ROUTE_MODE_U2U)
            {
                SendRouteSsReq();
            }

            //��ʼSIP����ʱ���
            SetTimer(SIP_TRANS_CHECK_TIMER, SIP_TRANS_CHECK_INTERVAL);

            //��һ��ע�ᵽProxy�ɹ�����Proxy������������һ�λص��ĵ�ַ��SipApp��Daemonʵ��
            TSipURI tProxyURI;
            if (g_tOspSipConf.tProxyURI.IsNull())
            {
                tProxyURI.SetUser(UNIQUE_SIP_UA_PROXY);
                tProxyURI.SetDomain(g_tOspSipConf.tLocalURI.GetDomain());
            }
            else
            {
                tProxyURI = g_tOspSipConf.tProxyURI;
            }
           
            TOspTransID tOspTransId;
            tOspTransId.dwIID = OSP_SIP_DAEMON;
            tOspTransId.dwTaskID = INVALID_TASKNO;
            tOspTransId.dwSeqNO = INVALID_SEQ_NO;
            OspSipSetHBParam(tProxyURI, tOspTransId);
        }
        break;

    case SIP_REG_PROXY_TIMEOUT:
        {        
            INSTLOG(OSPSIP, EVENT_LEV, "ע��proxy[%s]��ʱ\n", g_tOspSipConf.tProxyAddr.GetSipAddrStr().c_str());
            INSTLOG(OSPSIP, EVENT_LEV, "����ע��proxy......\n");

            RegProxy();
        }
        break;

    case SIP_TRANS_CHECK_TIMER:
        {
            CheckSipTrans();
        }
        break;

    case OSP_SIP_MSG_PROC_FAIL:
        {
            //ע��proxyʧ��
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            if (pcOspSipMsg == NULL)
            {
                INSTLOG(OSPSIP, ERROR_LEV, "ospsip ��ϢΪ��\n");
                return;
            }

            INSTLOG(OSPSIP, ERROR_LEV, "ע��proxy[%s]�յ�����Ӧ��ip status code[%d]\n",
                g_tOspSipConf.tProxyAddr.GetSipAddrStr().c_str(), pcOspSipMsg->GetSipErrorCode());
            INSTLOG(OSPSIP, EVENT_LEV, "����ע��proxy......\n");
        }
        break;

    default:
        {
            INSTLOG(OSPSIP, WARNING_LEV, "Receive unkown Msg[%s-%hu]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }    
}

//DaemonService״̬��ʵ��������
void COspSipInst::OnDaemonService(CMessage* const pcMsg)
{
    switch(pcMsg->event)
    {
    case UA_ROUTER_SS_RSP:
    case UA_ROUTER_NTF_REQ:
    case OSP_SIP_DIALOG_TERMINATE:
    case OSP_SIP_MSG_PROC_FAIL:
        {
            COspSipMsg* pSipMsg = (COspSipMsg*)pcMsg->content;
            CTask* pTask = FindTask(pSipMsg->GetOspTaskNO());
            if (pTask != NULL)
            {
                ProcTaskMsg(pTask, pcMsg);
            }
            else
            {
                INSTLOG(OSPSIP, ERROR_LEV, "taskNO[%u]û�ж�Ӧ����---msg[%s-%u]\n", 
                    pSipMsg->GetOspTaskNO(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            }
        }
        break;

    case SIP_REG_PROXY_UPDATE_TIMER:
        {
            RegProxyUpdate();
        }
        break;

    case OSP_PROXY_DISCONNECT:
        {
            //sip proxy��������
            g_cHbNodeList.SetRegSipProxy(false);

            //1. ɾ��������ʱ��        
            KillTimer(SIP_REG_PROXY_UPDATE_TIMER);

            //2. ֪ͨ�ϲ�ҵ��proxy����
            COspSipMsg cMsg;
            cMsg.SetOspEventID(OSP_PROXY_DISCONNECT);
            cMsg.SetOspIID(g_tOspSipConf.dwDefaultOspIID);
            cMsg.SetOspTaskNO(INVALID_TASKNO);
            cMsg.SetOspSeqNO(INVALID_SEQ_NO);
            SendMsgToOspApp(cMsg);

            //3. ���³���ע��proxy
            NextState(DaemonWaitReg);
            RegProxy();

            //4. ����·�ɶ�������
            if (g_tOspSipConf.nUaRouteMode == UA_ROUTE_MODE_U2U)
            {
                ResetRouteSsTask();
            }  
        }
        break;

    case SIP_TRANS_CHECK_TIMER:
        {
            CheckSipTrans();
        }
        break;

    case SIP_REG_PROXY_RSP:
        {
            //�����ɹ�
        }
        break;

    default:
        {
            INSTLOG(OSPSIP, WARNING_LEV, "Receive unkown Msg[%s-%hu]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }    
}

void COspSipInst::NormalEntry(CMessage *const pcMsg)
{
    u8 byLogLev = EVENT_LEV;
    if (pcMsg->event == SIP_HB_REQ
        || pcMsg->event == SIP_HB_RSP
        || pcMsg->event == SIP_HB_TIMER)
    {
        byLogLev = TIMER_LEV;
    }

    INSTLOG(OSPSIP, byLogLev, "Receive Msg[%s-%hu]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

    switch(CurState())
    {
    case Idle:
        {
            OnIdle(pcMsg);
            break;
        }

    case Service:
        {
            OnService(pcMsg);
            break;
        }

    default:
        {            
            INSTLOG(OSPSIP, WARNING_LEV, "unkown osp state recv Msg[%s-%hu]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }    
}

void COspSipInst::OnIdle(CMessage* const pcMsg)
{
    switch(pcMsg->event)
    {
    case SIP_HB_ADD_NODE:
        {
            const char* pStr = (const char*)pcMsg->content;
            m_tHbDst.SetNOString(pStr);
            TOspSipHbNode* pHbNode = g_cHbNodeList.FindHbNode(m_tHbDst);
            if (pHbNode == NULL)
            {
                INSTLOG(OSPSIP, ERROR_LEV, "URI[%s]�����ڶ�Ӧ���������\n", pStr);
                return;
            }

            m_tHbSrc = pHbNode->tHbSrc;
            m_wHbCnt = pHbNode->wHbCnt;
            m_wHbTime = pHbNode->wHbTime;
            m_strDirectUAAddr = pHbNode->strDirectUAAddr;
            pHbNode->wHbWorkInst = GetInsID();
            m_wCurrHbCnt = 0;
            
            HeartBeat();

            NextState(Service);
        }
        break;

    default:
        {
            INSTLOG(OSPSIP, WARNING_LEV, "Receive unkown Msg[%s-%hu]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }    
}

void COspSipInst::OnService(CMessage* const pcMsg)
{
    switch(pcMsg->event)
    {
    case SIP_HB_TIMER:
        {
            HeartBeat();            
        }
        break;

    case SIP_HB_RSP:
        {
            m_wCurrHbCnt = 0;
        }
        break;

    case OSP_SIP_MSG_PROC_FAIL:
        {
            //��������Ӧ��
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            if (pcOspSipMsg == NULL)
            {
                INSTLOG(OSPSIP, ERROR_LEV, "ospsip ��ϢΪ��\n");
                return;
            }

            INSTLOG(OSPSIP, ERROR_LEV, "��Ŀ��[%s]��������յ�����Ӧ��sip status code[%d]\n",
                m_tHbDst.GetURIString().c_str(), pcOspSipMsg->GetSipErrorCode());
        }
        break;

    case SIP_HB_ADD_NODE:
        {
            //�����������
            LPCSTR pStr = (LPCSTR)pcMsg->content;
            TSipURI tDstURI;
            tDstURI.SetNOString(pStr);

            TOspSipHbNode* pHbNode = g_cHbNodeList.FindHbNode(tDstURI);
            if (pHbNode != NULL && pHbNode->wHbWorkInst == GetInsID())
            {
                m_tHbSrc = pHbNode->tHbSrc;
                m_wHbCnt = pHbNode->wHbCnt;
                m_wHbTime = pHbNode->wHbTime;
                m_strDirectUAAddr = pHbNode->strDirectUAAddr;
            }
            else
            {
                INSTLOG(OSPSIP, ERROR_LEV, "��Ч�������[%s], ����ʧ��\n",
                    tDstURI.GetURIString().c_str());
            }
        }
        break;

    case SIP_HB_DEL_NODE:
        {
            LPCSTR pStr = (LPCSTR)pcMsg->content;
            TSipURI tDstURI;
            tDstURI.SetNOString(pStr);

            if (tDstURI != m_tHbDst)
            {
                INSTLOG(OSPSIP, ERROR_LEV, "����Ŀ�겻ƥ�䣬ɾ������ʧ��\n");
                return;
            }
            else
            {
                g_cHbNodeList.EraseHbNode(tDstURI);
                ClearInst();
                NextState(Idle);
            }
        }
        break;

    default:
        {
            INSTLOG(OSPSIP, WARNING_LEV, "Receive unkown Msg[%s-%hu]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }
}

void InitOspSipMsgDesc()
{    
    //�ⲿ��Ϣ

    //SIP����
    OSP_ADD_EVENT_DESC(OSP_SIP_DISCONNECT);

    //Proxy����֪ͨ
    OSP_ADD_EVENT_DESC(OSP_PROXY_DISCONNECT);

    //Proxyע��ɹ�֪ͨ
    OSP_ADD_EVENT_DESC(OSP_PROXY_CONNECT);

    //SIP BYE����
    OSP_ADD_EVENT_DESC(OSP_SIP_BYE_REQ);

    //SIP BYEӦ��
    OSP_ADD_EVENT_DESC(OSP_SIP_BYE_RSP);

    //SIP�㴦�����
    OSP_ADD_EVENT_DESC(OSP_SIP_MSG_PROC_FAIL);

    //SIP��Dialog�쳣��ֹ֪ͨ
    OSP_ADD_EVENT_DESC(OSP_SIP_DIALOG_TERMINATE);

    //ospsip֪ͨ�ϲ����¼��������ļ�
    OSP_ADD_EVENT_DESC(OSP_SIP_SIGNAL_RELOAD_CFG);

    OSP_ADD_EVENT_DESC(OSP_SIP_GB28181_MSG);

    //�ڲ���Ϣ

    //SIP��������
    OSP_ADD_EVENT_DESC(SIP_HB_REQ);

    //SIP����Ӧ��
    OSP_ADD_EVENT_DESC(SIP_HB_RSP);

    //������ʱ��
    OSP_ADD_EVENT_DESC(SIP_HB_TIMER);                 //��Ϣ�壺 

    //����һ���������
    OSP_ADD_EVENT_DESC(SIP_HB_ADD_NODE);             //��Ϣ�壺

    //ɾ��һ���������
    OSP_ADD_EVENT_DESC(SIP_HB_DEL_NODE);             //��Ϣ�壺

    //ע��PROXY����
    OSP_ADD_EVENT_DESC(SIP_REG_PROXY_REQ);

    //ע��PROXYӦ��
    OSP_ADD_EVENT_DESC(SIP_REG_PROXY_RSP);

    //ע��PROXY��ʱ��ʱ��
    OSP_ADD_EVENT_DESC(SIP_REG_PROXY_TIMEOUT);

    //ע��PROXY������ʱ��
    OSP_ADD_EVENT_DESC(SIP_REG_PROXY_UPDATE_TIMER);

    //SIP����ʱ��ⶨʱ��
    OSP_ADD_EVENT_DESC(SIP_TRANS_CHECK_TIMER);

    //Э����Ϣ
    //UA·�ɵǼ�����
    OSP_ADD_EVENT_DESC(UA_ROUTER_REG_REQ);        //��Ϣ��: CUaRouterRegReq
    OSP_ADD_EVENT_DESC(UA_ROUTER_REG_RSP);        //��Ϣ��: CUaRouterRegRsp

    //UA·�ɶ�������
    OSP_ADD_EVENT_DESC(UA_ROUTER_SS_REQ);         //��Ϣ��: CUaRouterSsReq
    OSP_ADD_EVENT_DESC(UA_ROUTER_SS_RSP);         //��Ϣ��: CUaRouterSsRsp

    //UA·��֪ͨ����
    OSP_ADD_EVENT_DESC(UA_ROUTER_NTF_REQ);        //��Ϣ��: CUaRouterNtfReq
    OSP_ADD_EVENT_DESC(UA_ROUTER_NTF_RSP);        //��Ϣ��: CUaRouterNtfRsp
}


