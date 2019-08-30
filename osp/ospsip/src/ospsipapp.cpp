
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
        INSTLOG(OSPSIP, WARNING_LEV, "非service状态，不处理SIP断链!\n");
        return;
    }

    INSTLOG(OSPSIP, ERROR_LEV, "SIP设备[%s]断链!!!\n", m_tHbDst.GetURIString().c_str());
    
    //通知上层
    COspSipMsg cMsg;
    if ( m_tHbDst.GetUser() == UNIQUE_SIP_UA_PROXY )
    {
        cMsg.SetOspEventID(OSP_PROXY_DISCONNECT);
        //对于Proxy断链，该消息会先转发给Daemon实例处理完后，再回调给上层
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

    //删除心跳结点
    g_cHbNodeList.EraseHbNode(m_tHbDst);

    //删除该结点对应的事务、对话绑定关系
    //此处不做任何处理
    //一次性的事务消息，由定时器超时清理
    //长期会话，由OSP_SIP_DISCONNECT回调给上层事务清理

    //断链处理
    ClearInst();
    NextState(Idle);
}

void COspSipInst::HeartBeat()
{
    //到期的定时器osp会自动删除，所以这里不需要再调用KillTimer()
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
        //注册快要到期，开始续航
        INSTLOG(OSPSIP, EVENT_LEV, "注册proxy[%s]到期，开始续航\n", g_tOspSipConf.tProxyAddr.GetSipAddrStr().c_str());

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

    //获取本地的网络信息，构造注册消息体
    CUaRouterRegReq cReq;
    cReq.SetDevUri(g_tOspSipConf.tLocalURI.GetURIString());
    cReq.SetUAType(g_tOspSipConf.tUAType);

    //U2U模式需要将本地的网络信息发送给proxy
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
        //订阅事务已经存在，事务的定时器会自己开启订阅
    }
    else
    {
        //订阅事务不可能多余一个
        INSTLOG(OSPSIP, ERROR_LEV, "ospsip有[%u]个路由订阅事务，程序出错\n", tSsTaskList.size());
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

    return "未知状态";
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
        INSTLOG(OSPSIP, ALL_LEV, "事务[%s-%u]处理后需要删除---msg[%s-%u]\n", 
            pTask->GetObjName(), pTask->GetTaskNO(),
            OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

        delete pTask;
        pTask = NULL;
    }
    else if (dwRet == PROCMSG_FAIL)
    {
        INSTLOG(OSPSIP, ERROR_LEV, "事务[%s-%u]处理失败---msg[%s-%u]\n", 
            pTask->GetObjName(), pTask->GetTaskNO(),
            OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
    }
    else if (dwRet == PROCMSG_OK)
    {
        INSTLOG(OSPSIP, ALL_LEV, "事务[%s-%u]处理成功---msg[%s-%u]\n", 
            pTask->GetObjName(), pTask->GetTaskNO(),
            OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
    }
    else
    {
        INSTLOG(OSPSIP, ERROR_LEV, "事务[%s-%u]处理结果未知---msg[%s-%u]\n", 
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

//DaemonIdle状态下实例处理函数
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

//DaemonWaitReg状态下实例处理函数
void COspSipInst::OnDaemonWaitReg(CMessage* const pcMsg)
{
    switch(pcMsg->event)
    {
    case SIP_REG_PROXY_RSP:
        {
            //注册成功

            //立即开启续航定时器
            SetTimer(SIP_REG_PROXY_UPDATE_TIMER, 1);

            //通知上层业务注册proxy成功
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            COspSipMsg cMsg;
            cMsg.SetOspEventID(OSP_PROXY_CONNECT);
            cMsg.SetOspIID(g_tOspSipConf.dwDefaultOspIID);
            cMsg.SetOspTaskNO(INVALID_TASKNO);
            cMsg.SetOspSeqNO(INVALID_SEQ_NO);

            //注册成功通知中填写该UA和PROXY使用的网络地址
            cMsg.SetSipLocalIp(pcOspSipMsg->GetSipLocalIp().c_str());
            cMsg.SetSipLocalPort(pcOspSipMsg->GetSipLocalPort());
            cMsg.SetSipRemoteIp(pcOspSipMsg->GetSipRemoteIp().c_str());
            cMsg.SetSipRemotePort(pcOspSipMsg->GetSipRemotePort());
            SendMsgToOspApp(cMsg);

            //删除注册定时器
            KillTimer(SIP_REG_PROXY_TIMEOUT);    

            NextState(DaemonService);
            g_cHbNodeList.SetRegSipProxy(true);

            //订阅路由信息
            if (g_tOspSipConf.nUaRouteMode == UA_ROUTE_MODE_U2U)
            {
                SendRouteSsReq();
            }

            //开始SIP事务超时检测
            SetTimer(SIP_TRANS_CHECK_TIMER, SIP_TRANS_CHECK_INTERVAL);

            //第一次注册到Proxy成功，把Proxy设置心跳，第一次回调的地址是SipApp的Daemon实例
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
            INSTLOG(OSPSIP, EVENT_LEV, "注册proxy[%s]超时\n", g_tOspSipConf.tProxyAddr.GetSipAddrStr().c_str());
            INSTLOG(OSPSIP, EVENT_LEV, "重试注册proxy......\n");

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
            //注册proxy失败
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            if (pcOspSipMsg == NULL)
            {
                INSTLOG(OSPSIP, ERROR_LEV, "ospsip 消息为空\n");
                return;
            }

            INSTLOG(OSPSIP, ERROR_LEV, "注册proxy[%s]收到错误应答，ip status code[%d]\n",
                g_tOspSipConf.tProxyAddr.GetSipAddrStr().c_str(), pcOspSipMsg->GetSipErrorCode());
            INSTLOG(OSPSIP, EVENT_LEV, "重试注册proxy......\n");
        }
        break;

    default:
        {
            INSTLOG(OSPSIP, WARNING_LEV, "Receive unkown Msg[%s-%hu]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }    
}

//DaemonService状态下实例处理函数
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
                INSTLOG(OSPSIP, ERROR_LEV, "taskNO[%u]没有对应事务---msg[%s-%u]\n", 
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
            //sip proxy断链处理
            g_cHbNodeList.SetRegSipProxy(false);

            //1. 删除续航定时器        
            KillTimer(SIP_REG_PROXY_UPDATE_TIMER);

            //2. 通知上层业务proxy断链
            COspSipMsg cMsg;
            cMsg.SetOspEventID(OSP_PROXY_DISCONNECT);
            cMsg.SetOspIID(g_tOspSipConf.dwDefaultOspIID);
            cMsg.SetOspTaskNO(INVALID_TASKNO);
            cMsg.SetOspSeqNO(INVALID_SEQ_NO);
            SendMsgToOspApp(cMsg);

            //3. 重新尝试注册proxy
            NextState(DaemonWaitReg);
            RegProxy();

            //4. 重置路由订阅事务
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
            //续航成功
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
                INSTLOG(OSPSIP, ERROR_LEV, "URI[%s]不存在对应的心跳结点\n", pStr);
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
            //心跳错误应答
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            if (pcOspSipMsg == NULL)
            {
                INSTLOG(OSPSIP, ERROR_LEV, "ospsip 消息为空\n");
                return;
            }

            INSTLOG(OSPSIP, ERROR_LEV, "和目的[%s]心跳检测收到错误应答，sip status code[%d]\n",
                m_tHbDst.GetURIString().c_str(), pcOspSipMsg->GetSipErrorCode());
        }
        break;

    case SIP_HB_ADD_NODE:
        {
            //更新心跳结点
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
                INSTLOG(OSPSIP, ERROR_LEV, "无效心跳结点[%s], 更新失败\n",
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
                INSTLOG(OSPSIP, ERROR_LEV, "心跳目标不匹配，删除心跳失败\n");
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
    //外部消息

    //SIP断链
    OSP_ADD_EVENT_DESC(OSP_SIP_DISCONNECT);

    //Proxy断链通知
    OSP_ADD_EVENT_DESC(OSP_PROXY_DISCONNECT);

    //Proxy注册成功通知
    OSP_ADD_EVENT_DESC(OSP_PROXY_CONNECT);

    //SIP BYE请求
    OSP_ADD_EVENT_DESC(OSP_SIP_BYE_REQ);

    //SIP BYE应答
    OSP_ADD_EVENT_DESC(OSP_SIP_BYE_RSP);

    //SIP层处理错误
    OSP_ADD_EVENT_DESC(OSP_SIP_MSG_PROC_FAIL);

    //SIP层Dialog异常终止通知
    OSP_ADD_EVENT_DESC(OSP_SIP_DIALOG_TERMINATE);

    //ospsip通知上层重新加载配置文件
    OSP_ADD_EVENT_DESC(OSP_SIP_SIGNAL_RELOAD_CFG);

    OSP_ADD_EVENT_DESC(OSP_SIP_GB28181_MSG);

    //内部消息

    //SIP心跳请求
    OSP_ADD_EVENT_DESC(SIP_HB_REQ);

    //SIP心跳应答
    OSP_ADD_EVENT_DESC(SIP_HB_RSP);

    //心跳定时器
    OSP_ADD_EVENT_DESC(SIP_HB_TIMER);                 //消息体： 

    //增加一个心跳结点
    OSP_ADD_EVENT_DESC(SIP_HB_ADD_NODE);             //消息体：

    //删除一个心跳结点
    OSP_ADD_EVENT_DESC(SIP_HB_DEL_NODE);             //消息体：

    //注册PROXY请求
    OSP_ADD_EVENT_DESC(SIP_REG_PROXY_REQ);

    //注册PROXY应答
    OSP_ADD_EVENT_DESC(SIP_REG_PROXY_RSP);

    //注册PROXY超时定时器
    OSP_ADD_EVENT_DESC(SIP_REG_PROXY_TIMEOUT);

    //注册PROXY续航定时器
    OSP_ADD_EVENT_DESC(SIP_REG_PROXY_UPDATE_TIMER);

    //SIP事务超时检测定时器
    OSP_ADD_EVENT_DESC(SIP_TRANS_CHECK_TIMER);

    //协议消息
    //UA路由登记请求
    OSP_ADD_EVENT_DESC(UA_ROUTER_REG_REQ);        //消息体: CUaRouterRegReq
    OSP_ADD_EVENT_DESC(UA_ROUTER_REG_RSP);        //消息体: CUaRouterRegRsp

    //UA路由订阅请求
    OSP_ADD_EVENT_DESC(UA_ROUTER_SS_REQ);         //消息体: CUaRouterSsReq
    OSP_ADD_EVENT_DESC(UA_ROUTER_SS_RSP);         //消息体: CUaRouterSsRsp

    //UA路由通知请求
    OSP_ADD_EVENT_DESC(UA_ROUTER_NTF_REQ);        //消息体: CUaRouterNtfReq
    OSP_ADD_EVENT_DESC(UA_ROUTER_NTF_RSP);        //消息体: CUaRouterNtfRsp
}


