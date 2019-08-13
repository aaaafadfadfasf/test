
#include "cms/cms_errorcode.h"

#include "cmudata.h"
#include "cmuconst.h"
#include "cmucoreapp.h"
#include "puptzctrltask.h"
#include "puregtask.h"
#include "cmuconnecttask.h"
#include "culogintask.h"

CPuPtzCtrlTask::CPuPtzCtrlTask(CInstExt* pInst) : CSipTask(pInst)
{
    m_tSipTransId = INVALID_TRANS_ID;
}

CPuPtzCtrlTask::~CPuPtzCtrlTask()
{	
    ReleaseResource();
}

void CPuPtzCtrlTask::ReleaseResource()
{	
	/* ������ʱ��ֻҪ�ҵ������˾�Del����Ҫ��Add��ʱ����û���������� 2018/9/26 zsy
    switch(GetState())
    {
    case Req:
        {
            if (!IsLocalDomainByDevUri(m_tReq.GetDevChn().GetDevUri()))
            {
                //���������Ҫɾ����¼����
                TSipURI tCmuUri = GetCmuUriFromDevUri(m_tReq.GetDevChn().GetDevUri());
                CLoginSession* ptCmuTask = NULL;
                g_cCmuList.Find(tCmuUri.GetURIString(), ptCmuTask);
                if (ptCmuTask != NULL)
                {
                    ptCmuTask->DelTask(this);
                }
            }            
        }
        break;

    case WaitRsp:
        {
            //���������Ҫɾ����¼����
            TSipURI tCmuUri = GetCmuUriFromDevUri(m_tReq.GetDevChn().GetDevUri());
            CLoginSession* ptCmuTask = NULL;
            g_cCmuList.Find(tCmuUri.GetURIString(), ptCmuTask);
            if (ptCmuTask != NULL)
            {
                ptCmuTask->DelTask(this);
            }           
        }
        break;

    case Hold:
        {
            //���������Ҫɾ����¼����
            TSipURI tCmuUri = GetCmuUriFromDevUri(m_tReq.GetDevChn().GetDevUri());
            CLoginSession* ptCmuTask = NULL;
            g_cCmuList.Find(tCmuUri.GetURIString(), ptCmuTask);
            if (ptCmuTask != NULL)
            {
                ptCmuTask->DelTask(this);
            }          

            CPuData *pcPuData = CCmuData::GetInstance()->GetPuDataByUri(m_tReq.GetDevChn().GetDevUri());
            if (NULL != pcPuData && NULL != pcPuData->GetPuRegTask())
            {
                pcPuData->GetPuRegTask()->ErasePtzCtrlTask(m_tReq.GetDevChn().GetChnNO());
            }
        }
        break;

    default:
        {
            TASKLOG(CMU_MOD_PUI, ERROR_LEV, "δ֪����״̬[%d]\n", GetState());
        }
        break;
    }
	*/

	TSipURI tCmuUri = GetCmuUriFromDevUri(m_tReq.GetDevChn().GetDevUri());
	CLoginSession* ptCmuTask = NULL;
	g_cCmuList.Find(tCmuUri.GetURIString(), ptCmuTask);
	if (ptCmuTask != NULL)
	{
		ptCmuTask->DelTask(this);
	}


	CLoginSession* ptUserLoginTask = GetLoginSession(m_tReq.GetSession());
	if (ptUserLoginTask != NULL)
	{
		ptUserLoginTask->DelTask(this);
	}

	if (GetState() == Hold)//ֻ��Hold״̬����ʾ��ǰ�û��Ǿ���PTZʵ�ʿ��Ƶģ������ʱ�Լ�ҪRelease�ˣ��ſ����ͷ�ErasePtzCtrlTask��������Щ��Ϊ��ռʧ�ܵ�������뱾����Ҳ����������Ȩ�޵�Task���£�������PTZ���ƶ����������� 2018/11/28 zsy
	{
		CPuData *pcPuData = CCmuData::GetInstance()->GetPuDataByUri(m_tReq.GetDevChn().GetDevUri());
		if (NULL != pcPuData && NULL != pcPuData->GetPuRegTask())
		{
			pcPuData->GetPuRegTask()->ErasePtzCtrlTask(m_tReq.GetDevChn());
		}
	}
}


