#include "tac_ssn_task.h"
#include "cctrl.h"
#include "tasks/tas_config_task.h"
#include "tasks/tas_ctrl_task.h"
#include "tasks/license_mng_task.h"
#include "tasks/g300_del_task.h"
#include "commonconfig/cmcfg.h"

CTacSsnTask::CTacSsnTask( CInstExt *pcInst ): CSsnTaskbase(pcInst)
{
    // printf("appdata pointer=%u in CTacSsnTask\n", (u32)&AppData()); 
    // printf("appdata real pointer=%u in CTacSsnTask\n", (u32)&g_cCtrlApp.GetAppData()); 
    AppData().GetChangeNtfSubscribeTasks().insert(this);

    m_cNtfFilterTypes.clear();

    m_cNtfFilterTypes.insert(TasResourceGeneralDev);
    m_cNtfFilterTypes.insert(TasResourceDevModel);
    m_cNtfFilterTypes.insert(TasResourceDomainRelation);

    m_cNtfFilterTypes.insert(TasResourceTuser);
    m_cNtfFilterTypes.insert(TasResourceSyncDstDomainCfg);
}

CTacSsnTask::~CTacSsnTask()
{
    // printf("appdata pointer=%u in ~CTacSsnTask\n", (u32)&AppData()); 
    // printf("appdata real pointer=%u in ~CTacSsnTask\n", (u32)&g_cCtrlApp.GetAppData()); 

    AppData().GetChangeNtfSubscribeTasks().erase(this);
}

const char* CTacSsnTask::GetStateName( u32 dwState ) const
{
    if (enStateInit == dwState)
    {
        return "InitState";
    }
    else if (enStateLoginInProcess == dwState)
    {
        return "LoginInProcessState";
    }
    else if (enStateLogined == dwState)
    {
        return "LoginedState";
    }
    else
    {
        return "unknown";
    }
}

void CTacSsnTask::InitStateMachine()
{
    NextState(enStateInit);

    CStateProc cProc;
    cProc.TimerPoll = &CTask::TimerPollSkip;

    cProc.ProcMsg = (CTask::PFProcMsg)&CTacSsnTask::InitStateProcMsg;
    cProc.ProcErrMsg = cProc.ProcMsg;
    AddRuleProc(enStateInit, cProc);
    cProc.ProcMsg = (CTask::PFProcMsg)&CTacSsnTask::LoginInProcessStateProcMsg;
    cProc.ProcErrMsg = cProc.ProcMsg;
    AddRuleProc(enStateLoginInProcess, cProc);
    cProc.ProcMsg = (CTask::PFProcMsg)&CTacSsnTask::LoginedStateProcMsg;
    cProc.ProcErrMsg = cProc.ProcMsg;
    cProc.TimerPoll = static_cast<PFTimerPoll>(&CSsnTaskbase::PollChangeNotifyLst);
    AddRuleProc(enStateLogined, cProc);
}

u32 CTacSsnTask::InitStateProcMsg( CMessage *const pcMsg )
{
    if (pcMsg->event != TAS_LOGIN_REQ)
    {
        Log(ERROR_LEV, "InitState Recved unknown message [%d-%s]\n", pcMsg->event, OspExtEventDesc(pcMsg->event).c_str());
        return PROCMSG_DEL;
    }

    COspSipMsg *pcSipMsg = reinterpret_cast<COspSipMsg*>(pcMsg->content);

    CTaskMsgTransfer<CLoginTasReq> cTrans(GetTaskNO(), pcSipMsg->GetSipTransID(), pcMsg->event);
    pcSipMsg->GetMsgBody(cTrans.GetMsgObj());

    CLoginTasReq &cLoginReq = cTrans.GetMsgObj();
    CLoginTasRsp cLoginRsp;

    string strModuleUri = pcSipMsg->GetSipFromUri().GetURIString();
    m_strUserName = cLoginReq.GetUserName();

    if (AppData().IsClientModuleUriExist(strModuleUri))
    {
        cLoginRsp.SetErrorCode(ERR_TAS_LOGINED);
        PostSipRsp2UA(cLoginReq, cLoginRsp, pcSipMsg->GetSipTransID());

        Log(CRITICAL_LEV, "tac login repeatly, moduleuri=%s\n", strModuleUri.c_str());
        return PROCMSG_DEL;
    }  

    if (PostTaskMsgTransfer2DbTaskPool(cTrans))
    {
        SetClientSipURI(pcSipMsg->GetSipFromUri());
        NextState(enStateLoginInProcess);
        return PROCMSG_OK;
    }
    else
    {
        cLoginRsp.SetErrorCode(ERR_TAS_SYSTEM_BUSY);
        PostSipRsp2UA(cLoginReq, cLoginRsp, pcSipMsg->GetSipTransID());

        return PROCMSG_DEL;
    }
}

