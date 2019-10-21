#include "cctrl.h"
#include "inner_common.h"
#include "tasks/tac_ssn_task.h"
#include "tasks/cmu_ssn_task.h"
#include "tasks/cui_ssn_task.h"
#include "tasks/child_tas_ssn_task.h"
#include "tasks/p2p_tas_ssn_task.h"
#include "tasks/ps_ssn_task.h"
#include "tasks/p2p_tas_conn_task.h"
#include "tasks/parent_tas_conn_task.h"
#include "db_task_pool.h"
#include "tasks/tas_conn_task_poweron.h"
#include "tasks/common/hb_detect_multi_task.h"
#include "tasks/other_tas_domainname_changentf.h"
#include "tasks/common/ssn_taskbase.h"
#include "cms/cmu/cmu_proto.h"
#include "gbs/gbs_proto.h"
#include "cms/commonconfig/cmcfg.h"
#include "license_mng.h"
#include "redis/fastdbmng.h"
#include "gbs/gbs_event.h"

extern CDbTaskPool g_cDbTaskPool;

CCtrlApp g_cCtrlApp;
CCtrlAppData *g_pcAppData = NULL;


CCtrlInst::~CCtrlInst()
{
    /* 我们主动找到未释放的会话任务和连接任务析构(Inst先于AppData析构) */

    const map<string, CTask*> &cSsnTasks = GetAppData().GetSsnId2TaskMap();
    while (!cSsnTasks.empty())
    {
        delete (cSsnTasks.begin()->second);
    }

    set<CConnTaskbase*> cConnTasks = GetAppData().GetTasConnTaskSet();
    for (set<CConnTaskbase*>::const_iterator it = cConnTasks.begin(); it != cConnTasks.end(); ++it)
    {
        delete (*it);
    }
}

void CCtrlInst::DaemonEntry( CMessage * const pcMsg )
{
    u16 wEvent = pcMsg->event;
    Log(enLogSrcCCtrl, EVENT_LEV, "CCtrlInst::DaemonEntry recved message[%u-%s], curstate:%u\n", wEvent, OspExtEventDesc(wEvent).c_str(), CurState());

    if (!ValidateMsg(pcMsg)) 
    {
        Log(enLogSrcCCtrl, WARNING_LEV, "CCtrlInst::DaemonEntry recved invalid message[%u-%s]\n", wEvent, OspExtEventDesc(wEvent).c_str());
        return;
    }
    if (CurState() == enStateServer)
    {
        ServerStateHandle(pcMsg);
    }
    else if (CurState() == enStateRegProxy)
    {
        RegProxyStateHandle(pcMsg);
    }
    else if (CurState() == enStateInit)
    {
        InitStateHandle(pcMsg);
    }
    else if (CurState() == enStateIdle)
    {
        IdleStateHandle(pcMsg);
    }
    else
    {
        ExitStateHandle(pcMsg);
    }
}


void CCtrlInst::IdleStateHandle( CMessage* const pcMsg )
{
    if (OSP_POWERON == pcMsg->event)
    {
        OnPowerOn();
    }
}


void CCtrlInst::InitStateHandle( CMessage* const pcMsg )
{
    if (msgPoweronReadyNtf == pcMsg->event)
    {
        u32 dwModuleId = *reinterpret_cast<u32*>(pcMsg->content);
        m_cWaitPoweronReadyModuleIds.erase(dwModuleId);

        if (m_cWaitPoweronReadyModuleIds.empty())
        {
            NextState(enStateRegProxy);
            // 注册proxy
            if (!OspSipRegProxy(3600))
            { 
                // 失败了简单打印一下错误即可(无需重复注册)，底层会不断尝试的
                Log(enLogSrcCCtrl, CRITICAL_LEV, "OspSipRegProxy failed\n");
            }

            // 准备连接任务
            CTasConnTaskPowerOn *pcConnTask = new CTasConnTaskPowerOn(this);
            pcConnTask->InitStateMachine();
        }
    }
    else if (TAS_INNER_MSG_BEGIN <= pcMsg->event && pcMsg->event <= TAS_INNER_MSG_END)
    {
        OnInnerOspMsg(pcMsg);
    }
}


