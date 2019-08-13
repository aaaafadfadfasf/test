
#include "cms/cms_errorcode.h"
#include "cms/cmu/cmu_proto.h"


#include "devlogintask.h"
#include "cmucoreapp.h"
#include "cms/commonconfig/cmcfg.h"

CGeneralLoginList g_cGeneralLoginList;

CLoginSenderTask::CLoginSenderTask(CInstExt *pcInst) : CLoginSession(pcInst)
{
}

CLoginSenderTask::~CLoginSenderTask()
{
    ReleaseResource();
}

void CLoginSenderTask::ReleaseResource()
{
    TASKLOG(EVENT_LEV, "dev[%s-%s]-sess[%s]��¼��������\n",
        GetDevType().c_str(), GetDevUri().c_str(), GetSession().c_str());

    switch(GetState())
    {
    case WaitLogin:
        {
        }
        break;

    case Service:
        {
            //��uasд�豸������־
            ((CCmuCoreInst*)GetInstance())->GetUasLogTask()->AddDeviceLog(GetDevLogInfo(false));

            //����õ�¼�Ự����������
            DestroyChildTasks();

            //ֹͣ����
            StopHB(GetServerSipUri());

            //�����豸��¼�б�
            g_cGeneralLoginList.Erase(GetSession());
        }
        break;

    case WaitLogout:
        {
            //����õ�¼�Ự����������
            DestroyChildTasks();

            //�����豸��¼�б�
            g_cGeneralLoginList.Erase(GetSession());
        }
        break;

    default:
        {
            TASKLOG(ERROR_LEV, "δ֪����״̬[%d]\n", GetState());
        }
        break;
    }
}

void CLoginSenderTask::InitStateMachine()
{
    CStateProc cWaitLoginProc;
    cWaitLoginProc.ProcMsg = (CTask::PFProcMsg)&CLoginSenderTask::OnWaitLogin;
    cWaitLoginProc.ProcErrMsg = (CTask::PFProcMsg)&CLoginSenderTask::OnWaitLogin;
    cWaitLoginProc.TimerPoll = (CTask::PFTimerPoll)&CLoginSenderTask::OnWaitLoginTimer;
    AddRuleProc(WaitLogin, cWaitLoginProc);

    CStateProc cServiceProc;
    cServiceProc.ProcMsg = (CTask::PFProcMsg)&CLoginSenderTask::OnService;
    cServiceProc.ProcErrMsg = (CTask::PFProcMsg)&CLoginSenderTask::OnService;
    cServiceProc.TimerPoll = (CTask::PFTimerPoll)&CLoginSenderTask::OnServiceTimer;
    AddRuleProc(Service, cServiceProc);

    CStateProc cWaitLogoutProc;
    cWaitLogoutProc.ProcMsg = (CTask::PFProcMsg)&CLoginSenderTask::OnWaitLogout;
    cWaitLogoutProc.ProcErrMsg = (CTask::PFProcMsg)&CLoginSenderTask::OnWaitLogout;
    cWaitLogoutProc.TimerPoll = (CTask::PFTimerPoll)&CLoginSenderTask::OnWaitLogoutTimer;
    AddRuleProc(WaitLogout, cWaitLogoutProc);

    NextState(WaitLogin);
}

void CLoginSenderTask::StartLogin()
{
    NextState(WaitLogin);
    CDevLoginReq cReq;
    cReq.SetDevUri(GetDevUri());
    cReq.SetDevType(GetDevType());
    cReq.SetDevAddrList(GetDevAddrList());

    u32 dwRet = PROCMSG_FAIL;
    dwRet = PostMsgReq(cReq, GetServerSipUri());
    if (dwRet != PROCMSG_OK)
    {
        TASKLOG(ERROR_LEV, "����SIP��Ϣʧ��!\n");
    }
}

