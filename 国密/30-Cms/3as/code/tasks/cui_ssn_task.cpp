#include "cui_ssn_task.h"
#include "cctrl.h"
#include "license_mng.h"
#include "file_export_task.h"
#include "log_system.h"

CCuiSsnTask::CCuiSsnTask( CInstExt *pcInst ): CSsnTaskbase(pcInst), m_cCuserOnlineData(AppData())
{
    AppData().GetChangeNtfSubscribeTasks().insert(this);

    m_cNtfFilterTypes.clear();

    //m_cNtfFilterTypes.insert(TasResourceGeneralDev);
    m_cNtfFilterTypes.insert(TasResourceTvwallDev);
    m_cNtfFilterTypes.insert(TasResourceDevModel);
    m_cNtfFilterTypes.insert(TasResourceDevCapIndexName);
    m_cNtfFilterTypes.insert(TasResourceDevGrp);
    m_cNtfFilterTypes.insert(TasResourceDevGrp2Dev);
    m_cNtfFilterTypes.insert(TasResourceDomainRelation);

    m_cNtfFilterTypes.insert(TasResourceCuser);
    //m_cNtfFilterTypes.insert(TasResourceCuser2Dev);
    m_cNtfFilterTypes.insert(TasResourceCuserGrp);
    //m_cNtfFilterTypes.insert(TasResourceCuserGrp2Dev);

    // m_cNtfFilterTypes.insert(TasResourceTuser);
    m_cNtfFilterTypes.insert(TasResourceSyncDstDomainCfg);
    m_cNtfFilterTypes.insert(TasResourceTmpAuthorizationCfg);

	// cui关注ptz锁变化 [4/13/2016 pangubing]
	 m_cNtfFilterTypes.insert(TasResourcePtzLock);
}

CCuiSsnTask::~CCuiSsnTask()
{
    AppData().GetChangeNtfSubscribeTasks().erase(this);
}

void CCuiSsnTask::PrintData() const
{
    CSsnTaskbase::PrintData();
    m_cCuserOnlineData.PrintData();
}

const char* CCuiSsnTask::GetStateName( u32 dwState ) const
{
    if (enStateInit == dwState)
    {
        return "InitState";
    }
    else if (enStateServer == dwState)
    {
        return "ServerState";
    }
    else
    {
        return "unknown";
    }
}

void CCuiSsnTask::InitStateMachine()
{
    NextState(enStateInit);

    CStateProc cProc;
    cProc.TimerPoll = &CTask::TimerPollSkip;

    cProc.ProcMsg = (CTask::PFProcMsg)&CCuiSsnTask::InitStateProcMsg;
    cProc.ProcErrMsg = cProc.ProcMsg;
    AddRuleProc(enStateInit, cProc);
    cProc.ProcMsg = (CTask::PFProcMsg)&CCuiSsnTask::ServerStateProcMsg;
    cProc.ProcErrMsg = cProc.ProcMsg;
    cProc.TimerPoll = static_cast<PFTimerPoll>(&CSsnTaskbase::PollChangeNotifyLst);
    AddRuleProc(enStateServer, cProc);
}

u32 CCuiSsnTask::InitStateProcMsg( CMessage *const pcMsg )
{
    COspSipMsg *pcSipMsg = reinterpret_cast<COspSipMsg*>(pcMsg->content);

    CLoginTasReq cLoginReq;
    pcSipMsg->GetMsgBody(cLoginReq);

    string strModuleUri;
    strModuleUri = pcSipMsg->GetSipFromUri().GetURIString();

    if (AppData().IsClientModuleUriExist(strModuleUri))
    {
        CLoginTasRsp cLoginRsp;
        cLoginRsp.SetErrorCode(ERR_TAS_LOGINED);
        PostSipRsp2UA(cLoginReq, cLoginRsp, pcSipMsg->GetSipTransID());

        Log(CRITICAL_LEV, "CUI login repeatly, moduleuri=%s\n", strModuleUri.c_str());
        return PROCMSG_DEL;
    }

    CLoginTasRsp cLoginRsp;
    cLoginRsp.SetErrorCode(CMS_SUCCESS);
    cLoginRsp.SetSession(GetSessionId());
    PostSipRsp2UA(cLoginReq, cLoginRsp, pcSipMsg->GetSipTransID());

	TPSLoginExternInfo tLoginExtern;
	tLoginExtern.FromXml(cLoginReq.GetExtraInfo());
	for (set<int>::iterator it = tLoginExtern.resourceTypes.begin(); 
		it != tLoginExtern.resourceTypes.end(); ++it)
	{
		m_cNtfFilterTypes.insert(static_cast<ENTasResourceTypes>(*it));
	}
    SetClientSipURI(pcSipMsg->GetSipFromUri());
    if (!SetHb())
    {
        return PROCMSG_DEL;
    }

    NextState(enStateServer);
    return PROCMSG_OK;
}