void CCtrlInst::RegProxyStateHandle( CMessage* const pcMsg )
{
    switch(pcMsg->event)
    {
    case OSP_PROXY_CONNECT:
        OnProxyConnect();
        break;
    case OSP_SIP_DISCONNECT:
        OnSipDisconnect(pcMsg);
        break;
    case OSP_SIP_MSG_PROC_FAIL:
        OnSipMsgProcFail(pcMsg);
        break;
    case TAS_LOGOUT_REQ:
        OnLogout(pcMsg);
        break;
    default:
        {
            if (TAS_INNER_MSG_BEGIN <= pcMsg->event && pcMsg->event <= TAS_INNER_MSG_END)
            {
                OnInnerOspMsg(pcMsg);
            }
        }
        break;
    }
}

void CCtrlInst::ServerStateHandle( CMessage* const pcMsg )
{
    switch(pcMsg->event)
    {
    case OSP_PROXY_DISCONNECT:
        OnProxyDisconnect();
        break;
    case OSP_SIP_DISCONNECT:
        OnSipDisconnect(pcMsg);
        break;
    case OSP_SIP_MSG_PROC_FAIL:
        OnSipMsgProcFail(pcMsg);
        break;
    case TAS_LOGIN_REQ:
        OnLogin(pcMsg);
        break;
    case TAS_LOGOUT_REQ:
        OnLogout(pcMsg);
        break;
    case TAS_DOMAIN_RELATION_MOD_NTF_REQ:
        {
            COtherTasDomainnameChangeNtf *pcTask = new COtherTasDomainnameChangeNtf(this);
			pcTask->InitStateMachine();
            pcTask->ProcMsg(pcMsg);
            delete pcTask;
        }
        break;
    case TAS_LICENSE_UPDATE_RSP:
        {
            COspSipMsg* const pcSipMsg = reinterpret_cast<COspSipMsg* const>(pcMsg->content);
            CLicenseUpdateRsp cRsp;
            pcSipMsg->GetMsgBody(cRsp);

            TOspTransation tOspTrans;
            tOspTrans.dwIID = MAKEIID(GetAppID(), GetInsID());
            tOspTrans.dwTaskID = INVALID_TASKNO;
            tOspTrans.dwSeqNO = cRsp.GetSeqNum();

            OspSipPostRsp(KDSIP_EVENT_MESSAGE_RSP, pcSipMsg->GetSipTransID(), cRsp, tOspTrans);
        }
        break;
    case TAS_LICENSE_SET_RSP:
        {
            COspSipMsg* const pcSipMsg = reinterpret_cast<COspSipMsg* const>(pcMsg->content);
            CLicenseSetRsp cRsp;
            pcSipMsg->GetMsgBody(cRsp);

            TOspTransation tOspTrans;
            tOspTrans.dwIID = MAKEIID(GetAppID(), GetInsID());
            tOspTrans.dwTaskID = INVALID_TASKNO;
            tOspTrans.dwSeqNO = cRsp.GetSeqNum();

            OspSipPostRsp(KDSIP_EVENT_MESSAGE_RSP, pcSipMsg->GetSipTransID(), cRsp, tOspTrans);
        }
        break;
    case NETADDR_QUERY_REQ:
        {
            CNetAddrQueryReq cReq;
            COspSipMsg *pcSipMsg = reinterpret_cast<COspSipMsg*>(pcMsg->content);
            pcSipMsg->GetMsgBody(cReq);

            CNetAddrQueryRsp cRsp;
            cRsp.SetSession(cReq.GetSession());
            cRsp.SetSeqNum(cReq.GetSeqNum());
            TNetAddr tNetAddr;
            tNetAddr.SetNetIp(GetLocalIPAddress());
            cRsp.GetDevAddrList().push_back(tNetAddr);

            OspSipPostRsp(KDSIP_EVENT_MESSAGE_RSP, pcSipMsg->GetSipTransID(), cRsp, pcSipMsg->GetOspTransID());
        }
        break;
	case TAS_DEVICE_BATCH_CHANGE_REQ:
		{
			CTasDeviceBatchChangeReq cDevBatchReq;
			cDevBatchReq.FromXml(string(reinterpret_cast<s8*>(pcMsg->content)));
			CTask *pcTask = GetAppData().FindTaskBySsnId(cDevBatchReq.loginsession);
			if (!pcTask)
			{
				 Log(ERROR_LEV, "can't find session task by ssnId[%s], throw TAS_DEVICE_BATCH_CHANGE_REQ \n",
					 cDevBatchReq.loginsession.c_str());
				 break;
			}
			CTaskMsgTransfer<CTasDeviceBatchChangeReq> cTrans(pcTask->GetTaskNO(), TKDSIP_TRANS_ID(), TAS_DEVICE_BATCH_CHANGE_REQ);

			cTrans.SetMsgObj(cDevBatchReq);

			if (!PostTaskMsgTransfer2DbTaskPool(cTrans))
			{
				Log(ERROR_LEV, "PostTaskMsgTransfer2DbTaskPool TAS_DEVICE_BATCH_CHANGE_REQ fail \n");
				CTasDeviceBatchChangeRsp cRsp;
				cRsp.taskno = cDevBatchReq.taskno;
				for (u32 dwIndex = 0; dwIndex < cDevBatchReq.Items.size(); ++dwIndex)
				{
					cRsp.Items.push_back(ERR_TAS_SYSTEM_BUSY);
				}
				RedisPublishDeviceBatchRsp(cDevBatchReq.gbuId, cRsp);
			}
		}
		break;
	case GBS_DEV_GRP_GBID_BATCH_GET_RSP:
	case GBS_DEV_GBID_BATCH_GET_RSP:
		{
			 COspSipMsg* const pcSipMsg = reinterpret_cast<COspSipMsg* const>(pcMsg->content);
			 CTask *pTask = FindTask(pcSipMsg->GetOspTaskNO());
			 if (pTask)
			 {
				 if(PROCMSG_DEL == pTask->ProcMsg(pcMsg))
				 {
					 delete pTask;
				 }
			 }
			 else
			 {
				 Log(ERROR_LEV, "find no task[%d] to proc GBS_DEVGRP_CIVILCODE_GET_RSP \n", pcSipMsg->GetOspTaskNO());
			 }
		}
		break;
    case OSP_SIP_BYE_REQ:
    case OSP_SIP_BYE_RSP:
        OnExternSipMsg(pcMsg);
        break;
    default:
        {
            if ((TAS_EXTERN_MSG_BEGIN <= pcMsg->event && pcMsg->event <= TAS_EXTERN_MSG_END)
                || (CMS_EVENT_BEGIN <= pcMsg->event && pcMsg->event <= CMS_EVENT_END)
				|| (GBS_EVENT_BEGIN <= pcMsg->event && pcMsg->event <= GBS_EVENT_END))
            {
                OnExternSipMsg(pcMsg);
            }
            else if (TAS_INNER_MSG_BEGIN <= pcMsg->event && pcMsg->event <= TAS_INNER_MSG_END)
            {
                OnInnerOspMsg(pcMsg);
            }
        }
        break;
    }
}

