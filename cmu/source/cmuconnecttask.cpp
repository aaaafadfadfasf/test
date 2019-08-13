
#include "cms/cms_errorcode.h"
#include "cms/cmu/cmu_event.h"
#include "cms/cmu/cmu_struct.h"
#include "cms/cmu/cmu_proto.h"

#include "cmuconst.h"
#include "cmudata.h"
#include "cmucoreapp.h"
#include "cmuconnecttask.h"
#include "cmureqrsptask.h"
#include "cmuconf.h"
#include "domainredissynctask.h"
#include "domainpulistssertask.h"
#include "domaincatalogssertask.h"
#include "cms/cmu/cmu_redis.h"

CCmuList g_cCmuList;
extern CCmuConf g_cCmuConf;
extern TDomainTable g_tDomainTable;


CCmuConnecterTask::CCmuConnecterTask(CInstExt *pcInst, CDomainData *pcDomainData) : CLoginSession(pcInst), m_pcDomainData(pcDomainData)
{
    m_bSupportRedis = false;
    m_bRedisSync    = true;

    m_pcDomainRedisSyncTask   = NULL;
    m_pcDomainPuListSserTask  = NULL;
    m_pcDomainCatalogSserTask = NULL;
    m_pcNotifyTask            = NULL;
    m_wConnectTimes           = 0;
}

CCmuConnecterTask::~CCmuConnecterTask()
{  
    NotifyCmuState(CMU_CMU_DELETE);

    g_cCmuList.Erase(m_tDstURI.GetURIString());

    if (NULL != m_pcDomainRedisSyncTask)
    {
        delete m_pcDomainRedisSyncTask;
    }

    if (NULL != m_pcDomainPuListSserTask)
    {
        delete m_pcDomainPuListSserTask;
    }

    if (NULL != m_pcDomainCatalogSserTask)
    {
        delete m_pcDomainCatalogSserTask;
    }

    m_pcDomainData->ExitCmuConnectTask(this);
}

void CCmuConnecterTask::InitStateMachine()
{
    CStateProc cTryConnProc;
    cTryConnProc.ProcMsg = (CTask::PFProcMsg)&CCmuConnecterTask::OnTryConn;
    cTryConnProc.ProcErrMsg = (CTask::PFProcMsg)&CCmuConnecterTask::OnTryConn;
    cTryConnProc.TimerPoll = (CTask::PFTimerPoll)&CCmuConnecterTask::OnTryConnTimer;
    AddRuleProc(TryConn, cTryConnProc);

    CStateProc cServiceProc;
    cServiceProc.ProcMsg = (CTask::PFProcMsg)&CCmuConnecterTask::OnService;
    cServiceProc.ProcErrMsg = (CTask::PFProcMsg)&CCmuConnecterTask::OnService;
    cServiceProc.TimerPoll = (CTask::PFTimerPoll)&CCmuConnecterTask::OnServiceTimer;
    AddRuleProc(Service, cServiceProc);

    NextState(TryConn);
}

u32 CCmuConnecterTask::Connect()
{
    SetServerUri(m_tDstURI.GetURIString());
    SetDevUri(GetLocalUri().GetURIString());
    SetDevType(DEV_TYPE_CMU);
    SetSession(m_tDstURI.GetURIString());

    //��ӵ�CMU�б���
    g_cCmuList.Insert(m_tDstURI.GetURIString(), this);

    return StartConnect();
}

