/*****************************************************************************
ģ����      : cui_fcgi 
�ļ���      : cuitask.h
����ļ�    : 
�ļ�ʵ�ֹ���: cuitask.h �������е�����
����        : liangli
�汾        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
�޸ļ�¼:
��  ��      �汾        �޸���      �޸�����
2011/11/9   1.0         liangli        ����
******************************************************************************/
#include <cctype>
#include <string>
#include <algorithm>
#include "culogintask.h"
#include "cuiproto.h"
#include "cms/commonconfig/cmcfg.h"
#include "cuiapp.h"
#include "cms/utility/md5.h"
#include "subscribe_task.h"
#include "cms/commonconfig/cmcfg.h"
#include "cms/commonconfig/xmlconfig.h"
#include "cms/utility/digital_string.h"
#include "export_task.h"
#include "import_task.h"
#include "dpssearchtask.h"
#include "redisdb.h"
#include "StreamDecentrationAdapter.h"
#include "sdk/redissdk/redissdk.h"

extern CRedisDBInterface g_cRedisDb;
extern CRedisSdk g_cCuiRedisSdk;

static s32 s_nMobileClientNum = 0;
extern vector<std::string> g_cuSessionForNatPacketClear;
CCuAgent::CCuAgent(CInstExt *pcInst, const string &strClientType) : CSipTask(pcInst)
{
    //Clear();
    m_ptCuLoginNode = NULL;
    m_ptCuLogoutNode = NULL;
    m_pcUserAccount = NULL;
    m_bIsSupportNA = true;
    m_bNeedPlatformServiceStatue = false;
    m_ptNotifyRecvNode = NULL;
    m_strClientType = strClientType;
	m_strClientLocalIp="";
	m_strWebCuClientIp="";
	m_dwClientVer = CU_VER_WITHOUT_NATOPT;
	m_eClientNat=NATModeAuto;
	m_ePlatNat=NATModeAuto;
	m_nPwdLockRemainTime = 0;
	m_nPwdExpireRemainTime = 0;
	m_nPwdRemainError = CMS_SUCCESS;
    if (IsMobileClientType())
    {
        ++s_nMobileClientNum;
    }
	m_cNotifyPool.SetMaxCuNtfSize(g_cCuiConfig.GetMaxCuNtfSize());

	m_strCuSessionIn3AS = CUUID();
}

CCuAgent::~CCuAgent()
{
    if (IsMobileClientType())
    {
        --s_nMobileClientNum;
		ECMS_ERRORCODE eErrCode = g_cCuiRedisSdk.UpdateCuiOnlineMobileNum(g_cCuiConfig.GetLocalURI().GetUser(), GetMobileClientNumber());
		if( eErrCode != CMS_SUCCESS )
		{
			TASKLOG(LOGIN_TASK, EVENT_LEV, "�����ƶ��ͻ���������redis���ݿ�ʧ��\n");
		}
		TASKLOG(LOGIN_TASK, EVENT_LEV, "�����ƶ��ͻ�������[%d -- %d]��redis���ݿ�\n",(GetMobileClientNumber()+1),GetMobileClientNumber());
    }

    if (NULL != m_ptNotifyRecvNode)
    {
        // �ظ�һ�������ͷ�node
        CGetNotifyRsp cRsp;
        SendRspToCu<CGetNotifyRsp>(cRsp, m_ptNotifyRecvNode);
    }

    TASKLOG(LOGIN_TASK, EVENT_LEV, "��ʼ�����û�[%s]�Ự[%s]��CCuAgent[TaskNO %u]\n", 
        m_strUserName.c_str(), m_strSession.c_str(), GetTaskNO());

    TSipURI sessionUri( m_strSession );
    CStreamDecentrationAdapter::UnregisterCu( g_cCuiConfig.GetLocalURI().GetUser(), sessionUri.GetUser() );

    C3acAgent *pc3acAgentTask = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(m_str3asName, GetInstance());
    if (NULL != pc3acAgentTask)
    { //tas��������CU�˳�������٣����Բ�������Ҳ���
        CMS_ASSERT( 0 == strcmp(pc3acAgentTask->GetObjName(), "C3acAgent") );
    }
    else
    { //CU��¼ʱTAS�����ߣ����ᴴ��C3acAgent
        TASKLOG(LOGIN_TASK, CRITICAL_LEV, "���棡�������û�[%s]�Ự[%s]��CCuAgent[TaskNO %u]ʱ���Ҳ���[%s]\n",
            m_strUserName.c_str(), m_strSession.c_str(), GetTaskNO(), m_str3asName.c_str());
        return;
    }


    //������ý���ʽ�ͷ���Դ, û��break
    switch (GetState())
    {
    case ONLINE_STATE:
        {
			if ( NULL != pc3acAgentTask )
            {
                CUserAccount *pcUserAccount = pc3acAgentTask->m_cUserMgr.FindUserAccountByUserName(m_strUserName);
                CMS_ASSERT(NULL != pcUserAccount);
                if (pcUserAccount)
				{
					bool bRet = pcUserAccount->DeleteCuAgent(this);
					CMS_ASSERT(bRet && "CuAgent����ʱ��������UserAccount���ڵǼ�");
				}

                /*
                ĳ���û��˻����ܴ��û����˳���Ҳ���ܼ����û���
                ����CU�˳�ʱҪ���̼���Ƿ���Ҫɾ���û��˻�
                //2012.3.27 �޸����㷨�����ݽṹ��CUserAccount�Ӵ�����Ͳ��ᱻɾ��������3acAgent������
                */
                //pc3acAgentTask->CheckUserOnline();]

				if (! m_strCuSessionIn3AS.empty())
				{
					CCuOfflineReq cCuOfflineReq;
					cCuOfflineReq.SetSession(pc3acAgentTask->m_strSessionID);
					cCuOfflineReq.SetCuSessionId(GetNameWithoutDomainFromURI(m_strCuSessionIn3AS));
					FakePostMsgReq(cCuOfflineReq, pc3acAgentTask->m_t3asURI);

					g_cCuiDataMgr.m_mapSessToTaskMap.Erase(m_strCuSessionIn3AS);
				}
            }

            //ɾ�����û�������������
            ClearAllSubTask();

            CCuLogoutReq cCuLogoutReq;
            cCuLogoutReq.SetSession(m_strSession);
            FakePostMsgReq(cCuLogoutReq, m_tCmuURI);

            g_cCuiDataMgr.m_mapSessToTaskMap.Erase(m_strSession);
        }
		break;
    case WAIT_CMU_ADD_CU_RSP_STATE:
        {
			if ( NULL != pc3acAgentTask )
            {
                CUserAccount *pcUserAccount = pc3acAgentTask->m_cUserMgr.FindUserAccountByUserName(m_strUserName);
                CMS_ASSERT(NULL != pcUserAccount);
                if (pcUserAccount)
				{
					bool bRet = pcUserAccount->DeleteCuAgent(this);
					CMS_ASSERT(bRet && "CuAgent����ʱ��������UserAccount���ڵǼ�");
				}

                /*
                ĳ���û��˻����ܴ��û����˳���Ҳ���ܼ����û���
                ����CU�˳�ʱҪ���̼���Ƿ���Ҫɾ���û��˻�
                //2012.3.27 �޸����㷨�����ݽṹ��CUserAccount�Ӵ�����Ͳ��ᱻɾ��������3acAgent������
                */
                //pc3acAgentTask->CheckUserOnline();]

				if (! m_strCuSessionIn3AS.empty())
				{
					CCuOfflineReq cCuOfflineReq;
					cCuOfflineReq.SetSession(pc3acAgentTask->m_strSessionID);
					cCuOfflineReq.SetCuSessionId(GetNameWithoutDomainFromURI(m_strCuSessionIn3AS));
					FakePostMsgReq(cCuOfflineReq, pc3acAgentTask->m_t3asURI);

					g_cCuiDataMgr.m_mapSessToTaskMap.Erase(m_strCuSessionIn3AS);
				}
            }
        }
		break;
    case WAIT_3AS_ADD_CU_RSP_STATE:
        {
            //TAS�˳���¼��3acAgent�����
            if ( NULL != pc3acAgentTask )
            {
                CUserAccount *pcUserAccount = pc3acAgentTask->m_cUserMgr.FindUserAccountByUserName(m_strUserName);
                CMS_ASSERT(NULL != pcUserAccount);
				if (pcUserAccount)
				{
					bool bRet = pcUserAccount->DeleteCuAgent(this);
					CMS_ASSERT(bRet && "CuAgent����ʱ��������UserAccount���ڵǼ�");
				}

                /*
                ĳ���û��˻����ܴ��û����˳���Ҳ���ܼ����û���
                ����CU�˳�ʱҪ���̼���Ƿ���Ҫɾ���û��˻�
                //2012.3.27 �޸����㷨�����ݽṹ��CUserAccount�Ӵ�����Ͳ��ᱻɾ��������3acAgent������
                */
                //pc3acAgentTask->CheckUserOnline();]

				if (! m_strCuSessionIn3AS.empty())
				{
					CCuOfflineReq cCuOfflineReq;
					cCuOfflineReq.SetSession(pc3acAgentTask->m_strSessionID);
					cCuOfflineReq.SetCuSessionId(GetNameWithoutDomainFromURI(m_strCuSessionIn3AS));
					FakePostMsgReq(cCuOfflineReq, pc3acAgentTask->m_t3asURI);

					g_cCuiDataMgr.m_mapSessToTaskMap.Erase(m_strCuSessionIn3AS);
				}
            }
        }
		break;
    default:
        {
        }
		break;
    }

    if ( m_ptCuLogoutNode != NULL )
    {
        CCuLogoutRsp cRsp;
        cRsp.SetSession(m_strSession);
        cRsp.SetErrorCode(CMS_SUCCESS);
        SendRspToCu<CCuLogoutRsp>(cRsp, m_ptCuLogoutNode);
    }

    NextState(OFFLINE_STATE);
    Clear();
}

void CCuAgent::Clear()
{
    m_ptCuLoginNode = NULL;
    m_ptCuLogoutNode = NULL;
    m_pcUserAccount = NULL;
    m_ptNotifyRecvNode = NULL;
	m_nPwdRemainError = CMS_SUCCESS;
	m_nPwdExpireRemainTime = 0;
	m_nPwdLockRemainTime = 0;

	//�����ǰsession��
	RemoveNatData(m_strSession,0);

	SetTimer(CUI_CLEAR_VTDUNATPACKET_TIMER, CUI_CLEAR_VTDUNATPACKET_INTERVAL);

	g_cuSessionForNatPacketClear.push_back(m_strSession);
	
	// CU�˳�ʱ��ջ����������ƵԴ����
	CMapCuVideoSrcStatList::iterator iter = g_mapDevVideoSrcStatList.find(m_strSession);
	if ( iter != g_mapDevVideoSrcStatList.end() )
	{
		g_mapDevVideoSrcStatList.erase(iter);
	}

	Iterator iPos;
	TTvChannel tTvCHn;
	while(!iPos.End())
	{
		CTask* pRecTvTask = NULL;
		if (m_tRecordTaskSets.Iterate(iPos,tTvCHn,pRecTvTask))
		{
			if (NULL != pRecTvTask)
			{
				delete pRecTvTask;
				pRecTvTask = NULL;
			}
		}
	}
}

//״̬����
u32 CCuAgent::OfflineProcMsg(CMessage *const pcMsg)
{
    return PROCMSG_OK;
}