void CCtrlInst::ExitStateHandle( CMessage* const pcMsg )
{
    if (pcMsg->event == msgPoweroffReadyNtf)
    {
        u32 dwModuleId = *reinterpret_cast<u32*>(pcMsg->content);
        m_cWaitPoweroffReadyModuleIds.erase(dwModuleId);

        if (m_cWaitPoweroffReadyModuleIds.empty())
        {
            g_cDbTaskPool.Quit();

            extern SEMHANDLE g_hTasExitSem;
            OspSemGive(g_hTasExitSem);
        }
    }
    else
    {
        // 其它的消息直接丢弃
    }
}

void CCtrlInst::OnPowerOn()
{
    NextState(enStateInit);

    if (!g_cDbTaskPool.Init())
    {
        Log(enLogSrcCCtrl, ERROR_LEV, "DbTaskPool init failed...\n");
    }

    m_cWaitPoweronReadyModuleIds.insert(AID_License);
    m_cWaitPoweronReadyModuleIds.insert(AID_SyncAnalyze);

    post(MAKEIID(AID_License, CInstance::DAEMON), OSP_POWERON);
    post(MAKEIID(AID_SyncAnalyze, CInstance::DAEMON), OSP_POWERON);

    CHbDetectMultiTask *pcHbDetectTask = new CHbDetectMultiTask(this);
    pcHbDetectTask->InitStateMachine();
}