void CLoginSenderTask::StartLogout()
{
    if (!IsService())
    {
        TASKLOG(ERROR_LEV, "��û�е�¼�ɹ�������ǳ�!\n");
        return;
    }

    CDevLogoutReq cReq;
    cReq.SetSession(GetSession());
    cReq.SetDevUri(GetDevUri());

    u32 dwRet = PROCMSG_FAIL;
    dwRet = PostMsgReq(cReq, GetServerSipUri());
    if (dwRet != PROCMSG_OK)
    {
        TASKLOG(ERROR_LEV, "����SIP��Ϣʧ��!\n");
    }

    StopHB(GetServerSipUri());

    NextState(WaitLogout);
}

u32 CLoginSenderTask::OnWaitLogin(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_FAIL;
    switch(pcMsg->event)
    {
    case DEV_LOGIN_RSP:
        {
            //�豸�յ���¼Ӧ��
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            //���ټ��pcOspSipMsg�Ƿ�Ϊ�գ����Ϊ�վ�ֱ�ӱ�����

            CDevLoginRsp cRsp;
            pcOspSipMsg->GetMsgBody(cRsp);
            int dwErrorCode = cRsp.GetErrorCode();
            if (dwErrorCode == CMS_SUCCESS)
            {
                //��¼�ɹ�
                TASKLOG(CRITICAL_LEV, "��¼������[%s]�ɹ�!\n",
                    GetServerUri().c_str());

                //����session��
                SetSession(cRsp.GetSession());

                //��ʼ����
                SetHBParam(GetServerSipUri());

                //��ȡ�Զ˵�ַ��Ϣ
                CNetAddrQueryReq cReq;
                cReq.SetSession(cRsp.GetSession());
                PostMsgReq(cReq, GetServerSipUri());

                //�������̬
                NextState(Service);

                //��uasд�豸��־
                 ((CCmuCoreInst*)GetInstance())->GetUasLogTask()->AddDeviceLog(GetDevLogInfo(true));

                //��ӵ�Dev�б���
                g_cGeneralLoginList.Insert(GetServerSipUri().GetURIString(), this);

                dwRet = PROCMSG_OK;
            }
            else
            {
                //��¼ʧ��
                TASKLOG(ERROR_LEV, "��¼������[%s]ʧ��,�ȴ���ʱ��������errcode[%d]\n",
                    GetServerUri().c_str(), dwErrorCode);

                dwRet = PROCMSG_OK;
            }
        }
        break;

    case OSP_SIP_MSG_PROC_FAIL:
        {
            //�豸����ĵ�¼�����յ�SIP��Ĵ���Ӧ��
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            //���ټ��pcOspSipMsg�Ƿ�Ϊ�գ����Ϊ�վ�ֱ�ӱ�����

            //��¼ʧ��
            TASKLOG(ERROR_LEV, "��¼������[%s]ʧ��,�ȴ���ʱ��������sip status code[%d]\n",
                GetServerUri().c_str(), pcOspSipMsg->GetSipErrorCode());

            dwRet = PROCMSG_OK;
        }
        break;

    default:
        {
            TASKLOG(WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }

    return dwRet;
}

u32 CLoginSenderTask::OnWaitLoginTimer()
{
    if (GetCurStateHoldTime() > DEV_LOGIN_TIMEOUT)
    {
        TASKLOG(WARNING_LEV, "��¼������[%s]��ʱ,�ȴ���ʱ������\n",
            GetServerUri().c_str());

        //���µ�¼
        StartLogin();
    }

    return TIMERPOLL_DONE;
}

u32 CLoginSenderTask::OnService(CMessage *const pcMsg)
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
                TASKLOG(CRITICAL_LEV, "��ȡ������[%s]��ַ�ɹ�!\n",
                    GetServerUri().c_str());

                SetServerAddrList(cRsp.GetDevAddrList());

                dwRet = PROCMSG_OK;
            }
            else
            {
                TASKLOG(ERROR_LEV, "��ȡ������[%s]ʧ��, errcode[%d]\n",
                    GetServerUri().c_str(), dwErrorCode);

                dwRet = PROCMSG_OK;
            }
        }
        break;

    case OSP_SIP_DISCONNECT:
        {
            TASKLOG(CRITICAL_LEV, "dev[%s]��server[%s]����, �ȴ���ʱ������\n", 
                GetDevUri().c_str(), GetServerUri().c_str());

            //��uasд�豸������־
            ((CCmuCoreInst*)GetInstance())->GetUasLogTask()->AddDeviceLog(GetDevLogInfo(false));

            //���µ�¼
            StartLogin();

            //֪ͨ�������������������
            CTask* pTask = NULL;
            Iterator pPos;
            while (!pPos.End())
            {
                if (IterateChildTasks(pPos, pTask) && pTask != NULL)
                {
                    pTask->ProcMsg(pcMsg);
                }               
            }            

            dwRet = PROCMSG_OK;
        }
        break;

    default:
        {
            TASKLOG(WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }

    return dwRet;
}

