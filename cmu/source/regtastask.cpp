#include "regtastask.h"

#include "cms/cms_errorcode.h"
#include "cms/tas/tas_event.h"
#include "cms/tas/tas_struct.h"
#include "cms/tas/tas_proto.h"
#include "cms/cmu/cmu_proto.h"
#include "cms/commonconfig/cmcfg.h"

#include "cmuconst.h"
#include "cmucoreapp.h"
#include "cmudata.h"

CRegTasTask::CRegTasTask(CInstExt *pcInst) : CLoginSession(pcInst)
{
}

CRegTasTask::~CRegTasTask()
{
}

void CRegTasTask::InitStateMachine()
{
    CStateProc cRegTasProc;
    cRegTasProc.ProcMsg = (CTask::PFProcMsg)&CRegTasTask::OnWaitRegTas;
    cRegTasProc.ProcErrMsg = (CTask::PFProcMsg)&CRegTasTask::OnWaitRegTas;
    cRegTasProc.TimerPoll = (CTask::PFTimerPoll)&CRegTasTask::OnWaitRegTasTimer;
    AddRuleProc(WaitRegTas, cRegTasProc);

	CStateProc cQryRspProc;
	cQryRspProc.ProcMsg = (CTask::PFProcMsg)&CRegTasTask::OnWaitQryRsp;
	cQryRspProc.ProcErrMsg = (CTask::PFProcMsg)&CRegTasTask::OnWaitQryRsp;
	cQryRspProc.TimerPoll = (CTask::PFTimerPoll)&CRegTasTask::OnWaitQryRspTimer;
	AddRuleProc(WaitQryRsp, cQryRspProc);

    CStateProc cServiceProc;
    cServiceProc.ProcMsg = (CTask::PFProcMsg)&CRegTasTask::OnService;
    cServiceProc.ProcErrMsg = (CTask::PFProcMsg)&CRegTasTask::OnService;
    cServiceProc.TimerPoll = (CTask::PFTimerPoll)&CRegTasTask::OnServiceTimer;
    AddRuleProc(Service, cServiceProc);

    NextState(WaitRegTas);
}