void CCtrlInst::OnTasExit()
{
    NextState(enStateExit);

    m_cWaitPoweroffReadyModuleIds.insert(AID_SyncAnalyze);
    post(MAKEIID(AID_SyncAnalyze, CInstance::DAEMON), msgTasExitCmd);
}

void CCtrlInst::OnLogin( CMessage* const pcMsg )
{
    CLoginTasReq cLoginReq;
    CLoginTasReq::TLoginType tLoginType;

    reinterpret_cast<COspSipMsg* const>(pcMsg->content)->GetMsgBody(cLoginReq);
    tLoginType = cLoginReq.GetLoginType();

    Log(enLogSrcCCtrl, CRITICAL_LEV, "LoginReq Content(%d, %s, %s, %s)\n", cLoginReq.GetLoginType(), cLoginReq.GetUserName().c_str(), 
               cLoginReq.GetPassword().c_str(), cLoginReq.GetExtraInfo().c_str());
    CTask *pcTask = NULL;

    switch (tLoginType)
    {
    case CLoginTasReq::enLoginType_TAC:
        pcTask = new CTacSsnTask(this);
        break;
    case CLoginTasReq::enLoginType_CMU:
        pcTask = new CCmuSsnTask(this);
        break;
    case CLoginTasReq::enLoginType_CUI:
        pcTask = new CCuiSsnTask(this);
        break;
    case CLoginTasReq::enLoginType_SubTAS:
        pcTask = new CChildTasSsnTask(this);
        break;
    case CLoginTasReq::enLoginType_P2PTAS:
        pcTask = new CP2PTasSsnTask(this);
        break;
    case CLoginTasReq::enLoginType_Peripheral:
        pcTask = new CPsSsnTask(this);
        break;
    default:
        Log(enLogSrcCCtrl, WARNING_LEV, "unknown login type, type=%d\n", tLoginType);
        break;
    }

    if (NULL != pcTask)
    {
        pcTask->InitStateMachine(); // 初始化状态机
        if (PROCMSG_DEL == pcTask->ProcMsg(pcMsg))
            delete pcTask;
    }
}

void CCtrlInst::OnLogout( CMessage* const pcMsg )
{
    CLogoutTasReq cReq;
    reinterpret_cast<COspSipMsg* const>(pcMsg->content)->GetMsgBody(cReq);

    CTask *pcTask = GetAppData().FindTaskBySsnId(GetSipMsgSessionId(pcMsg));
    if (NULL != pcTask)
    {
        if (PROCMSG_DEL == pcTask->ProcMsg(pcMsg))
            delete pcTask;
    }
    else
    {
        Log(enLogSrcCCtrl, WARNING_LEV, "找不到注销消息对应的任务\n");
    }
}

void CCtrlInst::OnSipDisconnect( CMessage* const pcMsg )
{
    COspSipMsg *pcSipMsg = reinterpret_cast<COspSipMsg*>(pcMsg->content);
    Log(enLogSrcCCtrl, CRITICAL_LEV, "recved OSP_SIP_DISCONNECT msg, peer uri=%s\n", pcSipMsg->GetMsgBody());
    CTask *pcTask = FindTask(pcSipMsg->GetOspTaskNO());
    if ( NULL == pcTask )
    {
        Log(enLogSrcCCtrl, WARNING_LEV, "找不到断链节点的任务, 对端的Sip Uri=%s, 任务号=%u\n", pcSipMsg->GetMsgBody(), pcSipMsg->GetOspTaskNO());
    }
    else
    {
        if (PROCMSG_DEL == pcTask->ProcMsg(pcMsg))
            delete pcTask;
    }
}