void CPuPtzCtrlTask::InitStateMachine()
{
    CStateProc cReqProc;
    cReqProc.ProcMsg = (CTask::PFProcMsg)&CPuPtzCtrlTask::OnReq;
    cReqProc.ProcErrMsg = (CTask::PFProcMsg)&CPuPtzCtrlTask::OnReq;
    cReqProc.TimerPoll = (CTask::PFTimerPoll)&CPuPtzCtrlTask::OnReqTimer;
    AddRuleProc(Req, cReqProc);

    CStateProc cWaitRspProc;
    cWaitRspProc.ProcMsg = (CTask::PFProcMsg)&CPuPtzCtrlTask::OnWaitRsp;
    cWaitRspProc.ProcErrMsg = (CTask::PFProcMsg)&CPuPtzCtrlTask::OnWaitRsp;
    cWaitRspProc.TimerPoll = (CTask::PFTimerPoll)&CPuPtzCtrlTask::OnWaitRspTimer;
    AddRuleProc(WaitRsp, cWaitRspProc);

    CStateProc cHoldProc;
    cHoldProc.ProcMsg = (CTask::PFProcMsg)&CPuPtzCtrlTask::OnHold;
    cHoldProc.ProcErrMsg = (CTask::PFProcMsg)&CPuPtzCtrlTask::OnHold;
    cHoldProc.TimerPoll = (CTask::PFTimerPoll)&CPuPtzCtrlTask::OnHoldTimer;
    AddRuleProc(Hold, cHoldProc);

    NextState(Req);
}

void CPuPtzCtrlTask::PrintData() const
{
    CTask::PrintData();

    OspPrintf(TRUE, FALSE, "\n  opr_dev[%s-%d] - opr_user[%s-%s]  SIP-trans[%u]\n",
        m_tReq.GetDevChn().GetDevUri().c_str(), m_tReq.GetDevChn().GetChnNO(),
        m_tReq.GetUserName().c_str(), m_tReq.GetSession().c_str(), m_tSipTransId);

    const TCamCtrl& tCamCtrl = m_tReq.GetCamCtrl();
    const vector<int>& tParams = tCamCtrl.GetParams();

    OspPrintf(TRUE, FALSE, "  commandId[%d], priLevel[%d], holdTime[%d], matrixIn[%d]\n",
        tCamCtrl.GetCommandId(), tCamCtrl.GetPriLevel(),
        tCamCtrl.GetHoldTime(), tCamCtrl.GetMatrixIn());

    for (size_t i=0; i<tParams.size(); i++)
    {
        char szParam[64] = {0};
        sprintf(szParam, "Param[%u]: %d", i, tParams[i]);
        OspPrintf(TRUE, FALSE, "    %s\n", szParam);
    }

    OspPrintf(TRUE, FALSE, "\n");
}

