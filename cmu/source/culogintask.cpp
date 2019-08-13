
#include "cms/cms_errorcode.h"
#include "cms/cmu/cmu_event.h"
#include "cms/cmu/cmu_struct.h"
#include "cms/cmu/cmu_proto.h"

#include "culogintask.h"
#include "cuiregtask.h"
#include "cmuconst.h"
#include "cmudata.h"
#include "cmucoreapp.h"

CCuList g_cCuList;

CCuLoginTask::CCuLoginTask(CInstExt *pcInst) : CLoginSession(pcInst)
{
}

CCuLoginTask::~CCuLoginTask()
{
    ReleaseResource();
}

void CCuLoginTask::ReleaseResource()
{
    TASKLOG(CMU_MOD_CU, EVENT_LEV, "cu[%s]-sess[%s]��¼��������\n",
        m_tUserName.GetURIString().c_str(), GetSession().c_str());

    switch(GetState())
    {
    case WaitLogin:
        {
        }
        break;

    case Service:
        {
            //��uasд�豸������־
            //((CCmuCoreInst*)GetInstance())->GetUasLogTask()->AddDeviceLog(GetDevLogInfo(false));

            //����õ�¼�Ự����������
            DestroyChildTasks();

            //����cu��¼�б�
            g_cCuList.Erase(GetSession());

            //ɾ��cui�еǼǵ�cu��¼����
            CCuiRegTask* ptCuiRegTask = GetCuiRegTask();
            if (ptCuiRegTask != NULL)
            {
                ptCuiRegTask->DelTask(this);
            }
        }
        break;

    default:
        {
            TASKLOG(ERROR_LEV, "δ֪����״̬[%d]\n", GetState());
        }
        break;
    }
}

void CCuLoginTask::InitStateMachine()
{
    CStateProc cWaitLoginProc;
    cWaitLoginProc.ProcMsg = (CTask::PFProcMsg)&CCuLoginTask::OnWaitLogin;
    cWaitLoginProc.ProcErrMsg = (CTask::PFProcMsg)&CCuLoginTask::OnWaitLogin;
    cWaitLoginProc.TimerPoll = (CTask::PFTimerPoll)&CCuLoginTask::OnWaitLoginTimer;
    AddRuleProc(WaitLogin, cWaitLoginProc);

    CStateProc cServiceProc;
    cServiceProc.ProcMsg = (CTask::PFProcMsg)&CCuLoginTask::OnService;
    cServiceProc.ProcErrMsg = (CTask::PFProcMsg)&CCuLoginTask::OnService;
    cServiceProc.TimerPoll = (CTask::PFTimerPoll)&CCuLoginTask::OnServiceTimer;
    AddRuleProc(Service, cServiceProc);

    NextState(WaitLogin);
}