u32 CTacSsnTask::LoginInProcessStateProcMsg( CMessage *const pcMsg )
{
    if (msgTaskMsgTransfer4Db == pcMsg->event)
    {
        CTaskMsgTransfer<CLoginTasRsp> *pcTransfer = *reinterpret_cast<CTaskMsgTransfer<CLoginTasRsp>**>(pcMsg->content);

        CLoginTasRsp &cLoginTasRsp = pcTransfer->GetMsgObj();
        cLoginTasRsp.SetSession(GetSessionId());
        PostMsgRsp(pcTransfer->GetSipTransId(), cLoginTasRsp);

        if (CMS_SUCCESS == cLoginTasRsp.GetErrorCode())
        {
            if (!SetHb())
            {
                return PROCMSG_DEL;
            }

            NextState(enStateLogined);
            return PROCMSG_OK;
        }
        else
        {
            return PROCMSG_DEL;
        }
    }
    else
    {
        Log(WARNING_LEV, "CTacSsnTask::LoginInProcessStateProcMsg recved unknown message, [%u-%s]\n", 
            pcMsg->event, OspExtEventDesc(pcMsg->event).c_str());
        return PROCMSG_DEL;
    }
}

u32 CTacSsnTask::LoginedStateProcMsg( CMessage *const pcMsg )
{
    switch(pcMsg->event)
    {
    case TAS_CONFIG_TAS_SET_REQ:
    case TAS_CONFIG_TAS_GET_REQ:
    case TAS_CHILDTAS_ADD_REQ:
    case TAS_CHILDTAS_DEL_REQ:
        {
            CTasConfigTask *pcTask = new CTasConfigTask(GetInstance());
            pcTask->InitStateMachine();

            u32 dwRet = pcTask->ProcMsg(pcMsg);
            if (PROCMSG_DEL == dwRet)
            {
                delete pcTask;
            }

            return PROCMSG_OK;
        }
        break;
    case TAS_CTRL_TAS_REQ:
        {
            CTasCtrlTask *pcTask = new CTasCtrlTask(GetInstance());
            pcTask->InitStateMachine();

            u32 dwRet = pcTask->ProcMsg(pcMsg);
            if (PROCMSG_DEL == dwRet)
            {
                delete pcTask;
            }

            return PROCMSG_OK;
        }
        break;
    case TAS_LICENSE_UPDATE_REQ:
    case TAS_LICENSE_GET_REQ:
    case TAS_LICENSE_SET_REQ:
        {
            CLicenseMngTask *pcTask = new CLicenseMngTask(GetInstance());
            pcTask->InitStateMachine();

            u32 dwRet = pcTask->ProcMsg(pcMsg);
            if (PROCMSG_DEL == dwRet)
            {
                delete pcTask;
            }

            return PROCMSG_OK;
        }
        break;
	case TAS_GATEWAY_DEL_REQ:
		{
			CG300DelTask *pcTask = new CG300DelTask(GetInstance());
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

bool CTacSsnTask::ChangeNotifyFilter( u16 wMsgId/*wMsgId*/, TChangeNtfItem &cItem )
{
	ENTasResourceTypes enResourceType;
	ENTasResourceTypes enResourceTypeSub=TasResourceEnd;
	switch(wMsgId)
	{
	case TAS_DEVICE_ADD_NTF_REQ:
		{
			CDeviceAddNtfReq cNtfReq;
			cNtfReq.FromXml(cItem);
			return GetDomainUUID()==cNtfReq.GetDeviceInfo().GetDomainUUID()
				&& cNtfReq.GetDeviceInfo().GetCreater() != CREATE_BY_GBS_FOREIGN_DOMAIN;
		}
	case TAS_DEVICE_DEL_NTF_REQ:
		{
			CDeviceDelNtfReq cNtfReq;
			cNtfReq.FromXml(cItem);
			return GetDomainUUID()==cNtfReq.GetDomainId()
				&& cNtfReq.GetCreater() != CREATE_BY_GBS_FOREIGN_DOMAIN;
		}
	case TAS_DEVICE_MOD_NTF_REQ:
		{
			CDeviceModNtfReq cNtfReq;
			cNtfReq.FromXml(cItem);
			return GetDomainUUID()==cNtfReq.GetDeviceInfo().GetDomainUUID()
				&& cNtfReq.GetDeviceInfo().GetCreater() != CREATE_BY_GBS_FOREIGN_DOMAIN;
		}
	case TAS_TVWALL_ADD_NTF_REQ:
	case TAS_TVWALL_DEL_NTF_REQ:
	case TAS_TVWALL_MOD_NTF_REQ:
		{
			enResourceType = TasResourceTvwallDev;
			break;
		}
	case TAS_DEVICE_MODEL_ADD_NTF_REQ:
		{
			CDeviceModelAddNtfReq cNtfReq;
			cNtfReq.FromXml(cItem);
			return GetDomainUUID()==cNtfReq.GetDeviceModelInfo().GetDomainId()
				&& cNtfReq.GetDeviceModelInfo().GetCreater() != CREATE_BY_GBS_FOREIGN_DOMAIN;
		}
		break;
	case TAS_DEVICE_MODEL_DEL_NTF_REQ:
		{
			enResourceType = TasResourceDevModel;
			break;
		}
	case TAS_DEVICE_CAP_INDEX_NAME_MOD_NTF_REQ:
		{
			enResourceType = TasResourceDevCapIndexName;
			break;
		}
	case TAS_CUSER_ADD_NTF_REQ:
	case TAS_CUSER_DEL_NTF_REQ:
	case TAS_CUSER_MOD_NTF_REQ:
		{
			enResourceType = TasResourceCuser;
			break;
		}
	case TAS_CUSER_DEVICE_ADD_NTF_REQ:
	case TAS_CUSER_DEVICE_DEL_NTF_REQ:
	case TAS_CUSER_DEVICE_MOD_NTF_REQ:
		{
		    enResourceType = TasResourceCuser2Dev;
			break;
		}
	case TAS_DEVICE_GROUP_ADD_NTF_REQ:
	case TAS_DEVICE_GROUP_DEL_NTF_REQ:
	case TAS_DEVICE_GROUP_MOD_NTF_REQ:
		{
			enResourceType = TasResourceDevGrp;
			break;
		}
	case TAS_DEVICE_GROUP_DEVICE_ADD_NTF_REQ:
	case TAS_DEVICE_GROUP_DEVICE_DEL_NTF_REQ:
		{
			enResourceType = TasResourceDevGrp2Dev;
			break;
		}
	case TAS_CUSER_GROUP_ADD_NTF_REQ:
	case TAS_CUSER_GROUP_DEL_NTF_REQ:
	case TAS_CUSER_GROUP_MOD_NTF_REQ:
		{
			enResourceType = TasResourceCuserGrp;
			break;
		}
	case TAS_CUSER_GROUP_DEVICE_ADD_NTF_REQ:
	case TAS_CUSER_GROUP_DEVICE_DEL_NTF_REQ:
	case TAS_CUSER_GROUP_DEVICE_MOD_NTF_REQ:
		{
			enResourceType = TasResourceCuserGrp2Dev;
			break;
		}
	case TAS_DOMAIN_RELATION_ADD_NTF_REQ:
	case TAS_DOMAIN_RELATION_DEL_NTF_REQ:
	case TAS_DOMAIN_RELATION_MOD_NTF_REQ:
		{
			enResourceType = TasResourceDomainRelation;
			break;
		}
	case TAS_TACUSER_ADD_NTF_REQ:
	case TAS_TACUSER_DEL_NTF_REQ:
	case TAS_TACUSER_MOD_NTF_REQ:
		{
			enResourceType = TasResourceTuser;
			break;
		}
	case TAS_SYNCDSTDOMAIN_ADD_NTF_REQ:
	case TAS_SYNCDSTDOMAIN_DEL_NTF_REQ:
	case TAS_SYNCDSTDOMAIN_MOD_NTF_REQ:
		{
			enResourceType = TasResourceSyncDstDomainCfg;
			break;
		}
	case TAS_TEMP_AUTHORIZATION_ADD_NTF_REQ:
	case TAS_TEMP_AUTHORIZATION_DEL_NTF_REQ:
	case TAS_TEMP_AUTHORIZATION_MOD_NTF_REQ:
		{
			enResourceType = TasResourceTmpAuthorizationCfg;
			break;
		}
	case TAS_PTZLOCK_ADD_NTF_REQ:
	case TAS_PTZLOCK_DEL_NTF_REQ:
		{
			enResourceType = TasResourcePtzLock;
			break;
		}
	case TAS_LICENSE_UPDATE_NTF_REQ:
		{
			enResourceType = TasLicenseUpdateNotify;
			break;
		}
	default:
		return false;
	}

	return m_cNtfFilterTypes.end() != m_cNtfFilterTypes.find(enResourceType)||m_cNtfFilterTypes.end() != m_cNtfFilterTypes.find(enResourceTypeSub);
}