u32 CCuiSsnTask::ServerStateProcMsg( CMessage *const pcMsg )
{
    if (g_enLicenseValidateResult != CMS_SUCCESS)
    {
        Log(ERROR_LEV, "CuiSsnTask is killed, because of the license validate failed, errno=%d\n", (s32)g_enLicenseValidateResult);
        return PROCMSG_DEL;
    }

    switch (pcMsg->event)
    {
    case TAS_CU_ONLINE_REQ:
        OnCuOnlineReq(pcMsg);
        break;
    case TAS_CU_OFFLINE_REQ:
        OnCuOfflineReq(pcMsg);
        break;
    case msgTaskMsgTransfer4Db:
        OnTaskMsgTransfer(pcMsg);
        break;
    case FILE_EXPORT_REQ:
        OnExportCuserDevData(pcMsg);
        break;
    default:
        return DefaultProc(pcMsg);
        break;
    }

    return PROCMSG_OK;
}

void CCuiSsnTask::OnCuOnlineReq( CMessage* const pcMsg )
{
    COspSipMsg *pcSipMsg = reinterpret_cast<COspSipMsg*>(pcMsg->content);

    CCuOnlineReq cReq;
    CCuOnlineRsp cRsp;

    pcSipMsg->GetMsgBody(cReq);
    if (cReq.GetLoginSession().empty()) cReq.SetLoginSession(CUUID()); // 兼容：老的cuiserver,tas为它生成uuid

    TCuserOnlineKey cKey(cReq.GetCuUserName(), cReq.GetSeqNum());
    string strSessId;
    
    if (!m_cCuserOnlineData.GetCuserSessId(cKey, strSessId))
    {
        CTaskMsgTransfer<CCuOnlineReq> cTrans(GetTaskNO(), pcSipMsg->GetSipTransID(), msgCuserOnlineReq);
        cTrans.GetMsgObj() = cReq;
        if (PostTaskMsgTransfer2DbTaskPool(cTrans))
        {
            m_cCuserOnlineData.AddCuserOnlineKey(cKey, cReq.GetLoginSession());
        }
        else
        {
            cRsp.SetErrorCode(ERR_TAS_SYSTEM_BUSY);
            PostSipRsp2UA(cReq, cRsp, pcSipMsg->GetSipTransID());
        }
    }
    else
    {
        if (strSessId.empty())
        {
            cRsp.SetErrorCode(ERR_TAS_HANDLE_IN_PROGRESS);
        }
        else
        {
            // 可能是CUI未收到上线应答, 重新发一下
            cRsp.SetCuSessionId(strSessId);
        }
        PostSipRsp2UA(cReq, cRsp, pcSipMsg->GetSipTransID());
    }
}

void CCuiSsnTask::OnCuOfflineReq( CMessage* const pcMsg )
{
    COspSipMsg *pcSipMsg = reinterpret_cast<COspSipMsg*>(pcMsg->content);

    CCuOfflineReq cReq;
    pcSipMsg->GetMsgBody(cReq);
    m_cCuserOnlineData.DeleteCuserOnlineKey(cReq.GetCuSessionId());
    CCuOfflineRsp cRsp;
    PostSipRsp2UA(cReq, cRsp, pcSipMsg->GetSipTransID());
}