u32 CPuPtzCtrlTask::OnReq(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_OK;
    switch(pcMsg->event)
    {
    case PU_PTZ_CTRL_REQ:
        {
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

            m_tSipTransId = pcOspSipMsg->GetSipTransID();

            CReq cReq;
            pcOspSipMsg->GetMsgBody(cReq);
            m_tReq = cReq;

            //ȷ��pu·��
            TSipURI tPuRouterUri = GetPuRouter(cReq.GetDevChn().GetDevUri());
            if (tPuRouterUri.IsNull())
            {
                TASKLOG(ERROR_LEV, "Dev[%s]�޷�·��, Msg[%s-%d]����ʧ��\n", 
                    m_tReq.GetDevChn().GetDevUri().c_str(), m_tReq.GetEventStr().c_str(), m_tReq.GetEventId());

                SendRsp(ERR_CMU_DEV_NO_ROUTER);
                dwRet = PROCMSG_DEL;
                return dwRet;
            }

            //�û��Ƿ��¼
            CLoginSession* ptUserLoginTask = GetLoginSession(cReq.GetSession());
            if (ptUserLoginTask == NULL)
            {
                TASKLOG(CRITICAL_LEV, "�û�[%s]��¼session[%s]�Ƿ�\n",
                    cReq.GetUserName().c_str(), cReq.GetSession().c_str());

                if (IsLocalDomainByDevUri(cReq.GetSession()))
                {
                    SendRsp(ERR_CMU_CU_LOGIN_SESS_ILLEGAL);
                }
                else
                {
                    SendRsp(ERR_CMU_CMU_NOT_CONN);
                }
                dwRet = PROCMSG_DEL;
                return dwRet;
            }

            //�����豸�Ƚ��п���Ȩ���жϣ������豸�������ж�
            if (IsLocalDomainByDevUri(tPuRouterUri))
            {
                //����Ŀ����ƵԴͨ���Ƿ���PTZ��������
                CPuRegBaseTask* ptPuRegTask = NULL;
                CPuData *pcPuData = CCmuData::GetInstance()->GetPuDataByUri(cReq.GetDevChn().GetDevUri());
                if (NULL != pcPuData)
                {
                    ptPuRegTask = pcPuData->GetPuRegTask();
                }

                if (ptPuRegTask == NULL)
                {
                    TASKLOG(ERROR_LEV, "Dev[%s]������, Msg[%s-%d]����ʧ��\n", 
                        cReq.GetDevChn().GetDevUri().c_str(), m_tReq.GetEventStr().c_str(), m_tReq.GetEventId());

                    SendRsp(ERR_CMU_PU_NOT_ONLINE);
                    dwRet = PROCMSG_DEL;
                    return dwRet;
                }

                //����pu����Ϣ��Ҫ��session��дpu�ĵ�¼session
                cReq.SetSession(ptPuRegTask->GetSession());

                CPuPtzCtrlTask* ptExistTask = (CPuPtzCtrlTask*)ptPuRegTask->GetPtzCtrlTask(m_tReq.GetDevChn());

                //������ڣ��жϵ�ǰ�����Ƿ���Ȩ����ռ��ͬһ��¼�Ự����ֱ����ռ
                if (ptExistTask != NULL)
                {
                    if (ptExistTask->GetReq().GetSession() == m_tReq.GetSession())
                    {
                        //ֱ����ռ
                    }
                    else
                    {
                        CReq& cExistReq = ptExistTask->GetReq();
                        if (m_tReq.GetCamCtrl().GetPriLevel() >= cExistReq.GetCamCtrl().GetPriLevel())
                        {
                            //�µĿ���Ȩ�޴��ڵ�ǰ����Ȩ�ޣ�������ռ
                            TASKLOG(ERROR_LEV, "�û�[%s]-PriLevel[%d]��ռ�û�[%s]-PriLevel[%d]PTZ���Ƴɹ�\n", 
                                cReq.GetUserName().c_str(), cReq.GetCamCtrl().GetPriLevel(),
                                cExistReq.GetUserName().c_str(), cExistReq.GetCamCtrl().GetPriLevel());
                        }
                        else
                        {
                            //��ռʧ��
                            TASKLOG(ERROR_LEV, "�û�[%s]-PriLevel[%d]��ռ�û�[%s]-PriLevel[%d]PTZ����ʧ��\n", 
                                cReq.GetUserName().c_str(), cReq.GetCamCtrl().GetPriLevel(),
                                cExistReq.GetUserName().c_str(), cExistReq.GetCamCtrl().GetPriLevel());

                            CRsp cRsp;
                            cRsp.SetHeadFromReq(m_tReq);
                            cRsp.SetErrorCode(ERR_CMU_USER_PTZ_PRI_LOW);

                            //��д��Ȩ�޵��û���������ռ�û���֪ʧ��ԭ��
                            cRsp.SetErrorDesc(cExistReq.GetUserName());
                            SendRsp(cRsp);
                            dwRet = PROCMSG_DEL;
                            return dwRet;
                        }
                    }
                }
            }
            else
            {
                if (!IsDomainInTopByDevUri(tPuRouterUri))
                {
                    TASKLOG(CMU_MOD_PU, WARNING_LEV, "Dev[%s]������δ����, Msg[%s-%d]����ʧ��\n", 
                        cReq.GetDevChn().GetDevUri().c_str(), m_tReq.GetEventStr().c_str(), m_tReq.GetEventId());

                    SendRsp(ERR_CMU_DOMAIN_NOT_IN_TOP);
                    dwRet = PROCMSG_DEL;
                    return dwRet;
                }   
            }

            //ת����Ϣ
            SendReq(cReq, tPuRouterUri);

            ptUserLoginTask->AddTask(this);
        }
        break;

    case CMU_CMU_CONNECT_SUCCESS:
        {
            //����cmu��¼�ɹ�, ���Է�������
            TSipURI tPuRouterUri = GetPuRouter(m_tReq.GetDevChn().GetDevUri());
            if (!IsLocalDomainByDevUri(tPuRouterUri))
            {
                SendReq(m_tReq, tPuRouterUri);
            }
            else
            {
                TASKLOG(CMU_MOD_PU, ERROR_LEV, "�������豸[%s], Receive Msg[%s-%d]\n", 
                    m_tReq.GetDevChn().GetDevUri().c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            }
        }
        break;

    default:
        {
            TASKLOG(WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            dwRet = PROCMSG_DEL;
        }
        break;
    }

    return dwRet;
}

u32 CPuPtzCtrlTask::OnReqTimer()
{
    //�ö�ʱ���������ʱ���õ�
    if (GetCurStateHoldTime() > CMU_CONNECT_CMU_TIMEOUT)
    {
        TSipURI tCmuUri = GetCmuUriFromDevUri(m_tReq.GetDevChn().GetDevUri());
        TASKLOG(CMU_MOD_PU, WARNING_LEV, "��¼����cmu[%s]��ʱ, Msg[%s-%d]����ʧ��\n", 
            tCmuUri.GetURIString().c_str(), m_tReq.GetEventStr().c_str(), m_tReq.GetEventId());
        SendRsp(ERR_CMU_CMU_NOT_CONN);
        return TIMERPOLL_DEL;
    }

    return TIMERPOLL_DONE;
}

u32 CPuPtzCtrlTask::OnWaitRsp(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_DEL;
    switch(pcMsg->event)
    {
    case PU_PTZ_CTRL_RSP:
        {
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

            CRsp cRsp;
            pcOspSipMsg->GetMsgBody(cRsp);
            cRsp.SetSession(m_tReq.GetSession());
            SendRsp(cRsp);

            if (cRsp.GetErrorCode() == CMS_SUCCESS)
            {
                //ptz���Ƴɹ�������hold״̬
                NextState(Hold);

                CPuRegBaseTask* ptPuRegTask = NULL;
                CPuData *pcPuData = CCmuData::GetInstance()->GetPuDataByUri(m_tReq.GetDevChn().GetDevUri());
                if (NULL != pcPuData)
                {
                    ptPuRegTask = pcPuData->GetPuRegTask();
                }

                if (ptPuRegTask != NULL)
                {
                    ptPuRegTask->SetPtzCtrlTask(m_tReq.GetDevChn(), this);
                }
                return PROCMSG_OK;
            }
            else
            {
                TASKLOG(ERROR_LEV, "�û�[%s]���豸[%s-%d]PTZ����ʧ�ܣ�������[%d]\n", 
                    m_tReq.GetUserName().c_str(), m_tReq.GetDevChn().GetDevUri().c_str(), 
                    m_tReq.GetDevChn().GetChnNO(), cRsp.GetErrorCode());

                return dwRet;
            }
        }

    case OSP_SIP_MSG_PROC_FAIL:
        {
            //����������յ�SIP��Ĵ���Ӧ��
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

            TASKLOG(ERROR_LEV, "���豸[%s]����ʧ��, ����SIP�����sip status code[%d]\n",
                m_tReq.GetDevChn().GetDevUri().c_str(), pcOspSipMsg->GetSipErrorCode());

            SendRsp(ERR_CMU_DEV_NO_ROUTER);

            return dwRet;
        }
        break;

    default:
        {
            TASKLOG(WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            dwRet = PROCMSG_FAIL;
        }
        break;
    }

    return dwRet;
}

u32 CPuPtzCtrlTask::OnWaitRspTimer()
{
    //Ӧ��ʱ
    if (GetCurStateHoldTime() > CMU_REQ_RSP_TASK_TIMEOUT)
    {
        TASKLOG(ERROR_LEV, "Dev[%s]Ӧ��ʱ, Msg[%s-%d]����ʧ��\n", 
            m_tReq.GetDevChn().GetDevUri().c_str(), m_tReq.GetEventStr().c_str(), m_tReq.GetEventId());

        SendRsp(ERR_CMU_DEV_RSP_TIMEOUT);
        return TIMERPOLL_DEL;
    }

    return TIMERPOLL_DONE;
}

u32 CPuPtzCtrlTask::OnHold(CMessage *const pcMsg)
{
    //����ֹͣ���Ƶ��߼�������ʹ������holdtimeΪ0����ֹ��ptz�Ŀ���
    //todo

    return PROCMSG_OK;
}

u32 CPuPtzCtrlTask::OnHoldTimer()
{
    if ((int)GetCurStateHoldTime() > m_tReq.GetCamCtrl().GetHoldTime())
    {
        TASKLOG(WARNING_LEV, "�û�[%s]���豸[%s-%d]PTZ����ʱ�䵽�ڣ�ֹͣ���û��Ŀ���\n", 
            m_tReq.GetUserName().c_str(), m_tReq.GetDevChn().GetDevUri().c_str(), m_tReq.GetDevChn().GetChnNO());

        return TIMERPOLL_DEL;
    }

    return TIMERPOLL_DONE;
}

void CPuPtzCtrlTask::SendReq(const CReq& cReq, const TSipURI& tDstUri)
{
    if (IsLocalDomainByDevUri(tDstUri))
    {
        //����ֱ��Ͷ��
        PostMsgReq(cReq, tDstUri);
        NextState(WaitRsp);
    }
    else
    {
        //���������û�е�½��������Ҫ�ȵ�¼����¼�ɹ����ٷ����������
        TSipURI tCmuUri = GetCmuUriFromDevUri(tDstUri);
        CLoginSession* ptCmuTask = NULL;
        g_cCmuList.Find(tCmuUri.GetURIString(), ptCmuTask);
        CMS_ASSERT(NULL != ptCmuTask && "ptCmuTask = NULL!!!");         // ǰ���Ѿ�������ʱ���ڵ�
		if (ptCmuTask)
		{
			if (ptCmuTask->GetState() == CCmuConnecterTask::Service)
			{
				PostMsgReq(cReq, tCmuUri);
				NextState(WaitRsp);
			}

			//����������Ҫ�Ǽǵ�cmu��¼�Ự��
			//���ڳ������������Ϊ�������ʱ�ͷų����������Դ
			//����˲ʱ���������Ϊ��cmu��¼�ɹ���ʱ֪ͨ��������
			//����ɾ��ʱ��������������Ҫȥ����������cmu��¼�Ự�еĵǼ�
			ptCmuTask->AddTask(this);
		}
    }
}

bool CPuPtzCtrlTask::SendRsp(u32 dwError)
{
    CRsp cRsp;
    cRsp.SetSeqNum(m_tReq.GetSeqNum());
    cRsp.SetSession(m_tReq.GetSession());
    cRsp.SetErrorCode((int)dwError);

    return (PROCMSG_OK == PostMsgRsp(m_tSipTransId, cRsp));
}

bool CPuPtzCtrlTask::SendRsp(const CRsp& cRsp)
{
    return (PROCMSG_OK == PostMsgRsp(m_tSipTransId, cRsp));
}