u32 CCmuConnecterTask::StartConnect()
{
    CCmuConnectReq cReq;
    cReq.SetSession(GetDevUri());
    cReq.SetSrcUri(GetDevUri());
    cReq.SetDstUri(m_tDstURI.GetURIString());
	//cmu���ӵ�ַ��Ҫѡ���Ӧ������
	//cReq.SetLocalIp(g_cCmuConf.m_tLocalAddr.GetIpStr());
	TIpAddress tLocalIp;
	TDomainTable::const_iterator itLocalIp = g_tDomainTable.find(m_tDstURI.GetDomain());
	tLocalIp = (g_tDomainTable.end() == itLocalIp) ? g_cCmuConf.m_tLocalAddr.GetIpStr() : itLocalIp->second.GetLocalIP();		
	cReq.SetLocalIp(tLocalIp);

    u32 dwRet = PROCMSG_FAIL;
    dwRet = PostMsgReq(cReq, m_tDstURI);
    if (dwRet != PROCMSG_OK)
    {
        TASKLOG(CMU_MOD_CMU, ERROR_LEV, "����SIP��Ϣʧ��!\n");
        return dwRet;
    }

    NextState(TryConn);

    if (m_wConnectTimes++ >= g_cCmuConf.m_wMaxDomainConnectTimes)
    {
        set<string> cPuIdList;
        m_pcDomainData->GetPuList().GetOnlinePuIdList(cPuIdList);

        if (!cPuIdList.empty())
        {
            if (NULL == m_pcNotifyTask)         // m_pcNotifyTaskΪ��ʱ˵����δ��ʼ�������ߴ���
            {
                m_pcNotifyTask = new CSlidingWindowTask<CCmuConnecterTask>(GetInstance(), this, NULL, &CCmuConnecterTask::DomainPuOfflineProc);
                m_pcNotifyTask->InitStateMachine();
                m_pcNotifyTask->SetWindowSize(CRedisPipe::GetInitDevStatusNumPerTime());
            }
            else
            {
                m_pcNotifyTask->Reset();
            }

            m_pcNotifyTask->Add(cPuIdList);

            TASKLOG(CMU_MOD_CMU, ERROR_LEV, "��������[%s]ʧ�ܴ����ﵽ[%d]�Σ���ʼΪ����������豸[%u]�����ߴ���\n", 
                m_pcDomainData->GetDomainName().c_str(), m_wConnectTimes, cPuIdList.size());
        }
    }

    return dwRet;
}