void CCtrlInst::OnSipMsgProcFail( CMessage* const pcMsg )
{
    COspSipMsg *pcSipMsg = reinterpret_cast<COspSipMsg*>(pcMsg->content);
    CTask *pcTask = FindTask(pcSipMsg->GetOspTaskNO());
    if ( NULL == pcTask )
    {
        Log(enLogSrcCCtrl, WARNING_LEV, "找不到SIP消息处理出错的任务\n");
    }
    else
    {
        if (PROCMSG_DEL == pcTask->ProcMsg(pcMsg))
            delete pcTask;
    }
}

void CCtrlInst::OnExternSipMsg( CMessage* const pcMsg )
{
    // 请求消息利用SessionId定位到Ssn Task, 应答消息利用taskid定位到task
    CTask *pcTask;

    COspSipMsg *pcSipMsg = reinterpret_cast<COspSipMsg*>(pcMsg->content);
    if (!IsRspType(pcMsg->event))
    {
        pcTask = GetAppData().FindTaskBySsnId(GetSipMsgSessionId(pcMsg));
        // 回复一个通用的应答
        if (NULL == pcTask)
        {
            string strSeqNum = GetSipSeqNO(pcSipMsg->GetMsgBody());
            CEventRsp cRsp;
            cRsp.SetSeqNum(atoi(strSeqNum.c_str()));
            cRsp.SetSession(GetSipSessionID(pcSipMsg->GetMsgBody()));
            cRsp.SetEvent(pcMsg->event+1);

            if (CMS_SUCCESS != g_enLicenseValidateResult)
            {
                cRsp.SetErrorCode(g_enLicenseValidateResult);
            }
            else
            {
                cRsp.SetErrorCode(ERR_INVALID_SESSIONID);
            }

            TOspTransation tOspTrans;
            tOspTrans.dwIID = MAKEIID(GetAppID(), GetInsID());
            tOspTrans.dwTaskID = INVALID_TASKNO;
            tOspTrans.dwSeqNO = cRsp.GetSeqNum();

            OspSipPostRsp(KDSIP_EVENT_MESSAGE_RSP, pcSipMsg->GetSipTransID(), cRsp.ToXml(), tOspTrans);
        }
    }
    else
    {
        pcTask = FindTask(pcSipMsg->GetOspTaskNO());
    }
    

    if ( NULL == pcTask )
    {
        Log(enLogSrcCCtrl, WARNING_LEV, "找不到外部SIP消息对应的任务, SipFromUri=%s\n", 
            pcSipMsg->GetSipFromUri().GetURIString().c_str());
    }
    else
    {
        if (PROCMSG_DEL == pcTask->ProcMsg(pcMsg))
            delete pcTask;
    }
}