u32 CCuLoginTask::OnWaitLogin(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_FAIL;
    switch(pcMsg->event)
    {
    case CU_LOGIN_REQ:
        {
            int dwErrorCode = CMS_SUCCESS;
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            if (pcOspSipMsg == NULL)
            {
                TASKLOG(CMU_MOD_CU, ERROR_LEV, "ospsip ��ϢΪ��\n");
                dwErrorCode = ERR_CMU_SIP_BODY_NULL;
                dwRet = PROCMSG_DEL;
                return dwRet;
            }

            CCuLoginReq cReq;
            pcOspSipMsg->GetMsgBody(cReq);
            m_tUserName.SetURIString(cReq.GetUserName());
            //���ɵ�¼session
            CUUID cCuLoginSess;  //���캯���Զ�����Session ID
            TSipURI tSessUri;    //session��Ҳ����session@domain�ķ�ʽ������CMUͳһ����
            tSessUri.SetUser(cCuLoginSess);
            tSessUri.SetDomain(GetLocalUri().GetDomain());
            SetSession(tSessUri.GetURIString());
            m_strCuVer = cReq.GetVersion();
            m_tCuiUri = pcOspSipMsg->GetSipFromUri();

            TASKLOG(CMU_MOD_CU, CRITICAL_LEV, "CU[%s]ͨ��CUI[%s]��CMUע��\n",
                m_tUserName.GetURIString().c_str(), m_tCuiUri.GetURIString().c_str());

            CCuiRegTask* ptCuiRegTask = GetCuiRegTask();
            if (ptCuiRegTask == NULL)
            {
                TASKLOG(CMU_MOD_CU, ERROR_LEV, "cu��¼ʹ�õ�cui[%s]δ��cmuע��\n",
                    m_tCuiUri.GetURIString().c_str());
                dwErrorCode = ERR_CMU_CUI_NOT_REG;
            }

            CCuLoginRsp cRsp;
            cRsp.SetErrorCode(dwErrorCode);
            cRsp.SetSeqNum(cReq.GetSeqNum());
            cRsp.SetSession(GetSession());
			cRsp.SetCmsVer(GetCommonCmsVer());
            PostMsgRsp(pcOspSipMsg->GetSipTransID(), cRsp);

            if (dwErrorCode == CMS_SUCCESS)
            {
                //��¼�ɹ�
                TASKLOG(CMU_MOD_CU, CRITICAL_LEV, "cu[%s]��¼cmu�ɹ�!\n",
                    m_tUserName.GetURIString().c_str());

                SetDevUri(GetSession());  //cuʹ�õ�¼session��ΪDevUri
                SetDevType(DEV_TYPE_CU);
                SetServerUri(OspSipGetLocalURI().GetURIString());
                SetCuiIp(cReq.GetUsedCuiIp());

                //����punch����
                if (cReq.GetIsSupportNA())
                {
                    SetIsNeedPunch(true);
                }
                else
                {
                    SetIsNeedPunch(false);
                }

                //��ӵ�CU�б���
                g_cCuList.Insert(GetSession(), this);

                //��ӵ�CUI�У�CUI����ʱ��Ҫʹ��
                ptCuiRegTask->AddTask(this);

                //�������̬
                NextState(Service);

                //��uasд�豸��־
                // ((CCmuCoreInst*)GetInstance())->GetUasLogTask()->AddDeviceLog(GetDevLogInfo(true));

                dwRet = PROCMSG_OK;
            }
            else
            {
                //ע��ʧ��
                TASKLOG(CMU_MOD_CU, ERROR_LEV, "cu[%s]��¼cmuʧ��!\n",
                    m_tUserName.GetURIString().c_str());

                dwRet = PROCMSG_DEL;
            }
        }
        break;

    default:
        {
            TASKLOG(CMU_MOD_CU, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }

    return dwRet;
}

u32 CCuLoginTask::OnWaitLoginTimer()
{
    return TIMERPOLL_DONE;
}

u32 CCuLoginTask::OnService(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_FAIL;
    switch(pcMsg->event)
    {
	case UPDATE_CULOGIN_INFO_REQ:
		{
			u32 dwErrorCode = CMS_SUCCESS;
			COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
			if (pcOspSipMsg == NULL)
			{
				TASKLOG(CMU_MOD_CU, ERROR_LEV, "ospsip ��ϢΪ��\n");
				dwErrorCode = ERR_CMU_SIP_BODY_NULL;
				return dwRet;
			}

			CUpdateCuLoginInfoReq cReq;
			pcOspSipMsg->GetMsgBody(cReq);

			TASKLOG(CMU_MOD_CU, CRITICAL_LEV, "UpdateCuLoginInfoReq[%s]\n", cReq.ToXml().c_str());
			SetIsNeedPunch(cReq.GetIsSupportNA());

			CUpdateCuLoginInfoRsp cRsp;
			cRsp.SetHeadFromReq(cReq);
			PostMsgRsp(pcOspSipMsg->GetSipTransID(), cRsp);

			dwRet = PROCMSG_OK;
		}
		break;

    case CU_GETCUILIST_REQ:
        {
            u32 dwErrorCode = CMS_SUCCESS;
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            if (pcOspSipMsg == NULL)
            {
                TASKLOG(CMU_MOD_CU, ERROR_LEV, "ospsip ��ϢΪ��\n");
                dwErrorCode = ERR_CMU_SIP_BODY_NULL;
                return dwRet;
            }

            CCuGetCuiListReq cReq;
            pcOspSipMsg->GetMsgBody(cReq);

            CCuGetCuiListRsp cRsp;
            cRsp.SetSeqNum(cReq.GetSeqNum());
            cRsp.SetSession(cReq.GetSession());

            //��дipaddrlist
            CIpAddrList& cIpAddrList = cRsp.GetIpAddrList();
            Iterator pPos;
            string strUri;
            CLoginSession* pTask = NULL;
            while (!pPos.End())
            {
                if (g_cCuiList.Iterate(pPos, strUri, pTask) && pTask != NULL)
                {
                    CCuiRegTask* pCuiRegTask = (CCuiRegTask*)pTask;
                    const CIpAddrList& cCuiIpAddrList = pCuiRegTask->GetIpAddrList();
                    cIpAddrList.insert(cIpAddrList.begin(), cCuiIpAddrList.begin(), cCuiIpAddrList.end());
                }
            }

            PostMsgRsp(pcOspSipMsg->GetSipTransID(), cRsp);

            dwRet = PROCMSG_OK;
        }
        break;

    case CU_LOGOUT_REQ:
        {
            LogoutProc(pcMsg);
            dwRet = PROCMSG_DEL;
        }
        break;

    default:
        {
            TASKLOG(CMU_MOD_CU, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }

    return dwRet;
}

u32 CCuLoginTask::OnServiceTimer()
{
    return TIMERPOLL_DONE;
}

void CCuLoginTask::LogoutProc(CMessage *const pcMsg)
{
    int dwErrorCode = CMS_SUCCESS;
    COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
    if (pcOspSipMsg == NULL)
    {
        TASKLOG(CMU_MOD_CU, ERROR_LEV, "ospsip ��ϢΪ��\n");
        return;
    }

    CCuLogoutReq cReq;
    pcOspSipMsg->GetMsgBody(cReq);

    CCuLogoutRsp cRsp;
    cRsp.SetErrorCode(dwErrorCode);
    cRsp.SetSeqNum(cReq.GetSeqNum());
    cRsp.SetSession(GetSession());
    PostMsgRsp(pcOspSipMsg->GetSipTransID(), cRsp);
}

CCuiRegTask* CCuLoginTask::GetCuiRegTask()
{
    CLoginSession* ptCuiRegTask = NULL;
    g_cCuiList.Find(m_tCuiUri.GetURIString(), ptCuiRegTask);
    return (CCuiRegTask*)ptCuiRegTask;
}

size_t CCuLoginTask::GetDevNetSegNO()
{
    size_t nCuNetSegNO = 0;
    CCuiRegTask* pcCuiTask = GetCuiRegTask();
    if (pcCuiTask == NULL)
    {
        TASKLOG(CMU_MOD_CU, ERROR_LEV, "pcCuiTask == NULL\n");
        return nCuNetSegNO;
    }

    //const CIpAddrList& cCuiAddrList = pcCuiTask->GetDevAddrList();
    //for (size_t i=0; i<cCuiAddrList.size(); i++)
    //{
    //    const TNetAddr& tCuiAddr = cCuiAddrList.at(i);
    //    if (tCuiAddr.GetNetIp() == GetCuiIp())
    //    {
    //        nCuNetSegNO = i;
    //        TASKLOG(CMU_MOD_CU, CRITICAL_LEV, "Cu[%s]-usedCuiIp[%s]ʹ��cui[%s]��NetSegNO[%u]����\n", 
    //            GetDevUri().c_str(), GetCuiIp().c_str(), 
    //            pcCuiTask->GetDevUri().c_str(), nCuNetSegNO);

    //        return nCuNetSegNO;
    //    }
    //}
	// �޸�Ϊʹ��CUI�Լ���������������Ϣ [1/26/2015 pangubing]

	//todo Royan 20150318
	//const TNetSegmentList&  cCuiNetSegmentList = pcCuiTask->GetNetSegmentList();
	//for (size_t i=0; i<cCuiNetSegmentList.size(); i++)
	//{
	//	const TNetSegment& tCuiAddr = cCuiNetSegmentList.at(i);
	//	if (tCuiAddr.GetSegmentIp() == GetCuiIp())
	//	{
	//		nCuNetSegNO = tCuiAddr.GetSegmentNum();
	//		TASKLOG(CMU_MOD_CU, CRITICAL_LEV, "Cu[%s]-usedCuiIp[%s]ʹ��cui[%s]��NetSegNO[%u]����\n", 
	//			GetDevUri().c_str(), GetCuiIp().c_str(), 
	//			pcCuiTask->GetDevUri().c_str(), nCuNetSegNO);

	//		return nCuNetSegNO;
	//	}
	//}


    TASKLOG(CMU_MOD_CU, ERROR_LEV, "cu[%s]-usedCuiIp[%s]û���ҵ�cui[%s]��Ӧ��NetSegNO\n", 
        GetDevUri().c_str(), GetCuiIp().c_str(), pcCuiTask->GetDevUri().c_str());

    return nCuNetSegNO;
}

string CCuLoginTask::GetJoinInterfaceIp(const string &strSession)
{
	return m_strCuiIp;
}

void CCuList::PrintData()
{
    Iterator pPos;
    string strSession;
    CLoginSession* pTask = NULL;

    OspPrintf(TRUE, FALSE, "��ǰ���ߵ�cu��[%u]��\n", GetSize());    
    u32 i = 0;

    while (!pPos.End())
    {
        if (this->Iterate(pPos, strSession, pTask) && pTask != NULL)
        {
            CCuLoginTask* pCuLoginTask = (CCuLoginTask*)(pTask);
            i++;
            OspPrintf(TRUE, FALSE, "  %4u. cu[%s]-taskNO[%u]-CuiIp[%s]-session[%s]\n", i, 
                pCuLoginTask->GetUserName().GetURIString().c_str(), pTask->GetTaskNO(),
                pCuLoginTask->GetCuiIp().c_str(),pCuLoginTask->GetSession().c_str());
        }
    }

    OspPrintf(TRUE, FALSE, "��ǰ���ߵ�cu��[%u]��\n", GetSize());
}
