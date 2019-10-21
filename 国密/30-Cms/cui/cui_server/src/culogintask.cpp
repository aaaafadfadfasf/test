/*****************************************************************************
模块名      : cui_fcgi 
文件名      : cuitask.h
相关文件    : 
文件实现功能: cuitask.h 定义所有的事务
作者        : liangli
版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2011/11/9   1.0         liangli        创建
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
			TASKLOG(LOGIN_TASK, EVENT_LEV, "更新移动客户端数量到redis数据库失败\n");
		}
		TASKLOG(LOGIN_TASK, EVENT_LEV, "更新移动客户端数量[%d -- %d]到redis数据库\n",(GetMobileClientNumber()+1),GetMobileClientNumber());
    }

    if (NULL != m_ptNotifyRecvNode)
    {
        // 回复一下用于释放node
        CGetNotifyRsp cRsp;
        SendRspToCu<CGetNotifyRsp>(cRsp, m_ptNotifyRecvNode);
    }

    TASKLOG(LOGIN_TASK, EVENT_LEV, "开始销毁用户[%s]会话[%s]的CCuAgent[TaskNO %u]\n", 
        m_strUserName.c_str(), m_strSession.c_str(), GetTaskNO());

    TSipURI sessionUri( m_strSession );
    CStreamDecentrationAdapter::UnregisterCu( g_cCuiConfig.GetLocalURI().GetUser(), sessionUri.GetUser() );

    C3acAgent *pc3acAgentTask = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(m_str3asName, GetInstance());
    if (NULL != pc3acAgentTask)
    { //tas是在所有CU退出后才销毁，所以不会出现找不到
        CMS_ASSERT( 0 == strcmp(pc3acAgentTask->GetObjName(), "C3acAgent") );
    }
    else
    { //CU登录时TAS不在线，不会创建C3acAgent
        TASKLOG(LOGIN_TASK, CRITICAL_LEV, "警告！在销毁用户[%s]会话[%s]的CCuAgent[TaskNO %u]时，找不到[%s]\n",
            m_strUserName.c_str(), m_strSession.c_str(), GetTaskNO(), m_str3asName.c_str());
        return;
    }


    //这里采用阶梯式释放资源, 没有break
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
					CMS_ASSERT(bRet && "CuAgent消亡时，必须在UserAccount存在登记");
				}

                /*
                某个用户账户可能从用户组退出，也可能加入用户组
                所以CU退出时要立刻检查是否需要删除用户账户
                //2012.3.27 修改了算法和数据结构，CUserAccount从创建后就不会被删除，除非3acAgent被析构
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

            //删除该用户的所有子事务
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
					CMS_ASSERT(bRet && "CuAgent消亡时，必须在UserAccount存在登记");
				}

                /*
                某个用户账户可能从用户组退出，也可能加入用户组
                所以CU退出时要立刻检查是否需要删除用户账户
                //2012.3.27 修改了算法和数据结构，CUserAccount从创建后就不会被删除，除非3acAgent被析构
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
            //TAS退出登录在3acAgent里完成
            if ( NULL != pc3acAgentTask )
            {
                CUserAccount *pcUserAccount = pc3acAgentTask->m_cUserMgr.FindUserAccountByUserName(m_strUserName);
                CMS_ASSERT(NULL != pcUserAccount);
				if (pcUserAccount)
				{
					bool bRet = pcUserAccount->DeleteCuAgent(this);
					CMS_ASSERT(bRet && "CuAgent消亡时，必须在UserAccount存在登记");
				}

                /*
                某个用户账户可能从用户组退出，也可能加入用户组
                所以CU退出时要立刻检查是否需要删除用户账户
                //2012.3.27 修改了算法和数据结构，CUserAccount从创建后就不会被删除，除非3acAgent被析构
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

	//清楚当前session的
	RemoveNatData(m_strSession,0);

	SetTimer(CUI_CLEAR_VTDUNATPACKET_TIMER, CUI_CLEAR_VTDUNATPACKET_INTERVAL);

	g_cuSessionForNatPacketClear.push_back(m_strSession);
	
	// CU退出时清空缓存的离线视频源数据
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

//状态函数
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

            //判断错误码是重复注册
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
                            TASKLOG(LOGIN_TASK, CRITICAL_LEV, "已经发送过登录请求并成功taskID[%d]\n",
                                dwTaskID);
                            CCuOnlineReq cAddCuReq;
                            FillOnlineReq( cAddCuReq, cRsp.GetSession() );

                            FakePostMsgReq(cAddCuReq, pc3acAgentTask->m_t3asURI);

                            NextState(WAIT_3AS_ADD_CU_RSP_STATE);

                            return PROCMSG_OK;
                        }
                        else
                        {
                            TASKLOG(LOGIN_TASK, CRITICAL_LEV, "(1)通过taskID[%d]找不到task\n",
                                dwTaskID);
                        }
                    }
                    else
                    {
                        TASKLOG(LOGIN_TASK, CRITICAL_LEV, "taskID[%d]无效\n",
                            dwTaskID);
                    }
                }
                else
                {
                    TASKLOG(LOGIN_TASK, CRITICAL_LEV, "(2)通过taskID[%d]找不到task\n",
                        dwTaskID);
                }
            }

            if (cRsp.GetErrorCode() != CMS_SUCCESS)
            { //注册3AS失败
                TASKLOG(LOGIN_TASK, ERROR_LEV, "登录TAS失败，错误码[%d], 用户[%s]登录失败\n",
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
                TASKLOG(LOGIN_TASK, ERROR_LEV, "缓存中的URI[%s]与Sip URI[%s]不一致\n",
                    m_str3asName.c_str(), pcOspSipMsg->GetSipFromUri().GetURIString().c_str());
                CCuiCuLoginRsp cCuLoginRsp;
                cCuLoginRsp.SetErrorCode(ERR_CUI_PARAMS_INVALID);
                SendRspToCu<CCuiCuLoginRsp>(cCuLoginRsp, m_ptCuLoginNode);
                WriteLog(ERR_CUI_PARAMS_INVALID, true);
                delete this;
                return PROCMSG_OK;
            }

            //注册3AS成功，创建一个3ASAgent事务，以便后续保持与3AS的注册状态，以及接收3AS的通知
            C3acAgent *pc3acAgentTask = new C3acAgent(GetInstance());
            CMS_ASSERT(NULL != pc3acAgentTask);
            pc3acAgentTask->InitStateMachine();
            pc3acAgentTask->m_t3asURI.SetNOString(m_str3asName);
            g_cCuiDataMgr.m_map3asURITo3asAgentMap.Insert(pc3acAgentTask->m_t3asURI.GetURIString(), pc3acAgentTask->GetTaskNO());   //只在这里登记一次，直到C3acAgent销毁
            //将3AS注册应答消息传送给3ASAgent，以便其记录3AS信息并转换状态
            //由3ASAgent负责对3AS设置心跳, 仿佛就是3ASAgent发送的注册请求得到应答
            pc3acAgentTask->ProcMsg(pcMsg);

            //向3AS添加CU子用户
            CCuOnlineReq cAddCuReq;
            FillOnlineReq( cAddCuReq, cRsp.GetSession() );

            FakePostMsgReq(cAddCuReq, pc3acAgentTask->m_t3asURI);
            TASKLOG(LOGIN_TASK, CRITICAL_LEV, "发送向3AS添加CU子用户请求成功\n");

            NextState(WAIT_3AS_ADD_CU_RSP_STATE);
        }
        break;
    case OSP_SIP_MSG_PROC_FAIL:
        {
            TASKLOG(LOGIN_TASK, ERROR_LEV, "向TAS发送[ CUI登录请求 ]失败，SIP错误码[%d], 用户[%s]登录失败\n",
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
            TASKLOG(LOGIN_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }
    return PROCMSG_OK;
}
u32 CCuAgent::Wait3ASRspTimerPoll()
{
    if ( GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout() )
    { //超时
        TASKLOG(LOGIN_TASK, ERROR_LEV, "等待服务器应答超时，用户[%s]登录失败\n",
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
            { //注册3AS失败
                TASKLOG(LOGIN_TASK, EVENT_LEV, "TAS添加CU在线状态失败，错误码[%d]，用户[%s]登录失败\n",
                    cAddCuRsp.GetErrorCode(), m_strUserName.c_str());
                CCuiCuLoginRsp cCuLoginRsp;
                cCuLoginRsp.SetErrorCode(cAddCuRsp.GetErrorCode());
                SendRspToCu<CCuiCuLoginRsp>(cCuLoginRsp, m_ptCuLoginNode);
                
                //写异常日志
                WriteLog(ERR_CUI_OPR_TIME_OUT, true);

                delete this;
                return PROCMSG_DEL;
            }

			if (ERR_TAS_INPUT_ERROR_PASSWORD_TOO_MUCH == cAddCuRsp.GetErrorCode())
			{
				//用户被锁定
				TASKLOG(LOGIN_TASK, EVENT_LEV, "用户被锁定，错误码[%d]，用户[%s]登录失败\n",
					cAddCuRsp.GetErrorCode(), m_strUserName.c_str());
				CCuiCuLoginRsp cCuLoginRsp;
				cCuLoginRsp.SetErrorCode(cAddCuRsp.GetErrorCode());
				cCuLoginRsp.SetPasswordRemainTime(cAddCuRsp.GetPwdExpireLeftTime());
				cCuLoginRsp.SetUserLockRemainTime(cAddCuRsp.GetPwdLockedLeftTime());
				SendRspToCu<CCuiCuLoginRsp>(cCuLoginRsp, m_ptCuLoginNode);

                m_strLogDesc = GetLanDesc( LAN_CU_PASSWORD_LOCKED );
				//写异常日志
				WriteLog(ERR_TAS_INPUT_ERROR_PASSWORD_TOO_MUCH, true, false);

				delete this;
				return PROCMSG_DEL;
			}

			if ( ERR_TAS_PASSWORD_EXPIRED == cAddCuRsp.GetErrorCode() )
			{
				m_nPwdRemainError = ERR_TAS_PASSWORD_EXPIRED;
				TASKLOG(LOGIN_TASK, EVENT_LEV, "密码已过期！但允许用户[%s]登录\n",m_strUserName.c_str());
			}

            C3acAgent *pc3acAgentTask = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(m_str3asName, GetInstance());
            if (pc3acAgentTask == NULL)
            {
                TASKLOG(LOGIN_TASK, ERROR_LEV, "TAS添加CU在线状态成功，但是3acAgent已经不存在了，用户[%s]登录失败\n",
                    m_strUserName.c_str());
                CCuiCuLoginRsp cCuLoginRsp;
                cCuLoginRsp.SetErrorCode(ERR_CUI_SIP_MSG_FAIL);
                SendRspToCu<CCuiCuLoginRsp>(cCuLoginRsp, m_ptCuLoginNode);
                WriteLog(ERR_CUI_SIP_MSG_FAIL, true);
                delete this;
                return PROCMSG_DEL;
            }
            CMS_ASSERT( 0 == strcmp(pc3acAgentTask->GetObjName(), "C3acAgent") );

            //m_strCuSessionIn3AS = cAddCuRsp.GetCuSessionId();//自己生成，传递给tas使用。
            g_cCuiDataMgr.m_mapSessToTaskMap.Insert(m_strCuSessionIn3AS, GetTaskNO());
            //判断是否已经存在该用户账户
            CUserAccount *pcUserAccount = pc3acAgentTask->m_cUserMgr.FindUserAccountByUserName(m_strUserName);
            if ( NULL == pcUserAccount )
            {
                //先创建一个空的CUserAccount
                pcUserAccount = new CUserAccount(&(pc3acAgentTask->m_cUserMgr));
                pcUserAccount->m_strUserName = m_strUserName;
                pcUserAccount->m_bDevPriState = DEVPRI_NOT_START;
                pc3acAgentTask->m_cUserMgr.m_mapUserName2PCuserAccount.Insert(pcUserAccount->m_strUserName, pcUserAccount);
            }
            CMS_ASSERT(NULL != pcUserAccount);
            //将本Task登记到3acAgent中
            pcUserAccount->AddCuAgent(this);
            m_pcUserAccount = pcUserAccount;    //在这里记录指针，//2012.3.37修改了算法数据结构，CUserAccount从创建后就不会被删除，除非3acAgent被析构
            m_strServerDataVersion = cAddCuRsp.GetDeviceTreeVer();
			m_nPwdLockRemainTime = cAddCuRsp.GetPwdLockedLeftTime();
			m_nPwdExpireRemainTime = cAddCuRsp.GetPwdExpireLeftTime();
            { 
                //向CMU添加CU
                CCuLoginReq cReq;
                cReq.SetUserName(m_strUserName);
                cReq.SetVersion(m_strVersion);
                //cReq.SetUsedCuiIp(m_strHostIp);// 这里应该是用错了吧？应该使用NAT内地址 [1/26/2015 pangubing]
				cReq.SetUsedCuiIp(m_tNatInfo.GetIpAddr());
                cReq.SetIsSupportNA(m_bIsSupportNA);

                FakePostMsgReq(cReq, m_tCmuURI);
                NextState(WAIT_CMU_ADD_CU_RSP_STATE);
            }
        }
        break;
    case OSP_SIP_MSG_PROC_FAIL:
        {
            TASKLOG(LOGIN_TASK, EVENT_LEV, "向TAS发送[ 添加CU在线状态 ]SIP消息超时，SIP错误码[%d]，用户[%s]登录失败\n",
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
            TASKLOG(LOGIN_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
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

            //将CuAgentTask加入到SessionIDToMap
            m_strSession = cRsp.GetSession();
            g_cCuiDataMgr.m_mapSessToTaskMap.Insert(m_strSession, GetTaskNO());

            if ( CMS_SUCCESS != cRsp.GetErrorCode() )
            {
                TASKLOG(LOGIN_TASK, ERROR_LEV, "CU登录CMU失败，错误码[%d]，用户[%s]登录失败\n",
                    cRsp.GetErrorCode(), m_strUserName.c_str());
                //CMU添加CU失败，拒绝CU登录
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
                    TASKLOG(LOGIN_TASK, ERROR_LEV, "最终的IP[%s] 端口[%s]\n",
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
			//从redis中读取下面这个值
						// const string FDB_KEY_PMS_GB_SECURITY_ENABLE = "pms_gb_security_enable"; // 值为字符串enable或disable，key不存在时表示enable
			bool bSecureEnable = true;
			string strEnable;
			EFDB_ErrorCode er = g_cRedisDb.Get(Redis_GB_Security, strEnable);
			TASKLOG(LOGIN_TASK, ERROR_LEV, "获取国标是否开启安全模式标记get(%s) return [%s],err=%d\n", Redis_GB_Security, strEnable.c_str(), er);
			//默认启用，所以只有获取到标记为"disable"才不启用 2018/10/18 zsy
			if (er == EC_OK && strEnable == "disable")
			{
				bSecureEnable = false;
			}
			cCuLoginRsp.SetSecureEnable(bSecureEnable);
            //暂时不修改
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
					TASKLOG(LOGIN_TASK, EVENT_LEV, "更新移动客户端数量到redis数据库失败\n");
				}
				TASKLOG(LOGIN_TASK, EVENT_LEV, "更新移动客户端数量[%d -- %d]到redis数据库\n",(GetMobileClientNumber()-1),GetMobileClientNumber());
			}
            
            TASKLOG(LOGIN_TASK, EVENT_LEV, "CU用户[%s]登录成功，TAS会话ID为[%s],CMU会话ID为[%s]\n",
                m_strUserName.c_str(),m_strCuSessionIn3AS.c_str(),m_strSession.c_str());

            //剔除 session 中的域id
            TSipURI sessionUri( m_strSession );
            CStreamDecentrationAdapter::RegisterCu( g_cCuiConfig.GetLocalURI().GetUser(), sessionUri.GetUser(),
				m_strUserName, m_strClientIp, m_tNatInfo.GetIpAddr() );

            NextState(ONLINE_STATE);

            //创建task进行用户信息获取(包括设备权限信息)
            CGetUserInfo* pcGetUserInfo = new CGetUserInfo(GetInstance(), m_str3asName, m_strUserName, m_strSession);
            pcGetUserInfo->InitStateMachine();
            pcGetUserInfo->StartQryUserInfo();

            //WriteLog(cRsp.GetErrorCode(), true);
        }
        break;
    case OSP_SIP_MSG_PROC_FAIL:
        {
            TASKLOG(LOGIN_TASK, ERROR_LEV, "向CMU发送[ CU登录请求 ]SIP消息失败，SIP错误码[%d]，用户[%s]登录失败\n",
                pcOspSipMsg->GetSipErrorCode(), m_strUserName.c_str());
            //CMU添加CU失败，拒绝CU登录
            CCuiCuLoginRsp cTmpRsp;
            cTmpRsp.SetErrorCode(ERR_CUI_SIP_MSG_FAIL);
            SendRspToCu<CCuiCuLoginRsp>(cTmpRsp, m_ptCuLoginNode);
            WriteLog(ERR_CUI_SIP_MSG_FAIL, true);
            delete this;
        }
        break;
    default:
        {
            TASKLOG(LOGIN_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
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
	
	string strDevLogDesc = GetLanDesc(LAN_DESC_MOD) //"修改"
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

//提供的对外方法
u32 CCuAgent::StartLogin(const string &strUserName, const string &strPasswd, const string &strVersion)
{
    CMS_ASSERT(string::npos != strUserName.find('@'));
    m_strUserName = strUserName;
  //  std::transform(m_strUserName.begin(), m_strUserName.end(), m_strUserName.begin(), ::tolower);
	//MD5(strPasswd, m_strPassWord);//由于加入了安全 SM3_Base64 算法，又要兼容老的明文密码，所以cui里都透传，fcgi那边会根据新的接口判断是透传还是先md5加密 2018/10/24 zsy
	m_strPassWord = strPasswd;
    m_strVersion = strVersion;
    TSipURI tUserURI;
    tUserURI.SetNOString(m_strUserName);
    m_str3asName = tUserURI.Get3ASURI().GetURIString();
    TASKLOG(LOGIN_TASK, EVENT_LEV, "收到CU用户[%s]的登录请求，CUI分配CuAgent的TaskNO为[%u]\n", 
        strUserName.c_str(), GetTaskNO());

    //1 首先判断CUI是否向3AS注册过
    C3acAgent *pc3acAgentTask = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(m_str3asName, GetInstance());
    if ( NULL != pc3acAgentTask )
    {
        //有可能3AS断链
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
        TASKLOG(LOGIN_TASK, EVENT_LEV, "TAS[%s]已经建立了连接，直接添加CU在线状态\n",
            m_str3asName.c_str());
        //向3AS添加CU子用户
        CCuOnlineReq cAddCuReq;
        FillOnlineReq( cAddCuReq, pc3acAgentTask->m_strSessionID );

        TSipURI t3asURI;
        t3asURI.SetNOString(m_str3asName);
        FakePostMsgReq(cAddCuReq, t3asURI);
        pc3acAgentTask->FlashTimePoll();

        NextState(WAIT_3AS_ADD_CU_RSP_STATE);
    }
    else
    { //向3AS注册
        TASKLOG(LOGIN_TASK, EVENT_LEV, "TAS[%s]尚未建立连接，本CUI先向TAS注册\n",
            m_str3asName.c_str());
        CLoginTasReq cLogin3ASReq;
        cLogin3ASReq.SetUserName(OspSipGetLocalURI().GetURIString());
        cLogin3ASReq.SetLoginType(CLoginTasReq::enLoginType_CUI);

		//设置需要获取的3as的资源类型集合
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
    TASKLOG(LOGIN_TASK, EVENT_LEV, "CU用户[%s]收到服务器消息[%s][%hu]\n",
        m_strUserName.c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

    switch ( pcMsg->event )
    {
    case OSP_SIP_MSG_PROC_FAIL:
        {
            TASKLOG(LOGIN_TASK, CRITICAL_LEV, "CCuAgent[TaskNO %u]收到消息[%s][%hu]，未期望的消息\n",
                GetTaskNO(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
	case UPDATE_CULOGIN_INFO_RSP:
		{
			TASKLOG(LOGIN_TASK, CRITICAL_LEV, "CCuAgent[TaskNO %u]收到消息应答[%s][%hu]\n",
				GetTaskNO(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
		}
		break;
    default:
        {
            TASKLOG(LOGIN_TASK, CRITICAL_LEV, "CCuAgent[TaskNO %u]收到未知的消息[%s][%hu]\n\n",
                GetTaskNO(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"[CCuAgent::OnlineProcMsg]未知通知消息类型");
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
    { //计数超时
        WriteLog(ERR_CUI_OPR_TIME_OUT, false);
        TASKLOG(LOGIN_TASK, ERROR_LEV, "CU用户[%s]会话[%s]心跳超时[%u > %u]，强制退出登录\n",
            m_strUserName.c_str(), m_strSession.c_str(), tHoldTime, g_cCuiConfig.GetCuHbTimeout());
        //销毁CuAgent，自动向CMU发送Cu Logout Req
        return TIMERPOLL_DEL;
    }

    return TIMERPOLL_DONE;
}

u32 CCuAgent::CuHBAndGetNotify( PTRecvNode ptNode, s32 nTimeoutSeconds )
{
    TASKLOG(LOGIN_TASK, TIMER_LEV, "收到CU用户[%s]会话[%s]的心跳消息\n",
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
            // 等待超时
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
    // 清除subscribe, invite, callpu  hzytodo  到时在设备删除的地方调用每个cuagent的此方法
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
    // 获取IP列表
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
    NextState(ONLINE_STATE);    //将当前状态清零
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

    //将消息池中的Ntf拷贝到新消息中
    m_cNotifyPool.PopNotifyPool(cRsp.notifyPool, 20);
    TASKLOG(LOGIN_TASK, TIMER_LEV, "本次发送给CU[%s]有[%d]条通知消息，剩余通知条数[%d]\n",
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

	// 如果客户端自己填写了平台NAT，则平台不予判断NAT [1/26/2015 pangubing]
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
			 //当含有二层NAT时，平台不能同时把一个端口（8001）映射到第一层和第二层
			 //这里直接使用cu的登陆ip，即为平台在的最外层映射IP
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
		//向CMU修改CU登录信息
		cReq.SetUserName(m_strUserName);
		cReq.SetVersion(m_strVersion);
		cReq.SetUsedCuiIp(m_tNatInfo.GetIpAddr());
		cReq.SetIsSupportNA(m_bIsSupportNA);

		FakePostMsgReq(cReq, m_tCmuURI);
	}

	TASKLOG(LOGIN_TASK, EVENT_LEV, "CU用户[%s]更新属性成功，CU NAT[%s-%s-%d],平台NAT[%s-%s-%d]\n",m_strUserName.c_str(),
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
    //4，从会话映射表中删除此事务
    // 2012.3.29 修改逻辑，不删除TAS的URI登记，即使TAS不在线，也可以查询得到
    // TAS不在线需要通过其他途径判断
    // 只有当所有CU都下线了，才能销毁TAS
    // g_cCuiDataMgr.m_map3asURITo3asAgentMap.Erase(m_t3asURI.GetURIString());
    g_cCuiDataMgr.m_mapSessToTaskMap.Erase(m_strSessionID);
    m_strSessionID.clear();

    //5，从会话映射表中删除所有的QueryTag
    vector<string>::iterator pItem = m_vctTmpQueryTag.begin();
    while ( pItem != m_vctTmpQueryTag.end() )
    {
        g_cCuiDataMgr.m_mapSessToTaskMap.Erase(*pItem);
        pItem++;
    }
    m_vctTmpQueryTag.clear();

    //6，清空所有的CuAgent的TAS Session
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
                //pcCuAgentTask->m_pcUserAccount = NULL;  //TAS断链后CuAgent仍然需要
            }

            pcCuAgentItem++;
        }
    }


}

void C3acAgent::TerminateTask()
{
    TASKLOG(LOGIN_TASK, EVENT_LEV, "收到登出TAS[%s]请求\n",
        m_t3asURI.GetURIString().c_str());
    OspPrintf(TRUE, FALSE, "!!!!!!!!!!!!!!!退出[%s]！\n",
        m_t3asURI.GetURIString().c_str());

    if ( GetState() == ONLINE_STATE )
    {
        //1, 登出3as
        CLogoutTasReq cReq;
        cReq.SetSession(m_strSessionID);

        FakePostMsgReq(cReq, m_t3asURI);
        //2，取消心跳检测
        FakeStopHB(m_t3asURI);
    }

    //3, 清空成员
    //3AS断链后，已经登录的CU不受影响, 所以不强迫3AS下的CU退出
    ClearDataWhenTASOfflineOrDisconnect();

    //只有当TAS销毁时，才删除URI登记
    g_cCuiDataMgr.m_map3asURITo3asAgentMap.Erase(m_t3asURI.GetURIString());

    NextState(OFFLINE_STATE);
}

//状态函数
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

            //将此3ASAgent登记到全局列表中, 注意：这里记录的是TAC的SessionID，所以在OnService()中不能根据SessionID查找CuAgent
            g_cCuiDataMgr.m_mapSessToTaskMap.Insert(cRsp.GetSession(), GetTaskNO());

            TASKLOG(LOGIN_TASK, EVENT_LEV, "收到TAS的登录应答, 登录TAS[%s]的会话为[%s], 进入[Online]状态\n",
                m_t3asURI.GetURIString().c_str(), m_strSessionID.c_str());

            FakeSetHBParam(m_t3asURI);
            NextState(ONLINE_STATE);

            //向3AS查询其所在域的ID和名称
            CDomainNameGetReq cDomainReq;
            cDomainReq.SetSession(m_strSessionID);
            FakePostMsgReq(cDomainReq, m_t3asURI);

			//向3AS查询有PTZ锁的视频源
			m_cPtzlockInfoList.Clear();
			CPtzLockQryReq cPtzLockQryReq;
			cPtzLockQryReq.SetSession(m_strSessionID);
			FakePostMsgReq(cPtzLockQryReq, m_t3asURI);
        }
        break;
    case OSP_SIP_MSG_PROC_FAIL:
        {
            //在CuAgent里处理过了
        }
        break;
    default:
        TASKLOG(LOGIN_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
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
            TASKLOG(LOGIN_TASK, EVENT_LEV, "TAS[%s]断链, 销毁本Task[%u]登录事务\n",
                m_t3asURI.GetURIString().c_str(), GetTaskNO());
            NextState(OFFLINE_STATE);

            //3，销毁事务
            delete this;
        }
        break;
    case OSP_SIP_MSG_PROC_FAIL:
        {
            TASKLOG(LOGIN_TASK, ERROR_LEV, "发送给TAS[%s]的SIP消息失败, SIP错误码为[%d]\n",
                m_t3asURI.GetURIString().c_str(), pcOspSipMsg->GetSipErrorCode());
        }
        break;
    case TAS_DOMAIN_NAME_GET_RSP:
        {
            CDomainNameGetRsp cDomainRsp;
            pcOspSipMsg->GetMsgBody(cDomainRsp);
            m_str3ASDomainName = cDomainRsp.GetDomainName();
            m_t3ASDomainID = cDomainRsp.GetDomainId();

            TASKLOG(LOGIN_TASK, EVENT_LEV, "收到TAS[%s]的获取域名应答，本域域名[%s]域ID[%s]\n",
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
				TASKLOG(LOGIN_TASK, EVENT_LEV, "没有视频源存在PTZ锁!\n");
			}
			else
			{
				TASKLOG(LOGIN_TASK, EVENT_LEV, "PTZ锁定列表查询的QueryTag为[%s],查询到的总数为%d, 将本Task[%s--%u]登记到会话表中\n",
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
			
			//先回复NTF通知
			CNtfRsp cRsp;
			cRsp.SetEvent(pcMsg->event + 1);
			cRsp.SetSeqNum(cNtfReq.GetSeqNum());
			cRsp.SetSession(cNtfReq.GetSession());
			PostMsgRsp(pcOspSipMsg->GetSipTransID(), cRsp);
			//判断是否是最后一个Ntf, 如果是则删除m_mapSessToTaskMap中的映射
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
					TASKLOG(LOGIN_TASK, EVENT_LEV, "添加PTZ锁信息失败？\n");
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
				TASKLOG(LOGIN_TASK, ERROR_LEV, "CUI删除过期锁失败,不应该!CPtzLockDelRsp[%s]\n", cRsp.ToXml().c_str());
			}
		}
		break;
    default:
        {
            TASKLOG(LOGIN_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
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

            //将此3ASAgent登记到全局列表中, 注意：这里记录的是TAC的SessionID，所以在OnService()中不能根据SessionID查找CuAgent
            g_cCuiDataMgr.m_mapSessToTaskMap.Insert(cRsp.GetSession(), GetTaskNO());

            TASKLOG(LOGIN_TASK, EVENT_LEV, "收到TAS的登录应答, 登录TAS[%s]的会话为[%s], 进入[Online]状态\n",
                m_t3asURI.GetURIString().c_str(), m_strSessionID.c_str());

            FakeSetHBParam(m_t3asURI);
            NextState(ONLINE_STATE);

            //向3AS查询其所在域的ID和名称
            CDomainNameGetReq cDomainReq;
            cDomainReq.SetSession(m_strSessionID);
            FakePostMsgReq(cDomainReq, m_t3asURI);

			//向3AS查询有PTZ锁的视频源
			m_cPtzlockInfoList.Clear();
			CPtzLockQryReq cPtzLockQryReq;
			cPtzLockQryReq.SetSession(m_strSessionID);
			FakePostMsgReq(cPtzLockQryReq, m_t3asURI);
        }
        break;
    case OSP_SIP_MSG_PROC_FAIL:
        {
            //会在CReloginTask中处理
        }
        break;
    default:
        TASKLOG(LOGIN_TASK, EVENT_LEV, "[C3acAgent::ReloginProcMsg]收到未知消息[%s--%hu]\n",
            OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        break;
    }
    return PROCMSG_OK;
}

u32 C3acAgent::ReloginTimerPoll()
{
    if ( GetCurStateHoldTime() > TACAGENT_EMPTY_USERACCOUNT_CHECK_COUNT ) //每5秒检测一次
    {
        if ( CheckUserOnline() == 0 )
        {
            TASKLOG(LOGIN_TASK, EVENT_LEV, "TAS[%s]所在域没有CU登录，自动断开与TAS的连接\n",
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
函数名      : CheckUserOnline
功能        : 检查用户账户是否有对应的CU在线
算法实现    : CUserAccount即使所有CU都不在线了，也不会删除，除非C3acAgent被析构
避免引用空指针
引用全局变量:
输入参数说明:
返回值说明  : 0表示所有账户都不在线, >0表示还有账户在线
----------------------------------------------------------------------
修改记录    :
日  期      版本        修改人        修改内容
2012/1/31/             liangli
====================================================================*/
u32 C3acAgent::CheckUserOnline()
{
    u32 dwCuOnlineNum = 0;
    //if ( m_dwRefCounter == 0 )
    {
        //检查用户
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
    //如果用户列表都为空, 则退出3AS
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

//获取当前3acAgent的所有CU在线任务集合
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
		TASKLOG(LOGIN_TASK, ERROR_LEV, "获取域信息失败，ec[%d]，ErrMsg[%s]\n",
			ec, strErrMsg.c_str());
		NextState(GetState()); //当前状态重新计时
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
                TASKLOG(LOGIN_TASK, ERROR_LEV, "收到登录TAS[%s]应答，流水号[%d]与请求值[%d]不匹配，忽略此消息",
                    pcOspSipMsg->GetSipToUri().GetURIString().c_str(), cRsp.GetSeqNum(), m_nReqSeqNum);
                return PROCMSG_OK;
            }

            if (cRsp.GetErrorCode() != CMS_SUCCESS)
            { //注册3AS失败
                TASKLOG(LOGIN_TASK, ERROR_LEV, "CReLoginTasTask登录TAS[%s]失败，错误码[%d], 尝试重复登录",
                    pcOspSipMsg->GetSipToUri().GetURIString().c_str(), cRsp.GetErrorCode());
                NextState(LOGIN_TAS_STATE);
                return PROCMSG_OK;
            }

            //注册3AS成功，创建一个3ASAgent事务，以便后续保持与3AS的注册状态，以及接收3AS的通知
            C3acAgent *pc3acAgentTask = (C3acAgent *)(GetInstance()->FindTask(m_dw3acAgentTask));
            if ( pc3acAgentTask != NULL )
            {
                CMS_ASSERT( 0 == strcmp(pc3acAgentTask->GetObjName(), "C3acAgent") );
                //将3AS注册应答消息传送给3ASAgent，以便其记录3AS信息并转换状态
                //由3ASAgent负责对3AS设置心跳, 仿佛就是3ASAgent发送的注册请求得到应答
                pc3acAgentTask->ProcMsg(pcMsg);
            }
            else
            {
                //3acAgent已经因为CU全部下线而销毁，可以删除本任务
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
            TASKLOG(LOGIN_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
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
            //3acAgent已经因为CU全部下线而销毁，可以删除本任务
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
                TASKLOG(LOGIN_TASK, ERROR_LEV, "收到登录TAS[%s]应答，流水号[%d]与请求值[%d]不匹配，忽略此消息",
                    pcOspSipMsg->GetSipToUri().GetURIString().c_str(), cAddCuRsp.GetSeqNum(), m_nReqSeqNum);
                return PROCMSG_OK;
            }

            if (cAddCuRsp.GetErrorCode() != CMS_SUCCESS)
            { //注册3AS失败
                TASKLOG(LOGIN_TASK, EVENT_LEV, "CReLoginTasTask向TAS添加CU在线状态失败，错误码[%d]\n",
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
                    TASKLOG(LOGIN_TASK, EVENT_LEV, "CReLoginTasTask向TAS添加CU[%s]在线状态成功\n",
                        pcCuAgentTask->m_strUserName.c_str());
                    pcCuAgentTask->m_strCuSessionIn3AS = cAddCuRsp.GetCuSessionId();
                    g_cCuiDataMgr.m_mapSessToTaskMap.Insert(pcCuAgentTask->m_strCuSessionIn3AS, pcCuAgentTask->GetTaskNO());
                }
            }

            m_tCuAgentIds.pop();
            //继续处理下一个CuAgent
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
            TASKLOG(LOGIN_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
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
    TASKLOG(LOGIN_TASK, EVENT_LEV, "TAS[%s]尚未建立连接，CReLoginTasTask替本CUI先向TAS注册\n",
        m_t3asURI.GetURIString().c_str());
    CLoginTasReq cLogin3ASReq;
    cLogin3ASReq.SetUserName(OspSipGetLocalURI().GetURIString());
    cLogin3ASReq.SetLoginType(CLoginTasReq::enLoginType_CUI);

	//设置需要获取的3as的资源类型集合
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
    //找到第一个在线的CU
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
            TASKLOG(LOGIN_TASK, EVENT_LEV, "TAS[%s]已经建立了连接，CReLoginTasTask直接添加CU[%s]在线状态\n",
                m_t3asURI.GetURIString().c_str(), GetNameWithoutDomainFromURI(pcCuAgent->m_strUserName).c_str());
            C3acAgent *pc3acAgentTask = (C3acAgent *)(GetInstance()->FindTask(m_dw3acAgentTask));
            if ( NULL == pc3acAgentTask )
            {
                delete this;
                return PROCMSG_OK;
            }
            CMS_ASSERT(0 == strcmp(pc3acAgentTask->GetObjName(), "C3acAgent"));
            //向3AS添加CU子用户
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
            //CU已经下线
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
	//析构里面就不做Msg应答处理了
	m_VecNtfReqList.clear();
    //while(!m_VecNtfReqList.empty())
    //{
    //    string strNtfMsg = m_VecNtfReqList.front();
    //    m_VecNtfReqList.pop_front();
    //    COspSipMsg *pcOspSipMsg = (COspSipMsg *)strNtfMsg.c_str();
    //    TASKLOG(LOGIN_TASK, EVENT_LEV, "有未处理的NTF消息[%s],直接回复\n", OspExtEventDesc(pcOspSipMsg->GetOspEventID()).c_str());
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
        TASKLOG(LOGIN_TASK, ERROR_LEV, "[CGetUserInfo]3acAgent已经不存在了,消亡该Task!\n");       
        return PROCMSG_DEL;
    }
    CMS_ASSERT( 0 == strcmp(pc3acAgentTask->GetObjName(), "C3acAgent") );

    CUserAccount* pcUserAccount = pc3acAgentTask->m_cUserMgr.FindUserAccountByUserName(m_strUserName);
    if(NULL == pcUserAccount)
    {
        TASKLOG(LOGIN_TASK, ERROR_LEV, "[CGetUserInfo]UserAccount已经不存在了,消亡该Task!\n");       
        return PROCMSG_DEL;
    }

    //向3AS发送查询用户请求，以便更新用户信息
	CUserQryReq cQryReq;
	cQryReq.SetStartEntryIndex(0);
	cQryReq.SetExpectEntryNum(1);
    cQryReq.SetSession(pc3acAgentTask->m_strSessionID);
    CUserInfoQueryCondition userInfoQueryCondition;
    userInfoQueryCondition.SetUserName(GetNameWithoutDomainFromURI(pcUserAccount->m_strUserName));
    cQryReq.SetUserInfoQueryCondition(userInfoQueryCondition);

    TASKLOG(LOGIN_TASK, EVENT_LEV, "以会话ID[%s]向TAS[%s]更新用户[%s]账户信息\n",
        pc3acAgentTask->m_strSessionID.c_str(), pc3acAgentTask->m_t3asURI.GetURIString().c_str(), pcUserAccount->m_strUserName.c_str());

    FakePostMsgReq(cQryReq, pc3acAgentTask->m_t3asURI);
    if (!m_strQueryTag.empty())
    {
        g_cCuiDataMgr.m_mapSessToTaskMap.Erase(m_strQueryTag);
    }

	//新的查询请求发出时，清空保存的通知请求。
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
    TASKLOG(LOGIN_TASK, EVENT_LEV, "CGetUserInfo[TaskNO %u]收到服务器[%s]的应答消息[%s--%hu]\n",
        GetTaskNO(), pcOspSipMsg->GetSipToUri().GetURIString().c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

    switch (pcMsg->event)
    {
    case OSP_SIP_MSG_PROC_FAIL:
        {
            TASKLOG(LOGIN_TASK, EVENT_LEV, "收到[OSP_SIP_MSG_PROC_FAIL]消息\n");
        }
        break;
    case OSP_SIP_DIALOG_TERMINATE:
        {
            CMS_ASSERT( 0 && "CQueryTask::WaitRspProcMsg不可能出现会话内消息" );
        }
        break;
    case TAS_CUSER_QRY_RSP:
        {
            CUserQryRsp cQryRsp;
            pcOspSipMsg->GetMsgBody(cQryRsp);
            if ( CMS_SUCCESS != cQryRsp.GetErrorCode() )
            {
                TASKLOG(LOGIN_TASK, ERROR_LEV, "GetUserInfo在用户登录时查询用户信息失败，错误码[%d]\n",
                    cQryRsp.GetErrorCode());
                delete this;
                return PROCMSG_DEL;
            }
            CMS_ASSERT( 1 == cQryRsp.GetTotalEntryNum() );
            TASKLOG(LOGIN_TASK, EVENT_LEV, "GetUserInfo信息查询的QueryTag为[%s], 将本Task[%s--%u]登记到会话表中\n", 
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
                TASKLOG(LOGIN_TASK, CRITICAL_LEV, "在[WAIT_USER_INFO_RESPONSE_STATE]收到相应的Ntf消息[%s--%hu]\n",
                    OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            }           
        }
        break;
    default:
        {
            TASKLOG(LOGIN_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"收到未知消息");
        }
        break;
    }
    return PROCMSG_OK;
}

u32 CGetUserInfo::WaitRspTimerPoll()
{
    if ( GetCurStateHoldTime() > g_cCuiConfig.GetDbQueryTimeout() )
    {   
        TASKLOG(LOGIN_TASK, ERROR_LEV, "CUI[%s]用户名[%s]等待应答消息[TAS_CUSER_DEVICE_QRY_RSP]超时\n", 
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
            CMS_ASSERT( 0 && "CQueryTask::WaitNotifyProcMsg不可能出现SIP异常" );
        }
        break;
    case TAS_CUSER_QRY_NTF_REQ:
        {
            CUserQryNtfReq cNtfReq;
            pcOspSipMsg->GetMsgBody(cNtfReq);
            CMS_ASSERT(cNtfReq.GetEntryNum() == 1);
            CMS_ASSERT(cNtfReq.IsLastNtf());

            //先回复NTF通知
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
                TASKLOG(LOGIN_TASK, ERROR_LEV, "[CGetUserInfo]3acAgent已经不存在了,消亡该Task!\n");       
                delete this;
                return PROCMSG_DEL;
            }
            CMS_ASSERT( 0 == strcmp(pc3acAgentTask->GetObjName(), "C3acAgent") );

            CUserAccount* pcUserAccount = pc3acAgentTask->m_cUserMgr.FindUserAccountByUserName(m_strUserName);
            if(NULL == pcUserAccount)
            {
                TASKLOG(LOGIN_TASK, ERROR_LEV, "[CGetUserInfo]UserAccount已经不存在了,消亡该Task!\n");       
                delete this;
                return PROCMSG_DEL;
            }

            pcUserAccount->SetUserAccountBaseInfo(cUserInfo, pcUserAccount->m_strUserName);
            pc3acAgentTask->m_cUserMgr.m_mapUserID2UserName.Insert(cUserInfo.GetUserID(), pcUserAccount->m_strUserName);

            bool bIsInGroup = false;

            //如果用户属于组，则更新用户组信息
            if ( !cUserInfo.GetUsergrpID().empty())
            {
                CUserGroup *pcUserGroup = pc3acAgentTask->m_cUserMgr.FindUserGroupByGroupID(cUserInfo.GetUsergrpID());
                //用户是否在组中
                bIsInGroup = true;
                //如果有该组信息则不进行查询
                if ( pcUserGroup == NULL )
                { //如果用户组不存在，则先创建一个空组
                    CUserGroup cUserGroup(&pc3acAgentTask->m_cUserMgr);
                    cUserGroup.m_tGroupID = pcUserAccount->m_tUserGroupID;
                    cUserGroup.m_bDevPriState = DEVPRI_NOT_START;

                    //获取组信息
                    pc3acAgentTask->m_cUserMgr.m_mapGroupID2UserGroup.Insert(cUserGroup.m_tGroupID, cUserGroup);

                    CGetGroupInfo *pcTask = new CGetGroupInfo(GetInstance(), m_str3asName, pcUserAccount->m_tUserGroupID);
                    pcTask->InitStateMachine();
                    pcTask->StartQryGroupInfo();
                }
            }

            //清除本次查询
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
            TASKLOG(LOGIN_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"收到未知消息");
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
        TASKLOG(LOGIN_TASK, ERROR_LEV, "CGetUserInfo[NO: %u]等待NTF消息[TAS_CUSER_DEVICE_QRY_NTF_RSP]超时\n",
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
        TASKLOG(LOGIN_TASK, EVENT_LEV, "有未处理的NTF消息[%s],直接回复\n", OspExtEventDesc(pcOspSipMsg->GetOspEventID()).c_str());
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
        TASKLOG(LOGIN_TASK, ERROR_LEV, "[CGetGroupInfo]3acAgent已经不存在了,消亡该Task!\n");       
        return PROCMSG_DEL;
    }
    CMS_ASSERT( 0 == strcmp(pc3acAgentTask->GetObjName(), "C3acAgent") );

    CUserGroup* pcUserGroup = pc3acAgentTask->m_cUserMgr.FindUserGroupByGroupID(m_strGroupID);
    if(NULL == pcUserGroup)
    {
        TASKLOG(LOGIN_TASK, ERROR_LEV, "[CGetGroupInfo]UserAccount已经不存在了,消亡该Task!\n");       
        return PROCMSG_DEL;
    }

    //向3AS发送查询用户请求，以便更新用户信息
    CUserGroupQryReq cQryReq;
    cQryReq.SetSession(pc3acAgentTask->m_strSessionID);
    CUserGroupInfoQueryCondition userGroupInfoQueryCondition;
    userGroupInfoQueryCondition.SetGroupNO(m_strGroupID);
    cQryReq.SetUserGroupInfoQueryCondition(userGroupInfoQueryCondition);

    TASKLOG(LOGIN_TASK, EVENT_LEV, "CUI会话ID[%s]向TAS[%s]更新用户组[%s]账户信息\n",
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
    TASKLOG(LOGIN_TASK, EVENT_LEV, "CGetGroupInfo[TaskNO %u]收到服务器[%s]的应答消息[%s--%hu]\n",
        GetTaskNO(), pcOspSipMsg->GetSipToUri().GetURIString().c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

    switch (pcMsg->event)
    {
    case OSP_SIP_MSG_PROC_FAIL:
        {
            TASKLOG(LOGIN_TASK, EVENT_LEV, "收到[OSP_SIP_MSG_PROC_FAIL]消息\n");
        }
        break;
    case OSP_SIP_DIALOG_TERMINATE:
        {
            CMS_ASSERT( 0 && "CGetGroupInfo::WaitRspProcMsg不可能出现会话内消息" );
        }
        break;
    case TAS_CUSER_GROUP_QRY_RSP:
        {
            CUserGroupQryRsp cQryRsp;
            pcOspSipMsg->GetMsgBody(cQryRsp);
            if ( CMS_SUCCESS != cQryRsp.GetErrorCode() )
            {
                TASKLOG(LOGIN_TASK, ERROR_LEV, "[CGetGroupInfo]在用户登录时查询用户信息失败，错误码[%d]\n",
                    cQryRsp.GetErrorCode());
                delete this;
                return PROCMSG_DEL;
            }
            CMS_ASSERT( 1 == cQryRsp.GetTotalEntryNum() );
            TASKLOG(LOGIN_TASK, EVENT_LEV, "[CGetGroupInfo]信息查询的QueryTag为[%s], 将本Task[%s--%u]登记到会话表中\n", 
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
                TASKLOG(LOGIN_TASK, CRITICAL_LEV, "在[WAIT_USER_INFO_RESPONSE_STATE]收到相应的Ntf消息[%s--%hu]\n",
                    OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            }           
        }
        break;
    default:
        {
            TASKLOG(LOGIN_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"收到未知消息");
        }
        break;
    }
    return PROCMSG_OK;
}

u32 CGetGroupInfo::WaitRspTimerPoll()
{
    if ( GetCurStateHoldTime() > g_cCuiConfig.GetDbQueryTimeout() )
    {   
        TASKLOG(LOGIN_TASK, ERROR_LEV, "CUI[%s]用户组ID[%s]等待应答消息[TAS_CUSER_GROUP_QRY_RSP]超时\n", 
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
            CMS_ASSERT( 0 && "CGetGroupInfo::WaitNotifyProcMsg不可能出现SIP异常" );
        }
        break;
    case TAS_CUSER_GROUP_QRY_NTF_REQ:
        {
            CUserGroupQryNtfReq cNtfReq;
            pcOspSipMsg->GetMsgBody(cNtfReq);
            CMS_ASSERT(cNtfReq.GetEntryNum() == 1);
            CMS_ASSERT(cNtfReq.IsLastNtf());

            //先回复NTF通知
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
                TASKLOG(LOGIN_TASK, ERROR_LEV, "[CGetGroupInfo]3acAgent已经不存在了,消亡该Task!\n");       
                delete this;
                return PROCMSG_DEL;
            }
            CMS_ASSERT( 0 == strcmp(pc3acAgentTask->GetObjName(), "C3acAgent") );

            CUserGroup* pcUserGroup = pc3acAgentTask->m_cUserMgr.FindUserGroupByGroupID(m_strGroupID);
            if(NULL == pcUserGroup)
            {
                TASKLOG(LOGIN_TASK, ERROR_LEV, "[CGetGroupInfo]UserAccount已经不存在了,消亡该Task!\n");       
                delete this;
                return PROCMSG_DEL;
            }

            //更新组基本信息
            pcUserGroup->SetUserGroupBaseInfo(cUserGroupInfo);

            //清除本次查询
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
            TASKLOG(LOGIN_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"收到未知消息");
        }
        break;
    }
    return PROCMSG_OK;
}

u32 CGetGroupInfo::WaitNotifyTimerPoll()
{
    if ( GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout() )
    {
        TASKLOG(LOGIN_TASK, ERROR_LEV, "CGetGroupInfo[NO: %u]等待NTF消息[TAS_CUSER_GROUP_QRY_NTF_REQ]超时\n",
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

    //子类需要做登出操作
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
        //后续统一用一个task表保存
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
                    TASKLOG(LOGIN_TASK, CRITICAL_LEV, "找不到CCuAgent的CSubBaseTask[NO: %u]的子任务\n", dwTaskId);
                    CMS_ASSERT(0 && "找不到CCuAgent的CSubscribeTask子任务");
                }
            }
        }

        //不能清空，由于订阅不删除
        //m_cSubscribeTaskSet.Empty();    //注意，CSubscribeTask的TerminateTask函数中也会删除，重复删没有问题
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
                    //由CInviteTask自己删除自己，并且注销CuAgent中的登记
                    // 2012.3.13 不能等InviteByeRsp再删除，因为此时CuAgent已经销毁
                }
                else
                {
                    TASKLOG(LOGIN_TASK, CRITICAL_LEV, "找不到CCuAgent的CInviteTask[NO: %u]的子任务\n",
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
                    TASKLOG(LOGIN_TASK, CRITICAL_LEV, "找不到CCuAgent的CInviteTask[NO: %u]的子任务\n",
                        dwTaskId);
                }
            }
        }
        m_cFileExportTaskSet.Empty();
    }
    //清楚callback中残存的
    m_tCallBackTaskList.clear();


	if (!m_setDpsSearchTaskNo.empty())
	{
		// 这个地方要复制一份，否则 task销毁时会删除原有set，导致崩溃。
		set<u32> tSetDpsSearchTaskNo = m_setDpsSearchTaskNo;

		for (set<u32>::const_iterator taskNoIter = tSetDpsSearchTaskNo.begin();
		taskNoIter != tSetDpsSearchTaskNo.end(); taskNoIter++)
		{
			CDpsSearchTask *ptTask = (CDpsSearchTask*)(GetInstance()->FindTask(*taskNoIter));
			if (ptTask != NULL)
			{
				TASKLOG(LOGIN_TASK, CRITICAL_LEV, "删除CCuAgent的CDpsSearchTask[NO: %u]的子任务\n", *taskNoIter);
				delete ptTask;
				ptTask = NULL;
			}
			else
			{
				TASKLOG(LOGIN_TASK, CRITICAL_LEV, "找不到CCuAgent的CDpsSearchTask[NO: %u]的子任务\n",
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
        TASKLOG(LOGIN_TASK, EVENT_LEV, "找到task，立马处理消息时间！\n");
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
        TASKLOG(LOGIN_TASK, WARNING_LEV,"在[CPeAgent::OnIdle]向[%s]注册失败, 稍后尝试重新注册\n",
            m_tPeURI.GetURIString().c_str());
        return PROCMSG_FAIL;
    }

    TASKLOG(LOGIN_TASK, CRITICAL_LEV, "在[CPeAgent::OnIdle]收到未知消息[%s--%hu]，不处理\n",
        OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

    CMS_ASSERT(0&&"CPeAgent::OnIdle收到未知消息");
    return PROCMSG_OK;
}

u32 CPeAgent::OnIdleTimer()
{
    if (GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout())
    {
        TASKLOG(LOGIN_TASK, TIMER_LEV, "尝试向服务器[%s]注册\n", 
            m_tPeURI.GetURIString().c_str());

        StartLogin();
    }

    return TIMERPOLL_DONE;
}

u32 CPeAgent::OnWaitRegRsp(CMessage *const pcMsg)
{
    TASKLOG(LOGIN_TASK, EVENT_LEV, "在[CPeAgent::OnWaitRegRsp]收到消息[%s--%hu]\n",
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
                //等待定时器重新连接服务器
                TASKLOG(LOGIN_TASK, ERROR_LEV, "注册[%s]失败，错误码 %d, 稍后尝试重新注册\n",
                    m_tPeURI.GetURIString().c_str(), dwErrorCode);
                NextState(Idle);
            }
            dwRet = PROCMSG_OK;
        }
        break;
    case OSP_SIP_MSG_PROC_FAIL: 
        {
            //等待定时器重新连接服务器
            TASKLOG(LOGIN_TASK, ERROR_LEV,"向[%s]注册失败, 稍后尝试重新注册\n",
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

    //删除所有挂载的task
    //如果CPeAgent未到服务态则会删除所有回调task
    CallBackTasks();

    return dwRet;
}

u32 CPeAgent::OnWaitRegRspTimer()
{
    if (GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout())
    {
        TASKLOG(LOGIN_TASK, WARNING_LEV, "注册服务器[%s]超时，立刻开始重连\n", 
            m_tPeURI.GetURIString().c_str());
        NextState(Idle);
        StartLogin();
    }

    return TIMERPOLL_DONE;
}

u32 CPeAgent::OnService( CMessage *const pcMsg )
{
    TASKLOG(LOGIN_TASK, CRITICAL_LEV, "在[CPeAgent::OnService]收到消息[%s--%hu]\n",
        OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
    COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
    u32 dwRet = PROCMSG_FAIL;

    switch (pcMsg->event)
    {
    case OSP_SIP_DISCONNECT:
        {
            TASKLOG(LOGIN_TASK, EVENT_LEV, "Pe[%s]断链, 销毁本Task[%u]登录事务\n",
                m_tPeURI.GetURIString().c_str(), GetTaskNO());

            //清除订阅资源
            //ReleaseSource();
            ClearDataWhenPeOfflineOrDisconnect();
            NextState(Idle);
            StartLogin();
            g_cCuiDataMgr.FlushPlatformServiceStatus(GetInstance(), NULL, m_tPeURI);
        }
        break;
    case OSP_SIP_MSG_PROC_FAIL:
        {
            TASKLOG(LOGIN_TASK, ERROR_LEV, "发送给PE[%s]的SIP消息失败, SIP错误码为[%d]\n",
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
            TASKLOG(LOGIN_TASK, ERROR_LEV, "[CPeAgent::StartLogin]发送SIP消息给[%s]失败!\n",
                m_tPeURI.GetURIString().c_str());

            //暂时不错处理
            //dwRet = PROCMSG_DEL;
        }
        else
        {
            TASKLOG(LOGIN_TASK, EVENT_LEV, "[CPeAgent::StartLogin]发送SIP消息给[%s]成功!\n",
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
        TASKLOG(LOGIN_TASK, ERROR_LEV, "发送Logout消息给服务器[%s]失败!\n",
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