u32 CCuAgent::Wait3ASRegRspProcMsg(CMessage *const pcMsg)
{
    COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;

    switch (pcMsg->event)
    {
    case TAS_LOGIN_RSP:
        {
            CLoginTasRsp cRsp;
            pcOspSipMsg->GetMsgBody(cRsp);

            //�жϴ��������ظ�ע��
            if(cRsp.GetErrorCode() == ERR_TAS_USER_DISABLE_MULTILOGIN || cRsp.GetErrorCode() == ERR_TAS_LOGINED)
            {
                u32 dwTaskID = INVALID_TASKNO;			    
                if (g_cCuiDataMgr.m_map3asURITo3asAgentMap.Find(m_str3asName, dwTaskID) )
                {
                    if(dwTaskID != INVALID_TASKNO)
                    {
                        C3acAgent *pc3acAgentTask = (C3acAgent *)(GetInstance()->FindTask(dwTaskID));

                        if(pc3acAgentTask != NULL)
                        {
                            TASKLOG(LOGIN_TASK, CRITICAL_LEV, "�Ѿ����͹���¼���󲢳ɹ�taskID[%d]\n",
                                dwTaskID);
                            CCuOnlineReq cAddCuReq;
                            FillOnlineReq( cAddCuReq, cRsp.GetSession() );

                            FakePostMsgReq(cAddCuReq, pc3acAgentTask->m_t3asURI);

                            NextState(WAIT_3AS_ADD_CU_RSP_STATE);

                            return PROCMSG_OK;
                        }
                        else
                        {
                            TASKLOG(LOGIN_TASK, CRITICAL_LEV, "(1)ͨ��taskID[%d]�Ҳ���task\n",
                                dwTaskID);
                        }
                    }
                    else
                    {
                        TASKLOG(LOGIN_TASK, CRITICAL_LEV, "taskID[%d]��Ч\n",
                            dwTaskID);
                    }
                }
                else
                {
                    TASKLOG(LOGIN_TASK, CRITICAL_LEV, "(2)ͨ��taskID[%d]�Ҳ���task\n",
                        dwTaskID);
                }
            }

            if (cRsp.GetErrorCode() != CMS_SUCCESS)
            { //ע��3ASʧ��
                TASKLOG(LOGIN_TASK, ERROR_LEV, "��¼TASʧ�ܣ�������[%d], �û�[%s]��¼ʧ��\n",
                    cRsp.GetErrorCode(), m_strUserName.c_str());
                CCuiCuLoginRsp cCuLoginRsp;
                cCuLoginRsp.SetErrorCode(cRsp.GetErrorCode());
                SendRspToCu<CCuiCuLoginRsp>(cCuLoginRsp, m_ptCuLoginNode);

                WriteLog(cRsp.GetErrorCode(), true);
                delete this;
                return PROCMSG_OK;
            }


            if(m_str3asName != pcOspSipMsg->GetSipToUri().GetURIString())
            {
                TASKLOG(LOGIN_TASK, ERROR_LEV, "�����е�URI[%s]��Sip URI[%s]��һ��\n",
                    m_str3asName.c_str(), pcOspSipMsg->GetSipFromUri().GetURIString().c_str());
                CCuiCuLoginRsp cCuLoginRsp;
                cCuLoginRsp.SetErrorCode(ERR_CUI_PARAMS_INVALID);
                SendRspToCu<CCuiCuLoginRsp>(cCuLoginRsp, m_ptCuLoginNode);
                WriteLog(ERR_CUI_PARAMS_INVALID, true);
                delete this;
                return PROCMSG_OK;
            }

            //ע��3AS�ɹ�������һ��3ASAgent�����Ա����������3AS��ע��״̬���Լ�����3AS��֪ͨ
            C3acAgent *pc3acAgentTask = new C3acAgent(GetInstance());
            CMS_ASSERT(NULL != pc3acAgentTask);
            pc3acAgentTask->InitStateMachine();
            pc3acAgentTask->m_t3asURI.SetNOString(m_str3asName);
            g_cCuiDataMgr.m_map3asURITo3asAgentMap.Insert(pc3acAgentTask->m_t3asURI.GetURIString(), pc3acAgentTask->GetTaskNO());   //ֻ������Ǽ�һ�Σ�ֱ��C3acAgent����
            //��3ASע��Ӧ����Ϣ���͸�3ASAgent���Ա����¼3AS��Ϣ��ת��״̬
            //��3ASAgent�����3AS��������, �·����3ASAgent���͵�ע������õ�Ӧ��
            pc3acAgentTask->ProcMsg(pcMsg);

            //��3AS���CU���û�
            CCuOnlineReq cAddCuReq;
            FillOnlineReq( cAddCuReq, cRsp.GetSession() );

            FakePostMsgReq(cAddCuReq, pc3acAgentTask->m_t3asURI);
            TASKLOG(LOGIN_TASK, CRITICAL_LEV, "������3AS���CU���û�����ɹ�\n");

            NextState(WAIT_3AS_ADD_CU_RSP_STATE);
        }
        break;
    case OSP_SIP_MSG_PROC_FAIL:
        {
            TASKLOG(LOGIN_TASK, ERROR_LEV, "��TAS����[ CUI��¼���� ]ʧ�ܣ�SIP������[%d], �û�[%s]��¼ʧ��\n",
                pcOspSipMsg->GetSipErrorCode(), m_strUserName.c_str());
            CCuiCuLoginRsp cCuLoginRsp;
            cCuLoginRsp.SetErrorCode(ERR_CUI_TAS_OFFLINE);
            SendRspToCu<CCuiCuLoginRsp>(cCuLoginRsp, m_ptCuLoginNode);
            WriteLog(ERR_CUI_TAS_OFFLINE, true);
            delete this;
        }
        break;
    default:
        {
            TASKLOG(LOGIN_TASK, CRITICAL_LEV, "�յ�δ֪��Ϣ[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }
    return PROCMSG_OK;
}
u32 CCuAgent::Wait3ASRspTimerPoll()
{
    if ( GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout() )
    { //��ʱ
        TASKLOG(LOGIN_TASK, ERROR_LEV, "�ȴ�������Ӧ��ʱ���û�[%s]��¼ʧ��\n",
            m_strUserName.c_str());
        CCuiCuLoginRsp cCuLoginRsp;
        cCuLoginRsp.SetErrorCode(ERR_CUI_OPR_TIME_OUT);
        SendRspToCu<CCuiCuLoginRsp>(cCuLoginRsp, m_ptCuLoginNode);
        WriteLog(ERR_CUI_OPR_TIME_OUT, true);

       // WriteLog();

        return TIMERPOLL_DEL;
    }
    return TIMERPOLL_DONE;
}
u32 CCuAgent::Wait3ASAddCuRspProcMsg(CMessage *const pcMsg)
{
    COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;

    switch (pcMsg->event)
    {
    case TAS_CU_ONLINE_RSP:
        {
            CCuOnlineRsp cAddCuRsp;
            pcOspSipMsg->GetMsgBody(cAddCuRsp);

            if ((cAddCuRsp.GetErrorCode() != CMS_SUCCESS) && (cAddCuRsp.GetErrorCode() != ERR_TAS_PASSWORD_EXPIRED)
				&& ( cAddCuRsp.GetErrorCode() != ERR_TAS_INPUT_ERROR_PASSWORD_TOO_MUCH))
            { //ע��3ASʧ��
                TASKLOG(LOGIN_TASK, EVENT_LEV, "TAS���CU����״̬ʧ�ܣ�������[%d]���û�[%s]��¼ʧ��\n",
                    cAddCuRsp.GetErrorCode(), m_strUserName.c_str());
                CCuiCuLoginRsp cCuLoginRsp;
                cCuLoginRsp.SetErrorCode(cAddCuRsp.GetErrorCode());
                SendRspToCu<CCuiCuLoginRsp>(cCuLoginRsp, m_ptCuLoginNode);
                
                //д�쳣��־
                WriteLog(ERR_CUI_OPR_TIME_OUT, true);

                delete this;
                return PROCMSG_DEL;
            }

			if (ERR_TAS_INPUT_ERROR_PASSWORD_TOO_MUCH == cAddCuRsp.GetErrorCode())
			{
				//�û�������
				TASKLOG(LOGIN_TASK, EVENT_LEV, "�û���������������[%d]���û�[%s]��¼ʧ��\n",
					cAddCuRsp.GetErrorCode(), m_strUserName.c_str());
				CCuiCuLoginRsp cCuLoginRsp;
				cCuLoginRsp.SetErrorCode(cAddCuRsp.GetErrorCode());
				cCuLoginRsp.SetPasswordRemainTime(cAddCuRsp.GetPwdExpireLeftTime());
				cCuLoginRsp.SetUserLockRemainTime(cAddCuRsp.GetPwdLockedLeftTime());
				SendRspToCu<CCuiCuLoginRsp>(cCuLoginRsp, m_ptCuLoginNode);

                m_strLogDesc = GetLanDesc( LAN_CU_PASSWORD_LOCKED );
				//д�쳣��־
				WriteLog(ERR_TAS_INPUT_ERROR_PASSWORD_TOO_MUCH, true, false);

				delete this;
				return PROCMSG_DEL;
			}

			if ( ERR_TAS_PASSWORD_EXPIRED == cAddCuRsp.GetErrorCode() )
			{
				m_nPwdRemainError = ERR_TAS_PASSWORD_EXPIRED;
				TASKLOG(LOGIN_TASK, EVENT_LEV, "�����ѹ��ڣ��������û�[%s]��¼\n",m_strUserName.c_str());
			}

            C3acAgent *pc3acAgentTask = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(m_str3asName, GetInstance());
            if (pc3acAgentTask == NULL)
            {
                TASKLOG(LOGIN_TASK, ERROR_LEV, "TAS���CU����״̬�ɹ�������3acAgent�Ѿ��������ˣ��û�[%s]��¼ʧ��\n",
                    m_strUserName.c_str());
                CCuiCuLoginRsp cCuLoginRsp;
                cCuLoginRsp.SetErrorCode(ERR_CUI_SIP_MSG_FAIL);
                SendRspToCu<CCuiCuLoginRsp>(cCuLoginRsp, m_ptCuLoginNode);
                WriteLog(ERR_CUI_SIP_MSG_FAIL, true);
                delete this;
                return PROCMSG_DEL;
            }
            CMS_ASSERT( 0 == strcmp(pc3acAgentTask->GetObjName(), "C3acAgent") );

            //m_strCuSessionIn3AS = cAddCuRsp.GetCuSessionId();//�Լ����ɣ����ݸ�tasʹ�á�
            g_cCuiDataMgr.m_mapSessToTaskMap.Insert(m_strCuSessionIn3AS, GetTaskNO());
            //�ж��Ƿ��Ѿ����ڸ��û��˻�
            CUserAccount *pcUserAccount = pc3acAgentTask->m_cUserMgr.FindUserAccountByUserName(m_strUserName);
            if ( NULL == pcUserAccount )
            {
                //�ȴ���һ���յ�CUserAccount
                pcUserAccount = new CUserAccount(&(pc3acAgentTask->m_cUserMgr));
                pcUserAccount->m_strUserName = m_strUserName;
                pcUserAccount->m_bDevPriState = DEVPRI_NOT_START;
                pc3acAgentTask->m_cUserMgr.m_mapUserName2PCuserAccount.Insert(pcUserAccount->m_strUserName, pcUserAccount);
            }
            CMS_ASSERT(NULL != pcUserAccount);
            //����Task�Ǽǵ�3acAgent��
            pcUserAccount->AddCuAgent(this);
            m_pcUserAccount = pcUserAccount;    //�������¼ָ�룬//2012.3.37�޸����㷨���ݽṹ��CUserAccount�Ӵ�����Ͳ��ᱻɾ��������3acAgent������
            m_strServerDataVersion = cAddCuRsp.GetDeviceTreeVer();
			m_nPwdLockRemainTime = cAddCuRsp.GetPwdLockedLeftTime();
			m_nPwdExpireRemainTime = cAddCuRsp.GetPwdExpireLeftTime();
            { 
                //��CMU���CU
                CCuLoginReq cReq;
                cReq.SetUserName(m_strUserName);
                cReq.SetVersion(m_strVersion);
                //cReq.SetUsedCuiIp(m_strHostIp);// ����Ӧ�����ô��˰ɣ�Ӧ��ʹ��NAT�ڵ�ַ [1/26/2015 pangubing]
				cReq.SetUsedCuiIp(m_tNatInfo.GetIpAddr());
                cReq.SetIsSupportNA(m_bIsSupportNA);

                FakePostMsgReq(cReq, m_tCmuURI);
                NextState(WAIT_CMU_ADD_CU_RSP_STATE);
            }
        }
        break;
    case OSP_SIP_MSG_PROC_FAIL:
        {
            TASKLOG(LOGIN_TASK, EVENT_LEV, "��TAS����[ ���CU����״̬ ]SIP��Ϣ��ʱ��SIP������[%d]���û�[%s]��¼ʧ��\n",
                pcOspSipMsg->GetSipErrorCode(), m_strUserName.c_str());
            CCuiCuLoginRsp cCuLoginRsp;
            cCuLoginRsp.SetErrorCode(ERR_CUI_SIP_MSG_FAIL);
            SendRspToCu<CCuiCuLoginRsp>(cCuLoginRsp, m_ptCuLoginNode);
            WriteLog(ERR_CUI_SIP_MSG_FAIL, true);
            delete this;
        }
        break;
    default:
        {
            TASKLOG(LOGIN_TASK, CRITICAL_LEV, "�յ�δ֪��Ϣ[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }
    return PROCMSG_OK;
}

u32 CCuAgent::WaitCmuAddCuRspProcMsg(CMessage *const pcMsg)
{
    COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;

    switch (pcMsg->event)
    {
    case CU_LOGIN_RSP:
        {
            CCuLoginRsp cRsp;
            pcOspSipMsg->GetMsgBody(cRsp);

            //��CuAgentTask���뵽SessionIDToMap
            m_strSession = cRsp.GetSession();
            g_cCuiDataMgr.m_mapSessToTaskMap.Insert(m_strSession, GetTaskNO());

            if ( CMS_SUCCESS != cRsp.GetErrorCode() )
            {
                TASKLOG(LOGIN_TASK, ERROR_LEV, "CU��¼CMUʧ�ܣ�������[%d]���û�[%s]��¼ʧ��\n",
                    cRsp.GetErrorCode(), m_strUserName.c_str());
                //CMU���CUʧ�ܣ��ܾ�CU��¼
                CCuiCuLoginRsp cTmpRsp;
                cTmpRsp.SetErrorCode(cRsp.GetErrorCode());
                SendRspToCu<CCuiCuLoginRsp>(cTmpRsp, m_ptCuLoginNode);
                WriteLog(cRsp.GetErrorCode(), true);
                delete this;
                return PROCMSG_OK;
            }

            CCuiCuLoginRsp cCuLoginRsp;
            cCuLoginRsp.SetCmsVer(cRsp.GetCmsVer());
            cCuLoginRsp.SetEvent(cRsp.GetEventStr());
            cCuLoginRsp.SetSeqNum(cRsp.GetSeqNum());
            cCuLoginRsp.SetSession(cRsp.GetSession());

			if(ERR_TAS_PASSWORD_EXPIRED == m_nPwdRemainError)
			{
				cCuLoginRsp.SetErrorCode(m_nPwdRemainError);
			}
			else
			{
				cCuLoginRsp.SetErrorCode(cRsp.GetErrorCode());
			}

            string strCommonCuUpdateUrl;
            if (m_strClientType.empty() || m_strClientType == "cuver")
            {
                cCuLoginRsp.SetCuVersion(GetCommonCuVer());
                strCommonCuUpdateUrl = GetCommonCuUpdateUrl();
            }
            else
            {
                do 
                {
                    string strXmlContent;
                    if (!ReadFileToString("/etc/kdm/commonconfig.ini", strXmlContent))
                    {
                        TASKLOG(LOGIN_TASK, CRITICAL_LEV, "we failed to read commonconfig.ini\n");
                        break;
                    }

                    CXmlConfig cXmlConfig("commonconfig");
                    cXmlConfig.SetXmlString(strXmlContent);

                    if (!cXmlConfig.GetRegKeyString("version", m_strClientType, "", cCuLoginRsp.GetCuVersion()))
                    {
                        TASKLOG(LOGIN_TASK, CRITICAL_LEV, "we failed to get client version, client ip:%s, client type:%s\n",
                            m_strClientIp.c_str(), m_strClientType.c_str());
                    }
                    if (!cXmlConfig.GetRegKeyString("version", m_strClientType+"_update_url", "", strCommonCuUpdateUrl))
                    {
                        TASKLOG(LOGIN_TASK, CRITICAL_LEV, "we failed to get client update uri, client ip:%s, client type:%s\n",
                            m_strClientIp.c_str(), m_strClientType.c_str());
                    }
                } while (false);
            }
            switch(m_tNatInfo.GetIsHasNat())
            {
            case DEVICE_LINEMODE_INTRANET:
                {
                    cCuLoginRsp.SetUpdateUrl(GetCuURL(m_strHostIp, digital_string(::GetHttpPort()), strCommonCuUpdateUrl));
                }
                break;
            case DEVICE_LINEMODE_EXTRANET:
                {
                    string strPort = digital_string(::GetHttpPort());
                    const vector<CCommonNetNatInfo>& NetNatInfoList = GetCommonNetNatInfo();
                    if(NetNatInfoList.size() > 0)
                    {
                        vector<CCommonNetNatInfo>::const_iterator item = NetNatInfoList.begin(); 
                        while(item != NetNatInfoList.end())
                        {
                            if(item->platformIP == m_tNatInfo.GetIpAddr() && item->platformNatIP == m_strHostIp
                                && item->platformPort == ::GetHttpPort())
                            {
                                s8 szPort[NAME_MAX_LEN] = {0};
                                sprintf(szPort, "%d", item->platformNatPort);
                                strPort = szPort;
                                break;
                            }
                            item++;
                        }
                    }
                    TASKLOG(LOGIN_TASK, ERROR_LEV, "���յ�IP[%s] �˿�[%s]\n",
                        m_strHostIp.c_str(), strPort.c_str());
                    cCuLoginRsp.SetUpdateUrl(GetCuURL(m_strHostIp, strPort, strCommonCuUpdateUrl));
                }
                break;
            default:
                break;
            }
            cCuLoginRsp.SetDataVersion(m_strServerDataVersion);
			cCuLoginRsp.SetPasswordRemainTime(m_nPwdExpireRemainTime);
			cCuLoginRsp.SetUserLockRemainTime(m_nPwdLockRemainTime);
			//��redis�ж�ȡ�������ֵ
						// const string FDB_KEY_PMS_GB_SECURITY_ENABLE = "pms_gb_security_enable"; // ֵΪ�ַ���enable��disable��key������ʱ��ʾenable
			bool bSecureEnable = true;
			string strEnable;
			EFDB_ErrorCode er = g_cRedisDb.Get(Redis_GB_Security, strEnable);
			TASKLOG(LOGIN_TASK, ERROR_LEV, "��ȡ�����Ƿ�����ȫģʽ���get(%s) return [%s],err=%d\n", Redis_GB_Security, strEnable.c_str(), er);
			//Ĭ�����ã�����ֻ�л�ȡ�����Ϊ"disable"�Ų����� 2018/10/18 zsy
			if (er == EC_OK && strEnable == "disable")
			{
				bSecureEnable = false;
			}
			cCuLoginRsp.SetSecureEnable(bSecureEnable);
            //��ʱ���޸�
            //cCuLoginRsp.SetUpdateUrl();
            CCuiInst* ptCuiInst = (CCuiInst *)GetInstance();

            CSTUNServerInfo stunInfo;
            stunInfo.SetIp(ptCuiInst->GetStunAddr().GetNetIp());
            stunInfo.SetPort(ptCuiInst->GetStunAddr().GetNetPort());
            cCuLoginRsp.SetStunServerInfo(stunInfo);

            SendRspToCu<CCuiCuLoginRsp>(cCuLoginRsp, m_ptCuLoginNode);
			if (IsMobileClientType())
			{
				ECMS_ERRORCODE eErrCode = g_cCuiRedisSdk.UpdateCuiOnlineMobileNum(g_cCuiConfig.GetLocalURI().GetUser(), GetMobileClientNumber());
				if( eErrCode != CMS_SUCCESS )
				{
					TASKLOG(LOGIN_TASK, EVENT_LEV, "�����ƶ��ͻ���������redis���ݿ�ʧ��\n");
				}
				TASKLOG(LOGIN_TASK, EVENT_LEV, "�����ƶ��ͻ�������[%d -- %d]��redis���ݿ�\n",(GetMobileClientNumber()-1),GetMobileClientNumber());
			}
            
            TASKLOG(LOGIN_TASK, EVENT_LEV, "CU�û�[%s]��¼�ɹ���TAS�ỰIDΪ[%s],CMU�ỰIDΪ[%s]\n",
                m_strUserName.c_str(),m_strCuSessionIn3AS.c_str(),m_strSession.c_str());

            //�޳� session �е���id
            TSipURI sessionUri( m_strSession );
            CStreamDecentrationAdapter::RegisterCu( g_cCuiConfig.GetLocalURI().GetUser(), sessionUri.GetUser(),
				m_strUserName, m_strClientIp, m_tNatInfo.GetIpAddr() );

            NextState(ONLINE_STATE);

            //����task�����û���Ϣ��ȡ(�����豸Ȩ����Ϣ)
            CGetUserInfo* pcGetUserInfo = new CGetUserInfo(GetInstance(), m_str3asName, m_strUserName, m_strSession);
            pcGetUserInfo->InitStateMachine();
            pcGetUserInfo->StartQryUserInfo();

            //WriteLog(cRsp.GetErrorCode(), true);
        }
        break;
    case OSP_SIP_MSG_PROC_FAIL:
        {
            TASKLOG(LOGIN_TASK, ERROR_LEV, "��CMU����[ CU��¼���� ]SIP��Ϣʧ�ܣ�SIP������[%d]���û�[%s]��¼ʧ��\n",
                pcOspSipMsg->GetSipErrorCode(), m_strUserName.c_str());
            //CMU���CUʧ�ܣ��ܾ�CU��¼
            CCuiCuLoginRsp cTmpRsp;
            cTmpRsp.SetErrorCode(ERR_CUI_SIP_MSG_FAIL);
            SendRspToCu<CCuiCuLoginRsp>(cTmpRsp, m_ptCuLoginNode);
            WriteLog(ERR_CUI_SIP_MSG_FAIL, true);
            delete this;
        }
        break;
    default:
        {
            TASKLOG(LOGIN_TASK, CRITICAL_LEV, "�յ�δ֪��Ϣ[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }
    return PROCMSG_OK;
}

void CCuAgent::GetOptLogDesc(bool bLogin)
{
    if(bLogin)
    {
        if(m_strSession.empty())
        {
            m_strLogDesc = GetLanDesc(LAN_DESC_LOGIN) + GetLanDesc(LAN_DESC_FAIL);
        }
        else
        {
            m_strLogDesc = GetLanDesc(LAN_DESC_LOGIN) + GetLanDesc(LAN_DESC_SUCCESS);
        }
    }
    else
    {
        m_strLogDesc = GetLanDesc(LAN_DESC_LOGOUT) + GetLanDesc(LAN_DESC_SUCCESS);
    }
}

void CCuAgent::WriteLog( int dwErrorCode, bool bLogin, bool bEnableDefaultDesc ) 
{
    if( bEnableDefaultDesc )
    {
        GetOptLogDesc(bLogin);
    }
    CUsrOprLogTask* pcTask = new CUsrOprLogTask(GetInstance(), m_strSession, 
        string(DEV_TYPE_UAS) + "@" + m_tCmuURI.GetDomain(),
        enUserOpTypeLogin,
        (u32)dwErrorCode,
        m_strLogDesc,
        string()
        );

    u32 dwTaskNo;
    if (!g_cCuiDataMgr.m_mapSessToTaskMap.Find(m_strSession, dwTaskNo))
    {
        pcTask->m_tLogInfo.userName = m_strUserName;
        pcTask->m_tLogInfo.netAddr = m_strClientIp;

        if (NULL != m_pcUserAccount)
        {
            pcTask->m_tLogInfo.userUri = m_pcUserAccount->m_tUserID + "@" + GetDomainNameWithoutPreNameFromURI(m_str3asName);
            pcTask->m_tLogInfo.userName = m_pcUserAccount->m_strUserName;
        }
    }

    pcTask->InitStateMachine();
    pcTask->StartSendLog();
}
void CCuAgent::WriteDeviceModLog(int dwErrorCode, bool bDevGroup)
{
	string rsltId = LAN_DESC_SUCCESS;
	if (dwErrorCode != 0)
	{
		rsltId = LAN_DESC_FAIL;
	}
	string descId = LAN_DESC_DEVPRIVILEGE;
	if (bDevGroup)
	{
		descId = LAN_DESC_DEVGRPPRIVILEGE;
	}
	
	string strDevLogDesc = GetLanDesc(LAN_DESC_MOD) //"�޸�"
		+ GetLanDesc(descId) + GetLanDesc(rsltId);

	CUsrOprLogTask* pcTask = new CUsrOprLogTask(GetInstance(), m_strSession,
												string(DEV_TYPE_UAS) + "@" + m_tCmuURI.GetDomain(),
												enUserOpTypeOther,
												(u32)dwErrorCode,
												strDevLogDesc,
												string()
												);

	u32 dwTaskNo;
	if (!g_cCuiDataMgr.m_mapSessToTaskMap.Find(m_strSession, dwTaskNo))
	{
		pcTask->m_tLogInfo.userName = m_strUserName;
		pcTask->m_tLogInfo.netAddr = m_strClientIp;

		if (NULL != m_pcUserAccount)
		{
			pcTask->m_tLogInfo.userUri = m_pcUserAccount->m_tUserID + "@" + GetDomainNameWithoutPreNameFromURI(m_str3asName);
			pcTask->m_tLogInfo.userName = m_pcUserAccount->m_strUserName;
		}
	}

	pcTask->InitStateMachine();
	pcTask->StartSendLog();
}
void CCuAgent::InitStateMachine()
{
    CStateProc cOfflineProc;
    cOfflineProc.ProcMsg = (CSipTask::PFProcMsg)&CCuAgent::OfflineProcMsg;
    cOfflineProc.TimerPoll = &CSipTask::TimerPollSkip;
    cOfflineProc.ProcErrMsg = (CSipTask::PFProcMsg)&CCuAgent::OfflineProcMsg;
    AddRuleProc(OFFLINE_STATE, cOfflineProc);

    CStateProc cWaitReg3ASProc;
    cWaitReg3ASProc.ProcMsg = (CSipTask::PFProcMsg)&CCuAgent::Wait3ASRegRspProcMsg;
    cWaitReg3ASProc.TimerPoll = (CSipTask::PFTimerPoll)&CCuAgent::Wait3ASRspTimerPoll;
    cWaitReg3ASProc.ProcErrMsg = (CSipTask::PFProcMsg)&CCuAgent::Wait3ASRegRspProcMsg;
    AddRuleProc(WAIT_3AS_REG_RSP_STATE, cWaitReg3ASProc);

    CStateProc cWaitQry3ASProc;
    cWaitQry3ASProc.ProcMsg = (CSipTask::PFProcMsg)&CCuAgent::Wait3ASAddCuRspProcMsg;
    cWaitQry3ASProc.TimerPoll = (CSipTask::PFTimerPoll)&CCuAgent::Wait3ASRspTimerPoll;
    cWaitQry3ASProc.ProcErrMsg = (CSipTask::PFProcMsg)&CCuAgent::Wait3ASAddCuRspProcMsg;
    AddRuleProc(WAIT_3AS_ADD_CU_RSP_STATE, cWaitQry3ASProc);

    CStateProc cWaitCmuAddCuProc;
    cWaitCmuAddCuProc.ProcMsg = (CSipTask::PFProcMsg)&CCuAgent::WaitCmuAddCuRspProcMsg;
    cWaitCmuAddCuProc.TimerPoll = (CSipTask::PFTimerPoll)&CCuAgent::Wait3ASRspTimerPoll;
    cWaitCmuAddCuProc.ProcErrMsg = (CSipTask::PFProcMsg)&CCuAgent::WaitCmuAddCuRspProcMsg;
    AddRuleProc(WAIT_CMU_ADD_CU_RSP_STATE, cWaitCmuAddCuProc);

    CStateProc cOnlineProc;
    cOnlineProc.ProcMsg = (CSipTask::PFProcMsg)&CCuAgent::OnlineProcMsg;
    cOnlineProc.TimerPoll = (CSipTask::PFTimerPoll)&CCuAgent::OnlineTimerPoll;
    cOnlineProc.ProcErrMsg = (CSipTask::PFProcMsg)&CCuAgent::OnlineProcMsg;
    AddRuleProc(ONLINE_STATE, cOnlineProc);

    NextState(OFFLINE_STATE);
}

//�ṩ�Ķ��ⷽ��
u32 CCuAgent::StartLogin(const string &strUserName, const string &strPasswd, const string &strVersion)
{
    CMS_ASSERT(string::npos != strUserName.find('@'));
    m_strUserName = strUserName;
  //  std::transform(m_strUserName.begin(), m_strUserName.end(), m_strUserName.begin(), ::tolower);
	//MD5(strPasswd, m_strPassWord);//���ڼ����˰�ȫ SM3_Base64 �㷨����Ҫ�����ϵ��������룬����cui�ﶼ͸����fcgi�Ǳ߻�����µĽӿ��ж���͸��������md5���� 2018/10/24 zsy
	m_strPassWord = strPasswd;
    m_strVersion = strVersion;
    TSipURI tUserURI;
    tUserURI.SetNOString(m_strUserName);
    m_str3asName = tUserURI.Get3ASURI().GetURIString();
    TASKLOG(LOGIN_TASK, EVENT_LEV, "�յ�CU�û�[%s]�ĵ�¼����CUI����CuAgent��TaskNOΪ[%u]\n", 
        strUserName.c_str(), GetTaskNO());

    //1 �����ж�CUI�Ƿ���3ASע���
    C3acAgent *pc3acAgentTask = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(m_str3asName, GetInstance());
    if ( NULL != pc3acAgentTask )
    {
        //�п���3AS����
        if ( pc3acAgentTask->GetState() == C3acAgent::RELOGIN_STATE )
        {
            CCuiCuLoginRsp cTmpRsp;
            cTmpRsp.SetErrorCode(ERR_CUI_TAS_OFFLINE);
            SendRspToCu<CCuiCuLoginRsp>(cTmpRsp, m_ptCuLoginNode);

            WriteLog(ERR_CUI_TAS_OFFLINE, true);
            delete this;
            return PROCMSG_DEL;
        }

        CMS_ASSERT( 0 == strcmp(pc3acAgentTask->GetObjName(), "C3acAgent") );
        TASKLOG(LOGIN_TASK, EVENT_LEV, "TAS[%s]�Ѿ����������ӣ�ֱ�����CU����״̬\n",
            m_str3asName.c_str());
        //��3AS���CU���û�
        CCuOnlineReq cAddCuReq;
        FillOnlineReq( cAddCuReq, pc3acAgentTask->m_strSessionID );

        TSipURI t3asURI;
        t3asURI.SetNOString(m_str3asName);
        FakePostMsgReq(cAddCuReq, t3asURI);
        pc3acAgentTask->FlashTimePoll();

        NextState(WAIT_3AS_ADD_CU_RSP_STATE);
    }
    else
    { //��3ASע��
        TASKLOG(LOGIN_TASK, EVENT_LEV, "TAS[%s]��δ�������ӣ���CUI����TASע��\n",
            m_str3asName.c_str());
        CLoginTasReq cLogin3ASReq;
        cLogin3ASReq.SetUserName(OspSipGetLocalURI().GetURIString());
        cLogin3ASReq.SetLoginType(CLoginTasReq::enLoginType_CUI);

		//������Ҫ��ȡ��3as����Դ���ͼ���
		TPSLoginExternInfo tResourceSet;
		tResourceSet.resourceTypes.insert(TasResourceGeneralDevAdd);
		tResourceSet.resourceTypes.insert(TasResourceGeneralDevDel);
		cLogin3ASReq.SetExtraInfo(tResourceSet.ToXml());

        TSipURI t3asURI;
        t3asURI.SetNOString(m_str3asName);
        FakePostMsgReq(cLogin3ASReq, t3asURI);

        NextState(WAIT_3AS_REG_RSP_STATE);
    }

    return PROCMSG_OK;
}

void CCuAgent::SetCMUURI(const TSipURI &tURI)
{
    m_tCmuURI = tURI;
}

u32 CCuAgent::OnlineProcMsg(CMessage *const pcMsg)
{
//    COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
    TASKLOG(LOGIN_TASK, EVENT_LEV, "CU�û�[%s]�յ���������Ϣ[%s][%hu]\n",
        m_strUserName.c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

    switch ( pcMsg->event )
    {
    case OSP_SIP_MSG_PROC_FAIL:
        {
            TASKLOG(LOGIN_TASK, CRITICAL_LEV, "CCuAgent[TaskNO %u]�յ���Ϣ[%s][%hu]��δ��������Ϣ\n",
                GetTaskNO(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
	case UPDATE_CULOGIN_INFO_RSP:
		{
			TASKLOG(LOGIN_TASK, CRITICAL_LEV, "CCuAgent[TaskNO %u]�յ���ϢӦ��[%s][%hu]\n",
				GetTaskNO(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
		}
		break;
    default:
        {
            TASKLOG(LOGIN_TASK, CRITICAL_LEV, "CCuAgent[TaskNO %u]�յ�δ֪����Ϣ[%s][%hu]\n\n",
                GetTaskNO(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"[CCuAgent::OnlineProcMsg]δ֪֪ͨ��Ϣ����");
        }
        break;
    }

    return PROCMSG_OK;
}

u32 CCuAgent::OnlineTimerPoll()
{
    TryReplyNotifyRsp();

    time_t tHoldTime = GetCurStateHoldTime();
    if ( tHoldTime > g_cCuiConfig.GetCuHbTimeout() )
    { //������ʱ
        WriteLog(ERR_CUI_OPR_TIME_OUT, false);
        TASKLOG(LOGIN_TASK, ERROR_LEV, "CU�û�[%s]�Ự[%s]������ʱ[%u > %u]��ǿ���˳���¼\n",
            m_strUserName.c_str(), m_strSession.c_str(), tHoldTime, g_cCuiConfig.GetCuHbTimeout());
        //����CuAgent���Զ���CMU����Cu Logout Req
        return TIMERPOLL_DEL;
    }

    return TIMERPOLL_DONE;
}

u32 CCuAgent::CuHBAndGetNotify( PTRecvNode ptNode, s32 nTimeoutSeconds )
{
    TASKLOG(LOGIN_TASK, TIMER_LEV, "�յ�CU�û�[%s]�Ự[%s]��������Ϣ\n",
        m_strUserName.c_str(), m_strSession.c_str());
    CMS_ASSERT(ptNode != NULL);
    CMS_ASSERT(GetState() == ONLINE_STATE);

    ClearCuHBCount();

    if (m_ptNotifyRecvNode != NULL)
    {
        TASKLOG(LOGIN_TASK, TIMER_LEV, "Get Notify too fast, cuip=%s, taskno:%d\n", m_strClientIp.c_str(), GetTaskNO());
        ReplyNotifyRsp(m_ptNotifyRecvNode);
    }
    m_ptNotifyRecvNode = ptNode;
    m_nNotifyRecvNodeTimeout = nTimeoutSeconds;

    if (m_cNotifyPool.IsEmpty())
    {
        if (m_nNotifyRecvNodeTimeout > 0)
        {
            // �ȴ���ʱ
        }
        else
        {
            ReplyNotifyRsp(m_ptNotifyRecvNode);
        }
    }
    else
    {
        ReplyNotifyRsp(m_ptNotifyRecvNode);
    }

    return PROCMSG_OK;
}

CSipTask * CCuAgent::GetSubTaskPtrByDevUri(const string& strDev)
{
    u32 dwTaskID = INVALID_TASKNO;
    if ( ! m_cDevSubTaskSet.Find(strDev, dwTaskID) )
    {
        return NULL;
    }

    if ( INVALID_TASKNO == dwTaskID )
    {
        return NULL;
    }
    return (CSipTask *)(GetInstance()->FindTask(dwTaskID));
}

CSipTask * CCuAgent::GetInviteTaskPtrByPlayId( const int nPlayId )
{
    u32 dwTaskID = INVALID_TASKNO;
    if ( ! m_cInviteTaskSet.Find(nPlayId, dwTaskID) )
    {
        return NULL;
    }

    if ( INVALID_TASKNO == dwTaskID )
    {
        return NULL;
    }
    return (CSipTask *)(GetInstance()->FindTask(dwTaskID));
}

CExportTask *CCuAgent::GetExportTaskPtrByFileUrl(const string& strFile)
{
    u32 dwTaskID = INVALID_TASKNO;
    if ( ! m_cFileExportTaskSet.Find(strFile, dwTaskID) )
    {
        return NULL;
    }
    if ( INVALID_TASKNO == dwTaskID )
    {
        return NULL;
    }
    
    return dynamic_cast<CExportTask *>(GetInstance()->FindTask(dwTaskID));
}

void CCuAgent::ClearDevPermanentTaskRef( const string &strDevId )
{
    // ���subscribe, invite, callpu  hzytodo  ��ʱ���豸ɾ���ĵط�����ÿ��cuagent�Ĵ˷���
}

CPeInviteTask * CCuAgent::GetPeInviteTaskPtrByPlayId( const int nPlayId )
{
    u32 dwTaskID = INVALID_TASKNO;
    if ( ! m_cPeInviteTaskSet.Find(nPlayId, dwTaskID) )
    {
        return NULL;
    }

    if ( INVALID_TASKNO == dwTaskID )
    {
        return NULL;
    }
    return (CPeInviteTask *)(GetInstance()->FindTask(dwTaskID));
}

// CTvsSubTask * CCuAgent::GetTvsSubTaskPtrByDevURI(const string &strDevURI)
// {
//     u32 dwTaskID = INVALID_TASKNO;
//     if ( ! m_cTvsSubTaskSet.Find(strDevURI, dwTaskID) )
//     {
//         return NULL;
//     }
// 
//     if ( INVALID_TASKNO == dwTaskID )
//     {
//         return NULL;
//     }
//     return (CTvsSubTask *)(GetInstance()->FindTask(dwTaskID));
// }
// 
// CRcsSubTask * CCuAgent::GetRcsSubTaskPtrByDevURI(const string &strDevURI)
// {
//     u32 dwTaskID = INVALID_TASKNO;
//     if ( ! m_cRcsSubTaskSet.Find(strDevURI, dwTaskID) )
//     {
//         return NULL;
//     }
// 
//     if ( INVALID_TASKNO == dwTaskID )
//     {
//         return NULL;
//     }
//     return (CRcsSubTask *)(GetInstance()->FindTask(dwTaskID));
// }

 void CCuAgent::AddSubTask( CSipTask *pcSubTask )
 {
     CMS_ASSERT(pcSubTask != NULL);
     m_cSubTaskSet.Insert(pcSubTask->GetTaskNO(), pcSubTask);
 }
 
 void CCuAgent::DelSubTask( const CSipTask *pcSubTask )
 {
     CMS_ASSERT(pcSubTask != NULL);
     m_cSubTaskSet.Erase(pcSubTask->GetTaskNO());
 }

 void CCuAgent::AddRecTvTask(const TTvChannel& tTvChn,CTask *pcTask)
 {
	 CMS_ASSERT(pcTask != NULL);
	 m_tRecordTaskSets.Insert(tTvChn,pcTask);
 }
 void CCuAgent::DelRecTvTask(const TTvChannel& tTvChn)
 {
	 m_tRecordTaskSets.Erase(tTvChn);
 }
 CTask* CCuAgent::GetRecTvTask(const TTvChannel& tTvChn)
 {
	 if (NULL != m_tRecordTaskSets.Find(tTvChn))
	 {
		 return *(m_tRecordTaskSets.Find(tTvChn));
	 }
	 return NULL;
 }

bool CCuAgent::IsTasOnline()
{
    return (! m_strCuSessionIn3AS.empty());
}

bool CCuAgent::IsExistDomainInfo(const string& strDomainName)
{
    bool bIsExist = false;
    C3acAgent *pc3acAgentTask = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(m_str3asName, GetInstance());
    if (NULL != pc3acAgentTask)
    { 
       bIsExist = pc3acAgentTask->IsExistDomainInfo(strDomainName);
    }
    return bIsExist;
}

int CCuAgent::CalcEthNo(const TIpAddr& strIp)
{
    u32 adwLocalIPList[20] = {0};
    // ��ȡIP�б�
    u16 wIpNum = OspAddrListGet( adwLocalIPList, 20);
    u32 dwIp = inet_addr(strIp.c_str());
    int i = 0;

    bool IsHas = false;
    for(; i< wIpNum; i++)
    {
        if(adwLocalIPList[i] == dwIp)
        {
            IsHas = true;
            break;
        }
    }

    if(IsHas)
    {
        return i;
    }

    return -1;
}

int CCuAgent::IsHasNat(const TIpAddr& tServiceIp, const TIpAddr& tHostIp)
{
    if(tServiceIp == tHostIp)
    {
        return DEVICE_LINEMODE_INTRANET;
    }
    return DEVICE_LINEMODE_EXTRANET;
}


string CCuAgent::GetHostIp( const string& strHost )
{
	string strHostIp=strHost;
	if (strHost.find(":")!=string::npos)
	{
		strHostIp=strHost.substr(0,strHost.find(":"));
	}

	return strHostIp;
}

void CCuAgent::ClearCuHBCount()
{
    NextState(ONLINE_STATE);    //����ǰ״̬����
}

bool CCuAgent::IsHasDevPrivilege( const string &strDevUri, s32 nPrivilege )
{
	if (m_pcUserAccount->IsSuperAdmin())
	{
		return true;
	}

	CEncoderPrivilege cPri;
	EFDB_ErrorCode er = g_cRedisDb.GetUserEncoderPrivilege(m_pcUserAccount->m_tUserID, GetNameWithoutDomainFromURI(strDevUri), cPri);
	if ( !IsFdbResultPositive(er) )
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "GetUserEncoderPrivilege failed er[%d]\n", er);
		return false;
	}

	return cPri.HasPrivilege((CEncoderPrivilege::TPrivilege)nPrivilege);
}

void CCuAgent::PrintData() const
{
   // CTask::PrintData();
    OspPrintf(TRUE, FALSE, "\tUserName[%s]", m_strUserName.c_str());
	OspPrintf(TRUE, FALSE, "\tClientVersion[%d]\n", m_dwClientVer);
    OspPrintf(TRUE, FALSE, "\tClientIp && tClientLocalIp && tClientNat[%s-%s-%s]\n", m_strClientIp.c_str(), m_strClientLocalIp.c_str(),NATMODEDSC[m_eClientNat].c_str());
	OspPrintf(TRUE, FALSE, "\tPlatMapIp && PlatLocalIp && PlatNat[%s-%s-%s]\n", m_strHostIp.c_str(),m_tNatInfo.GetIpAddr().c_str(),NATMODEDSC[m_ePlatNat].c_str());
	OspPrintf(TRUE, FALSE, "\tSupportCuiTransPlatRec:%d\n", m_bClientSupCuiTransPlatRec);
	OspPrintf(TRUE, FALSE, "\tCuiTransStream4PlatRec:%d\n", CuiTransStream4PlatRec());
	OspPrintf(TRUE, FALSE, "\tCuiTransStream4Pu:%d\n", CuiTransStream4Pu());
	if (m_pcUserAccount != NULL)
	{
		OspPrintf(TRUE, FALSE, "\tUserId:%s\n", m_pcUserAccount->m_tUserID.c_str());
	}
    OspPrintf(TRUE, FALSE, "\tUser Session4cmu:%s", m_strSession.c_str());
    OspPrintf(TRUE, FALSE, "\tUser Session4Tas:%s\n", m_strCuSessionIn3AS.c_str());
    OspPrintf(TRUE, FALSE, "\tSupportNA setting value:%d", (s32)m_bIsSupportNA);
    OspPrintf(TRUE, FALSE, "\tClientType:%s", m_strClientType.c_str());
    OspPrintf(TRUE, FALSE, "\tNeedPlatformServiceStatue:%d\n", (s32)m_bNeedPlatformServiceStatue);
    OspPrintf(TRUE, FALSE, "\tNotifyPool IsEmpty:%d, RecvNode IsNull:%d, Timeout:%d\n", 
        (s32)m_cNotifyPool.IsEmpty(), (s32)(m_ptNotifyRecvNode==NULL), m_nNotifyRecvNodeTimeout);

}

void CCuAgent::ReplyNotifyRsp( PTRecvNode &ptNode )
{
    CGetNotifyRsp cRsp;
    cRsp.SetSession(m_strSession);
    cRsp.SetErrorCode(CMS_SUCCESS);

    //����Ϣ���е�Ntf����������Ϣ��
    m_cNotifyPool.PopNotifyPool(cRsp.notifyPool, 20);
    TASKLOG(LOGIN_TASK, TIMER_LEV, "���η��͸�CU[%s]��[%d]��֪ͨ��Ϣ��ʣ��֪ͨ����[%d]\n",
        m_strSession.c_str(), cRsp.notifyPool.GetCuNtfSize(), m_cNotifyPool.GetCuNtfSize());

    SendRspToCu<CGetNotifyRsp>(cRsp, ptNode);
    ptNode = NULL;
}

void CCuAgent::TryReplyNotifyRsp()
{
    if (m_ptNotifyRecvNode != NULL)
    {
        if (--m_nNotifyRecvNodeTimeout <= 0 || !m_cNotifyPool.IsEmpty())
        {
            ReplyNotifyRsp(m_ptNotifyRecvNode);
        }
    }
}

bool CCuAgent::IsMobileClientType() const
{
    return ::IsMobileClientType(m_strClientType);
}

void CCuAgent::OnUpdateCuProperty()
{
	CCuUpdateCUPropertyRsp cCuUpdatePropertyRsp;
	cCuUpdatePropertyRsp.SetCuiSupTransPlatRec(true);

	m_strClientIp!=m_strClientLocalIp?m_eClientNat = NATModeInner:m_eClientNat = NATModeOutside;
	cCuUpdatePropertyRsp.SetClientNat(m_eClientNat);

	// ����ͻ����Լ���д��ƽ̨NAT����ƽ̨�����ж�NAT [1/26/2015 pangubing]
	if (NATModeAuto==m_ePlatNat)
	{
		m_tNatInfo.GetIsHasNat()?m_ePlatNat = NATModeInner:m_ePlatNat = NATModeOutside;
	}
	cCuUpdatePropertyRsp.SetPlatNat(m_ePlatNat);

	TNetAddr cCuPunchAddr;
	if (NATModeInner==m_ePlatNat)
	{
		m_dwClientVer = CU_VER_WITH_NATOPT;
		if (!g_cCuiConfig.GetMapedNatIP().empty())
		{
			 //cCuPunchAddr.SetNetIp(g_cCuiConfig.GetMapedNatIP());
			 //�����ж���NATʱ��ƽ̨����ͬʱ��һ���˿ڣ�8001��ӳ�䵽��һ��͵ڶ���
			 //����ֱ��ʹ��cu�ĵ�½ip����Ϊƽ̨�ڵ������ӳ��IP
			 cCuPunchAddr.SetNetIp(m_strHostIp);
			 cCuPunchAddr.SetNetPort(g_cCuiConfig.GetMapedNatPort());
		}
	}
	else
	{
		m_dwClientVer = CU_VER_WITH_NATOPT;
		cCuPunchAddr.SetNetIp(m_tNatInfo.GetIpAddr());
		cCuPunchAddr.SetNetPort(g_cCuiConfig.GetNatPort());
	}
	cCuUpdatePropertyRsp.SetCuPunchAddr(cCuPunchAddr);

	if (CU_VER_WITH_NATOPT==m_dwClientVer)
	{
		CUpdateCuLoginInfoReq cReq;
		cReq.SetSession(m_strSession);
		//��CMU�޸�CU��¼��Ϣ
		cReq.SetUserName(m_strUserName);
		cReq.SetVersion(m_strVersion);
		cReq.SetUsedCuiIp(m_tNatInfo.GetIpAddr());
		cReq.SetIsSupportNA(m_bIsSupportNA);

		FakePostMsgReq(cReq, m_tCmuURI);
	}

	TASKLOG(LOGIN_TASK, EVENT_LEV, "CU�û�[%s]�������Գɹ���CU NAT[%s-%s-%d],ƽ̨NAT[%s-%s-%d]\n",m_strUserName.c_str(),
			m_strClientLocalIp.c_str(),m_strClientIp.c_str(),m_eClientNat,m_tNatInfo.GetIpAddr().c_str(),m_strHostIp.c_str(),m_ePlatNat);
	
	 SendRspToCu<CCuUpdateCUPropertyRsp>(cCuUpdatePropertyRsp, m_ptCuLoginNode);
}

void CCuAgent::FillOnlineReq( CCuOnlineReq &cAddCuReq, const string &strSession )
{
    cAddCuReq.SetSession(strSession);
    cAddCuReq.SetCuUserName(GetNameWithoutDomainFromURI(m_strUserName));

    cAddCuReq.SetCuUserPassword(m_strPassWord);
    cAddCuReq.SetClientType(m_strClientType);
    if(m_strWebCuClientIp.length()>0)
    {
        cAddCuReq.SetLoginIp(m_strWebCuClientIp);
    }
    else
    {
        cAddCuReq.SetLoginIp(m_strClientIp);
    }
	cAddCuReq.SetLoginSession(m_strCuSessionIn3AS);
}

//////////////////////////////////////////////////////////////////////////
//C3acAgent

C3acAgent::C3acAgent(CInstExt *pcInst) : CSipTask(pcInst)
{
	m_bGetMapDomainInfoFinish = false;
}

C3acAgent::~C3acAgent()
{
    TerminateTask();
}

void C3acAgent::ClearDataWhenTASOfflineOrDisconnect()
{
    //4���ӻỰӳ�����ɾ��������
    // 2012.3.29 �޸��߼�����ɾ��TAS��URI�Ǽǣ���ʹTAS�����ߣ�Ҳ���Բ�ѯ�õ�
    // TAS��������Ҫͨ������;���ж�
    // ֻ�е�����CU�������ˣ���������TAS
    // g_cCuiDataMgr.m_map3asURITo3asAgentMap.Erase(m_t3asURI.GetURIString());
    g_cCuiDataMgr.m_mapSessToTaskMap.Erase(m_strSessionID);
    m_strSessionID.clear();

    //5���ӻỰӳ�����ɾ�����е�QueryTag
    vector<string>::iterator pItem = m_vctTmpQueryTag.begin();
    while ( pItem != m_vctTmpQueryTag.end() )
    {
        g_cCuiDataMgr.m_mapSessToTaskMap.Erase(*pItem);
        pItem++;
    }
    m_vctTmpQueryTag.clear();

    //6��������е�CuAgent��TAS Session
    vector<CTask*> vctCuAgents = GetInstance()->GetTasksArray("CCuAgent");
    if ( ! vctCuAgents.empty() )
    {
        vector<CTask*>::iterator pcCuAgentItem = vctCuAgents.begin();
        while ( pcCuAgentItem != vctCuAgents.end() )
        {
            CMS_ASSERT( 0 == strcmp((*pcCuAgentItem)->GetObjName(), "CCuAgent" ) );
            CCuAgent *pcCuAgentTask = (CCuAgent *)(*pcCuAgentItem);

            if ( pcCuAgentTask->m_str3asName == m_t3asURI.GetURIString() )
            {
                g_cCuiDataMgr.m_mapSessToTaskMap.Erase(pcCuAgentTask->m_strCuSessionIn3AS);
                pcCuAgentTask->m_strCuSessionIn3AS.clear();
                //pcCuAgentTask->m_pcUserAccount = NULL;  //TAS������CuAgent��Ȼ��Ҫ
            }

            pcCuAgentItem++;
        }
    }


}

void C3acAgent::TerminateTask()
{
    TASKLOG(LOGIN_TASK, EVENT_LEV, "�յ��ǳ�TAS[%s]����\n",
        m_t3asURI.GetURIString().c_str());
    OspPrintf(TRUE, FALSE, "!!!!!!!!!!!!!!!�˳�[%s]��\n",
        m_t3asURI.GetURIString().c_str());

    if ( GetState() == ONLINE_STATE )
    {
        //1, �ǳ�3as
        CLogoutTasReq cReq;
        cReq.SetSession(m_strSessionID);

        FakePostMsgReq(cReq, m_t3asURI);
        //2��ȡ���������
        FakeStopHB(m_t3asURI);
    }

    //3, ��ճ�Ա
    //3AS�������Ѿ���¼��CU����Ӱ��, ���Բ�ǿ��3AS�µ�CU�˳�
    ClearDataWhenTASOfflineOrDisconnect();

    //ֻ�е�TAS����ʱ����ɾ��URI�Ǽ�
    g_cCuiDataMgr.m_map3asURITo3asAgentMap.Erase(m_t3asURI.GetURIString());

    NextState(OFFLINE_STATE);
}

//״̬����
u32 C3acAgent::OfflineProcMsg(CMessage *const pcMsg)
{
    COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;

    switch (pcMsg->event)
    {
    case TAS_LOGIN_RSP:
        {
            CLoginTasRsp cRsp;
            pcOspSipMsg->GetMsgBody(cRsp);

            m_strSessionID = cRsp.GetSession();

            //����3ASAgent�Ǽǵ�ȫ���б���, ע�⣺�����¼����TAC��SessionID��������OnService()�в��ܸ���SessionID����CuAgent
            g_cCuiDataMgr.m_mapSessToTaskMap.Insert(cRsp.GetSession(), GetTaskNO());

            TASKLOG(LOGIN_TASK, EVENT_LEV, "�յ�TAS�ĵ�¼Ӧ��, ��¼TAS[%s]�ĻỰΪ[%s], ����[Online]״̬\n",
                m_t3asURI.GetURIString().c_str(), m_strSessionID.c_str());

            FakeSetHBParam(m_t3asURI);
            NextState(ONLINE_STATE);

            //��3AS��ѯ���������ID������
            CDomainNameGetReq cDomainReq;
            cDomainReq.SetSession(m_strSessionID);
            FakePostMsgReq(cDomainReq, m_t3asURI);

			//��3AS��ѯ��PTZ������ƵԴ
			m_cPtzlockInfoList.Clear();
			CPtzLockQryReq cPtzLockQryReq;
			cPtzLockQryReq.SetSession(m_strSessionID);
			FakePostMsgReq(cPtzLockQryReq, m_t3asURI);
        }
        break;
    case OSP_SIP_MSG_PROC_FAIL:
        {
            //��CuAgent�ﴦ�����
        }
        break;
    default:
        TASKLOG(LOGIN_TASK, CRITICAL_LEV, "�յ�δ֪��Ϣ[%s--%hu]\n",
            OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        break;
    }
    return PROCMSG_OK;
}
u32 C3acAgent::OnlineProcMsg(CMessage *const pcMsg)
{
    COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;

    switch (pcMsg->event)
    {
    case OSP_SIP_DISCONNECT:
        {
            TASKLOG(LOGIN_TASK, EVENT_LEV, "TAS[%s]����, ���ٱ�Task[%u]��¼����\n",
                m_t3asURI.GetURIString().c_str(), GetTaskNO());
            NextState(OFFLINE_STATE);

            //3����������
            delete this;
        }
        break;
    case OSP_SIP_MSG_PROC_FAIL:
        {
            TASKLOG(LOGIN_TASK, ERROR_LEV, "���͸�TAS[%s]��SIP��Ϣʧ��, SIP������Ϊ[%d]\n",
                m_t3asURI.GetURIString().c_str(), pcOspSipMsg->GetSipErrorCode());
        }
        break;
    case TAS_DOMAIN_NAME_GET_RSP:
        {
            CDomainNameGetRsp cDomainRsp;
            pcOspSipMsg->GetMsgBody(cDomainRsp);
            m_str3ASDomainName = cDomainRsp.GetDomainName();
            m_t3ASDomainID = cDomainRsp.GetDomainId();

            TASKLOG(LOGIN_TASK, EVENT_LEV, "�յ�TAS[%s]�Ļ�ȡ����Ӧ�𣬱�������[%s]��ID[%s]\n",
                m_t3asURI.GetURIString().c_str(), m_str3ASDomainName.c_str(), m_t3ASDomainID.c_str());

			GetMapDomainInfo();		
        }
        break;
	case TAS_PTZLOCK_QRY_RSP:
		{
			CPtzLockQryRsp cPtzLockQryRsp;

			pcOspSipMsg->GetMsgBody(cPtzLockQryRsp);

			if ( cPtzLockQryRsp.GetTotalEntryNum() <= 0)
			{
				TASKLOG(LOGIN_TASK, EVENT_LEV, "û����ƵԴ����PTZ��!\n");
			}
			else
			{
				TASKLOG(LOGIN_TASK, EVENT_LEV, "PTZ�����б��ѯ��QueryTagΪ[%s],��ѯ��������Ϊ%d, ����Task[%s--%u]�Ǽǵ��Ự����\n",
					cPtzLockQryRsp.GetQueryTag().c_str(), cPtzLockQryRsp.GetTotalEntryNum(), GetObjName(), GetTaskNO());
				m_vctTmpQueryTag.push_back(cPtzLockQryRsp.GetQueryTag());
				g_cCuiDataMgr.m_mapSessToTaskMap.Insert(cPtzLockQryRsp.GetQueryTag(), GetTaskNO());
			}
		}
		break;
	case TAS_PTZLOCK_QRY_NTF_REQ:
		{
			CPtzLockQryNtfReq cNtfReq;
			pcOspSipMsg->GetMsgBody(cNtfReq);
			
			//�Ȼظ�NTF֪ͨ
			CNtfRsp cRsp;
			cRsp.SetEvent(pcMsg->event + 1);
			cRsp.SetSeqNum(cNtfReq.GetSeqNum());
			cRsp.SetSession(cNtfReq.GetSession());
			PostMsgRsp(pcOspSipMsg->GetSipTransID(), cRsp);
			//�ж��Ƿ������һ��Ntf, �������ɾ��m_mapSessToTaskMap�е�ӳ��
			if (cNtfReq.IsLastNtf())
			{
				g_cCuiDataMgr.m_mapSessToTaskMap.Erase(cNtfReq.GetQueryTag());
				vector<string>::iterator pItem = m_vctTmpQueryTag.begin();
				while (pItem != m_vctTmpQueryTag.end())
				{
					if (*pItem == cNtfReq.GetQueryTag())
					{
						m_vctTmpQueryTag.erase(pItem);
						break;
					}
					pItem++;
				}
			}

			CCuPtzLockInfo cuPtzLockInfo;
			for (vector <CPtzLockInfo>::iterator iter = cNtfReq.GetInfos().begin();
					iter != cNtfReq.GetInfos().end(); iter++)
			{
				cuPtzLockInfo.Clear();
				cuPtzLockInfo.SetDeviceId(iter->GetDeviceId());
				cuPtzLockInfo.SetVideoSrcId(iter->GetVideoSrcId());
				cuPtzLockInfo.SetUserId(iter->GetUserId());
				//cuPtzLockInfo.SetUserName();
				//cuPtzLockInfo.SetUserPtzLevel();
				cuPtzLockInfo.SetPassword(iter->GetPassword());
				cuPtzLockInfo.SetBeginTime(iter->GetBeginTime());
				cuPtzLockInfo.SetEndTime(iter->GetEndTime());
				
				if (!m_cPtzlockInfoList.AddCuPtzLockInfo(cuPtzLockInfo))
				{
					TASKLOG(LOGIN_TASK, EVENT_LEV, "���PTZ����Ϣʧ�ܣ�\n");
					cuPtzLockInfo.PrintData();
				}
			}
		}
		break;
	case TAS_PTZLOCK_DEL_RSP:
		{
			CCuPtzLockDelRsp cRsp;
			pcOspSipMsg->GetMsgBody(cRsp);

			if ( cRsp.GetErrorCode() != CMS_SUCCESS )
			{
				TASKLOG(LOGIN_TASK, ERROR_LEV, "CUIɾ��������ʧ��,��Ӧ��!CPtzLockDelRsp[%s]\n", cRsp.ToXml().c_str());
			}
		}
		break;
    default:
        {
            TASKLOG(LOGIN_TASK, CRITICAL_LEV, "�յ�δ֪��Ϣ[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }

        break;
    }

    return PROCMSG_OK;
}


u32 C3acAgent::OnlineTimerPoll()
{
	if (GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout())
	{
		GetMapDomainInfo();
		NextState(GetState());
	}

    return TIMERPOLL_DONE;
}


u32 C3acAgent::ReloginProcMsg( CMessage *const pcMsg )
{
    COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;

    switch (pcMsg->event)
    {
    case TAS_LOGIN_RSP:
        {
            CLoginTasRsp cRsp;
            pcOspSipMsg->GetMsgBody(cRsp);

            m_strSessionID = cRsp.GetSession();

            //����3ASAgent�Ǽǵ�ȫ���б���, ע�⣺�����¼����TAC��SessionID��������OnService()�в��ܸ���SessionID����CuAgent
            g_cCuiDataMgr.m_mapSessToTaskMap.Insert(cRsp.GetSession(), GetTaskNO());

            TASKLOG(LOGIN_TASK, EVENT_LEV, "�յ�TAS�ĵ�¼Ӧ��, ��¼TAS[%s]�ĻỰΪ[%s], ����[Online]״̬\n",
                m_t3asURI.GetURIString().c_str(), m_strSessionID.c_str());

            FakeSetHBParam(m_t3asURI);
            NextState(ONLINE_STATE);

            //��3AS��ѯ���������ID������
            CDomainNameGetReq cDomainReq;
            cDomainReq.SetSession(m_strSessionID);
            FakePostMsgReq(cDomainReq, m_t3asURI);

			//��3AS��ѯ��PTZ������ƵԴ
			m_cPtzlockInfoList.Clear();
			CPtzLockQryReq cPtzLockQryReq;
			cPtzLockQryReq.SetSession(m_strSessionID);
			FakePostMsgReq(cPtzLockQryReq, m_t3asURI);
        }
        break;
    case OSP_SIP_MSG_PROC_FAIL:
        {
            //����CReloginTask�д���
        }
        break;
    default:
        TASKLOG(LOGIN_TASK, EVENT_LEV, "[C3acAgent::ReloginProcMsg]�յ�δ֪��Ϣ[%s--%hu]\n",
            OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        break;
    }
    return PROCMSG_OK;
}

u32 C3acAgent::ReloginTimerPoll()
{
    if ( GetCurStateHoldTime() > TACAGENT_EMPTY_USERACCOUNT_CHECK_COUNT ) //ÿ5����һ��
    {
        if ( CheckUserOnline() == 0 )
        {
            TASKLOG(LOGIN_TASK, EVENT_LEV, "TAS[%s]������û��CU��¼���Զ��Ͽ���TAS������\n",
                m_t3asURI.GetURIString().c_str());
            return TIMERPOLL_DEL;
        }

        NextState(RELOGIN_STATE);
    }

	if (GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout())
	{
		GetMapDomainInfo();
	}

    return TIMERPOLL_DONE;
}

/*====================================================================
������      : CheckUserOnline
����        : ����û��˻��Ƿ��ж�Ӧ��CU����
�㷨ʵ��    : CUserAccount��ʹ����CU���������ˣ�Ҳ����ɾ��������C3acAgent������
�������ÿ�ָ��
����ȫ�ֱ���:
�������˵��:
����ֵ˵��  : 0��ʾ�����˻���������, >0��ʾ�����˻�����
----------------------------------------------------------------------
�޸ļ�¼    :
��  ��      �汾        �޸���        �޸�����
2012/1/31/             liangli
====================================================================*/
u32 C3acAgent::CheckUserOnline()
{
    u32 dwCuOnlineNum = 0;
    //if ( m_dwRefCounter == 0 )
    {
        //����û�
        Iterator pPos;
        while ( ! pPos.End() )
        {
            string *pstrUserName = NULL;
            CUserAccount **ppcUserAccount = NULL;
            if ( m_cUserMgr.m_mapUserName2PCuserAccount.Iterate(pPos, pstrUserName, ppcUserAccount) )
            {
                CMS_ASSERT(NULL != ppcUserAccount);
                dwCuOnlineNum += (*ppcUserAccount)->GetCuAgentList().size();
            }
        }
    }
    //����û��б�Ϊ��, ���˳�3AS
    return dwCuOnlineNum;
}

void C3acAgent::InitStateMachine()
{
    CStateProc cOfflineProc;
    cOfflineProc.ProcMsg = (CSipTask::PFProcMsg)&C3acAgent::OfflineProcMsg;
    cOfflineProc.TimerPoll = &CSipTask::TimerPollSkip;
    cOfflineProc.ProcErrMsg = (CSipTask::PFProcMsg)&C3acAgent::OfflineProcMsg;
    AddRuleProc(OFFLINE_STATE, cOfflineProc);

    CStateProc cOnlineProc;
    cOnlineProc.ProcMsg = (CSipTask::PFProcMsg)&C3acAgent::OnlineProcMsg;
    cOnlineProc.TimerPoll = (CSipTask::PFTimerPoll)&C3acAgent::OnlineTimerPoll;
    cOnlineProc.ProcErrMsg = (CSipTask::PFProcMsg)&C3acAgent::OnlineProcMsg;
    AddRuleProc(ONLINE_STATE, cOnlineProc);

    CStateProc cReloginProc;
    cReloginProc.ProcMsg = (CSipTask::PFProcMsg)&C3acAgent::ReloginProcMsg;
    cReloginProc.TimerPoll = (CSipTask::PFTimerPoll)&C3acAgent::ReloginTimerPoll;
    cReloginProc.ProcErrMsg = (CSipTask::PFProcMsg)&C3acAgent::ReloginProcMsg;
    AddRuleProc(RELOGIN_STATE, cReloginProc);

    NextState(OFFLINE_STATE);
}

//��ȡ��ǰ3acAgent������CU�������񼯺�
COnlineCuAgentQueue C3acAgent::GetCuAgentList()
{
    COnlineCuAgentQueue cTaskQue;
    vector<CTask*> vctCuAgents = GetInstance()->GetTasksArray("CCuAgent");
    vector<CTask*>::iterator pcCuAgentItem = vctCuAgents.begin();
    while ( pcCuAgentItem != vctCuAgents.end() )
    {
        CMS_ASSERT( 0 == strcmp((*pcCuAgentItem)->GetObjName(), "CCuAgent" ) );
        CCuAgent *pcCuAgentTask = (CCuAgent *)(*pcCuAgentItem);

        if ( pcCuAgentTask->m_str3asName == m_t3asURI.GetURIString() )
        {
            cTaskQue.push(pcCuAgentTask->GetTaskNO());
        }

        pcCuAgentItem++;
    }
    return cTaskQue;
}

void C3acAgent::EnterWaitReloginState()
{
    NextState(C3acAgent::RELOGIN_STATE);
}

void C3acAgent::FlashTimePoll()
{
    NextState(C3acAgent::ONLINE_STATE);
}

void C3acAgent::ClearDomainResource(const string& strDomainName)
{
    string strID = m_mapDomainInfo.DomainNameToId(strDomainName);
	//Iterator pUsrPos;
	//while ( ! pUsrPos.End() )
	//{
	//	string strUserName;
	//	CUserAccount *pcUserAccount;
	//	m_cUserMgr.m_mapUserName2PCuserAccount.Iterate(pUsrPos, strUserName, pcUserAccount);
	//}

	//Iterator pGroupPos;
	//while ( ! pGroupPos.End() )
	//{
	//	string strUserName;
	//	CUserGroup cUserGroup;
	//	m_cUserMgr.m_mapGroupID2UserGroup.Iterate(pGroupPos, strUserName, cUserGroup);

	//	m_cUserMgr.m_mapGroupID2UserGroup.Insert(strUserName, cUserGroup);
	//}

    DeleteSubscribeKeyByDevDomain(strDomainName);
}
string C3acAgent::GetDomainNameByDomainID(const string& strDomainID)
{
    return m_mapDomainInfo.DomainIdToName(strDomainID);
}
bool C3acAgent::IsExistDomainInfo(const string& strDomainName)
{
    return (!m_mapDomainInfo.DomainNameToId(strDomainName).empty());
}

void C3acAgent::Del3APtzLockInfoByVs(const TChannel &tVsChn)
{
	m_cPtzlockInfoList.DelCuPtzLockInfoByVs(tVsChn);

	CCuPtzLockDelReq lockDelReq;

	lockDelReq.SetSession(m_strSessionID);
	lockDelReq.SetDeviceId(tVsChn.GetDevUri());
	lockDelReq.SetVideoSrcId(tVsChn.GetChnNO());
	lockDelReq.SetOperateType(PTZLOCK_DELETE);

	FakePostMsgReq(lockDelReq, m_t3asURI);
}

void C3acAgent::GetMapDomainInfo()
{
	if (m_bGetMapDomainInfoFinish)
	{
		return;
	}

	string strErrMsg;
	EFDB_ErrorCode ec = g_cRedisDb.GetAllDomainRelationInfo(m_mapDomainInfo, strErrMsg);
	if (EC_OK != ec)
	{
		TASKLOG(LOGIN_TASK, ERROR_LEV, "��ȡ����Ϣʧ�ܣ�ec[%d]��ErrMsg[%s]\n",
			ec, strErrMsg.c_str());
		NextState(GetState()); //��ǰ״̬���¼�ʱ
	}
	else
	{
		m_bGetMapDomainInfoFinish = true;
	}
}

CReLoginTasTask::CReLoginTasTask(CInstExt *pcInst) : CSipTask(pcInst)
{
    m_nReqSeqNum = 0;
    m_dw3acAgentTask = INVALID_TASKNO;
}

CReLoginTasTask::~CReLoginTasTask()
{

}

void CReLoginTasTask::InitStateMachine()
{
    CStateProc cLoginTasState;
    cLoginTasState.ProcMsg = (CSipTask::PFProcMsg)&CReLoginTasTask::LoginTasProcMsg;
    cLoginTasState.TimerPoll = (CSipTask::PFTimerPoll)&CReLoginTasTask::LoginTasTimerPoll;
    cLoginTasState.ProcErrMsg = (CSipTask::PFProcMsg)&CReLoginTasTask::LoginTasProcMsg;
    AddRuleProc(LOGIN_TAS_STATE, cLoginTasState);

    CStateProc cLoginCuState;
    cLoginCuState.ProcMsg = (CSipTask::PFProcMsg)&CReLoginTasTask::LoginCuProcMsg;
    cLoginCuState.TimerPoll = (CSipTask::PFTimerPoll)&CReLoginTasTask::LoginCuTimerPoll;
    cLoginCuState.ProcErrMsg = (CSipTask::PFProcMsg)&CReLoginTasTask::LoginCuProcMsg;
    AddRuleProc(LOGIN_CU_STATE, cLoginCuState);

    NextState(LOGIN_TAS_STATE);
}

u32 CReLoginTasTask::LoginTasProcMsg( CMessage *const pcMsg )
{
    COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;

    switch (pcMsg->event)
    {
    case TAS_LOGIN_RSP:
        {
            CLoginTasRsp cRsp;
            pcOspSipMsg->GetMsgBody(cRsp);

            if ( m_nReqSeqNum != cRsp.GetSeqNum() )
            {
                TASKLOG(LOGIN_TASK, ERROR_LEV, "�յ���¼TAS[%s]Ӧ����ˮ��[%d]������ֵ[%d]��ƥ�䣬���Դ���Ϣ",
                    pcOspSipMsg->GetSipToUri().GetURIString().c_str(), cRsp.GetSeqNum(), m_nReqSeqNum);
                return PROCMSG_OK;
            }

            if (cRsp.GetErrorCode() != CMS_SUCCESS)
            { //ע��3ASʧ��
                TASKLOG(LOGIN_TASK, ERROR_LEV, "CReLoginTasTask��¼TAS[%s]ʧ�ܣ�������[%d], �����ظ���¼",
                    pcOspSipMsg->GetSipToUri().GetURIString().c_str(), cRsp.GetErrorCode());
                NextState(LOGIN_TAS_STATE);
                return PROCMSG_OK;
            }

            //ע��3AS�ɹ�������һ��3ASAgent�����Ա����������3AS��ע��״̬���Լ�����3AS��֪ͨ
            C3acAgent *pc3acAgentTask = (C3acAgent *)(GetInstance()->FindTask(m_dw3acAgentTask));
            if ( pc3acAgentTask != NULL )
            {
                CMS_ASSERT( 0 == strcmp(pc3acAgentTask->GetObjName(), "C3acAgent") );
                //��3ASע��Ӧ����Ϣ���͸�3ASAgent���Ա����¼3AS��Ϣ��ת��״̬
                //��3ASAgent�����3AS��������, �·����3ASAgent���͵�ע������õ�Ӧ��
                pc3acAgentTask->ProcMsg(pcMsg);
            }
            else
            {
                //3acAgent�Ѿ���ΪCUȫ�����߶����٣�����ɾ��������
                delete this;
                return PROCMSG_OK;
            }

            NextState(LOGIN_CU_STATE);
            if ( PROCMSG_DEL == LoginCuReq() )
            {
                delete this;
                return PROCMSG_OK;
            }
        }
        break;
    case OSP_SIP_MSG_PROC_FAIL:
        {

        }
        break;
    default:
        {
            TASKLOG(LOGIN_TASK, CRITICAL_LEV, "�յ�δ֪��Ϣ[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }
    return PROCMSG_OK;
}

u32 CReLoginTasTask::LoginTasTimerPoll()
{
    if ( (u32)GetPollCounter() > g_cCuiConfig.GetSipWaitRspTimeout() )
    {
        C3acAgent *pc3acAgentTask = (C3acAgent *)(GetInstance()->FindTask(m_dw3acAgentTask));
        if ( pc3acAgentTask != NULL )
        {
            CMS_ASSERT( 0 == strcmp(pc3acAgentTask->GetObjName(), "C3acAgent") );
            LoginTasReq();
        }
        else
        {
            //3acAgent�Ѿ���ΪCUȫ�����߶����٣�����ɾ��������
            return TIMERPOLL_DEL;
        }
        NextState(LOGIN_TAS_STATE);
    }
    return TIMERPOLL_DONE;
}

u32 CReLoginTasTask::LoginCuProcMsg( CMessage *const pcMsg )
{
    COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;

    switch (pcMsg->event)
    {
    case TAS_CU_ONLINE_RSP:
        {
            CCuOnlineRsp cAddCuRsp;
            pcOspSipMsg->GetMsgBody(cAddCuRsp);

            if ( m_nReqSeqNum != cAddCuRsp.GetSeqNum() )
            {
                TASKLOG(LOGIN_TASK, ERROR_LEV, "�յ���¼TAS[%s]Ӧ����ˮ��[%d]������ֵ[%d]��ƥ�䣬���Դ���Ϣ",
                    pcOspSipMsg->GetSipToUri().GetURIString().c_str(), cAddCuRsp.GetSeqNum(), m_nReqSeqNum);
                return PROCMSG_OK;
            }

            if (cAddCuRsp.GetErrorCode() != CMS_SUCCESS)
            { //ע��3ASʧ��
                TASKLOG(LOGIN_TASK, EVENT_LEV, "CReLoginTasTask��TAS���CU����״̬ʧ�ܣ�������[%d]\n",
                    cAddCuRsp.GetErrorCode());
            }
            else
            {
                C3acAgent *pc3acAgentTask = (C3acAgent *)(GetInstance()->FindTask(m_dw3acAgentTask));
                if ( NULL == pc3acAgentTask )
                {
                    delete this;
                    return PROCMSG_OK;
                }
                CMS_ASSERT(0 == strcmp(pc3acAgentTask->GetObjName(), "C3acAgent"));

                CCuAgent *pcCuAgentTask = (CCuAgent *)GetInstance()->FindTask(m_tCuAgentIds.front());
                if ( NULL != pcCuAgentTask )
                {
                    CMS_ASSERT( 0 == strcmp( pcCuAgentTask->GetObjName(), "CCuAgent" ) );
                    TASKLOG(LOGIN_TASK, EVENT_LEV, "CReLoginTasTask��TAS���CU[%s]����״̬�ɹ�\n",
                        pcCuAgentTask->m_strUserName.c_str());
                    pcCuAgentTask->m_strCuSessionIn3AS = cAddCuRsp.GetCuSessionId();
                    g_cCuiDataMgr.m_mapSessToTaskMap.Insert(pcCuAgentTask->m_strCuSessionIn3AS, pcCuAgentTask->GetTaskNO());
                }
            }

            m_tCuAgentIds.pop();
            //����������һ��CuAgent
            if ( PROCMSG_DEL == LoginCuReq() )
            {
                delete this;
                return PROCMSG_OK;
            }
        }
        break;
    case OSP_SIP_MSG_PROC_FAIL:
        {

        }
        break;
    default:
        {
            TASKLOG(LOGIN_TASK, CRITICAL_LEV, "�յ�δ֪��Ϣ[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }
    return PROCMSG_OK;
}

u32 CReLoginTasTask::LoginCuTimerPoll()
{
    if ( (u32)GetPollCounter() > g_cCuiConfig.GetSipWaitRspTimeout() )
    {
        if ( PROCMSG_DEL == LoginCuReq() )
        {
            return TIMERPOLL_DEL;
        }
    }
    return TIMERPOLL_DONE;
}

u32 CReLoginTasTask::LoginTasReq()
{
    TASKLOG(LOGIN_TASK, EVENT_LEV, "TAS[%s]��δ�������ӣ�CReLoginTasTask�汾CUI����TASע��\n",
        m_t3asURI.GetURIString().c_str());
    CLoginTasReq cLogin3ASReq;
    cLogin3ASReq.SetUserName(OspSipGetLocalURI().GetURIString());
    cLogin3ASReq.SetLoginType(CLoginTasReq::enLoginType_CUI);

	//������Ҫ��ȡ��3as����Դ���ͼ���
	TPSLoginExternInfo tResourceSet;
	tResourceSet.resourceTypes.insert(TasResourceGeneralDevAdd);
	tResourceSet.resourceTypes.insert(TasResourceGeneralDevDel);
	cLogin3ASReq.SetExtraInfo(tResourceSet.ToXml());

    m_nReqSeqNum = cLogin3ASReq.GetSeqNum();
    FakePostMsgReq(cLogin3ASReq, m_t3asURI);
    return PROCMSG_OK;
}

u32 CReLoginTasTask::LoginCuReq()
{
    //�ҵ���һ�����ߵ�CU
    do 
    {
        if ( m_tCuAgentIds.empty())
        {
            return PROCMSG_DEL;
        }
        CCuAgent *pcCuAgent = (CCuAgent *)(GetInstance()->FindTask(m_tCuAgentIds.front()));
        if ( NULL != pcCuAgent )
        {
            CMS_ASSERT( 0 == strcmp(pcCuAgent->GetObjName(), "CCuAgent") );
            TASKLOG(LOGIN_TASK, EVENT_LEV, "TAS[%s]�Ѿ����������ӣ�CReLoginTasTaskֱ�����CU[%s]����״̬\n",
                m_t3asURI.GetURIString().c_str(), GetNameWithoutDomainFromURI(pcCuAgent->m_strUserName).c_str());
            C3acAgent *pc3acAgentTask = (C3acAgent *)(GetInstance()->FindTask(m_dw3acAgentTask));
            if ( NULL == pc3acAgentTask )
            {
                delete this;
                return PROCMSG_OK;
            }
            CMS_ASSERT(0 == strcmp(pc3acAgentTask->GetObjName(), "C3acAgent"));
            //��3AS���CU���û�
            CCuOnlineReq cAddCuReq;
            cAddCuReq.SetSession(pc3acAgentTask->m_strSessionID);
            cAddCuReq.SetCuUserName(GetNameWithoutDomainFromURI(pcCuAgent->m_strUserName));
            cAddCuReq.SetCuUserPassword(pcCuAgent->m_strPassWord);
            m_nReqSeqNum = cAddCuReq.GetSeqNum();
            FakePostMsgReq(cAddCuReq, m_t3asURI);
            break;
        }
        else
        {
            //CU�Ѿ�����
            m_tCuAgentIds.pop();
        }
    } while (1);
    return PROCMSG_OK;
}

CGetUserInfo::CGetUserInfo(CInstExt *pcInst, const string& str3asName, const string& strUserName, const string& strSession) : CSipTask(pcInst) 
{
    m_str3asName  = str3asName;
    m_strUserName = strUserName;
    m_strSession = strSession;
}

CGetUserInfo::~CGetUserInfo()
{   
	//��������Ͳ���MsgӦ������
	m_VecNtfReqList.clear();
    //while(!m_VecNtfReqList.empty())
    //{
    //    string strNtfMsg = m_VecNtfReqList.front();
    //    m_VecNtfReqList.pop_front();
    //    COspSipMsg *pcOspSipMsg = (COspSipMsg *)strNtfMsg.c_str();
    //    TASKLOG(LOGIN_TASK, EVENT_LEV, "��δ�����NTF��Ϣ[%s],ֱ�ӻظ�\n", OspExtEventDesc(pcOspSipMsg->GetOspEventID()).c_str());
    //    CNtfRsp cRsp;
    //    cRsp.SetEvent(pcOspSipMsg->GetOspEventID() + 1);
    //    string strSession = GetSipSessionID(pcOspSipMsg->GetMsgBody());
    //    cRsp.SetSession(strSession);
    //    string strSeqNum = GetSipSeqNO(pcOspSipMsg->GetMsgBody());
    //    int nSeqNum = static_cast<int>(atol(strSeqNum.c_str()));
    //    cRsp.SetSeqNum(nSeqNum);
    //    PostMsgRsp(pcOspSipMsg->GetSipTransID(), cRsp);
    //}

    if ( ! m_strQueryTag.empty() )
    {
        g_cCuiDataMgr.m_mapSessToTaskMap.Erase(m_strQueryTag);
        m_strQueryTag.clear();
    }
}

void CGetUserInfo::InitStateMachine()
{
    CStateProc cQueryProc;
    cQueryProc.ProcMsg = (CSipTask::PFProcMsg)&CGetUserInfo::NotQueryProcMsg;
    cQueryProc.TimerPoll = &CSipTask::TimerPollSkip;
    cQueryProc.ProcErrMsg = (CSipTask::PFProcMsg)&CGetUserInfo::NotQueryProcMsg;
    AddRuleProc(NOT_QUERY_STATE, cQueryProc);

    CStateProc cWaitRspProc;
    cWaitRspProc.ProcMsg = (CSipTask::PFProcMsg)&CGetUserInfo::WaitRspProcMsg;
    cWaitRspProc.TimerPoll = (CSipTask::PFTimerPoll)&CGetUserInfo::WaitRspTimerPoll;
    cWaitRspProc.ProcErrMsg = (CSipTask::PFProcMsg)&CGetUserInfo::WaitRspProcMsg;
    AddRuleProc(WAIT_RESPONSE_STATE, cWaitRspProc);

    CStateProc cWaitNtfProc;
    cWaitNtfProc.ProcMsg = (CSipTask::PFProcMsg)&CGetUserInfo::WaitNotifyProcMsg;
    cWaitNtfProc.TimerPoll = (CSipTask::PFTimerPoll)&CGetUserInfo::WaitNotifyTimerPoll;
    cWaitNtfProc.ProcErrMsg = (CSipTask::PFProcMsg)&CGetUserInfo::WaitNotifyProcMsg;
    AddRuleProc(WAIT_NOTIFY_STATE, cWaitNtfProc);

    NextState(NOT_QUERY_STATE);
}

u32 CGetUserInfo::StartQryUserInfo()
{ 
    u32 dwProcRet = DoStartQryUserInfo();
    if (PROCMSG_DEL == dwProcRet)
    {
        delete this;
    }
    
    return dwProcRet;
}

u32 CGetUserInfo::DoStartQryUserInfo()
{
    C3acAgent *pc3acAgentTask = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(m_str3asName, GetInstance());
    if (NULL == pc3acAgentTask)
    {
        TASKLOG(LOGIN_TASK, ERROR_LEV, "[CGetUserInfo]3acAgent�Ѿ���������,������Task!\n");       
        return PROCMSG_DEL;
    }
    CMS_ASSERT( 0 == strcmp(pc3acAgentTask->GetObjName(), "C3acAgent") );

    CUserAccount* pcUserAccount = pc3acAgentTask->m_cUserMgr.FindUserAccountByUserName(m_strUserName);
    if(NULL == pcUserAccount)
    {
        TASKLOG(LOGIN_TASK, ERROR_LEV, "[CGetUserInfo]UserAccount�Ѿ���������,������Task!\n");       
        return PROCMSG_DEL;
    }

    //��3AS���Ͳ�ѯ�û������Ա�����û���Ϣ
	CUserQryReq cQryReq;
	cQryReq.SetStartEntryIndex(0);
	cQryReq.SetExpectEntryNum(1);
    cQryReq.SetSession(pc3acAgentTask->m_strSessionID);
    CUserInfoQueryCondition userInfoQueryCondition;
    userInfoQueryCondition.SetUserName(GetNameWithoutDomainFromURI(pcUserAccount->m_strUserName));
    cQryReq.SetUserInfoQueryCondition(userInfoQueryCondition);

    TASKLOG(LOGIN_TASK, EVENT_LEV, "�ԻỰID[%s]��TAS[%s]�����û�[%s]�˻���Ϣ\n",
        pc3acAgentTask->m_strSessionID.c_str(), pc3acAgentTask->m_t3asURI.GetURIString().c_str(), pcUserAccount->m_strUserName.c_str());

    FakePostMsgReq(cQryReq, pc3acAgentTask->m_t3asURI);
    if (!m_strQueryTag.empty())
    {
        g_cCuiDataMgr.m_mapSessToTaskMap.Erase(m_strQueryTag);
    }

	//�µĲ�ѯ���󷢳�ʱ����ձ����֪ͨ����
	m_VecNtfReqList.clear();

    g_cCuiDataMgr.m_mapSessToTaskMap.Insert(cQryReq.GetQueryTag(), GetTaskNO());
    m_strQueryTag = cQryReq.GetQueryTag();

    NextState(WAIT_RESPONSE_STATE);
    return PROCMSG_OK;
}

u32 CGetUserInfo::NotQueryProcMsg(CMessage *const pcMsg)
{
    return PROCMSG_OK;
}

u32 CGetUserInfo::WaitRspProcMsg( CMessage *const pcMsg )
{
    COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
    TASKLOG(LOGIN_TASK, EVENT_LEV, "CGetUserInfo[TaskNO %u]�յ�������[%s]��Ӧ����Ϣ[%s--%hu]\n",
        GetTaskNO(), pcOspSipMsg->GetSipToUri().GetURIString().c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

    switch (pcMsg->event)
    {
    case OSP_SIP_MSG_PROC_FAIL:
        {
            TASKLOG(LOGIN_TASK, EVENT_LEV, "�յ�[OSP_SIP_MSG_PROC_FAIL]��Ϣ\n");
        }
        break;
    case OSP_SIP_DIALOG_TERMINATE:
        {
            CMS_ASSERT( 0 && "CQueryTask::WaitRspProcMsg�����ܳ��ֻỰ����Ϣ" );
        }
        break;
    case TAS_CUSER_QRY_RSP:
        {
            CUserQryRsp cQryRsp;
            pcOspSipMsg->GetMsgBody(cQryRsp);
            if ( CMS_SUCCESS != cQryRsp.GetErrorCode() )
            {
                TASKLOG(LOGIN_TASK, ERROR_LEV, "GetUserInfo���û���¼ʱ��ѯ�û���Ϣʧ�ܣ�������[%d]\n",
                    cQryRsp.GetErrorCode());
                delete this;
                return PROCMSG_DEL;
            }
            CMS_ASSERT( 1 == cQryRsp.GetTotalEntryNum() );
            TASKLOG(LOGIN_TASK, EVENT_LEV, "GetUserInfo��Ϣ��ѯ��QueryTagΪ[%s], ����Task[%s--%u]�Ǽǵ��Ự����\n", 
                cQryRsp.GetQueryTag().c_str(), GetObjName(), GetTaskNO());
            NextState(WAIT_NOTIFY_STATE);
        }
        break;
    case TAS_CUSER_QRY_NTF_REQ:
        {           
            if(pcMsg->content != NULL)
            {              
                string strContect((char *)pcMsg->content, pcMsg->length);
                m_VecNtfReqList.push_back(strContect);
                TASKLOG(LOGIN_TASK, CRITICAL_LEV, "��[WAIT_USER_INFO_RESPONSE_STATE]�յ���Ӧ��Ntf��Ϣ[%s--%hu]\n",
                    OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            }           
        }
        break;
    default:
        {
            TASKLOG(LOGIN_TASK, CRITICAL_LEV, "�յ�δ֪��Ϣ[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"�յ�δ֪��Ϣ");
        }
        break;
    }
    return PROCMSG_OK;
}

u32 CGetUserInfo::WaitRspTimerPoll()
{
    if ( GetCurStateHoldTime() > g_cCuiConfig.GetDbQueryTimeout() )
    {   
        TASKLOG(LOGIN_TASK, ERROR_LEV, "CUI[%s]�û���[%s]�ȴ�Ӧ����Ϣ[TAS_CUSER_DEVICE_QRY_RSP]��ʱ\n", 
            m_str3asName.c_str(), m_strUserName.c_str());
        
        if (DoStartQryUserInfo() == PROCMSG_DEL)
        {
            NextState(NOT_QUERY_STATE);
            return TIMERPOLL_DEL;
        }
    }
    return TIMERPOLL_DONE;
}

u32 CGetUserInfo::WaitNotifyProcMsg( CMessage *const pcMsg )
{
    while(m_VecNtfReqList.size() != 0)
    {
        CMessage cMsg;
        cMsg.event = TAS_CUSER_QRY_NTF_REQ;
        string content = m_VecNtfReqList.front();
        m_VecNtfReqList.pop_front();
        cMsg.content = (u8 *)content.c_str();

        ProcNotifyMsg(&cMsg);
    }
    ProcNotifyMsg(pcMsg);

    return PROCMSG_OK;
}

u32 CGetUserInfo::ProcNotifyMsg(CMessage *const pcMsg)
{
    COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;

    switch (pcMsg->event)
    {
    case OSP_SIP_MSG_PROC_FAIL:
    case OSP_SIP_DIALOG_TERMINATE:
        {
            CMS_ASSERT( 0 && "CQueryTask::WaitNotifyProcMsg�����ܳ���SIP�쳣" );
        }
        break;
    case TAS_CUSER_QRY_NTF_REQ:
        {
            CUserQryNtfReq cNtfReq;
            pcOspSipMsg->GetMsgBody(cNtfReq);
            CMS_ASSERT(cNtfReq.GetEntryNum() == 1);
            CMS_ASSERT(cNtfReq.IsLastNtf());

            //�Ȼظ�NTF֪ͨ
            CNtfRsp cRsp;
            cRsp.SetEvent(pcMsg->event + 1);
            cRsp.SetSeqNum(cNtfReq.GetSeqNum());
            cRsp.SetSession(cNtfReq.GetSession());
            PostMsgRsp(pcOspSipMsg->GetSipTransID(), cRsp);

            vector<CUserInfo> &cUserInfos = cNtfReq.GetUserInfo();
			if (cUserInfos.empty())
			{
				TASKLOG(LOGIN_TASK, ERROR_LEV, "[CGetUserInfo]Ntf UserInfo Empty!!!!\n");
				delete this;
				return PROCMSG_DEL;
			}
            CUserInfo &cUserInfo = cUserInfos.at(0);

            C3acAgent *pc3acAgentTask = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(m_str3asName, GetInstance());
            if (NULL == pc3acAgentTask)
            {
                TASKLOG(LOGIN_TASK, ERROR_LEV, "[CGetUserInfo]3acAgent�Ѿ���������,������Task!\n");       
                delete this;
                return PROCMSG_DEL;
            }
            CMS_ASSERT( 0 == strcmp(pc3acAgentTask->GetObjName(), "C3acAgent") );

            CUserAccount* pcUserAccount = pc3acAgentTask->m_cUserMgr.FindUserAccountByUserName(m_strUserName);
            if(NULL == pcUserAccount)
            {
                TASKLOG(LOGIN_TASK, ERROR_LEV, "[CGetUserInfo]UserAccount�Ѿ���������,������Task!\n");       
                delete this;
                return PROCMSG_DEL;
            }

            pcUserAccount->SetUserAccountBaseInfo(cUserInfo, pcUserAccount->m_strUserName);
            pc3acAgentTask->m_cUserMgr.m_mapUserID2UserName.Insert(cUserInfo.GetUserID(), pcUserAccount->m_strUserName);

            bool bIsInGroup = false;

            //����û������飬������û�����Ϣ
            if ( !cUserInfo.GetUsergrpID().empty())
            {
                CUserGroup *pcUserGroup = pc3acAgentTask->m_cUserMgr.FindUserGroupByGroupID(cUserInfo.GetUsergrpID());
                //�û��Ƿ�������
                bIsInGroup = true;
                //����и�����Ϣ�򲻽��в�ѯ
                if ( pcUserGroup == NULL )
                { //����û��鲻���ڣ����ȴ���һ������
                    CUserGroup cUserGroup(&pc3acAgentTask->m_cUserMgr);
                    cUserGroup.m_tGroupID = pcUserAccount->m_tUserGroupID;
                    cUserGroup.m_bDevPriState = DEVPRI_NOT_START;

                    //��ȡ����Ϣ
                    pc3acAgentTask->m_cUserMgr.m_mapGroupID2UserGroup.Insert(cUserGroup.m_tGroupID, cUserGroup);

                    CGetGroupInfo *pcTask = new CGetGroupInfo(GetInstance(), m_str3asName, pcUserAccount->m_tUserGroupID);
                    pcTask->InitStateMachine();
                    pcTask->StartQryGroupInfo();
                }
            }

            //������β�ѯ
            if ( cNtfReq.IsLastNtf() )
            {
                g_cCuiDataMgr.m_mapSessToTaskMap.Erase(cNtfReq.GetQueryTag());

                NextState(NOT_QUERY_STATE);

                string strLogDesc = GetOptLogDesc(pcUserAccount);
                WriteLog(strLogDesc);

                delete this;
            }
        }
        break;
    default:
        {
            TASKLOG(LOGIN_TASK, CRITICAL_LEV, "�յ�δ֪��Ϣ[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"�յ�δ֪��Ϣ");
        }
        break;
    }
    return PROCMSG_OK;
}

void CGetUserInfo::WriteLog(const string& strLogDesc)
{
    CUsrOprLogTask* pcTask = new CUsrOprLogTask(GetInstance(), m_strSession, 
        string(DEV_TYPE_UAS) + "@" + GetDomainName(),
        enUserOpTypeLogin,
        CMS_SUCCESS,
        strLogDesc,
        string()
        );
    pcTask->InitStateMachine();
    pcTask->StartSendLog();
}

string CGetUserInfo::GetOptLogDesc(const CUserAccount* pcUserAccount)
{
    return GetLanDesc(LAN_DESC_LOGIN) + GetLanDesc(LAN_DESC_SUCCESS);
}

u32 CGetUserInfo::WaitNotifyTimerPoll()
{
    if ( GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout() )
    {
        TASKLOG(LOGIN_TASK, ERROR_LEV, "CGetUserInfo[NO: %u]�ȴ�NTF��Ϣ[TAS_CUSER_DEVICE_QRY_NTF_RSP]��ʱ\n",
            GetTaskNO());

        if (DoStartQryUserInfo() == PROCMSG_DEL)
        {
            g_cCuiDataMgr.m_mapSessToTaskMap.Erase(m_strQueryTag);
            m_strQueryTag.clear();
            NextState(NOT_QUERY_STATE);
            return TIMERPOLL_DEL;
        }
    }

    return TIMERPOLL_DONE;
}

CGetGroupInfo::CGetGroupInfo(CInstExt *pcInst, const string& str3asName, const string& strGroupID) : CSipTask(pcInst) 
{
    m_str3asName  = str3asName;
    m_strGroupID = strGroupID;
}

CGetGroupInfo::~CGetGroupInfo()
{   
    while(!m_VecNtfReqList.empty())
    { 
        string strNtfMsg = m_VecNtfReqList.front();
        m_VecNtfReqList.pop_front();
        COspSipMsg *pcOspSipMsg = (COspSipMsg *)strNtfMsg.c_str();
        TASKLOG(LOGIN_TASK, EVENT_LEV, "��δ�����NTF��Ϣ[%s],ֱ�ӻظ�\n", OspExtEventDesc(pcOspSipMsg->GetOspEventID()).c_str());
        CNtfRsp cRsp;
        cRsp.SetEvent(pcOspSipMsg->GetOspEventID() + 1);
        string strSession = GetSipSessionID(pcOspSipMsg->GetMsgBody());
        cRsp.SetSession(strSession);
        string strSeqNum = GetSipSeqNO(pcOspSipMsg->GetMsgBody());
        int nSeqNum = static_cast<int>(atol(strSeqNum.c_str()));
        cRsp.SetSeqNum(nSeqNum);
        PostMsgRsp(pcOspSipMsg->GetSipTransID(), cRsp);
    }

    if ( ! m_strQueryTag.empty() )
    {
        g_cCuiDataMgr.m_mapSessToTaskMap.Erase(m_strQueryTag);
        m_strQueryTag.clear();
    }
}

void CGetGroupInfo::InitStateMachine()
{
    CStateProc cQueryProc;
    cQueryProc.ProcMsg = (CSipTask::PFProcMsg)&CGetGroupInfo::NotQueryProcMsg;
    cQueryProc.TimerPoll = &CSipTask::TimerPollSkip;
    cQueryProc.ProcErrMsg = (CSipTask::PFProcMsg)&CGetGroupInfo::NotQueryProcMsg;
    AddRuleProc(NOT_QUERY_STATE, cQueryProc);

    CStateProc cWaitRspProc;
    cWaitRspProc.ProcMsg = (CSipTask::PFProcMsg)&CGetGroupInfo::WaitRspProcMsg;
    cWaitRspProc.TimerPoll = (CSipTask::PFTimerPoll)&CGetGroupInfo::WaitRspTimerPoll;
    cWaitRspProc.ProcErrMsg = (CSipTask::PFProcMsg)&CGetGroupInfo::WaitRspProcMsg;
    AddRuleProc(WAIT_RESPONSE_STATE, cWaitRspProc);

    CStateProc cWaitNtfProc;
    cWaitNtfProc.ProcMsg = (CSipTask::PFProcMsg)&CGetGroupInfo::WaitNotifyProcMsg;
    cWaitNtfProc.TimerPoll = (CSipTask::PFTimerPoll)&CGetGroupInfo::WaitNotifyTimerPoll;
    cWaitNtfProc.ProcErrMsg = (CSipTask::PFProcMsg)&CGetGroupInfo::WaitNotifyProcMsg;
    AddRuleProc(WAIT_NOTIFY_STATE, cWaitNtfProc);

    NextState(NOT_QUERY_STATE);
}

u32 CGetGroupInfo::StartQryGroupInfo()
{  
    u32 dwProcRet = DoStartQryGroupInfo();
    if (PROCMSG_DEL == dwProcRet)
    {
        delete this;
    }
    return dwProcRet;
}

u32 CGetGroupInfo::DoStartQryGroupInfo()
{
    C3acAgent *pc3acAgentTask = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(m_str3asName, GetInstance());
    if (NULL == pc3acAgentTask)
    {
        TASKLOG(LOGIN_TASK, ERROR_LEV, "[CGetGroupInfo]3acAgent�Ѿ���������,������Task!\n");       
        return PROCMSG_DEL;
    }
    CMS_ASSERT( 0 == strcmp(pc3acAgentTask->GetObjName(), "C3acAgent") );

    CUserGroup* pcUserGroup = pc3acAgentTask->m_cUserMgr.FindUserGroupByGroupID(m_strGroupID);
    if(NULL == pcUserGroup)
    {
        TASKLOG(LOGIN_TASK, ERROR_LEV, "[CGetGroupInfo]UserAccount�Ѿ���������,������Task!\n");       
        return PROCMSG_DEL;
    }

    //��3AS���Ͳ�ѯ�û������Ա�����û���Ϣ
    CUserGroupQryReq cQryReq;
    cQryReq.SetSession(pc3acAgentTask->m_strSessionID);
    CUserGroupInfoQueryCondition userGroupInfoQueryCondition;
    userGroupInfoQueryCondition.SetGroupNO(m_strGroupID);
    cQryReq.SetUserGroupInfoQueryCondition(userGroupInfoQueryCondition);

    TASKLOG(LOGIN_TASK, EVENT_LEV, "CUI�ỰID[%s]��TAS[%s]�����û���[%s]�˻���Ϣ\n",
        pc3acAgentTask->m_strSessionID.c_str(), pc3acAgentTask->m_t3asURI.GetURIString().c_str(), m_strGroupID.c_str());

    FakePostMsgReq(cQryReq, pc3acAgentTask->m_t3asURI);
    if (!m_strQueryTag.empty())
    {
        g_cCuiDataMgr.m_mapSessToTaskMap.Erase(m_strQueryTag);
    }
    g_cCuiDataMgr.m_mapSessToTaskMap.Insert(cQryReq.GetQueryTag(), GetTaskNO());
    m_strQueryTag = cQryReq.GetQueryTag();

    NextState(WAIT_RESPONSE_STATE);
    return PROCMSG_OK;
}


u32 CGetGroupInfo::NotQueryProcMsg(CMessage *const pcMsg)
{
    return PROCMSG_OK;
}

u32 CGetGroupInfo::WaitRspProcMsg( CMessage *const pcMsg )
{
    COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
    TASKLOG(LOGIN_TASK, EVENT_LEV, "CGetGroupInfo[TaskNO %u]�յ�������[%s]��Ӧ����Ϣ[%s--%hu]\n",
        GetTaskNO(), pcOspSipMsg->GetSipToUri().GetURIString().c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

    switch (pcMsg->event)
    {
    case OSP_SIP_MSG_PROC_FAIL:
        {
            TASKLOG(LOGIN_TASK, EVENT_LEV, "�յ�[OSP_SIP_MSG_PROC_FAIL]��Ϣ\n");
        }
        break;
    case OSP_SIP_DIALOG_TERMINATE:
        {
            CMS_ASSERT( 0 && "CGetGroupInfo::WaitRspProcMsg�����ܳ��ֻỰ����Ϣ" );
        }
        break;
    case TAS_CUSER_GROUP_QRY_RSP:
        {
            CUserGroupQryRsp cQryRsp;
            pcOspSipMsg->GetMsgBody(cQryRsp);
            if ( CMS_SUCCESS != cQryRsp.GetErrorCode() )
            {
                TASKLOG(LOGIN_TASK, ERROR_LEV, "[CGetGroupInfo]���û���¼ʱ��ѯ�û���Ϣʧ�ܣ�������[%d]\n",
                    cQryRsp.GetErrorCode());
                delete this;
                return PROCMSG_DEL;
            }
            CMS_ASSERT( 1 == cQryRsp.GetTotalEntryNum() );
            TASKLOG(LOGIN_TASK, EVENT_LEV, "[CGetGroupInfo]��Ϣ��ѯ��QueryTagΪ[%s], ����Task[%s--%u]�Ǽǵ��Ự����\n", 
                cQryRsp.GetQueryTag().c_str(), GetObjName(), GetTaskNO());
            NextState(WAIT_NOTIFY_STATE);
        }
        break;
    case TAS_CUSER_GROUP_QRY_NTF_REQ:
        {           
            if(pcMsg->content != NULL)
            {              
                string strContect((char *)pcMsg->content, pcMsg->length);
                m_VecNtfReqList.push_back(strContect);
                TASKLOG(LOGIN_TASK, CRITICAL_LEV, "��[WAIT_USER_INFO_RESPONSE_STATE]�յ���Ӧ��Ntf��Ϣ[%s--%hu]\n",
                    OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            }           
        }
        break;
    default:
        {
            TASKLOG(LOGIN_TASK, CRITICAL_LEV, "�յ�δ֪��Ϣ[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"�յ�δ֪��Ϣ");
        }
        break;
    }
    return PROCMSG_OK;
}

u32 CGetGroupInfo::WaitRspTimerPoll()
{
    if ( GetCurStateHoldTime() > g_cCuiConfig.GetDbQueryTimeout() )
    {   
        TASKLOG(LOGIN_TASK, ERROR_LEV, "CUI[%s]�û���ID[%s]�ȴ�Ӧ����Ϣ[TAS_CUSER_GROUP_QRY_RSP]��ʱ\n", 
            m_str3asName.c_str(), m_strGroupID.c_str());
        if (PROCMSG_DEL == DoStartQryGroupInfo())
        {
            NextState(NOT_QUERY_STATE);
            return TIMERPOLL_DEL;
        }
    }
    return TIMERPOLL_DONE;
}

u32 CGetGroupInfo::WaitNotifyProcMsg( CMessage *const pcMsg )
{
    while(m_VecNtfReqList.size() != 0)
    {
        CMessage cMsg;
        cMsg.event = TAS_CUSER_GROUP_QRY_NTF_REQ;
        string content = m_VecNtfReqList.front();
        m_VecNtfReqList.pop_front();
        cMsg.content = (u8 *)content.c_str();

        ProcNotifyMsg(&cMsg);
    }
    ProcNotifyMsg(pcMsg);

    return PROCMSG_OK;
}

u32 CGetGroupInfo::ProcNotifyMsg(CMessage *const pcMsg)
{
    COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;

    switch (pcMsg->event)
    {
    case OSP_SIP_MSG_PROC_FAIL:
    case OSP_SIP_DIALOG_TERMINATE:
        {
            CMS_ASSERT( 0 && "CGetGroupInfo::WaitNotifyProcMsg�����ܳ���SIP�쳣" );
        }
        break;
    case TAS_CUSER_GROUP_QRY_NTF_REQ:
        {
            CUserGroupQryNtfReq cNtfReq;
            pcOspSipMsg->GetMsgBody(cNtfReq);
            CMS_ASSERT(cNtfReq.GetEntryNum() == 1);
            CMS_ASSERT(cNtfReq.IsLastNtf());

            //�Ȼظ�NTF֪ͨ
            CNtfRsp cRsp;
            cRsp.SetEvent(pcMsg->event + 1);
            cRsp.SetSeqNum(cNtfReq.GetSeqNum());
            cRsp.SetSession(cNtfReq.GetSession());
            PostMsgRsp(pcOspSipMsg->GetSipTransID(), cRsp);

            vector<CUserGroupInfo> &cUserInfos = cNtfReq.GetUserGroupInfo();
            CUserGroupInfo &cUserGroupInfo = cUserInfos.at(0);

            C3acAgent *pc3acAgentTask = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(m_str3asName, GetInstance());
            if (NULL == pc3acAgentTask)
            {
                TASKLOG(LOGIN_TASK, ERROR_LEV, "[CGetGroupInfo]3acAgent�Ѿ���������,������Task!\n");       
                delete this;
                return PROCMSG_DEL;
            }
            CMS_ASSERT( 0 == strcmp(pc3acAgentTask->GetObjName(), "C3acAgent") );

            CUserGroup* pcUserGroup = pc3acAgentTask->m_cUserMgr.FindUserGroupByGroupID(m_strGroupID);
            if(NULL == pcUserGroup)
            {
                TASKLOG(LOGIN_TASK, ERROR_LEV, "[CGetGroupInfo]UserAccount�Ѿ���������,������Task!\n");       
                delete this;
                return PROCMSG_DEL;
            }

            //�����������Ϣ
            pcUserGroup->SetUserGroupBaseInfo(cUserGroupInfo);

            //������β�ѯ
            if ( cNtfReq.IsLastNtf() )
            {
                g_cCuiDataMgr.m_mapSessToTaskMap.Erase(cNtfReq.GetQueryTag());
				
                NextState(NOT_QUERY_STATE);
                delete this;
            }
        }
        break;
    default:
        {
            TASKLOG(LOGIN_TASK, CRITICAL_LEV, "�յ�δ֪��Ϣ[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"�յ�δ֪��Ϣ");
        }
        break;
    }
    return PROCMSG_OK;
}

u32 CGetGroupInfo::WaitNotifyTimerPoll()
{
    if ( GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout() )
    {
        TASKLOG(LOGIN_TASK, ERROR_LEV, "CGetGroupInfo[NO: %u]�ȴ�NTF��Ϣ[TAS_CUSER_GROUP_QRY_NTF_REQ]��ʱ\n",
            GetTaskNO());
        if (PROCMSG_DEL == DoStartQryGroupInfo())
        {
            NextState(NOT_QUERY_STATE);
            return TIMERPOLL_DEL;
        }
    }
    return TIMERPOLL_DONE;
}

CPeAgentBase::CPeAgentBase(CInstExt *pcInst) 
    : CSipTask(pcInst), m_pcModuleListSubscribeTask(NULL)
{
    m_strSessionID.clear();
}

CPeAgentBase::~CPeAgentBase()
{
    if (NULL != m_pcModuleListSubscribeTask)
    {
        delete m_pcModuleListSubscribeTask;
        m_pcModuleListSubscribeTask = NULL;
    }
    if (NULL != m_pcModuleListSubscribeTask)
    {
        delete m_pcModuleListSubscribeTask;
        m_pcModuleListSubscribeTask = NULL;
    }

    //������Ҫ���ǳ�����
    StopHB(m_tPeURI);
    ClearDataWhenPeOfflineOrDisconnect();

    g_cCuiDataMgr.m_mapPeNameToPeTaskMap.Erase(m_tPeURI.GetURIString());
}

void CPeAgentBase::ClearDataWhenPeOfflineOrDisconnect()
{
    m_strSessionID.clear();

    if (NULL != m_pcModuleListSubscribeTask)
    {
        m_pcModuleListSubscribeTask->ModuleDisconnect();
    }

    if ( ! m_cSubscribeTaskSet.IsEmpty() )
    {
        //����ͳһ��һ��task����
        //CDevUriToSubscribeTaskMap tmpSubscribeTaskSet = m_cSubscribeTaskSet;
        Iterator pPos;
        while ( ! pPos.End() )
        {
            string strDevURI;
            u32 dwTaskId = INVALID_TASKNO;
            if ( m_cSubscribeTaskSet.Iterate(pPos, strDevURI, dwTaskId) )
            {
                CSubBaseTask *pcSubTask = (CSubBaseTask *)(GetInstance()->FindTask(dwTaskId));
                if ( NULL != pcSubTask )
                {
                    pcSubTask->ReSub();
                }
                else
                {
                    TASKLOG(LOGIN_TASK, CRITICAL_LEV, "�Ҳ���CCuAgent��CSubBaseTask[NO: %u]��������\n", dwTaskId);
                    CMS_ASSERT(0 && "�Ҳ���CCuAgent��CSubscribeTask������");
                }
            }
        }

        //������գ����ڶ��Ĳ�ɾ��
        //m_cSubscribeTaskSet.Empty();    //ע�⣬CSubscribeTask��TerminateTask������Ҳ��ɾ�����ظ�ɾû������
    }

    if ( ! m_cInviteTaskSet.IsEmpty() )
    {
        CPlayIdToInviteTaskMap tmpInviteTaskSet = m_cInviteTaskSet;
        Iterator pPos;
        while ( ! pPos.End() )
        {
            s32 nPlayId = INVALID_PLAY_ID;
            u32 dwTaskId = INVALID_TASKNO;
            if ( tmpInviteTaskSet.Iterate(pPos, nPlayId, dwTaskId) )
            {
                CPeInviteTask *pcInviteTask = (CPeInviteTask *)( GetInstance()->FindTask(dwTaskId) );
                if ( NULL != pcInviteTask )
                {
                    pcInviteTask->NotifyStop2Cu(ERR_CUI_PE_OFFLINE);
                    pcInviteTask->StartSendInviteBye();
                    //delete pcInviteTask;
                    //��CInviteTask�Լ�ɾ���Լ�������ע��CuAgent�еĵǼ�
                    // 2012.3.13 ���ܵ�InviteByeRsp��ɾ������Ϊ��ʱCuAgent�Ѿ�����
                }
                else
                {
                    TASKLOG(LOGIN_TASK, CRITICAL_LEV, "�Ҳ���CCuAgent��CInviteTask[NO: %u]��������\n",
                        dwTaskId);
                }
            }
        }
        m_cInviteTaskSet.Empty();
    }

    if ( ! m_cFileExportTaskSet.IsEmpty() )
    {
        CFileUrlToExportTaskMap tmpFileExportTaskSet = m_cFileExportTaskSet;
        Iterator pPos;
        while ( ! pPos.End() )
        {
            string strFileUrl;
            u32 dwTaskId = INVALID_TASKNO;
            if ( tmpFileExportTaskSet.Iterate(pPos, strFileUrl, dwTaskId) )
            {
                CExportTask *pcTask = (CExportTask *)( GetInstance()->FindTask(dwTaskId) );
                if ( NULL != pcTask )
                {
                    pcTask->NotifyStop2Cu(ERR_CUI_PE_OFFLINE);
                    pcTask->StartSendExportBye();
                }
                else
                {
                    TASKLOG(LOGIN_TASK, CRITICAL_LEV, "�Ҳ���CCuAgent��CInviteTask[NO: %u]��������\n",
                        dwTaskId);
                }
            }
        }
        m_cFileExportTaskSet.Empty();
    }
    //���callback�вд��
    m_tCallBackTaskList.clear();


	if (!m_setDpsSearchTaskNo.empty())
	{
		// ����ط�Ҫ����һ�ݣ����� task����ʱ��ɾ��ԭ��set�����±�����
		set<u32> tSetDpsSearchTaskNo = m_setDpsSearchTaskNo;

		for (set<u32>::const_iterator taskNoIter = tSetDpsSearchTaskNo.begin();
		taskNoIter != tSetDpsSearchTaskNo.end(); taskNoIter++)
		{
			CDpsSearchTask *ptTask = (CDpsSearchTask*)(GetInstance()->FindTask(*taskNoIter));
			if (ptTask != NULL)
			{
				TASKLOG(LOGIN_TASK, CRITICAL_LEV, "ɾ��CCuAgent��CDpsSearchTask[NO: %u]��������\n", *taskNoIter);
				delete ptTask;
				ptTask = NULL;
			}
			else
			{
				TASKLOG(LOGIN_TASK, CRITICAL_LEV, "�Ҳ���CCuAgent��CDpsSearchTask[NO: %u]��������\n",
					*taskNoIter);
			}
		}

		m_setDpsSearchTaskNo.clear();
	}
}

void CPeAgentBase::CallBackTasks()
{
    if(0 == m_tCallBackTaskList.size())
    {
        return;
    }

    for ( TCallBackTaskList::const_iterator pTask = m_tCallBackTaskList.begin();pTask != m_tCallBackTaskList.end(); pTask++ )
    {
        CCallBackTask *pcTaskCallback = (CCallBackTask *)(GetInstance()->FindTask(*pTask));
        TASKLOG(LOGIN_TASK, EVENT_LEV, "�ҵ�task����������Ϣʱ�䣡\n");
        if ( pcTaskCallback && pcTaskCallback->StartSendReq() == PROCMSG_DEL )
        {
            delete pcTaskCallback;
            pcTaskCallback = NULL;
        }
    }

    m_tCallBackTaskList.clear();
}

bool CPeAgentBase::IsEmpty()
{
    if(!m_cInviteTaskSet.IsEmpty() || !m_tCallBackTaskList.empty() || m_cSubscribeTaskSet.IsEmpty())
    {
        return false;
    }
    return true;
}

CSipTask * CPeAgentBase::GetSubTaskPtrByDevURI(const string &strDevURI)
{
    u32 dwTaskID = INVALID_TASKNO;
    if ( ! m_cSubscribeTaskSet.Find(strDevURI, dwTaskID) )
    {
        return NULL;
    }

    if ( INVALID_TASKNO == dwTaskID )
    {
        return NULL;
    }
    return static_cast<CSipTask *>(GetInstance()->FindTask(dwTaskID));
}

void CPeAgentBase::PrintData() const
{
    CSipTask::PrintData();
    OspPrintf(TRUE, FALSE, "PeUri:%s\n", m_tPeURI.GetURIString().c_str());
}

CPeAgent::CPeAgent(CInstExt *pcInst) : CPeAgentBase(pcInst)
{
}

CPeAgent::~CPeAgent()
{
}

void CPeAgent::TerminateTask()
{
}

void CPeAgent::InitStateMachine()
{
    CStateProc cIdleSvrProc;
    cIdleSvrProc.ProcMsg = (CTask::PFProcMsg)&CPeAgent::OnIdle;
    cIdleSvrProc.ProcErrMsg = (CTask::PFProcMsg)&CPeAgent::OnIdle;
    cIdleSvrProc.TimerPoll = (CTask::PFTimerPoll)&CPeAgent::OnIdleTimer;
    AddRuleProc(Idle, cIdleSvrProc);

    CStateProc cRegSvrProc;
    cRegSvrProc.ProcMsg = (CTask::PFProcMsg)&CPeAgent::OnWaitRegRsp;
    cRegSvrProc.ProcErrMsg = (CTask::PFProcMsg)&CPeAgent::OnWaitRegRsp;
    cRegSvrProc.TimerPoll = (CTask::PFTimerPoll)&CPeAgent::OnWaitRegRspTimer;
    AddRuleProc(WaitRegRsp, cRegSvrProc);

    CStateProc cServiceProc;
    cServiceProc.ProcMsg = (CTask::PFProcMsg)&CPeAgent::OnService;
    cServiceProc.ProcErrMsg = (CTask::PFProcMsg)&CPeAgent::OnService;
    cServiceProc.TimerPoll = (CTask::PFTimerPoll)&CPeAgent::OnServiceTimer;
    AddRuleProc(Service, cServiceProc);

    NextState(Idle);
}

u32 CPeAgent::OnIdle(CMessage *const pcMsg)
{
    if(OSP_SIP_MSG_PROC_FAIL == pcMsg->event)
    {
        TASKLOG(LOGIN_TASK, WARNING_LEV,"��[CPeAgent::OnIdle]��[%s]ע��ʧ��, �Ժ�������ע��\n",
            m_tPeURI.GetURIString().c_str());
        return PROCMSG_FAIL;
    }

    TASKLOG(LOGIN_TASK, CRITICAL_LEV, "��[CPeAgent::OnIdle]�յ�δ֪��Ϣ[%s--%hu]��������\n",
        OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

    CMS_ASSERT(0&&"CPeAgent::OnIdle�յ�δ֪��Ϣ");
    return PROCMSG_OK;
}

u32 CPeAgent::OnIdleTimer()
{
    if (GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout())
    {
        TASKLOG(LOGIN_TASK, TIMER_LEV, "�����������[%s]ע��\n", 
            m_tPeURI.GetURIString().c_str());

        StartLogin();
    }

    return TIMERPOLL_DONE;
}

u32 CPeAgent::OnWaitRegRsp(CMessage *const pcMsg)
{
    TASKLOG(LOGIN_TASK, EVENT_LEV, "��[CPeAgent::OnWaitRegRsp]�յ���Ϣ[%s--%hu]\n",
        OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
    u32 dwRet = PROCMSG_FAIL;

    switch(pcMsg->event)
    {
    case DEV_LOGIN_RSP:
        {
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

            u32 dwErrorCode = (u32)atoi(GetSipErrorcode(pcOspSipMsg->GetMsgBody()).c_str());
            if ( dwErrorCode == CMS_SUCCESS )
            {
                m_strSessionID = GetSipSessionID(pcOspSipMsg->GetMsgBody());
                SetHBParam(m_tPeURI);
                NextState(Service);

                g_cCuiDataMgr.FlushPlatformServiceStatus(GetInstance(), NULL, m_tPeURI);

                if (m_tPeURI.GetUser() == UNIQUE_SIP_UA_MPS)
                {
                    if (NULL == m_pcModuleListSubscribeTask)
                    {
                        m_pcModuleListSubscribeTask = new CMpsListSubscribeTask(GetInstance(), m_tPeURI.GetURIString(), m_strSessionID);
                        m_pcModuleListSubscribeTask->InitStateMachine();
						m_pcModuleListSubscribeTask->DoSubscribe();
                    }
                    else
                    {
                        m_pcModuleListSubscribeTask->ModuleReconnect(m_strSessionID);
                    }
                }
                else if (m_tPeURI.GetUser() == UNIQUE_SIP_UA_RCS)
                {
                    if (NULL == m_pcModuleListSubscribeTask)
                    {
                        m_pcModuleListSubscribeTask = new CRcsListSubscribeTask(GetInstance(), m_tPeURI.GetURIString(), m_strSessionID);
                        m_pcModuleListSubscribeTask->InitStateMachine();
						m_pcModuleListSubscribeTask->DoSubscribe();
                    }
                    else
                    {
                        m_pcModuleListSubscribeTask->ModuleReconnect(m_strSessionID);
                    }
                }
            }
            else
            {
                //�ȴ���ʱ���������ӷ�����
                TASKLOG(LOGIN_TASK, ERROR_LEV, "ע��[%s]ʧ�ܣ������� %d, �Ժ�������ע��\n",
                    m_tPeURI.GetURIString().c_str(), dwErrorCode);
                NextState(Idle);
            }
            dwRet = PROCMSG_OK;
        }
        break;
    case OSP_SIP_MSG_PROC_FAIL: 
        {
            //�ȴ���ʱ���������ӷ�����
            TASKLOG(LOGIN_TASK, ERROR_LEV,"��[%s]ע��ʧ��, �Ժ�������ע��\n",
                m_tPeURI.GetURIString().c_str());

            NextState(Idle);
        }
        break;
    default:
        {
            TASKLOG(LOGIN_TASK, ERROR_LEV, "Receive unkown Msg[%s-%hu]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }

    //ɾ�����й��ص�task
    //���CPeAgentδ������̬���ɾ�����лص�task
    CallBackTasks();

    return dwRet;
}

u32 CPeAgent::OnWaitRegRspTimer()
{
    if (GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout())
    {
        TASKLOG(LOGIN_TASK, WARNING_LEV, "ע�������[%s]��ʱ�����̿�ʼ����\n", 
            m_tPeURI.GetURIString().c_str());
        NextState(Idle);
        StartLogin();
    }

    return TIMERPOLL_DONE;
}

u32 CPeAgent::OnService( CMessage *const pcMsg )
{
    TASKLOG(LOGIN_TASK, CRITICAL_LEV, "��[CPeAgent::OnService]�յ���Ϣ[%s--%hu]\n",
        OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
    COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
    u32 dwRet = PROCMSG_FAIL;

    switch (pcMsg->event)
    {
    case OSP_SIP_DISCONNECT:
        {
            TASKLOG(LOGIN_TASK, EVENT_LEV, "Pe[%s]����, ���ٱ�Task[%u]��¼����\n",
                m_tPeURI.GetURIString().c_str(), GetTaskNO());

            //���������Դ
            //ReleaseSource();
            ClearDataWhenPeOfflineOrDisconnect();
            NextState(Idle);
            StartLogin();
            g_cCuiDataMgr.FlushPlatformServiceStatus(GetInstance(), NULL, m_tPeURI);
        }
        break;
    case OSP_SIP_MSG_PROC_FAIL:
        {
            TASKLOG(LOGIN_TASK, ERROR_LEV, "���͸�PE[%s]��SIP��Ϣʧ��, SIP������Ϊ[%d]\n",
                m_tPeURI.GetURIString().c_str(), pcOspSipMsg->GetSipErrorCode());
        }
        break;
    default:
        {
            TASKLOG(LOGIN_TASK, WARNING_LEV, "Receive unkown Msg[%s-%hu]\n", 
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }
    return dwRet;
}

u32 CPeAgent::OnServiceTimer()
{
    return TIMERPOLL_DONE;
}

u32 CPeAgent::StartLogin()
{
    u32 dwRet = PROCMSG_OK;

    if( Idle == GetState())
    {
        if ( PROCMSG_OK != PostMsgReq(m_strReqMsg, m_tPeURI) )
        {
            TASKLOG(LOGIN_TASK, ERROR_LEV, "[CPeAgent::StartLogin]����SIP��Ϣ��[%s]ʧ��!\n",
                m_tPeURI.GetURIString().c_str());

            //��ʱ������
            //dwRet = PROCMSG_DEL;
        }
        else
        {
            TASKLOG(LOGIN_TASK, EVENT_LEV, "[CPeAgent::StartLogin]����SIP��Ϣ��[%s]�ɹ�!\n",
                m_tPeURI.GetURIString().c_str());

            NextState(WaitRegRsp);
            m_strSessionID.clear();
        }
    }
    return dwRet;
}

u32 CPeAgent::StartLogOut()
{
    u32 dwRet = PROCMSG_DEL;

    CDevLogoutReq cReq;
    cReq.SetDevUri(g_cCuiConfig.GetLocalURI().GetURIString());

    if ( PROCMSG_OK != PostMsgReq(m_strReqMsg, m_tPeURI) )
    {
        TASKLOG(LOGIN_TASK, ERROR_LEV, "����Logout��Ϣ��������[%s]ʧ��!\n",
            m_tPeURI.GetURIString().c_str());
    }

    return dwRet;
};

void CPeAgent::PrintData() const
{
    CPeAgentBase::PrintData();
}

s32 GetMobileClientNumber()
{
    return s_nMobileClientNum;
}

bool IsMobileClientType( const string &strClientType )
{
    return (strClientType == "ANDROID_PHONE" || strClientType == "IOS_PHONE" 
        || strClientType == "ANDROID_PAD" || strClientType == "IOS_PAD");
}
