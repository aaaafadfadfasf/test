#include "ps_ssn_task.h"
#include "cctrl.h"
#include "gb_report_catalog_file_export.h"

CPsSsnTask::CPsSsnTask( CInstExt *pcInst ): CSsnTaskbase(pcInst)
{
    AppData().GetChangeNtfSubscribeTasks().insert(this);
}

CPsSsnTask::~CPsSsnTask()
{
    AppData().GetChangeNtfSubscribeTasks().erase(this);
}

const char* CPsSsnTask::GetStateName( u32 dwState ) const
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

void CPsSsnTask::InitStateMachine()
{
    NextState(enStateInit);

    CStateProc cProc;
    cProc.TimerPoll = &CTask::TimerPollSkip;

    cProc.ProcMsg = (CTask::PFProcMsg)&CPsSsnTask::InitStateProcMsg;
    cProc.ProcErrMsg = cProc.ProcMsg;
    AddRuleProc(enStateInit, cProc);
    cProc.ProcMsg = (CTask::PFProcMsg)&CPsSsnTask::ServerStateProcMsg;
    cProc.ProcErrMsg = cProc.ProcMsg;
    cProc.TimerPoll = static_cast<PFTimerPoll>(&CSsnTaskbase::PollChangeNotifyLst);
    AddRuleProc(enStateServer, cProc);
}

u32 CPsSsnTask::InitStateProcMsg( CMessage *const pcMsg )
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

        return PROCMSG_DEL;
    }

    CLoginTasRsp cLoginRsp;
    cLoginRsp.SetErrorCode(CMS_SUCCESS);
    cLoginRsp.SetSession(GetSessionId());
    PostSipRsp2UA(cLoginReq, cLoginRsp, pcSipMsg->GetSipTransID());

    TPSLoginExternInfo tLoginExtern;
    tLoginExtern.FromXml(cLoginReq.GetExtraInfo());

    m_cNtfFilterTypes.clear();
    for (set<int>::iterator it = tLoginExtern.resourceTypes.begin(); 
         it != tLoginExtern.resourceTypes.end(); ++it)
    {
        m_cNtfFilterTypes.insert(static_cast<ENTasResourceTypes>(*it));
    }

	m_bNeedDevGrpGbId=tLoginExtern.needGrpGbid;

    SetClientSipURI(pcSipMsg->GetSipFromUri());
    if (!SetHb())
    {
        return PROCMSG_DEL;
    }

    NextState(enStateServer);
    return PROCMSG_OK;
}

u32 CPsSsnTask::ServerStateProcMsg( CMessage *const pcMsg )
{
	switch(pcMsg->event)
	{
	case FILE_EXPORT_REQ:
		{
			CGbReportCatalogFileExportTask *pcTask = new CGbReportCatalogFileExportTask(GetInstance());
			pcTask->InitStateMachine();
			u32 dwRet = pcTask->ProcMsg(pcMsg);
			if (PROCMSG_DEL == dwRet)
			{
				delete pcTask;
				pcTask = NULL;
			}
			return PROCMSG_OK;
		}
		break;
	default:
		break;
	}

	return DefaultProc(pcMsg);
}