u32 CLoginSenderTask::OnServiceTimer()
{
    return TIMERPOLL_DONE;
}

u32 CLoginSenderTask::OnWaitLogout(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_FAIL;
    switch(pcMsg->event)
    {
    case DEV_LOGOUT_RSP:
        {
            //�豸�յ��ǳ�Ӧ��
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            //���ټ��pcOspSipMsg�Ƿ�Ϊ�գ����Ϊ�վ�ֱ�ӱ�����

            CDevLogoutRsp cRsp;
            pcOspSipMsg->GetMsgBody(cRsp);
            int dwErrorCode = cRsp.GetErrorCode();
            if (dwErrorCode == CMS_SUCCESS)
            {
                //�ǳ��ɹ�
                TASKLOG(CRITICAL_LEV, "�ǳ�������[%s]�ɹ�!\n",
                    GetServerUri().c_str());         
            }
            else
            {
                //�ǳ�ʧ��
                TASKLOG(ERROR_LEV, "�ǳ�������[%s]ʧ�ܣ�errcode[%d]����Ҫǿ��\n",
                    GetServerUri().c_str(), dwErrorCode);
            }

            //������������Ҫɾ��
            dwRet = PROCMSG_DEL;
        }
        break;

    case OSP_SIP_MSG_PROC_FAIL:
        {
            //�豸����ĵ�¼�����յ�SIP��Ĵ���Ӧ��
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            //���ټ��pcOspSipMsg�Ƿ�Ϊ�գ����Ϊ�վ�ֱ�ӱ�����

            //�ǳ�ʧ��
            TASKLOG(ERROR_LEV, "�ǳ�������[%s]ʧ��, sip status code[%d]����Ҫǿ��\n",
                GetServerUri().c_str(), pcOspSipMsg->GetSipErrorCode());

            //������������Ҫɾ��
            dwRet = PROCMSG_DEL;
        }
        break;

    default:
        {
            TASKLOG(WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }

    return dwRet;
}

u32 CLoginSenderTask::OnWaitLogoutTimer()
{
    if (GetCurStateHoldTime() > DEV_LOGIN_TIMEOUT)
    {
        TASKLOG(WARNING_LEV, "�ǳ�������[%s]��ʱ,��Ҫǿ��\n",
            GetServerUri().c_str());

        return TIMERPOLL_DEL;
    }

    return TIMERPOLL_DONE;
}

TDeviceLogInfo CLoginSenderTask::GetDevLogInfo(bool bOnOffState)
{
    TDeviceLogInfo tLogInfo;
    tLogInfo.devUri = GetServerUri();

    tLogInfo.devName = GetServerSipUri().GetUser();
    tLogInfo.devAlias = GetServerSipUri().GetUser();

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

//////////////////////////////////////////////////////////////////////////////////////////////

CLoginRecverTask::CLoginRecverTask(CInstExt *pcInst) : CLoginSession(pcInst)
{
}

CLoginRecverTask::~CLoginRecverTask()
{
    ReleaseResource();
}

void CLoginRecverTask::ReleaseResource()
{
    TASKLOG(EVENT_LEV, "dev[%s-%s]-sess[%s]��¼��������\n",
        GetDevType().c_str(), GetDevUri().c_str(), GetSession().c_str());

    switch(GetState())
    {
    case WaitLogin:
        {
        }
        break;

    case Service:
        {
            //��uasд�豸������־
            ((CCmuCoreInst*)GetInstance())->GetUasLogTask()->AddDeviceLog(GetDevLogInfo(false));

            //����õ�¼�Ự����������
            DestroyChildTasks();

            //�����豸��¼�б�
            g_cGeneralLoginList.Erase(GetSession());
        }
        break;

    default:
        {
            TASKLOG(ERROR_LEV, "δ֪����״̬[%d]\n", GetState());
        }
        break;
    }
}

void CLoginRecverTask::InitStateMachine()
{
    CStateProc cWaitLoginProc;
    cWaitLoginProc.ProcMsg = (CTask::PFProcMsg)&CLoginRecverTask::OnWaitLogin;
    cWaitLoginProc.ProcErrMsg = (CTask::PFProcMsg)&CLoginRecverTask::OnWaitLogin;
    cWaitLoginProc.TimerPoll = (CTask::PFTimerPoll)&CLoginRecverTask::OnWaitLoginTimer;
    AddRuleProc(WaitLogin, cWaitLoginProc);

    CStateProc cServiceProc;
    cServiceProc.ProcMsg = (CTask::PFProcMsg)&CLoginRecverTask::OnService;
    cServiceProc.ProcErrMsg = (CTask::PFProcMsg)&CLoginRecverTask::OnService;
    cServiceProc.TimerPoll = (CTask::PFTimerPoll)&CLoginRecverTask::OnServiceTimer;
    AddRuleProc(Service, cServiceProc);

    NextState(WaitLogin);
}

u32 CLoginRecverTask::OnWaitLogin(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_FAIL;
    switch(pcMsg->event)
    {
    case DEV_LOGIN_REQ:
        {
            //�������յ���¼����
            int dwErrorCode = CMS_SUCCESS;
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            //���ټ��pcOspSipMsg�Ƿ�Ϊ�գ����Ϊ�վ�ֱ�ӱ�����

            CDevLoginReq cReq;
            pcOspSipMsg->GetMsgBody(cReq);
            
			//add by Royan 20130828 ����punch����
			if (cReq.GetIsSupportNA())
			{
				SetIsNeedPunch(true);

				//���������ȫ������pu��ƽ̨�������ϵ
			}
			else
			{
				SetIsNeedPunch(false);
			}

            SetServerUri(OspSipGetLocalURI().GetURIString());

            TASKLOG(EVENT_LEV, "�豸[%s][%s]�������[%s]�����¼����\n",
                cReq.GetDevType().c_str(), cReq.GetDevUri().c_str(), GetServerUri().c_str());

            //DevUri�Ϸ��Լ��
            do 
            {   
                TSipURI tDevUri;
                tDevUri.SetURIString(cReq.GetDevUri().c_str());
                if (!tDevUri.IsValidURI())   //��Ч��SIP URI
                {
                    TASKLOG(ERROR_LEV, "��Чdev uri[%s]��¼������[%s]\n",
                        cReq.GetDevUri().c_str(), GetServerUri().c_str());
                    dwErrorCode = ERR_COMMON_INVALID_DEV_URI;
                    break;
                }

                if (g_cGeneralLoginList.Exist(cReq.GetDevUri()))   //�ظ���¼
                {
                    TASKLOG(ERROR_LEV, "�Ѵ���dev uri[%s]��¼������[%s]\n",
                        cReq.GetDevUri().c_str(), GetServerUri().c_str());
                    dwErrorCode = ERR_COMMON_DEV_ALREADY_LOGIN;
                    break;
                }
            } 
            while (0); 

            //���ɵ�¼session, ֱ��ʹ���豸URI      
            CDevLoginRsp cRsp;
            cRsp.SetHeadFromReq(cReq);
            cRsp.SetSession(cReq.GetDevUri());
            cRsp.SetErrorCode(dwErrorCode);
            PostMsgRsp(pcOspSipMsg->GetSipTransID(), cRsp);

            if (dwErrorCode == CMS_SUCCESS)
            {
                SetSession(cReq.GetDevUri());
                SetDevUri(cReq.GetDevUri());
                SetDevType(cReq.GetDevType());
                SetDevAddrList(cReq.GetDevAddrList());

                //��¼�ɹ�
                TASKLOG(CRITICAL_LEV, "�豸[%s][%s]��¼�ɹ�!\n",
                    GetDevType().c_str(), GetDevUri().c_str());

                //��ӵ�Dev�б���
                g_cGeneralLoginList.Insert(GetDevUri(), this);

                //��ʼ����
                SetHBParam(GetDevSipUri());

                //�������̬
                NextState(Service);

                //֪ͨstun��ַ��Ŀǰ������rcs��Ҫ����
                if (GetDevSipUri().GetUser() == DEV_TYPE_RCS)
                {
                    ((CCmuCoreInst*)GetInstance())->UpdateDevStunAddr(GetDevSipUri());
                }
                
                //��uasд�豸��־
                ((CCmuCoreInst*)GetInstance())->GetUasLogTask()->AddDeviceLog(GetDevLogInfo(true));

                dwRet = PROCMSG_OK;
            }
            else
            {
                //ע��ʧ��
                TASKLOG(CRITICAL_LEV, "�豸[%s][%s]��¼ʧ��\n",
                    cReq.GetDevType().c_str(), cReq.GetDevUri().c_str());

                dwRet = PROCMSG_DEL;
            }
        }
        break;

    default:
        {
            TASKLOG(WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }

    return dwRet;
}

u32 CLoginRecverTask::OnWaitLoginTimer()
{
    return TIMERPOLL_DONE;
}

u32 CLoginRecverTask::OnService(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_FAIL;
    switch(pcMsg->event)
    {
    case DEV_LOGOUT_REQ:
        {
            //�������յ��ǳ�����
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            //���ټ��pcOspSipMsg�Ƿ�Ϊ�գ����Ϊ�վ�ֱ�ӱ�����

            CDevLogoutReq cReq;
            pcOspSipMsg->GetMsgBody(cReq);

            CDevLogoutRsp cRsp;
            cRsp.SetHeadFromReq(cReq);
            cRsp.SetErrorCode(CMS_SUCCESS);
            PostMsgRsp(pcOspSipMsg->GetSipTransID(), cRsp);

            //�ǳ��ɹ�
            TASKLOG(CRITICAL_LEV, "�豸[%s][%s]�ǳ��ɹ�!\n",
                GetDevType().c_str(), GetDevUri().c_str());

            //ֹͣ����
            StopHB(GetDevSipUri());

            //ɾ����¼�Ự
            dwRet = PROCMSG_DEL;
        }
        break;

    case OSP_SIP_DISCONNECT:
        {
            TASKLOG(CRITICAL_LEV, "dev[%s]��server[%s]����\n", 
                GetDevUri().c_str(), GetServerUri().c_str());

            dwRet = PROCMSG_DEL;
        }
        break;

    default:
        {
            TASKLOG(WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }

    return dwRet;
}

u32 CLoginRecverTask::OnServiceTimer()
{
    return TIMERPOLL_DONE;
}

//////////////////////////////////////////////////////////////////////////////////////////////

void CGeneralLoginList::PrintData()
{
    Iterator pPos;
    string strSession;
    CLoginSession* pTask = NULL;

    OspPrintf(TRUE, FALSE, "��ǰ���ߵ����裺[%u]��\n", GetSize());    
    u32 i = 0;

    while (!pPos.End())
    {
        if (this->Iterate(pPos, strSession, pTask) && pTask != NULL)
        {
            i++;
            OspPrintf(TRUE, FALSE, "  %4u. dev[%s-%s]-Server[%s]-session[%s]-TaskNO[%u]-State[%s]\n", i, 
                pTask->GetDevType().c_str(), pTask->GetDevUri().c_str(), pTask->GetServerUri().c_str(),
                pTask->GetSession().c_str(), pTask->GetTaskNO(), pTask->GetStateName(pTask->GetState()));
        }

        pTask = NULL;
    }

    OspPrintf(TRUE, FALSE, "��ǰ���ߵ����裺[%u]��\n", GetSize());
}