u32 CRegTasTask::OnWaitRegTas(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_FAIL;

    switch(pcMsg->event)
    {
    case TAS_LOGIN_RSP:
        {
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            if (pcOspSipMsg == NULL)
            {
                TASKLOG(CMU_MOD_3AS, ERROR_LEV, "ospsip ��ϢΪ��\n");
                return PROCMSG_FAIL;
            }

            CLoginTasRsp cRsp;
            pcOspSipMsg->GetMsgBody(cRsp);
            if (cRsp.GetErrorCode() == CMS_SUCCESS)
            {
                //��¼3AS�ɹ�
                TASKLOG(CMU_MOD_3AS, CRITICAL_LEV, "CMU��¼3AS�ɹ�!\n");
                SetSession(cRsp.GetSession());
				SetHBParam(m_tTasURI);

				//��ȡtas��ַ
				StartQryNetAddr();

				NextState(WaitQryRsp);

                //��uasд�豸��־
                ((CCmuCoreInst*)GetInstance())->GetUasLogTask()->AddDeviceLog(GetDevLogInfo(true));
                dwRet = PROCMSG_OK;
            }
            else
            {
                //��¼3ASʧ�ܣ���ʱ�������Ե�¼
                TASKLOG(CMU_MOD_3AS, ERROR_LEV, "CMU��¼3ASʧ�ܣ�errCode[%d], �Ժ�����...\n", 
                    cRsp.GetErrorCode());
            }
        }
        break;

    case OSP_SIP_MSG_PROC_FAIL:
        {
            //����������յ�SIP��Ĵ���Ӧ��
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            if (pcOspSipMsg == NULL)
            {
                TASKLOG(CMU_MOD_3AS, ERROR_LEV, "ospsip ��ϢΪ��\n");
                dwRet = PROCMSG_FAIL;
                return dwRet;
            }

            TASKLOG(CMU_MOD_3AS, ERROR_LEV, "���豸[%s]����ʧ��, ����SIP�����, �ȴ���ʱ���ط���sip status code[%d]\n",
                m_tTasURI.GetURIString().c_str(), pcOspSipMsg->GetSipErrorCode());

            return dwRet;
        }
        break;

    case OSP_SIP_DISCONNECT:
        {
            TasDisProc(pcMsg);
            dwRet = PROCMSG_OK;
        }
        break;

    default:
        {
            TASKLOG(CMU_MOD_3AS, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }

    return dwRet;
}

u32 CRegTasTask::OnWaitRegTasTimer()
{
    if (GetCurStateHoldTime() > CMU_REG_TAS_TIMEOUT)
    {
        TASKLOG(CMU_MOD_3AS, WARNING_LEV, "����3as[%s]��ʱ����ʼ����\n", 
            m_tTasURI.GetURIString().c_str());

        //���·�������
        StartRegTas();
    }

    return TIMERPOLL_DONE;
}

u32 CRegTasTask::OnWaitQryRsp(CMessage *const pcMsg)
{
	u32 dwRet = PROCMSG_FAIL;

	switch(pcMsg->event)
	{
	case NETADDR_QUERY_RSP:
		{
			COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
			//���ټ��pcOspSipMsg�Ƿ�Ϊ�գ����Ϊ�վ�ֱ�ӱ�����

			CNetAddrQueryRsp cRsp;
			pcOspSipMsg->GetMsgBody(cRsp);
			int dwErrorCode = cRsp.GetErrorCode();
			if (dwErrorCode == CMS_SUCCESS)
			{
				TASKLOG(CMU_MOD_3AS, CRITICAL_LEV, "��ȡ������[%s]��ַ�ɹ�!\n",
					GetServerUri().c_str());

				SetServerAddrList(cRsp.GetDevAddrList());

				NextState(Service);

				dwRet = PROCMSG_OK;
			}
			else
			{
				TASKLOG(CMU_MOD_3AS, ERROR_LEV, "��ȡ������[%s]��ַʧ��, errcode[%d]\n",
					GetServerUri().c_str(), dwErrorCode);

				dwRet = PROCMSG_OK;
			}
		}
		break;

	case OSP_SIP_DISCONNECT:
		{
			TasDisProc(pcMsg);
			dwRet = PROCMSG_OK;
		}
		break;

	default:
		{
			TASKLOG(CMU_MOD_3AS, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
		}
		break;
	}

	return dwRet;
}

u32 CRegTasTask::OnWaitQryRspTimer()
{
	if (GetCurStateHoldTime() > CMU_REQ_RSP_TASK_TIMEOUT)
	{
		TASKLOG(CMU_MOD_3AS, WARNING_LEV, "��ѯ3as[%s]��ַ��Ϣ��ʱ�����²�ѯ\n", 
			m_tTasURI.GetURIString().c_str());

		//���·����ѯ
		StartQryNetAddr();
	}

	return TIMERPOLL_DONE;
}

u32 CRegTasTask::OnService(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_FAIL;

    switch(pcMsg->event)
    {
    case OSP_SIP_DISCONNECT:
        {
            TasDisProc(pcMsg);
            dwRet = PROCMSG_OK;
        }
        break;

    default:
        {
            TASKLOG(CMU_MOD_3AS, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }

    return dwRet;
}

u32 CRegTasTask::OnServiceTimer()
{
    return TIMERPOLL_DONE;
}

void CRegTasTask::TasDisProc(CMessage *const pcMsg)
{
    COspSipMsg* pOspSipMsg = (COspSipMsg*)pcMsg->content;
    if (pOspSipMsg == NULL)
    {
        TASKLOG(CMU_MOD_3AS, ERROR_LEV, "ospsip ��ϢΪ��\n");
        return;
    }

    //�Ƿ������
    TSipURI tDevUri;
    tDevUri.SetNOString(pOspSipMsg->GetMsgBody());
    if (tDevUri != m_tTasURI)
    {
        TASKLOG(CMU_MOD_3AS, WARNING_LEV, "�յ���Ч������Ϣ�������豸[%s]\n", tDevUri.GetNOString());
        return;
    }

    TASKLOG(CMU_MOD_3AS, CRITICAL_LEV, "3as[%s]��cmu����\n", tDevUri.GetNOString());

    switch(GetState())
    {
    case WaitRegTas:
        {
        }
        break;

    case Service:
        {
            //��uasд�豸������־
            ((CCmuCoreInst*)GetInstance())->GetUasLogTask()->AddDeviceLog(GetDevLogInfo(false));

            StartRegTas();
        }
        break;

    default:
        {
            TASKLOG(CMU_MOD_3AS, ERROR_LEV, "δ֪����״̬[%d]\n", GetState());
        }
        break;
    }
}

TDeviceLogInfo CRegTasTask::GetDevLogInfo(bool bOnOffState)
{
    TDeviceLogInfo tLogInfo;

    tLogInfo.devUri = GetServerUri();

    tLogInfo.devName = UNIQUE_SIP_UA_3AS;
    tLogInfo.devAlias = UNIQUE_SIP_UA_3AS;

    tLogInfo.happenedTime = CCMSTime().ToString();
    tLogInfo.logType = (bOnOffState ? enDeviceLogTypeOnline : enDeviceLogTypeOffline);

    if (GetDevAddrList().size() > 0)
    {
        //ȡ��һ����ַ
        tLogInfo.netAddr = GetDevAddrList()[0].GetNetIp();
    } 
    else
    {
        tLogInfo.netAddr = "127.0.0.1";
    }

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


u32 CRegTasTask::StartRegTas()
{
	SetDevUri(GetLocalUri().GetURIString());
	SetDevType(UNIQUE_SIP_UA_CMU);
	SetServerUri(m_tTasURI.GetURIString());

	CLoginTasReq cReq;
	cReq.SetLoginType(CLoginTasReq::enLoginType_CMU);
	cReq.SetUserName(m_strUserName);

	u32 dwRet = PROCMSG_FAIL;
	dwRet = PostMsgReq(cReq, m_tTasURI);
	if (dwRet != PROCMSG_OK)
	{
		TASKLOG(CMU_MOD_3AS, ERROR_LEV, "����SIP��Ϣʧ��!\n");
		return dwRet;
	}

	NextState(WaitRegTas);

	return dwRet;
}


u32 CRegTasTask::StartQryNetAddr()
{
    //��ȡtas��ַ
    CNetAddrQueryReq cQryAddrReq;
    cQryAddrReq.SetSession(GetSession());
    PostMsgReq(cQryAddrReq, GetTasUri());

    return PROCMSG_OK;
}