void CCtrlInst::OnInnerOspMsg( CMessage* const pcMsg )
{
    switch(pcMsg->event)
    {
    case msgTaskMsgTransfer4Db:
        OnTaskMsgTransfer(pcMsg);
        break;
    case msgDbMsgTransfer:
        OnDbMsgTransfer(pcMsg);
        break;
    case msgSyncDataChangeNtf:
        OnSyncDataChangeNtf(pcMsg);
        break;
	case msgNtfSendTimer:
		{
			//事务定时器处理
			u32 dwTaskNO = *(u32*)pcMsg->content;
			CTask* pcTask = FindTask(dwTaskNO);
			if (pcTask != NULL)
			{
				ProcTaskMsg(pcTask, pcMsg);
			}
			else
			{
				INSTLOG(enLogSrcCCtrl, ERROR_LEV, "taskNO[%u]没有对应事务---msg[%s-%u]\n", 
					dwTaskNO, OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
			}
		}
		break;
    case msgTasExitCmd:
        OnTasExit();
        break;
    case msgDump:
        {
            u32 *pdwCode = reinterpret_cast<u32*>(pcMsg->content);
            DaemonInstanceDump(*pdwCode);
        }
        break;
    case msgLicenseUpdateNtfReq:
        {
            string strMsg(reinterpret_cast<s8*>(pcMsg->content));
            GetAppData().HandleChangeNtfReq(TAS_LICENSE_UPDATE_NTF_REQ, strMsg);
        }
        break;
    default:
        {
            Log(enLogSrcCCtrl, EVENT_LEV, "CCtrlInst::OnInnerOspMsg recved unknown msg, [%d-%s]\n", 
                pcMsg->event, OspExtEventDesc(pcMsg->event).c_str());
            CMS_ASSERT(false);
        }
        break;
    }
}

void CCtrlInst::OnTaskMsgTransfer( CMessage* const pcMsg )
{
    ptrint ptr= *reinterpret_cast<ptrint*>(pcMsg->content);
    CTaskMsgTransferBase *pcTaskMsgTransfer = reinterpret_cast<CTaskMsgTransferBase*>(ptr);

    u32 dwTaskNo = pcTaskMsgTransfer->GetOspTaskNo();

    CTask *pcTask = FindTask(dwTaskNo);
    s32 nProcResult = PROCMSG_OK;
    if ( NULL != pcTask )
    {
        nProcResult = pcTask->ProcMsg(pcMsg);
    }
    else
    {
        Log(enLogSrcCCtrl, CRITICAL_LEV, "TaskMsgTransfer回来时找不到对应的任务\n");
    }

    ptr= *reinterpret_cast<ptrint*>(pcMsg->content);
    pcTaskMsgTransfer = reinterpret_cast<CTaskMsgTransferBase*>(ptr);

    Log(enLogSrcCCtrl, EVENT_LEV, "CCtrlInst::OnTaskMsgTransfer delete taskmsgtransfer, msgobj pointer=%p\n", pcTaskMsgTransfer);
    if (NULL != pcTaskMsgTransfer) delete pcTaskMsgTransfer;

    if (PROCMSG_DEL == nProcResult)
    {
        delete pcTask;
    }
}


void CCtrlInst::OnDbMsgTransfer( CMessage* const pcMsg )
{
    CDbMsgTransferBase *pcTransBase = reinterpret_cast<CDbMsgTransferBase*>(*reinterpret_cast<ptrint*>(pcMsg->content));

    CTask *pcTask = FindTask(pcTransBase->GetOspTaskNo());
    if (NULL != pcTask)
    {
        if (PROCMSG_DEL == pcTask->ProcMsg(pcMsg))
        {
            delete pcTask;
        }
    }
    else
    {
        Log(WARNING_LEV, "Not found the task for DbMsgTransfer, eventid=%u\n", (u32)pcTransBase->GetEventId());
    }

    delete pcTransBase;
}

void CCtrlInst::OnSyncDataChangeNtf( CMessage* const pcMsg )
{
    ptrint ptr = *reinterpret_cast<ptrint*>(pcMsg->content);
    CEventReq *pcReq = reinterpret_cast<CEventReq*>(ptr);

    GetAppData().HandleChangeNtfReq(pcReq->GetEventId(), pcReq->ToXml());
    delete pcReq;
}

LPCSTR CCtrlInst::GetStrState() const
{
    if (enStateIdle == CurState()) return "enStateIdle";
    if (enStateInit == CurState()) return "enStateInit";
    if (enStateRegProxy == CurState()) return "enStateRegProxy";
    if (enStateServer == CurState()) return "enStateServer";
    if (enStateExit == CurState()) return "enStateExit";
    CMS_ASSERT(false && "CCtrlInst::GetStrState");
    return "enStateUnknown";
}

void CCtrlInst::DaemonInstanceDump( u32 dwCode )
{
    TelePrint(" 中心App状态：%s\n", GetStrState());

    GetAppData().Dump(dwCode);
    if (1 == dwCode)
    {
        s8 szBuf[1024];
        s8 szTemp[100];
        TelePrint("\n 上电与去电状态数据=============================================================");

        TelePrint(" 上电未就绪的模块数目：%u", (u32)m_cWaitPoweronReadyModuleIds.size());
        if (m_cWaitPoweronReadyModuleIds.size() > 0)
        {
            sprintf(szBuf, " \t具体模块Id分别为:");
            for (set<u32>::const_iterator it = m_cWaitPoweronReadyModuleIds.begin(); 
                 it != m_cWaitPoweronReadyModuleIds.end(); ++it)
            {
                sprintf(szTemp, "%u ", *it);
                strcat(szBuf, szTemp);
            }

            TelePrint("%s", szBuf);
        }

        TelePrint(" 去电未就绪的模块数目：%u", (u32)m_cWaitPoweroffReadyModuleIds.size());
        if (m_cWaitPoweroffReadyModuleIds.size() > 0)
        {
            sprintf(szBuf, " \t具体模块Id分别为:");
            for (set<u32>::const_iterator it = m_cWaitPoweroffReadyModuleIds.begin(); 
                it != m_cWaitPoweroffReadyModuleIds.end(); ++it)
            {
                sprintf(szTemp, "%u ", *it);
                strcat(szBuf, szTemp);
            }

            TelePrint("%s", szBuf);
        }
    }
}

CCtrlAppData& CCtrlInst::GetAppData()
{
    return g_cCtrlApp.GetAppData(); // hzytodo2, 下面的方式在Task析构时无法获取正确的引用
    // return GETAPPDATA(CCtrlApp);
}

void CCtrlInst::OnProxyConnect()
{
    NextState(enStateServer);
    GetAppData().SetProxyConnectState(true);
}

void CCtrlInst::OnProxyDisconnect()
{
    NextState(enStateRegProxy);
    GetAppData().SetProxyConnectState(false);
}


bool CCtrlAppData::IsClientModuleUriExist( const string &strUri )
{
    return m_cClientModuleUris.end() != m_cClientModuleUris.find(strUri);
}

void CCtrlAppData::AddClientModuleUri( const string &strUri )
{
    m_cClientModuleUris.insert(strUri);
}

void CCtrlAppData::DelClientModuleUri( const string &strUri )
{
    m_cClientModuleUris.erase(strUri);
}


void CCtrlAppData::HandleChangeNtfReq( u16 wNtfReqId, string strNtfReqXml )
{
    /* 通知给关注者(变更触发者也通知，因为比如添加时有些默认值可能是由TAS给填写的，添加者本人也不知的) */
    set<CSsnTaskbase*> cSsnSet = m_cChangeNtfSubscribeTasks;

    for (set<CSsnTaskbase*>::iterator it = cSsnSet.begin(); it != cSsnSet.end(); ++it)
    {
        CSsnTaskbase* pcTask = *it;
        pcTask->SaveNewChangeNotify(wNtfReqId, strNtfReqXml);
    }
}


CTask* CCtrlAppData::FindTaskBySsnId( const string& strSsnId )
{
    map<string, CTask*>::iterator it = m_cSsnId2Task.find(strSsnId);
    if (it != m_cSsnId2Task.end())
    {
        return it->second;
    }
    else
    {
        return NULL;
    }
}

void CCtrlAppData::AddSsnId2TaskMap( const string& strSsnId, CTask* pcTask )
{
    m_cSsnId2Task.insert(map<string, CTask*>::value_type(strSsnId, pcTask));
}

void CCtrlAppData::DelSsnId2TaskMap( const string& strSsnId )
{
    m_cSsnId2Task.erase(strSsnId);
}


const map<string, CTask*>& CCtrlAppData::GetSsnId2TaskMap() const
{
    return m_cSsnId2Task;
}


void CCtrlAppData::Dump(u32 dwCode)
{
    TelePrint("\n 中心app状态数据=============================================================");
    TelePrint(" 是否注册PROXY :[%d]", (s32)m_bProxyConnected);

    TelePrint(" 客户端模块uri列表-------------------------------------------------------------");
    u32 i = 0;
    for (set<string>::iterator it = m_cClientModuleUris.begin(); it != m_cClientModuleUris.end(); ++it)
    {
        TelePrint(" [%u] -- %s", i, it->c_str());
        ++i;
    }

    TelePrint(" 关注变更通知的会话任务数目 : %u", m_cChangeNtfSubscribeTasks.size());

    TelePrint(" 会话任务列表(会话任务数目:%u)-----------------------------------------------", m_cSsnId2Task.size());
    i = 0;
    for (map<string, CTask*>::iterator it = m_cSsnId2Task.begin(); it != m_cSsnId2Task.end(); ++it)
    {
        TelePrint(" [%u] 会话号-任务名-任务号 : %s-%s-%u", i, it->first.c_str(), 
            it->second->GetObjName(), it->second->GetTaskNO());
        ++i;
    }
    
    i = 0;
    for (map<string, u32>::iterator it = m_cOnlineCuserNames.begin(); it != m_cOnlineCuserNames.end(); ++it)
    {
        i += it->second;
    }
    TelePrint(" 在线用户数目（不包括重复登录）:%u", (u32)m_cOnlineCuserNames.size());
    TelePrint(" 在线用户数目（包括重复登录）: %u", (u32)i);
    if (dwCode > 0 && m_cOnlineCuserNames.size() > 0)
    {
        TelePrint(" 在线用户详细信息---------------------------------------------------");
        i = 0;
        for (map<string, u32>::iterator it = m_cOnlineCuserNames.begin(); it != m_cOnlineCuserNames.end(); ++it)
        {
            TelePrint(" [%u] 用户名-登录次数 : %s-%u", i, it->first.c_str(), it->second);
            ++i;
        }
    }
}

void CCtrlAppData::AddCuserName( const string &strName, const string &strSessionId, const string &strUserId )
{
    string strNameTmp = strName;
    //MakeLower(strNameTmp);

    map<string, u32>::iterator it = m_cOnlineCuserNames.find(strNameTmp);
    if (it == m_cOnlineCuserNames.end())
    {
        m_cOnlineCuserNames.insert(map<string, u32>::value_type(strNameTmp, 1));
    }
    else
    {
        ++it->second;
    }

    m_cLock4CuserSessId2CuserId.Lock();
    m_cCuserSessId2CuserId.insert(map<string, string>::value_type(strSessionId, strUserId));
    m_cLock4CuserSessId2CuserId.UnLock();
}

void CCtrlAppData::DelCuserName( const string &strName, const string *pstrSessionId )
{
    string strNameTmp = strName;
    //MakeLower(strNameTmp);

    map<string, u32>::iterator it = m_cOnlineCuserNames.find(strNameTmp);
    if (it != m_cOnlineCuserNames.end())
    {
        if (0 == --it->second)
        {
            m_cOnlineCuserNames.erase(it);
        }
    }
    else
    {
        CMS_ASSERT("CCtrlAppData::DelCuserName not found certain name");
    }

    if (NULL != pstrSessionId)
    {
        m_cLock4CuserSessId2CuserId.Lock();
        m_cCuserSessId2CuserId.erase(*pstrSessionId);
        m_cLock4CuserSessId2CuserId.UnLock();
    }
}

bool CCtrlAppData::IsCuserNameExist( const string &strName )
{
    string strNameTmp = strName;
    //MakeLower(strNameTmp);

    map<string, u32>::iterator it = m_cOnlineCuserNames.find(strNameTmp);
    return (it != m_cOnlineCuserNames.end());
}

bool CCtrlAppData::GetCuserId( const string &strSessionId, string &strUserId )
{
    bool bRet;
    m_cLock4CuserSessId2CuserId.Lock();
    map<string, string>::const_iterator it = m_cCuserSessId2CuserId.find(strSessionId);
    if (it != m_cCuserSessId2CuserId.end())
    {
        strUserId = it->second;
        bRet = true;
    }
    else
    {
        bRet = false;
    }
    m_cLock4CuserSessId2CuserId.UnLock();
    return bRet;
}


void CCtrlAppData::AddTasConnTask( CConnTaskbase *pcTask )
{
    m_cTasConnTasks.insert(pcTask);
}


void CCtrlAppData::DelTasConnTask( CConnTaskbase *pcTask )
{
    m_cTasConnTasks.erase(pcTask);
}

set<CConnTaskbase*> CCtrlAppData::GetTasConnTaskSet() const
{
    return m_cTasConnTasks;
}

set<CSsnTaskbase*>& CCtrlAppData::TasSessTasks()
{
    return m_cTasSessTasks;
}