void CCuiSsnTask::OnTaskMsgTransfer( CMessage* const pcMsg )
{
    ptrint ptr= *reinterpret_cast<ptrint*>(pcMsg->content);
    CTaskMsgTransferBase *pcTrans = reinterpret_cast<CTaskMsgTransferBase*>(ptr);

    if (pcTrans->GetEventId() == msgCuserOnlineRsp)
    {
        TCuOnlineRspExtern *ptRspExtern = &static_cast<CTaskMsgTransfer<TCuOnlineRspExtern>*>(pcTrans)->GetMsgObj();

        if ((ptRspExtern->cRsp.GetErrorCode() == CMS_SUCCESS || ptRspExtern->cRsp.GetErrorCode() == ERR_TAS_PASSWORD_EXPIRED) && !ptRspExtern->MultiLogin)
        {
			if (ptRspExtern->cRsp.GetErrorCode() == CMS_SUCCESS)
			{
				if (AppData().IsCuserNameExist(ptRspExtern->UserName))
				{
					ptRspExtern->cRsp.SetErrorCode(ERR_TAS_USER_DISABLE_MULTILOGIN);
				}
			}    
        }

        TCuserOnlineKey cKey(ptRspExtern->UserName, ptRspExtern->cRsp.GetSeqNum());
        if (ptRspExtern->cRsp.GetErrorCode() == CMS_SUCCESS || ptRspExtern->cRsp.GetErrorCode() == ERR_TAS_PASSWORD_EXPIRED)
        {
            m_cCuserOnlineData.AssignCuserSessId(cKey, ptRspExtern->cRsp.GetCuSessionId(), ptRspExtern->UserId, this);

            PostMsgRsp(pcTrans->GetSipTransId(), ptRspExtern->cRsp);
        }
        else
        {
            m_cCuserOnlineData.DeleteCuserOnlineKey(cKey);
            PostMsgRsp(pcTrans->GetSipTransId(), ptRspExtern->cRsp);
        }

        return;
    }

    DefaultProc(pcMsg);
}

void CCuiSsnTask::OnExportCuserDevData( CMessage* const pcMsg )
{
    COspSipMsg *pcSipMsg = reinterpret_cast<COspSipMsg*>(pcMsg->content);

    CFileExportReq cReq;
    CFileExportRsp cRsp;

    pcSipMsg->GetMsgBody(cReq);
    if (!IsFileExportTaskExisted(cReq.GetSession()))
    {
        CFileExportTask *pcTask = new CFileExportTask(GetInstance());
        pcTask->InitStateMachine();
        if (pcTask->ProcMsg(pcMsg) == PROCMSG_DEL)
        {
            delete pcTask;
        }
    }
    else
    {
        cRsp.SetErrorCode(ERR_TAS_HANDLE_IN_PROGRESS);
        PostSipRsp2UA(cReq, cRsp, pcSipMsg->GetSipTransID());

        TryTerminateFileExportTask(cReq.GetSession());
        TASKLOG(WARNING_LEV, "Try terminate the old FileExportTask, session=%s\n", cReq.GetSession().c_str());
    }
}

void CuserOnlineData::AddCuserOnlineKey( const TCuserOnlineKey &cKey, const string& cuiAssignSessId )
{
    m_cCuserOnlineInfos.insert(map<TCuserOnlineKey, string>::value_type(cKey, string()));
    m_cCuserSsnId2CuserOnlineKey4Logining.insert(map<string, TCuserOnlineKey>::value_type(cuiAssignSessId, cKey));
}

void CuserOnlineData::DeleteCuserOnlineKey( const TCuserOnlineKey &cKey )
{
    map<TCuserOnlineKey, string>::iterator it = m_cCuserOnlineInfos.find(cKey);
    if (it != m_cCuserOnlineInfos.end())
    {
        if (!it->second.empty()){// 成功登录
            m_cCuserSsnId2CuserOnlineKey.erase(it->second);
            m_cCtrlApp.DelSsnId2TaskMap(it->second);
            m_cCtrlApp.DelCuserName(it->first.Cusername, &it->second);
            TryTerminateFileExportTask(it->second);
        }else{// 正在登录
            for (map<string, TCuserOnlineKey>::iterator itLogining=m_cCuserSsnId2CuserOnlineKey4Logining.begin();
                itLogining!=m_cCuserSsnId2CuserOnlineKey4Logining.end(); ++itLogining)
            {
                if (itLogining->second==cKey)
                {
                    m_cCuserSsnId2CuserOnlineKey4Logining.erase(itLogining);
                    break;
                }
            }
        }

        m_cCuserOnlineInfos.erase(it);
    }
}