u32 CCmuConnecterTask::OnTryConn(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_FAIL;
    switch(pcMsg->event)
    {
    case CMU_CONNECT_RSP:
        {
            //����cmu����������յ�Ӧ��
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

            CCmuConnectRsp cRsp;
            pcOspSipMsg->GetMsgBody(cRsp);
            int dwErrorCode = cRsp.GetErrorCode();
            if (dwErrorCode == CMS_SUCCESS)
            {
                //���ӳɹ�
                TASKLOG(CMU_MOD_CMU, CRITICAL_LEV, "��������cmu[%s]�ɹ�!\n",
                    m_tDstURI.GetURIString().c_str());

                //��ʼ����
                SetHBParam(m_tDstURI);

				m_tPeerIp = cRsp.GetLocalIp();
				if (!g_cCmuConf.m_bUseRedisDirectConnect)
				{
					cRsp.SetSupportRedis(false);
				} 
				m_bSupportRedis = cRsp.GetSupportRedis();
                
                // ����redis����
				string strRedisRealIp = cRsp.GetRedisIp();
				TASKLOG(CMU_MOD_CMU, ERROR_LEV, "RedisRealIp-Bef[%s]!\n",
					strRedisRealIp.c_str());
				GetRedisRealIp(strRedisRealIp);
				TASKLOG(CMU_MOD_CMU, ERROR_LEV, "RedisRealIp-Aft[%s]!\n",
					strRedisRealIp.c_str());

				//������IP
				TDomainConnectIpList::const_iterator itDomIp = g_cCmuConf.m_tDomainConnectIpList.find(m_tDstURI.GetDomain());
				if (g_cCmuConf.m_tDomainConnectIpList.end() != itDomIp)
				{
					strRedisRealIp = itDomIp->second;
					TASKLOG(CMU_MOD_CMU, ERROR_LEV, "RedisRealIp-Cfg[%s]!\n",
						strRedisRealIp.c_str());
				}

                m_pcDomainData->SetRedisParam(strRedisRealIp, cRsp.GetRedisPort(), cRsp.GetRedisPassword());

                //�������̬
                NextState(Service);

                //֪ͨ���������ӽ��
                NotifyCmuState(CMU_CMU_CONNECT_SUCCESS);

				//��������Ŀ�������ϼ������¼�cmu�ɹ����һ��������־
				 ((CCmuCoreInst*)GetInstance())->GetUasLogTask()->AddDeviceLog(GetDevLogInfo(true));

                 m_wConnectTimes = 0;

                 // m_pcNotifyTask���������޷����ӵ�����ƽ̨ʱ�������豸�����ߴ���ģ���Ȼ���ӳɹ��ˣ�Ҳ��û�д��ڵı�Ҫ��
                 // ��ʵҲ���Ե�����Reset�ӿڣ����ﻹ��ֱ���ͷţ�����Ҫ��ʱ���ٴ�����
                 if (NULL != m_pcNotifyTask)
                 {
                     delete m_pcNotifyTask;
                     m_pcNotifyTask = NULL;
                 }

                StartSyncData();

                dwRet = PROCMSG_OK;
            }
            else
            {
				NotifyCmuState(CMU_CMU_CONNECT_FAIL);

                //����ʧ��
                TASKLOG(CMU_MOD_CMU, ERROR_LEV, "cmu����cmu[%s]ʧ��,�ȴ���ʱ��������errcode[%d]\n",
                    m_tDstURI.GetURIString().c_str(), dwErrorCode);

                dwRet = PROCMSG_OK;
            }
        }
        break;

    case OSP_SIP_MSG_PROC_FAIL:
        {
            //����cmu����������յ�SIP��Ĵ���Ӧ��
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            if (pcOspSipMsg == NULL)
            {
                TASKLOG(CMU_MOD_CMU, ERROR_LEV, "ospsip ��ϢΪ��\n");
                dwRet = PROCMSG_FAIL;
                return dwRet;
            }

			NotifyCmuState(CMU_CMU_CONNECT_FAIL);

            TASKLOG(CMU_MOD_CMU, ERROR_LEV, "cmu����cmu[%s]ʧ��,�ȴ���ʱ��������sip status code[%d]\n",
                m_tDstURI.GetURIString().c_str(), pcOspSipMsg->GetSipErrorCode());

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

void CCmuConnecterTask::NotifyCmuState(u32 dwConnResult)
{
    //֪ͨ����������cmu�ĵ�¼״̬
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

u32 CCmuConnecterTask::OnTryConnTimer()
{
    if (GetCurStateHoldTime() > CMU_CONNECT_CMU_TIMEOUT)
    {
        TASKLOG(CMU_MOD_CMU, WARNING_LEV, "����cmu[%s]��ʱ����ʼ����\n", 
            m_tDstURI.GetURIString().c_str());

        //���·�������
        StartConnect();
    }

    return TIMERPOLL_DONE;
}

u32 CCmuConnecterTask::OnService(CMessage *const pcMsg)
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

u32 CCmuConnecterTask::OnServiceTimer()
{
    if (GetCurStateHoldTime() > CMU_CONNECT_CMU_TIMEOUT)
    {
        if (m_bRedisSync)
        {
            StartSyncData();
        }

        NextState(Service);
    }

    return TIMERPOLL_DONE;
}

u32 CCmuConnecterTask::DisProc(CMessage *const pcMsg)
{
    //CMU��������
    COspSipMsg* pOspSipMsg = (COspSipMsg*)pcMsg->content;
    if (pOspSipMsg == NULL)
    {
        TASKLOG(CMU_MOD_CMU, ERROR_LEV, "ospsip ��ϢΪ��\n");
        return PROCMSG_FAIL;
    }

    //�Ƿ�����CMU����
    TSipURI tDevUri;
    tDevUri.SetNOString(pOspSipMsg->GetMsgBody());
    if (tDevUri != m_tDstURI)
    {
        TASKLOG(CMU_MOD_CMU, WARNING_LEV, "�յ���Ч������Ϣ�������豸[%s]\n", 
            m_tDstURI.GetURIString().c_str());
        return PROCMSG_FAIL;
    }

    TASKLOG(CMU_MOD_CMU, CRITICAL_LEV, "cmu[%s]��cmu����\n", 
        m_tDstURI.GetURIString().c_str());

    //����ص�¼�Ự�㲥����֪ͨ
    GetWorkInst()->BroadcastCmuDisNtf(tDevUri.GetURIString());

    //��������Ŀ�������ϼ����¼�cmu�Ͽ����һ��������־
    ((CCmuCoreInst*)GetInstance())->GetUasLogTask()->AddDeviceLog(GetDevLogInfo(false));

    //���������������Ҫ����������������      

    //�����������״̬
    //״̬�л������ڶ���֮ǰִ�У�����ͻ��ڶ�������ʱ��Ź����·���Ķ��ĵ���Ϣ(ֻ��cmu��¼�ɹ����ܶ���)
    NextState(TryConn);

    //cmu������֪ͨ�͸�cmu������ص�����
    NotifyCmuState(CMU_CMU_DISCONNECT);

    // ���¼�cmu����֮���Ǻ��¼�redis֮�������Ҳ�Ͽ�����������һ�°�
    if (m_bSupportRedis)
    {
        m_bRedisSync = true;
    }

    //���·�������
    StartConnect();

    return PROCMSG_OK;
}

TDeviceLogInfo CCmuConnecterTask::GetDevLogInfo(bool bOnOffState)
{
	TDeviceLogInfo tLogInfo;

	tLogInfo.devUri = m_tDstURI.GetURIString();

	tLogInfo.devAlias = UNIQUE_SIP_UA_CMU;

	tLogInfo.happenedTime = CCMSTime().ToString();
	tLogInfo.logType = (bOnOffState ? enDeviceLogTypeOnline : enDeviceLogTypeOffline);

	tLogInfo.devName = tLogInfo.devUri;
	tLogInfo.netAddr = m_tPeerIp;

	if (bOnOffState)
	{
		tLogInfo.logDesc = GetLanDesc(LAN_DESC_ONLINE);
	}
	else
	{
		tLogInfo.logDesc = GetLanDesc(LAN_DESC_OFFLINE);
	}

	return tLogInfo;
}

void CCmuConnecterTask::PrintData() const
{
    CLoginSession::PrintData();

    if (m_bSupportRedis)
    {
        u32 dwDomainRedisSyncTask  = NULL != m_pcDomainRedisSyncTask ? m_pcDomainRedisSyncTask->GetTaskNO() : INVALID_TASKNO;
        OspPrintf(TRUE, FALSE, "\nDstUri[%s], DstIp[%s], ConnectTimes[%d], SupportRedis[%d], DomainRedisSyncTask[%u], RedisSync[%d]\n", 
            m_tDstURI.GetURIString().c_str(), m_tPeerIp.c_str(), m_wConnectTimes, m_bSupportRedis, dwDomainRedisSyncTask, m_bRedisSync);    
    }
    else
    {
        u32 dwDomainPuListSserTask  = NULL != m_pcDomainPuListSserTask ? m_pcDomainPuListSserTask->GetTaskNO() : INVALID_TASKNO;
        u32 dwDomainCatalogSserTask = NULL != m_pcDomainCatalogSserTask ? m_pcDomainCatalogSserTask->GetTaskNO() : INVALID_TASKNO;
        OspPrintf(TRUE, FALSE, "\nDstUri[%s], DstIp[%s], ConnectTimes[%d], SupportRedis[%d], DomainPuListSserTask[%u], "
            "DomainCatalogSserTask[%u], RedisSync[%d]\n", 
            m_tDstURI.GetURIString().c_str(), m_tPeerIp.c_str(), m_wConnectTimes, m_bSupportRedis, 
            dwDomainPuListSserTask, dwDomainCatalogSserTask, m_bRedisSync);
    }
}

void CCmuConnecterTask::SetSyncParam(u32 dwSyncNumOnce)
{
    if (NULL != m_pcDomainRedisSyncTask)
    {
        m_pcDomainRedisSyncTask->SetSyncNumOnce(dwSyncNumOnce);
    }
}

void CCmuConnecterTask::AddSync(const string &strDevId)
{
    if (NULL != m_pcDomainRedisSyncTask)
    {
        m_pcDomainRedisSyncTask->AddSync(strDevId);
    }
    else if (NULL != m_pcDomainCatalogSserTask)
    {
        m_pcDomainCatalogSserTask->PopCache(strDevId);
    }
}

void CCmuConnecterTask::ExitDomainRedisSyncTask(const CDomainRedisSyncTask *pcTask)
{
    if (m_pcDomainRedisSyncTask == pcTask)
    {
        m_pcDomainRedisSyncTask = NULL;
    }
}

void CCmuConnecterTask::ExitDomainPuListSserTask(const CDomainPuListSserTask *pcTask)
{
    if (m_pcDomainPuListSserTask == pcTask)
    {
        m_pcDomainPuListSserTask = NULL;
    }
}

void CCmuConnecterTask::ExitDomainCatalogSserTask(const CDomainCatalogSserTask *pcTask)
{
    if (m_pcDomainCatalogSserTask == pcTask)
    {
        m_pcDomainCatalogSserTask = NULL;
    }
}

void CCmuList::PrintData()
{
    OspPrintf(TRUE, FALSE, "��ǰ���ߵ�cmu��[%u]��\n", m_cCmuMap.size());    
    u32 i = 0;

    for (CCmuMap::const_iterator it = m_cCmuMap.begin(); m_cCmuMap.end() != it; ++it)
    {
        CCmuConnecterTask* pcCmuTask = (CCmuConnecterTask*)it->second;
        OspPrintf(TRUE, FALSE, "  %4u. cmu[%s][%s]-taskNo[%u]-State[%s]\n", 
            ++i, it->first.c_str(), pcCmuTask->GetPeerIp().c_str(), 
            pcCmuTask->GetTaskNO(), pcCmuTask->GetStateName(pcCmuTask->GetState()));
    }

    OspPrintf(TRUE, FALSE, "��ǰ���ߵ�cmu��[%u]��\n", m_cCmuMap.size());
}

void CCmuList::Insert(const string &strSession, CLoginSession *pcTask, bool bInitiative)
{
    if (bInitiative)
    {
        m_cCmuMap.insert(CCmuMap::value_type(strSession, pcTask));
    }
    else
    {
        m_cCmuPassiveMap.insert(CCmuMap::value_type(strSession, pcTask));
    }
}

void CCmuList::Erase(const string &strSession, bool bInitiative)
{
    if (bInitiative)
    {
        m_cCmuMap.erase(strSession);
    }
    else
    {
        m_cCmuPassiveMap.erase(strSession);
    }
}

bool CCmuList::Find(const string &strSession, CLoginSession *&pcTask, bool bInitiative)
{
    if (bInitiative)
    {
        CCmuMap::iterator it4Init = m_cCmuMap.find(strSession);
        if (m_cCmuMap.end() != it4Init)
        {
            pcTask = it4Init->second;
            return true;
        }

        CCmuMap::iterator it4Pass = m_cCmuPassiveMap.find(strSession);
        if (m_cCmuPassiveMap.end() != it4Pass)
        {
            pcTask = it4Pass->second;
            return true;
        }
    }
    else
    {
        CCmuMap::iterator it4Pass = m_cCmuPassiveMap.find(strSession);
        if (m_cCmuPassiveMap.end() != it4Pass)
        {
            pcTask = it4Pass->second;
            return true;
        }

        CCmuMap::iterator it4Init = m_cCmuMap.find(strSession);
        if (m_cCmuMap.end() != it4Init)
        {
            pcTask = it4Init->second;
            return true;
        }
    }

    return false;
}

void CCmuConnecterTask::StartSyncData()
{
    // ��������cmu�ɹ�������Զ�֧��redis����Ҫ��ʼ���ģ���������sip����
    if (NULL != m_pcDomainData)
    {
        CDomainData::DomainType emDomainType = m_pcDomainData->GetDomainType();

        if (m_bSupportRedis && (CDomainData::DomainType_Authorizer != emDomainType || g_cCmuConf.m_bAuthDomainRedisSync))
        {
            if (m_bRedisSync)
            {
                CRedisPipe &cRedisPipe = m_pcDomainData->GetRedisPipe();
                vector<string> cSsList;

                if (cRedisPipe.Connect())
                {
                    // �ؼ���Ϣ��ʹ��ERROR������־���Ա��¼
                    TASKLOG(CMU_MOD_CMU, ERROR_LEV, "����ƽ̨[%s]��Redis�ɹ�!\n", m_pcDomainData->GetDomainName().c_str());
                    cRedisPipe.GetFastDBInstance().SetCommandTimeOut(g_cCmuConf.m_nRedisDomainSyncCmdTimeout);

                    const string &strDomainId = m_pcDomainData->GetDomainId();

                    if (CDomainData::DomainType_Child == emDomainType)              // �¼���ֱ�Ӷ��������豸����������
                    {
                        if (!g_cCmuConf.m_bDomainSsSkipPuStatus)
                        {
                            cSsList.push_back(RedisGetOnOffCatalogSubChn(strDomainId));
                        }

                        if (!g_cCmuConf.m_bDomainSsSkipPuConfig)
                        {
                            cSsList.push_back(RedisGetConfigCatalogSubChn(strDomainId));
                        }

                        if (!g_cCmuConf.m_bDomainSsSkipPuService)
                        {
                            cSsList.push_back(RedisGetServiceCatalogSubChn(strDomainId));
                        }

                        if (!g_cCmuConf.m_bDomainSsSkipPuAlarm)
                        {
                            cSsList.push_back(RedisGetAlarmCatalogSubChn(strDomainId));
                        }

                        if (!g_cCmuConf.m_bDomainSsSkipPuGpsData)
                        {
                            cSsList.push_back(RedisGetGpsCatalogSubChn(strDomainId));
                        }

                        if (!g_cCmuConf.m_bDomainSsSkipPuTransData)
                        {
                            cSsList.push_back(RedisGetTransDataCatalogSubChn(strDomainId));
                        }

                        cRedisPipe.ClearSs();
                        cRedisPipe.AddSs(cSsList);
                    }
                    else if (CDomainData::DomainType_Authorizer == emDomainType)    // ������Ȩ����������Ȩ�豸
                    {
                        vector<string> cPuIdList;
                        m_pcDomainData->GetPuList().GetPuIdList(cPuIdList);

                        // ÿһ���豸��Ӧ��6�ֶ�������
                        cSsList.reserve(6 * cPuIdList.size());
                        for (vector<string>::const_iterator it = cPuIdList.begin(); cPuIdList.end() != it; ++it)
                        {
                            if (!g_cCmuConf.m_bAuthDomainSsSkipPuStatus)
                            {
                                cSsList.push_back(RedisGetOnOffSubChn(*it));
                            }

                            if (!g_cCmuConf.m_bAuthDomainSsSkipPuConfig)
                            {
                                cSsList.push_back(RedisGetConfigSubChn(*it));
                            }

                            if (!g_cCmuConf.m_bAuthDomainSsSkipPuService)
                            {
                                cSsList.push_back(RedisGetServiceSubChn(*it));
                            }

                            if (!g_cCmuConf.m_bAuthDomainSsSkipPuAlarm)
                            {
                                cSsList.push_back(RedisGetAlarmSubChn(*it));
                            }

                            if (!g_cCmuConf.m_bAuthDomainSsSkipPuGpsData)
                            {
                                cSsList.push_back(RedisGetGpsSubChn(*it));
                            }

                            if (!g_cCmuConf.m_bAuthDomainSsSkipPuTransData)
                            {
                                cSsList.push_back(RedisGetTransDataSubChn(*it));
                            }
                        }

                        cRedisPipe.ClearSs();
                        cRedisPipe.AddSs(cSsList);
                    }

                    if (NULL == m_pcDomainRedisSyncTask)
                    {
                        m_pcDomainRedisSyncTask = new CDomainRedisSyncTask(GetInstance(), m_pcDomainData, this);
                        m_pcDomainRedisSyncTask->InitStateMachine();
                        m_pcDomainRedisSyncTask->SetSyncNumOnce(g_cCmuConf.m_dwRedisDomainSyncNumOnce);
                    }

                    // �״�����ͬ��
                    m_pcDomainRedisSyncTask->AddSync();

                    m_bRedisSync = false;
                }
                else
                {
                    TASKLOG(CMU_MOD_CMU, ERROR_LEV, "����ƽ̨[%s]��Redisʧ��!\n", m_pcDomainData->GetDomainName().c_str());
                }
            }
            else
            {
                TASKLOG(CMU_MOD_CMU, WARNING_LEV, "�Ѿ����ӵ�ƽ̨ [%s]��redis�������ٴ�ͬ��!!\n", m_pcDomainData->GetDomainName().c_str());
            }
        }
        else
        {
            if (CDomainData::DomainType_Child == emDomainType)
            {
                if (NULL == m_pcDomainCatalogSserTask)
                {
                    m_pcDomainCatalogSserTask = new CDomainCatalogSserTask(GetInstance(), this);
                    CMS_ASSERT(NULL != m_pcDomainCatalogSserTask && "m_pcDomainCatalogSserTask = NULL!!!");
                    m_pcDomainCatalogSserTask->InitStateMachine();
                }

                TSsTypes tSsTypes;

                if (!g_cCmuConf.m_bDomainSsSkipPuStatus)
                {
                    tSsTypes.insert(PU_ON_OFF_STATUS);
                }

                if (!g_cCmuConf.m_bDomainSsSkipPuConfig)
                {
                    tSsTypes.insert(PU_CONFIG_STATUS);
                }

                if (!g_cCmuConf.m_bDomainSsSkipPuService)
                {
                    tSsTypes.insert(PU_SERVICE_STATUS);
                }

                if (!g_cCmuConf.m_bDomainSsSkipPuAlarm)
                {
                    tSsTypes.insert(PU_ALARM_STATUS);
                }

                if (!g_cCmuConf.m_bDomainSsSkipPuGpsData)
                {
                    tSsTypes.insert(PU_GPS_STATUS);
                }

                if (!g_cCmuConf.m_bDomainSsSkipPuTransData)
                {
                    tSsTypes.insert(PU_TRANSDATA_STATUS);
                }

                m_pcDomainCatalogSserTask->AddSs(m_tDstURI.GetDomain(), tSsTypes);
            }
            else if (CDomainData::DomainType_Authorizer == emDomainType)
            {
                if (NULL == m_pcDomainPuListSserTask)
                {
                    m_pcDomainPuListSserTask = new CDomainPuListSserTask(GetInstance(), this);
                    CMS_ASSERT(NULL != m_pcDomainPuListSserTask && "m_pcDomainPuListSserTask = NULL!!!");
                    m_pcDomainPuListSserTask->InitStateMachine();
                }

                vector<string> cPuIdList;
                m_pcDomainData->GetPuList().GetPuIdList(cPuIdList);

                // ÿһ���豸��Ӧ��6�ֶ�������
                set<CSsUnit<string> > cSsUnits;
                CSsUnit<string> cUnit;
                for (vector<string>::const_iterator it = cPuIdList.begin(); cPuIdList.end() != it; ++it)
                {
                    TSipURI tDevUri(*it, m_pcDomainData->GetDomainName());
                    cUnit.SetObject(tDevUri.GetURIString());

                    if (!g_cCmuConf.m_bAuthDomainSsSkipPuStatus)
                    {
                        cUnit.SetSsType(PU_ON_OFF_STATUS);
                        cSsUnits.insert(cUnit);
                    }

                    if (!g_cCmuConf.m_bAuthDomainSsSkipPuConfig)
                    {
                        cUnit.SetSsType(PU_CONFIG_STATUS);
                        cSsUnits.insert(cUnit);
                    }

                    if (!g_cCmuConf.m_bAuthDomainSsSkipPuService)
                    {
                        cUnit.SetSsType(PU_SERVICE_STATUS);
                        cSsUnits.insert(cUnit);
                    }

                    if (!g_cCmuConf.m_bAuthDomainSsSkipPuAlarm)
                    {
                        cUnit.SetSsType(PU_ALARM_STATUS);
                        cSsUnits.insert(cUnit);
                    }

                    if (!g_cCmuConf.m_bAuthDomainSsSkipPuGpsData)
                    {
                        cUnit.SetSsType(PU_GPS_STATUS);
                        cSsUnits.insert(cUnit);
                    }

                    if (!g_cCmuConf.m_bAuthDomainSsSkipPuTransData)
                    {
                        cUnit.SetSsType(PU_TRANSDATA_STATUS);
                        cSsUnits.insert(cUnit);
                    }
                }

                m_pcDomainPuListSserTask->AddSs(cSsUnits);
            }
        }
    }
}

bool CCmuConnecterTask::DomainPuOfflineProc(const vector<ElementIdType> &cIds)
{
    CDomainData *pcLocalDomainData = GetLocalDomainData();
    if (NULL != pcLocalDomainData)
    {
        if (GetLocalRedisPipe().InitDevStatus(cIds, true))
        {
            for (vector<ElementIdType>::const_iterator it = cIds.begin(); cIds.end() != it; ++it)
            {
                CPuData *pcPuData = GetPuData(m_pcDomainData->GetDomainId(), *it);
                if (NULL != pcPuData)
                {
                    // ���û�����豸״̬
                    pcPuData->SetPuIsOnline(false, false);

                    // ��sip�����߷����豸״̬֪ͨ
                    pcPuData->PublishSipOnOff();
                }
            }
        }
        else
        {
            TASKLOG(CMU_MOD_REDIS, ERROR_LEV, "�豸״̬��ʼ��ʧ��, �豸����[%u]\n", cIds.size());
        }
    }

    return true;
}

void CCmuConnecterTask::GetRedisRealIp(string& strRedisRealIp)
{
	TDomainTable::const_iterator it = g_tDomainTable.find(m_tDstURI.GetDomain());
	strRedisRealIp = (g_tDomainTable.end() != it) ? it->second.GetPeerIP():strRedisRealIp;
}