void CuserOnlineData::DeleteCuserOnlineKey( const string &strSessId )
{
    map<string, TCuserOnlineKey>::iterator it = m_cCuserSsnId2CuserOnlineKey.find(strSessId);
    if (it != m_cCuserSsnId2CuserOnlineKey.end()){ // 用户登录完成后，用户注销
        m_cCuserOnlineInfos.erase(it->second);
        m_cCtrlApp.DelSsnId2TaskMap(it->first);
        m_cCtrlApp.DelCuserName(it->second.Cusername, &it->first);

        m_cCuserSsnId2CuserOnlineKey.erase(it);
        TryTerminateFileExportTask(strSessId);
    }else{ // 可能是cui提前取消登录
        map<string, TCuserOnlineKey>::iterator itLogining=m_cCuserSsnId2CuserOnlineKey4Logining.find(strSessId);
        if (itLogining != m_cCuserSsnId2CuserOnlineKey4Logining.end())
        {
            m_cCuserOnlineInfos.erase(itLogining->second);
            m_cCuserSsnId2CuserOnlineKey4Logining.erase(itLogining);
        }
    }
}

void CuserOnlineData::AssignCuserSessId( const TCuserOnlineKey &cKey, const string &strSessId, const string &strCuserId, CCuiSsnTask *pcSsnTask )
{
    map<TCuserOnlineKey, string>::iterator it = m_cCuserOnlineInfos.find(cKey);
    if (it != m_cCuserOnlineInfos.end())
    {
        m_cCuserSsnId2CuserOnlineKey.insert(map<string, TCuserOnlineKey>::value_type(strSessId, it->first));
        m_cCuserSsnId2CuserOnlineKey4Logining.erase(strSessId);
        it->second = strSessId;

        m_cCtrlApp.AddSsnId2TaskMap(strSessId, pcSsnTask);
        m_cCtrlApp.AddCuserName(it->first.Cusername, strSessId, strCuserId);
    }// 找不到有可能是cui提前取消了登录
}

bool CuserOnlineData::GetCuserSessId( const TCuserOnlineKey &cKey, string &strSessId )
{
    map<TCuserOnlineKey, string>::iterator it = m_cCuserOnlineInfos.find(cKey);
    if (it != m_cCuserOnlineInfos.end())
    {
        strSessId = it->second;
        return true;
    }
    else
    {
        return false;
    }
}

CuserOnlineData::CuserOnlineData( CCtrlAppData &cAppData ): m_cCtrlApp(cAppData)
{

}

CuserOnlineData::~CuserOnlineData()
{
    for (map<string, TCuserOnlineKey>::iterator it = m_cCuserSsnId2CuserOnlineKey.begin(); 
        it != m_cCuserSsnId2CuserOnlineKey.end(); ++it)
    {
        m_cCtrlApp.DelSsnId2TaskMap(it->first);
        m_cCtrlApp.DelCuserName(it->second.Cusername, &it->first);
        TryTerminateFileExportTask(it->first);
    }
}

void CuserOnlineData::PrintData() const
{
    TelePrint("CuserOnlineData size is %u", m_cCuserSsnId2CuserOnlineKey.size());

    u32 i = 0;
    for (map<string, TCuserOnlineKey>::const_iterator it = m_cCuserSsnId2CuserOnlineKey.begin(); it != m_cCuserSsnId2CuserOnlineKey.end(); ++it)
    {
        TelePrint("%u Logined UserName:%s SessId:%s", i, it->second.Cusername.c_str(), it->first.c_str());
        ++i;
    }
    i = 0;
    for (map<string, TCuserOnlineKey>::const_iterator it = m_cCuserSsnId2CuserOnlineKey4Logining.begin(); 
        it != m_cCuserSsnId2CuserOnlineKey4Logining.end(); ++it)
    {
        TelePrint("%u Logining UserName:%s SessId:%s", i, it->second.Cusername.c_str(), it->first.c_str());
        ++i;
    }
}

