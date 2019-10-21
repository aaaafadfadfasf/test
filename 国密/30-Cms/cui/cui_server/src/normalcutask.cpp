/*****************************************************************************
模块名      : cui_fcgi 
文件名      : normalcutask.cpp
相关文件    : 
文件实现功能: 定义一些Cu登录事务之外的例行事务
作者        : liangli
版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
修改记录:
日  期      版本        修改人      修改内容
2012/1/11   1.0         liangli        创建
******************************************************************************/
#include "culogintask.h"
#include "cuiproto.h"
//#include "transthread.h"
#include "normalcutask.h"
//#include "cuidata.h"
#include "cuiapp.h"
#include "cms/commonconfig/cmcfg.h"
#include "cms/utility/md5.h"
#include "cms/utility/flow_limit_counter.h"
#include "cbb/zlib/zlib.h"
#include "cms/utility/base64codec.h"
#include "subscribe_task.h"
#include "invitetasknatswitch.h"
#include "peinvitetasknatswitch.h"
#include "dpssearchtask.h"
#include "redisdb.h"
#include "sdk/redissdk/redissdk.h"
#include "StreamDecentrationAdapter.h"
#include "redisssmng.h"



extern CRedisDBInterface g_cRedisDb;
extern CRedisSdk g_cCuiRedisSdk;

static CFlowLimitCounter *s_cFlowLimitCounter4Subscribe = NULL;
static CFlowLimitCounter *s_cFlowLimitCounter4Unsubscribe = NULL;


string Num2Str(int nNum)
{
	ostringstream oss;
	oss << nNum;
	return oss.str();
}


void Convert_DeviceCapPrivilege_TO_DeviceCapIndexs(const vector<TDeviceCapPrivilege>& tFrom, vector<TDeviceCapIndexs> &tTo)
{
    vector<TDeviceCapPrivilege>::const_iterator pDevCapPri = tFrom.begin();
    while ( pDevCapPri != tFrom.end() )
    {
        TDeviceCapIndexs tDevCapIndex;
        tDevCapIndex.id = pDevCapPri->id;

        map<int, CBitsContainer>::const_iterator pPriItem = pDevCapPri->privileges.begin();
        while ( pPriItem != pDevCapPri->privileges.end() )
        {
            tDevCapIndex.indexs.push_back(pPriItem->first);
            pPriItem++;
        }

        tTo.push_back(tDevCapIndex);
        pDevCapPri++;
    }
}

CSingleInterAction::CSingleInterAction( CInstExt *pcInst, CCuAgent *pcCuAgent, PTRecvNode ptRecvNode ) : CSipTask(pcInst)
{
    CMS_ASSERT(pcCuAgent != NULL);
    CMS_ASSERT(ptRecvNode != NULL);
    m_pcCuAgent = pcCuAgent;
    m_ptRecvNode = ptRecvNode;
    m_pcCuAgent->AddSubTask(this);
}

void CSingleInterAction::TerminateTask()
{
    m_pcCuAgent->DelSubTask(this);
    //     if ( GetState() == WAIT_RESPONSE_STATE && m_ptRecvNode != NULL )
    //     {
    //         CEventReq cReq;
    //         cReq.FromXml(m_strReq);
    // 
    //         CEventRsp cRsp;
    //         cRsp.SetSession(cReq.GetSession());
    //         cRsp.SetSeqNum(cReq.GetSeqNum());
    //         cRsp.SetErrorCode(ERR_CUI_TASK_DEL);
    //         SendRspToCu<CEventRsp>(cRsp, m_ptRecvNode);
    //     }
}

void CSingleInterAction::InitStateMachine()
{
    CStateProc cNotRequestProc;
    cNotRequestProc.ProcMsg = (CSipTask::PFProcMsg)&CSingleInterAction::NotReqProcMsg;
    cNotRequestProc.TimerPoll = &CSipTask::TimerPollSkip;
    cNotRequestProc.ProcErrMsg = (CSipTask::PFProcMsg)&CSingleInterAction::NotReqProcMsg;
    AddRuleProc(NOT_REQUEST_STATE, cNotRequestProc);

    CStateProc cWaitRspProc;
    cWaitRspProc.ProcMsg = (CSipTask::PFProcMsg)&CSingleInterAction::WaitRspProcMsg;
    cWaitRspProc.TimerPoll = (CSipTask::PFTimerPoll)&CSingleInterAction::WaitRspTimerPoll;
    cWaitRspProc.ProcErrMsg = (CSipTask::PFProcMsg)&CSingleInterAction::WaitRspProcMsg;
    AddRuleProc(WAIT_RESPONSE_STATE, cWaitRspProc);

    NextState(NOT_REQUEST_STATE);
}

u32 CSingleInterAction::NotReqProcMsg( CMessage *const pcMsg )
{
    TASKLOG(NORMAL_TASK, EVENT_LEV, "[CSingleInterAction]收到CU用户[%s]会话[%s]的请求[%s--%hu]\n",
        m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

    //     TASKLOG(NORMAL_TASK, EVENT_LEV, "CSingleInterAction[TaskNO %u]发送消息[%s--%d]给服务器\n", 
    //         GetTaskNO(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
    switch (pcMsg->event)
    {
    //case UAS_OPERATE_LOG_ADD_REQ:
    //    {
    //        CCuOperateLogAddReq *pcReq = (CCuOperateLogAddReq *)pcMsg->content;
    //        CCuOperateLogAddRsp cRsp;
    //        cRsp.SetSeqNum(pcReq->GetSeqNum());
    //        cRsp.SetSession(pcReq->GetSession());

    //        const TOperateLogInfo &tLog = pcReq->GetInfo();

    //        if (tLog.operateType == enUserOpTypeCuLocalRecord)
    //        {
    //            CUsrOprLogTask* pcTask = new CUsrOprLogTask(GetInstance(), m_pcCuAgent->m_strSession, 
    //                string(DEV_TYPE_UAS) + "@" + ::GetDomainName(),
    //                tLog.operateType,
    //                tLog.operateResult,
    //                tLog.operateDesc,
				//	tLog.devUri);
				//pcTask->InitStateMachine();
				//pcTask->StartSendLog();
    //        }
    //        else
    //        {
    //            // 不允许记录的日志类型
    //            cRsp.SetErrorCode(ERR_CUI_INVALID_PARAM);
    //        }
    //        
    //        SendRspToCu<CEventRsp>(cRsp, m_ptRecvNode);

    //        delete this;
    //        return PROCMSG_DEL;
    //    }
    //    break;
    case CU_GETCUILIST_REQ:
        {
            OnNotReqMsg<CCuGetCuiListReq>(pcMsg);
        }
        break;
    case PU_PTZ_CTRL_REQ:
        {
            OnNotReqMsg<CPuPtzCtrlReq>(pcMsg);
        }
        break;
	case PU_LONG_OSD_GET_REQ:
		{
			OnNotReqMsg<CCuPuLongOsdGetReq>(pcMsg);
		}
		break;
	case PU_LONG_OSD_SET_REQ:
		{
			OnNotReqMsg<CCuPuLongOsdSetReq>(pcMsg);
		}
		break;
    case PU_PTZ_LOCK_REQ:
        {
            OnNotReqMsg<CPuPtzLockReq>(pcMsg);
        }
        break;
    case PU_PTZ_UNLOCK_REQ:
        {
            OnNotReqMsg<CPuPtzUnlockReq>(pcMsg);
        }
        break;
	case TAS_PTZLOCK_ADD_REQ:
		{
			CCuPtzLockAddReq *pcReq = (CCuPtzLockAddReq *)pcMsg->content;

			if (!pcReq->GetCPtzLockInfo().GetDeviceId().empty())
			{
				pcReq->GetCPtzLockInfo().SetDeviceId(GetNameWithoutDomainFromURI(pcReq->GetCPtzLockInfo().GetDeviceId()));
			}

			u32 dwRet = PROCMSG_OK;

			dwRet = PostMsgReqTo3AS(*pcReq);

			if (PROCMSG_DEL == dwRet)
			{
				TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]发送请求[%s--%d]，TAS不在线，直接拒绝请求\n",
					m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
					pcReq->GetEventStr().c_str(), pcReq->GetEventId());
				CMS_ASSERT(NULL != m_ptRecvNode);
				CEventRsp cRsp;
				cRsp.SetSeqNum(pcReq->GetSeqNum());
				cRsp.SetSession(pcReq->GetSession());
				cRsp.SetErrorCode(ERR_CUI_TAS_OFFLINE);
				SendRspToCu<CEventRsp>(cRsp, m_ptRecvNode);
				NextState(NOT_REQUEST_STATE);
				delete this;
				return dwRet;
			}

			m_strReq = pcReq->ToXml();
			//2 转换状态，等待应答
			NextState(WAIT_RESPONSE_STATE);
			//3 设置超时消息处理
			return PROCMSG_OK;
		}
		break;
	case TAS_PTZLOCK_DEL_REQ:
	{
		CCuPtzLockDelReq *pcReq = (CCuPtzLockDelReq *)pcMsg->content;

		if (!pcReq->GetDeviceId().empty())
		{
			pcReq->SetDeviceId(GetNameWithoutDomainFromURI(pcReq->GetDeviceId()));
		}

		u32 dwRet = PROCMSG_OK;

		dwRet = PostMsgReqTo3AS(*pcReq);

		if (PROCMSG_DEL == dwRet)
		{
			TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]发送请求[%s--%d]，TAS不在线，直接拒绝请求\n",
				m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
				pcReq->GetEventStr().c_str(), pcReq->GetEventId());
			CMS_ASSERT(NULL != m_ptRecvNode);
			CEventRsp cRsp;
			cRsp.SetSeqNum(pcReq->GetSeqNum());
			cRsp.SetSession(pcReq->GetSession());
			cRsp.SetErrorCode(ERR_CUI_TAS_OFFLINE);
			SendRspToCu<CEventRsp>(cRsp, m_ptRecvNode);
			NextState(NOT_REQUEST_STATE);
			delete this;
			return dwRet;
		}

		m_strReq = pcReq->ToXml();
		//2 转换状态，等待应答
		NextState(WAIT_RESPONSE_STATE);
		//3 设置超时消息处理
		return PROCMSG_OK;
	}
	break;
    case PU_TRANSPARENT_OPERATE_REQ:
        {
            OnNotReqMsg<CCuPuTransparentOperateReq>(pcMsg);
        }
        break;
    case KEY_FRAME_REQ:
        {
            OnNotReqMsg<CCuKeyFrameReq>(pcMsg);
        }
        break;
    case PU_VSIP_OPERATE_REQ:
        {
            OnNotReqMsg<CCuPuVsipOperateReq>(pcMsg);
        }
        break;
    case PU_REC_DAYS_QRY_REQ:
        {
            OnNotReqMsg<CCuPuRecDaysQryReq>(pcMsg);
        }
        break;
    case TAS_DEVICE_GROUP_ADD_REQ:
        {
            //OnNotReqMsg<CDeviceGroupAddReq>(pcMsg, SERVER_3AS);
            CDeviceGroupAddReq *pcReq = (CDeviceGroupAddReq *)pcMsg->content;

            if(!pcReq->GetDeviceGroupInfo().GetDeviceGroupID().empty())
            {
                pcReq->GetDeviceGroupInfo().SetDeviceGroupID(GetNameWithoutDomainFromURI(pcReq->GetDeviceGroupInfo().GetDeviceGroupID()));
            }

            if(!pcReq->GetDeviceGroupInfo().GetDeviceParentGroupID().empty())
            {
                pcReq->GetDeviceGroupInfo().SetDeviceParentGroupID(GetNameWithoutDomainFromURI(pcReq->GetDeviceGroupInfo().GetDeviceParentGroupID()));
            }


            u32 dwRet = PROCMSG_OK;

            dwRet = PostMsgReqTo3AS(*pcReq);

            if ( PROCMSG_DEL == dwRet )
            {
                TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]发送请求[%s--%d]，TAS不在线，直接拒绝请求\n",
                    m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
                    pcReq->GetEventStr().c_str(), pcReq->GetEventId());
                CMS_ASSERT(NULL != m_ptRecvNode);
                CEventRsp cRsp;
                cRsp.SetSeqNum(pcReq->GetSeqNum());
                cRsp.SetSession(pcReq->GetSession());
                cRsp.SetErrorCode(ERR_CUI_TAS_OFFLINE);
                SendRspToCu<CEventRsp>(cRsp, m_ptRecvNode);
                NextState(NOT_REQUEST_STATE);
                delete this;
                return dwRet;
            }

            m_strReq = pcReq->ToXml();
            //2 转换状态，等待应答
            NextState(WAIT_RESPONSE_STATE);
            //3 设置超时消息处理
            return PROCMSG_OK;
        }
        break;
    case TAS_DEVICE_GROUP_DEL_REQ:
        {
            //OnNotReqMsg<CDeviceGroupDelReq>(pcMsg, SERVER_3AS);
            CDeviceGroupDelReq *pcReq = (CDeviceGroupDelReq *)pcMsg->content;
            pcReq->GetDeviceGroupInfoKey().SetDeviceGroupID(GetNameWithoutDomainFromURI(pcReq->GetDeviceGroupInfoKey().GetDeviceGroupID()));


            u32 dwRet = PROCMSG_OK;

            dwRet = PostMsgReqTo3AS(*pcReq);

            if ( PROCMSG_DEL == dwRet )
            {
                TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]发送请求[%s--%d]，TAS不在线，直接拒绝请求\n",
                    m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
                    pcReq->GetEventStr().c_str(), pcReq->GetEventId());
                CMS_ASSERT(NULL != m_ptRecvNode);
                CEventRsp cRsp;
                cRsp.SetSeqNum(pcReq->GetSeqNum());
                cRsp.SetSession(pcReq->GetSession());
                cRsp.SetErrorCode(ERR_CUI_TAS_OFFLINE);
                SendRspToCu<CEventRsp>(cRsp, m_ptRecvNode);
                NextState(NOT_REQUEST_STATE);
                delete this;
                return dwRet;
            }

            m_strReq = pcReq->ToXml();
            //2 转换状态，等待应答
            NextState(WAIT_RESPONSE_STATE);
            //3 设置超时消息处理
            return PROCMSG_OK;
        }
        break;
    case TAS_DEVICE_GROUP_MOD_REQ:
        {
            //OnNotReqMsg<CDeviceGroupModReq>(pcMsg, SERVER_3AS);
            CDeviceGroupModReq *pcReq = (CDeviceGroupModReq *)pcMsg->content;

            string strGroupID;
            pcReq->GetDeviceGroupID(strGroupID);

            if(!strGroupID.empty())
            {
                pcReq->SetDeviceGroupID(GetNameWithoutDomainFromURI(strGroupID));
            }

            string strParentGroupID;
            pcReq->GetDeviceParentGroupID(strParentGroupID);
            if(!strParentGroupID.empty())
            {
                pcReq->SetDeviceParentGroupID(GetNameWithoutDomainFromURI(strParentGroupID));
            }

            u32 dwRet = PROCMSG_OK;

            dwRet = PostMsgReqTo3AS(*pcReq);

            if ( PROCMSG_DEL == dwRet )
            {
                TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]发送请求[%s--%d]，TAS不在线，直接拒绝请求\n",
                    m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
                    pcReq->GetEventStr().c_str(), pcReq->GetEventId());
                CMS_ASSERT(NULL != m_ptRecvNode);
                CEventRsp cRsp;
                cRsp.SetSeqNum(pcReq->GetSeqNum());
                cRsp.SetSession(pcReq->GetSession());
                cRsp.SetErrorCode(ERR_CUI_TAS_OFFLINE);
                SendRspToCu<CEventRsp>(cRsp, m_ptRecvNode);
                NextState(NOT_REQUEST_STATE);
                delete this;
                return dwRet;
            }

            m_strReq = pcReq->ToXml();
            //2 转换状态，等待应答
            NextState(WAIT_RESPONSE_STATE);
            //3 设置超时消息处理
            return PROCMSG_OK;
        }
        break;
	case TAS_DEVICE_GROUP_MOD_4GB_REQ:
		{
			CCuModifyGBDeviceGroupReq *pcReq = (CCuModifyGBDeviceGroupReq *)pcMsg->content;

			string strGroupID = pcReq->GetDevGrpId();
			if (!strGroupID.empty())
			{
				pcReq->SetDevGrpId(GetNameWithoutDomainFromURI(strGroupID));
			}
			
			u32 dwRet = PROCMSG_OK;

			dwRet = PostMsgReqTo3AS(*pcReq);

			if (PROCMSG_DEL == dwRet)
			{
				TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]发送请求[%s--%d]，TAS不在线，直接拒绝请求\n",
					m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
					pcReq->GetEventStr().c_str(), pcReq->GetEventId());
				CMS_ASSERT(NULL != m_ptRecvNode);
				CEventRsp cRsp;
				cRsp.SetSeqNum(pcReq->GetSeqNum());
				cRsp.SetSession(pcReq->GetSession());
				cRsp.SetErrorCode(ERR_CUI_TAS_OFFLINE);
				SendRspToCu<CEventRsp>(cRsp, m_ptRecvNode);
				NextState(NOT_REQUEST_STATE);
				delete this;
				return dwRet;
			}

			m_strReq = pcReq->ToXml();
			//2 转换状态，等待应答
			NextState(WAIT_RESPONSE_STATE);
			//3 设置超时消息处理
			return PROCMSG_OK;
		}
		break;

    case TAS_DEVICE_GROUP_DEVICE_ORDER_REQ:
        {
            CDeviceGroupDeviceOrderReq *pcReq = (CDeviceGroupDeviceOrderReq *)pcMsg->content;

			//string strGroupID = pcReq->GetDevGrpId();
			//if (!strGroupID.empty())
			//{
			//	pcReq->SetDevGrpId(GetNameWithoutDomainFromURI(strGroupID));
			//}
			
			u32 dwRet = PROCMSG_OK;

			dwRet = PostMsgReqTo3AS(*pcReq);

			if (PROCMSG_DEL == dwRet)
			{
				TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]发送请求[%s--%d]，TAS不在线，直接拒绝请求\n",
					m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
					pcReq->GetEventStr().c_str(), pcReq->GetEventId());
				CMS_ASSERT(NULL != m_ptRecvNode);
				CEventRsp cRsp;
				cRsp.SetSeqNum(pcReq->GetSeqNum());
				cRsp.SetSession(pcReq->GetSession());
				cRsp.SetErrorCode(ERR_CUI_TAS_OFFLINE);
				SendRspToCu<CEventRsp>(cRsp, m_ptRecvNode);
				NextState(NOT_REQUEST_STATE);
				delete this;
				return dwRet;
			}

			m_strReq = pcReq->ToXml();
			//2 转换状态，等待应答
			NextState(WAIT_RESPONSE_STATE);
			//3 设置超时消息处理
			return PROCMSG_OK;

        }
        break;

    case TAS_CUSER_GROUP_ADD_REQ:
        {
            //OnNotReqMsg<CUserGroupAddReq>(pcMsg, SERVER_3AS);
            CUserGroupAddReq *pcReq = (CUserGroupAddReq *)pcMsg->content;

            if(!pcReq->GetUserGroupInfo().GetCreateUser().empty())
            {
                pcReq->GetUserGroupInfo().SetCreateUser(GetNameWithoutDomainFromURI(pcReq->GetUserGroupInfo().GetCreateUser()));
            }

            u32 dwRet = PROCMSG_OK;

            dwRet = PostMsgReqTo3AS(*pcReq);

            if ( PROCMSG_DEL == dwRet )
            {
                TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]发送请求[%s--%d]，TAS不在线，直接拒绝请求\n",
                    m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
                    pcReq->GetEventStr().c_str(), pcReq->GetEventId());
                CMS_ASSERT(NULL != m_ptRecvNode);
                CEventRsp cRsp;
                cRsp.SetSeqNum(pcReq->GetSeqNum());
                cRsp.SetSession(pcReq->GetSession());
                cRsp.SetErrorCode(ERR_CUI_TAS_OFFLINE);
                SendRspToCu<CEventRsp>(cRsp, m_ptRecvNode);
                NextState(NOT_REQUEST_STATE);
                delete this;
                return dwRet;
            }

            m_strReq = pcReq->ToXml();
            //2 转换状态，等待应答
            NextState(WAIT_RESPONSE_STATE);
            //3 设置超时消息处理
            return PROCMSG_OK;
        }
        break;
    case TAS_CUSER_GROUP_DEL_REQ:
        {
            //           OnNotReqMsg<CUserGroupDelReq>(pcMsg, SERVER_3AS);
            CUserGroupDelReq *pcReq = (CUserGroupDelReq *)pcMsg->content;

            pcReq->GetUserGroupInfoKey().SetGroupNO(GetNameWithoutDomainFromURI(pcReq->GetUserGroupInfoKey().GetGroupNO()));

            u32 dwRet = PROCMSG_OK;

            dwRet = PostMsgReqTo3AS(*pcReq);

            if ( PROCMSG_DEL == dwRet )
            {
                TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]发送请求[%s--%d]，TAS不在线，直接拒绝请求\n",
                    m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
                    pcReq->GetEventStr().c_str(), pcReq->GetEventId());
                CMS_ASSERT(NULL != m_ptRecvNode);
                CEventRsp cRsp;
                cRsp.SetSeqNum(pcReq->GetSeqNum());
                cRsp.SetSession(pcReq->GetSession());
                cRsp.SetErrorCode(ERR_CUI_TAS_OFFLINE);
                SendRspToCu<CEventRsp>(cRsp, m_ptRecvNode);
                NextState(NOT_REQUEST_STATE);
                delete this;
                return dwRet;
            }

            m_strReq = pcReq->ToXml();
            //2 转换状态，等待应答
            NextState(WAIT_RESPONSE_STATE);
            //3 设置超时消息处理
            return PROCMSG_OK;
        }
        break;
    case TAS_CUSER_GROUP_MOD_REQ:
        {
            //OnNotReqMsg<CUserGroupModReq>(pcMsg, SERVER_3AS);
            CUserGroupModReq *pcReq = (CUserGroupModReq *)pcMsg->content;

            string strGroupN0;
            pcReq->GetGroupNO(strGroupN0);
            if(!strGroupN0.empty())
            {
                pcReq->SetGroupNO(GetNameWithoutDomainFromURI(strGroupN0));
            }    

            u32 dwRet = PROCMSG_OK;

            dwRet = PostMsgReqTo3AS(*pcReq);

            if ( PROCMSG_DEL == dwRet )
            {
                TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]发送请求[%s--%d]，TAS不在线，直接拒绝请求\n",
                    m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
                    pcReq->GetEventStr().c_str(), pcReq->GetEventId());
                CMS_ASSERT(NULL != m_ptRecvNode);
                CEventRsp cRsp;
                cRsp.SetSeqNum(pcReq->GetSeqNum());
                cRsp.SetSession(pcReq->GetSession());
                cRsp.SetErrorCode(ERR_CUI_TAS_OFFLINE);
                SendRspToCu<CEventRsp>(cRsp, m_ptRecvNode);
                NextState(NOT_REQUEST_STATE);
                delete this;
                return dwRet;
            }

            m_strReq = pcReq->ToXml();
            //2 转换状态，等待应答
            NextState(WAIT_RESPONSE_STATE);
            //3 设置超时消息处理

            return PROCMSG_OK;
        }
        break;
    case TAS_CUSER_ADD_REQ:
        {
            //OnNotReqMsg<CUserAddReq>(pcMsg, SERVER_3AS);
            CUserAddReq *pcReq = (CUserAddReq *)pcMsg->content;

            if(!pcReq->GetUserInfo().GetUsergrpID().empty())
            {
                pcReq->GetUserInfo().SetUsergrpID(GetNameWithoutDomainFromURI(pcReq->GetUserInfo().GetUsergrpID()));
            }

            if(!pcReq->GetUserInfo().GetUserName().empty())
            {
                pcReq->GetUserInfo().SetUserName(GetNameWithoutDomainFromURI(pcReq->GetUserInfo().GetUserName()));
            }

            if(!pcReq->GetUserInfo().GetUserID().empty())
            {
                pcReq->GetUserInfo().SetUserID(GetNameWithoutDomainFromURI(pcReq->GetUserInfo().GetUserID()));
            }

            if(!pcReq->GetUserInfo().GetCreatorID().empty())
            {
                pcReq->GetUserInfo().SetCreatorID(GetNameWithoutDomainFromURI(pcReq->GetUserInfo().GetCreatorID()));
            }

            if(!pcReq->GetUserInfo().GetUserPwd().empty())
            {
				//由于加入了安全 SM3_Base64 算法，又要兼容老的明文密码，所以cui里都透传，fcgi那边会根据新的接口判断是透传还是先md5加密 2018/10/24 zsy
				//string strPwd = pcReq->GetUserInfo().GetUserPwd();
				//MD5(strPwd, pcReq->GetUserInfo().GetUserPwd());
            }

            u32 dwRet = PROCMSG_OK;

            dwRet = PostMsgReqTo3AS(*pcReq);

            if ( PROCMSG_DEL == dwRet )
            {
                TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]发送请求[%s--%d]，TAS不在线，直接拒绝请求\n",
                    m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
                    pcReq->GetEventStr().c_str(), pcReq->GetEventId());
                CMS_ASSERT(NULL != m_ptRecvNode);
                CEventRsp cRsp;
                cRsp.SetSeqNum(pcReq->GetSeqNum());
                cRsp.SetSession(pcReq->GetSession());
                cRsp.SetErrorCode(ERR_CUI_TAS_OFFLINE);
                SendRspToCu<CEventRsp>(cRsp, m_ptRecvNode);
                NextState(NOT_REQUEST_STATE);
                delete this;
                return dwRet;
            }

            m_strReq = pcReq->ToXml();
            //2 转换状态，等待应答
            NextState(WAIT_RESPONSE_STATE);
            //3 设置超时消息处理

            return PROCMSG_OK;
        }
        break;
    case TAS_CUSER_DEL_REQ:
        {
            //OnNotReqMsg<CUserDelReq>(pcMsg, SERVER_3AS);
            CUserDelReq *pcReq = (CUserDelReq *)pcMsg->content;

            pcReq->SetUserID(GetNameWithoutDomainFromURI(pcReq->GetUserID()));

            u32 dwRet = PROCMSG_OK;

            dwRet = PostMsgReqTo3AS(*pcReq);

            if ( PROCMSG_DEL == dwRet )
            {
                TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]发送请求[%s--%d]，TAS不在线，直接拒绝请求\n",
                    m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
                    pcReq->GetEventStr().c_str(), pcReq->GetEventId());
                CMS_ASSERT(NULL != m_ptRecvNode);
                CEventRsp cRsp;
                cRsp.SetSeqNum(pcReq->GetSeqNum());
                cRsp.SetSession(pcReq->GetSession());
                cRsp.SetErrorCode(ERR_CUI_TAS_OFFLINE);
                SendRspToCu<CEventRsp>(cRsp, m_ptRecvNode);
                NextState(NOT_REQUEST_STATE);
                delete this;
                return dwRet;
            }

            m_strReq = pcReq->ToXml();
            //2 转换状态，等待应答
            NextState(WAIT_RESPONSE_STATE);
            //3 设置超时消息处理

            return PROCMSG_OK;
        }
        break;
    case TAS_CUSER_MOD_REQ:
        {
            //OnNotReqMsg<CUserModReq>(pcMsg, SERVER_3AS);
            CUserModReq *pcReq = (CUserModReq *)pcMsg->content;

            string strUserID;
            pcReq->GetUserID(strUserID);
            if(!strUserID.empty())
            {
                pcReq->SetUserID(GetNameWithoutDomainFromURI(strUserID));
            }

            string strUserGroupID;
            pcReq->GetUsergrpID(strUserGroupID);
            if(!strUserGroupID.empty())
            {
                pcReq->SetUsergrpID(GetNameWithoutDomainFromURI(strUserGroupID));
            }

            string strUserName;
            pcReq->GetUserName(strUserName);
            if(!strUserName.empty())
            {
                pcReq->SetUserName(GetNameWithoutDomainFromURI(strUserName));
            }

            string strOldPassword;
            pcReq->GetUserOldPwd(strOldPassword);
            if(!strOldPassword.empty())
            {
				//由于加入了安全 SM3_Base64 算法，又要兼容老的明文密码，所以cui里都透传，fcgi那边会根据新的接口判断是透传还是先md5加密 2018/10/24 zsy
				//string strMd5OldPassWord;
				//MD5(strOldPassword, strMd5OldPassWord);
				//pcReq->SetUserOldPwd(strMd5OldPassWord);
            }

            string strNewPassword;
            pcReq->GetUserPwd(strNewPassword);
            if(!strNewPassword.empty())
            {
				//由于加入了安全 SM3_Base64 算法，又要兼容老的明文密码，所以cui里都透传，fcgi那边会根据新的接口判断是透传还是先md5加密 2018/10/24 zsy
				//string strMd5NewPassWord;
				//MD5(strNewPassword, strMd5NewPassWord);
				//pcReq->SetUserPwd(strMd5NewPassWord);
            }

            u32 dwRet = PROCMSG_OK;

            dwRet = PostMsgReqTo3AS(*pcReq);

            if ( PROCMSG_DEL == dwRet )
            {
                TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]发送请求[%s--%d]，TAS不在线，直接拒绝请求\n",
                    m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
                    pcReq->GetEventStr().c_str(), pcReq->GetEventId());
                CMS_ASSERT(NULL != m_ptRecvNode);
                CEventRsp cRsp;
                cRsp.SetSeqNum(pcReq->GetSeqNum());
                cRsp.SetSession(pcReq->GetSession());
                cRsp.SetErrorCode(ERR_CUI_TAS_OFFLINE);
                SendRspToCu<CEventRsp>(cRsp, m_ptRecvNode);
                NextState(NOT_REQUEST_STATE);
                delete this;
                return dwRet;
            }

            m_strReq = pcReq->ToXml();
            //2 转换状态，等待应答
            NextState(WAIT_RESPONSE_STATE);
            //3 设置超时消息处理

            return PROCMSG_OK;
        }
        break;
    case TAS_TEMP_AUTHORIZATION_ADD_REQ:
        {
            //OnNotReqMsg<CTempAuthorizationAddReq>(pcMsg, SERVER_3AS);
            CTempAuthorizationAddReq *pcReq = (CTempAuthorizationAddReq *)pcMsg->content;

            pcReq->GetTempAuthorizationInfo().SetDeviceID(GetNameWithoutDomainFromURI(pcReq->GetTempAuthorizationInfo().GetDeviceID()));
            pcReq->GetTempAuthorizationInfo().SetUsername(GetNameWithoutDomainFromURI(pcReq->GetTempAuthorizationInfo().GetUsername()));


            u32 dwRet = PROCMSG_OK;

            dwRet = PostMsgReqTo3AS(*pcReq);

            if ( PROCMSG_DEL == dwRet )
            {
                TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]发送请求[%s--%d]，TAS不在线，直接拒绝请求\n",
                    m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
                    pcReq->GetEventStr().c_str(), pcReq->GetEventId());
                CMS_ASSERT(NULL != m_ptRecvNode);
                CEventRsp cRsp;
                cRsp.SetSeqNum(pcReq->GetSeqNum());
                cRsp.SetSession(pcReq->GetSession());
                cRsp.SetErrorCode(ERR_CUI_TAS_OFFLINE);
                SendRspToCu<CEventRsp>(cRsp, m_ptRecvNode);
                NextState(NOT_REQUEST_STATE);
                delete this;
                return dwRet;
            }

            m_strReq = pcReq->ToXml();
            //2 转换状态，等待应答
            NextState(WAIT_RESPONSE_STATE);
            //3 设置超时消息处理

            return PROCMSG_OK;
        }
        break;
    case TAS_TEMP_AUTHORIZATION_DEL_REQ:
        {
            //OnNotReqMsg<CTempAuthorizationDelReq>(pcMsg, SERVER_3AS);
            CTempAuthorizationDelReq *pcReq = (CTempAuthorizationDelReq *)pcMsg->content;

            pcReq->GetTempAuthorizationInfoKey().SetDeviceID(GetNameWithoutDomainFromURI(pcReq->GetTempAuthorizationInfoKey().GetDeviceID()));
            pcReq->GetTempAuthorizationInfoKey().SetUserName(GetNameWithoutDomainFromURI(pcReq->GetTempAuthorizationInfoKey().GetUserName()));

            u32 dwRet = PROCMSG_OK;

            dwRet = PostMsgReqTo3AS(*pcReq);

            if ( PROCMSG_DEL == dwRet )
            {
                TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]发送请求[%s--%d]，TAS不在线，直接拒绝请求\n",
                    m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
                    pcReq->GetEventStr().c_str(), pcReq->GetEventId());
                CMS_ASSERT(NULL != m_ptRecvNode);
                CEventRsp cRsp;
                cRsp.SetSeqNum(pcReq->GetSeqNum());
                cRsp.SetSession(pcReq->GetSession());
                cRsp.SetErrorCode(ERR_CUI_TAS_OFFLINE);
                SendRspToCu<CEventRsp>(cRsp, m_ptRecvNode);
                NextState(NOT_REQUEST_STATE);
                delete this;
                return dwRet;
            }

            m_strReq = pcReq->ToXml();
            //2 转换状态，等待应答
            NextState(WAIT_RESPONSE_STATE);
            //3 设置超时消息处理

            return PROCMSG_OK;
        }
        break;
    case TAS_TEMP_AUTHORIZATION_MOD_REQ:
        {
            //OnNotReqMsg<CTempAuthorizationModReq>(pcMsg, SERVER_3AS);
            CTempAuthorizationModReq *pcReq = (CTempAuthorizationModReq *)pcMsg->content;

            string strDevID;
            pcReq->GetDeviceID(strDevID);
            if(!strDevID.empty())
            {
                pcReq->SetDeviceID(GetNameWithoutDomainFromURI(strDevID));
            }   

            u32 dwRet = PROCMSG_OK;

            dwRet = PostMsgReqTo3AS(*pcReq);

            if ( PROCMSG_DEL == dwRet )
            {
                TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]发送请求[%s--%d]，TAS不在线，直接拒绝请求\n",
                    m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
                    pcReq->GetEventStr().c_str(), pcReq->GetEventId());
                CMS_ASSERT(NULL != m_ptRecvNode);
                CEventRsp cRsp;
                cRsp.SetSeqNum(pcReq->GetSeqNum());
                cRsp.SetSession(pcReq->GetSession());
                cRsp.SetErrorCode(ERR_CUI_TAS_OFFLINE);
                SendRspToCu<CEventRsp>(cRsp, m_ptRecvNode);
                NextState(NOT_REQUEST_STATE);
                delete this;
                return dwRet;
            }

            m_strReq = pcReq->ToXml();
            //2 转换状态，等待应答
            NextState(WAIT_RESPONSE_STATE);
            //3 设置超时消息处理

            return PROCMSG_OK;
        }
        break;
    case TAS_DEVICE_CAP_INDEX_NAME_GET_REQ:
        {
            //OnNotReqMsg<CCuDeviceCapIndexNameGetReq>(pcMsg, SERVER_3AS);
            CCuDeviceCapIndexNameGetReq *pcReq = (CCuDeviceCapIndexNameGetReq *)pcMsg->content;

            pcReq->SetDeviceId(GetNameWithoutDomainFromURI(pcReq->GetDeviceId()));

            u32 dwRet = PROCMSG_OK;

            dwRet = PostMsgReqTo3AS(*pcReq);

            if ( PROCMSG_DEL == dwRet )
            {
                TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]发送请求[%s--%d]，TAS不在线，直接拒绝请求\n",
                    m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
                    pcReq->GetEventStr().c_str(), pcReq->GetEventId());
                CMS_ASSERT(NULL != m_ptRecvNode);
                CEventRsp cRsp;
                cRsp.SetSeqNum(pcReq->GetSeqNum());
                cRsp.SetSession(pcReq->GetSession());
                cRsp.SetErrorCode(ERR_CUI_TAS_OFFLINE);
                SendRspToCu<CEventRsp>(cRsp, m_ptRecvNode);
                NextState(NOT_REQUEST_STATE);
                delete this;
                return dwRet;
            }

            m_strReq = pcReq->ToXml();
            //2 转换状态，等待应答
            NextState(WAIT_RESPONSE_STATE);
            //3 设置超时消息处理

            return PROCMSG_OK;
        }
        break;
    case TAS_DEVICE_CAP_INDEX_NAME_SET_REQ:
        {
            //OnNotReqMsg<CCuDeviceCapIndexNameSetReq>(pcMsg, SERVER_3AS);
            CCuDeviceCapIndexNameSetReq *pcReq = (CCuDeviceCapIndexNameSetReq *)pcMsg->content;
            if (!m_pcCuAgent->IsHasDevPrivilege(pcReq->GetDeviceCapName().deviceId, CEncoderPrivilege::en_Param_Config))
            {
                CEventRsp cRsp;
                cRsp.SetSeqNum(pcReq->GetSeqNum());
                cRsp.SetSession(pcReq->GetSession());
                cRsp.SetErrorCode(ERR_CUI_CU_NO_PERMISSION);
                SendRspToCu<CEventRsp>(cRsp, m_ptRecvNode);
                NextState(NOT_REQUEST_STATE);
                delete this;
                return PROCMSG_DEL;
            }

            pcReq->GetDeviceCapName().deviceId = GetNameWithoutDomainFromURI(pcReq->GetDeviceCapName().deviceId);

            u32 dwRet = PROCMSG_OK;

            dwRet = PostMsgReqTo3AS(*pcReq);

            if ( PROCMSG_DEL == dwRet )
            {
                TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]发送请求[%s--%d]，TAS不在线，直接拒绝请求\n",
                    m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
                    pcReq->GetEventStr().c_str(), pcReq->GetEventId());
                CMS_ASSERT(NULL != m_ptRecvNode);
                CEventRsp cRsp;
                cRsp.SetSeqNum(pcReq->GetSeqNum());
                cRsp.SetSession(pcReq->GetSession());
                cRsp.SetErrorCode(ERR_CUI_TAS_OFFLINE);
                SendRspToCu<CEventRsp>(cRsp, m_ptRecvNode);
                NextState(NOT_REQUEST_STATE);
                delete this;
                return dwRet;
            }

            m_strReq = pcReq->ToXml();
            //2 转换状态，等待应答
            NextState(WAIT_RESPONSE_STATE);
            //3 设置超时消息处理

            return PROCMSG_OK;
        }
		break;
	case TAS_DEVTREE_ADD_REQ:
		{
			CCuDevTreeAddReq *pcReq = (CCuDevTreeAddReq *)pcMsg->content;

			u32 dwRet = PROCMSG_OK;

			dwRet = PostMsgReqTo3AS(*pcReq);

			if ( PROCMSG_DEL == dwRet )
			{
				TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]发送请求[%s--%d]，TAS不在线，直接拒绝请求\n",
					m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
					pcReq->GetEventStr().c_str(), pcReq->GetEventId());
				CMS_ASSERT(NULL != m_ptRecvNode);
				CEventRsp cRsp;
				cRsp.SetSeqNum(pcReq->GetSeqNum());
				cRsp.SetSession(pcReq->GetSession());
				cRsp.SetErrorCode(ERR_CUI_TAS_OFFLINE);
				SendRspToCu<CEventRsp>(cRsp, m_ptRecvNode);
				NextState(NOT_REQUEST_STATE);
				delete this;
				return dwRet;
			}

			m_strReq = pcReq->ToXml();
			//2 转换状态，等待应答
			NextState(WAIT_RESPONSE_STATE);
			//3 设置超时消息处理

			return PROCMSG_OK;
		}
		break;
	case TAS_DEVTREE_MOD_REQ:
		{
			CCuDevTreeModReq *pcReq = (CCuDevTreeModReq *)pcMsg->content;

			u32 dwRet = PROCMSG_OK;

			dwRet = PostMsgReqTo3AS(*pcReq);

			if ( PROCMSG_DEL == dwRet )
			{
				TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]发送请求[%s--%d]，TAS不在线，直接拒绝请求\n",
					m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
					pcReq->GetEventStr().c_str(), pcReq->GetEventId());
				CMS_ASSERT(NULL != m_ptRecvNode);
				CEventRsp cRsp;
				cRsp.SetSeqNum(pcReq->GetSeqNum());
				cRsp.SetSession(pcReq->GetSession());
				cRsp.SetErrorCode(ERR_CUI_TAS_OFFLINE);
				SendRspToCu<CEventRsp>(cRsp, m_ptRecvNode);
				NextState(NOT_REQUEST_STATE);
				delete this;
				return dwRet;
			}

			m_strReq = pcReq->ToXml();
			//2 转换状态，等待应答
			NextState(WAIT_RESPONSE_STATE);
			//3 设置超时消息处理

			return PROCMSG_OK;
		}
		break;
	case TAS_DEVTREE_DEL_REQ:
		{
			CCuDevTreeDelReq *pcReq = (CCuDevTreeDelReq *)pcMsg->content;

			u32 dwRet = PROCMSG_OK;

			dwRet = PostMsgReqTo3AS(*pcReq);

			if ( PROCMSG_DEL == dwRet )
			{
				TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]发送请求[%s--%d]，TAS不在线，直接拒绝请求\n",
					m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
					pcReq->GetEventStr().c_str(), pcReq->GetEventId());
				CMS_ASSERT(NULL != m_ptRecvNode);
				CEventRsp cRsp;
				cRsp.SetSeqNum(pcReq->GetSeqNum());
				cRsp.SetSession(pcReq->GetSession());
				cRsp.SetErrorCode(ERR_CUI_TAS_OFFLINE);
				SendRspToCu<CEventRsp>(cRsp, m_ptRecvNode);
				NextState(NOT_REQUEST_STATE);
				delete this;
				return dwRet;
			}

			m_strReq = pcReq->ToXml();
			//2 转换状态，等待应答
			NextState(WAIT_RESPONSE_STATE);
			//3 设置超时消息处理

			return PROCMSG_OK;
		}
		break;
    case TAS_TVWALL_ADD_REQ:
        {
            //OnNotReqMsg<CCuTvwallAddReq>(pcMsg, SERVER_3AS);
            CCuTvwallAddReq *pcReq = (CCuTvwallAddReq *)pcMsg->content;

            vector<TvDecoderBinder>::iterator item= pcReq->GetTvwallInfo().GetTvDecoderBinders().begin();
            while(item != pcReq->GetTvwallInfo().GetTvDecoderBinders().end())
            {
                item->decoderId = GetNameWithoutDomainFromURI(item->decoderId); 

                item++;
            }

            u32 dwRet = PROCMSG_OK;

            dwRet = PostMsgReqTo3AS(*pcReq);

            if ( PROCMSG_DEL == dwRet )
            {
                TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]发送请求[%s--%d]，TAS不在线，直接拒绝请求\n",
                    m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
                    pcReq->GetEventStr().c_str(), pcReq->GetEventId());
                CMS_ASSERT(NULL != m_ptRecvNode);

                CCuTvwallDelRsp cRsp;
                cRsp.SetSeqNum(pcReq->GetSeqNum());
                cRsp.SetSession(pcReq->GetSession());
                cRsp.SetErrorCode(ERR_CUI_TAS_OFFLINE);
                SendRspToCu<CCuTvwallDelRsp>(cRsp, m_ptRecvNode);
                NextState(NOT_REQUEST_STATE);
                delete this;
                return dwRet;
            }

            m_strReq = pcReq->ToXml();
            //2 转换状态，等待应答
            NextState(WAIT_RESPONSE_STATE);
            //3 设置超时消息处理

            return PROCMSG_OK;

        }
        break;
    case TAS_TVWALL_DEL_REQ:
        {
            CCuTvwallDelReq *pcReq = (CCuTvwallDelReq *)pcMsg->content;
            //1 向服务器发送请求, 请求类型必须是服务器定义的接口类型
            string strTvwallID = GetNameWithoutDomainFromURI(pcReq->GetTvwallId());
            pcReq->SetTvwallId(strTvwallID);

            u32 dwRet = PROCMSG_OK;

            dwRet = PostMsgReqTo3AS(*pcReq);

            if ( PROCMSG_DEL == dwRet )
            {
                TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]发送请求[%s--%d]，TAS不在线，直接拒绝请求\n",
                    m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
                    pcReq->GetEventStr().c_str(), pcReq->GetEventId());
                CMS_ASSERT(NULL != m_ptRecvNode);

                CCuTvwallDelRsp cRsp;
                cRsp.SetSeqNum(pcReq->GetSeqNum());
                cRsp.SetSession(pcReq->GetSession());
                cRsp.SetErrorCode(ERR_CUI_TAS_OFFLINE);
                SendRspToCu<CCuTvwallDelRsp>(cRsp, m_ptRecvNode);
                NextState(NOT_REQUEST_STATE);
                delete this;
                return dwRet;
            }

            m_strReq = pcReq->ToXml();
            //2 转换状态，等待应答
            NextState(WAIT_RESPONSE_STATE);
            //3 设置超时消息处理

            return PROCMSG_OK;
        }
        break;
    case TAS_TVWALL_MOD_REQ:
        {
            CCuTvwallModReq *pcReq = (CCuTvwallModReq *)pcMsg->content;
            //1 向服务器发送请求, 请求类型必须是服务器定义的接口类型
            string strTvwallUri;
            pcReq->GetId(strTvwallUri);

            if(!strTvwallUri.empty())
            {
                strTvwallUri = GetNameWithoutDomainFromURI(strTvwallUri);
                pcReq->SetId(strTvwallUri);
            }                      

            vector<TvDecoderBinder> val;
            pcReq->GetTvDecoderBinders(val);

            vector<TvDecoderBinder>::iterator item= val.begin();
            vector<TvDecoderBinder> tvDecodeBinderList;
            while(item != val.end())
            {
                item->decoderId = GetNameWithoutDomainFromURI(item->decoderId); 
                tvDecodeBinderList.push_back(*item);

                item++;
            }
            pcReq->SetTvDecoderBinders(tvDecodeBinderList);

            u32 dwRet = PROCMSG_OK;

            dwRet = PostMsgReqTo3AS(*pcReq);

            if ( PROCMSG_DEL == dwRet )
            {
                TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]发送请求[%s--%d]，TAS不在线，直接拒绝请求\n",
                    m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
                    pcReq->GetEventStr().c_str(), pcReq->GetEventId());
                CMS_ASSERT(NULL != m_ptRecvNode);

                CCuTvwallDelRsp cRsp;
                cRsp.SetSeqNum(pcReq->GetSeqNum());
                cRsp.SetSession(pcReq->GetSession());
                cRsp.SetErrorCode(ERR_CUI_TAS_OFFLINE);
                SendRspToCu<CCuTvwallDelRsp>(cRsp, m_ptRecvNode);
                NextState(NOT_REQUEST_STATE);
                delete this;
                return dwRet;
            }

            m_strReq = pcReq->ToXml();
            //2 转换状态，等待应答
            NextState(WAIT_RESPONSE_STATE);
            //3 设置超时消息处理

            return PROCMSG_OK;
        }
        break;
    case TAS_DEVICE_MOD_REQ:
        {
            //OnNotReqMsg<CCuDeviceNameModReq>(pcMsg, SERVER_3AS);
            CCuDeviceNameModReq *pcReq = (CCuDeviceNameModReq *)pcMsg->content;
            //1 向服务器发送请求, 请求类型必须是服务器定义的接口类型
            pcReq->SetDeviceUUID(GetNameWithoutDomainFromURI(pcReq->GetDeviceUUID()));

            if (!m_pcCuAgent->IsHasDevPrivilege(pcReq->GetDeviceUUID(), CEncoderPrivilege::en_Param_Config))
            {
                CEventRsp cRsp;
                cRsp.SetSeqNum(pcReq->GetSeqNum());
                cRsp.SetSession(pcReq->GetSession());
                cRsp.SetErrorCode(ERR_CUI_CU_NO_PERMISSION);
                SendRspToCu<CEventRsp>(cRsp, m_ptRecvNode);
                NextState(NOT_REQUEST_STATE);
                delete this;
                return PROCMSG_DEL;
            }

            u32 dwRet = PROCMSG_OK;

            dwRet = PostMsgReqTo3AS(*pcReq);

            if ( PROCMSG_DEL == dwRet )
            {
                TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]发送请求[%s--%d]，TAS不在线，直接拒绝请求\n",
                    m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
                    pcReq->GetEventStr().c_str(), pcReq->GetEventId());
                CMS_ASSERT(NULL != m_ptRecvNode);

                CEventRsp cRsp;
                cRsp.SetSeqNum(pcReq->GetSeqNum());
                cRsp.SetSession(pcReq->GetSession());
                cRsp.SetErrorCode(ERR_CUI_TAS_OFFLINE);
                SendRspToCu<CEventRsp>(cRsp, m_ptRecvNode);
                NextState(NOT_REQUEST_STATE);
                delete this;
                return dwRet;
            }

            m_strReq = pcReq->ToXml();
            //2 转换状态，等待应答
            NextState(WAIT_RESPONSE_STATE);
            //3 设置超时消息处理

            return PROCMSG_OK;
        }
        break;
        //     case TAS_DEVICE_CAP_INDEX_NAME_SET_REQ:
        //         {
        //             OnNotReqMsg<CCuDeviceCapIndexNameSetReq>(pcMsg, SERVER_3AS);
        //         }
        //         break;
	case TAS_CUSER_DEVICE_GROUP_ADD_REQ:
		{
			CCuUserDeviceGroupAddReq *pcReq = (CCuUserDeviceGroupAddReq*)pcMsg->content;

			pcReq->SetDevGrpId(GetNameWithoutDomainFromURI(pcReq->GetDevGrpId()));
			pcReq->SetUserId(GetNameWithoutDomainFromURI(pcReq->GetUserId()));
			pcReq->SetOperId(m_pcCuAgent->m_pcUserAccount->m_tUserID);

			u32 dwRet = PROCMSG_OK;
			dwRet = PostMsgReqTo3AS(*pcReq);

			if ( PROCMSG_DEL == dwRet )
			{
				TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]发送请求[%s--%d]，TAS不在线，直接拒绝请求\n",
					m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
					pcReq->GetEventStr().c_str(), pcReq->GetEventId());
				CMS_ASSERT(NULL != m_ptRecvNode);
				CEventRsp cRsp;
				cRsp.SetSeqNum(pcReq->GetSeqNum());
				cRsp.SetSession(pcReq->GetSession());
				cRsp.SetErrorCode(ERR_CUI_TAS_OFFLINE);
				SendRspToCu<CEventRsp>(cRsp, m_ptRecvNode);
				NextState(NOT_REQUEST_STATE);
				delete this;
				return dwRet;
			}

			m_strReq = pcReq->ToXml();
			//2 转换状态，等待应答
			NextState(WAIT_RESPONSE_STATE);
			//3 设置超时消息处理
			return PROCMSG_OK;
		}
		break;
	case TAS_DEVTREE_DEVGROUP_ASSIGN_REQ:
	{
		CCuDevTreeDevGrpAssignReq *pcReq = (CCuDevTreeDevGrpAssignReq*)pcMsg->content;

		pcReq->SetSrcGroupId(GetNameWithoutDomainFromURI(pcReq->GetSrcGroupId()));
		pcReq->SetDstParentGrpId(GetNameWithoutDomainFromURI(pcReq->GetDstParentGrpId()));

		u32 dwRet = PROCMSG_OK;
		dwRet = PostMsgReqTo3AS(*pcReq);

		if (PROCMSG_DEL == dwRet)
		{
			TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]发送请求[%s--%d]，TAS不在线，直接拒绝请求\n",
				m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
				pcReq->GetEventStr().c_str(), pcReq->GetEventId());
			CMS_ASSERT(NULL != m_ptRecvNode);
			CEventRsp cRsp;
			cRsp.SetSeqNum(pcReq->GetSeqNum());
			cRsp.SetSession(pcReq->GetSession());
			cRsp.SetErrorCode(ERR_CUI_TAS_OFFLINE);
			SendRspToCu<CEventRsp>(cRsp, m_ptRecvNode);
			NextState(NOT_REQUEST_STATE);
			delete this;
			return dwRet;
		}

		m_strReq = pcReq->ToXml();
		//2 转换状态，等待应答
		NextState(WAIT_RESPONSE_STATE);
		//3 设置超时消息处理
		return PROCMSG_OK;
	}
	break;
	case TAS_CUSER_DEVICE_GROUP_DEL_REQ:
		{
			CCuUserDeviceGroupDelReq *pcReq = (CCuUserDeviceGroupDelReq*)pcMsg->content;

			pcReq->SetDevGrpId(GetNameWithoutDomainFromURI(pcReq->GetDevGrpId()));
			pcReq->SetUserId(GetNameWithoutDomainFromURI(pcReq->GetUserId()));
			pcReq->SetOperId(m_pcCuAgent->m_pcUserAccount->m_tUserID);

			u32 dwRet = PROCMSG_OK;
			dwRet = PostMsgReqTo3AS(*pcReq);
			if ( PROCMSG_DEL == dwRet )
			{
				TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]发送请求[%s--%d]，TAS不在线，直接拒绝请求\n",
					m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
					pcReq->GetEventStr().c_str(), pcReq->GetEventId());
				CMS_ASSERT(NULL != m_ptRecvNode);
				CEventRsp cRsp;
				cRsp.SetSeqNum(pcReq->GetSeqNum());
				cRsp.SetSession(pcReq->GetSession());
				cRsp.SetErrorCode(ERR_CUI_TAS_OFFLINE);
				SendRspToCu<CEventRsp>(cRsp, m_ptRecvNode);
				NextState(NOT_REQUEST_STATE);
				delete this;
				return dwRet;
			}

			m_strReq = pcReq->ToXml();
			//2 转换状态，等待应答
			NextState(WAIT_RESPONSE_STATE);
			//3 设置超时消息处理
			return PROCMSG_OK;
		}
		break;
	case TAS_CUSER_GROUP_DEVICE_GROUP_ADD_REQ:
		{
			CCuUserGroupDeviceGroupAddReq *pcReq = (CCuUserGroupDeviceGroupAddReq*)pcMsg->content;

			pcReq->SetUserGroupId(GetNameWithoutDomainFromURI(pcReq->GetUserGroupId()));
			pcReq->SetDevGrpId(GetNameWithoutDomainFromURI(pcReq->GetDevGrpId()));
			pcReq->SetOperId(m_pcCuAgent->m_pcUserAccount->m_tUserID);

			u32 dwRet = PROCMSG_OK;
			dwRet = PostMsgReqTo3AS(*pcReq);

			if ( PROCMSG_DEL == dwRet )
			{
				TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]发送请求[%s--%d]，TAS不在线，直接拒绝请求\n",
					m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
					pcReq->GetEventStr().c_str(), pcReq->GetEventId());
				CMS_ASSERT(NULL != m_ptRecvNode);
				CEventRsp cRsp;
				cRsp.SetSeqNum(pcReq->GetSeqNum());
				cRsp.SetSession(pcReq->GetSession());
				cRsp.SetErrorCode(ERR_CUI_TAS_OFFLINE);
				SendRspToCu<CEventRsp>(cRsp, m_ptRecvNode);
				NextState(NOT_REQUEST_STATE);
				delete this;
				return dwRet;
			}

			m_strReq = pcReq->ToXml();
			//2 转换状态，等待应答
			NextState(WAIT_RESPONSE_STATE);
			//3 设置超时消息处理
			return PROCMSG_OK;
		}
		break;
	case TAS_CUSER_GROUP_DEVICE_GROUP_DEL_REQ:
		{
			CCuUserGroupDeviceGroupDelReq *pcReq = (CCuUserGroupDeviceGroupDelReq*)pcMsg->content;

			pcReq->SetUserGroupId(GetNameWithoutDomainFromURI(pcReq->GetUserGroupId()));
			pcReq->SetDevGrpId(GetNameWithoutDomainFromURI(pcReq->GetDevGrpId()));
			pcReq->SetOperId(m_pcCuAgent->m_pcUserAccount->m_tUserID);

			u32 dwRet = PROCMSG_OK;
			dwRet = PostMsgReqTo3AS(*pcReq);

			if ( PROCMSG_DEL == dwRet )
			{
				TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]发送请求[%s--%d]，TAS不在线，直接拒绝请求\n",
					m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
					pcReq->GetEventStr().c_str(), pcReq->GetEventId());
				CMS_ASSERT(NULL != m_ptRecvNode);
				CEventRsp cRsp;
				cRsp.SetSeqNum(pcReq->GetSeqNum());
				cRsp.SetSession(pcReq->GetSession());
				cRsp.SetErrorCode(ERR_CUI_TAS_OFFLINE);
				SendRspToCu<CEventRsp>(cRsp, m_ptRecvNode);
				NextState(NOT_REQUEST_STATE);
				delete this;
				return dwRet;
			}

			m_strReq = pcReq->ToXml();
			//2 转换状态，等待应答
			NextState(WAIT_RESPONSE_STATE);
			//3 设置超时消息处理
			return PROCMSG_OK;
		}
		break;
	case TAS_DEVICE_AUTO_ASSIGN_TO_USER_SET_REQ:
		{
			CCuDeviceAutoAssignToUserSetReq *pcReq = (CCuDeviceAutoAssignToUserSetReq*)pcMsg->content;

			pcReq->SetUserId(GetNameWithoutDomainFromURI(pcReq->GetUserId()));
			pcReq->SetDevGrpId(GetNameWithoutDomainFromURI(pcReq->GetDevGrpId()));
			pcReq->SetOperId(m_pcCuAgent->m_pcUserAccount->m_tUserID);

			u32 dwRet = PROCMSG_OK;
			dwRet = PostMsgReqTo3AS(*pcReq);

			if ( PROCMSG_DEL == dwRet )
			{
				TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]发送请求[%s--%d]，TAS不在线，直接拒绝请求\n",
					m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
					pcReq->GetEventStr().c_str(), pcReq->GetEventId());
				CMS_ASSERT(NULL != m_ptRecvNode);
				CEventRsp cRsp;
				cRsp.SetSeqNum(pcReq->GetSeqNum());
				cRsp.SetSession(pcReq->GetSession());
				cRsp.SetErrorCode(ERR_CUI_TAS_OFFLINE);
				SendRspToCu<CEventRsp>(cRsp, m_ptRecvNode);
				NextState(NOT_REQUEST_STATE);
				delete this;
				return dwRet;
			}

			m_strReq = pcReq->ToXml();
			//2 转换状态，等待应答
			NextState(WAIT_RESPONSE_STATE);
			//3 设置超时消息处理
			return PROCMSG_OK;
		}
		break;
	case TAS_CUSER_DATA_VERSION_GET_REQ:
		{
			CCuUserDataVersionGetReq *pcReq = (CCuUserDataVersionGetReq*)pcMsg->content;

			u32 dwRet = PROCMSG_OK;
			dwRet = PostMsgReqTo3AS(*pcReq);

			if ( PROCMSG_DEL == dwRet )
			{
				TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]发送请求[%s--%d]，TAS不在线，直接拒绝请求\n",
					m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
					pcReq->GetEventStr().c_str(), pcReq->GetEventId());
				CMS_ASSERT(NULL != m_ptRecvNode);
				CEventRsp cRsp;
				cRsp.SetSeqNum(pcReq->GetSeqNum());
				cRsp.SetSession(pcReq->GetSession());
				cRsp.SetErrorCode(ERR_CUI_TAS_OFFLINE);
				SendRspToCu<CEventRsp>(cRsp, m_ptRecvNode);
				NextState(NOT_REQUEST_STATE);
				delete this;
				return dwRet;
			}

			m_strReq = pcReq->ToXml();
			//2 转换状态，等待应答
			NextState(WAIT_RESPONSE_STATE);
			//3 设置超时消息处理
			return PROCMSG_OK;
		}
		break;
	case TAS_DEVICE_AUTO_ASSIGN_TO_USER_GROUP_SET_REQ:
		{
			CCuDeviceAutoAssignToUserGroupSetReq *pcReq = (CCuDeviceAutoAssignToUserGroupSetReq*)pcMsg->content;

			pcReq->SetUserGroupId(GetNameWithoutDomainFromURI(pcReq->GetUserGroupId()));
			pcReq->SetDevGrpId(GetNameWithoutDomainFromURI(pcReq->GetDevGrpId()));
			pcReq->SetOperId(m_pcCuAgent->m_pcUserAccount->m_tUserID);

			u32 dwRet = PROCMSG_OK;
			dwRet = PostMsgReqTo3AS(*pcReq);

			if ( PROCMSG_DEL == dwRet )
			{
				TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]发送请求[%s--%d]，TAS不在线，直接拒绝请求\n",
					m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
					pcReq->GetEventStr().c_str(), pcReq->GetEventId());
				CMS_ASSERT(NULL != m_ptRecvNode);
				CEventRsp cRsp;
				cRsp.SetSeqNum(pcReq->GetSeqNum());
				cRsp.SetSession(pcReq->GetSession());
				cRsp.SetErrorCode(ERR_CUI_TAS_OFFLINE);
				SendRspToCu<CEventRsp>(cRsp, m_ptRecvNode);
				NextState(NOT_REQUEST_STATE);
				delete this;
				return dwRet;
			}

			m_strReq = pcReq->ToXml();
			//2 转换状态，等待应答
			NextState(WAIT_RESPONSE_STATE);
			//3 设置超时消息处理
			return PROCMSG_OK;
		}
		break;
	case TAS_TRANSACTION_PROGRESS_GET_REQ:
		{
			CCuTransactionProgressGetReq *pcReq = (CCuTransactionProgressGetReq*)pcMsg->content;

			u32 dwRet = PROCMSG_OK;
			dwRet = PostMsgReqTo3AS(*pcReq);

			if ( PROCMSG_DEL == dwRet )
			{
				TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]发送请求[%s--%d]，TAS不在线，直接拒绝请求\n",
					m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
					pcReq->GetEventStr().c_str(), pcReq->GetEventId());
				CMS_ASSERT(NULL != m_ptRecvNode);
				CEventRsp cRsp;
				cRsp.SetSeqNum(pcReq->GetSeqNum());
				cRsp.SetSession(pcReq->GetSession());
				cRsp.SetErrorCode(ERR_CUI_TAS_OFFLINE);
				SendRspToCu<CEventRsp>(cRsp, m_ptRecvNode);
				NextState(NOT_REQUEST_STATE);
				delete this;
				return dwRet;
			}

			m_strReq = pcReq->ToXml();
			//2 转换状态，等待应答
			NextState(WAIT_RESPONSE_STATE);
			//3 设置超时消息处理
			return PROCMSG_OK;
		}
		break;
	case TAS_DEVGRP_VIDSRC_TOTALNUM_GET_REQ:
		{
			CDevGrpVidsrcTotalnumGetReq *pcReq = (CDevGrpVidsrcTotalnumGetReq*)pcMsg->content;

			u32 dwRet = PROCMSG_OK;
			dwRet = PostMsgReqTo3AS(*pcReq);

			if (PROCMSG_DEL == dwRet)
			{
				TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]发送请求[%s--%d]，TAS不在线，直接拒绝请求\n",
					m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
					pcReq->GetEventStr().c_str(), pcReq->GetEventId());
				CMS_ASSERT(NULL != m_ptRecvNode);
				CEventRsp cRsp;
				cRsp.SetSeqNum(pcReq->GetSeqNum());
				cRsp.SetSession(pcReq->GetSession());
				cRsp.SetErrorCode(ERR_CUI_TAS_OFFLINE);
				SendRspToCu<CEventRsp>(cRsp, m_ptRecvNode);
				NextState(NOT_REQUEST_STATE);
				delete this;
				return dwRet;
			}

			m_strReq = pcReq->ToXml();
			//2 转换状态，等待应答
			NextState(WAIT_RESPONSE_STATE);
			//3 设置超时消息处理
			return PROCMSG_OK;
		}
		break;
	case TAS_DEVICE_MATCH_GBID_REQ:
		{
			CCuDeviceMatchGbidReq *pcReq = (CCuDeviceMatchGbidReq *)pcMsg->content;

			pcReq->SetDevGrpId(GetNameWithoutDomainFromURI(pcReq->GetDevGrpId()));
			pcReq->SetDevId(GetNameWithoutDomainFromURI(pcReq->GetDevId()));

			u32 dwRet = PROCMSG_OK;

			dwRet = PostMsgReqTo3AS(*pcReq);

			if (PROCMSG_DEL == dwRet)
			{
				TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]发送请求[%s--%d]，TAS不在线，直接拒绝请求\n",
					m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
					pcReq->GetEventStr().c_str(), pcReq->GetEventId());
				CMS_ASSERT(NULL != m_ptRecvNode);

				CCuDeviceMatchGbidRsp cRsp;
				cRsp.SetSeqNum(pcReq->GetSeqNum());
				cRsp.SetSession(pcReq->GetSession());
				cRsp.SetErrorCode(ERR_CUI_TAS_OFFLINE);
				SendRspToCu<CCuDeviceMatchGbidRsp>(cRsp, m_ptRecvNode);
				NextState(NOT_REQUEST_STATE);
				delete this;
				return dwRet;
			}

			m_strReq = pcReq->ToXml();
			//2 转换状态，等待应答
			NextState(WAIT_RESPONSE_STATE);
			//3 设置超时消息处理

			return PROCMSG_OK;
		}
		break;
	case TAS_DEVICE_GROUP_DEVICE_SUMNUM_REQ:
		{
			CCuDeviceGrpDevSumNumReq *pcReq = (CCuDeviceGrpDevSumNumReq *)pcMsg->content;

			pcReq->SetDevGrpId(GetNameWithoutDomainFromURI(pcReq->GetDevGrpId()));

			u32 dwRet = PROCMSG_OK;

			dwRet = PostMsgReqTo3AS(*pcReq);

			if (PROCMSG_DEL == dwRet)
			{
				TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]发送请求[%s--%d]，TAS不在线，直接拒绝请求\n",
					m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
					pcReq->GetEventStr().c_str(), pcReq->GetEventId());
				CMS_ASSERT(NULL != m_ptRecvNode);

				CCuDeviceGrpDevSumNumRsp cRsp;
				cRsp.SetSeqNum(pcReq->GetSeqNum());
				cRsp.SetSession(pcReq->GetSession());
				cRsp.SetErrorCode(ERR_CUI_TAS_OFFLINE);
				SendRspToCu<CCuDeviceGrpDevSumNumRsp>(cRsp, m_ptRecvNode);
				NextState(NOT_REQUEST_STATE);
				delete this;
				return dwRet;
			}

			m_strReq = pcReq->ToXml();
			//2 转换状态，等待应答
			NextState(WAIT_RESPONSE_STATE);
			//3 设置超时消息处理

			return PROCMSG_OK;
		}
		break;
	case TAS_DEVICE_GROUP_DEVICE_MOD_4GB_REQ:
		{
			CCuDeviceGroupDeviceMod4GbReq *pcReq = (CCuDeviceGroupDeviceMod4GbReq *)pcMsg->content;

			pcReq->SetDeviceId(GetNameWithoutDomainFromURI(pcReq->GetDeviceId()));
			pcReq->SetDeviceGroupID(GetNameWithoutDomainFromURI(pcReq->GetDeviceGroupID()));

			u32 dwRet = PROCMSG_OK;

			dwRet = PostMsgReqTo3AS(*pcReq);

			if (PROCMSG_DEL == dwRet)
			{
				TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]发送请求[%s--%d]，TAS不在线，直接拒绝请求\n",
					m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
					pcReq->GetEventStr().c_str(), pcReq->GetEventId());
				CMS_ASSERT(NULL != m_ptRecvNode);

				CCuDeviceGroupDeviceMod4GbRsp cRsp;
				cRsp.SetSeqNum(pcReq->GetSeqNum());
				cRsp.SetSession(pcReq->GetSession());
				cRsp.SetErrorCode(ERR_CUI_TAS_OFFLINE);
				SendRspToCu<CCuDeviceGroupDeviceMod4GbRsp>(cRsp, m_ptRecvNode);
				NextState(NOT_REQUEST_STATE);
				delete this;
				return dwRet;
			}

			m_strReq = pcReq->ToXml();
			//2 转换状态，等待应答
			NextState(WAIT_RESPONSE_STATE);
			//3 设置超时消息处理

			return PROCMSG_OK;
		}
		break;
	case TAS_DEVICE_GBID_ERROR_GET_REQ:
		{
			CCuDeviceGbidErrGetReq *pcReq = (CCuDeviceGbidErrGetReq *)pcMsg->content;

			pcReq->SetGroupId(GetNameWithoutDomainFromURI(pcReq->GetGroupId()));

			u32 dwRet = PROCMSG_OK;

			dwRet = PostMsgReqTo3AS(*pcReq);

			if (PROCMSG_DEL == dwRet)
			{
				TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]发送请求[%s--%d]，TAS不在线，直接拒绝请求\n",
					m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
					pcReq->GetEventStr().c_str(), pcReq->GetEventId());
				CMS_ASSERT(NULL != m_ptRecvNode);

				CCuDeviceGbidErrGetRsp cRsp;
				cRsp.SetSeqNum(pcReq->GetSeqNum());
				cRsp.SetSession(pcReq->GetSession());
				cRsp.SetErrorCode(ERR_CUI_TAS_OFFLINE);
				SendRspToCu<CCuDeviceGbidErrGetRsp>(cRsp, m_ptRecvNode);
				NextState(NOT_REQUEST_STATE);
				delete this;
				return dwRet;
			}

			m_strReq = pcReq->ToXml();
			//2 转换状态，等待应答
			NextState(WAIT_RESPONSE_STATE);
			//3 设置超时消息处理

			return PROCMSG_OK;
		}
		break;
    case PU_VID_ENC_PARAM_GET_REQ:
        {
            OnNotReqMsg<CCuPuVidEncParamGetReq>(pcMsg);
        }
        break;
    case PU_VID_ENC_PARAM_SET_REQ:
        {
            CCuPuVidEncParamSetReq *pcReq = (CCuPuVidEncParamSetReq *)pcMsg->content;

            if (!m_pcCuAgent->IsHasDevPrivilege(pcReq->GetDevChn().GetDevUri(), CEncoderPrivilege::en_Param_Config))
            {
                CCuPuVidEncParamSetRsp cRsp;
                cRsp.SetSeqNum(pcReq->GetSeqNum());
                cRsp.SetSession(pcReq->GetSession());
                cRsp.SetErrorCode(ERR_CUI_CU_NO_PERMISSION);
                SendRspToCu<CCuPuVidEncParamSetRsp>(cRsp, m_ptRecvNode);
                NextState(NOT_REQUEST_STATE);
                delete this;
                return PROCMSG_DEL;
            }

            OnNotReqMsg<CCuPuVidEncParamSetReq>(pcMsg);
        }
		break;
	case PU_AUD_ENC_PARAM_GET_REQ:
		{
			OnNotReqMsg<CCuPuAudEncParamGetReq>(pcMsg);
		}
		break;
	case PU_VIDSRC_OSD_SET_REQ:
		{
			OnNotReqMsg<CCuPuVidSrcOsdSetReq>(pcMsg);
		}
		break;
	case PU_VIDSRC_OSD_GET_REQ:
		{
			OnNotReqMsg<CCuPuVidSrcOsdGetReq>(pcMsg);
		}
		break;
	case PU_RECORD_START_REQ:
		{
			OnNotReqMsg<CCuPuRecordStartReq>(pcMsg);
		}
		break;
	case PU_RECORD_STOP_REQ:
		{
			OnNotReqMsg<CCuPuRecordStopReq>(pcMsg);
		}
		break;
    case PU_CAMERA_TYPE_SET_REQ:
        {
            OnNotReqMsg<CCuPuCameraTypeSetReq>(pcMsg);
        }
        break;
	case PU_ALARM_RESET_REQ:
		{
			OnNotReqMsg<CCuPuAlarmResetReq>(pcMsg);
		}
		break;
	case PU_SERIALPARAM_CFG_GET_REQ:
		{
			OnNotReqMsg<CCuPuSerialParamCfgGetReq>(pcMsg);
		}
		break;
	case PU_SERIALPARAM_CFG_SET_REQ:
		{
			OnNotReqMsg<CCuPuSerialParamCfgSetReq>(pcMsg);
		}
		break;
    case PU_MOTION_DETECT_GET_REQ:
        {
            OnNotReqMsg<CCuPuMotionDetectGetReq>(pcMsg);
        }
        break;
    case PU_MOTION_DETECT_SET_REQ:
        {
            OnNotReqMsg<CCuPuMotionDetectSetReq>(pcMsg);
        }
		break;
	case CONFIG_SYNC_SWITCH_GET_REQ:
		{
			OnNotReqMsg<CCuGetSyncVideoSrcNameEnableReq>(pcMsg);
		}
		break;
	case CUI_GET_AZIMUTH_REQ:
		{
			OnNotReqMsg<CCuGetAzimuthReq>(pcMsg);
		}
		break;
	case CONFIG_SYNC_SWITCH_SET_REQ:
		{
			OnNotReqMsg<CCuSetSyncVideoSrcNameEnableReq>(pcMsg);
		}
		break;
    case PU_AREA_SHADE_GET_REQ:
        {
            OnNotReqMsg<CCuPuAreaShadeGetReq>(pcMsg);
        }
        break;
    case PU_AREA_SHADE_SET_REQ:
        {
            OnNotReqMsg<CCuPuAreaShadeSetReq>(pcMsg);
        }
        break;
	case CU_CUI_SECURE_CERTIFICATE_QRY_REQ:
		{
			CCertificateQryReq *pcReq = (CCertificateQryReq *)pcMsg->content;
			CSecurityModuleCertificateQueryReq cSecReq;
			cSecReq.SetId( pcReq->GetId() );
			cSecReq.SetModuleId( pcReq->GetModuleId() );

			FakePostMsgReq(cSecReq, g_cCuiConfig.GetSecureModuleURI() );
			NextState(WAIT_RESPONSE_STATE);
		}
		break;
	case PU_SIGNATURE_CTRL_REQ:
		{
			OnNotReqMsg< CPuSignatrueCtrlReq >(pcMsg);
		}
		break;
	case PU_ENCRYPTION_CTRL_REQ:
		{
			OnNotReqMsg< CPuEncryptionCtrlReq >(pcMsg);
		}
		break;
    default:
        {
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"收到未知消息");
        }
        break;
    }
    return PROCMSG_OK;
}

u32 CSingleInterAction::WaitRspProcMsg( CMessage *const pcMsg )
{
    COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
    TASKLOG(NORMAL_TASK, EVENT_LEV, "CSingleInterAction[TaskNO %u]收到服务器[%s]的应答消息[%s--%hu]\n",
        GetTaskNO(), pcOspSipMsg->GetSipToUri().GetURIString().c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

    switch (pcMsg->event)
    {
    case OSP_SIP_MSG_PROC_FAIL:
        {
            CMS_ASSERT(NULL != m_ptRecvNode);

            CEventRsp cRsp;
            cRsp.SetErrorCode(ERR_CUI_SIP_MSG_FAIL);
            SendRspToCu<CEventRsp>(cRsp, m_ptRecvNode);

            WriteAbnormalLog(ERR_CUI_SIP_MSG_FAIL);

            NextState(NOT_REQUEST_STATE);
            delete this;
        }
        break;
    case OSP_SIP_DIALOG_TERMINATE:
        {
            //不会有Dlg消息
            CMS_ASSERT( 0 && "CSingleInterAction::WaitRspProcMsg不可能出现会话内消息" );
        }
        break;
	case PU_LONG_OSD_SET_RSP:
		{
			CCuPuLongOsdSetRsp cRsp;
			GetRspMsg(cRsp, pcMsg);

			OnWaitRspMsg<CCuPuLongOsdSetRsp>(cRsp);
		}
		break;
	case PU_LONG_OSD_GET_RSP:
		{
			CCuPuLongOsdGetRsp cRsp;
			GetRspMsg(cRsp, pcMsg);

			OnWaitRspMsg<CCuPuLongOsdGetRsp>(cRsp);
		}
		break;
    case CU_GETCUILIST_RSP:
        {
            CCuGetCuiListRsp cRsp;
            GetRspMsg(cRsp, pcMsg);
            /*if (m_pcCuAgent->m_tNatInfo.GetIsHasNat())
            {
            for (CIpAddrList::iterator it = cRsp.GetIpAddrList().begin();it!=cRsp.GetIpAddrList().end();it++)
            {
            TNetAddr tNetAddr;
            if (g_cCuiConfig.m_cCmCfg.FindNetNatAddr(*it,tNetAddr))
            {
            it->SetNetIp(tNetAddr.GetNetIp());
            it->SetNetPort(tNetAddr.GetNetPort());
            }
            }
            }*/
            OnWaitRspMsg<CCuGetCuiListRsp>(cRsp);
        }
        break;
    case PU_PTZ_CTRL_RSP:
        {
            CPuPtzCtrlRsp cRsp;
            GetRspMsg(cRsp, pcMsg);        

            CPuPtzCtrlReq cReq;
            cReq.FromXml(m_strReq);

            string strDevDomain = GetDomainNameWithoutPreNameFromURI(cReq.GetDevChn().GetDevUri());


            //日志相关
            WriteLog(cReq.GetDevChn(), cRsp.GetErrorCode(), cReq.GetEventId(), GetDomainName());

            if(m_pcCuAgent->m_tCmuURI.GetDomain() != strDevDomain)
            {
                WriteLog(cReq.GetDevChn(), cRsp.GetErrorCode(), cReq.GetEventId(), strDevDomain);
            }

            OnWaitRspMsg<CPuPtzCtrlRsp>(cRsp);
        }
        break;
    case PU_PTZ_LOCK_RSP:
        {
            CPuPtzLockRsp cRsp;
            GetRspMsg(cRsp, pcMsg); 

            CPuPtzLockReq cReq;
            cReq.FromXml(m_strReq);

            string strLogDesc = GetOptLogDesc(LAN_DESC_PTZ_LOCK , GetLanDesc(LAN_DESC_DEVICE_ID) + " "
                + cReq.GetPuChn().GetDevUri() + ":" 
                + TransInt2String(cReq.GetPuChn().GetChnNO()+1), cRsp.GetErrorCode());

            WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypePtzCtrl, (u32)cRsp.GetErrorCode(), strLogDesc,
                cReq.GetPuChn().GetDevUri());

            string strDevDomain = GetDomainNameWithoutPreNameFromURI(cReq.GetPuChn().GetDevUri());
            if(m_pcCuAgent->m_tCmuURI.GetDomain() != strDevDomain)
            {
                WriteLog(strDevDomain, enUserOpTypePtzCtrl, (u32)cRsp.GetErrorCode(), strLogDesc,
                    cReq.GetPuChn().GetDevUri());
            }

            OnWaitRspMsg<CPuPtzLockRsp>(cRsp);
        }
        break;
    case PU_PTZ_UNLOCK_RSP:
        {
            CPuPtzUnlockRsp cRsp;
            GetRspMsg(cRsp, pcMsg); 

            CPuPtzUnlockReq cReq;
            cReq.FromXml(m_strReq);

            string strLogDesc = GetOptLogDesc(LAN_DESC_PTZ_UNLOCK , GetLanDesc(LAN_DESC_DEVICE_ID) + ":"
                + cReq.GetPuChn().GetDevUri() + " " + GetLanDesc(LAN_DESC_DEV_CHN) + ":"
                + TransInt2String(cReq.GetPuChn().GetChnNO()+1), cRsp.GetErrorCode());


            WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypePtzCtrl, (u32)cRsp.GetErrorCode(), 
                strLogDesc, cReq.GetPuChn().GetDevUri());

            string strDevDomain = GetDomainNameWithoutPreNameFromURI(cReq.GetPuChn().GetDevUri());
            if(m_pcCuAgent->m_tCmuURI.GetDomain() != strDevDomain)
            {
                WriteLog(strDevDomain, enUserOpTypePtzCtrl, (u32)cRsp.GetErrorCode(), strLogDesc,
                    cReq.GetPuChn().GetDevUri());
            }

            OnWaitRspMsg<CPuPtzUnlockRsp>(cRsp);
        }
        break;
	case TAS_PTZLOCK_ADD_RSP:
		{
			CCuPtzLockAddRsp cRsp;
			GetRspMsg(cRsp, pcMsg);

			CCuPtzLockAddReq cReq;
			cReq.FromXml(m_strReq);

			CPtzLockInfo &lockInfo = cReq.GetCPtzLockInfo();

			if (cRsp.GetErrorCode() == CMS_SUCCESS)
			{
				TChannel vsChn;
				vsChn.SetDevUri(cReq.GetCPtzLockInfo().GetDeviceId());
				vsChn.SetChnNO(cReq.GetCPtzLockInfo().GetVideoSrcId());

				WriteLog(vsChn, cRsp.GetErrorCode(), cReq.GetEventId(), GetDomainName());
			}
			//异域设备不能加锁
			//string strDevDomain = GetDomainNameWithoutPreNameFromURI(cReq.GetCPtzLockInfo().GetDeviceId());
			//if (m_pcCuAgent->m_tCmuURI.GetDomain() != strDevDomain)
			//{
			//	WriteLog(strDevDomain, enUserOpTypePtzCtrl, (u32)cRsp.GetErrorCode(), strLogDesc,
			//		cReq.GetCPtzLockInfo().GetDeviceId());
			//}

			// 加锁成功，保存结果
			C3acAgent *pc3acAgent = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(m_pcCuAgent->m_str3asName, this->GetInstance());
			if (NULL == pc3acAgent)
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "收到来自CU的消息[%s--%hu]，但是没有对应的3acAgent\n",
					OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
			}
			else
			{
				if (cRsp.GetErrorCode() == CMS_SUCCESS)
				{
					// 加锁成功使用加锁信息添加锁
					if (!cReq.GetCPtzLockInfo().GetDeviceId().empty()
						&& !cReq.GetCPtzLockInfo().GetUserId().empty())
					{
						CCuPtzLockInfo ptzLockInfo;
						ptzLockInfo.SetDeviceId(cReq.GetCPtzLockInfo().GetDeviceId());
						ptzLockInfo.SetVideoSrcId(cReq.GetCPtzLockInfo().GetVideoSrcId());
						ptzLockInfo.SetUserId(cReq.GetCPtzLockInfo().GetUserId());
						ptzLockInfo.SetUserName(m_pcCuAgent->m_strUserName);
						ptzLockInfo.SetPassword(cReq.GetCPtzLockInfo().GetPassword());
						ptzLockInfo.SetBeginTime(cReq.GetCPtzLockInfo().GetBeginTime());
						ptzLockInfo.SetEndTime(cReq.GetCPtzLockInfo().GetEndTime());

						if (!pc3acAgent->m_cPtzlockInfoList.AddCuPtzLockInfo(ptzLockInfo))
						{
							TASKLOG(NORMAL_TASK, ERROR_LEV, "添加PTZ锁定信息失败\n");
							this->PrintData();
						}
					}
				}
				else
				{
					// 加锁失败，会返回现在的锁信息，所以需要更新
					if (!cReq.GetCPtzLockInfo().GetDeviceId().empty()
						&& !cRsp.GetUserId().empty())
					{
						CCuPtzLockInfo ptzLockInfo;
						ptzLockInfo.SetDeviceId(cReq.GetCPtzLockInfo().GetDeviceId());
						ptzLockInfo.SetVideoSrcId(cReq.GetCPtzLockInfo().GetVideoSrcId());

						ptzLockInfo.SetUserId(cRsp.GetUserId());
						ptzLockInfo.SetUserName(cRsp.GetUsername());
						ptzLockInfo.SetUserPtzLevel(cRsp.GetPriPtzLevel());
						ptzLockInfo.SetBeginTime(cRsp.GetBeginTime());
						ptzLockInfo.SetEndTime(cRsp.GetEndTime());
						
						if (!pc3acAgent->m_cPtzlockInfoList.AddCuPtzLockInfo(ptzLockInfo))
						{
							TASKLOG(NORMAL_TASK, ERROR_LEV, "添加PTZ锁定信息失败\n");
							this->PrintData();
						}
					}
				}
			}
			OnWaitRspMsg<CCuPtzLockAddRsp>(cRsp);
		}
		break;
	case TAS_PTZLOCK_DEL_RSP:
		{
			CCuPtzLockDelRsp cRsp;
			GetRspMsg(cRsp, pcMsg);

			CCuPtzLockDelReq cReq;
			cReq.FromXml(m_strReq);

			if (cRsp.GetErrorCode() == CMS_SUCCESS)
			{
				if (cReq.GetOperateType() == PTZLOCK_UNLOCK)
				{
					TChannel vsChn;
					vsChn.SetDevUri(cReq.GetDeviceId());
					vsChn.SetChnNO(cReq.GetVideoSrcId());

					WriteLog(vsChn, cRsp.GetErrorCode(), cReq.GetEventId(), GetDomainName());
				}
				else if (cReq.GetOperateType() == PTZLOCK_UNLOCKALL)
				{
					string strLogDesc = GetOptLogDesc(LAN_DESC_PTZ_UNLOCKAll, "",(u32)cRsp.GetErrorCode());
					WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypePtzCtrl, (u32)cRsp.GetErrorCode(), strLogDesc);
				}
			}

			//异域设备不能加锁
			//string strDevDomain = GetDomainNameWithoutPreNameFromURI(cReq.GetDeviceId());
			//if (m_pcCuAgent->m_tCmuURI.GetDomain() != strDevDomain)
			//{
			//	WriteLog(strDevDomain, enUserOpTypePtzCtrl, (u32)cRsp.GetErrorCode(), strLogDesc,
			//		cReq.GetDeviceId());
			//}

			C3acAgent *pc3acAgent = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(m_pcCuAgent->m_str3asName, this->GetInstance());
			if (NULL != pc3acAgent)
			{
				TChannel tVsChn;
				if (!cReq.GetDeviceId().empty())
				{
					tVsChn.SetDevUri(GetDomainNameWithoutPreNameFromURI(cReq.GetDeviceId()));
					tVsChn.SetChnNO(cReq.GetVideoSrcId());
				}

				if (cReq.GetOperateType() == PTZLOCK_UNLOCK
					&& cRsp.GetErrorCode() == ERR_TAS_RECORD_NOT_EXISTS )
				{
					// 数据库中没有数据，说明解锁成功了
					cRsp.SetErrorCode(CMS_SUCCESS);
				}

				// 操作成功，删除对应锁信息
				if (cRsp.GetErrorCode() == CMS_SUCCESS)
				{

					// 考虑到时序问题过期锁在发送消息前已经删除了，不用再删除
					// 时序问题：用户加锁前删除过期锁，如果删除过期锁应答比加锁应答晚收到，会删除CUI缓存的视频源对应锁，导致加锁无效。
					if (cReq.GetOperateType() == PTZLOCK_UNLOCK)
					{
						pc3acAgent->m_cPtzlockInfoList.DelCuPtzLockInfoByVs(tVsChn);
					}
					else if (cReq.GetOperateType() == PTZLOCK_UNLOCKALL)
					{
						pc3acAgent->m_cPtzlockInfoList.Clear();
					}
				}
				else
				{
					if ( !tVsChn.GetDevUri().empty()
						&& !cRsp.GetUserId().empty() )
					{
						// 如果解锁失败/删除锁失败，看看有没有锁信息，如果有锁信息就需要更新锁信息
						CCuPtzLockInfo cuPtzLockInfo;
						cuPtzLockInfo.SetDeviceId(tVsChn.GetDevUri());
						cuPtzLockInfo.SetVideoSrcId(tVsChn.GetChnNO());
						cuPtzLockInfo.SetUserId(cRsp.GetUserId());
						cuPtzLockInfo.SetUserName(cRsp.GetUsername());
						cuPtzLockInfo.SetUserPtzLevel(cRsp.GetPriPtzLevel());
						cuPtzLockInfo.SetBeginTime(cRsp.GetBeginTime());
						cuPtzLockInfo.SetEndTime(cRsp.GetEndTime());

						pc3acAgent->m_cPtzlockInfoList.AddCuPtzLockInfo(cuPtzLockInfo);
					}
				}
			}
			else
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "收到来自CU的消息[%s--%hu]，但是没有对应的3acAgent\n",
					OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
			}

			if ( cReq.GetOperateType() == PTZLOCK_DELETE )
			{
				//任务完成, 销毁之
				NextState(NOT_REQUEST_STATE);
				delete this;
				return PROCMSG_OK;
			}
			else
			{
				OnWaitRspMsg<CCuPtzLockDelRsp>(cRsp);
			}
		}
		break;
    case PU_TRANSPARENT_OPERATE_RSP:
        {
            CCuPuTransparentOperateRsp cRsp;
            GetRspMsg(cRsp, pcMsg);

            OnWaitRspMsg<CCuPuTransparentOperateRsp>(cRsp);
        }
        break;
    case KEY_FRAME_RSP:
        {
            CCuKeyFrameRsp cRsp;
            GetRspMsg(cRsp, pcMsg);

            OnWaitRspMsg<CCuKeyFrameRsp>(cRsp);
        }
        break;
    case PU_VSIP_OPERATE_RSP:
        {
            CCuPuVsipOperateRsp cRsp;
            GetRspMsg(cRsp, pcMsg);

            OnWaitRspMsg<CCuPuVsipOperateRsp>(cRsp);
        }
        break;
    case PU_REC_DAYS_QRY_RSP:
        {
            CCuPuRecDaysQryRsp cRsp;
            GetRspMsg(cRsp, pcMsg);

            OnWaitRspMsg<CCuPuRecDaysQryRsp>(cRsp);
        }
        break;
    case TAS_DEVICE_GROUP_ADD_RSP:
        {
            CDeviceGroupAddRsp cRsp;
            GetRspMsg(cRsp, pcMsg);

            CDeviceGroupAddReq cReq;
            cReq.FromXml(m_strReq);

            cRsp.GetDeviceGroupInfoKey().SetDeviceGroupID(cRsp.GetDeviceGroupInfoKey().GetDeviceGroupID() + "@" + pcOspSipMsg->GetSipToUri().GetDomain());

            string strLogDesc = GetOptLogDesc(LAN_DESC_ADD, LAN_DESC_DEVICE_GROUP , GetLanDesc(LAN_DESC_DEVICE_GROUP_ID) + ":" + 
                cRsp.GetDeviceGroupInfoKey().GetDeviceGroupID() + 
                " " + GetLanDesc(LAN_DESC_DEVICE_GROUP_NAME) + ":" +
                cReq.GetDeviceGroupInfo().GetDeviceGroupName(), cRsp.GetErrorCode());

            WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypePuConfig, (u32)cRsp.GetErrorCode(), strLogDesc);

            OnWaitRspMsg<CDeviceGroupAddRsp>(cRsp);
        }
        break;
    case TAS_DEVICE_GROUP_DEL_RSP:
        {
            CDeviceGroupDelRsp cRsp;
            GetRspMsg(cRsp, pcMsg);

            CDeviceGroupDelReq cReq;
            cReq.FromXml(m_strReq);

            string strLogDesc = GetOptLogDesc(LAN_DESC_DEL, LAN_DESC_DEVICE_GROUP , GetLanDesc(LAN_DESC_DEVICE_GROUP_ID) + ":" + 
                cReq.GetDeviceGroupInfoKey().GetDeviceGroupID(), cRsp.GetErrorCode());

            WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypePuConfig, (u32)cRsp.GetErrorCode(), strLogDesc);

            OnWaitRspMsg<CDeviceGroupDelRsp>(cRsp);
        }
        break;
    case TAS_DEVICE_GROUP_MOD_RSP:
        {
            CDeviceGroupModRsp cRsp;
            GetRspMsg(cRsp, pcMsg);

            CDeviceGroupModReq cReq;
            cReq.FromXml(m_strReq);

            string strDevGroupID;
            cReq.GetDeviceGroupID(strDevGroupID);
            if(!strDevGroupID.empty())
            {
                cReq.SetDeviceGroupID(strDevGroupID + "@" + pcOspSipMsg->GetSipToUri().GetDomain());
            }

            string strDevGroupName;
            cReq.GetDeviceGroupName(strDevGroupName);

            string strParentGroupID;
            cReq.GetDeviceParentGroupID(strParentGroupID);
            if(!strParentGroupID.empty())
            {
                cReq.SetDeviceParentGroupID(strParentGroupID + "@" + pcOspSipMsg->GetSipToUri().GetDomain());
            }

            string strLogDesc = GetOptLogDesc(LAN_DESC_MOD, LAN_DESC_DEVICE_GROUP , GetLanDesc(LAN_DESC_DEVICE_GROUP_ID) + ":" +
                strDevGroupID + " " + GetLanDesc(LAN_DESC_DEVICE_GROUP_NAME) + ":" + strDevGroupName, cRsp.GetErrorCode());

            WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypePuConfig, (u32)cRsp.GetErrorCode(), strLogDesc);

            OnWaitRspMsg<CDeviceGroupModRsp>(cRsp);
        }
		break;
	case TAS_DEVICE_GROUP_MOD_4GB_RSP:
		{
			CDeviceGroupMod4GbRsp cRsp;
			GetRspMsg(cRsp, pcMsg);

			CDeviceGroupMod4GbReq cReq;
			cReq.FromXml(m_strReq);

			string strDevGroupID = cReq.GetDevGrpId();
			if (!strDevGroupID.empty())
			{
				cReq.SetDevGrpId(strDevGroupID + "@" + pcOspSipMsg->GetSipToUri().GetDomain());
			}
			
			string strLogDesc = GetOptLogDesc(LAN_DESC_MOD, LAN_DESC_DEVICE_GROUP, GetLanDesc(LAN_DESC_DEVICE_GROUP_ID) + ":" +
				strDevGroupID + " " /*+ GetLanDesc(LAN_DESC_DEVICE_GROUP_NAME) + ":" + strDevGroupName*/, cRsp.GetErrorCode());

			WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypePuConfig, (u32)cRsp.GetErrorCode(), strLogDesc);

			OnWaitRspMsg<CDeviceGroupMod4GbRsp>(cRsp);
		}
		break;

    case TAS_DEVICE_GROUP_DEVICE_ORDER_RSP:
        {
            CDeviceGroupDeviceOrderRsp cRsp;
            GetRspMsg(cRsp, pcMsg);
            CDeviceGroupDeviceOrderReq cReq;
			cReq.FromXml(m_strReq);

            TVidSrcOrderInfo orderInfo = cReq.GetVidsrcOrderInfo();
            string strDevGroupID = orderInfo.GetDevGrpId();

            string strLogDesc = GetOptLogDesc(LAN_DESC_DEL, LAN_DESC_USERGROUP , GetLanDesc(LAN_DESC_USERGROUP_ID) + ":" + 
                strDevGroupID, cRsp.GetErrorCode());

            WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypePuConfig, (u32)cRsp.GetErrorCode(), strLogDesc);

            OnWaitRspMsg< CDeviceGroupDeviceOrderRsp > ( cRsp );
        }
        break;

    case TAS_CUSER_GROUP_ADD_RSP:
        {
            CUserGroupAddRsp cRsp;
            GetRspMsg(cRsp, pcMsg);

            CUserGroupAddReq cReq;
            cReq.FromXml(m_strReq);

            cRsp.GetUserGroupInfoKey().SetGroupNO(cRsp.GetUserGroupInfoKey().GetGroupNO() + "@" + pcOspSipMsg->GetSipToUri().GetDomain());

            string strLogDesc = GetOptLogDesc(LAN_DESC_ADD, LAN_DESC_USERGROUP , GetLanDesc(LAN_DESC_USERGROUP_ID) + ":" + 
                cRsp.GetUserGroupInfoKey().GetGroupNO() + 
                " " + GetLanDesc(LAN_DESC_USERGROUP_NAME) +
                ":" + cReq.GetUserGroupInfo().GetGroupName(), cRsp.GetErrorCode());

            WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypeUserManage, (u32)cRsp.GetErrorCode(), strLogDesc);

            OnWaitRspMsg<CUserGroupAddRsp>(cRsp);
        }
        break;
    case TAS_CUSER_GROUP_DEL_RSP:
        {
            CUserGroupDelRsp cRsp;
            GetRspMsg(cRsp, pcMsg);

            CUserGroupDelReq cReq;
            cReq.FromXml(m_strReq);

            string strLogDesc = GetOptLogDesc(LAN_DESC_DEL, LAN_DESC_USERGROUP , GetLanDesc(LAN_DESC_USERGROUP_ID) + ":" + 
                cReq.GetUserGroupInfoKey().GetGroupNO(), cRsp.GetErrorCode());

            WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypeUserManage, (u32)cRsp.GetErrorCode(), strLogDesc);

            OnWaitRspMsg<CUserGroupDelRsp>(cRsp);
        }
        break;
    case TAS_CUSER_GROUP_MOD_RSP:
        {
            CUserGroupModRsp cRsp;
            GetRspMsg(cRsp, pcMsg);

            CUserGroupModReq cReq;
            cReq.FromXml(m_strReq);

            string strGroupNo;
            cReq.GetGroupNO(strGroupNo);

            string strGroupName;
            cReq.GetGroupName(strGroupName);

            string strLogDesc = GetOptLogDesc(LAN_DESC_MOD, LAN_DESC_USERGROUP , GetLanDesc(LAN_DESC_USERGROUP_ID) + ":" +
                strGroupNo + " " + GetLanDesc(LAN_DESC_USERGROUP_NAME) + ":" + strGroupName, cRsp.GetErrorCode());

            WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypeUserManage, (u32)cRsp.GetErrorCode(), strLogDesc);

            string strWaterMarkLogDesc;
            if( GetWaterLogDesc( cReq, strWaterMarkLogDesc ) )
            {
                WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypeOther, (u32)cRsp.GetErrorCode(), strWaterMarkLogDesc );
            }

            OnWaitRspMsg<CUserGroupModRsp>(cRsp);
        }
        break;
    case TAS_CUSER_ADD_RSP:
        {
            CUserAddRsp cRsp;
            GetRspMsg(cRsp, pcMsg);

            CUserAddReq cReq;
            cReq.FromXml(m_strReq);

            cRsp.SetUserID(cRsp.GetUserID() + "@" + pcOspSipMsg->GetSipToUri().GetDomain());

            string strLogDesc = GetOptLogDesc(LAN_DESC_ADD, LAN_DESC_USER , GetLanDesc(LAN_DESC_USERID) + ":" + cRsp.GetUserID()
                + " " +  GetLanDesc(LAN_DESC_USERNAME) +
                ":" + cReq.GetUserInfo().GetUserName(), cRsp.GetErrorCode());

            WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypeUserManage, (u32)cRsp.GetErrorCode(), strLogDesc);

            OnWaitRspMsg<CUserAddRsp>(cRsp);
        }
        break;
    case TAS_CUSER_DEL_RSP:
        {
            CUserDelRsp cRsp;
            GetRspMsg(cRsp, pcMsg);

            CUserDelReq cReq;
            cReq.FromXml(m_strReq);

            string strLogDesc = GetOptLogDesc(LAN_DESC_DEL, LAN_DESC_USER , GetLanDesc(LAN_DESC_USERID) + ":" + 
                cReq.GetUserID(), cRsp.GetErrorCode());

            WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypeUserManage, (u32)cRsp.GetErrorCode(), strLogDesc);

			if ( cRsp.GetErrorCode() == CMS_SUCCESS )
			{
				C3acAgent *pc3acAgent = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(m_pcCuAgent->m_str3asName, this->GetInstance());
				if (NULL == pc3acAgent)
				{
					TASKLOG(NORMAL_TASK, ERROR_LEV, "收到来自CU的消息[%s--%hu]，但是没有对应的3acAgent\n",
						OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
				}
				else
				{
					pc3acAgent->m_cPtzlockInfoList.DelCuPtzLockInfoByUserId(cReq.GetUserID());
				}
			}

            OnWaitRspMsg<CUserDelRsp>(cRsp);
        }
        break;
    case TAS_CUSER_MOD_RSP:
        {
			
            CUserModRsp cRsp;
            GetRspMsg(cRsp, pcMsg);

            CUserModReq cReq;
            cReq.FromXml(m_strReq);

            string strUserID;
            cReq.GetUserID(strUserID); 

            string strUserName;
            cReq.GetUserName(strUserName);			

			//只有密码是自己改自己，所以需要判断userid是否为自己 2018/8/15 zsy
            if(CMS_SUCCESS == cRsp.GetErrorCode() && (!strUserID.empty()&& strUserID == m_pcCuAgent->m_pcUserAccount->m_tUserID))
            {
                string strNewPwd;
                cReq.GetUserPwd( strNewPwd );
				if (m_pcCuAgent && m_pcCuAgent->m_strPassWord != strNewPwd)
                {
                    string strLogDesc = GetOptLogDesc( LAN_DESC_MOD, LAN_DESC_USER, GetLanDesc(LAN_CU_CHANGE_PASSWORD_SUCCESS), cRsp.GetErrorCode() );
                    WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypeOther, (u32)cRsp.GetErrorCode(), strLogDesc );
                }
			}

			//用户权限也记录日志 2018/8/15 zsy	
			CUserPrivilege userPri;
			if (cReq.GetUserPrivilege(userPri))
			{
				string strPriLogDesc = GetOptLogDesc(LAN_DESC_MOD, LAN_DESC_USER_PRIVILEGE,//"修改" "用户权限"
													 GetLanDesc(LAN_DESC_USERID) + ":" + strUserID//"用户ID"
													 + " " + GetLanDesc(LAN_DESC_USERNAME) + ":" + strUserName//"用户名"
													 + " " + GetLanDesc(LAN_DESC_DATA) + "[" + userPri.ToXml() + "]"//"权限值"
													 , cRsp.GetErrorCode());
				WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypeOther, (u32)cRsp.GetErrorCode(), strPriLogDesc);
			}
			




			//普通用户修改
            string strLogDesc = GetOptLogDesc(LAN_DESC_MOD, LAN_DESC_USER , GetLanDesc(LAN_DESC_USERID) + ":" +
                strUserID + " " + GetLanDesc(LAN_DESC_USERNAME) + ":" + strUserName, cRsp.GetErrorCode());
            WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypeUserManage, (u32)cRsp.GetErrorCode(), strLogDesc);
            string strWaterMarkLogDesc;
            if( GetWaterLogDesc( cReq, strWaterMarkLogDesc ) )
            {
                WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypeOther, (u32)cRsp.GetErrorCode(), strWaterMarkLogDesc );
            }
            OnWaitRspMsg<CUserModRsp>(cRsp); 
        }
        break;
	case TAS_DEVTREE_ADD_RSP:
		{
			CDevTreeAddRsp cRsp;
			GetRspMsg(cRsp, pcMsg);

			string devtreeId;
			devtreeId = cRsp.GetDevtreeId();

			CDevTreeAddReq cReq;
			cReq.FromXml(m_strReq);

			string devtreeName;
			devtreeName = cReq.GetDevtreeName();

			string strLogDesc = GetOptLogDesc(LAN_DESC_ADD, LAN_DESC_DEV_TREE , GetLanDesc(LAN_DESC_DEV_TREE_ID) + ":" +
				devtreeId + " " + GetLanDesc(LAN_DESC_DEV_TREE_NAME) + ":" + devtreeName, cRsp.GetErrorCode());

			WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypeUserManage, (u32)cRsp.GetErrorCode(), strLogDesc);

			OnWaitRspMsg<CDevTreeAddRsp>(cRsp); 
		}
		break;
	case TAS_DEVTREE_MOD_RSP:
		{
			CDevTreeModRsp cRsp;
			GetRspMsg(cRsp, pcMsg);

			CDevTreeModReq cReq;
			cReq.FromXml(m_strReq);

			string devtreeId;
			devtreeId = cReq.GetDevtreeId();

			string devtreeName;
			devtreeName = cReq.GetDevtreeName();

			string strLogDesc = GetOptLogDesc(LAN_DESC_MOD, LAN_DESC_DEV_TREE_NAME , GetLanDesc(LAN_DESC_DEV_TREE_ID) + ":" +
				devtreeId + " " + GetLanDesc(LAN_DESC_DEV_TREE_NAME) + ":" + devtreeName, cRsp.GetErrorCode());

			WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypeUserManage, (u32)cRsp.GetErrorCode(), strLogDesc);

			OnWaitRspMsg<CDevTreeModRsp>(cRsp); 
		}
		break;
	case TAS_DEVTREE_DEL_RSP:
		{
			CDevTreeDelRsp cRsp;
			GetRspMsg(cRsp, pcMsg);

			CDevTreeDelReq cReq;
			cReq.FromXml(m_strReq);

			string devtreeId;
			devtreeId = cReq.GetDevtreeId();
			
			string strLogDesc = GetOptLogDesc(LAN_DESC_DEL, LAN_DESC_DEV_TREE , GetLanDesc(LAN_DESC_DEV_TREE_ID) + ":" +
				devtreeId + " ", cRsp.GetErrorCode());

			WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypeUserManage, (u32)cRsp.GetErrorCode(), strLogDesc);

			OnWaitRspMsg<CDevTreeDelRsp>(cRsp); 
		}
		break;
    case TAS_TEMP_AUTHORIZATION_ADD_RSP:
        {  
            CTempAuthorizationAddRsp cRsp;
            GetRspMsg(cRsp, pcMsg);

            cRsp.GetTempAuthorizationInfoKey().SetDeviceID(cRsp.GetTempAuthorizationInfoKey().GetDeviceID() + "@" + pcOspSipMsg->GetSipToUri().GetDomain());
            cRsp.GetTempAuthorizationInfoKey().SetUserName(cRsp.GetTempAuthorizationInfoKey().GetUserName() + "@" + pcOspSipMsg->GetSipToUri().GetDomain());

            CTempAuthorizationAddReq cReq;
            cReq.FromXml(m_strReq);

            string strLogDesc = GetOptLogDesc(LAN_DESC_ADD, LAN_DESC_AUTH, GetLanDesc(LAN_DESC_USERNAME) + 
                ":" + cReq.GetTempAuthorizationInfo().GetUsername() +
                " " + GetLanDesc(LAN_DESC_AUTH) + GetLanDesc(LAN_DESC_DEV) + GetLanDesc(LAN_DESC_DEVICE_ID) + 
                ":" +
                cReq.GetTempAuthorizationInfo().GetDeviceID(),
                cRsp.GetErrorCode());

            WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypeUserManage, (u32)cRsp.GetErrorCode(), strLogDesc);
            if(m_pcCuAgent->m_tCmuURI.GetDomain() != cReq.GetTempAuthorizationInfo().GetDestinationDomain())
            {
                WriteLog(cReq.GetTempAuthorizationInfo().GetDestinationDomain(), 
                    enUserOpTypeUserManage, (u32)cRsp.GetErrorCode(), strLogDesc);
            }

            OnWaitRspMsg<CTempAuthorizationAddRsp>(cRsp);
        }
        break;
    case TAS_TEMP_AUTHORIZATION_DEL_RSP:
        {
            CTempAuthorizationDelRsp cRsp;
            GetRspMsg(cRsp, pcMsg);

            CTempAuthorizationDelReq cReq;
            cReq.FromXml(m_strReq);

            string strLogDesc = GetOptLogDesc(LAN_DESC_DEL, LAN_DESC_AUTH, GetLanDesc(LAN_DESC_USERNAME) + 
                ":" + cReq.GetTempAuthorizationInfoKey().GetUserName() +
                " " + GetLanDesc(LAN_DESC_AUTH) + GetLanDesc(LAN_DESC_DEV) + GetLanDesc(LAN_DESC_DEVICE_ID) +
                ":" + cReq.GetTempAuthorizationInfoKey().GetDeviceID(),
                cRsp.GetErrorCode());

            WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypeUserManage, (u32)cRsp.GetErrorCode(), strLogDesc);
            if(m_pcCuAgent->m_tCmuURI.GetDomain() != cReq.GetTempAuthorizationInfoKey().GetDestinationDomain())
            {
                WriteLog(cReq.GetTempAuthorizationInfoKey().GetDestinationDomain(), 
                    enUserOpTypeUserManage, (u32)cRsp.GetErrorCode(), strLogDesc);
            }

            OnWaitRspMsg<CTempAuthorizationDelRsp>(cRsp);
        }
        break;
    case TAS_TEMP_AUTHORIZATION_MOD_RSP:
        {
            CTempAuthorizationModRsp cRsp;
            GetRspMsg(cRsp, pcMsg);

            CTempAuthorizationModReq cReq;
            cReq.FromXml(m_strReq);

            string strDevID;
            cReq.GetDeviceID(strDevID);

            string strLogDesc = GetOptLogDesc(LAN_DESC_MOD, LAN_DESC_AUTH, 
                " " + GetLanDesc(LAN_DESC_AUTH)  + GetLanDesc(LAN_DESC_DEVICE_ID) +
                ":" + strDevID,
                cRsp.GetErrorCode());

            WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypeUserManage, (u32)cRsp.GetErrorCode(), strLogDesc);

            OnWaitRspMsg<CTempAuthorizationModRsp>(cRsp);
        }
        break;
	case TAS_CUSER_DEVICE_GROUP_ADD_RSP:
		{
			CCuUserDeviceGroupAddRsp cRsp;
			GetRspMsg(cRsp, pcMsg);

			CCuUserDeviceGroupAddReq cReq;
			cReq.FromXml(m_strReq);

			string strLogDesc = GetOptLogDesc(LAN_DESC_ADD, LAN_DESC_GROUP_DEVICE ,
				" " + GetLanDesc(LAN_DESC_GROUP_DEVICE) +
				" " + GetLanDesc(LAN_DESC_OPRUSERID) +  ":" + cReq.GetOperId() +
				" " + GetLanDesc(LAN_DESC_USERID) + ":" + cReq.GetUserId() +
				" " + GetLanDesc(LAN_DESC_DEVICE_GROUP_ID) + ":" + cReq.GetDevGrpId()
				, cRsp.GetErrorCode());

			WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypeUserManage, (u32)cRsp.GetErrorCode(), strLogDesc);

			OnWaitRspMsg<CCuUserDeviceGroupAddRsp>(cRsp);
		}
		break;
	case TAS_DEVTREE_DEVGROUP_ASSIGN_RSP:
		{
			CCuDevTreeDevGrpAssignRsp cRsp;
			GetRspMsg(cRsp, pcMsg);

			CCuDevTreeDevGrpAssignReq cReq;
			cReq.FromXml(m_strReq);

			string strLogDesc = GetOptLogDesc(LAN_DESC_ADD, LAN_DESC_GROUP_DEVICE,
				" " + GetLanDesc(LAN_DESC_GROUP_CLASSIFIED) +
				" " + GetLanDesc(LAN_DESC_GROUP_DEVICE) + ":" + cReq.GetSrcGroupId() +
				" " + GetLanDesc(LAN_DESC_DEV_TREE_ID) + ":" + cReq.GetSrcDevTreeId() +
				" " + GetLanDesc(LAN_DESC_GROUP_DEVICE) + ":" + cReq.GetDstParentGrpId() +
				" " + GetLanDesc(LAN_DESC_DEV_TREE_ID) + ":" + cReq.GetDstDevTreeId() 
				, cRsp.GetErrorCode());

			WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypeUserManage, (u32)cRsp.GetErrorCode(), strLogDesc);

			OnWaitRspMsg<CCuDevTreeDevGrpAssignRsp>(cRsp);
		}
		break;
	case TAS_CUSER_DEVICE_GROUP_DEL_RSP:
		{
			CCuUserDeviceGroupDelRsp cRsp;
			GetRspMsg(cRsp, pcMsg);

			CCuUserDeviceGroupDelReq cReq;
			cReq.FromXml(m_strReq);

			string strLogDesc = GetOptLogDesc(LAN_DESC_DEL, LAN_DESC_GROUP_DEVICE ,
				" " + GetLanDesc(LAN_DESC_GROUP_DEVICE) +
				" " + GetLanDesc(LAN_DESC_OPRUSERID) +  ":" + cReq.GetOperId() +
				" " + GetLanDesc(LAN_DESC_USERID) + ":" + cReq.GetUserId() +
				" " + GetLanDesc(LAN_DESC_DEVICE_GROUP_ID) + ":" + cReq.GetDevGrpId()
				, cRsp.GetErrorCode());

			WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypeUserManage, (u32)cRsp.GetErrorCode(), strLogDesc);

			OnWaitRspMsg<CCuUserDeviceGroupDelRsp>(cRsp);
		}
		break;
	case TAS_CUSER_GROUP_DEVICE_GROUP_ADD_RSP:
		{
			CCuUserGroupDeviceGroupAddRsp cRsp;
			GetRspMsg(cRsp, pcMsg);

			CCuUserGroupDeviceGroupAddReq cReq;
			cReq.FromXml(m_strReq);

			string strLogDesc = GetOptLogDesc(LAN_DESC_ADD, LAN_DESC_GROUP_DEVICE ,
				" " + GetLanDesc(LAN_DESC_GROUP_DEVICE) +
				" " + GetLanDesc(LAN_DESC_OPRUSERID) +  ":" + cReq.GetOperId() +
				" " + GetLanDesc(LAN_DESC_USERGROUP_ID) + ":" + cReq.GetUserGroupId() +
				" " + GetLanDesc(LAN_DESC_DEVICE_GROUP_ID) + ":" + cReq.GetDevGrpId()
				, cRsp.GetErrorCode());

			WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypeUserManage, (u32)cRsp.GetErrorCode(), strLogDesc);

			OnWaitRspMsg<CCuUserGroupDeviceGroupAddRsp>(cRsp);
		}
		break;
	case TAS_CUSER_GROUP_DEVICE_GROUP_DEL_RSP:
		{
			CCuUserGroupDeviceGroupDelRsp cRsp;
			GetRspMsg(cRsp, pcMsg);

			CCuUserGroupDeviceGroupDelReq cReq;
			cReq.FromXml(m_strReq);

			string strLogDesc = GetOptLogDesc(LAN_DESC_DEL, LAN_DESC_GROUP_DEVICE ,
				" " + GetLanDesc(LAN_DESC_GROUP_DEVICE) +
				" " + GetLanDesc(LAN_DESC_OPRUSERID) +  ":" + cReq.GetOperId() +
				" " + GetLanDesc(LAN_DESC_USERGROUP_ID) + ":" + cReq.GetUserGroupId() +
				" " + GetLanDesc(LAN_DESC_DEVICE_GROUP_ID) + ":" + cReq.GetDevGrpId()
				, cRsp.GetErrorCode());

			WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypeUserManage, (u32)cRsp.GetErrorCode(), strLogDesc);

			OnWaitRspMsg<CCuUserGroupDeviceGroupDelRsp>(cRsp);
		}
		break;
	case TAS_DEVICE_AUTO_ASSIGN_TO_USER_SET_RSP:
		{
			CCuDeviceAutoAssignToUserSetRsp cRsp;
			GetRspMsg(cRsp, pcMsg);

			CCuDeviceAutoAssignToUserSetReq cReq;
			cReq.FromXml(m_strReq);

			std::stringstream ss;
			ss << (int)cReq.GetAutoAssign();

			string strLogDesc = GetOptLogDesc(LAN_DESC_SET, LAN_DESC_DEV_GROUP_AUTO_ASSIGN ,
				" " + GetLanDesc(LAN_DESC_DEV_GROUP_AUTO_ASSIGN) +
				" " + GetLanDesc(LAN_DESC_OPRUSERID) +  ":" + cReq.GetOperId() +
				" " +GetLanDesc(LAN_DESC_USERID) + ":" + cReq.GetUserId() +
				" " + GetLanDesc(LAN_DESC_DEV_GROUP_AUTO_ASSIGN) + ":" + ss.str()
				, cRsp.GetErrorCode());

			WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypeUserManage, (u32)cRsp.GetErrorCode(), strLogDesc);

			OnWaitRspMsg<CCuDeviceAutoAssignToUserSetRsp>(cRsp);
		}
		break;
	case TAS_CUSER_DATA_VERSION_GET_RSP:
		{
			CCuUserDataVersionGetRsp cRsp;
			GetRspMsg(cRsp, pcMsg);

			CCuUserDataVersionGetReq cReq;
			cReq.FromXml(m_strReq);

			string strLogDesc = GetOptLogDesc(LAN_DESC_GET, LAN_DESC_DEV ,
				" " + GetLanDesc(LAN_DESC_VERSION) + cRsp.GetDataVersion()
				, cRsp.GetErrorCode());

			OnWaitRspMsg<CCuUserDataVersionGetRsp>(cRsp);
		}
		break;
	case TAS_DEVICE_AUTO_ASSIGN_TO_USER_QRY_RSP:
		{
			CDeviceAutoAssignToUserQryRsp cRsp;
			GetRspMsg(cRsp, pcMsg);

			CDeviceAutoAssignToUserQryReq cReq;
			cReq.FromXml(m_strReq);

			string strLogDesc = GetOptLogDesc(LAN_DESC_QRY, LAN_DESC_DEV_GROUP_AUTO_ASSIGN ,
				" " + GetLanDesc(LAN_DESC_DEV_GROUP_AUTO_ASSIGN) +
				" " +GetLanDesc(LAN_DESC_USERID) + ":" + cReq.GetUserId()
				, cRsp.GetErrorCode());

			WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypeUserManage, (u32)cRsp.GetErrorCode(), strLogDesc);

			OnWaitRspMsg<CDeviceAutoAssignToUserQryRsp>(cRsp);
		}
		break;
	case TAS_DEVICE_AUTO_ASSIGN_TO_USER_GROUP_SET_RSP:
		{
			CCuDeviceAutoAssignToUserGroupSetRsp cRsp;
			GetRspMsg(cRsp, pcMsg);

			CCuDeviceAutoAssignToUserGroupSetReq cReq;
			cReq.FromXml(m_strReq);

			std::stringstream ss;
			ss << (int)cReq.GetAutoAssign();

			string strLogDesc = GetOptLogDesc(LAN_DESC_SET, LAN_DESC_DEV_GROUP_AUTO_ASSIGN ,
				" " + GetLanDesc(LAN_DESC_DEV_GROUP_AUTO_ASSIGN) +
				" " + GetLanDesc(LAN_DESC_OPRUSERID) +  ":" + cReq.GetOperId() +
				" " +GetLanDesc(LAN_DESC_USERGROUP_ID) + ":" + cReq.GetUserGroupId() +
				" " + GetLanDesc(LAN_DESC_DEV_GROUP_AUTO_ASSIGN) + ":" + ss.str()
				, cRsp.GetErrorCode());

			WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypeUserManage, (u32)cRsp.GetErrorCode(), strLogDesc);

			OnWaitRspMsg<CCuDeviceAutoAssignToUserGroupSetRsp>(cRsp);
		}
		break;
	case TAS_DEVICE_AUTO_ASSIGN_TO_USER_GROUP_QRY_RSP:
		{
			CDeviceAutoAssignToUserGroupQryRsp cRsp;
			GetRspMsg(cRsp, pcMsg);

			CDeviceAutoAssignToUserGroupQryReq cReq;
			cReq.FromXml(m_strReq);

			string strLogDesc = GetOptLogDesc(LAN_DESC_QRY, LAN_DESC_DEV_GROUP_AUTO_ASSIGN ,
				" " + GetLanDesc(LAN_DESC_DEV_GROUP_AUTO_ASSIGN) +
				" " +GetLanDesc(LAN_DESC_USERGROUP_ID) + ":" + cReq.GetUserGroupId()
				, cRsp.GetErrorCode());

			WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypeUserManage, (u32)cRsp.GetErrorCode(), strLogDesc);

			OnWaitRspMsg<CDeviceAutoAssignToUserGroupQryRsp>(cRsp);
		}
		break;
	case TAS_TRANSACTION_PROGRESS_GET_RSP:
		{
			CCuTransactionProgressGetRsp cRsp;
			GetRspMsg(cRsp, pcMsg);

			OnWaitRspMsg<CCuTransactionProgressGetRsp>(cRsp);
		}
		break;
    case TAS_TVWALL_ADD_RSP:
        {
            CCuTvwallAddRsp cRsp;
            pcOspSipMsg->GetMsgBody(cRsp);

            string strTvWallID = cRsp.GetTvwallId() + "@" + pcOspSipMsg->GetSipToUri().GetDomain();
            cRsp.SetTvwallId(strTvWallID);

            SendRspToCu<CCuTvwallAddRsp>(cRsp, m_ptRecvNode);

            CCuTvwallAddReq cReq;
            cReq.FromXml(m_strReq);

            string strLogDesc = GetOptLogDesc(LAN_DESC_ADD, LAN_DESC_TVWALL, GetLanDesc(LAN_DESC_TVWALLID) + ":" +
                cRsp.GetTvwallId() + " " +
                GetLanDesc(LAN_DESC_TVWALL_NAME) + ":" + cReq.GetTvwallInfo().GetDeviceName(),
                cRsp.GetErrorCode());

            WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypeTVWall, (u32)cRsp.GetErrorCode(), strLogDesc);

            //任务完成, 销毁之
            NextState(NOT_REQUEST_STATE);
            delete this;
            return PROCMSG_OK;
        }
        break;
    case TAS_TVWALL_DEL_RSP:
        {
            CCuTvwallDelRsp cRsp;
            GetRspMsg(cRsp, pcMsg);

            CCuTvwallDelReq cReq;
            cReq.FromXml(m_strReq);

            string strLogDesc = GetOptLogDesc(LAN_DESC_DEL, LAN_DESC_TVWALL, GetLanDesc(LAN_DESC_TVWALLID) + ":" +
                cReq.GetTvwallId() + "@" + GetDomainName(),
                cRsp.GetErrorCode());

            WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypeTVWall, (u32)cRsp.GetErrorCode(), strLogDesc);

            OnWaitRspMsg<CCuTvwallDelRsp>(cRsp);
        }
        break;
    case TAS_TVWALL_MOD_RSP:
        {
            CCuTvwallModRsp cRsp;
            GetRspMsg(cRsp, pcMsg);

            CCuTvwallModReq cReq;
            cReq.FromXml(m_strReq);

            string strID;
            cReq.GetId(strID);

            string strName;
            cReq.GetName(strName);

            string strLogDesc = GetOptLogDesc(LAN_DESC_MOD, LAN_DESC_TVWALL, GetLanDesc(LAN_DESC_TVWALLID) + ":" +
                strID + "@" + GetDomainName() + " " +
                GetLanDesc(LAN_DESC_TVWALL_NAME) + ":" + strName,
                cRsp.GetErrorCode());

            WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypeTVWall, (u32)cRsp.GetErrorCode(), strLogDesc);

            OnWaitRspMsg<CCuTvwallModRsp>(cRsp);
        }
        break;
    case TAS_DEVICE_MOD_RSP:
        {
            CCuDeviceNameModRsp cRsp;
            GetRspMsg(cRsp, pcMsg);

            CCuDeviceNameModReq cReq;
            cReq.FromXml(m_strReq);

            string strID;
            cReq.GetDeviceUUID(strID);

            string strName;
            cReq.GetDeviceName(strName);

            string strLogDesc = GetOptLogDesc(LAN_DESC_MOD, LAN_DESC_TVWALL, GetLanDesc(LAN_DESC_TVWALLID) + ":" +
                strID + " " +
                GetLanDesc(LAN_DESC_TVWALL_NAME) + ":" + strName,
                cRsp.GetErrorCode());

            WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypeTVWall, (u32)cRsp.GetErrorCode(), strLogDesc);

            OnWaitRspMsg<CCuDeviceNameModRsp>(cRsp);
        }
        break;
    case TAS_DEVICE_CAP_INDEX_NAME_SET_RSP:
        {
            CCuDeviceCapIndexNameSetRsp cRsp;
            GetRspMsg(cRsp, pcMsg);

            CCuDeviceCapIndexNameSetReq cReq;
            cReq.FromXml(m_strReq);

            string strLogDesc = GetOptLogDesc(LAN_DESC_SET, LAN_DESC_DEVCAP, GetLanDesc(LAN_DESC_DEVICE_ID) + ":" +
                cReq.GetDeviceCapName().deviceId + "@" + pcOspSipMsg->GetSipToUri().GetDomain(),
                cRsp.GetErrorCode());

            WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypePuConfig, (u32)cRsp.GetErrorCode(), strLogDesc,
                cReq.GetDeviceCapName().deviceId + "@" + pcOspSipMsg->GetSipToUri().GetDomain());

            OnWaitRspMsg<CCuDeviceCapIndexNameSetRsp>(cRsp);
        }
        break;
    case TAS_DEVICE_CAP_INDEX_NAME_GET_RSP:
        {
            CCuDeviceCapIndexNameGetRsp cRsp;
            GetRspMsg(cRsp, pcMsg);

            cRsp.GetDeviceCapName().deviceId = cRsp.GetDeviceCapName().deviceId + "@" + pcOspSipMsg->GetSipToUri().GetDomain();

            SendRspToCu<CCuDeviceCapIndexNameGetRsp>(cRsp, m_ptRecvNode);

            //任务完成, 销毁之
            NextState(NOT_REQUEST_STATE);
            delete this;
            return PROCMSG_OK;
        }
        break;
	case TAS_DEVGRP_VIDSRC_TOTALNUM_GET_RSP:
		{
			CDevGrpVidsrcTotalnumGetRsp cRsp;
			GetRspMsg(cRsp, pcMsg);

			// rsp中，DeviceGroup是UUID，不带域名，在cui.fcgi中根据请求转换，这样不消耗cuiserver性能

			SendRspToCu<CDevGrpVidsrcTotalnumGetRsp>(cRsp, m_ptRecvNode);

			//任务完成, 销毁之
			NextState(NOT_REQUEST_STATE);
			delete this;
			return PROCMSG_OK;
		}
		break;
	case TAS_DEVICE_MATCH_GBID_RSP:
		{
			CCuDeviceMatchGbidRsp cRsp;
			GetRspMsg(cRsp, pcMsg);

			OnWaitRspMsg<CCuDeviceMatchGbidRsp>(cRsp);
		}
		break;
	case TAS_DEVICE_GROUP_DEVICE_SUMNUM_RSP:
		{
			CCuDeviceGrpDevSumNumRsp cRsp;
			GetRspMsg(cRsp, pcMsg);
			
			OnWaitRspMsg<CCuDeviceGrpDevSumNumRsp>(cRsp);
		}
		break;
	case TAS_DEVICE_GROUP_DEVICE_MOD_4GB_RSP:
		{
			CCuDeviceGroupDeviceMod4GbRsp cRsp;
			GetRspMsg(cRsp, pcMsg);

			CCuDeviceGroupDeviceMod4GbReq cReq;
			cReq.FromXml(m_strReq);

			cRsp.SetDevgroupId(cReq.GetDeviceGroupID());
			cRsp.SetDeviceId(cReq.GetDeviceId());

			OnWaitRspMsg<CCuDeviceGroupDeviceMod4GbRsp>(cRsp);
		}
		break;
	case TAS_DEVICE_GBID_ERROR_GET_RSP:
		{
			CCuDeviceGbidErrGetRsp cRsp;
			GetRspMsg(cRsp, pcMsg);

			if (cRsp.GetExistErr() == true)
			{
				C3acAgent *pc3acAgent = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(m_pcCuAgent->m_str3asName, GetInstance());
				CMS_ASSERT(NULL != pc3acAgent);
				CMS_ASSERT(0 == strcmp(pc3acAgent->GetObjName(), "C3acAgent"));
				if (pc3acAgent != NULL)
				{
					TDevGbidErrItem &tErrItem = cRsp.GetErrItem();
					tErrItem.devId = tErrItem.devId + "@" + pc3acAgent->GetDomainNameByDomainID(tErrItem.domainId);

					vector<CDeviceGroupInfo> &groupInfoList = tErrItem.devGrpFullGbinfos;

					string strTmp;
					for (size_t nIndex = 0; nIndex < groupInfoList.size(); nIndex++)
					{
						strTmp = groupInfoList[nIndex].GetDeviceGroupID() + "@" + pc3acAgent->GetDomainNameByDomainID(groupInfoList[nIndex].GetDomainUUID());

						if (tErrItem.devGrpId == groupInfoList[nIndex].GetDeviceGroupID())
						{
							tErrItem.devGrpId = strTmp;
						}
						groupInfoList[nIndex].SetDeviceGroupID(strTmp);

						strTmp = groupInfoList[nIndex].GetDeviceParentGroupID() + "@" + pc3acAgent->GetDomainNameByDomainID(groupInfoList[nIndex].GetParentDomainUUID());
					}
				}
				else
				{
					TASKLOG(NORMAL_TASK, CRITICAL_LEV, "和3A失去连接？ CCuDeviceGbidErrGetRsp 无法转换成 uud@域名的形式\n");
				}
			}

			OnWaitRspMsg<CCuDeviceGbidErrGetRsp>(cRsp);
		}
		break;
    case PU_VID_ENC_PARAM_GET_RSP:
        {
            CCuPuVidEncParamGetRsp cRsp;
            GetRspMsg(cRsp, pcMsg);

            OnWaitRspMsg<CCuPuVidEncParamGetRsp>(cRsp);
        }
		break;
	case PU_AUD_ENC_PARAM_GET_RSP:
		{
			CCuPuAudEncParamGetRsp cRsp;
			GetRspMsg(cRsp, pcMsg);

			OnWaitRspMsg<CCuPuAudEncParamGetRsp>(cRsp);
		}
		break;
    case PU_VID_ENC_PARAM_SET_RSP:
        {
            CCuPuVidEncParamSetRsp cRsp;
            GetRspMsg(cRsp, pcMsg);

            OnWaitRspMsg<CCuPuVidEncParamSetRsp>(cRsp);
        }
        break;
	case PU_VIDSRC_OSD_SET_RSP:
		{
			CPuVidSrcOsdSetRsp cRsp;
			GetRspMsg(cRsp, pcMsg);

			OnWaitRspMsg<CPuVidSrcOsdSetRsp>(cRsp);
		}
		break;
	case PU_VIDSRC_OSD_GET_RSP:
		{
			CPuVidSrcOsdGetRsp cRsp;
			GetRspMsg(cRsp, pcMsg);

			OnWaitRspMsg<CPuVidSrcOsdGetRsp>(cRsp);
		}
		break;
    case PU_CAMERA_TYPE_SET_RSP:
        {
            CCuPuCameraTypeSetRsp cRsp;
            GetRspMsg(cRsp, pcMsg);

            OnWaitRspMsg<CCuPuCameraTypeSetRsp>(cRsp);
        }
        break;
	case PU_ALARM_RESET_RSP:
		{
			CCuPuAlarmResetRsp cRsp;
			GetRspMsg(cRsp, pcMsg);

			OnWaitRspMsg<CCuPuAlarmResetRsp>(cRsp);
		}
		break;
	case PU_RECORD_START_RSP:
		{
			CCuPuRecordStartRsp cRsp;
			GetRspMsg(cRsp,pcMsg);

			CCuPuRecordStartReq cReq;
			cReq.FromXml(m_strReq);
			WriteLog(cReq.GetDevChn(),cRsp.GetErrorCode(),PU_RECORD_START_REQ,GetDomainName());

			OnWaitRspMsg<CCuPuRecordStartRsp>(cRsp);
		}
		break;
	case PU_RECORD_STOP_RSP:
		{
			CCuPuRecordStopRsp cRsp;
			GetRspMsg(cRsp,pcMsg);

			CCuPuRecordStopReq cReq;
			cReq.FromXml(m_strReq);
			WriteLog(cReq.GetDevChn(),cRsp.GetErrorCode(),PU_RECORD_STOP_REQ,GetDomainName());

			OnWaitRspMsg<CCuPuRecordStopRsp>(cRsp);
		}
		break;
	case PU_SERIALPARAM_CFG_GET_RSP:
		{
			CCuPuSerialParamCfgGetRsp cRsp;
			GetRspMsg(cRsp, pcMsg);
			OnWaitRspMsg<CCuPuSerialParamCfgGetRsp>(cRsp);
		}
		break;
	case PU_SERIALPARAM_CFG_SET_RSP:
		{
			CCuPuSerialParamCfgSetRsp cRsp;
			GetRspMsg(cRsp, pcMsg);
			OnWaitRspMsg<CCuPuSerialParamCfgSetRsp>(cRsp);
		}
		break;
    case PU_MOTION_DETECT_GET_RSP:
        {
            CCuPuMotionDetectGetRsp cRsp;
            GetRspMsg(cRsp, pcMsg);

            OnWaitRspMsg<CCuPuMotionDetectGetRsp>(cRsp);
        }
        break;
    case PU_MOTION_DETECT_SET_RSP:
        {
            CCuPuMotionDetectSetRsp cRsp;
            GetRspMsg(cRsp, pcMsg);

            CCuPuMotionDetectSetReq cReq;
            cReq.FromXml(m_strReq);

            string strLogDesc = GetOptLogDesc(LAN_DESC_SET, LAN_DESC_MOTION_DETECT, GetLanDesc(LAN_DESC_DEVICE_ID) + 
                " " + cReq.GetDevUri() + ":" +
                TransInt2String(cReq.GetDevChn().GetChnNO()+1), cRsp.GetErrorCode());

            WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypePuConfig, 
                (u32)cRsp.GetErrorCode(), strLogDesc, cReq.GetDevUri());

            OnWaitRspMsg<CCuPuMotionDetectSetRsp>(cRsp);
        }
		break;
	case CONFIG_SYNC_SWITCH_GET_RSP:
		{
			CCuGetSyncVideoSrcNameEnableRsp cRsp;
			GetRspMsg( cRsp, pcMsg );
			
			OnWaitRspMsg<CCuGetSyncVideoSrcNameEnableRsp>( cRsp );
		}
		break;
	case CUI_GET_AZIMUTH_RSP:
		{
			CCuGetAzimuthRsp cRsp;
			GetRspMsg(cRsp, pcMsg);

			OnWaitRspMsg<CCuGetAzimuthRsp>(cRsp);
		}
		break;
	case CONFIG_SYNC_SWITCH_SET_RSP:
		{
			CCuSetSyncVideoSrcNameEnableRsp cRsp;
			GetRspMsg( cRsp, pcMsg );

			CCuSetSyncVideoSrcNameEnableReq cReq;
			cReq.FromXml(m_strReq);

			bool syncPu2Plat = false;
			bool syncPlat2Pu = false;
			
			cReq.GetSwichSets( SYNC_PUSRCNAME_TO_PLAT, syncPu2Plat );
			cReq.GetSwichSets( SYNC_PUSRCNAME_TO_PU, syncPlat2Pu );

			std::stringstream ss;
			ss << LAN_DESC_SYNC_VS_NAME_PU2PLAT << "=" << syncPu2Plat
				<< " " << LAN_DESC_SYNC_VS_NAME_PLAT2PU << "=" << syncPlat2Pu;

			string strLogDesc = GetOptLogDesc(LAN_DESC_SET, LAN_DESC_SYNC_SWITCH , ss.str(), (s32)cRsp.GetErrorCode());

			WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypeUserManage, (u32)cRsp.GetErrorCode(), strLogDesc );

			OnWaitRspMsg<CCuSetSyncVideoSrcNameEnableRsp>( cRsp );
		}
		break;
    case PU_AREA_SHADE_GET_RSP:
        {
            CCuPuAreaShadeGetRsp cRsp;
            GetRspMsg(cRsp, pcMsg);

            OnWaitRspMsg<CCuPuAreaShadeGetRsp>(cRsp);
        }
        break;
    case PU_AREA_SHADE_SET_RSP:
        {
            CCuPuAreaShadeSetRsp cRsp;
            GetRspMsg(cRsp, pcMsg);

            CCuPuAreaShadeSetReq cReq;
            cReq.FromXml(m_strReq);

            string strLogDesc = GetOptLogDesc(LAN_DESC_SET, LAN_DESC_AREA_SHADE, GetLanDesc(LAN_DESC_DEVICE_ID) + 
                ":" + cReq.GetDevUri() + " " + GetLanDesc(LAN_DESC_DEV_CHN) + ":" +
                TransInt2String(cReq.GetDevChn().GetChnNO()+1), cRsp.GetErrorCode());

            WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypePuConfig, 
                (u32)cRsp.GetErrorCode(), strLogDesc, cReq.GetDevUri());

            OnWaitRspMsg<CCuPuAreaShadeSetRsp>(cRsp);
        }
        break;
	case SECURITY_MODULE_CERTIFICATE_QUERY_RSP:
		{
			CSecurityModuleCertificateQueryRsp cSecRsp;
			GetRspMsg(cSecRsp, pcMsg);

			CCertificateQryRsp cRsp;
			cRsp.SetErrorCode( cSecRsp.GetErrorCode() );
			cRsp.SetSignatureCert( cSecRsp.GetSignatureCert() );
			cRsp.SetEncryptCert( cSecRsp.GetEncryptCert() );

			OnWaitRspMsg<CCertificateQryRsp>(cRsp);
		}
		break;
    default:
        {
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"收到未知消息");
        }
        break;
    }

    return PROCMSG_OK;
}

u32 CSingleInterAction::WaitRspTimerPoll()
{
    if ( GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout() )
    {

        CEventRsp cRsp;
        cRsp.SetErrorCode(ERR_CUI_OPR_TIME_OUT);
        TASKLOG(NORMAL_TASK, ERROR_LEV, "请求[%s--%d]超时，直接向CU用户[%s]会话[%s]发送应答消息[%s--%d]\n",
            OspExtEventDesc((u16)cRsp.GetEventId() - 1).c_str(), cRsp.GetEventId() - 1,
            m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
            OspExtEventDesc((u16)cRsp.GetEventId()).c_str(), cRsp.GetEventId() );

        WriteAbnormalLog(ERR_CUI_OPR_TIME_OUT);

        CMS_ASSERT(NULL != m_ptRecvNode);
        SendRspToCu(cRsp, m_ptRecvNode);
        NextState(NOT_REQUEST_STATE);
        return TIMERPOLL_DEL;
    }
    return TIMERPOLL_DONE;
}

string CSingleInterAction::GetOptLogDesc(const string& Opt, const string& OptDevInfo, s32 dwErrorCode)
{
    return GetLanDesc(Opt) + GetErrorCode((u32)dwErrorCode) + " " + OptDevInfo ;
}

string CSingleInterAction::GetOptLogDesc(const string& OptType,const string& Type, const string& OptDevInfo, s32 dwErrorCode)
{
    return GetLanDesc(OptType) + GetLanDesc(Type) + GetErrorCode((u32)dwErrorCode) + " " + OptDevInfo;
}

void CSingleInterAction::WriteLog(const string& strDomain, TOperateType enOprType,
    u32 dwErrorCode, const string& strLogDesc, const string &strDevUri)
{
    CUsrOprLogTask* pcTask = new CUsrOprLogTask(GetInstance(), m_pcCuAgent->m_strSession, 
        string(DEV_TYPE_UAS) + "@" + strDomain,
        enOprType,
        dwErrorCode,
        strLogDesc,
        strDevUri
        );
    pcTask->InitStateMachine();
    pcTask->StartSendLog();
}
void CSingleInterAction::WriteLog(const TChannel& tDevChn, const u32 dwErrorCode, const u32 dwEvent, const string& strDomainName)
{
    CGetPuData *pcTask = new CGetPuData(GetInstance(), m_pcCuAgent->m_strSession, tDevChn, dwErrorCode, m_strReq, dwEvent, strDomainName);
    pcTask->InitStateMachine(); 
	if (pcTask->StartWriteLog() != PROCMSG_OK)
	{
		delete pcTask;
		pcTask = NULL;
	}
}

string CSingleInterAction::GetErrorCode(u32 dwErrorCode)
{
    if( CMS_SUCCESS == dwErrorCode)
    {
        return GetLanDesc(LAN_DESC_SUCCESS);
    }
    else
    {
        return GetLanDesc(LAN_DESC_FAIL);
    }
}

void CSingleInterAction::WriteAbnormalLog(u32 dwErrorCode)
{
    u16 wEvent = OspExtEventID(GetXmlKeyText(m_strReq, EVENT_BEGIN, EVENT_END).c_str());
    switch(wEvent)
    {
    case PU_PTZ_CTRL_REQ:
        {   
            CPuPtzCtrlReq cReq;
            cReq.FromXml(m_strReq);

            string strDevDomain = GetDomainNameWithoutPreNameFromURI(cReq.GetDevChn().GetDevUri());
            //日志相关
            WriteLog(cReq.GetDevChn(), dwErrorCode, cReq.GetEventId() ,GetDomainName());

            if(m_pcCuAgent->m_tCmuURI.GetDomain() != strDevDomain)
            {
                WriteLog(cReq.GetDevChn(), dwErrorCode, cReq.GetEventId() ,strDevDomain);
            }
        }
        break;
    case PU_PTZ_LOCK_REQ:
        {
            CPuPtzLockReq cReq;
            cReq.FromXml(m_strReq);

            string strLogDesc = GetOptLogDesc(LAN_DESC_PTZ_LOCK , GetLanDesc(LAN_DESC_DEVICE_ID) + ":"
                + cReq.GetPuChn().GetDevUri() + " " + GetLanDesc(LAN_DESC_DEV_CHN) + ":"
                + TransInt2String(cReq.GetPuChn().GetChnNO()+1), (s32)dwErrorCode);

            WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypePtzCtrl, (u32)dwErrorCode, strLogDesc, 
                cReq.GetPuChn().GetDevUri());

            string strDevDomain = GetDomainNameWithoutPreNameFromURI(cReq.GetPuChn().GetDevUri());
            if(m_pcCuAgent->m_tCmuURI.GetDomain() != strDevDomain)
            {
                WriteLog(strDevDomain, enUserOpTypePtzCtrl, (u32)dwErrorCode, strLogDesc, cReq.GetPuChn().GetDevUri());
            }
        }
        break;
    case PU_PTZ_UNLOCK_REQ:
        {
            CPuPtzUnlockReq cReq;
            cReq.FromXml(m_strReq);

            string strLogDesc = GetOptLogDesc(LAN_DESC_PTZ_UNLOCK , GetLanDesc(LAN_DESC_DEVICE_ID) + ":"
                + cReq.GetPuChn().GetDevUri() + " "  + GetLanDesc(LAN_DESC_DEV_CHN) + ":"
                + TransInt2String(cReq.GetPuChn().GetChnNO()+1), (s32)dwErrorCode);


            WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypePtzCtrl, (u32)dwErrorCode, strLogDesc, cReq.GetPuChn().GetDevUri());

            string strDevDomain = GetDomainNameWithoutPreNameFromURI(cReq.GetPuChn().GetDevUri());
            if(m_pcCuAgent->m_tCmuURI.GetDomain() != strDevDomain)
            {
                WriteLog(strDevDomain, enUserOpTypePtzCtrl, (u32)dwErrorCode, strLogDesc, cReq.GetPuChn().GetDevUri());
            }
        }
        break;
	case TAS_PTZLOCK_ADD_REQ:
		{
			CCuPtzLockAddReq cReq;
			cReq.FromXml(m_strReq);

			CPtzLockInfo &lockInfo = cReq.GetCPtzLockInfo();
			
			string strLogDesc = GetOptLogDesc(LAN_DESC_PTZ_LOCK, GetLanDesc(LAN_DESC_DEVICE_ID) + ":" +
				lockInfo.GetDeviceId() + GetLanDesc(LAN_DESC_VIDSRC) + TransInt2String(lockInfo.GetVideoSrcId() + 1) +
				GetLanDesc(LAN_DESC_TIME_RANGE) + lockInfo.GetBeginTime() + "-" + lockInfo.GetEndTime(), (u32)dwErrorCode);

			WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypeUserManage, (u32)dwErrorCode, strLogDesc);
		}
		break;
	case TAS_PTZLOCK_DEL_REQ:
		{
			CCuPtzLockDelReq cReq;
			cReq.FromXml(m_strReq);
			
			string strLogDesc = GetOptLogDesc(LAN_DESC_PTZ_UNLOCK, GetLanDesc(LAN_DESC_DEVICE_ID) + ":" +
				cReq.GetDeviceId() + GetLanDesc(LAN_DESC_VIDSRC) + TransInt2String(cReq.GetVideoSrcId() + 1), (u32)dwErrorCode);

			WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypeUserManage, (u32)dwErrorCode, strLogDesc);
		}
		break;
    case PU_TRANSPARENT_OPERATE_REQ:
        {
            CCuPuTransparentOperateReq cReq;
            cReq.FromXml(m_strReq);

            string strLogDesc = GetOptLogDesc(LAN_DESC_TRANS , GetLanDesc(LAN_DESC_TRANSPARENT_DATA) + ":"
                + cReq.GetDevChn().GetDevUri() + " " + GetLanDesc(LAN_DESC_DEV_CHN) + ":"
                + TransInt2String(cReq.GetDevChn().GetChnNO()+1), (s32)dwErrorCode);

            WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypePtzCtrl, (u32)dwErrorCode, strLogDesc, cReq.GetDevChn().GetDevUri());

            string strDevDomain = GetDomainNameWithoutPreNameFromURI(cReq.GetDevChn().GetDevUri());
            if(m_pcCuAgent->m_tCmuURI.GetDomain() != strDevDomain)
            {
                WriteLog(strDevDomain, enUserOpTypePtzCtrl, (u32)dwErrorCode, strLogDesc);
            }
        }
        break;
    case TAS_DEVICE_GROUP_ADD_REQ:
        {
            CDeviceGroupAddReq cReq;
            cReq.FromXml(m_strReq);

            string strLogDesc = GetOptLogDesc(LAN_DESC_ADD, LAN_DESC_DEVICE_GROUP , 
                GetLanDesc(LAN_DESC_DEVICE_GROUP_NAME) + ":" +
                cReq.GetDeviceGroupInfo().GetDeviceGroupName(), (s32)dwErrorCode);

            WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypePuConfig, (u32)dwErrorCode, strLogDesc);
        }
        break;
    case TAS_DEVICE_GROUP_DEL_REQ:
        {
            CDeviceGroupDelReq cReq;
            cReq.FromXml(m_strReq);

            string strLogDesc = GetOptLogDesc(LAN_DESC_DEL, LAN_DESC_DEVICE_GROUP , GetLanDesc(LAN_DESC_DEVICE_GROUP_ID) + ":" + 
                cReq.GetDeviceGroupInfoKey().GetDeviceGroupID(), (s32)dwErrorCode);

            WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypePuConfig, (u32)dwErrorCode, strLogDesc);
        }
        break;
    case TAS_DEVICE_GROUP_MOD_REQ:
        {
            CDeviceGroupModReq cReq;
            cReq.FromXml(m_strReq);

            string strDevGroupID;
            cReq.GetDeviceGroupID(strDevGroupID);
            string strDevGroupName;
            cReq.GetDeviceGroupName(strDevGroupName);

            string strLogDesc = GetOptLogDesc(LAN_DESC_MOD, LAN_DESC_DEVICE_GROUP , GetLanDesc(LAN_DESC_DEVICE_GROUP_ID) + ":" +
                strDevGroupID + " " + GetLanDesc(LAN_DESC_DEVICE_GROUP_NAME) + ":" + strDevGroupName, (s32)dwErrorCode);

            WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypePuConfig, (u32)dwErrorCode, strLogDesc);
        }
		break;
	case TAS_DEVICE_GROUP_MOD_4GB_REQ:
		{
			CCuModifyGBDeviceGroupReq cReq;
			cReq.FromXml(m_strReq);

			string strDevGroupID = cReq.GetDevGrpId();
			string strDevGroupName;

			string strLogDesc = GetOptLogDesc(LAN_DESC_MOD, LAN_DESC_DEVICE_GROUP, GetLanDesc(LAN_DESC_DEVICE_GROUP_ID) + ":" +
				strDevGroupID + " " + GetLanDesc(LAN_DESC_DEVICE_GROUP_NAME) + ":" + strDevGroupName, (s32)dwErrorCode);

			WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypePuConfig, (u32)dwErrorCode, strLogDesc);
		}
		break;

    case TAS_DEVICE_GROUP_DEVICE_ORDER_REQ:
        {
			CDeviceGroupDeviceOrderReq cReq;
			cReq.FromXml(m_strReq);

            TVidSrcOrderInfo orderInfo = cReq.GetVidsrcOrderInfo();
            string strDevGroupID = orderInfo.GetDevGrpId();

			string strLogDesc = GetOptLogDesc(LAN_DESC_MOD, LAN_DESC_DEVICE_GROUP, GetLanDesc(LAN_DESC_DEVICE_GROUP_ID) + ":" +
				strDevGroupID , (s32)dwErrorCode);

			WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypePuConfig, (u32)dwErrorCode, strLogDesc);
        }
        break;

    case TAS_CUSER_GROUP_ADD_REQ:
        {
            CUserGroupAddReq cReq;
            cReq.FromXml(m_strReq);

            string strLogDesc = GetOptLogDesc(LAN_DESC_ADD, LAN_DESC_USERGROUP ,
                GetLanDesc(LAN_DESC_USERGROUP_NAME) +
                ":" + cReq.GetUserGroupInfo().GetGroupName(), (s32)dwErrorCode);

            WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypeUserManage, (u32)dwErrorCode, strLogDesc);
        }
        break;
    case TAS_CUSER_GROUP_DEL_REQ:
        {
            CUserGroupDelReq cReq;
            cReq.FromXml(m_strReq);

            string strLogDesc = GetOptLogDesc(LAN_DESC_DEL, LAN_DESC_USERGROUP , GetLanDesc(LAN_DESC_USERGROUP_ID) + ":" + 
                cReq.GetUserGroupInfoKey().GetGroupNO(), (s32)dwErrorCode);

            WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypeUserManage, (u32)dwErrorCode, strLogDesc);
        }
        break;
    case TAS_CUSER_GROUP_MOD_REQ:
        {
            CUserGroupModReq cReq;
            cReq.FromXml(m_strReq);

            string strGroupNo;
            cReq.GetGroupNO(strGroupNo);

            string strGroupName;
            cReq.GetGroupName(strGroupName);

            string strLogDesc = GetOptLogDesc(LAN_DESC_MOD, LAN_DESC_USERGROUP , GetLanDesc(LAN_DESC_USERGROUP_ID) + ":" +
                strGroupNo + " " + GetLanDesc(LAN_DESC_USERGROUP_NAME) + ":" + strGroupName, (s32)dwErrorCode);

            WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypeUserManage, (u32)dwErrorCode, strLogDesc);
        }
        break;
    case TAS_CUSER_ADD_REQ:
        {
            CUserAddReq cReq;
            cReq.FromXml(m_strReq);

            string strLogDesc = GetOptLogDesc(LAN_DESC_ADD, LAN_DESC_USER ,  GetLanDesc(LAN_DESC_USERNAME) +
                ":" + cReq.GetUserInfo().GetUserName(), (s32)dwErrorCode);

            WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypeUserManage, (u32)dwErrorCode, strLogDesc);
        }
        break;
	case TAS_DEVTREE_ADD_REQ:
		{
			CDevTreeAddReq cReq;
			cReq.FromXml(m_strReq);

			string devtreeName;
			devtreeName = cReq.GetDevtreeName();

			string strLogDesc = GetOptLogDesc(LAN_DESC_ADD, LAN_DESC_DEV_TREE , GetLanDesc(LAN_DESC_DEV_TREE_NAME) +
				":" + devtreeName, (s32)dwErrorCode);

			WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypeUserManage, (u32)dwErrorCode, strLogDesc);
		}
		break;
	case TAS_DEVTREE_MOD_REQ:
		{
			CDevTreeModReq cReq;
			cReq.FromXml(m_strReq);

			string devtreeId;
			devtreeId = cReq.GetDevtreeId();

			string devtreeName;
			devtreeName = cReq.GetDevtreeName();

			string strLogDesc = GetOptLogDesc(LAN_DESC_MOD, LAN_DESC_DEV_TREE_NAME , GetLanDesc(LAN_DESC_DEV_TREE_ID) + ":" +
				devtreeId + " " + GetLanDesc(LAN_DESC_DEV_TREE_NAME) + ":" + devtreeName, (u32)dwErrorCode);

			WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypeUserManage, (u32)dwErrorCode, strLogDesc);
		}
		break;
	case TAS_DEVTREE_DEL_REQ:
		{
			CDevTreeDelReq cReq;
			cReq.FromXml(m_strReq);

			string devtreeId;
			devtreeId = cReq.GetDevtreeId();

			string strLogDesc = GetOptLogDesc(LAN_DESC_DEL, LAN_DESC_DEV_TREE , GetLanDesc(LAN_DESC_DEV_TREE_ID) + ":" +
				devtreeId + " ", (u32)dwErrorCode);

			WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypeUserManage, (u32)dwErrorCode, strLogDesc);
		}
		break;
    case TAS_CUSER_DEL_REQ:
        {
            CUserDelReq cReq;
            cReq.FromXml(m_strReq);

            string strLogDesc = GetOptLogDesc(LAN_DESC_DEL, LAN_DESC_USER , GetLanDesc(LAN_DESC_USERID) + ":" + 
                cReq.GetUserID(), (s32)dwErrorCode);

            WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypeUserManage, (u32)dwErrorCode, strLogDesc);
        }
        break;
    case TAS_CUSER_MOD_REQ:
        {
            CUserModReq cReq;
            cReq.FromXml(m_strReq);

            string strUserID;
            cReq.GetUserID(strUserID); 

            string strUserName;
            cReq.GetUserName(strUserName);

            string strLogDesc = GetOptLogDesc(LAN_DESC_MOD, LAN_DESC_USER , GetLanDesc(LAN_DESC_USERID) + ":" +
                strUserID +  " " + GetLanDesc(LAN_DESC_USERNAME) + ":" + strUserName, (s32)dwErrorCode);

            WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypeUserManage, (u32)dwErrorCode, strLogDesc);
        }
        break;
    case TAS_TEMP_AUTHORIZATION_ADD_REQ:
        {
            CTempAuthorizationAddReq cReq;
            cReq.FromXml(m_strReq);

            string strLogDesc = GetOptLogDesc(LAN_DESC_ADD, LAN_DESC_AUTH, GetLanDesc(LAN_DESC_USERNAME) + 
                ":" + cReq.GetTempAuthorizationInfo().GetUsername() +
                " " + GetLanDesc(LAN_DESC_AUTH) + GetLanDesc(LAN_DESC_DEV) + GetLanDesc(LAN_DESC_DEVICE_ID) + 
                ":" + cReq.GetTempAuthorizationInfo().GetDeviceID(),
                (s32)dwErrorCode);

            WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypeUserManage, (u32)dwErrorCode, strLogDesc);
            if(m_pcCuAgent->m_tCmuURI.GetDomain() != cReq.GetTempAuthorizationInfo().GetDestinationDomain())
            {
                WriteLog(cReq.GetTempAuthorizationInfo().GetDestinationDomain(), 
                    enUserOpTypeUserManage, (u32)dwErrorCode, strLogDesc);
            }
        }
        break;
    case TAS_TEMP_AUTHORIZATION_DEL_REQ:
        {
            CTempAuthorizationDelReq cReq;
            cReq.FromXml(m_strReq);

            string strLogDesc = GetOptLogDesc(LAN_DESC_DEL, LAN_DESC_AUTH, GetLanDesc(LAN_DESC_USERNAME) + 
                ":" + cReq.GetTempAuthorizationInfoKey().GetUserName() +
                " " + GetLanDesc(LAN_DESC_AUTH) + GetLanDesc(LAN_DESC_DEV) + GetLanDesc(LAN_DESC_DEVICE_ID) +
                ":" + cReq.GetTempAuthorizationInfoKey().GetDeviceID(),
                (s32)dwErrorCode);

            WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypeUserManage, (u32)dwErrorCode, strLogDesc);
            if(m_pcCuAgent->m_tCmuURI.GetDomain() != cReq.GetTempAuthorizationInfoKey().GetDestinationDomain())
            {
                WriteLog(cReq.GetTempAuthorizationInfoKey().GetDestinationDomain(), 
                    enUserOpTypeUserManage, (u32)dwErrorCode, strLogDesc);
            }
        }
        break;
    case TAS_TEMP_AUTHORIZATION_MOD_REQ:
        {
            CTempAuthorizationModReq cReq;
            cReq.FromXml(m_strReq);

            string strDev;
            cReq.GetDeviceID(strDev);
            string strLogDesc = GetOptLogDesc(LAN_DESC_MOD, LAN_DESC_AUTH, 
                GetLanDesc(LAN_DESC_AUTH) + GetLanDesc(LAN_DESC_DEV) + GetLanDesc(LAN_DESC_DEVICE_ID)
                + ":" + strDev,
                (s32)dwErrorCode);

            WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypeUserManage, (u32)dwErrorCode, strLogDesc);
        }
        break;
    case TAS_TVWALL_ADD_REQ:
        {
            CCuTvwallAddReq cReq;
            cReq.FromXml(m_strReq);

            string strLogDesc = GetOptLogDesc(LAN_DESC_ADD, LAN_DESC_TVWALL,
                GetLanDesc(LAN_DESC_TVWALL_NAME) + ":" + cReq.GetTvwallInfo().GetDeviceName(),
                (s32)dwErrorCode);

            WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypeTVWall, (u32)dwErrorCode, strLogDesc);

        }
        break;
    case TAS_TVWALL_DEL_REQ:
        {
            CCuTvwallDelReq cReq;
            cReq.FromXml(m_strReq);

            string strLogDesc = GetOptLogDesc(LAN_DESC_DEL, LAN_DESC_TVWALL, GetLanDesc(LAN_DESC_TVWALLID) + " " +
                cReq.GetTvwallId(),
                (s32)dwErrorCode);

            WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypeTVWall, (u32)dwErrorCode, strLogDesc);
        }
        break;
    case TAS_TVWALL_MOD_REQ:
        {
            CCuTvwallModReq cReq;
            cReq.FromXml(m_strReq);

            string strID;
            cReq.GetId(strID);

            string strName;
            cReq.GetName(strName);

            string strLogDesc = GetOptLogDesc(LAN_DESC_MOD, LAN_DESC_TVWALL, GetLanDesc(LAN_DESC_TVWALLID) + ":" +
                strID + " " +
                GetLanDesc(LAN_DESC_TVWALL_NAME) + ":" + strName,
                (s32)dwErrorCode);

            WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypeTVWall, (u32)dwErrorCode, strLogDesc);
        }
        break;
    case TAS_DEVICE_MOD_REQ:
        {
            CCuDeviceNameModReq cReq;
            cReq.FromXml(m_strReq);

            string strID;
            cReq.GetDeviceUUID(strID);

            string strName;
            cReq.GetDeviceName(strName);

            string strLogDesc = GetOptLogDesc(LAN_DESC_MOD, LAN_DESC_DEV, GetLanDesc(LAN_DESC_DEVICE_ID) + ":" +
                strID + " " +
                GetLanDesc(LAN_DESC_DEV_NAME) + ":" + strName,
                (s32)dwErrorCode);

            WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypePuConfig, 
                (u32)dwErrorCode, strLogDesc, strID+"@"+::GetDomainName());
        }
        break;
    case TAS_DEVICE_CAP_INDEX_NAME_SET_REQ:
        {
            CCuDeviceCapIndexNameSetReq cReq;
            cReq.FromXml(m_strReq);

            string strLogDesc = GetOptLogDesc(LAN_DESC_SET, LAN_DESC_DEVCAP, GetLanDesc(LAN_DESC_DEVICE_ID) + ":" +
                cReq.GetDeviceCapName().deviceId,
                (s32)dwErrorCode);

            WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypePuConfig, (u32)dwErrorCode, strLogDesc
                , cReq.GetDeviceCapName().deviceId+"@"+::GetDomainName());
        }
        break;
    case TAS_DEVICE_CAP_INDEX_NAME_GET_REQ:
        {
        }
        break;
	case CUI_GET_AZIMUTH_REQ:
		{
		}
		break;
	case CONFIG_SYNC_SWITCH_GET_REQ:
		{
		}
		break;
	case CONFIG_SYNC_SWITCH_SET_REQ:
		{
			CCuSetSyncVideoSrcNameEnableReq cReq;
			cReq.FromXml( m_strReq );

			bool syncPu2Plat = false;
			bool syncPlat2Pu = false;

			cReq.GetSwichSets( SYNC_PUSRCNAME_TO_PLAT, syncPu2Plat );
			cReq.GetSwichSets( SYNC_PUSRCNAME_TO_PU, syncPlat2Pu );
			
			std::stringstream ss;
			ss << LAN_DESC_SYNC_VS_NAME_PU2PLAT << "=" << syncPu2Plat
				<< " " << LAN_DESC_SYNC_VS_NAME_PLAT2PU << "=" << syncPlat2Pu;

			string strLogDesc = GetOptLogDesc(LAN_DESC_SET, LAN_DESC_SYNC_SWITCH , ss.str(), (s32)dwErrorCode);
			
			WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypeUserManage, (u32)dwErrorCode, strLogDesc );
		}
		break;
    case PU_VID_ENC_PARAM_GET_REQ:
        {
        }
        break;
    case PU_VID_ENC_PARAM_SET_REQ:
        {
            CCuPuVidEncParamSetReq cReq;
            cReq.FromXml(m_strReq);

            string strLogDesc = GetOptLogDesc(LAN_DESC_SET,  LAN_DESC_VIDEOENCODER_PARAM, GetLanDesc(LAN_DESC_DEVICE_ID) + 
                ":" + cReq.GetDevUri() + " " + GetLanDesc(LAN_DESC_DEV_CHN) +
                TransInt2String(cReq.GetDevChn().GetChnNO() + 1), (s32)dwErrorCode);

            WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypePuConfig, (u32)dwErrorCode, strLogDesc, cReq.GetDevUri());
        }
		break;
	case PU_AUD_ENC_PARAM_GET_REQ:
		{
		}
		break;
	case PU_VIDSRC_OSD_SET_REQ:
		{
		}
		break;
	case PU_VIDSRC_OSD_GET_REQ:
		{
		}
		break;
    case PU_CAMERA_TYPE_SET_REQ:
        {
        }
        break;
	case PU_ALARM_RESET_REQ:
		{
		}
		break;
    case PU_MOTION_DETECT_GET_REQ:
        {
        }
        break;
    case PU_REC_DAYS_QRY_REQ:
        {

        }
        break;
	case PU_RECORD_START_REQ:
		{
			CCuPuRecordStartReq cReq;
			cReq.FromXml(m_strReq);

			WriteLog(cReq.GetDevChn(),(s32)dwErrorCode,PU_RECORD_START_REQ,GetDomainName());
		}
		break;
	case PU_RECORD_STOP_REQ:
		{
			CCuPuRecordStopReq cReq;
			cReq.FromXml(m_strReq);

			WriteLog(cReq.GetDevChn(),(s32)dwErrorCode,PU_RECORD_STOP_REQ,GetDomainName());
		}
		break;
    case PU_MOTION_DETECT_SET_REQ:
        {
            CCuPuMotionDetectSetReq cReq;
            cReq.FromXml(m_strReq);

            string strLogDesc = GetOptLogDesc(LAN_DESC_SET, LAN_DESC_MOTION_DETECT, GetLanDesc(LAN_DESC_DEVICE_ID) + 
                ":" + cReq.GetDevUri() + " " + GetLanDesc(LAN_DESC_DEV_CHN) + ":" +
                TransInt2String(cReq.GetDevChn().GetChnNO()+1), (s32)dwErrorCode);

            WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypePuConfig, (u32)dwErrorCode, strLogDesc, cReq.GetDevUri());
        }
        break;
    case PU_AREA_SHADE_GET_REQ:
        {
        }
        break;
    case PU_AREA_SHADE_SET_REQ:
        {
            CCuPuAreaShadeSetReq cReq;
            cReq.FromXml(m_strReq);

            string strLogDesc = GetOptLogDesc(LAN_DESC_SET, LAN_DESC_AREA_SHADE, GetLanDesc(LAN_DESC_DEVICE_ID) + 
                ":" + cReq.GetDevUri() + " " + GetLanDesc(LAN_DESC_DEV_CHN) + ":" +
                TransInt2String(cReq.GetDevChn().GetChnNO()), (s32)dwErrorCode);

            WriteLog(m_pcCuAgent->m_tCmuURI.GetDomain(), enUserOpTypePuConfig, (u32)dwErrorCode, strLogDesc, cReq.GetDevUri());
        }
        break;
    default:
        break;
    }
}

CSingleInterAction::~CSingleInterAction()
{
    TerminateTask();
}
//////////////////////////////////////////////////////////////////////////
CQueryTask::CQueryTask( CInstExt *pcInst, CCuAgent *pcCuAgent, PTRecvNode ptRecvNode ) : CSipTask(pcInst)
{
    CMS_ASSERT(pcCuAgent != NULL);
    CMS_ASSERT(ptRecvNode != NULL);
    m_pcCuAgent = pcCuAgent;
    m_ptRecvNode = ptRecvNode;
    m_pRsp = NULL;
    m_pc3acAgent = NULL;
    m_dwReqEvent = 0;
    m_pcCuAgent->AddSubTask(this);
}

CQueryTask::~CQueryTask()
{
    TerminateTask();
    if ( ! m_strQueryTag.empty() )
    {
        g_cCuiDataMgr.m_mapSessToTaskMap.Erase(m_strQueryTag);
        m_strQueryTag.clear();
    }

    if ( NULL != m_pRsp )
    {
        delete m_pRsp;
        m_pRsp = NULL;
    }
}

void CQueryTask::TerminateTask()
{
    m_pcCuAgent->DelSubTask(this);
    while(!m_VecNtfReqList.empty())
    { 
        string strNtfMsg = m_VecNtfReqList.front();
        m_VecNtfReqList.pop_front();
        COspSipMsg *pcOspSipMsg = (COspSipMsg *)strNtfMsg.c_str();
        TASKLOG(NORMAL_TASK, EVENT_LEV, "有未处理的NTF消息[%s],直接回复\n", OspExtEventDesc(pcOspSipMsg->GetOspEventID()).c_str());
        CNtfRsp cRsp;
        cRsp.SetEvent(pcOspSipMsg->GetOspEventID() + 1);
        string strSession = GetSipSessionID(pcOspSipMsg->GetMsgBody());
        cRsp.SetSession(strSession);
        string strSeqNum = GetSipSeqNO(pcOspSipMsg->GetMsgBody());
        int nSeqNum = static_cast<int>(atol(strSeqNum.c_str()));
        cRsp.SetSeqNum(nSeqNum);
        PostMsgRsp(pcOspSipMsg->GetSipTransID(), cRsp);
    }

    //     if ( GetState() != NOT_QUERY_STATE && m_ptRecvNode != NULL )
    //     {
    //         TASKLOG(NORMAL_TASK, EVENT_LEV, "销毁CQueryTask\n");
    //         m_pRsp->SetErrorCode(ERR_CUI_TASK_DEL);
    //         m_strRspOfErr = m_pRsp->ToXml();
    //         SendRspToCu(m_strRspOfErr, m_ptRecvNode);
    //     }
}

void CQueryTask::InitStateMachine()
{
    CStateProc cNotQueryProc;
    cNotQueryProc.ProcMsg = (CSipTask::PFProcMsg)&CQueryTask::NotQueryProcMsg;
    cNotQueryProc.TimerPoll = &CSipTask::TimerPollSkip;
    cNotQueryProc.ProcErrMsg = (CSipTask::PFProcMsg)&CQueryTask::NotQueryProcMsg;
    AddRuleProc(NOT_QUERY_STATE, cNotQueryProc);

    CStateProc cWaitRspProc;
    cWaitRspProc.ProcMsg = (CSipTask::PFProcMsg)&CQueryTask::WaitRspProcMsg;
    cWaitRspProc.TimerPoll = (CSipTask::PFTimerPoll)&CQueryTask::WaitRspTimerPoll;
    cWaitRspProc.ProcErrMsg = (CSipTask::PFProcMsg)&CQueryTask::WaitRspProcMsg;
    AddRuleProc(WAIT_RESPONSE_STATE, cWaitRspProc);

    CStateProc cWaitNotifyProc;
    cWaitNotifyProc.ProcMsg = (CSipTask::PFProcMsg)&CQueryTask::WaitNotifyProcMsg;
    cWaitNotifyProc.TimerPoll = (CSipTask::PFTimerPoll)&CQueryTask::WaitNotifyTimerPoll;
    cWaitNotifyProc.ProcErrMsg = (CSipTask::PFProcMsg)&CQueryTask::WaitNotifyProcMsg;
    AddRuleProc(WAIT_NOTIFY_STATE, cWaitNotifyProc);

    NextState(NOT_QUERY_STATE);
}

u32 CQueryTask::NotQueryProcMsg( CMessage *const pcMsg )
{
    TASKLOG(NORMAL_TASK, EVENT_LEV, "[CQueryTask]收到CU用户[%s]会话[%s]的请求[%s--%hu]\n",
        m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

    switch (pcMsg->event)
    {
    case TAS_CUSER_QRY_REQ:
        {
            //             CCuQueryUserReq *pcReq = (CCuQueryUserReq *)pcMsg->content;
            //             //向3AS发送查询请求，请求可能是查询CU自身的信息，也可能是查询CU的子用户信息
            //             //所以查询条件应该由cui_fcgi指定
            //             OnNotQueryMsg<CUserQryReq, CCuQueryUserRsp>(*pcReq);

            CCuQueryUserReq *pcReq = (CCuQueryUserReq *)pcMsg->content;
            m_dwReqEvent = (u16)pcReq->GetEventId();

            string strTmp;

            pcReq->GetUserInfoQueryCondition().GetUserID(strTmp);
            if(!strTmp.empty())
            {
                pcReq->GetUserInfoQueryCondition().SetUserID(GetNameWithoutDomainFromURI(strTmp));
                strTmp.clear();
            } 

            pcReq->GetUserInfoQueryCondition().GetCuserID(strTmp);
            if(!strTmp.empty())
            {
                pcReq->GetUserInfoQueryCondition().SetCuserID(GetNameWithoutDomainFromURI(strTmp));
                strTmp.clear();
            }

            pcReq->GetUserInfoQueryCondition().GetUserName(strTmp);
            if(!strTmp.empty())
            {
                pcReq->GetUserInfoQueryCondition().SetUserName(GetNameWithoutDomainFromURI(strTmp));
            }

            //1 向服务器发送请求, 请求类型必须是服务器定义的接口类型
            u32 dwRet = PROCMSG_OK;

            dwRet = PostMsgReqTo3AS(*pcReq);

            if ( PROCMSG_DEL == dwRet )
            {
                TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]发送请求[%s--%d]，TAS不在线，直接拒绝请求\n",
                    m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
                    pcReq->GetEventStr().c_str(), pcReq->GetEventId());
                CCuQueryUserRsp cRsp;
                cRsp.SetSeqNum(pcReq->GetSeqNum());
                cRsp.SetSession(pcReq->GetSession());
                cRsp.SetErrorCode(ERR_CUI_TAS_OFFLINE);
                m_strRspOfErr = cRsp.ToXml();
                SendRspToCu(m_strRspOfErr, m_ptRecvNode);
                NextState(NOT_QUERY_STATE);
                delete this;
                return dwRet;
            }

            //将QueryTag插入到会话表中，等待NTF
            g_cCuiDataMgr.m_mapSessToTaskMap.Insert(pcReq->GetQueryTag(), GetTaskNO());
            m_strQueryTag = pcReq->GetQueryTag();

            //2 转换状态，等待应答
            NextState(WAIT_RESPONSE_STATE);

            //3 创建结果保存数据，超时时复用此结果数据
            m_pRsp = new CCuQueryUserRsp;
            m_pRsp->SetSeqNum(pcReq->GetSeqNum());
            m_pRsp->SetSession(pcReq->GetSession());
            return PROCMSG_OK;
        }
        break;
    case TAS_CUSER_GROUP_QRY_REQ:
        {
            CCuUserGroupQryReq *pcReq = (CCuUserGroupQryReq *)pcMsg->content;

            string strCuserID;
            pcReq->GetUserGroupInfoQueryCondition().GetCuserID(strCuserID);
            if(!strCuserID.empty())
            {
                pcReq->GetUserGroupInfoQueryCondition().SetCuserID(GetNameWithoutDomainFromURI(strCuserID));
            }

            string strCuserGroupID;
            pcReq->GetUserGroupInfoQueryCondition().GetGroupNO(strCuserGroupID);
            if(!strCuserGroupID.empty())
            {
                pcReq->GetUserGroupInfoQueryCondition().SetGroupNO(GetNameWithoutDomainFromURI(strCuserGroupID));
            }

            OnNotQueryMsg<CUserGroupQryReq, CCuUserGroupQryRsp>(*pcReq);
        }
        break;
    //case TAS_DEVICE_GROUP_QRY_REQ:
    //    {
    //        CCuUserDevGroupQryReq *pcReq = (CCuUserDevGroupQryReq *)pcMsg->content;
    //        string strCuserID;
    //        pcReq->GetDeviceGroupInfoQueryCondition().GetCuserID(strCuserID);
    //        if(!strCuserID.empty())
    //        {
    //            pcReq->GetDeviceGroupInfoQueryCondition().SetCuserID(GetNameWithoutDomainFromURI(strCuserID));
    //        }

    //        string strGroupID;
    //        pcReq->GetDeviceGroupInfoQueryCondition().GetDeviceGroupID(strGroupID);
    //        if(!strGroupID.empty())
    //        {
    //            pcReq->GetDeviceGroupInfoQueryCondition().SetDeviceGroupID(GetNameWithoutDomainFromURI(strGroupID));
    //        }

    //        string strParentID;
    //        pcReq->GetDeviceGroupInfoQueryCondition().GetDeviceParentGroupID(strParentID);
    //        if(!strParentID.empty())
    //        {
    //            pcReq->GetDeviceGroupInfoQueryCondition().SetDeviceParentGroupID(GetNameWithoutDomainFromURI(strParentID));
    //        }

    //        OnNotQueryMsg<CDeviceGroupQryReq, CCuUserDevGroupQryRsp>(*pcReq);
    //    }
    //    break;
    case TAS_CUSER_DEVICEFULLINFO_AND_PRIVILEGE_QRY_REQ:
        {
            CCuserDevicefullinfoAndPrivilegeQryReq *pcReq = (CCuserDevicefullinfoAndPrivilegeQryReq *)pcMsg->content;
            pcReq->SetCuserId(GetNameWithoutDomainFromURI(pcReq->GetCuserId()));

            string strGroupID;
            pcReq->GetCondition().GetDevicegroupId(strGroupID);
            if(!strGroupID.empty())
            {
                pcReq->GetCondition().SetDevicegroupId(GetNameWithoutDomainFromURI(strGroupID));
            }

            OnNotQueryMsg<CCuserDevicefullinfoAndPrivilegeQryReq, CCuUserDevFullInfoQryRsp>(*pcReq);
        }
        break;
    case TAS_DEVICE_GROUP_DEVICE_QRY_REQ:
        {
            //             CCuQryDevGroupOfDevInfoReq *pcReq = (CCuQryDevGroupOfDevInfoReq *)pcMsg->content;
            //             OnNotQueryMsg<CDeviceGroupDeviceQryReq, CCuQryDevGroupOfDevInfoRsp>(*pcReq);
            CCuQryDevGroupOfDevInfoReq *pcReq = (CCuQryDevGroupOfDevInfoReq *)pcMsg->content;
            m_dwReqEvent = (u16)pcReq->GetEventId();

            string strTmp;
            pcReq->GetDeviceGroupDeviceInfoQueryCondition().GetCuserID(strTmp);
            if(!strTmp.empty())
            {
                pcReq->GetDeviceGroupDeviceInfoQueryCondition().SetCuserID(GetNameWithoutDomainFromURI(strTmp));
                strTmp.clear();
            }


            pcReq->GetDeviceGroupDeviceInfoQueryCondition().GetDeviceID(strTmp);
            if(!strTmp.empty())
            {
                pcReq->GetDeviceGroupDeviceInfoQueryCondition().SetDeviceID(GetNameWithoutDomainFromURI(strTmp));
                strTmp.clear();
            }


            pcReq->GetDeviceGroupDeviceInfoQueryCondition().GetDeviceGroupID(strTmp);
            if(!strTmp.empty())
            {
                pcReq->GetDeviceGroupDeviceInfoQueryCondition().SetDeviceGroupID(GetNameWithoutDomainFromURI(strTmp));
            }

            //1 向服务器发送请求, 请求类型必须是服务器定义的接口类型
            u32 dwRet = PROCMSG_OK;

            dwRet = PostMsgReqTo3AS(*pcReq);

            if ( PROCMSG_DEL == dwRet )
            {
                TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]发送请求[%s--%d]，TAS不在线，直接拒绝请求\n",
                    m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
                    pcReq->GetEventStr().c_str(), pcReq->GetEventId());
                CCuQryDevGroupOfDevInfoRsp cRsp;
                cRsp.SetSeqNum(pcReq->GetSeqNum());
                cRsp.SetSession(pcReq->GetSession());
                cRsp.SetErrorCode(ERR_CUI_TAS_OFFLINE);
                m_strRspOfErr = cRsp.ToXml();
                SendRspToCu(m_strRspOfErr, m_ptRecvNode);
                NextState(NOT_QUERY_STATE);
                delete this;
                return dwRet;
            }

            //将QueryTag插入到会话表中，等待NTF
            g_cCuiDataMgr.m_mapSessToTaskMap.Insert(pcReq->GetQueryTag(), GetTaskNO());
            m_strQueryTag = pcReq->GetQueryTag();

            //2 转换状态，等待应答
            NextState(WAIT_RESPONSE_STATE);

            //3 创建结果保存数据，超时时复用此结果数据
            m_pRsp = new CCuQryDevGroupOfDevInfoRsp  ;
            m_pRsp->SetSeqNum(pcReq->GetSeqNum());
            m_pRsp->SetSession(pcReq->GetSession());
            return PROCMSG_OK;
        }
        break;
    case TAS_CUSER_DEVICE_QRY_REQ:
        {
            //             CCuGetUserDeviceReq *pcReq = (CCuGetUserDeviceReq *)pcMsg->content;
            //             OnNotQueryMsg<CUserDeviceQryReq, CCuGetUserDeviceRsp>(*pcReq);
            CCuGetUserDeviceReq *pcReq = (CCuGetUserDeviceReq *)pcMsg->content;
            m_dwReqEvent = (u16)pcReq->GetEventId();

            string strTmp;

            pcReq->GetUserDeviceInfoQueryCondition().GetUserID(strTmp);
            if(!strTmp.empty())
            {
                pcReq->GetUserDeviceInfoQueryCondition().SetUserID(GetNameWithoutDomainFromURI(strTmp));
                strTmp.clear();
            }

            pcReq->GetUserDeviceInfoQueryCondition().GetDeviceID(strTmp);
            if(!strTmp.empty())
            {
                pcReq->GetUserDeviceInfoQueryCondition().SetDeviceID(GetNameWithoutDomainFromURI(strTmp));
                strTmp.clear();
            }

            pcReq->GetUserDeviceInfoQueryCondition().GetDevGrpID(strTmp);
            if(!strTmp.empty())
            {
                pcReq->GetUserDeviceInfoQueryCondition().SetDevGrpID(GetNameWithoutDomainFromURI(strTmp));
                strTmp.clear();
            }

            //1 向服务器发送请求, 请求类型必须是服务器定义的接口类型
            u32 dwRet = PROCMSG_OK;

            dwRet = PostMsgReqTo3AS(*pcReq);

            if ( PROCMSG_DEL == dwRet )
            {
                TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]发送请求[%s--%d]，TAS不在线，直接拒绝请求\n",
                    m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
                    pcReq->GetEventStr().c_str(), pcReq->GetEventId());
                CCuGetUserDeviceRsp cRsp;
                cRsp.SetSeqNum(pcReq->GetSeqNum());
                cRsp.SetSession(pcReq->GetSession());
                cRsp.SetErrorCode(ERR_CUI_TAS_OFFLINE);
                m_strRspOfErr = cRsp.ToXml();
                SendRspToCu(m_strRspOfErr, m_ptRecvNode);
                NextState(NOT_QUERY_STATE);
                delete this;
                return dwRet;
            }

            //将QueryTag插入到会话表中，等待NTF
            g_cCuiDataMgr.m_mapSessToTaskMap.Insert(pcReq->GetQueryTag(), GetTaskNO());
            m_strQueryTag = pcReq->GetQueryTag();

            //2 转换状态，等待应答
            NextState(WAIT_RESPONSE_STATE);

            //3 创建结果保存数据，超时时复用此结果数据
            m_pRsp = new CCuGetUserDeviceRsp;
            m_pRsp->SetSeqNum(pcReq->GetSeqNum());
            m_pRsp->SetSession(pcReq->GetSession());
            return PROCMSG_OK;
        }
        break;
    case TAS_CUSER_GROUP_DEVICE_QRY_REQ:
        {
            CCuGetUserGroupDeviceReq *pcReq = (CCuGetUserGroupDeviceReq *)pcMsg->content;

            string strTemp;
            pcReq->GetUserGroupDeviceInfoQueryCondition().GetDevGrpID(strTemp);
            if(!strTemp.empty())
            {
                pcReq->GetUserGroupDeviceInfoQueryCondition().SetDevGrpID(GetNameWithoutDomainFromURI(strTemp));
                strTemp.clear();
            }

            pcReq->GetUserGroupDeviceInfoQueryCondition().GetDeviceID(strTemp);
            if(!strTemp.empty())
            {
                pcReq->GetUserGroupDeviceInfoQueryCondition().SetDeviceID(GetNameWithoutDomainFromURI(strTemp));
                strTemp.clear();
            }

            pcReq->GetUserGroupDeviceInfoQueryCondition().GetCuserGrpID(strTemp);
            if(!strTemp.empty())
            {
                pcReq->GetUserGroupDeviceInfoQueryCondition().SetCuserGrpID(GetNameWithoutDomainFromURI(strTemp));
                strTemp.clear();
            }

            OnNotQueryMsg<CUserGroupDeviceQryReq, CCuGetUserGroupDeviceRsp>(*pcReq);
        }
		break;
	case TAS_DEVICE_GROUP_DEVICE_QRY_4GB_REQ:
		{
			CCuDeviceGroupDeviceQry4GbReq *pcReq = (CCuDeviceGroupDeviceQry4GbReq *)pcMsg->content;

			string strTemp;
			pcReq->GetCondition().GetDevicegroupId(strTemp);
			if (!strTemp.empty())
			{
				pcReq->GetCondition().SetDevicegroupId(GetNameWithoutDomainFromURI(strTemp));
				strTemp.clear();
			}

			OnNotQueryMsg<CCuDeviceGroupDeviceQry4GbReq, CCuDeviceGroupDeviceQry4GbRsp>(*pcReq);
		}
		break;
	//case TAS_DEVICE_AUTO_ASSIGN_TO_USER_QRY_REQ:
	//	{
	//		CCuDeviceAutoAssignToUserQryReq *pcReq = (CCuDeviceAutoAssignToUserQryReq *)pcMsg->content;
	//		OnNotQueryMsg<CCuDeviceAutoAssignToUserQryReq, CCuDeviceAutoAssignToUserQryRsp>(*pcReq);
	//	}
	//	break;
	//case TAS_DEVICE_AUTO_ASSIGN_TO_USER_GROUP_QRY_REQ:
	//	{
	//		CCuDeviceAutoAssignToUserGroupQryReq *pcReq = (CCuDeviceAutoAssignToUserGroupQryReq *)pcMsg->content;
	//		OnNotQueryMsg<CCuDeviceAutoAssignToUserGroupQryReq, CCuDeviceAutoAssignToUserGroupQryRsp>(*pcReq);
	//	}
	//	break;
	case TAS_DEVICE_AUTO_ASSIGN_TO_USER_QRY_REQ:
		{
			CCuDeviceAutoAssignToUserQryReq *pcReq = (CCuDeviceAutoAssignToUserQryReq*)pcMsg->content;
			string strTmp;

			strTmp = pcReq->GetUserId();
			if(!strTmp.empty())
			{
				pcReq->SetUserId(GetNameWithoutDomainFromURI(pcReq->GetUserId()));
				strTmp.clear();
			}

			OnNotQueryMsg<CCuDeviceAutoAssignToUserQryReq, CCuDeviceAutoAssignToUserQryRsp>(*pcReq);
		}
		break;
	case TAS_DEVICE_AUTO_ASSIGN_TO_USER_GROUP_QRY_REQ:
		{
			CCuDeviceAutoAssignToUserGroupQryReq *pcReq = (CCuDeviceAutoAssignToUserGroupQryReq*)pcMsg->content;
			string strTmp;

			strTmp = pcReq->GetUserGroupId();
			if(!strTmp.empty())
			{
				pcReq->SetUserGroupId(GetNameWithoutDomainFromURI(pcReq->GetUserGroupId()));
				strTmp.clear();
			}

			OnNotQueryMsg<CCuDeviceAutoAssignToUserGroupQryReq, CCuDeviceAutoAssignToUserGroupQryRsp>(*pcReq);
		}
		break;
    case TAS_TEMP_AUTHORIZATION_QRY_REQ:
        {
            CCuGetAuthorizationDeviceReq *pcReq = (CCuGetAuthorizationDeviceReq *)pcMsg->content;
            string strTmp;
            pcReq->GetTempAuthorizationInfoQueryCondition().GetDeviceID(strTmp);
            if(!strTmp.empty())
            {
                pcReq->GetTempAuthorizationInfoQueryCondition().SetDeviceID(GetNameWithoutDomainFromURI(strTmp));
                strTmp.clear();
            }

            pcReq->GetTempAuthorizationInfoQueryCondition().GetCuserID(strTmp);
            if(!strTmp.empty())
            {
                pcReq->GetTempAuthorizationInfoQueryCondition().SetCuserID(GetNameWithoutDomainFromURI(strTmp));
                strTmp.clear();
            }

            pcReq->GetTempAuthorizationInfoQueryCondition().GetUsername(strTmp);
            if(!strTmp.empty())
            {
                pcReq->GetTempAuthorizationInfoQueryCondition().SetUsername(GetNameWithoutDomainFromURI(strTmp));
                strTmp.clear();
            }

            OnNotQueryMsg<CTempAuthorizationQryReq, CCuGetAuthorizationDeviceRsp>(*pcReq);
        }
        break;
    case TAS_DEVICE_QRY_REQ:
        {
            CCuGetDeviceBaseInfoReq *pcReq = (CCuGetDeviceBaseInfoReq *)pcMsg->content;

            string strTmp;
            pcReq->GetDeviceInfoQueryCondition().GetCuserID(strTmp);
            if(!strTmp.empty())
            {
                pcReq->GetDeviceInfoQueryCondition().SetCuserID(GetNameWithoutDomainFromURI(strTmp));
                strTmp.clear();
            }

            pcReq->GetDeviceInfoQueryCondition().GetDeviceUUID(strTmp);
            if(!strTmp.empty())
            {
                pcReq->GetDeviceInfoQueryCondition().SetDeviceUUID(GetNameWithoutDomainFromURI(strTmp));
                strTmp.clear();
            }

            OnNotQueryMsg<CDeviceQryReq, CCuGetDeviceBaseInfoRsp>(*pcReq);
        }
        break;
	case TAS_DEVICE_GBINFO_QRY_REQ:
		{
			CCuDeviceGbinfoQryReq *pcReq = (CCuDeviceGbinfoQryReq *)pcMsg->content;
			OnNotQueryMsg<CCuDeviceGbinfoQryReq, CCuDeviceGbinfoQryRsp>(*pcReq);
		}
		break;
	case TAS_DEVGRP_GBINFO_QRY_REQ:
		{
			CCuDevgrpGbinfoQryReq *pcReq = (CCuDevgrpGbinfoQryReq *)pcMsg->content;
			OnNotQueryMsg<CCuDevgrpGbinfoQryReq, CCuDevgrpGbinfoQryRsp>(*pcReq);
		}
		break;
	case TAS_VIDSRC_INFO_QRY_REQ:
		{
			CCuVidsrcInfoQryReq *pcReq = (CCuVidsrcInfoQryReq *)pcMsg->content;

			vector<CDevVidsrc> &devVsList = pcReq->GetDevVidsrcs();
			for (size_t nIndex = 0; nIndex < devVsList.size(); nIndex++)
			{
				devVsList[nIndex].GetDevId() = GetNameWithoutDomainFromURI(devVsList[nIndex].GetDevId());
			}

			pcReq->SetStartEntryIndex(0);
			pcReq->SetExpectEntryNum(0);

			OnNotQueryMsg<CVidsrcInfoQryReq, CCuVidsrcInfoQryRsp>(*pcReq);
		}
		break;
    case TAS_TVWALL_QRY_REQ:
        {
            //             CCuTvwallQryReq *pcReq = (CCuTvwallQryReq *)pcMsg->content;
            //             OnNotQueryMsg<CTvwallQryReq, CCuTvwallQryRsp>(*pcReq);
            CCuTvwallQryReq *pcReq = (CCuTvwallQryReq *)pcMsg->content;

            string strTmp;
            pcReq->GetCondition().GetCuserId(strTmp);
            if(!strTmp.empty())
            {
                pcReq->GetCondition().SetCuserId(GetNameWithoutDomainFromURI(strTmp));
                strTmp.clear();
            }

            pcReq->GetCondition().GetTvwallId(strTmp);
            if(!strTmp.empty())
            {
                pcReq->GetCondition().SetTvwallId(strTmp);
                strTmp.clear();
            }

            pcReq->GetCondition().GetCuserGrpId(strTmp);
            if(!strTmp.empty())
            {
                pcReq->GetCondition().SetCuserGrpId(GetNameWithoutDomainFromURI(strTmp));
            }

            m_dwReqEvent = (u16)pcReq->GetEventId();
            //1 向服务器发送请求, 请求类型必须是服务器定义的接口类型
            u32 dwRet = PROCMSG_OK;

            dwRet = PostMsgReqTo3AS(*pcReq);

            if ( PROCMSG_DEL == dwRet )
            {
                TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]发送请求[%s--%d]，TAS不在线，直接拒绝请求\n",
                    m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
                    pcReq->GetEventStr().c_str(), pcReq->GetEventId());
                CCuTvwallQryRsp cRsp;
                cRsp.SetSeqNum(pcReq->GetSeqNum());
                cRsp.SetSession(pcReq->GetSession());
                cRsp.SetErrorCode(ERR_CUI_TAS_OFFLINE);
                m_strRspOfErr = cRsp.ToXml();
                SendRspToCu(m_strRspOfErr, m_ptRecvNode);
                NextState(NOT_QUERY_STATE);
                delete this;
                return dwRet;
            }

            //将QueryTag插入到会话表中，等待NTF
            g_cCuiDataMgr.m_mapSessToTaskMap.Insert(pcReq->GetQueryTag(), GetTaskNO());
            m_strQueryTag = pcReq->GetQueryTag();

            //2 转换状态，等待应答
            NextState(WAIT_RESPONSE_STATE);

            //3 创建结果保存数据，超时时复用此结果数据
            m_pRsp = new CCuTvwallQryRsp;
            m_pRsp->SetSeqNum(pcReq->GetSeqNum());
            m_pRsp->SetSession(pcReq->GetSession());
            return PROCMSG_OK;
        }
        break;
    case TAS_DOMAIN_RELATION_QRY_REQ:
        {
            CCuDomainRelationQryReq *pcReq = (CCuDomainRelationQryReq *)pcMsg->content;
            OnNotQueryMsg<CDomainRelationQryReq, CCuDomainRelationQryRsp>(*pcReq);
        }
        break;
	case TAS_DEVTREE_QRY_REQ:
		{
			CCuDevTreeQryReq *pcReq = (CCuDevTreeQryReq *)pcMsg->content;
			
			m_dwReqEvent = (u16)pcReq->GetEventId();
			//1 向服务器发送请求, 请求类型必须是服务器定义的接口类型
			u32 dwRet = PROCMSG_OK;

			dwRet = PostMsgReqTo3AS(*pcReq);

			if ( PROCMSG_DEL == dwRet )
			{
				TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]发送请求[%s--%d]，TAS不在线，直接拒绝请求\n",
					m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
					pcReq->GetEventStr().c_str(), pcReq->GetEventId());
				CCuDevTreeQryRsp cRsp;
				cRsp.SetSeqNum(pcReq->GetSeqNum());
				cRsp.SetSession(pcReq->GetSession());
				cRsp.SetErrorCode(ERR_CUI_TAS_OFFLINE);
				m_strRspOfErr = cRsp.ToXml();
				SendRspToCu(m_strRspOfErr, m_ptRecvNode);
				NextState(NOT_QUERY_STATE);
				delete this;
				return dwRet;
			}

			//将QueryTag插入到会话表中，等待NTF
			g_cCuiDataMgr.m_mapSessToTaskMap.Insert(pcReq->GetQueryTag(), GetTaskNO());
			m_strQueryTag = pcReq->GetQueryTag();

			//2 转换状态，等待应答
			NextState(WAIT_RESPONSE_STATE);

			//3 创建结果保存数据，超时时复用此结果数据
			m_pRsp = new CCuDevTreeQryRsp;
			m_pRsp->SetSeqNum(pcReq->GetSeqNum());
			m_pRsp->SetSession(pcReq->GetSession());
			return PROCMSG_OK;
		}
		break;
	case TAS_DEVICE_GROUPPATH_QRY_REQ:
		{
			CCuDeviceGrpPathQryReq *pcReq = (CCuDeviceGrpPathQryReq *)pcMsg->content;

			m_dwReqEvent = (u16)pcReq->GetEventId();
			//1 向服务器发送请求, 请求类型必须是服务器定义的接口类型
			u32 dwRet = PROCMSG_OK;

			string strTmp;
			
			strTmp = pcReq->GetDeviceId();
			if(!strTmp.empty())
			{
				pcReq->SetDeviceId(GetNameWithoutDomainFromURI(strTmp));
				strTmp.clear();
			}

			dwRet = PostMsgReqTo3AS(*pcReq);

			if ( PROCMSG_DEL == dwRet )
			{
				TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]发送请求[%s--%d]，TAS不在线，直接拒绝请求\n",
					m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
					pcReq->GetEventStr().c_str(), pcReq->GetEventId());
				CCuDeviceGrpPathQryRsp cRsp;
				cRsp.SetSeqNum(pcReq->GetSeqNum());
				cRsp.SetSession(pcReq->GetSession());
				cRsp.SetErrorCode(ERR_CUI_TAS_OFFLINE);
				m_strRspOfErr = cRsp.ToXml();
				SendRspToCu(m_strRspOfErr, m_ptRecvNode);
				NextState(NOT_QUERY_STATE);
				delete this;
				return dwRet;
			}

			//将QueryTag插入到会话表中，等待NTF
			g_cCuiDataMgr.m_mapSessToTaskMap.Insert(pcReq->GetQueryTag(), GetTaskNO());
			m_strQueryTag = pcReq->GetQueryTag();

			//2 转换状态，等待应答
			NextState(WAIT_RESPONSE_STATE);

			//3 创建结果保存数据，超时时复用此结果数据
			m_pRsp = new CCuDeviceGrpPathQryRsp;
			m_pRsp->SetSeqNum(pcReq->GetSeqNum());
			m_pRsp->SetSession(pcReq->GetSession());
			return PROCMSG_OK;
		}
		break;
	//case PU_PTZ_CTRL_REQ:
	//	{
	//		CPuPtzCtrlReq *pcReq = (CPuPtzCtrlReq)pcMsg->content;

	//		CPtzLockQryReq cPtzLockQryReq;
	//		cPtzLockQryReq.SetSession;

	//		m_dwReqEvent = TAS_PTZLOCK_QRY_REQ;

	//		//1 向服务器发送请求, 请求类型必须是服务器定义的接口类型
	//		u32 dwRet = PROCMSG_OK;

	//		string strTmp = pcReq->GetDevChn().GetDevUri();
	//		if (!strTmp.empty())
	//		{
	//			cPtzLockQryReq.GetCondition().SetDeviceID(GetNameWithoutDomainFromURI(strTmp));
	//			strTmp.clear();
	//		}
	//		cPtzLockQryReq.GetCondition().SetVideoSrcId(pcReq->GetDevChn().GetChnNO);

	//		dwRet = PostMsgReqTo3AS(&cPtzLockQryReq);

	//		if (PROCMSG_DEL == dwRet)
	//		{
	//			TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]发送请求[%s--%d]，TAS不在线，直接拒绝请求\n",
	//				m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
	//				pcReq->GetEventStr().c_str(), pcReq->GetEventId());
	//			CCuDeviceGrpPathQryRsp cRsp;
	//			cRsp.SetSeqNum(pcReq->GetSeqNum());
	//			cRsp.SetSession(pcReq->GetSession());
	//			cRsp.SetErrorCode(ERR_CUI_TAS_OFFLINE);
	//			m_strRspOfErr = cRsp.ToXml();
	//			SendRspToCu(m_strRspOfErr, m_ptRecvNode);
	//			NextState(NOT_QUERY_STATE);
	//			delete this;
	//			return dwRet;
	//		}

	//		//将QueryTag插入到会话表中，等待NTF
	//		g_cCuiDataMgr.m_mapSessToTaskMap.Insert(pcReq->GetQueryTag(), GetTaskNO());
	//		m_strQueryTag = pcReq->GetQueryTag();

	//		//2 转换状态，等待应答
	//		NextState(WAIT_RESPONSE_STATE);

	//		//3 创建结果保存数据，超时时复用此结果数据
	//		m_pRsp = new CCuDeviceGrpPathQryRsp;
	//		m_pRsp->SetSeqNum(pcReq->GetSeqNum());
	//		m_pRsp->SetSession(pcReq->GetSession());
	//		return PROCMSG_OK;
	//	}
	//	break;
	default:
		{
			TASKLOG(NORMAL_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
				OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
			CMS_ASSERT(0 && "收到未知消息");
		}
		break;
	}
    return PROCMSG_OK;
}

u32 CQueryTask::WaitRspProcMsg( CMessage *const pcMsg )
{
    COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
    TASKLOG(NORMAL_TASK, EVENT_LEV, "CQueryTask[TaskNO %u]收到服务器[%s]的应答消息[%s--%hu]\n",
        GetTaskNO(), pcOspSipMsg->GetSipToUri().GetURIString().c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

    switch (pcMsg->event)
    {
    case OSP_SIP_MSG_PROC_FAIL:
        {
            TASKLOG(NORMAL_TASK, EVENT_LEV, "收到[OSP_SIP_MSG_PROC_FAIL]消息\n");
            CMS_ASSERT(NULL != m_ptRecvNode);
            m_pRsp->SetErrorCode(ERR_CUI_SIP_MSG_FAIL);
            m_strRspOfErr = m_pRsp->ToXml();
            SendRspToCu(m_strRspOfErr, m_ptRecvNode);
            NextState(NOT_QUERY_STATE);
            delete this;
        }
        break;
    case OSP_SIP_DIALOG_TERMINATE:
        {
            CMS_ASSERT( 0 && "CQueryTask::WaitRspProcMsg不可能出现会话内消息" );
        }
        break;
    case TAS_CUSER_QRY_RSP:
        {
            OnWaitRspMsg<CUserQryRsp, CCuQueryUserRsp>(pcMsg);
        }
        break;
    case TAS_CUSER_GROUP_QRY_RSP:
        {
            OnWaitRspMsg<CUserGroupQryRsp, CCuUserGroupQryRsp>(pcMsg);
        }
        break;
    case TAS_DEVICE_GROUP_QRY_RSP:
        {
            OnWaitRspMsg<CDeviceGroupQryRsp, CCuUserDevGroupQryRsp>(pcMsg);
        }
		break;
	case TAS_DEVTREE_QRY_RSP:
		{
			OnWaitRspMsg<CDevTreeQryRsp, CCuDevTreeQryRsp>(pcMsg);
		}
		break;
	case TAS_DEVICE_GROUPPATH_QRY_RSP:
		{
			OnWaitRspMsg<CDeviceGrpPathQryRsp, CCuDeviceGrpPathQryRsp>(pcMsg);
		}
		break;
    case TAS_CUSER_DEVICEFULLINFO_AND_PRIVILEGE_QRY_RSP:
        {
            OnWaitRspMsg<CCuserDevicefullinfoAndPrivilegeQryRsp, CCuUserDevFullInfoQryRsp>(pcMsg);
        }
        break;
    case TAS_DEVICE_GROUP_DEVICE_QRY_RSP:
        {
            OnWaitRspMsg<CDeviceGroupDeviceQryRsp, CCuQryDevGroupOfDevInfoRsp>(pcMsg);
        }
        break;
    case TAS_CUSER_DEVICE_QRY_RSP:
        {
            OnWaitRspMsg<CUserDeviceQryRsp, CCuGetUserDeviceRsp>(pcMsg);
        }
        break;
    case TAS_CUSER_GROUP_DEVICE_QRY_RSP:
        {
			OnWaitRspMsg<CUserGroupDeviceQryRsp, CCuGetUserGroupDeviceRsp>(pcMsg);
		}
		break;
	case TAS_DEVICE_GROUP_DEVICE_QRY_4GB_RSP:
		{
			OnWaitRspMsg<CDeviceGroupDeviceQry4GbRsp, CCuDeviceGroupDeviceQry4GbRsp>(pcMsg);
		}
		break;
	case TAS_DEVICE_AUTO_ASSIGN_TO_USER_QRY_RSP:
		{
			OnWaitRspMsg<CDeviceAutoAssignToUserQryRsp, CCuDeviceAutoAssignToUserQryRsp>(pcMsg);
		}
		break;
	case TAS_DEVICE_AUTO_ASSIGN_TO_USER_GROUP_QRY_RSP:
		{
			OnWaitRspMsg<CDeviceAutoAssignToUserGroupQryRsp, CCuDeviceAutoAssignToUserGroupQryRsp>(pcMsg);
		}
		break;
    case TAS_TEMP_AUTHORIZATION_QRY_RSP:
        {
            OnWaitRspMsg<CTempAuthorizationQryRsp, CCuGetAuthorizationDeviceRsp>(pcMsg);
        }
        break;
    case TAS_DEVICE_QRY_RSP:
        {
            OnWaitRspMsg<CDeviceQryRsp, CCuGetDeviceBaseInfoRsp>(pcMsg);
        }
        break;
	case TAS_VIDSRC_INFO_QRY_RSP:
		{
			OnWaitRspMsg<CVidsrcInfoQryRsp, CCuVidsrcInfoQryRsp>(pcMsg);
		}
		break;
    case TAS_TVWALL_QRY_RSP:
        {
            OnWaitRspMsg<CTvwallQryRsp, CCuTvwallQryRsp>(pcMsg);
        }
        break;
    case TAS_DOMAIN_RELATION_QRY_RSP:
        {
            OnWaitRspMsg<CDomainRelationQryRsp, CCuDomainRelationQryRsp>(pcMsg);
        }
        break;
	case TAS_DEVICE_GBINFO_QRY_RSP:
		{
			OnWaitRspMsg<CDeviceGbinfoQryRsp, CCuDeviceGbinfoQryRsp>(pcMsg);
		}
		break;
	case TAS_DEVGRP_GBINFO_QRY_RSP:
		{
			OnWaitRspMsg<CDevgrpGbinfoQryRsp, CCuDevgrpGbinfoQryRsp>(pcMsg);
		}
		break;
    case TAS_CUSER_QRY_NTF_REQ:
    case TAS_CUSER_GROUP_QRY_NTF_REQ:
    case TAS_DEVICE_GROUP_QRY_NTF_REQ:
    case TAS_CUSER_DEVICEFULLINFO_AND_PRIVILEGE_QRY_NTF_REQ:
	case TAS_DEVICE_GROUP_DEVICE_QRY_NTF_REQ:
	case TAS_DEVTREE_QRY_NTF_REQ:
	case TAS_DEVICE_GROUPPATH_QRY_NTF_REQ:
    case TAS_CUSER_DEVICE_QRY_NTF_REQ:
    case TAS_CUSER_GROUP_DEVICE_QRY_NTF_REQ:
	case TAS_DEVICE_GROUP_DEVICE_QRY_4GB_NTF_REQ:
    case TAS_TEMP_AUTHORIZATION_QRY_NTF_REQ:
    case TAS_DEVICE_QRY_NTF_REQ:
    case TAS_TVWALL_QRY_NTF_REQ:
    case TAS_DOMAIN_RELATION_QRY_NTF_REQ:
	case TAS_VIDSRC_INFO_QRY_NTF_REQ:
	case TAS_DEVICE_GBINFO_QRY_NTF_REQ:
	case TAS_DEVGRP_GBINFO_QRY_NTF_REQ:
    default:
        {
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"收到未知消息");
        }
        break;
    }
    return PROCMSG_OK;
}

u32 CQueryTask::WaitRspTimerPoll()
{
    if ( GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout() )
    {
        TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户[%s]会话[%s]等待应答消息[%s--%d]超时:SeqNum[%d]\n", 
            m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
            m_pRsp->GetEventStr().c_str(), m_pRsp->GetEventId(), m_pRsp->GetSeqNum());
        CMS_ASSERT(NULL != m_ptRecvNode);
        m_pRsp->SetErrorCode(ERR_CUI_OPR_TIME_OUT);
        m_strRspOfErr = m_pRsp->ToXml();
        SendRspToCu(m_strRspOfErr, m_ptRecvNode);
        NextState(NOT_QUERY_STATE);

		// 加上超时
        return TIMERPOLL_DEL;
    }
    return TIMERPOLL_DONE;
}

u32 CQueryTask::WaitNotifyProcMsg( CMessage *const pcMsg )
{
    while(m_VecNtfReqList.size() != 0)
    {
        CMessage cMsg;
        cMsg.event = m_dwReqEvent+2;
        string content = m_VecNtfReqList.front();
        m_VecNtfReqList.pop_front();
        cMsg.content = (u8 *)content.c_str();

        ProcNotifyMsg(&cMsg);
    }
    ProcNotifyMsg(pcMsg);

    return PROCMSG_OK;
}

u32 CQueryTask::ProcNotifyMsg(CMessage *const pcMsg)
{
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
            OnWaitNotifyMsg<CUserQryNtfReq, CNtfRsp, CCuQueryUserRsp>(pcMsg);
        }
        break;
    case TAS_CUSER_GROUP_QRY_NTF_REQ:
        {
            OnWaitNotifyMsg<CUserGroupQryNtfReq, CNtfRsp, CCuUserGroupQryRsp>(pcMsg);
        }
        break;
    case TAS_DEVICE_GROUP_QRY_NTF_REQ:
        {
            OnWaitNotifyMsg<CDeviceGroupQryNtfReq, CNtfRsp, CCuUserDevGroupQryRsp>(pcMsg);
        }
        break;
    case TAS_CUSER_DEVICEFULLINFO_AND_PRIVILEGE_QRY_NTF_REQ:
        {
            COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
            CCuserDevicefullinfoAndPrivilegeQryNtfReq cNtf;
            pcOspSipMsg->GetMsgBody(cNtf);

            TASKLOG(NORMAL_TASK, EVENT_LEV, "收到Ntf消息[%s--%d]，本次Ntf中含带[%d]条记录\n",
                cNtf.GetEventStr().c_str(), cNtf.GetEventId(),
                cNtf.GetEntryNum());

            //先回复NTF通知
            CNtfRsp cRsp;
            cRsp.SetEvent(pcMsg->event+1);
            cRsp.SetSeqNum(cNtf.GetSeqNum());
            cRsp.SetSession(cNtf.GetSession());
            PostMsgRsp(pcOspSipMsg->GetSipTransID(), cRsp);

            CCuUserDevFullInfoQryRsp *pcRspToCu = (CCuUserDevFullInfoQryRsp *)m_pRsp;
			pcRspToCu->SetQueryHint(cNtf.GetQueryHint());
            pcRspToCu->push_back(cNtf, this);

            //判断是否后续还有Ntf
            if ( cNtf.IsLastNtf() )
            {
                SendRspToCu<CCuUserDevFullInfoQryRsp>(*pcRspToCu, m_ptRecvNode);
                g_cCuiDataMgr.m_mapSessToTaskMap.Erase(m_strQueryTag);
                m_strQueryTag.clear();
                NextState(NOT_QUERY_STATE);
                delete this;
                return PROCMSG_DEL;
            }
            else
            {
                NextState(WAIT_NOTIFY_STATE);  //切换状态以清空轮询计数，继续超时判断
            }
        }
        break;
    case TAS_DEVICE_GROUP_DEVICE_QRY_NTF_REQ:
        {
            OnWaitNotifyMsg<CDeviceGroupDeviceQryNtfReq, CNtfRsp, CCuQryDevGroupOfDevInfoRsp>(pcMsg);
        }
        break;
    case TAS_CUSER_DEVICE_QRY_NTF_REQ:
        {
            OnWaitNotifyMsg<CUserDeviceQryNtfReq, CNtfRsp, CCuGetUserDeviceRsp>(pcMsg);
        }
        break;
    case TAS_CUSER_GROUP_DEVICE_QRY_NTF_REQ:
        {
            OnWaitNotifyMsg<CUserGroupDeviceQryNtfReq, CNtfRsp, CCuGetUserGroupDeviceRsp>(pcMsg);
        }
		break;
	case TAS_DEVICE_GROUP_DEVICE_QRY_4GB_NTF_REQ:
		{
			OnWaitNotifyMsg<CDeviceGroupDeviceQry4GbNtfReq, CNtfRsp, CCuDeviceGroupDeviceQry4GbRsp>(pcMsg);
		}
		break;
    case TAS_TEMP_AUTHORIZATION_QRY_NTF_REQ:
        {
            OnWaitNotifyMsg<CTempAuthorizationQryNtfReq, CNtfRsp, CCuGetAuthorizationDeviceRsp>(pcMsg);
        }
        break;
    case TAS_DEVICE_QRY_NTF_REQ:
        {
            OnWaitNotifyMsg<CDeviceQryNtfReq, CNtfRsp, CCuGetDeviceBaseInfoRsp>(pcMsg);
        }
        break;
    case TAS_TVWALL_QRY_NTF_REQ:
        {
            OnWaitNotifyMsg<CTvwallQryNtfReq, CNtfRsp, CCuTvwallQryRsp>(pcMsg);
        }
        break;
    case TAS_DOMAIN_RELATION_QRY_NTF_REQ:
        {
            OnWaitNotifyMsg<CDomainRelationQryNtfReq, CNtfRsp,CCuDomainRelationQryRsp>(pcMsg);
        }
        break;
	case TAS_DEVICE_AUTO_ASSIGN_TO_USER_QRY_NTF_REQ:
		{
			OnWaitNotifyMsg<CDeviceAutoAssignToUserQryNtfReq, CNtfRsp, CCuDeviceAutoAssignToUserQryRsp>(pcMsg);
		}
		break;
	case TAS_DEVICE_AUTO_ASSIGN_TO_USER_GROUP_QRY_NTF_REQ:
		{
			OnWaitNotifyMsg<CDeviceAutoAssignToUserGroupQryNtfReq, CNtfRsp, CCuDeviceAutoAssignToUserGroupQryRsp>(pcMsg);
		}
		break;
	case TAS_DEVTREE_QRY_NTF_REQ:
		{
			OnWaitNotifyMsg<CDevTreeQryNtfReq, CNtfRsp, CCuDevTreeQryRsp>(pcMsg);
		}
		break;
	case TAS_DEVICE_GROUPPATH_QRY_NTF_REQ:
		{
			OnWaitNotifyMsg<CDeviceGrpPathQryNtfReq, CNtfRsp, CCuDeviceGrpPathQryRsp>(pcMsg);
		}
		break;
	case TAS_VIDSRC_INFO_QRY_NTF_REQ:
		{
			OnWaitNotifyMsg<CVidsrcInfoQryNtfReq, CNtfRsp, CCuVidsrcInfoQryRsp>(pcMsg);
		}
		break;
	case TAS_DEVICE_GBINFO_QRY_NTF_REQ:
		{
			OnWaitNotifyMsg<CDeviceGbinfoQryNtfReq, CNtfRsp, CCuDeviceGbinfoQryRsp>(pcMsg);
		}
		break;
	case TAS_DEVGRP_GBINFO_QRY_NTF_REQ:
		{
			OnWaitNotifyMsg<CDevgrpGbinfoQryNtfReq, CNtfRsp, CCuDevgrpGbinfoQryRsp>(pcMsg);
		}
		break;
    default:
        {
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"收到未知消息");

            //ProcServerAbnormalMsg(pcMsg);
        }
        break;
    }
    return PROCMSG_OK;
}

u32 CQueryTask::WaitNotifyTimerPoll()
{
    if ( GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout() )
    {
        TASKLOG(NORMAL_TASK, ERROR_LEV, "CQueryTask[NO: %u]等待NTF消息超时SeqNum[%d]，直接向CU回复应答消息[%s--%d]\n",
            GetTaskNO(), m_pRsp->GetSeqNum(), m_pRsp->GetEventStr().c_str(), m_pRsp->GetEventId());
        CMS_ASSERT(NULL != m_ptRecvNode);
        m_pRsp->SetErrorCode(ERR_CUI_OPR_TIME_OUT);
        m_strRspOfErr = m_pRsp->ToXml();
        SendRspToCu(m_strRspOfErr, m_ptRecvNode);
        g_cCuiDataMgr.m_mapSessToTaskMap.Erase(m_strQueryTag);
        m_strQueryTag.clear();
        NextState(NOT_QUERY_STATE);
        return TIMERPOLL_DEL;
    }
    return TIMERPOLL_DONE;
}

bool CCuAgent::CuiTransStream4PlatRec() const
{
	/*
	按以下顺序判断是否走cui转发：
	1.客户端不支持平台录像回放/下载走cui转发，则不走cui转发。
	2.如果配置了cui转发，则必走cui转发。
	3.局域网就不走cui转发，非局域网就走cui转发。
	4.不再考虑端口映射问题和NAT探测包规则问题。
	*/
	if (! m_bClientSupCuiTransPlatRec)
	{
		// 客户端如果不支持，就不用判断了
		return false;
	}

	if (g_cCuiConfig.IsNeedCUISwitch())
	{
		return true;
	}

	//局域网不走cui转发，其他都走cui转发
	if (NATModeOutside == m_eClientNat && NATModeOutside == m_ePlatNat)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool CCuAgent::CuiTransStream4Pu() const
{
	/*
	按以下顺序判断是否走cui转发：
	1.如果配置了cui转发，则必走cui转发。
	2.客户端和平台位置关系决定是否走cui转发：
	  a.局域网不走cui转发；
	  b.客户NAT外，平台NAT内，vtdu半源半端口收，伪装cui端口发，不走cui转发；
	  c.客户端NAT内，为防止客户端对称型NAT，走cui转发。
	3.不再考虑端口映射问题和NAT探测包规则问题。
	*/

	if (g_cCuiConfig.IsNeedCUISwitch())
	{
		return true;
	}

	//局域网不走cui转发
	if (NATModeOutside == m_eClientNat && NATModeOutside == m_ePlatNat)
	{
		return false;
	}
	//客户NAT外，平台NAT内，vtdu半源半端口收，伪装cui端口发，不走cui转发
	else if (NATModeOutside == m_eClientNat && NATModeInner == m_ePlatNat)
	{
		return false;
	}
	//客户端NAT内，为防止客户端对称型NAT，走cui转发
	else //NATModeInner == m_eClientNat
	{
		return true;
	}
}

void CCuAgent::ClearAllSubTask()
{
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
                CInviteTask *pcInviteTask = (CInviteTask *)( GetInstance()->FindTask(dwTaskId) );
                if ( NULL != pcInviteTask )
                {
                    pcInviteTask->StartSendInviteBye();
                    delete pcInviteTask;
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

    if ( ! m_cSubTaskSet.IsEmpty() )
    {
        CTaskNOToSubTaskPtrMap tmpSubTaskSet = m_cSubTaskSet;
        Iterator pPos;
        while ( ! pPos.End() )
        {
            u32 dwTaskId = INVALID_TASKNO;
            PCSipTask pcSubTaskPtr = NULL;
            if ( tmpSubTaskSet.Iterate(pPos, dwTaskId, pcSubTaskPtr) )
            {
                CMS_ASSERT(INVALID_TASKNO != dwTaskId);
                PCSipTask pcSubTaskInTaskMgr = (PCSipTask)( GetInstance()->FindTask(dwTaskId) );
                if ( pcSubTaskPtr != pcSubTaskInTaskMgr )
                {
                    TASKLOG(LOGIN_TASK, CRITICAL_LEV, "CCuAgent中保存的子任务TaskNO: %u对应的Task指针不一致\n",
                        dwTaskId);
                }
                if ( NULL != pcSubTaskInTaskMgr )
                {
                    delete pcSubTaskInTaskMgr;
                }
            }
        }

        m_cSubTaskSet.Empty();
    }

    do 
    {
        set<CModuleListSubscribeTaskBase*>::iterator it = m_setModuleListSubTask.begin();
        if (it == m_setModuleListSubTask.end())
        {
            break;
        }
        
        (*it)->EraseUser(this);
        m_setModuleListSubTask.erase(it);
    } while (true);

    if ( !  m_cDevSubTaskSet.IsEmpty() )
    {
        CDevUriTypeToSubTaskMap tmpDevSubTaskSet = m_cDevSubTaskSet;
        Iterator pPos;
        while ( ! pPos.End() )
        {
            string strDevURI;
            u32 dwTaskId = INVALID_TASKNO;
            if ( tmpDevSubTaskSet.Iterate(pPos, strDevURI, dwTaskId) )
            {
                CSubBaseTask *pcSubTask = (CSubBaseTask *)(GetInstance()->FindTask(dwTaskId));
                if ( NULL != pcSubTask )
                {
                    pcSubTask->EraseCuSession(m_strSession);
                }
                else
                {
                    TASKLOG(LOGIN_TASK, CRITICAL_LEV, "找不到CCuAgent的CSubBaseTask[NO: %u]的子任务\n",
                        dwTaskId);
                    CMS_ASSERT(0 && "找不到CCuAgent的CSubBaseTask子任务");
                }
            }
        }

        m_cDevSubTaskSet.Empty();    //注意，CSubscribeTask的TerminateTask函数中也会删除，重复删没有问题
    }

    if( ! m_cPeInviteTaskSet.IsEmpty())
    {
        CPlayIdToInviteTaskMap tmpPeInviteTaskSet = m_cPeInviteTaskSet;
        Iterator pPos;
        while ( ! pPos.End() )
        {
            s32 nPlayId = INVALID_PLAY_ID;
            u32 dwTaskId = INVALID_TASKNO;
            if ( tmpPeInviteTaskSet.Iterate(pPos, nPlayId, dwTaskId) )
			{
				CSipTask *pcTask = (CSipTask *)( GetInstance()->FindTask(dwTaskId) );
				if ( NULL == pcTask )
				{
					TASKLOG(LOGIN_TASK, CRITICAL_LEV, "找不到CCuAgent的CPeInviteTask[NO: %u]的子任务\n",
						dwTaskId);
				}
				else
                {
					if ( this->CuiTransStream4PlatRec() )
					{
						CPeInviteTaskNatSwitch *pcInviteTask = (CPeInviteTaskNatSwitch *)pcTask;
						pcInviteTask->StartSendInviteBye();
						//由CInviteTask自己删除自己，并且注销CuAgent中的登记
						// 2012.3.13 不能等InviteByeRsp再删除，因为此时CuAgent已经销毁
					}
					else
					{
						CPeInviteTask *pcInviteTask = (CPeInviteTask *)pcTask;
						pcInviteTask->StartSendInviteBye();
						//由CInviteTask自己删除自己，并且注销CuAgent中的登记
						// 2012.3.13 不能等InviteByeRsp再删除，因为此时CuAgent已经销毁
					}
                }
            }
        }

        m_cInviteTaskSet.Empty(); 
    }

    if ( ! m_cCallPuTaskSet.IsEmpty() )
    {
        CPlayIdToCallPuTaskMap tmpCallPuTaskSet = m_cCallPuTaskSet;
        Iterator pPos;
        while ( ! pPos.End() )
        {
            s32 nPlayId = INVALID_PLAY_ID;
            u32 dwTaskId = INVALID_TASKNO;
            if ( tmpCallPuTaskSet.Iterate(pPos, nPlayId, dwTaskId) )
            {
                CCallPuTask *pcCallPuTask = (CCallPuTask *)( GetInstance()->FindTask(dwTaskId) );
                if ( NULL != pcCallPuTask )
                {
                    delete pcCallPuTask;
                    pcCallPuTask = NULL;
                    //由CInviteTask自己删除自己，并且注销CuAgent中的登记
                    // 2012.3.13 不能等InviteByeRsp再删除，因为此时CuAgent已经销毁
                }
                else
                {
                    TASKLOG(LOGIN_TASK, CRITICAL_LEV, "找不到CCuAgent的CCallPuTask[NO: %u]的子任务\n",
                        dwTaskId);
                }
            }
        }
        m_cCallPuTaskSet.Empty();
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
                CTask *pcTask = (CTask *)( GetInstance()->FindTask(dwTaskId) );
                if ( NULL != pcTask )
                {
                    delete pcTask;
                    pcTask = NULL;
                }
                else
                {
                    TASKLOG(LOGIN_TASK, CRITICAL_LEV, "找不到CCuAgent的CExportTask[NO: %u]的子任务\n",
                        dwTaskId);
                }
            }
        }

        m_cCallPuTaskSet.Empty();
    }

	set<u32> tSetDpsSearchTaskNo = m_setDpsSearchTaskNo;
	if ( !tSetDpsSearchTaskNo.empty() )
	{
		// 这个地方要复制一份，否则 task销毁时会删除原有set，导致崩溃。
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


//////////////////////////////////////////////////////////////////////////
CNotifyTransferTask::CNotifyTransferTask( CInstExt *pcInst, CCuAgent *pcCuAgent, PTRecvNode ptRecvNode ) : CSipTask(pcInst)
{
	CMS_ASSERT(pcCuAgent != NULL);
	CMS_ASSERT(ptRecvNode != NULL);
	m_pcCuAgent = pcCuAgent;
	m_ptRecvNode = ptRecvNode;
	m_pRsp = NULL;
	m_pc3acAgent = NULL;
	m_dwReqEvent = 0;
	m_pcCuAgent->AddSubTask(this);
}

CNotifyTransferTask::~CNotifyTransferTask()
{
	TerminateTask();
	if ( ! m_strQueryTag.empty() )
	{
		g_cCuiDataMgr.m_mapSessToTaskMap.Erase(m_strQueryTag);
		m_strQueryTag.clear();
	}

	if ( NULL != m_pRsp )
	{
		delete m_pRsp;
		m_pRsp = NULL;
	}
}

void CNotifyTransferTask::TerminateTask()
{
	m_pcCuAgent->DelSubTask(this);
	while(!m_VecNtfReqList.empty())
	{ 
		string strNtfMsg = m_VecNtfReqList.front();
		m_VecNtfReqList.pop_front();
		COspSipMsg *pcOspSipMsg = (COspSipMsg *)strNtfMsg.c_str();
		TASKLOG(NORMAL_TASK, EVENT_LEV, "有未处理的NTF消息[%s],直接回复\n", OspExtEventDesc(pcOspSipMsg->GetOspEventID()).c_str());
		CNtfRsp cRsp;
		cRsp.SetEvent(pcOspSipMsg->GetOspEventID() + 1);
		string strSession = GetSipSessionID(pcOspSipMsg->GetMsgBody());
		cRsp.SetSession(strSession);
		string strSeqNum = GetSipSeqNO(pcOspSipMsg->GetMsgBody());
		int nSeqNum = static_cast<int>(atol(strSeqNum.c_str()));
		cRsp.SetSeqNum(nSeqNum);
		PostMsgRsp(pcOspSipMsg->GetSipTransID(), cRsp);
	}
}

void CNotifyTransferTask::InitStateMachine()
{
	CStateProc cNotQueryProc;
	cNotQueryProc.ProcMsg = (CSipTask::PFProcMsg)&CNotifyTransferTask::NotQueryProcMsg;
	cNotQueryProc.TimerPoll = &CSipTask::TimerPollSkip;
	cNotQueryProc.ProcErrMsg = (CSipTask::PFProcMsg)&CNotifyTransferTask::NotQueryProcMsg;
	AddRuleProc(NOT_QUERY_STATE, cNotQueryProc);

	CStateProc cWaitRspProc;
	cWaitRspProc.ProcMsg = (CSipTask::PFProcMsg)&CNotifyTransferTask::WaitRspProcMsg;
	cWaitRspProc.TimerPoll = (CSipTask::PFTimerPoll)&CNotifyTransferTask::WaitRspTimerPoll;
	cWaitRspProc.ProcErrMsg = (CSipTask::PFProcMsg)&CNotifyTransferTask::WaitRspProcMsg;
	AddRuleProc(WAIT_RESPONSE_STATE, cWaitRspProc);

	CStateProc cWaitNotifyProc;
	cWaitNotifyProc.ProcMsg = (CSipTask::PFProcMsg)&CNotifyTransferTask::WaitNotifyProcMsg;
	cWaitNotifyProc.TimerPoll = (CSipTask::PFTimerPoll)&CNotifyTransferTask::WaitNotifyTimerPoll;
	cWaitNotifyProc.ProcErrMsg = (CSipTask::PFProcMsg)&CNotifyTransferTask::WaitNotifyProcMsg;
	AddRuleProc(WAIT_NOTIFY_STATE, cWaitNotifyProc);

	NextState(NOT_QUERY_STATE);
}

u32 CNotifyTransferTask::NotQueryProcMsg( CMessage *const pcMsg )
{
	u32 dwRet = PROCMSG_OK;
	TASKLOG(NORMAL_TASK, EVENT_LEV, "[CNotifyTransferTask]收到CU用户[%s]会话[%s]的请求[%s--%hu]\n",
		m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

	switch (pcMsg->event)
	{
	case PU_TRANSPARENT_QRY_REQ:
		{
			CCuPuTransparentQryReq *pcReq = (CCuPuTransparentQryReq *)pcMsg->content;
			OnNotQueryMsg<CCuPuTransparentQryReq, CCuPuTransparentQryRsp>(*pcReq,SERVER_CMU);
		}
		break;
	default:
		{
			TASKLOG(NORMAL_TASK, CRITICAL_LEV, "[CNotifyTransferTask]收到未知消息[%s--%hu]\n",
				OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
			CMS_ASSERT(0&&"收到未知消息");
			dwRet = PROCMSG_DEL;
		}
		break;
	}
	return dwRet;
}

u32 CNotifyTransferTask::WaitRspProcMsg( CMessage *const pcMsg )
{
	COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
	TASKLOG(NORMAL_TASK, EVENT_LEV, "CNotifyTransferTask[TaskNO %u]收到服务器[%s]的应答消息[%s--%hu]\n",
		GetTaskNO(), pcOspSipMsg->GetSipToUri().GetURIString().c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

	switch (pcMsg->event)
	{
	case OSP_SIP_MSG_PROC_FAIL:
		{
			TASKLOG(NORMAL_TASK, EVENT_LEV, "收到[OSP_SIP_MSG_PROC_FAIL]消息\n");
			CMS_ASSERT(NULL != m_ptRecvNode);
			m_pRsp->SetErrorCode(ERR_CUI_SIP_MSG_FAIL);
			m_strRspOfErr = m_pRsp->ToXml();
			SendRspToCu(m_strRspOfErr, m_ptRecvNode);
			NextState(NOT_QUERY_STATE);
			delete this;
		}
		break;
	case PU_TRANSPARENT_QRY_RSP:
		{
			OnWaitRspMsg<CPuTransparentQryRsp,CCuPuTransparentQryRsp>(pcMsg);
		}
		break;
	case PU_TRANSPARENT_QRY_NTF_REQ:
		{
			if((m_dwReqEvent+2) == pcMsg->event)
			{
				if(pcMsg->content != NULL)
				{
					string strContect((char *)pcMsg->content, pcMsg->length);
					m_VecNtfReqList.push_back(strContect);
					TASKLOG(NORMAL_TASK, CRITICAL_LEV, "在[Wait Rsp State]收到相应的Ntf消息[%s--%hu]\n",
						OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
				}              
			}
			else
			{
				TASKLOG(NORMAL_TASK, CRITICAL_LEV, "在[Wait Rsp State]收到不对应的Ntf消息[%s--%hu]\n",
					OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
			}
		}
		break;
	default:
		{
			TASKLOG(NORMAL_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
				OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
			CMS_ASSERT(0&&"收到未知消息");
		}
		break;
	}
	return PROCMSG_OK;
}

u32 CNotifyTransferTask::WaitRspTimerPoll()
{
	if ( GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout() )
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "[CNotifyTransferTask]CU用户[%s]会话[%s]等待应答消息[%s--%d]超时:SeqNum[%d]\n", 
			m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
			m_pRsp->GetEventStr().c_str(), m_pRsp->GetEventId(), m_pRsp->GetSeqNum());
		CMS_ASSERT(NULL != m_ptRecvNode);
		m_pRsp->SetErrorCode(ERR_CUI_OPR_TIME_OUT);
		m_strRspOfErr = m_pRsp->ToXml();
		SendRspToCu(m_strRspOfErr, m_ptRecvNode);
		NextState(NOT_QUERY_STATE);
		return TIMERPOLL_DEL;
	}
	return TIMERPOLL_DONE;
}

u32 CNotifyTransferTask::WaitNotifyProcMsg( CMessage *const pcMsg )
{
	while(m_VecNtfReqList.size() != 0)
	{
		CMessage cMsg;
		cMsg.event = m_dwReqEvent+2;
		string content = m_VecNtfReqList.front();
		m_VecNtfReqList.pop_front();
		cMsg.content = (u8 *)content.c_str();

		ProcNotifyMsg(&cMsg);
	}
	ProcNotifyMsg(pcMsg);

	return PROCMSG_OK;
}

u32 CNotifyTransferTask::ProcNotifyMsg(CMessage *const pcMsg)
{
	switch (pcMsg->event)
	{
	case OSP_SIP_MSG_PROC_FAIL:
	case OSP_SIP_DIALOG_TERMINATE:
		{
			CMS_ASSERT( 0 && "CNotifyTransferTask::WaitNotifyProcMsg不可能出现SIP异常" );
		}
		break;
	case PU_TRANSPARENT_QRY_NTF_REQ:
		{
			OnWaitNotifyMsg<CPuTransparentQryNtfReq,CNtfRsp>(pcMsg);
		}
		break;
	default:
		{
			TASKLOG(NORMAL_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
				OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
			CMS_ASSERT(0&&"收到未知消息");
		}
		break;
	}
	return PROCMSG_OK;
}

u32 CNotifyTransferTask::WaitNotifyTimerPoll()
{
	if ( GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout() )
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "CNotifyTransferTask[NO: %u]等待NTF消息超时SeqNum[%d]，直接向CU回复应答消息[%s--%d]\n",
			GetTaskNO(), m_pRsp->GetSeqNum(), m_pRsp->GetEventStr().c_str(), m_pRsp->GetEventId());
		CMS_ASSERT(NULL != m_ptRecvNode);
		m_pRsp->SetErrorCode(ERR_CUI_OPR_TIME_OUT);
		m_strRspOfErr = m_pRsp->ToXml();
		//SendRspToCu(m_strRspOfErr, m_ptRecvNode);
		g_cCuiDataMgr.m_mapSessToTaskMap.Erase(m_strQueryTag);
		m_strQueryTag.clear();
		NextState(NOT_QUERY_STATE);
		return TIMERPOLL_DEL;
	}
	return TIMERPOLL_DONE;
}

//////////////////////////////////////////////////////////////////////////
CSubBaseTask::CSubBaseTask(CInstExt *pcInst, const string& strUri, const string& strSvrUri, const string& strSubType) : CSipTask(pcInst)
{
    m_strDevUri = strUri;
    m_strSubType = strSubType;
    m_tSvrUri.SetURIString(strSvrUri);
    m_tSubscribeID = INVALID_DIALOG_ID;
    m_tCuSessionList.clear();
}

CSubBaseTask::~CSubBaseTask()
{
    if( m_tSubscribeID != INVALID_DIALOG_ID)
    {
        //释放会话
        OspSipReleaseDlg(m_tSubscribeID);
        m_tSubscribeID = INVALID_DIALOG_ID;
    }
}

string CSubBaseTask::GetKey()
{
    return (m_strDevUri + "+" + m_strSubType);
}


bool CSubBaseTask::IsOverFlowLimit4Subscribe()
{
    if (NULL != s_cFlowLimitCounter4Subscribe)
    {
        return !s_cFlowLimitCounter4Subscribe->Decrease();
    }
    else
    {
        s_cFlowLimitCounter4Subscribe = new CFlowLimitCounter(OspClkRateGet(), g_cCuiConfig.GetSubscribeFlowLimit());
        return true;
    }
}

bool CSubBaseTask::IsOverFlowLimit4Unsubscribe()
{
    if (NULL != s_cFlowLimitCounter4Unsubscribe)
    {
        return !s_cFlowLimitCounter4Unsubscribe->Decrease();
    }
    else
    {
        s_cFlowLimitCounter4Unsubscribe = new CFlowLimitCounter(OspClkRateGet(), g_cCuiConfig.GetUnsubscribeFlowLimit());
        return true;
    }
}

void CSubBaseTask::PrintData()
{
    OspPrintf(TRUE, FALSE, "DevUri:%s, SubType:%s, SvrUri:%s, CuSubscribeNum:%d\n", 
        m_strDevUri.c_str(), m_strSubType.c_str(), m_tSvrUri.GetURIString().c_str(),
        (s32)m_tCuSessionList.size());
}

void CSubBaseTask::EraseCuSession( const string &strCuSess )
{
    m_tCuSessionList.erase(strCuSess);
}

CSubBatchTask::CSubBatchTask( CInstExt *pcInst, CCuAgent *pcCuAgent, PTRecvNode ptRecvNode) : CSipTask(pcInst)
{
    CMS_ASSERT(pcCuAgent != NULL);
    CMS_ASSERT(ptRecvNode != NULL);
    m_pcCuAgent = pcCuAgent;
    m_ptRecvNode = ptRecvNode;
}

CSubBatchTask::~CSubBatchTask()
{
    TerminateTask();
}

void CSubBatchTask::TerminateTask()
{

}

void CSubBatchTask::InitStateMachine()
{
    CStateProc cNotRequestProc;
    cNotRequestProc.ProcMsg = (CSipTask::PFProcMsg)&CSubBatchTask::NotReqProcMsg;
    cNotRequestProc.TimerPoll = &CSipTask::TimerPollSkip;
    cNotRequestProc.ProcErrMsg = (CSipTask::PFProcMsg)&CSubBatchTask::NotReqProcMsg;
    AddRuleProc(NOT_REQUEST_STATE, cNotRequestProc);
    // 
    //      CStateProc cWaitRspProc;
    //      cWaitRspProc.ProcMsg = (CSipTask::PFProcMsg)&CSubBatchTask::WaitRspProcMsg;
    //      cWaitRspProc.TimerPoll = (CSipTask::PFTimerPoll)&CSubBatchTask::WaitRspTimerPoll;
    //      cWaitRspProc.ProcErrMsg = (CSipTask::PFProcMsg)&CSubBatchTask::WaitRspProcMsg;
    //      AddRuleProc(WAIT_RESPONSE_STATE, cWaitRspProc);

    NextState(NOT_REQUEST_STATE);
}

u32 CSubBatchTask::NotReqProcMsg(CMessage *const pcMsg)
{
    TASKLOG(NORMAL_TASK, EVENT_LEV, "收到CU用户[%s]会话[%s]的批处理请求[%s--%hu]\n",
        m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

    switch (pcMsg->event)
    {
    case CU_CUI_SUBSCRIBE_PUSTATUS_REQ:
        {
            CCuSubscribeNotifyReq *pcReq = (CCuSubscribeNotifyReq *)pcMsg->content;
            const vector<CNotifySubscription> &tmpVct = pcReq->GetSubscribeList();

            CCuSubscribeNotifyRsp cRsp;
            cRsp.SetSeqNum(pcReq->GetSeqNum());
            cRsp.SetSession(pcReq->GetSession());
            cRsp.SetErrorCode(CMS_SUCCESS);

            TSipURI tUri;
            tUri.SetDomain(GetDomainName());
            string strCmuUri = tUri.GetCmuURI().GetURIString();
            string strMpsUri = tUri.GetMpsURI().GetURIString();
            string strRcsUri = tUri.GetRcsURI().GetURIString();

            vector<CNotifySubscription>::const_iterator pSubItem = tmpVct.begin();
            while ( pSubItem != tmpVct.end() )
            {
                //1 鉴权
                bool bAuthRet = true; //鉴权结果
                do 
                {
                    if (SubscribeSeviceStatus == pSubItem->GetSubscribeNotifyType())
                    {
                        break;
                    }

                    if ( pSubItem->GetDeviceURI().empty())
                    {
                        TASKLOG(NORMAL_TASK, ERROR_LEV, "设备URI为空\n");
                        bAuthRet = false;
                        break;
                    }

					// 查看域是否存在
                    string strDomainName = GetDomainNameWithoutPreNameFromURI(pSubItem->GetDeviceURI());
                    if ( ! m_pcCuAgent->IsExistDomainInfo(strDomainName))
                    {
                        bAuthRet = false;
                        break;
                    }

					if (m_pcCuAgent->m_pcUserAccount->IsSuperAdmin())
                    {
                        bAuthRet = true;
                        break;
                    }

                    string uuid = GetNameWithoutDomainFromURI(pSubItem->GetDeviceURI());
					if ( !g_cRedisDb.IsUserHasDev(m_pcCuAgent->m_pcUserAccount->m_tUserID, uuid) )
					{
						TASKLOG(NORMAL_TASK, ERROR_LEV, "redis中 用户[%s - %s]没有划归设备[%s]\n",
							m_pcCuAgent->m_strUserName.c_str(),
							m_pcCuAgent->m_pcUserAccount->m_tUserID.c_str(),
							pSubItem->GetDeviceURI().c_str());
						bAuthRet = false;
						break;
					}
                } while (0);

                if (!bAuthRet)
                {
                    TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户[%s]会话[%s]订阅设备[%s]的状态，但是无此设备权限\n",
                        m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
                        pSubItem->GetDeviceURI().c_str());
                    //将无权限的订单放到CCuSubscribePuStatusRsp的失败列表中
                    cRsp.GetFailedSubscribeList().push_back(*pSubItem);

                    //更新应答错误码
                    cRsp.SetErrorCode(ERR_CUI_CU_NO_PERMISSION);

                    pSubItem++;
                    continue;  //下面的步骤跳过
                }

                if(SubscribeTVWallStatus == pSubItem->GetSubscribeNotifyType())
                {
                    string strDstUri = string(DEV_TYPE_TVS) + "@" + GetDomainName();
                    CPeAgent *pcPeAgent = (CPeAgent *)g_cCuiDataMgr.GetPeAgentPtrByPeName(strDstUri, GetInstance());

                    CMS_ASSERT(NULL != pcPeAgent);
                    if(NULL != pcPeAgent)
                    {
                        CTvsSubTask *pcSubTask = (CTvsSubTask *)pcPeAgent->GetSubTaskPtrByDevURI(pSubItem->GetDeviceURI() + "+" + TVWALL_ALL_STATUS);
                        if(NULL == pcSubTask || CTvsSubTask::UnSub == pcSubTask->GetState())
                        {
                            pcSubTask = new CTvsSubTask(GetInstance(), pSubItem->GetDeviceURI(), strDstUri, 
                                TVWALL_ALL_STATUS, pcPeAgent->m_cSubscribeTaskSet);
                            pcSubTask->InitStateMachine();
                            pcSubTask->StartSub();
                        }
                        pcSubTask->FlashCuInfo(m_pcCuAgent);
                    } 
                }
                else if(SubscribePuStatus == pSubItem->GetSubscribeNotifyType())
                {
                    CNotifySubscription cFailedSubscribe = *pSubItem;
                    cFailedSubscribe.GetSubscribeType().clear();
                    s32 nSubscribeRes;

                    set<int>::const_iterator iter = pSubItem->GetSubscribeType().begin();
                    while ( iter != pSubItem->GetSubscribeType().end() )
                    {
                        switch(*iter)
                        {
							// redis订阅
                        case SubscribeOnlineStatus:  
                            {
                                nSubscribeRes = ModuleListSubscribe(strCmuUri, pSubItem->GetDeviceURI(), 
                                    PU_ON_OFF_STATUS, m_pcCuAgent);
                                if (CMS_SUCCESS != nSubscribeRes)
                                {
                                    cFailedSubscribe.GetSubscribeType().insert(*iter);
                                }
                            }
                            break;
							// redis订阅
                        case SubscribeConfigStatus:
                            {
                                nSubscribeRes = ModuleListSubscribe(strCmuUri, pSubItem->GetDeviceURI(), 
                                    PU_CONFIG_STATUS, m_pcCuAgent);
                                if (CMS_SUCCESS != nSubscribeRes)
                                {
                                    cFailedSubscribe.GetSubscribeType().insert(*iter);
                                }
                            }
                            break;
							// redis订阅
                        case SubscribeServiceStatus:
                            {
                                nSubscribeRes = ModuleListSubscribe(strCmuUri, pSubItem->GetDeviceURI(), 
                                    PU_SERVICE_STATUS, m_pcCuAgent);
                                if (CMS_SUCCESS != nSubscribeRes)
                                {
                                    cFailedSubscribe.GetSubscribeType().insert(*iter);
                                    break;
                                }

                                // hzytodo pu的服务状态为什么还自动订阅录像状态, 这块后续要求CU那边的接口针对录像状态专门定义
                                nSubscribeRes = ModuleListSubscribe(strRcsUri, pSubItem->GetDeviceURI(), 
                                    SS_TYPE_PU_PLATFORM_RECORD_STATUS, m_pcCuAgent);
                                if (CMS_SUCCESS != nSubscribeRes)
                                {
                                    cFailedSubscribe.GetSubscribeType().insert(*iter);
                                }
                            }
                            break;
							// redis订阅
                        case SubscribeAlarmStatus:
                            {
                                nSubscribeRes = ModuleListSubscribe(strCmuUri, pSubItem->GetDeviceURI(), 
                                    PU_ALARM_STATUS, m_pcCuAgent);
                                if (CMS_SUCCESS != nSubscribeRes)
                                {
                                    cFailedSubscribe.GetSubscribeType().insert(*iter);
                                }
                            }
                            break;
							// redis订阅
                        case SubscribeGPS:
                            {
                                nSubscribeRes = ModuleListSubscribe(strMpsUri, pSubItem->GetDeviceURI(), 
                                    PU_GPS_STATUS, m_pcCuAgent);
                                if (CMS_SUCCESS != nSubscribeRes)
                                {
                                    cFailedSubscribe.GetSubscribeType().insert(*iter);
                                }
                            }
                            break;
							// redis订阅
                        case SubscribeTransdata:
                            {
                                nSubscribeRes = ModuleListSubscribe(strCmuUri, pSubItem->GetDeviceURI(), 
                                    PU_TRANSDATA_STATUS, m_pcCuAgent);
                                if (CMS_SUCCESS != nSubscribeRes)
                                {
                                    cFailedSubscribe.GetSubscribeType().insert(*iter);
                                }
                            }
                            break;
                        default:
                            {
                                TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户[%s]会话[%s]订阅不认识的类型[%d]\n",
                                    m_pcCuAgent->m_strUserName.c_str(), 
                                    m_pcCuAgent->m_strSession.c_str(),*iter);
                            }
                            break;
                        }

                        if (nSubscribeRes != CMS_SUCCESS)
                        {
                            cRsp.SetErrorCode(nSubscribeRes);
                        }

                        iter++;
                    }

                    if (!cFailedSubscribe.GetSubscribeType().empty())
                    {
                        cRsp.GetFailedSubscribeList().push_back(cFailedSubscribe);
                    }
                }
				// redis订阅
                else if (SubscribeSeviceStatus == pSubItem->GetSubscribeNotifyType())
                {
                    CCuAgent *pcCuAgent = dynamic_cast<CCuAgent*>(g_cCuiDataMgr.GetTaskPtrBySession(pcReq->GetSession(), GetInstance()));
                    if (pcCuAgent != NULL)
                    {
                        pcCuAgent->m_bNeedPlatformServiceStatue = true;
                        g_cCuiDataMgr.FlushPlatformServiceStatus(GetInstance(), pcCuAgent);
                    }
                }
                else
                {
                    CMS_ASSERT(0 && "无效的订阅类型");
                }

                pSubItem++;
            }

            SendRspToCu<CCuSubscribeNotifyRsp>(cRsp, m_ptRecvNode);

            delete this;
            return PROCMSG_OK;
        }
        break;
    case CU_CUI_UNSUBSCRIBE_PUSTATUS_REQ:
        {
            CCuUnSubscribeNotifyReq *pcReq = (CCuUnSubscribeNotifyReq *)pcMsg->content;
            vector<CNotifySubscription> &tmpUnSubVct = pcReq->GetUnSubscribeList();

            CCuUnSubscribeNotifyRsp cRsp;
            cRsp.SetSeqNum(pcReq->GetSeqNum());
            cRsp.SetSession(pcReq->GetSession());
            cRsp.SetErrorCode(CMS_SUCCESS);

            TSipURI tUri;
            tUri.SetDomain(GetDomainName());
            string strCmuUri = tUri.GetCmuURI().GetURIString();
            string strMpsUri = tUri.GetMpsURI().GetURIString();
            string strRcsUri = tUri.GetRcsURI().GetURIString();

            vector<CNotifySubscription>::iterator pUnSubItem = tmpUnSubVct.begin();
            while (pUnSubItem != tmpUnSubVct.end())
            {
                //对输入数据进行校验
                if ( pUnSubItem->GetDeviceURI().empty() || pUnSubItem->GetSubscribeType().empty() )
                {                  
                    pUnSubItem++;
                    continue;   //不做任何处理
                }

                if(SubscribeTVWallStatus == pUnSubItem->GetSubscribeNotifyType())
                {
                    CTvsSubTask *pcSubTask = (CTvsSubTask *)m_pcCuAgent->GetSubTaskPtrByDevUri(pUnSubItem->GetDeviceURI() + "+" +TVWALL_ALL_STATUS);
                    if( NULL == pcSubTask)
                    {
                        cRsp.GetFailedUnSubscribeList().push_back(*pUnSubItem);
                        cRsp.SetErrorCode(ERR_CUI_NOT_SUBSCRIBE);
                    }
                    else
                    {
                        pcSubTask->EarseCuInfo(m_pcCuAgent);

                    }
                }
                else if (SubscribeSeviceStatus == pUnSubItem->GetSubscribeNotifyType())
                {
                    CCuAgent *pcCuAgent = dynamic_cast<CCuAgent*>(g_cCuiDataMgr.GetTaskPtrBySession(pcReq->GetSession(), GetInstance()));
                    if (pcCuAgent != NULL)
                    {
                        pcCuAgent->m_bNeedPlatformServiceStatue = false;
                    }
                }
                else if (SubscribePuStatus == pUnSubItem->GetSubscribeNotifyType())
                {
                    set<int>::const_iterator iter = pUnSubItem->GetSubscribeType().begin();
                    while ( iter != pUnSubItem->GetSubscribeType().end() )
                    {
                        switch(*iter)
                        {
                        case SubscribeOnlineStatus:
                            {
                                ModuleListUnsubscribe(strCmuUri, pUnSubItem->GetDeviceURI(), 
                                    PU_ON_OFF_STATUS, m_pcCuAgent);
                            }
                            break;
                        case SubscribeConfigStatus:
                            {
                                ModuleListUnsubscribe(strCmuUri, pUnSubItem->GetDeviceURI(), 
                                    PU_CONFIG_STATUS, m_pcCuAgent);
                            }
                            break;
                        case SubscribeServiceStatus:
                            {
                                ModuleListUnsubscribe(strCmuUri, pUnSubItem->GetDeviceURI(), 
                                    PU_SERVICE_STATUS, m_pcCuAgent);

                                ModuleListUnsubscribe(strRcsUri, pUnSubItem->GetDeviceURI(), 
                                    SS_TYPE_PU_PLATFORM_RECORD_STATUS, m_pcCuAgent);
                            }
                            break;
                        case SubscribeAlarmStatus:
                            {
                                ModuleListUnsubscribe(strCmuUri, pUnSubItem->GetDeviceURI(), 
                                    PU_ALARM_STATUS, m_pcCuAgent);
                            }
                            break;
                        case SubscribeGPS:
                            {
                                ModuleListUnsubscribe(strMpsUri, pUnSubItem->GetDeviceURI(), 
                                    PU_GPS_STATUS, m_pcCuAgent);
                            }
                            break;
                        case SubscribeTransdata:
                            {
                                ModuleListUnsubscribe(strCmuUri, pUnSubItem->GetDeviceURI(), 
                                    PU_TRANSDATA_STATUS, m_pcCuAgent);
                            }
                            break;
                        default:
                            {
                                TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户[%s]会话[%s]取消订阅不认识的类型[%d]\n",
                                    m_pcCuAgent->m_strUserName.c_str(), 
                                    m_pcCuAgent->m_strSession.c_str(),*iter);
                            }
                            break;
                        }

                        iter++;
                    }                  
                }
                else
                {
                    CMS_ASSERT(false && "Not Implement");
                }

                pUnSubItem++;
            }

            SendRspToCu<CCuUnSubscribeNotifyRsp>(cRsp, m_ptRecvNode);
     
            delete this;
            return PROCMSG_OK;
        }
        break;
    default:
        {
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"收到未知消息");
        }
        break;
    }
    return PROCMSG_OK;
}

CBatchInterAction::CBatchInterAction( CInstExt *pcInst, CCuAgent *pcCuAgent, PTRecvNode ptRecvNode ) : CSipTask(pcInst)
{
    CMS_ASSERT(pcCuAgent != NULL);
    CMS_ASSERT(ptRecvNode != NULL);
    m_pcCuAgent = pcCuAgent;
    m_ptRecvNode = ptRecvNode;
    m_pc3acAgent = NULL;
    m_pRsp = NULL;
    m_pcCuAgent->AddSubTask(this);
}

CBatchInterAction::~CBatchInterAction()
{
    TerminateTask();
    if ( NULL != m_pRsp )
    {
        delete m_pRsp;
        m_pRsp = NULL;
    }
}

void CBatchInterAction::TerminateTask()
{
    m_pcCuAgent->DelSubTask(this);
    if ( GetState() != NOT_REQUEST_STATE && m_ptRecvNode != NULL )
    {
        TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]发送请求[%s--%d]时，批处理任务被销毁，向CU发送应答\n",
            m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
            m_pRsp->GetEventStr().c_str(), m_pRsp->GetEventId());
        string strRsp = m_pRsp->ToXml();
        SendRspToCu(strRsp, m_ptRecvNode);
    }

    //可能因为超时而消亡，没有子任务，不需要遍历处理
    //    delete this;
}

void CBatchInterAction::InitStateMachine()
{
    CStateProc cNotRequestProc;
    cNotRequestProc.ProcMsg = (CSipTask::PFProcMsg)&CBatchInterAction::NotReqProcMsg;
    cNotRequestProc.TimerPoll = &CSipTask::TimerPollSkip;
    cNotRequestProc.ProcErrMsg = (CSipTask::PFProcMsg)&CBatchInterAction::NotReqProcMsg;
    AddRuleProc(NOT_REQUEST_STATE, cNotRequestProc);

    CStateProc cWaitRspProc;
    cWaitRspProc.ProcMsg = (CSipTask::PFProcMsg)&CBatchInterAction::WaitRspProcMsg;
    cWaitRspProc.TimerPoll = (CSipTask::PFTimerPoll)&CBatchInterAction::WaitRspTimerPoll;
    cWaitRspProc.ProcErrMsg = (CSipTask::PFProcMsg)&CBatchInterAction::WaitRspProcMsg;
    AddRuleProc(WAIT_RESPONSE_STATE, cWaitRspProc);

    NextState(NOT_REQUEST_STATE);
}

/*
处理的接口有
1，设备组添加/删除设备
2，用户添加/删除设备
3，用户组添加/删除设备
4，用户/用户组修改设备权限
因为设备与设备组只有划归关系，没有赋予权限，所以添加删除操作划归关系
而用户和用户组对设备有权限关系，通过修改接口操作，其他通过添加/删除操作来划归设备通道
*/
u32 CBatchInterAction::NotReqProcMsg( CMessage *const pcMsg )
{
    TASKLOG(NORMAL_TASK, EVENT_LEV, "收到CU用户[%s]会话[%s]的批处理请求[%s--%hu]\n",
        m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

    switch (pcMsg->event)
    {
    case TAS_DEVICE_GROUP_DEVICE_ADD_REQ:
        {
            CCuDeviceGroupAddDeviceReq *pcReq = (CCuDeviceGroupAddDeviceReq *)pcMsg->content;

            CCuDeviceGroupAddDeviceRsp *pcRsp = new CCuDeviceGroupAddDeviceRsp;
            pcRsp->SetSession(pcReq->GetSession());
            pcRsp->SetSeqNum(pcReq->GetSeqNum());
            m_pRsp = pcRsp;

            vector<CDeviceGroupDeviceInfo> &tmpVct = pcReq->GetDeviceGroupDeviceInfo();
            vector<CDeviceGroupDeviceInfo>::iterator pDevGrpDevItem = tmpVct.begin();
            while ( pDevGrpDevItem != tmpVct.end() )
            {
                pDevGrpDevItem->SetDeviceID(GetNameWithoutDomainFromURI(pDevGrpDevItem->GetDeviceID()));
                pDevGrpDevItem->SetDeviceGroupID(GetNameWithoutDomainFromURI(pDevGrpDevItem->GetDeviceGroupID()));

				pDevGrpDevItem->SetDevDevTreeId(pcReq->GetSrcDevTreeId());
				pDevGrpDevItem->SetGrpDevTreeId(pcReq->GetDstDevTreeId());

                if ( pDevGrpDevItem->GetDeviceCapIndexs().empty() )
                {
                    TASKLOG(NORMAL_TASK, ERROR_LEV, "设备组添加设备请求，参数非法\n");
                    pDevGrpDevItem++;
                    continue;
                }

                CDeviceGroupDeviceAddReq cTasDeviceGroupDeviceAddReq;
                cTasDeviceGroupDeviceAddReq.SetSession(pcReq->GetSession());
                cTasDeviceGroupDeviceAddReq.SetDeviceGroupDeviceInfo(*pDevGrpDevItem);
                //用新生成的序列号ID作为应答键值，初始状态为未收到应答
                m_tRspMarks.InsertToWaitRsp(cTasDeviceGroupDeviceAddReq.GetSeqNum());

                //发送请求
                u32 dwRet = PostMsgReqTo3AS(cTasDeviceGroupDeviceAddReq);
                if ( PROCMSG_DEL == dwRet )
                {
                    TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]往设备组里添加设备，TAS不在线，直接拒绝请求\n",
                        m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str());
                    pcRsp->SetErrorCode(ERR_CUI_TAS_OFFLINE);
                    SendRspToCu<CCuDeviceGroupAddDeviceRsp>(*pcRsp, m_ptRecvNode);
                    delete this;
                    return PROCMSG_DEL;
                }
                pDevGrpDevItem++;
            }

            //判断是否已经收集完所有结果
            if ( m_tRspMarks.IsAllReqHasRsp() )
            {
                TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]往设备组里添加设备，所有设备都收到应答，批处理任务完成\n",
                    m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str());

                SendRspToCu<CCuDeviceGroupAddDeviceRsp>(*pcRsp, m_ptRecvNode);
                //任务完成, 销毁之
                delete this;
            }
            else
            {
                NextState(WAIT_RESPONSE_STATE);
            }
        }
        break;
    case TAS_DEVICE_GROUP_DEVICE_DEL_REQ:
        {
            CCuDeviceGroupDelDeviceReq *pcReq = (CCuDeviceGroupDelDeviceReq *)pcMsg->content;

            CCuDeviceGroupDelDeviceRsp *pcRsp = new CCuDeviceGroupDelDeviceRsp;
            pcRsp->SetSession(pcReq->GetSession());
            pcRsp->SetSeqNum(pcReq->GetSeqNum());
            m_pRsp = pcRsp;

            const vector<CDeviceGroupDeviceInfo> &tmpVct = pcReq->GetDeviceGroupDeviceInfo();
            vector<CDeviceGroupDeviceInfo>::const_iterator pDevGrpDevItem = tmpVct.begin();
            while ( pDevGrpDevItem != tmpVct.end() )
            {
                if ( pDevGrpDevItem->GetDeviceCapIndexs().empty() )
                {
                    TASKLOG(NORMAL_TASK, ERROR_LEV, "设备组删除设备请求，参数非法\n");
                    pDevGrpDevItem++;
                    continue;
                }

                CDeviceGroupDeviceInfoKey tDevGrpDevInfoKey;
                tDevGrpDevInfoKey.SetDeviceID(GetNameWithoutDomainFromURI(pDevGrpDevItem->GetDeviceID()));
                tDevGrpDevInfoKey.SetDeviceGroupID(GetNameWithoutDomainFromURI(pDevGrpDevItem->GetDeviceGroupID()));
				tDevGrpDevInfoKey.SetDeviceCapIndexs(pDevGrpDevItem->GetDeviceCapIndexs());
				tDevGrpDevInfoKey.SetDevtreeId(pcReq->GetDevTreeId());
                CDeviceGroupDeviceDelReq cTasDeviceGroupDeviceDelReq;
                cTasDeviceGroupDeviceDelReq.SetSession(pcReq->GetSession());
                cTasDeviceGroupDeviceDelReq.SetDeviceGroupDeviceInfoKey(tDevGrpDevInfoKey);
                //用新生成的序列号ID作为应答键值，初始状态为未收到应答
                m_tRspMarks.InsertToWaitRsp(cTasDeviceGroupDeviceDelReq.GetSeqNum());

                //发送请求
                u32 dwRet = PostMsgReqTo3AS(cTasDeviceGroupDeviceDelReq);
                if ( PROCMSG_DEL == dwRet )
                {
                    TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]往设备组里删除设备，TAS不在线，直接拒绝请求\n",
                        m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str());
                    pcRsp->SetErrorCode(ERR_CUI_TAS_OFFLINE);
                    SendRspToCu<CCuDeviceGroupDelDeviceRsp>(*pcRsp, m_ptRecvNode);
                    //任务完成, 销毁之
                    delete this;
                    return PROCMSG_DEL;
                }
                pDevGrpDevItem++;
            }
            //判断是否已经收集完所有结果
            if ( m_tRspMarks.IsAllReqHasRsp() )
            {
                TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]往设备组里删除设备，所有设备都收到应答，批处理任务完成\n",
                    m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str());
                SendRspToCu<CCuDeviceGroupDelDeviceRsp>(*pcRsp, m_ptRecvNode);
                //任务完成, 销毁之
                delete this;
            }
            else
            {
                NextState(WAIT_RESPONSE_STATE);
            }
        }
        break;
    case TAS_CUSER_DEVICE_ADD_REQ:
        {
            CCuUserAddDeviceReq *pcReq = (CCuUserAddDeviceReq *)pcMsg->content;

            CCuUserAddDeviceRsp *pcRsp = new CCuUserAddDeviceRsp;
            pcRsp->SetSession(pcReq->GetSession());
            pcRsp->SetSeqNum(pcReq->GetSeqNum());
            m_pRsp = pcRsp;

            vector<CUserDeviceInfo>::iterator pUserDevItem = pcReq->GetUserDeviceInfoList().begin();
            while ( pUserDevItem != pcReq->GetUserDeviceInfoList().end() )
            {
                CUserDeviceAddReq cUserDevAddReq;
                cUserDevAddReq.SetSession(pcReq->GetSession());
                cUserDevAddReq.SetUserDeviceInfo(*pUserDevItem);
                cUserDevAddReq.GetUserDeviceInfo().SetUserID(GetNameWithoutDomainFromURI(pUserDevItem->GetUserID()));
                cUserDevAddReq.GetUserDeviceInfo().SetDeviceID(GetNameWithoutDomainFromURI(pUserDevItem->GetDeviceID()));

                //用新生成的序列号ID作为应答键值，初始状态为未收到应答
                m_tRspMarks.InsertToWaitRsp(cUserDevAddReq.GetSeqNum());

                //发送请求
                u32 dwRet = PostMsgReqTo3AS(cUserDevAddReq);
                if ( PROCMSG_DEL == dwRet )
                {
                    TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]给用户添加设备，TAS不在线，直接拒绝请求\n",
                        m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str());
                    pcRsp->SetErrorCode(ERR_CUI_TAS_OFFLINE);
                    SendRspToCu<CCuUserAddDeviceRsp>(*pcRsp, m_ptRecvNode);

                    //任务完成, 销毁之
                    delete this;
                    return PROCMSG_DEL;
                }
                pUserDevItem++;
            }
            //判断是否已经收集完所有结果
            if ( m_tRspMarks.IsAllReqHasRsp() )
            {
                TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]给用户添加设备，所有设备都收到应答，批处理任务完成\n",
                    m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str());
                SendRspToCu<CCuUserAddDeviceRsp>(*pcRsp, m_ptRecvNode);
                //任务完成, 销毁之
                delete this;
            }
            else
            {
                NextState(WAIT_RESPONSE_STATE);
            }
        }
        break;
    case TAS_CUSER_GROUP_DEVICE_ADD_REQ:
        {
            CCuUserGroupAddDeviceReq *pcReq = (CCuUserGroupAddDeviceReq *)pcMsg->content;

            CCuUserGroupAddDeviceRsp *pcRsp = new CCuUserGroupAddDeviceRsp;
            pcRsp->SetSession(pcReq->GetSession());
            pcRsp->SetSeqNum(pcReq->GetSeqNum());
            m_pRsp = pcRsp;

            vector<CUserGroupDeviceInfo>::iterator pUserGroupDevItem = pcReq->GetUserGroupDeviceInfoList().begin();
            while ( pUserGroupDevItem != pcReq->GetUserGroupDeviceInfoList().end() )
            {
                pUserGroupDevItem->SetDeviceID(GetNameWithoutDomainFromURI(pUserGroupDevItem->GetDeviceID()));
                pUserGroupDevItem->SetGroupID(GetNameWithoutDomainFromURI(pUserGroupDevItem->GetGroupID()));

                CUserGroupDeviceAddReq cUserGroupDevAddReq;
                cUserGroupDevAddReq.SetSession(pcReq->GetSession());
                cUserGroupDevAddReq.SetUserGroupDeviceInfo(*pUserGroupDevItem);

                //用新生成的序列号ID作为应答键值，初始状态为未收到应答
                m_tRspMarks.InsertToWaitRsp(cUserGroupDevAddReq.GetSeqNum());

                //发送请求
                u32 dwRet = PostMsgReqTo3AS(cUserGroupDevAddReq);
                if ( PROCMSG_DEL == dwRet )
                {
                    TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]给用户组添加设备，TAS不在线，直接拒绝请求\n",
                        m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str());
                    pcRsp->SetErrorCode(ERR_CUI_TAS_OFFLINE);
                    SendRspToCu<CCuUserGroupAddDeviceRsp>(*pcRsp, m_ptRecvNode);
                    //任务完成, 销毁之
                    delete this;
                    return PROCMSG_DEL;
                }
                pUserGroupDevItem++;
            }
            //判断是否已经收集完所有结果
            if ( m_tRspMarks.IsAllReqHasRsp() )
            {
                TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]给用户组添加设备，所有设备都收到应答，批处理任务完成\n",
                    m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str());
                SendRspToCu<CCuUserGroupAddDeviceRsp>(*pcRsp, m_ptRecvNode);
                //任务完成, 销毁之
                delete this;
            }
            else
            {
                NextState(WAIT_RESPONSE_STATE);
            }
        }
        break;
    case TAS_CUSER_DEVICE_DEL_REQ:
        {
            CCuUserDelDeviceReq *pcReq = (CCuUserDelDeviceReq *)pcMsg->content;

            CCuUserDelDeviceRsp *pcRsp = new CCuUserDelDeviceRsp;
            pcRsp->SetSession(pcReq->GetSession());
            pcRsp->SetSeqNum(pcReq->GetSeqNum());
            m_pRsp = pcRsp;

            vector<CUserDeviceInfo>::iterator pUserDevItem = pcReq->GetUserDeviceInfoList().begin();
            while ( pUserDevItem != pcReq->GetUserDeviceInfoList().end() )
            {
                CUserDeviceDelReq cUserDevReq;
                cUserDevReq.SetSession(pcReq->GetSession());
                CUserDeviceInfoKey cDelKey;
                cDelKey.SetUserID(GetNameWithoutDomainFromURI(pUserDevItem->GetUserID()));
                cDelKey.SetDeviceID(GetNameWithoutDomainFromURI(pUserDevItem->GetDeviceID()));
                Convert_DeviceCapPrivilege_TO_DeviceCapIndexs(pUserDevItem->GetDeviceCapPrivilege(), cDelKey.GetDeviceCapIndexs());
                cUserDevReq.SetUserDeviceInfoKey(cDelKey);

                //用新生成的序列号ID作为应答键值，初始状态为未收到应答
                m_tRspMarks.InsertToWaitRsp(cUserDevReq.GetSeqNum());

                //发送请求
                u32 dwRet = PostMsgReqTo3AS(cUserDevReq);
                if ( PROCMSG_DEL == dwRet )
                {
                    TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]给用户删除设备，TAS不在线，直接拒绝请求\n",
                        m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str());
                    pcRsp->SetErrorCode(ERR_CUI_TAS_OFFLINE);
                    SendRspToCu<CCuUserDelDeviceRsp>(*pcRsp, m_ptRecvNode);

                    //任务完成, 销毁之
                    delete this;
                    return PROCMSG_DEL;
                }
                pUserDevItem++;
            }
            //判断是否已经收集完所有结果
            if ( m_tRspMarks.IsAllReqHasRsp() )
            {
                TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]给用户删除设备，所有设备都收到应答，批处理任务完成\n",
                    m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str());
                SendRspToCu<CCuUserDelDeviceRsp>(*pcRsp, m_ptRecvNode);
                //任务完成, 销毁之
                delete this;
            }
            else
            {
                NextState(WAIT_RESPONSE_STATE);
            }
        }
        break;
    case TAS_CUSER_GROUP_DEVICE_DEL_REQ:
        {
            CCuUserGroupDelDeviceReq *pcReq = (CCuUserGroupDelDeviceReq *)pcMsg->content;

            CCuUserGroupDelDeviceRsp *pcRsp = new CCuUserGroupDelDeviceRsp;
            pcRsp->SetSession(pcReq->GetSession());
            pcRsp->SetSeqNum(pcReq->GetSeqNum());
            m_pRsp = pcRsp;

            vector<CUserGroupDeviceInfo>::iterator pUserGroupDevItem = pcReq->GetUserGroupDeviceInfoList().begin();
            while (pUserGroupDevItem != pcReq->GetUserGroupDeviceInfoList().end())
            {
                CUserGroupDeviceDelReq cUserGroupDeviceDelReq;
                cUserGroupDeviceDelReq.SetSession(pcReq->GetSession());
                CUserGroupDeviceInfoKey cDelKey;
                cDelKey.SetGroupID(GetNameWithoutDomainFromURI(pUserGroupDevItem->GetGroupID()));
                cDelKey.SetDeviceID(GetNameWithoutDomainFromURI(pUserGroupDevItem->GetDeviceID()));
                Convert_DeviceCapPrivilege_TO_DeviceCapIndexs(pUserGroupDevItem->GetDeviceCapPrivilege(), cDelKey.GetDeviceCapIndexs());
                cUserGroupDeviceDelReq.SetUserGroupDeviceInfoKey(cDelKey);

                //用新生成的序列号ID作为应答键值，初始状态为未收到应答
                m_tRspMarks.InsertToWaitRsp(cUserGroupDeviceDelReq.GetSeqNum());

                //发送请求
                u32 dwRet = PostMsgReqTo3AS(cUserGroupDeviceDelReq);
                if ( PROCMSG_DEL == dwRet )
                {
                    TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]给用户组删除设备，TAS不在线，直接拒绝请求\n",
                        m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str());
                    pcRsp->SetErrorCode(ERR_CUI_TAS_OFFLINE);
                    SendRspToCu<CCuUserGroupDelDeviceRsp>(*pcRsp, m_ptRecvNode);

                    //任务完成, 销毁之
                    delete this;
                    return PROCMSG_DEL;
                }
                pUserGroupDevItem++;
            }
            //判断是否已经收集完所有结果
            if ( m_tRspMarks.IsAllReqHasRsp() )
            {
                TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]给用户组删除设备，所有设备都收到应答，批处理任务完成\n",
                    m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str());
                SendRspToCu<CCuUserGroupDelDeviceRsp>(*pcRsp, m_ptRecvNode);
                //任务完成, 销毁之
                delete this;
            }
            else
            {
                NextState(WAIT_RESPONSE_STATE);
            }
        }
        break;
    case TAS_CUSER_DEVICE_MOD_REQ:
        {
            CCuUserModDeviceReq *pcReq = (CCuUserModDeviceReq *)pcMsg->content;

            CCuUserModDeviceRsp *pcRsp = new CCuUserModDeviceRsp;
            pcRsp->SetSession(pcReq->GetSession());
            pcRsp->SetSeqNum(pcReq->GetSeqNum());
            m_pRsp = pcRsp;

            vector<CUserDeviceInfo>::iterator pUserDevItem = pcReq->GetUserDeviceInfoList().begin();
            while (pUserDevItem != pcReq->GetUserDeviceInfoList().end())
            {
                CUserDeviceModReq cUserDevModReq;
                cUserDevModReq.SetSession(pcReq->GetSession());
                cUserDevModReq.SetUserID(GetNameWithoutDomainFromURI(pUserDevItem->GetUserID()));
                cUserDevModReq.SetDeviceID(GetNameWithoutDomainFromURI(pUserDevItem->GetDeviceID()));
                cUserDevModReq.SetDevicePrivilege(pUserDevItem->GetDevicePrivilege());
                cUserDevModReq.SetDeviceCapPrivilege(pUserDevItem->GetDeviceCapPrivilege());

                //用新生成的序列号ID作为应答键值，初始状态为未收到应答
                m_tRspMarks.InsertToWaitRsp(cUserDevModReq.GetSeqNum());

                //发送请求
                u32 dwRet = PostMsgReqTo3AS(cUserDevModReq);
                if ( PROCMSG_DEL == dwRet )
                {
                    TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]为用户修改设备权限，TAS不在线，直接拒绝请求\n",
                        m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str());
                    pcRsp->SetErrorCode(ERR_CUI_TAS_OFFLINE);
                    SendRspToCu<CCuUserModDeviceRsp>(*pcRsp, m_ptRecvNode);

                    //任务完成, 销毁之
                    delete this;
                    return PROCMSG_DEL;
                }
                pUserDevItem++;
            }
            //判断是否已经收集完所有结果
            if ( m_tRspMarks.IsAllReqHasRsp() )
            {
                TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]为用户修改设备权限，所有设备都收到应答，批处理任务完成\n",
                    m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str());
                SendRspToCu<CCuUserModDeviceRsp>(*pcRsp, m_ptRecvNode);				
				m_pcCuAgent->WriteDeviceModLog(pcRsp->GetErrorCode(), false);
                //任务完成, 销毁之
                delete this;
            }
            else
            {
                NextState(WAIT_RESPONSE_STATE);
            }
        }
        break;
    case TAS_CUSER_GROUP_DEVICE_MOD_REQ:
        {
            CCuUserGroupModDeviceReq *pcReq = (CCuUserGroupModDeviceReq *)pcMsg->content;

            CCuUserGroupModDeviceRsp *pcRsp = new CCuUserGroupModDeviceRsp;
            pcRsp->SetSession(pcReq->GetSession());
            pcRsp->SetSeqNum(pcReq->GetSeqNum());
            m_pRsp = pcRsp;

            vector<CUserGroupDeviceInfo>::iterator pUserGroupDevItem = pcReq->GetUserGroupDeviceInfoList().begin();
            while (pUserGroupDevItem != pcReq->GetUserGroupDeviceInfoList().end())
            {
                CUserGroupDeviceModReq cUserGroupDeviceModReq;
                cUserGroupDeviceModReq.SetSession(pcReq->GetSession());
                cUserGroupDeviceModReq.SetGroupID(GetNameWithoutDomainFromURI(pUserGroupDevItem->GetGroupID()));
                cUserGroupDeviceModReq.SetDeviceID(GetNameWithoutDomainFromURI(pUserGroupDevItem->GetDeviceID()));
                cUserGroupDeviceModReq.SetDevicePrivilege(pUserGroupDevItem->GetDevicePrivilege());
                cUserGroupDeviceModReq.SetDeviceCapPrivilege(pUserGroupDevItem->GetDeviceCapPrivilege());

                //用新生成的序列号ID作为应答键值，初始状态为未收到应答
                m_tRspMarks.InsertToWaitRsp(cUserGroupDeviceModReq.GetSeqNum());

                //发送请求
                u32 dwRet = PostMsgReqTo3AS(cUserGroupDeviceModReq);
                if ( PROCMSG_DEL == dwRet )
                {
                    TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]为用户组修改设备权限，TAS不在线，直接拒绝请求\n",
                        m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str());
                    pcRsp->SetErrorCode(ERR_CUI_TAS_OFFLINE);
                    SendRspToCu<CCuUserGroupModDeviceRsp>(*pcRsp, m_ptRecvNode);
					if (m_pcCuAgent)
					{
						m_pcCuAgent->WriteDeviceModLog(pcRsp->GetErrorCode(), true);
					}
                    //任务完成, 销毁之
                    delete this;
                    return PROCMSG_DEL;
                }
                pUserGroupDevItem++;
            }
            //判断是否已经收集完所有结果
            if ( m_tRspMarks.IsAllReqHasRsp() )
            {
                TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]为用户组修改设备权限，所有设备都收到应答，批处理任务完成\n",
                    m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str());
                SendRspToCu<CCuUserGroupModDeviceRsp>(*pcRsp, m_ptRecvNode);
				if (m_pcCuAgent)
				{
					m_pcCuAgent->WriteDeviceModLog(pcRsp->GetErrorCode(), true);
				}
                //任务完成, 销毁之
                delete this;
            }
            else
            {
                NextState(WAIT_RESPONSE_STATE);
            }
        }
        break;
        //以上步骤总结
        //先将Rsp消息构造好
        //将CU请求分解成一条条TAS请求
        //将无效请求过滤
        //视情况考虑是否鉴权
        //将本次处理对象加入到Marks标签集中，以便收集结果
        //发送请求
        //判断是否已经收集完所有结果
        //进入应答状态
    default:
        {
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"收到未知消息");
        }
        break;
    }
    return PROCMSG_OK;
}

u32 CBatchInterAction::WaitRspProcMsg( CMessage *const pcMsg )
{
    COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;

    TASKLOG(NORMAL_TASK, EVENT_LEV, "CBatchInterAction[TaskNO %u]收到服务器[%s]的应答消息[%s--%hu]\n",
        GetTaskNO(), pcOspSipMsg->GetSipToUri().GetURIString().c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

    switch (pcMsg->event)
    {
    case OSP_SIP_MSG_PROC_FAIL:
        {
            TASKLOG(NORMAL_TASK, EVENT_LEV, "[CBatchInterAction]收到OSP_SIP_MSG_PROC_FAIL或者OSP_SIP_DIALOG_TERMINATE消息\n");
            m_tRspMarks.SetRspMark((s32)pcOspSipMsg->GetOspSeqNO(), true);
            SetRspErrorCode(ERR_CUI_SIP_MSG_FAIL);

            //判断是否已经收集完所有结果
            if ( m_tRspMarks.IsAllReqHasRsp() )
            {
                TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]发送请求[%s--%d]失败，所有设备都处理完毕，批处理任务完成\n",
                    m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
                    m_pRsp->GetEventStr().c_str(), m_pRsp->GetEventId());
                string strRsp = m_pRsp->ToXml();
                SendRspToCu(strRsp, m_ptRecvNode);

                //任务完成, 销毁之
                NextState(NOT_REQUEST_STATE);
                delete this;
            }
        }
        break;
    case OSP_SIP_DIALOG_TERMINATE:
        {
            CMS_ASSERT( 0 && "CBatchInterAction::WaitRspProcMsg 不可能有会话内消息" );
        }
        break;
    case TAS_DEVICE_GROUP_DEVICE_ADD_RSP:
        {
            CDeviceGroupDeviceAddRsp cRsp;
            GetRspMsg(cRsp, pcMsg);

            OnWaitRspMsg<CDeviceGroupDeviceAddRsp, CCuDeviceGroupAddDeviceRsp>(cRsp);
        }
        break;
    case TAS_DEVICE_GROUP_DEVICE_DEL_RSP:
        {  
            CDeviceGroupDeviceDelRsp cRsp;
            GetRspMsg(cRsp, pcMsg);

            OnWaitRspMsg<CDeviceGroupDeviceDelRsp, CCuDeviceGroupDelDeviceRsp>(cRsp);
        }
        break;
    case TAS_CUSER_DEVICE_ADD_RSP:
        {
            CUserDeviceAddRsp cRsp;
            GetRspMsg(cRsp, pcMsg);

            OnWaitRspMsg<CUserDeviceAddRsp, CCuUserAddDeviceRsp>(cRsp);
        }
        break;
    case TAS_CUSER_GROUP_DEVICE_ADD_RSP:
        {
            CUserGroupDeviceAddRsp cRsp;
            GetRspMsg(cRsp, pcMsg);

            OnWaitRspMsg<CUserGroupDeviceAddRsp, CCuUserGroupAddDeviceRsp>(cRsp);
        }
        break;
    case TAS_CUSER_DEVICE_DEL_RSP:
        {
            CUserDeviceDelRsp cRsp;
            GetRspMsg(cRsp, pcMsg);

            OnWaitRspMsg<CUserDeviceDelRsp, CCuUserDelDeviceRsp>(cRsp);
        }
        break;
    case TAS_CUSER_GROUP_DEVICE_DEL_RSP:
        {
            CUserGroupDeviceDelRsp cRsp;
            GetRspMsg(cRsp, pcMsg);

            OnWaitRspMsg<CUserGroupDeviceDelRsp, CCuUserGroupDelDeviceRsp>(cRsp);
        }
        break;
    case TAS_CUSER_DEVICE_MOD_RSP:
        {
            CUserDeviceModRsp cRsp;
            GetRspMsg(cRsp, pcMsg);
            OnWaitRspMsg<CUserDeviceModRsp, CCuUserModDeviceRsp>(cRsp);
			if (m_pcCuAgent)
			{
				m_pcCuAgent->WriteDeviceModLog(cRsp.GetErrorCode(), false);
			}
        }
        break;
    case TAS_CUSER_GROUP_DEVICE_MOD_RSP:
        {
            CUserGroupDeviceModRsp cRsp;
            GetRspMsg(cRsp, pcMsg);

            OnWaitRspMsg<CUserGroupDeviceModRsp, CCuUserGroupModDeviceRsp>(cRsp);
			if (m_pcCuAgent)
			{
				m_pcCuAgent->WriteDeviceModLog(cRsp.GetErrorCode(), true);
			}
        }
        break;
        //以上步骤总结
        //处理服务器的单个应答, 设置应答标记
        //收集结果，更新错误码
        //判断是否已经收集完所有结果
    default:
        {
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"收到未知消息");
        }
        break;
    }
    return PROCMSG_OK;
}

u32 CBatchInterAction::WaitRspTimerPoll()
{
    if ( GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout() )
    {
        TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户[%s]会话[%s]等待应答消息[%s--%d]超时:SeqNum[%d]\n",
            m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
            m_pRsp->GetEventStr().c_str(), m_pRsp->GetEventId(), m_pRsp->GetSeqNum());
        CMS_ASSERT(NULL != m_ptRecvNode);
        m_pRsp->SetErrorCode(ERR_CUI_OPR_TIME_OUT);
        string strRspOfErr = m_pRsp->ToXml();
        SendRspToCu(strRspOfErr, m_ptRecvNode);

        NextState(NOT_REQUEST_STATE);
        return TIMERPOLL_DEL;
    }
    return TIMERPOLL_DONE;
}

void CBatchInterAction::SetRspErrorCode( int nErrorCode )
{
    if ( nErrorCode == CMS_SUCCESS )
    {
        return;
    }
    if ( m_pRsp->GetErrorCode() != CMS_SUCCESS )
    {
        return;
    }
    m_pRsp->SetErrorCode(nErrorCode);
}

//////////////////////////////////////////////////////////////////////////
//CInviteTask
CInviteTaskEx::CInviteTaskEx(CInstExt *pcInst, CCuAgent *pcCuAgent, CSipTask *pcInviteInterActTask,s32 nPlayId) : CSipTask(pcInst), m_cStrmNumRestorer( pcInst )
{
    CMS_ASSERT(NULL != pcCuAgent);
    CMS_ASSERT(NULL != pcInviteInterActTask);
    m_pcCuAgent = pcCuAgent;
    m_pcInviteInterActTask = pcInviteInterActTask;
    m_tInviteID = INVALID_DIALOG_ID;
    m_bByeWhileWaitInviteRsp = false;
	m_nPlayId = nPlayId;
    m_strVtduId = "";
    m_cStrmNumRestorer.SetUserLoginSession( pcCuAgent->m_strSession );
}
CInviteTaskEx::~CInviteTaskEx()
{
    //释放选中的vtdu资源
    ReleaseVtduResource();
    if (!m_strVtduId.empty())
	{
		CRedisSsChannel cSsChn = g_cCuiRedisSdk.GetModuleSsChannelById(m_strVtduId);
		CRedisSsMng::GetInstance()->DelSs(cSsChn, GetTaskNO());
	}
    TerminateTask();
}

void CInviteTaskEx::TerminateTask()
{
    Iterator pPos;
    int nSeqNo = 0;
    int nTaskNo = INVALID_TASKNO;
    while(!pPos.End())
    {
        if (m_cChildTaskList.Iterate(pPos, nSeqNo, nTaskNo) )
        {
            CTask* pcTask = NULL;
            pcTask = GetInstance()->FindTask(nTaskNo);
            if(pcTask != NULL)
            {
                delete pcTask;
                pcTask = NULL;
            }        
        }
    }
    if ( m_tInviteID != INVALID_DIALOG_ID )
    { //释放会话
		PostInDlgReq(KDSIP_EVENT_INVITE_BYE_REQ, CByeReq(), m_tInviteID);
        //OspSipReleaseDlg(m_tInviteID);
        m_tInviteID = INVALID_DIALOG_ID;
    }

    //注销CuAgent中的浏览事务
    if ( INVALID_PLAY_ID != m_tReq.GetDstChn().GetChnNO() )
    {
        m_pcCuAgent->m_cInviteTaskSet.Erase(m_tReq.GetDstChn().GetChnNO());
        m_tReq.GetDstChn().SetChnNO(INVALID_PLAY_ID);

		PopMediaTask(m_pcCuAgent->m_strSession,m_nPlayId);
    }

    m_pcInviteInterActTask = NULL;
}


bool CInviteTaskEx::IsNotNeedPollState()
{
    if (GetState() == INVITE_HOLD_STATE)
    {
        return true;
    }

    return false;
}


void CInviteTaskEx::InitStateMachine()
{
    CStateProc cNotReqState;
    cNotReqState.ProcMsg = (CSipTask::PFProcMsg)&CInviteTaskEx::NotReqProcMsg;
    cNotReqState.TimerPoll = &CSipTask::TimerPollSkip;
    cNotReqState.ProcErrMsg = (CSipTask::PFProcMsg)&CInviteTaskEx::NotReqProcMsg;
    AddRuleProc(NOT_REQUEST_STATE, cNotReqState);

    CStateProc cWaitSetupRspState;
    cWaitSetupRspState.ProcMsg = (CSipTask::PFProcMsg)&CInviteTaskEx::WaitSetupRspProcMsg;
    cWaitSetupRspState.TimerPoll = (CSipTask::PFTimerPoll)&CInviteTaskEx::WaitSetupRspTimerPoll;
    cWaitSetupRspState.ProcErrMsg = (CSipTask::PFProcMsg)&CInviteTaskEx::WaitSetupRspProcMsg;
    AddRuleProc(WAIT_SETUP_RSP_STATE, cWaitSetupRspState);

    CStateProc cWaitSetupAckState;
    cWaitSetupAckState.ProcMsg = (CSipTask::PFProcMsg)&CInviteTaskEx::WaitSetupAckProcMsg;
    cWaitSetupAckState.TimerPoll = (CSipTask::PFTimerPoll)&CInviteTaskEx::WaitAckTimerPoll;
    cWaitSetupAckState.ProcErrMsg = (CSipTask::PFProcMsg)&CInviteTaskEx::WaitSetupAckProcMsg;
    AddRuleProc(WAIT_SETUP_ACK_STATE, cWaitSetupAckState);

    CStateProc cInviteHoldState;
    cInviteHoldState.ProcMsg = (CSipTask::PFProcMsg)&CInviteTaskEx::InviteHoldProcMsg;
    cInviteHoldState.TimerPoll = &CSipTask::TimerPollSkip;
    cInviteHoldState.ProcErrMsg = (CSipTask::PFProcMsg)&CInviteTaskEx::InviteHoldProcMsg;
    AddRuleProc(INVITE_HOLD_STATE, cInviteHoldState);

    CStateProc cWaitByeRspState;
    cWaitByeRspState.ProcMsg = (CSipTask::PFProcMsg)&CInviteTaskEx::WaitByeRspProcMsg;
    cWaitByeRspState.TimerPoll = (CSipTask::PFTimerPoll)&CInviteTaskEx::WaitByeRspTimerPoll;
    cWaitByeRspState.ProcErrMsg = (CSipTask::PFProcMsg)&CInviteTaskEx::WaitByeRspProcMsg;
    AddRuleProc(WAIT_BYE_RSP_STATE, cWaitByeRspState);

    NextState(NOT_REQUEST_STATE);
}

u32 CInviteTaskEx::NotReqProcMsg( CMessage *const pcMsg )
{
    TASKLOG(NORMAL_TASK, CRITICAL_LEV, "在[CInviteTask::NotReqProcMsg]收到未知消息[%s--%hu]，不处理\n",
        OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
    CMS_ASSERT(0&&"CInviteTask::NotReqProcMsg收到未知消息");
    return PROCMSG_OK;
}

s32 CInviteTaskEx::StartSendInviteReq( CInviteReq &cReq )
{
	m_tReq = cReq;//2018/8/22 zsy
    if( !m_cStrmNumRestorer.Start() )
    {
        TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户[%s]会话[%s] 已达到最大并发数，本次请求被拒绝\n",
            m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str() );

        m_dwErrorCode = ERR_CUI_USER_EXCEEDED_MAX_STREAM;
        return PROCMSG_FAIL;
    }

    CMS_ASSERT( NOT_REQUEST_STATE == GetState() );

    TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]发送Invite请求，建立源通道[%s--%d]到目的通道[%s--%d]的交换\n",
        m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
        cReq.GetSrcChn().GetDevUri().c_str(), cReq.GetSrcChn().GetChnNO(),
        cReq.GetDstChn().GetDevUri().c_str(), cReq.GetDstChn().GetChnNO());

    //m_tReq = cReq;//2018/8/22 zsy 需要提前到第一个可能的return 之前，否则前面return了，后面销毁本对象的时候就无法释放invite了。
    TASKLOG(NORMAL_TASK, EVENT_LEV, "CU PlayBack Process Ntf Req--StartTime[%s]\n",
        cReq.GetSdp().GetTimeRange().GetStartTime().c_str());
    //m_tCuChn = cReq.GetDstChn();
    //m_tDevChn = cReq.GetSrcChn();

	//平台在NAT内，使用NatPacketMode
	if (NATModeInner == m_pcCuAgent->m_ePlatNat)
	{
		m_tReq.GetSdp().SetIsNatPacketMode(true);
	}
	 //cu在NAT內，更好碼流地址為其NAT外地址
	if (m_pcCuAgent->m_eClientNat == NATModeInner) 
	{
		SwitchToNatAddress(m_tReq.GetSdp());
	}
#ifdef _UNIT_TEST_
    g_cTaskNOQue.push(GetTaskNO());
    cout << "Post InviteReq To " << m_pcCuAgent->m_tCmuURI.GetURIString().c_str() << endl;
    cout << cReq.ToXml().c_str() << endl;
#else
	//改为直接发给VTDU
    //PostReq<CInviteReq>(KDSIP_EVENT_INVITE_REQ, m_tReq, m_pcCuAgent->m_tCmuURI);
	string strVtduId;
	string strDomainName = g_cCuiConfig.m_cCmCfg.GetDomainName();
    
    TSipURI tDefaultUrl = g_cCuiConfig.GetCmuURI();

    TSipURI tSrcUri;
    tSrcUri.SetURIString( cReq.GetSrcChn().GetDevUri() );
    TSipURI tDstUri;
    tDstUri.SetURIString( cReq.GetDstChn().GetDevUri() );

    TVtduResource tVtduRes;
    tVtduRes.tSrcAgent.nChannel = cReq.GetSrcChn().GetChnNO();
    tVtduRes.tSrcAgent.strId    = tSrcUri.GetUser();
    tVtduRes.tDstAgent.nChannel = cReq.GetDstChn().GetChnNO();
    tVtduRes.tDstAgent.strId    = tDstUri.GetUser();
    
    tVtduRes.switchType = CStreamDecentrationAdapter::SdpSessionNameToRedisSwitchType( cReq.GetSdp().GetSessionName() );
    bool bSelSuccessful = CStreamDecentrationAdapter::SelectVtduResouce( tVtduRes, tDefaultUrl );
    
	TASKLOG(NORMAL_TASK, EVENT_LEV, "ErrCode[%d]\n", tVtduRes.err);
	
    if ( bSelSuccessful )//CMS_SUCCESS == eErrCode
	{
        m_strVtduResourceId = tVtduRes.resId;
        m_strVtduId = tVtduRes.uri.GetUser();
        CRedisSsChannel cSsChn = g_cCuiRedisSdk.GetModuleSsChannelById(m_strVtduId);
		CRedisSsMng::GetInstance()->AddSs(cSsChn, GetTaskNO());

        m_tReq.GetSdp().SetResourceId( tVtduRes.resId );		
		PostReq<CInviteReq>(KDSIP_EVENT_INVITE_REQ, m_tReq, tVtduRes.uri );
	}
	else
	{
		PostReq<CInviteReq>(KDSIP_EVENT_INVITE_REQ, m_tReq, m_pcCuAgent->m_tCmuURI);
	}
#endif

    NextState(WAIT_SETUP_RSP_STATE);
    return 0;
}

u32 CInviteTaskEx::WaitSetupRspProcMsg( CMessage *const pcMsg )
{
    COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
    TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]收到业务应答消息[%s--%d] (SIP应答消息[%s--%d])\n",
        m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
        OspExtEventDesc(pcOspSipMsg->GetOspEventID()).c_str(), pcOspSipMsg->GetOspEventID(),
        pcOspSipMsg->GetSipEventStr().c_str(), pcOspSipMsg->GetSipEventID());

    switch ( pcMsg->event )
    {
    case OSP_SIP_BYE_REQ:   //其实这里不可能会收到Bye，因为呼叫还未建立，要么成功要么拒绝，收到403错误码
        {
            if ( m_pcInviteInterActTask != NULL )
            {
                m_pcInviteInterActTask->ProcMsg(pcMsg);
            }

            if(pcOspSipMsg->GetMsgBodyLen() >0)
            {
                CByeReq cByeReq;
                pcOspSipMsg->GetMsgBody(cByeReq);
                NotifyStop2Cu(cByeReq.GetReason());
            }
            else
            {
                NotifyStop2Cu(ERR_CUI_DIALOG_TERMINATED);
            }

            string strNullMsg;
            PostRsp(KDSIP_EVENT_INVITE_BYE_RSP, pcOspSipMsg->GetSipTransID(), strNullMsg);
            m_tInviteID = INVALID_DIALOG_ID;
            NextState(NOT_REQUEST_STATE);
            delete this;
            return PROCMSG_DEL;
            //CMS_ASSERT(0&&"不可能在WaitSetupRsp状态下收到OSP_SIP_BYE_REQ消息");
        }
        break;
    case OSP_SIP_MSG_PROC_FAIL:
    case OSP_SIP_DIALOG_TERMINATE:
        {
            //将消息回调给InviteInterAckTask类
            if ( m_pcInviteInterActTask != NULL )
            {
                m_pcInviteInterActTask->ProcMsg(pcMsg);
            }
            TASKLOG(NORMAL_TASK, ERROR_LEV, "收到InviteRsp失败消息，销毁CInviteTask[TaskNO: %u]\n",
                GetTaskNO());
            m_tInviteID = INVALID_DIALOG_ID;
            NextState(NOT_REQUEST_STATE);
            delete this;
            return PROCMSG_DEL;
        }
        break;
    case INVITE_RSP:
        {
            //判断是否业务层失败
            CInviteRsp cRsp;
            pcOspSipMsg->GetMsgBody(cRsp);

            //保存DlgID
            m_tInviteID = pcOspSipMsg->GetSipDlgID();
            m_tRsp = cRsp;

            if (CMS_SUCCESS == cRsp.GetErrorCode() && m_bByeWhileWaitInviteRsp)
            {
                cRsp.SetErrorCode(ERR_CUI_DIALOG_TERMINATED_BY_CMU);
            }

            //将消息回调给InviteInterAckTask类
            if ( m_pcInviteInterActTask != NULL )
            {
                m_pcInviteInterActTask->ProcMsg(pcMsg);
            }

            if ( INVALID_DIALOG_ID == m_tInviteID )
            {
                TASKLOG(NORMAL_TASK, ERROR_LEV, "收到的InviteRsp含带DialogID为 0 !!!!直接销毁CInviteTask[TaskNO %u]\n",
                    GetTaskNO());
                m_tInviteID = INVALID_DIALOG_ID;
                NextState(NOT_REQUEST_STATE);
                delete this;
                return PROCMSG_DEL;
            }

            if ( CMS_SUCCESS != cRsp.GetErrorCode() )
            {
                TASKLOG(NORMAL_TASK, ERROR_LEV, "Invite请求失败，错误码[%d]，销毁CInviteTask[TaskNO: %u]，结束Sip Dialog: [%u]\n",
                    cRsp.GetErrorCode(), GetTaskNO(), m_tInviteID);

                StartSendInviteBye();
                m_tInviteID = INVALID_DIALOG_ID;
                delete this;
                return PROCMSG_DEL;
                //OspSipReleaseDlg(m_tInviteID); 发送了Bye就不需要再主动释放
                //等待Bye应答之后再销毁本Task
                //delete this;
                //return PROCMSG_OK;
            }
            else
            {
                TASKLOG(NORMAL_TASK, EVENT_LEV, "Invite请求成功，将CInviteTask[TaskNO: %u, PlayId: %d]登记到CCuAgent的任务列表中\n",
                    GetTaskNO(), m_tReq.GetDstChn().GetChnNO());

                //调整录像时间
                CCMSTime cStartTime(m_tReq.GetSdp().GetTimeRange().GetStartTime());
                CCMSTime cEndTime(m_tReq.GetSdp().GetTimeRange().GetEndTime());

                m_tNtfReq.SetCurPlayTime((u32)cStartTime.GetTime());
                m_tNtfReq.SetStartPlayTime((u32)cStartTime.GetTime());
                if(cEndTime.GetTime() >= cStartTime.GetTime())
                {
                    m_tNtfReq.SetTotalPlayTime((u32)(cEndTime.GetTime() - cStartTime.GetTime()));
                }
                else
                {
                    m_tNtfReq.SetTotalPlayTime(0);
                }
                m_tNtfReq.SetIsPlayFinished(false);

                NextState(WAIT_SETUP_ACK_STATE);
                StartSendInviteAck();
            }
        }
        break;
    default:
        {
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            //CMS_ASSERT(0&&"收到未知消息");
        }
        break;
    }
    return PROCMSG_OK;
}

u32 CInviteTaskEx::WaitSetupAckProcMsg( CMessage *const pcMsg )
{
    COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
    TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]收到业务应答消息[%s--%d] (SIP应答消息[%s--%d])\n",
        m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
        OspExtEventDesc(pcOspSipMsg->GetOspEventID()).c_str(), pcOspSipMsg->GetOspEventID(),
        pcOspSipMsg->GetSipEventStr().c_str(), pcOspSipMsg->GetSipEventID());

    switch ( pcMsg->event )
    {
    case OSP_SIP_BYE_REQ:
        {
            TASKLOG(NORMAL_TASK, EVENT_LEV, "CInviteTask[TaskNO %u]收到InviteBye Req，DialogID[%u]，自动回复应答\n",
                GetTaskNO(), pcOspSipMsg->GetSipDlgID() );
            //将消息回调给InviteInterAckTask类
            if ( m_pcInviteInterActTask != NULL )
            {
                m_pcInviteInterActTask->ProcMsg(pcMsg);
            }

            if(pcOspSipMsg->GetMsgBodyLen() >0)
            {
                CByeReq cByeReq;
                pcOspSipMsg->GetMsgBody(cByeReq);
                NotifyStop2Cu(cByeReq.GetReason());
            }
            else
            {
                NotifyStop2Cu(ERR_CUI_DIALOG_TERMINATED);
            }

            string strNullMsg;
            PostRsp(KDSIP_EVENT_INVITE_BYE_RSP, pcOspSipMsg->GetSipTransID(), strNullMsg);
            m_tInviteID = INVALID_DIALOG_ID;
            NextState(NOT_REQUEST_STATE);
            delete this;
            return PROCMSG_DEL;
        }
        break;
    case OSP_SIP_MSG_PROC_FAIL:
    case OSP_SIP_DIALOG_TERMINATE:
        {
            //将消息回调给InviteInterAckTask类
            if ( m_pcInviteInterActTask != NULL )
            {
                m_pcInviteInterActTask->ProcMsg(pcMsg);
            }
            TASKLOG(NORMAL_TASK, ERROR_LEV, "销毁CInviteTask[TaskNO: %u]，结束Sip Dialog: [%u]\n",
                GetTaskNO(), m_tInviteID);
            delete this;
            return PROCMSG_DEL;
        }
        break;
    default:
        {
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"收到未知消息");
        }
        break;
    }
    return PROCMSG_OK;
}

u32 CInviteTaskEx::InviteHoldProcMsg( CMessage *const pcMsg )
{
    COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
    TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]收到业务应答消息[%s--%d] (SIP应答消息[%s--%d])\n",
        m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
        OspExtEventDesc(pcOspSipMsg->GetOspEventID()).c_str(), pcOspSipMsg->GetOspEventID(),
        pcOspSipMsg->GetSipEventStr().c_str(), pcOspSipMsg->GetSipEventID());

    u32 dwRet = PROCMSG_OK;
    switch ( pcMsg->event )
    {
	case CUI_NATDETECT_CALLBACK:
		{
			ProcNatCBMsg(pcMsg);
		}
		break;
	case UPDATE_SWITCH_ADDRESS_RSP:
		{
			CUpdateSwitchAddressRsp cRsp;
			pcOspSipMsg->GetMsgBody(cRsp);

			if (CMS_SUCCESS != cRsp.GetErrorCode())
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "CInviteTaskEx[TaskNO %u]更新交换地址失败，DialogID[%u],错误码[%d].\n",
					GetTaskNO(), pcOspSipMsg->GetSipDlgID(),cRsp.GetErrorCode());
			}
			else
			{
				TASKLOG(NORMAL_TASK, EVENT_LEV, "CInviteTaskEx[TaskNO %u]更新交换地址成功，DialogID[%u].\n",
					GetTaskNO(), pcOspSipMsg->GetSipDlgID());
			}
		}
		break;
    case OSP_SIP_BYE_REQ:
        {
            TASKLOG(NORMAL_TASK, EVENT_LEV, "CInviteTaskEx[TaskNO %u]收到InviteBye Req，DialogID[%u]，自动回复应答\n",
                GetTaskNO(), pcOspSipMsg->GetSipDlgID() );
            //将消息回调给InviteInterAckTask类
            if ( m_pcInviteInterActTask != NULL )
            {
                m_pcInviteInterActTask->ProcMsg(pcMsg);
            }

            if(pcOspSipMsg->GetMsgBodyLen() >0)
            {
                CByeReq cByeReq;
                pcOspSipMsg->GetMsgBody(cByeReq);
                NotifyStop2Cu(cByeReq.GetReason());
            }
            else
            {
                NotifyStop2Cu(ERR_CUI_DIALOG_TERMINATED);
            }

            string strNullMsg;
            PostRsp(KDSIP_EVENT_INVITE_BYE_RSP, pcOspSipMsg->GetSipTransID(), strNullMsg);
            m_tInviteID = INVALID_DIALOG_ID;
            NextState(NOT_REQUEST_STATE);
            delete this;
            dwRet = PROCMSG_DEL;
        }
        break;
    case OSP_SIP_MSG_PROC_FAIL:
    case OSP_SIP_DIALOG_TERMINATE:
        {
            //将消息回调给InviteInterAckTask类
            if ( m_pcInviteInterActTask != NULL )
            {
                m_pcInviteInterActTask->ProcMsg(pcMsg);
            }
            if (OSP_SIP_MSG_PROC_FAIL == pcMsg->event)
            {
                TASKLOG(NORMAL_TASK, ERROR_LEV, "recv OSP_SIP_MSG_PROC_FAIL, sipErrorCode=%d, invite req:%s\n", 
                    pcOspSipMsg->GetSipErrorCode(), m_tReq.ToXml().c_str());
                StartSendInviteBye();
            }

            TASKLOG(NORMAL_TASK, ERROR_LEV, "销毁CInviteTask[TaskNO: %u]，结束Sip Dialog: [%u]\n",
                GetTaskNO(), m_tInviteID);
            NextState(NOT_REQUEST_STATE);

            delete this;
            dwRet = PROCMSG_DEL;
        }
        break;
    case END_OF_FILE_NTF_REQ:
        {
            CEndOfFileNtfReq cReq;
            pcOspSipMsg->GetMsgBody(cReq);

            CEndOfFileNtfRsp cRsp;
            cRsp.SetHeadFromReq(cReq);

            PostRsp(KDSIP_EVENT_MESSAGE_DIALOG_RSP, pcOspSipMsg->GetSipTransID(), cRsp);

			if (!m_tNtfReq.GetIsPlayFinished())
			{
				NotifyStop2Cu(ERR_CUI_RECORD_END);
				StartSendInviteBye();
			}

        }
        break;
    case PLAYBACK_PROGRESS_NTF_REQ:
        {
            TASKLOG(NORMAL_TASK, EVENT_LEV, "CInviteTask[TaskNO %u]收到PlayBack Process Ntf Req，DialogID[%u]，自动回复应答\n",
                GetTaskNO(), pcOspSipMsg->GetSipDlgID() );
            CPlaybackProgressNtfReq cReq;
            pcOspSipMsg->GetMsgBody(cReq);
            CPlaybackProgressNtfRsp cRsp;
            cRsp.SetHeadFromReq(cReq);
            TASKLOG(NORMAL_TASK, EVENT_LEV, "PlayBack Process Ntf Req--CurTime[%d], TotalTime[%d]\n",
                cReq.GetCurPlayTime(), cReq.GetTotalPlayTime());
            PostRsp(KDSIP_EVENT_MESSAGE_DIALOG_RSP, pcOspSipMsg->GetSipTransID(), cRsp);
            m_tNtfReq = cReq;
            CCMSTime cStartTime(m_tReq.GetSdp().GetTimeRange().GetStartTime());
            m_tNtfReq.SetStartPlayTime((u32)cStartTime.GetTime());

			if (m_tNtfReq.GetIsPlayFinished())
			{
				NotifyStop2Cu(ERR_CUI_RECORD_END);
				StartSendInviteBye();
			}
        }
        break;
    case VCR_CTRL_RSP:
        {
            TASKLOG(NORMAL_TASK, EVENT_LEV, "CInviteTask[TaskNO %u]收到VCR Ctrl Req，DialogID[%u]，自动回复应答\n",
                GetTaskNO(), pcOspSipMsg->GetSipDlgID() );
            CTask* pcTask =FindChildTask(pcOspSipMsg->GetOspSeqNO());
            if(pcTask != NULL)
            {
                pcTask->ProcMsg(pcMsg);
            }

        }
        break;
    default:
        {
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"收到未知消息");
            dwRet = PROCMSG_FAIL;
        }
        break;
    }
    return dwRet;
}

u32 CInviteTaskEx::WaitSetupRspTimerPoll()
{
    if ( GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout() )
    {
        TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户[%s]会话[%s]等待InviteRsp超时，销毁CInviteTask[TaskNO: %u]\n",
            m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(), GetTaskNO());

        /*
        这里不用通知CInviteInterAction操作超时
        只是取消m_pcCuAgent->m_cInviteTaskSet中的登记
        等CInviteInterAction消息自己超时好了

        因为此时还未建立会话，所以也无需发送Bye，以及ReleaseDialog
        */
        return TIMERPOLL_DEL;
    }
    return TIMERPOLL_DONE;
}

u32 CInviteTaskEx::WaitAckTimerPoll()
{
    if ( GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout() * 2 )
    {
        TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户[%s]会话[%s]等待InviteAck超时，直接发送InviteBye结束Dlg\n",
            m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str());
        StartSendInviteBye();
        //return TIMERPOLL_DEL;
        return TIMERPOLL_DONE;  //发送ByeReq之后切换到WaitByeRsp状态了，不用删除
    }
    return TIMERPOLL_DONE;
}

s32 CInviteTaskEx::StartSendInviteAck()
{
    CMS_ASSERT(INVALID_DIALOG_ID != m_tInviteID);
    TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]通过TaskNO[%u]发送InviteACK请求, Sip DialogID为 %u\n",
        m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(), GetTaskNO(), m_tInviteID);

    CInviteACK cAck;
    cAck.SetSession(m_pcCuAgent->m_strSession);

#ifdef _UNIT_TEST_
    g_cTaskNOQue.push(GetTaskNO());
    cout << "Post InviteAck To " << m_pcCuAgent->m_tCmuURI.GetURIString().c_str() << endl;
    cout << cAck.ToXml().c_str() << endl;
#else
    PostInDlgReq<CInviteACK>(KDSIP_EVENT_INVITE_ACK, cAck, m_tInviteID);
#endif

    NextState(INVITE_HOLD_STATE);
    return 0;
}

s32 CInviteTaskEx::StartSendInviteBye()
{
    if ( m_tInviteID != INVALID_DIALOG_ID )
    {
        TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]通过TaskNO[%u]发送InviteBye请求, Sip DialogID为 %u\n",
            m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(), GetTaskNO(), m_tInviteID);
        string strNULL;
        PostInDlgReq(KDSIP_EVENT_INVITE_BYE_REQ, strNULL, m_tInviteID);
    }

    NextState(WAIT_BYE_RSP_STATE);
    //BYE Rsp由对端SIP协议栈直接回复，等待回复后delete task
    return PROCMSG_OK;
}

bool CInviteTaskEx::TryBye(CSipTask *pcActTask)
{
    if (GetState() != WAIT_SETUP_RSP_STATE)
    {
        m_pcInviteInterActTask = pcActTask;
        StartSendInviteBye();
        return true;
    }
    else
    {
        m_bByeWhileWaitInviteRsp = true;
        return false;
    }
}

s32 CInviteTaskEx::StartVcrCtrl(const CCuVcrCtrlReq&cCuReq)
{
    if( m_tInviteID != INVALID_DIALOG_ID)
    {
        TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]通过TaskNO[%u]发送VcrCtrl请求, Sip DialogID为 %u\n",
            m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(), GetTaskNO(), m_tInviteID);

        CVcrCtrlReq cReq;
        cReq.SetSeqNum(cCuReq.GetSeqNum());
        cReq.SetSession(m_pcCuAgent->m_strSession);
        cReq.SetPlaybackSessUri(m_tRsp.GetSdp().GetPlaybackSessUri());
        cReq.SetPlayRate(cCuReq.GetPlayRate());
        cReq.SetSeekTime(cCuReq.GetSeekTime());
        cReq.SetVcrCmd(cCuReq.GetVcrCmd());
        PostInDlgReq(KDSIP_EVENT_INFO_REQ, cReq, m_tInviteID);
    }

    return PROCMSG_OK;
}
CPlaybackProgressNtfReq &CInviteTaskEx::GetPlayBackProgressNtf()
{
    return m_tNtfReq;
}
const CPlaybackProgressNtfReq &CInviteTaskEx::GetPlayBackProgressNtf() const
{
    return m_tNtfReq;
}

void CInviteTaskEx::NotifyStop2Cu(s32 dwErrorCode)
{
    CInviteStopNotify cInviteStopNtf;
    cInviteStopNtf.playId = m_tReq.GetDstChn().GetChnNO();
    cInviteStopNtf.errorCode = dwErrorCode;
    m_pcCuAgent->m_cNotifyPool.InsertInviteStopNotify(cInviteStopNtf);
}

void CInviteTaskEx::SwitchToNatAddress(TSdp &tSdp)
{
	TMediaDescList& tMediaDescList = tSdp.GetMediaDescList();

	for(TMediaDescList::iterator itrMediaDesc = tMediaDescList.begin(); 
		itrMediaDesc!=tMediaDescList.end(); itrMediaDesc++)
	{
		//if (itrMediaDesc->GetMediaType() == MEDIA_TYPE_VIDEO)
		//{
			TMediaTransChannelList& tMediaTransChnList = itrMediaDesc->GetMediaTransChnList();
			for(TMediaTransChannelList::iterator itrTransChn = tMediaTransChnList.begin(); 
				itrTransChn!=tMediaTransChnList.end(); itrTransChn++)
			{
				if (itrTransChn->GetTransChnType() == MEDIA_TRANS_CHN_RTP)
				{
					TNetAddrList& tNatAddrList = itrTransChn->GetAddrList();
					for (TNetAddrList::iterator itrNatAddr = tNatAddrList.begin();
						itrNatAddr != tNatAddrList.end();itrNatAddr++)
					{
						ChangeToNatAddr(m_pcCuAgent->m_strSession,*itrNatAddr,m_nPlayId);
					}

					//交换需要地址伪装为NAT探测包地址
					itrTransChn->SetSendBindAddr(GetSendBindAddr());
				}
				else if (itrTransChn->GetTransChnType() == MEDIA_TRANS_CHN_RTCP)
				{
					TNetAddrList& tNatAddrList = itrTransChn->GetAddrList();
					for (TNetAddrList::iterator itrNatAddr = tNatAddrList.begin();
							itrNatAddr != tNatAddrList.end();itrNatAddr++)
					{
						ChangeToNatAddr(m_pcCuAgent->m_strSession,*itrNatAddr,m_nPlayId);
					}
				}
				else
				{
					TASKLOG(NORMAL_TASK, ERROR_LEV, "不支持的媒体类型[%s]\n",
						itrTransChn->GetTransChnType().c_str());
				}
			}
		//}
	}
}

TNetAddr CInviteTaskEx::GetSendBindAddr() const
{
	TNetAddr tAddr;
	if (m_pcCuAgent->m_ePlatNat == NATModeInner && g_cCuiConfig.IsEnableMapedNatAddr())     //平台在NAT内
	{
		//当含有二层NAT时，平台不能同时把一个端口（8001）映射到第一层和第二层
		//这里直接使用cu的登陆ip，即为平台在的最外层映射IP
		//tAddr.SetNetIp(g_cCuiConfig.GetMapedNatIP());
		tAddr.SetNetIp(m_pcCuAgent->m_strHostIp);
		tAddr.SetNetPort(g_cCuiConfig.GetMapedNatPort());
	}
	else                                             //平台在NAT外
	{
		tAddr.SetNetIp(m_pcCuAgent->m_tNatInfo.GetIpAddr());
		tAddr.SetNetPort(g_cCuiConfig.GetNatPort());
	}  

	TASKLOG(NORMAL_TASK, EVENT_LEV, "获取到的伪装地址[%s-%d]\n",tAddr.GetNetIp().c_str(),tAddr.GetNetPort());
	return tAddr;
}

//NAT探测包回调地址，如果地址有变化需要通知VTDU 更改交换地址
void CInviteTaskEx::ProcNatCBMsg(CMessage *const pcMsg)
{
	TNatPacketInfo tNatPacket = *(reinterpret_cast<TNatPacketInfo*>(pcMsg->content));

	TMediaDescList& tMediaDescList = m_tReq.GetSdp().GetMediaDescList();
	
	//nat探测包的reserve无效，走老的替换流程：源地址和交换地址匹配才替换nat地址；
	//reserve有效，走（sdk）新的替换流程：nat地址和当前交换地址不匹配就替换nat地址。
	TASKLOG(NORMAL_TASK, EVENT_LEV, "Reserver[%d]\n", tNatPacket.m_byReserve);
	if (tNatPacket.IsReserveInvalid())//老流程
	{
		for(TMediaDescList::iterator itrMediaDesc = tMediaDescList.begin(); 
			itrMediaDesc!=tMediaDescList.end(); itrMediaDesc++)
		{
			//		if (itrMediaDesc->GetMediaType() == MEDIA_TYPE_VIDEO)
			//		{
			TMediaTransChannelList& tMediaTransChnList = itrMediaDesc->GetMediaTransChnList();
			for(TMediaTransChannelList::iterator itrTransChn = tMediaTransChnList.begin(); 
				itrTransChn!=tMediaTransChnList.end(); itrTransChn++)
			{
				if (itrTransChn->GetTransChnType() == MEDIA_TRANS_CHN_RTP)
				{
					TNetAddrList& tNatAddrList = itrTransChn->GetAddrList();
					for (TNetAddrList::iterator itrNatAddr = tNatAddrList.begin();
						itrNatAddr != tNatAddrList.end();itrNatAddr++)
					{
						if (itrNatAddr->GetNetIpv4() == tNatPacket.m_dwSrcIp && itrNatAddr->GetNetPort() == tNatPacket.m_wSrcPort)
						{
							CUpdateSwitchAddressReq cReq;
							cReq.SetMediaType(itrMediaDesc->GetMediaType());
							cReq.SetTransChannel(MEDIA_TRANS_CHN_RTP);
							cReq.SetPreviousAddr(*itrNatAddr);
							cReq.SetPresentAddr(tNatPacket.m_dwNatIp,tNatPacket.m_wNatPort);

							PostInDlgReq<CUpdateSwitchAddressReq>(KDSIP_EVENT_MESSAGE_DIALOG_REQ, cReq, m_tInviteID);

							itrNatAddr->SetNetIpv4(tNatPacket.m_dwNatIp);
							itrNatAddr->SetNetPort(tNatPacket.m_wNatPort);
						}
					}
				}
				else if (itrTransChn->GetTransChnType() == MEDIA_TRANS_CHN_RTCP)
				{
					TNetAddrList& tNatAddrList = itrTransChn->GetAddrList();
					for (TNetAddrList::iterator itrNatAddr = tNatAddrList.begin();
						itrNatAddr != tNatAddrList.end();itrNatAddr++)
					{
						if (itrNatAddr->GetNetIpv4() == tNatPacket.m_dwSrcIp && itrNatAddr->GetNetPort() == tNatPacket.m_wSrcPort)
						{
							CUpdateSwitchAddressReq cReq;
							cReq.SetMediaType(itrMediaDesc->GetMediaType());
							cReq.SetTransChannel(MEDIA_TRANS_CHN_RTCP);
							cReq.SetPreviousAddr(*itrNatAddr);
							cReq.SetPresentAddr(tNatPacket.m_dwNatIp,tNatPacket.m_wNatPort);

							PostInDlgReq<CUpdateSwitchAddressReq>(KDSIP_EVENT_MESSAGE_DIALOG_REQ, cReq, m_tInviteID);

							itrNatAddr->SetNetIpv4(tNatPacket.m_dwNatIp);
							itrNatAddr->SetNetPort(tNatPacket.m_wNatPort);
						}
					}
				}
				else
				{
					TASKLOG(NORMAL_TASK, ERROR_LEV, "不支持的媒体类型[%s]\n",
						itrTransChn->GetTransChnType().c_str());
				}
			}
			//		}
		}
	} 
	else//新流程
	{
		for(TMediaDescList::iterator itrMediaDesc = tMediaDescList.begin(); 
			itrMediaDesc!=tMediaDescList.end(); itrMediaDesc++)
		{
			if (MEDIA_TYPE_VIDEO == itrMediaDesc->GetMediaType()
				&& (NAT_PACK_TYPE_VIDEO_RTP == tNatPacket.m_byReserve || NAT_PACK_TYPE_VIDEO_RTCP == tNatPacket.m_byReserve))
			{
				TMediaTransChannelList& tMediaTransChnList = itrMediaDesc->GetMediaTransChnList();
				for(TMediaTransChannelList::iterator itrTransChn = tMediaTransChnList.begin(); 
					itrTransChn!=tMediaTransChnList.end(); itrTransChn++)
				{
					if (MEDIA_TRANS_CHN_RTP == itrTransChn->GetTransChnType() && NAT_PACK_TYPE_VIDEO_RTP == tNatPacket.m_byReserve)
					{
						TNetAddrList& tNatAddrList = itrTransChn->GetAddrList();
						for (TNetAddrList::iterator itrNatAddr = tNatAddrList.begin();
							itrNatAddr != tNatAddrList.end();itrNatAddr++)
						{
							if (itrNatAddr->GetNetIpv4() != tNatPacket.m_dwNatIp || itrNatAddr->GetNetPort() != tNatPacket.m_wNatPort)
							{
								CUpdateSwitchAddressReq cReq;
								cReq.SetMediaType(itrMediaDesc->GetMediaType());
								cReq.SetTransChannel(MEDIA_TRANS_CHN_RTP);
								cReq.SetPreviousAddr(*itrNatAddr);
								cReq.SetPresentAddr(tNatPacket.m_dwNatIp,tNatPacket.m_wNatPort);

								PostInDlgReq<CUpdateSwitchAddressReq>(KDSIP_EVENT_MESSAGE_DIALOG_REQ, cReq, m_tInviteID);

								TASKLOG(NORMAL_TASK, EVENT_LEV, "Bef LocalAddr[%u:%d], NatAddr[%u:%d], NatPacketNatAddr[%u:%d]\n",
									tNatPacket.m_dwNatIp, tNatPacket.m_wSrcPort,
									itrNatAddr->GetNetIpv4(), itrNatAddr->GetNetPort(),
									tNatPacket.m_dwNatIp, tNatPacket.m_wNatPort);
								itrNatAddr->SetNetIpv4(tNatPacket.m_dwNatIp);
								itrNatAddr->SetNetPort(tNatPacket.m_wNatPort);
								TASKLOG(NORMAL_TASK, EVENT_LEV, "Aft NatAddr[%u:%d]\n",
									itrNatAddr->GetNetIpv4(), itrNatAddr->GetNetPort());
							}
						}
					}
					else if (MEDIA_TRANS_CHN_RTCP == itrTransChn->GetTransChnType() && NAT_PACK_TYPE_VIDEO_RTCP == tNatPacket.m_byReserve)
					{
						TNetAddrList& tNatAddrList = itrTransChn->GetAddrList();
						for (TNetAddrList::iterator itrNatAddr = tNatAddrList.begin();
							itrNatAddr != tNatAddrList.end();itrNatAddr++)
						{
							if (itrNatAddr->GetNetIpv4() != tNatPacket.m_dwNatIp || itrNatAddr->GetNetPort() != tNatPacket.m_wNatPort)
							{
								CUpdateSwitchAddressReq cReq;
								cReq.SetMediaType(itrMediaDesc->GetMediaType());
								cReq.SetTransChannel(MEDIA_TRANS_CHN_RTCP);
								cReq.SetPreviousAddr(*itrNatAddr);
								cReq.SetPresentAddr(tNatPacket.m_dwNatIp,tNatPacket.m_wNatPort);

								PostInDlgReq<CUpdateSwitchAddressReq>(KDSIP_EVENT_MESSAGE_DIALOG_REQ, cReq, m_tInviteID);

								TASKLOG(NORMAL_TASK, EVENT_LEV, "Bef LocalAddr[%u:%d], NatAddr[%u:%d], NatPacketNatAddr[%u:%d]\n",
									tNatPacket.m_dwNatIp, tNatPacket.m_wSrcPort,
									itrNatAddr->GetNetIpv4(), itrNatAddr->GetNetPort(),
									tNatPacket.m_dwNatIp, tNatPacket.m_wNatPort);
								itrNatAddr->SetNetIpv4(tNatPacket.m_dwNatIp);
								itrNatAddr->SetNetPort(tNatPacket.m_wNatPort);
								TASKLOG(NORMAL_TASK, EVENT_LEV, "Aft NatAddr[%u:%d]\n",
									itrNatAddr->GetNetIpv4(), itrNatAddr->GetNetPort());
							}
						}
					}
				}
			}
			else if (MEDIA_TYPE_AUDIO == itrMediaDesc->GetMediaType()
				&& (NAT_PACK_TYPE_AUDIO_RTP == tNatPacket.m_byReserve || NAT_PACK_TYPE_AUDIO_RTCP == tNatPacket.m_byReserve))
			{
				TMediaTransChannelList& tMediaTransChnList = itrMediaDesc->GetMediaTransChnList();
				for(TMediaTransChannelList::iterator itrTransChn = tMediaTransChnList.begin(); 
					itrTransChn!=tMediaTransChnList.end(); itrTransChn++)
				{
					if (MEDIA_TRANS_CHN_RTP == itrTransChn->GetTransChnType() && NAT_PACK_TYPE_AUDIO_RTP == tNatPacket.m_byReserve)
					{
						TNetAddrList& tNatAddrList = itrTransChn->GetAddrList();
						for (TNetAddrList::iterator itrNatAddr = tNatAddrList.begin();
							itrNatAddr != tNatAddrList.end();itrNatAddr++)
						{
							if (itrNatAddr->GetNetIpv4() != tNatPacket.m_dwNatIp || itrNatAddr->GetNetPort() != tNatPacket.m_wNatPort)
							{
								CUpdateSwitchAddressReq cReq;
								cReq.SetMediaType(itrMediaDesc->GetMediaType());
								cReq.SetTransChannel(MEDIA_TRANS_CHN_RTP);
								cReq.SetPreviousAddr(*itrNatAddr);
								cReq.SetPresentAddr(tNatPacket.m_dwNatIp,tNatPacket.m_wNatPort);

								PostInDlgReq<CUpdateSwitchAddressReq>(KDSIP_EVENT_MESSAGE_DIALOG_REQ, cReq, m_tInviteID);

								TASKLOG(NORMAL_TASK, EVENT_LEV, "Bef LocalAddr[%u:%d], NatAddr[%u:%d], NatPacketNatAddr[%u:%d]\n",
									tNatPacket.m_dwNatIp, tNatPacket.m_wSrcPort,
									itrNatAddr->GetNetIpv4(), itrNatAddr->GetNetPort(),
									tNatPacket.m_dwNatIp, tNatPacket.m_wNatPort);
								itrNatAddr->SetNetIpv4(tNatPacket.m_dwNatIp);
								itrNatAddr->SetNetPort(tNatPacket.m_wNatPort);
								TASKLOG(NORMAL_TASK, EVENT_LEV, "Aft NatAddr[%u:%d]\n",
									itrNatAddr->GetNetIpv4(), itrNatAddr->GetNetPort());
							}
						}
					}
					else if (MEDIA_TRANS_CHN_RTCP == itrTransChn->GetTransChnType() && NAT_PACK_TYPE_AUDIO_RTCP == tNatPacket.m_byReserve)
					{
						TNetAddrList& tNatAddrList = itrTransChn->GetAddrList();
						for (TNetAddrList::iterator itrNatAddr = tNatAddrList.begin();
							itrNatAddr != tNatAddrList.end();itrNatAddr++)
						{
							if (itrNatAddr->GetNetIpv4() != tNatPacket.m_dwNatIp || itrNatAddr->GetNetPort() != tNatPacket.m_wNatPort)
							{
								CUpdateSwitchAddressReq cReq;
								cReq.SetMediaType(itrMediaDesc->GetMediaType());
								cReq.SetTransChannel(MEDIA_TRANS_CHN_RTCP);
								cReq.SetPreviousAddr(*itrNatAddr);
								cReq.SetPresentAddr(tNatPacket.m_dwNatIp,tNatPacket.m_wNatPort);

								PostInDlgReq<CUpdateSwitchAddressReq>(KDSIP_EVENT_MESSAGE_DIALOG_REQ, cReq, m_tInviteID);

								TASKLOG(NORMAL_TASK, EVENT_LEV, "Bef LocalAddr[%u:%d], NatAddr[%u:%d], NatPacketNatAddr[%u:%d]\n",
									tNatPacket.m_dwNatIp, tNatPacket.m_wSrcPort,
									itrNatAddr->GetNetIpv4(), itrNatAddr->GetNetPort(),
									tNatPacket.m_dwNatIp, tNatPacket.m_wNatPort);
								itrNatAddr->SetNetIpv4(tNatPacket.m_dwNatIp);
								itrNatAddr->SetNetPort(tNatPacket.m_wNatPort);
								TASKLOG(NORMAL_TASK, EVENT_LEV, "Aft NatAddr[%u:%d]\n",
									itrNatAddr->GetNetIpv4(), itrNatAddr->GetNetPort());
							}
						}
					}
				}
			}
			else if (MEDIA_TYPE_DATA == itrMediaDesc->GetMediaType()
				&& (NAT_PACK_TYPE_DATA_RTP == tNatPacket.m_byReserve
				|| NAT_PACK_TYPE_DATA_RTCP == tNatPacket.m_byReserve
				|| NAT_PACK_TYPE_DATA_DATA == tNatPacket.m_byReserve))
			{
				TMediaTransChannelList& tMediaTransChnList = itrMediaDesc->GetMediaTransChnList();
				for(TMediaTransChannelList::iterator itrTransChn = tMediaTransChnList.begin(); 
					itrTransChn!=tMediaTransChnList.end(); itrTransChn++)
				{
					if (MEDIA_TRANS_CHN_RTP == itrTransChn->GetTransChnType() && NAT_PACK_TYPE_DATA_RTP == tNatPacket.m_byReserve)
					{
						TNetAddrList& tNatAddrList = itrTransChn->GetAddrList();
						for (TNetAddrList::iterator itrNatAddr = tNatAddrList.begin();
							itrNatAddr != tNatAddrList.end();itrNatAddr++)
						{
							if (itrNatAddr->GetNetIpv4() != tNatPacket.m_dwNatIp || itrNatAddr->GetNetPort() != tNatPacket.m_wNatPort)
							{
								CUpdateSwitchAddressReq cReq;
								cReq.SetMediaType(itrMediaDesc->GetMediaType());
								cReq.SetTransChannel(MEDIA_TRANS_CHN_RTP);
								cReq.SetPreviousAddr(*itrNatAddr);
								cReq.SetPresentAddr(tNatPacket.m_dwNatIp,tNatPacket.m_wNatPort);

								PostInDlgReq<CUpdateSwitchAddressReq>(KDSIP_EVENT_MESSAGE_DIALOG_REQ, cReq, m_tInviteID);

								TASKLOG(NORMAL_TASK, EVENT_LEV, "Bef LocalAddr[%u:%d], NatAddr[%u:%d], NatPacketNatAddr[%u:%d]\n",
									tNatPacket.m_dwNatIp, tNatPacket.m_wSrcPort,
									itrNatAddr->GetNetIpv4(), itrNatAddr->GetNetPort(),
									tNatPacket.m_dwNatIp, tNatPacket.m_wNatPort);
								itrNatAddr->SetNetIpv4(tNatPacket.m_dwNatIp);
								itrNatAddr->SetNetPort(tNatPacket.m_wNatPort);
								TASKLOG(NORMAL_TASK, EVENT_LEV, "Aft NatAddr[%u:%d]\n",
									itrNatAddr->GetNetIpv4(), itrNatAddr->GetNetPort());
							}
						}
					}
					else if (MEDIA_TRANS_CHN_RTCP == itrTransChn->GetTransChnType() && NAT_PACK_TYPE_DATA_RTCP == tNatPacket.m_byReserve)
					{
						TNetAddrList& tNatAddrList = itrTransChn->GetAddrList();
						for (TNetAddrList::iterator itrNatAddr = tNatAddrList.begin();
							itrNatAddr != tNatAddrList.end();itrNatAddr++)
						{
							if (itrNatAddr->GetNetIpv4() != tNatPacket.m_dwNatIp || itrNatAddr->GetNetPort() != tNatPacket.m_wNatPort)
							{
								CUpdateSwitchAddressReq cReq;
								cReq.SetMediaType(itrMediaDesc->GetMediaType());
								cReq.SetTransChannel(MEDIA_TRANS_CHN_RTCP);
								cReq.SetPreviousAddr(*itrNatAddr);
								cReq.SetPresentAddr(tNatPacket.m_dwNatIp,tNatPacket.m_wNatPort);

								PostInDlgReq<CUpdateSwitchAddressReq>(KDSIP_EVENT_MESSAGE_DIALOG_REQ, cReq, m_tInviteID);

								TASKLOG(NORMAL_TASK, EVENT_LEV, "Bef LocalAddr[%u:%d], NatAddr[%u:%d], NatPacketNatAddr[%u:%d]\n",
									tNatPacket.m_dwNatIp, tNatPacket.m_wSrcPort,
									itrNatAddr->GetNetIpv4(), itrNatAddr->GetNetPort(),
									tNatPacket.m_dwNatIp, tNatPacket.m_wNatPort);
								itrNatAddr->SetNetIpv4(tNatPacket.m_dwNatIp);
								itrNatAddr->SetNetPort(tNatPacket.m_wNatPort);
								TASKLOG(NORMAL_TASK, EVENT_LEV, "Aft NatAddr[%u:%d]\n",
									itrNatAddr->GetNetIpv4(), itrNatAddr->GetNetPort());
							}
						}
					}
					else if (MEDIA_TRANS_CHN_DATA == itrTransChn->GetTransChnType() && NAT_PACK_TYPE_DATA_DATA == tNatPacket.m_byReserve)
					{
						TNetAddrList& tNatAddrList = itrTransChn->GetAddrList();
						for (TNetAddrList::iterator itrNatAddr = tNatAddrList.begin();
							itrNatAddr != tNatAddrList.end();itrNatAddr++)
						{
							if (itrNatAddr->GetNetIpv4() != tNatPacket.m_dwNatIp || itrNatAddr->GetNetPort() != tNatPacket.m_wNatPort)
							{
								CUpdateSwitchAddressReq cReq;
								cReq.SetMediaType(itrMediaDesc->GetMediaType());
								cReq.SetTransChannel(MEDIA_TRANS_CHN_DATA);
								cReq.SetPreviousAddr(*itrNatAddr);
								cReq.SetPresentAddr(tNatPacket.m_dwNatIp,tNatPacket.m_wNatPort);

								PostInDlgReq<CUpdateSwitchAddressReq>(KDSIP_EVENT_MESSAGE_DIALOG_REQ, cReq, m_tInviteID);

								TASKLOG(NORMAL_TASK, EVENT_LEV, "Bef LocalAddr[%u:%d], NatAddr[%u:%d], NatPacketNatAddr[%u:%d]\n",
									tNatPacket.m_dwNatIp, tNatPacket.m_wSrcPort,
									itrNatAddr->GetNetIpv4(), itrNatAddr->GetNetPort(),
									tNatPacket.m_dwNatIp, tNatPacket.m_wNatPort);
								itrNatAddr->SetNetIpv4(tNatPacket.m_dwNatIp);
								itrNatAddr->SetNetPort(tNatPacket.m_wNatPort);
								TASKLOG(NORMAL_TASK, EVENT_LEV, "Aft NatAddr[%u:%d]\n",
									itrNatAddr->GetNetIpv4(), itrNatAddr->GetNetPort());
							}
						}
					}
				}
			}
		}
	}	
}

u32 CInviteTaskEx::WaitByeRspProcMsg( CMessage *const pcMsg )
{
    COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
    TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]收到业务应答消息[%s--%d] (SIP应答消息[%s--%d])\n",
        m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
        OspExtEventDesc(pcOspSipMsg->GetOspEventID()).c_str(), pcOspSipMsg->GetOspEventID(),
        pcOspSipMsg->GetSipEventStr().c_str(), pcOspSipMsg->GetSipEventID());
    u32 dwRet = PROCMSG_DEL;

    switch ( pcMsg->event )
    {
    case OSP_SIP_BYE_REQ:   
        {
            TASKLOG(NORMAL_TASK, EVENT_LEV, "CInviteTask[TaskNO %u]收到InviteBye Req，DialogID[%u]，回复应答后销毁CInviteTask\n",
                GetTaskNO(), pcOspSipMsg->GetSipDlgID() );
            //将消息回调给InviteInterAckTask类
            if ( m_pcInviteInterActTask != NULL )
            {
                m_pcInviteInterActTask->ProcMsg(pcMsg);
            }

            //             CInviteStopNotify cInviteStopNtf;
            //             cInviteStopNtf.playId = m_tCuChn.GetChnNO();
            //             cInviteStopNtf.errorCode = ERR_CUI_DIALOG_TERMINATED;
            //             m_pcCuAgent->m_cNotifyPool.InsertInviteStopNotify(cInviteStopNtf);

            string strNullMsg;
            PostRsp(KDSIP_EVENT_INVITE_BYE_RSP, pcOspSipMsg->GetSipTransID(), strNullMsg);
            //已经删除了OspSip的Dialog绑定，可以直接删除task
            m_tInviteID = INVALID_DIALOG_ID;
            ReleaseVtduResource();

            NextState(NOT_REQUEST_STATE);
            delete this;
        }
        break;
    case OSP_SIP_BYE_RSP:
        {
            if ( m_pcInviteInterActTask != NULL )
            {
                m_pcInviteInterActTask->ProcMsg(pcMsg);
            }

            TASKLOG(NORMAL_TASK, EVENT_LEV, "销毁CInviteTask[TaskNO: %u]，结束Sip Dialog: [%u]\n",
                GetTaskNO(), m_tInviteID);
            m_tInviteID = INVALID_DIALOG_ID;
            ReleaseVtduResource();

            NextState(NOT_REQUEST_STATE);
            delete this;
        }
        break;
    case OSP_SIP_MSG_PROC_FAIL:
    case OSP_SIP_DIALOG_TERMINATE:
        {
            //将消息回调给InviteInterAckTask类
            if ( m_pcInviteInterActTask != NULL )
            {
                m_pcInviteInterActTask->ProcMsg(pcMsg);
            }
            TASKLOG(NORMAL_TASK, ERROR_LEV, "销毁CInviteTask[TaskNO: %u]，结束Sip Dialog: [%u]\n",
                GetTaskNO(), m_tInviteID);
            //? 这块不能直接设置为无效
            //m_tInviteID = INVALID_DIALOG_ID;
            NextState(NOT_REQUEST_STATE);
            delete this;
        }
        break;
    case VCR_CTRL_RSP:
        {
            TASKLOG(NORMAL_TASK, EVENT_LEV, "CInviteTask[TaskNO %u]收到VCR Ctrl Req，DialogID[%u]，自动回复应答\n",
                GetTaskNO(), pcOspSipMsg->GetSipDlgID() );

            CTask* pcTask =FindChildTask(pcOspSipMsg->GetOspSeqNO());
            if(pcTask != NULL)
            {
                pcTask->ProcMsg(pcMsg);
            }
            dwRet = PROCMSG_OK;
        }
        break;
    case PLAYBACK_PROGRESS_NTF_REQ:
        {
            TASKLOG(NORMAL_TASK, EVENT_LEV, "CInviteTask[TaskNO %u]收到PlayBack Process Ntf Req，DialogID[%u]，自动回复应答\n",
                GetTaskNO(), pcOspSipMsg->GetSipDlgID() );
            CPlaybackProgressNtfReq cReq;
            pcOspSipMsg->GetMsgBody(cReq);
            CPlaybackProgressNtfRsp cRsp;
            cRsp.SetHeadFromReq(cReq);
            TASKLOG(NORMAL_TASK, EVENT_LEV, "PlayBack Process Ntf Req--CurTime[%d], TotalTime[%d]\n",
                cReq.GetCurPlayTime(), cReq.GetTotalPlayTime());
            PostRsp(KDSIP_EVENT_MESSAGE_DIALOG_RSP, pcOspSipMsg->GetSipTransID(), cRsp);
            m_tNtfReq = cReq;
            CCMSTime cStartTime(m_tReq.GetSdp().GetTimeRange().GetStartTime());
            m_tNtfReq.SetStartPlayTime((u32)cStartTime.GetTime());
            dwRet =  PROCMSG_OK;
        }
        break;
    default:
        {
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"收到未知消息");
            dwRet = PROCMSG_FAIL;
        }
        break;
    }
    return dwRet;
}

u32 CInviteTaskEx::WaitByeRspTimerPoll()
{
    if ( GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout() * 2 )
    {
        TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户[%s]会话[%s]等待Bye Rsp超时，结束Dialog\n",
            m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str());
        m_tInviteID = INVALID_DIALOG_ID;
        return TIMERPOLL_DEL;
    }
    return TIMERPOLL_DONE;
}

TChannel& CInviteTaskEx::GetDevChn()
{
    return m_tReq.GetSrcChn();
}

void CInviteTaskEx::ReleaseVtduResource()
{
    if( !m_strVtduResourceId.empty() )
    {
        g_cCuiRedisSdk.ReleaseVtduResource( m_strVtduResourceId );
        m_strVtduResourceId = "";
    }
}

void CInviteTaskEx::AddChildTask(int nSeqNo, int nTaskNo)
{
    if(nTaskNo == INVALID_TASKNO)
    {
        TASKLOG(ERROR_LEV, "TaskNo is Invalid");
        return;
    }
    m_cChildTaskList.Insert(nSeqNo, nTaskNo);
}
void CInviteTaskEx::DelChildTask(int nSeqNo)
{
    m_cChildTaskList.Erase(nSeqNo);
}
CTask* CInviteTaskEx::FindChildTask(int nSeqNo)
{
    int nTaskNo = INVALID_TASKNO;
    m_cChildTaskList.Find(nSeqNo, nTaskNo);
    return GetInstance()->FindTask(nTaskNo);
}

//////////////////////////////////////////////////////////////////////////
//CInviteTask
CInviteTask::CInviteTask(CInstExt *pcInst, CCuAgent *pcCuAgent, CSipTask *pcInviteInterActTask, s32 nPlayId) : CSipTask(pcInst), m_cStrmNumRestorer( pcInst )
{
    CMS_ASSERT(NULL != pcCuAgent);
    CMS_ASSERT(NULL != pcInviteInterActTask);
    m_pcCuAgent = pcCuAgent;
    m_pcInviteInterActTask = pcInviteInterActTask;
    m_tInviteID = INVALID_DIALOG_ID;
    m_bByeWhileWaitInviteRsp = false;
    m_strVtduId = "";

	m_nPlayId = nPlayId;
    m_cStrmNumRestorer.SetUserLoginSession( pcCuAgent->m_strSession );
}
CInviteTask::~CInviteTask()
{
    //释放选中的vtdu
    ReleaseVtduResource();
    if(!m_strVtduId.empty())
    {
        CRedisSsChannel cSsChn = g_cCuiRedisSdk.GetModuleSsChannelById(m_strVtduId);
		CRedisSsMng::GetInstance()->DelSs(cSsChn, GetTaskNO());
    }
    TerminateTask();
}

void CInviteTask::TerminateTask()
{
    Iterator pPos;
    int nSeqNo = 0;
    int nTaskNo = INVALID_TASKNO;
    while(!pPos.End())
    {
        if (m_cChildTaskList.Iterate(pPos, nSeqNo, nTaskNo) )
        {
            CTask* pcTask = NULL;
            pcTask = GetInstance()->FindTask(nTaskNo);
            if(pcTask != NULL)
            {
                delete pcTask;
                pcTask = NULL;
            }        
        }
    }
    if ( m_tInviteID != INVALID_DIALOG_ID )
    { //释放会话
		PostInDlgReq(KDSIP_EVENT_INVITE_BYE_REQ, CByeReq(), m_tInviteID);
        //OspSipReleaseDlg(m_tInviteID);
        m_tInviteID = INVALID_DIALOG_ID;
    }

    //注销CuAgent中的浏览事务
    if ( INVALID_PLAY_ID != m_tReq.GetDstChn().GetChnNO() )
    {
        m_pcCuAgent->m_cInviteTaskSet.Erase(m_tReq.GetDstChn().GetChnNO());
        m_tReq.GetDstChn().SetChnNO(INVALID_PLAY_ID);
    }

    m_pcInviteInterActTask = NULL;
}


bool CInviteTask::IsNotNeedPollState()
{
    if (GetState() == INVITE_HOLD_STATE)
    {
        return true;
    }

    return false;
}


void CInviteTask::InitStateMachine()
{
    CStateProc cNotReqState;
    cNotReqState.ProcMsg = (CSipTask::PFProcMsg)&CInviteTask::NotReqProcMsg;
    cNotReqState.TimerPoll = &CSipTask::TimerPollSkip;
    cNotReqState.ProcErrMsg = (CSipTask::PFProcMsg)&CInviteTask::NotReqProcMsg;
    AddRuleProc(NOT_REQUEST_STATE, cNotReqState);

    CStateProc cWaitSetupRspState;
    cWaitSetupRspState.ProcMsg = (CSipTask::PFProcMsg)&CInviteTask::WaitSetupRspProcMsg;
    cWaitSetupRspState.TimerPoll = (CSipTask::PFTimerPoll)&CInviteTask::WaitSetupRspTimerPoll;
    cWaitSetupRspState.ProcErrMsg = (CSipTask::PFProcMsg)&CInviteTask::WaitSetupRspProcMsg;
    AddRuleProc(WAIT_SETUP_RSP_STATE, cWaitSetupRspState);

    CStateProc cWaitSetupAckState;
    cWaitSetupAckState.ProcMsg = (CSipTask::PFProcMsg)&CInviteTask::WaitSetupAckProcMsg;
    cWaitSetupAckState.TimerPoll = (CSipTask::PFTimerPoll)&CInviteTask::WaitAckTimerPoll;
    cWaitSetupAckState.ProcErrMsg = (CSipTask::PFProcMsg)&CInviteTask::WaitSetupAckProcMsg;
    AddRuleProc(WAIT_SETUP_ACK_STATE, cWaitSetupAckState);

    CStateProc cInviteHoldState;
    cInviteHoldState.ProcMsg = (CSipTask::PFProcMsg)&CInviteTask::InviteHoldProcMsg;
    cInviteHoldState.TimerPoll = &CSipTask::TimerPollSkip;
    cInviteHoldState.ProcErrMsg = (CSipTask::PFProcMsg)&CInviteTask::InviteHoldProcMsg;
    AddRuleProc(INVITE_HOLD_STATE, cInviteHoldState);

    CStateProc cWaitByeRspState;
    cWaitByeRspState.ProcMsg = (CSipTask::PFProcMsg)&CInviteTask::WaitByeRspProcMsg;
    cWaitByeRspState.TimerPoll = (CSipTask::PFTimerPoll)&CInviteTask::WaitByeRspTimerPoll;
    cWaitByeRspState.ProcErrMsg = (CSipTask::PFProcMsg)&CInviteTask::WaitByeRspProcMsg;
    AddRuleProc(WAIT_BYE_RSP_STATE, cWaitByeRspState);

    NextState(NOT_REQUEST_STATE);
}

u32 CInviteTask::NotReqProcMsg( CMessage *const pcMsg )
{
    TASKLOG(NORMAL_TASK, CRITICAL_LEV, "在[CInviteTask::NotReqProcMsg]收到未知消息[%s--%hu]，不处理\n",
        OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
    CMS_ASSERT(0&&"CInviteTask::NotReqProcMsg收到未知消息");
    return PROCMSG_OK;
}

TNetAddr CInviteTask::GetSendBindAddr() const
{
	TNetAddr tAddr;
	if (m_pcCuAgent->m_ePlatNat == NATModeInner && g_cCuiConfig.IsEnableMapedNatAddr())     //平台在NAT内
	{
		//当含有二层NAT时，平台不能同时把一个端口（8001）映射到第一层和第二层
		//这里直接使用cu的登陆ip，即为平台在的最外层映射IP
		//tAddr.SetNetIp(g_cCuiConfig.GetMapedNatIP());
		tAddr.SetNetIp(m_pcCuAgent->m_strHostIp);
		tAddr.SetNetPort(g_cCuiConfig.GetMapedNatPort());
	}
	else                                             //平台在NAT外
	{
		tAddr.SetNetIp(m_pcCuAgent->m_tNatInfo.GetIpAddr());
		tAddr.SetNetPort(g_cCuiConfig.GetNatPort());
	}  

	TASKLOG(NORMAL_TASK, EVENT_LEV, "获取到的伪装地址[%s-%d]\n",tAddr.GetNetIp().c_str(),tAddr.GetNetPort());
	return tAddr;
}

void CInviteTask::SwitchToNatAddress(TSdp &tSdp)
{
	TMediaDescList& tMediaDescList = tSdp.GetMediaDescList();

	for(TMediaDescList::iterator itrMediaDesc = tMediaDescList.begin(); 
		itrMediaDesc!=tMediaDescList.end(); itrMediaDesc++)
	{
		//if (itrMediaDesc->GetMediaType() == MEDIA_TYPE_VIDEO)
		//{
		TMediaTransChannelList& tMediaTransChnList = itrMediaDesc->GetMediaTransChnList();
		for(TMediaTransChannelList::iterator itrTransChn = tMediaTransChnList.begin(); 
			itrTransChn!=tMediaTransChnList.end(); itrTransChn++)
		{
			if (itrTransChn->GetTransChnType() == MEDIA_TRANS_CHN_RTP)
			{
				TNetAddrList& tNatAddrList = itrTransChn->GetAddrList();
				for (TNetAddrList::iterator itrNatAddr = tNatAddrList.begin();
					itrNatAddr != tNatAddrList.end();itrNatAddr++)
				{
					ChangeToNatAddr(m_pcCuAgent->m_strSession,*itrNatAddr,m_nPlayId);
				}

				//交换需要地址伪装为NAT探测包地址
				itrTransChn->SetSendBindAddr(GetSendBindAddr());
			}
			else if (itrTransChn->GetTransChnType() == MEDIA_TRANS_CHN_RTCP)
			{
				TNetAddrList& tNatAddrList = itrTransChn->GetAddrList();
				for (TNetAddrList::iterator itrNatAddr = tNatAddrList.begin();
					itrNatAddr != tNatAddrList.end();itrNatAddr++)
				{
					ChangeToNatAddr(m_pcCuAgent->m_strSession,*itrNatAddr,m_nPlayId);
				}
			}
			else
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "不支持的媒体类型[%s]\n",
					itrTransChn->GetTransChnType().c_str());
			}
		}
		//}
	}
}

s32 CInviteTask::StartSendInviteReq( CInviteReq &cReq )
{
    CMS_ASSERT( NOT_REQUEST_STATE == GetState() );

    TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]发送Invite请求，建立源通道[%s--%d]到目的通道[%s--%d]的交换\n",
        m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
        cReq.GetSrcChn().GetDevUri().c_str(), cReq.GetSrcChn().GetChnNO(),
        cReq.GetDstChn().GetDevUri().c_str(), cReq.GetDstChn().GetChnNO());

    m_tReq = cReq;
    TASKLOG(NORMAL_TASK, EVENT_LEV, "CU PlayBack Process Ntf Req--StartTime[%s]\n",
        m_tReq.GetSdp().GetTimeRange().GetStartTime().c_str());
    //m_tCuChn = cReq.GetDstChn();
    //m_tDevChn = cReq.GetSrcChn();

	//cu在NAT內，更好碼流地址為其NAT外地址
	if (m_pcCuAgent->m_eClientNat == NATModeInner) 
	{
		SwitchToNatAddress(m_tReq.GetSdp());
	}

#ifdef _UNIT_TEST_
    g_cTaskNOQue.push(GetTaskNO());
    cout << "Post InviteReq To " << m_pcCuAgent->m_tCmuURI.GetURIString().c_str() << endl;
    cout << m_tReq.ToXml().c_str() << endl;
#else
	//改为直接发给VTDU
    //PostReq<CInviteReq>(KDSIP_EVENT_INVITE_REQ, cReq, m_pcCuAgent->m_tCmuURI);
	string strVtduId;
	string strDomainName = g_cCuiConfig.m_cCmCfg.GetDomainName();

    //tSwitchAgent.tId.tModuleType = RedisModuleType_Pui;

    TSipURI tDefaultUrl;
    tDefaultUrl = g_cCuiConfig.GetCmuURI();

    TSipURI tSrcUri;
    tSrcUri.SetURIString( m_tReq.GetSrcChn().GetDevUri() );
    TSipURI tDstUri;
    tDstUri.SetURIString( m_tReq.GetDstChn().GetDevUri() );

    TVtduResource tVtduRes;
    tVtduRes.tSrcAgent.nChannel = m_tReq.GetSrcChn().GetChnNO();
    tVtduRes.tSrcAgent.strId    = tSrcUri.GetUser();
    tVtduRes.tDstAgent.nChannel = m_tReq.GetDstChn().GetChnNO();
    tVtduRes.tDstAgent.strId    = tDstUri.GetUser();
    
    tVtduRes.switchType = CStreamDecentrationAdapter::SdpSessionNameToRedisSwitchType( m_tReq.GetSdp().GetSessionName() );
    bool bSelSuccessful = CStreamDecentrationAdapter::SelectVtduResouce( tVtduRes, tDefaultUrl );

	//ECMS_ERRORCODE eErrCode = g_cCuiRedisSdk.SelectVtdu( SdpSessionNameToRedisSwitchType( cReq.GetSdp().GetSessionName() ), 
 //                                                       tSwitchAgent, strVtduId, strDomainName, m_strVtduResourceId);
	TASKLOG(NORMAL_TASK, EVENT_LEV, "ErrCode[%d]\n", tVtduRes.err );
	
    if ( bSelSuccessful )//CMS_SUCCESS == eErrCode
	{		
        m_strVtduResourceId = tVtduRes.resId;
        m_strVtduId = tVtduRes.uri.GetUser();
        CRedisSsChannel cSsChn = g_cCuiRedisSdk.GetModuleSsChannelById(m_strVtduId);
		CRedisSsMng::GetInstance()->AddSs(cSsChn, GetTaskNO());



        m_tReq.GetSdp().SetResourceId( tVtduRes.resId );
        PostReq<CInviteReq>(KDSIP_EVENT_INVITE_REQ, m_tReq, tVtduRes.uri );
	}
	else
	{
		PostReq<CInviteReq>(KDSIP_EVENT_INVITE_REQ, m_tReq, m_pcCuAgent->m_tCmuURI);
	}
#endif

    NextState(WAIT_SETUP_RSP_STATE);
    return 0;
}

u32 CInviteTask::WaitSetupRspProcMsg( CMessage *const pcMsg )
{
    COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
    TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]收到业务应答消息[%s--%d] (SIP应答消息[%s--%d])\n",
        m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
        OspExtEventDesc(pcOspSipMsg->GetOspEventID()).c_str(), pcOspSipMsg->GetOspEventID(),
        pcOspSipMsg->GetSipEventStr().c_str(), pcOspSipMsg->GetSipEventID());

    switch ( pcMsg->event )
    {
    case OSP_SIP_BYE_REQ:   //其实这里不可能会收到Bye，因为呼叫还未建立，要么成功要么拒绝，收到403错误码
        {
            if ( m_pcInviteInterActTask != NULL )
            {
                m_pcInviteInterActTask->ProcMsg(pcMsg);
            }

            if(pcOspSipMsg->GetMsgBodyLen() >0)
            {
                CByeReq cByeReq;
                pcOspSipMsg->GetMsgBody(cByeReq);
                NotifyStop2Cu(cByeReq.GetReason());
            }
            else
            {
                NotifyStop2Cu(ERR_CUI_DIALOG_TERMINATED);
            }

            string strNullMsg;
            PostRsp(KDSIP_EVENT_INVITE_BYE_RSP, pcOspSipMsg->GetSipTransID(), strNullMsg);
            m_tInviteID = INVALID_DIALOG_ID;
            NextState(NOT_REQUEST_STATE);
            delete this;
            return PROCMSG_DEL;
            //CMS_ASSERT(0&&"不可能在WaitSetupRsp状态下收到OSP_SIP_BYE_REQ消息");
        }
        break;
    case OSP_SIP_MSG_PROC_FAIL:
    case OSP_SIP_DIALOG_TERMINATE:
        {
            //将消息回调给InviteInterAckTask类
            if ( m_pcInviteInterActTask != NULL )
            {
                m_pcInviteInterActTask->ProcMsg(pcMsg);
            }
            TASKLOG(NORMAL_TASK, ERROR_LEV, "收到InviteRsp失败消息，销毁CInviteTask[TaskNO: %u]\n",
                GetTaskNO());
            m_tInviteID = INVALID_DIALOG_ID;
            NextState(NOT_REQUEST_STATE);
            delete this;
            return PROCMSG_DEL;
        }
        break;
    case INVITE_RSP:
        {
            //判断是否业务层失败
            CInviteRsp cRsp;
            pcOspSipMsg->GetMsgBody(cRsp);

            //保存DlgID
            m_tInviteID = pcOspSipMsg->GetSipDlgID();
            m_tRsp = cRsp;

            if (CMS_SUCCESS == cRsp.GetErrorCode() && m_bByeWhileWaitInviteRsp)
            {
                cRsp.SetErrorCode(ERR_CUI_DIALOG_TERMINATED_BY_CMU);
            }

            //将消息回调给InviteInterAckTask类
            if ( m_pcInviteInterActTask != NULL )
            {
                m_pcInviteInterActTask->ProcMsg(pcMsg);
            }

            if ( INVALID_DIALOG_ID == m_tInviteID )
            {
                TASKLOG(NORMAL_TASK, ERROR_LEV, "收到的InviteRsp含带DialogID为 0 !!!!直接销毁CInviteTask[TaskNO %u]\n",
                    GetTaskNO());
                m_tInviteID = INVALID_DIALOG_ID;
                NextState(NOT_REQUEST_STATE);
                delete this;
                return PROCMSG_DEL;
            }

            if ( CMS_SUCCESS != cRsp.GetErrorCode() )
            {
                TASKLOG(NORMAL_TASK, ERROR_LEV, "Invite请求失败，错误码[%d]，销毁CInviteTask[TaskNO: %u]，结束Sip Dialog: [%u]\n",
                    cRsp.GetErrorCode(), GetTaskNO(), m_tInviteID);

                StartSendInviteBye();
                m_tInviteID = INVALID_DIALOG_ID;
                delete this;
                return PROCMSG_DEL;
                //OspSipReleaseDlg(m_tInviteID); 发送了Bye就不需要再主动释放
                //等待Bye应答之后再销毁本Task
                //delete this;
                //return PROCMSG_OK;
            }
            else
            {
                TASKLOG(NORMAL_TASK, EVENT_LEV, "Invite请求成功，将CInviteTask[TaskNO: %u, PlayId: %d]登记到CCuAgent的任务列表中\n",
                    GetTaskNO(), m_tReq.GetDstChn().GetChnNO());

                //调整录像时间
                CCMSTime cStartTime(m_tReq.GetSdp().GetTimeRange().GetStartTime());
                CCMSTime cEndTime(m_tReq.GetSdp().GetTimeRange().GetEndTime());

                m_tNtfReq.SetCurPlayTime((u32)cStartTime.GetTime());
                m_tNtfReq.SetStartPlayTime((u32)cStartTime.GetTime());
                if(cEndTime.GetTime() >= cStartTime.GetTime())
                {
                    m_tNtfReq.SetTotalPlayTime((u32)(cEndTime.GetTime() - cStartTime.GetTime()));
                }
                else
                {
                    m_tNtfReq.SetTotalPlayTime(0);
                }
                m_tNtfReq.SetIsPlayFinished(false);

                NextState(WAIT_SETUP_ACK_STATE);
                StartSendInviteAck();
            }
        }
        break;
    default:
        {
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"收到未知消息");
        }
        break;
    }
    return PROCMSG_OK;
}

u32 CInviteTask::WaitSetupAckProcMsg( CMessage *const pcMsg )
{
    COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
    TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]收到业务应答消息[%s--%d] (SIP应答消息[%s--%d])\n",
        m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
        OspExtEventDesc(pcOspSipMsg->GetOspEventID()).c_str(), pcOspSipMsg->GetOspEventID(),
        pcOspSipMsg->GetSipEventStr().c_str(), pcOspSipMsg->GetSipEventID());

    switch ( pcMsg->event )
    {
    case OSP_SIP_BYE_REQ:
        {
            TASKLOG(NORMAL_TASK, EVENT_LEV, "CInviteTask[TaskNO %u]收到InviteBye Req，DialogID[%u]，自动回复应答\n",
                GetTaskNO(), pcOspSipMsg->GetSipDlgID() );
            //将消息回调给InviteInterAckTask类
            if ( m_pcInviteInterActTask != NULL )
            {
                m_pcInviteInterActTask->ProcMsg(pcMsg);
            }

            if(pcOspSipMsg->GetMsgBodyLen() >0)
            {
                CByeReq cByeReq;
                pcOspSipMsg->GetMsgBody(cByeReq);
                NotifyStop2Cu(cByeReq.GetReason());
            }
            else
            {
                NotifyStop2Cu(ERR_CUI_DIALOG_TERMINATED);
            }

            string strNullMsg;
            PostRsp(KDSIP_EVENT_INVITE_BYE_RSP, pcOspSipMsg->GetSipTransID(), strNullMsg);
            m_tInviteID = INVALID_DIALOG_ID;
            NextState(NOT_REQUEST_STATE);
            delete this;
            return PROCMSG_DEL;
        }
        break;
    case OSP_SIP_MSG_PROC_FAIL:
    case OSP_SIP_DIALOG_TERMINATE:
        {
            //将消息回调给InviteInterAckTask类
            if ( m_pcInviteInterActTask != NULL )
            {
                m_pcInviteInterActTask->ProcMsg(pcMsg);
            }
            TASKLOG(NORMAL_TASK, ERROR_LEV, "销毁CInviteTask[TaskNO: %u]，结束Sip Dialog: [%u]\n",
                GetTaskNO(), m_tInviteID);
            delete this;
            return PROCMSG_DEL;
        }
        break;
    default:
        {
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"收到未知消息");
        }
        break;
    }
    return PROCMSG_OK;
}

u32 CInviteTask::InviteHoldProcMsg( CMessage *const pcMsg )
{
    COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
    TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]收到业务应答消息[%s--%d] (SIP应答消息[%s--%d])\n",
        m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
        OspExtEventDesc(pcOspSipMsg->GetOspEventID()).c_str(), pcOspSipMsg->GetOspEventID(),
        pcOspSipMsg->GetSipEventStr().c_str(), pcOspSipMsg->GetSipEventID());

    u32 dwRet = PROCMSG_OK;
    switch ( pcMsg->event )
    {
    case OSP_SIP_BYE_REQ:
        {
            TASKLOG(NORMAL_TASK, EVENT_LEV, "CInviteTask[TaskNO %u]收到InviteBye Req，DialogID[%u]，自动回复应答\n",
                GetTaskNO(), pcOspSipMsg->GetSipDlgID() );
            //将消息回调给InviteInterAckTask类
            if ( m_pcInviteInterActTask != NULL )
            {
                m_pcInviteInterActTask->ProcMsg(pcMsg);
            }

            if(pcOspSipMsg->GetMsgBodyLen() >0)
            {
                CByeReq cByeReq;
                pcOspSipMsg->GetMsgBody(cByeReq);
                NotifyStop2Cu(cByeReq.GetReason());
            }
            else
            {
                NotifyStop2Cu(ERR_CUI_DIALOG_TERMINATED);
            }

            string strNullMsg;
            PostRsp(KDSIP_EVENT_INVITE_BYE_RSP, pcOspSipMsg->GetSipTransID(), strNullMsg);
            m_tInviteID = INVALID_DIALOG_ID;
            NextState(NOT_REQUEST_STATE);
            delete this;
            dwRet = PROCMSG_DEL;
        }
        break;
    case OSP_SIP_MSG_PROC_FAIL:
    case OSP_SIP_DIALOG_TERMINATE:
        {
            //将消息回调给InviteInterAckTask类
            if ( m_pcInviteInterActTask != NULL )
            {
                m_pcInviteInterActTask->ProcMsg(pcMsg);
            }
            if (OSP_SIP_MSG_PROC_FAIL == pcMsg->event)
            {
                TASKLOG(NORMAL_TASK, ERROR_LEV, "recv OSP_SIP_MSG_PROC_FAIL, sipErrorCode=%d, invite req:%s\n", 
                    pcOspSipMsg->GetSipErrorCode(), m_tReq.ToXml().c_str());
                StartSendInviteBye();
            }

            TASKLOG(NORMAL_TASK, ERROR_LEV, "销毁CInviteTask[TaskNO: %u]，结束Sip Dialog: [%u]\n",
                GetTaskNO(), m_tInviteID);
            NextState(NOT_REQUEST_STATE);

            delete this;
            dwRet = PROCMSG_DEL;
        }
        break;
    case END_OF_FILE_NTF_REQ:
        {
            CEndOfFileNtfReq cReq;
            pcOspSipMsg->GetMsgBody(cReq);

            CEndOfFileNtfRsp cRsp;
            cRsp.SetHeadFromReq(cReq);

            PostRsp(KDSIP_EVENT_MESSAGE_DIALOG_RSP, pcOspSipMsg->GetSipTransID(), cRsp);

			if (!m_tNtfReq.GetIsPlayFinished())
			{
				NotifyStop2Cu(ERR_CUI_RECORD_END);
				StartSendInviteBye();
			}
        }
        break;
    case PLAYBACK_PROGRESS_NTF_REQ:
        {
            TASKLOG(NORMAL_TASK, EVENT_LEV, "CInviteTask[TaskNO %u]收到PlayBack Process Ntf Req，DialogID[%u]，自动回复应答\n",
                GetTaskNO(), pcOspSipMsg->GetSipDlgID() );
            CPlaybackProgressNtfReq cReq;
            pcOspSipMsg->GetMsgBody(cReq);
            CPlaybackProgressNtfRsp cRsp;
            cRsp.SetHeadFromReq(cReq);
            TASKLOG(NORMAL_TASK, EVENT_LEV, "PlayBack Process Ntf Req--CurTime[%d], TotalTime[%d]\n",
                cReq.GetCurPlayTime(), cReq.GetTotalPlayTime());
            PostRsp(KDSIP_EVENT_MESSAGE_DIALOG_RSP, pcOspSipMsg->GetSipTransID(), cRsp);
            m_tNtfReq = cReq;
            CCMSTime cStartTime(m_tReq.GetSdp().GetTimeRange().GetStartTime());
            m_tNtfReq.SetStartPlayTime((u32)cStartTime.GetTime());

			if (m_tNtfReq.GetIsPlayFinished())
			{
				NotifyStop2Cu(ERR_CUI_RECORD_END);
				StartSendInviteBye();
			}
        }
        break;
    case VCR_CTRL_RSP:
        {
            TASKLOG(NORMAL_TASK, EVENT_LEV, "CInviteTask[TaskNO %u]收到VCR Ctrl Req，DialogID[%u]，自动回复应答\n",
                GetTaskNO(), pcOspSipMsg->GetSipDlgID() );
            CTask* pcTask =FindChildTask(pcOspSipMsg->GetOspSeqNO());
            if(pcTask != NULL)
            {
                pcTask->ProcMsg(pcMsg);
            }

        }
        break;
    default:
        {
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"收到未知消息");
            dwRet = PROCMSG_FAIL;
        }
        break;
    }
    return dwRet;
}

u32 CInviteTask::WaitSetupRspTimerPoll()
{
    if ( GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout() )
    {
        TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户[%s]会话[%s]等待InviteRsp超时，销毁CInviteTask[TaskNO: %u]\n",
            m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(), GetTaskNO());

        /*
        这里不用通知CInviteInterAction操作超时
        只是取消m_pcCuAgent->m_cInviteTaskSet中的登记
        等CInviteInterAction消息自己超时好了

        因为此时还未建立会话，所以也无需发送Bye，以及ReleaseDialog
        */
        return TIMERPOLL_DEL;
    }
    return TIMERPOLL_DONE;
}

u32 CInviteTask::WaitAckTimerPoll()
{
    if ( GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout() * 2 )
    {
        TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户[%s]会话[%s]等待InviteAck超时，直接发送InviteBye结束Dlg\n",
            m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str());
        StartSendInviteBye();
        //return TIMERPOLL_DEL;
        return TIMERPOLL_DONE;  //发送ByeReq之后切换到WaitByeRsp状态了，不用删除
    }
    return TIMERPOLL_DONE;
}

s32 CInviteTask::StartSendInviteAck()
{
    CMS_ASSERT(INVALID_DIALOG_ID != m_tInviteID);
    TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]通过TaskNO[%u]发送InviteACK请求, Sip DialogID为 %u\n",
        m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(), GetTaskNO(), m_tInviteID);

    CInviteACK cAck;
    cAck.SetSession(m_pcCuAgent->m_strSession);

#ifdef _UNIT_TEST_
    g_cTaskNOQue.push(GetTaskNO());
    cout << "Post InviteAck To " << m_pcCuAgent->m_tCmuURI.GetURIString().c_str() << endl;
    cout << cAck.ToXml().c_str() << endl;
#else
    PostInDlgReq<CInviteACK>(KDSIP_EVENT_INVITE_ACK, cAck, m_tInviteID);
#endif

    NextState(INVITE_HOLD_STATE);
    return 0;
}

s32 CInviteTask::StartSendInviteBye()
{
    if ( m_tInviteID != INVALID_DIALOG_ID )
    {
        TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]通过TaskNO[%u]发送InviteBye请求, Sip DialogID为 %u\n",
            m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(), GetTaskNO(), m_tInviteID);
        string strNULL;

        CByeReq cReq;
        cReq.SetReason( ERR_CMU_SWITCH_STOP_BY_BYE );
        PostInDlgReq(KDSIP_EVENT_INVITE_BYE_REQ, cReq, m_tInviteID);
    }

    NextState(WAIT_BYE_RSP_STATE);
    //BYE Rsp由对端SIP协议栈直接回复，等待回复后delete task
    return PROCMSG_OK;
}

bool CInviteTask::TryBye(CSipTask *pcActTask)
{
    if (GetState() != WAIT_SETUP_RSP_STATE)
    {
        m_pcInviteInterActTask = pcActTask;
        StartSendInviteBye();
        return true;
    }
    else
    {
        m_bByeWhileWaitInviteRsp = true;
        return false;
    }
}

s32 CInviteTask::StartVcrCtrl(const CCuVcrCtrlReq&cCuReq)
{
    if( m_tInviteID != INVALID_DIALOG_ID)
    {
        TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]通过TaskNO[%u]发送VcrCtrl请求, Sip DialogID为 %u\n",
            m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(), GetTaskNO(), m_tInviteID);

        CVcrCtrlReq cReq;
        cReq.SetSeqNum(cCuReq.GetSeqNum());
        cReq.SetSession(m_pcCuAgent->m_strSession);
        cReq.SetPlaybackSessUri(m_tRsp.GetSdp().GetPlaybackSessUri());
        cReq.SetPlayRate(cCuReq.GetPlayRate());
        cReq.SetSeekTime(cCuReq.GetSeekTime());
        cReq.SetVcrCmd(cCuReq.GetVcrCmd());
        PostInDlgReq(KDSIP_EVENT_INFO_REQ, cReq, m_tInviteID);
    }

    return PROCMSG_OK;
}
CPlaybackProgressNtfReq &CInviteTask::GetPlayBackProgressNtf()
{
    return m_tNtfReq;
}
const CPlaybackProgressNtfReq &CInviteTask::GetPlayBackProgressNtf() const
{
    return m_tNtfReq;
}

void CInviteTask::NotifyStop2Cu(s32 dwErrorCode)
{
    CInviteStopNotify cInviteStopNtf;
    cInviteStopNtf.playId = m_tReq.GetDstChn().GetChnNO();
    cInviteStopNtf.errorCode = dwErrorCode;
    m_pcCuAgent->m_cNotifyPool.InsertInviteStopNotify(cInviteStopNtf);
}

u32 CInviteTask::WaitByeRspProcMsg( CMessage *const pcMsg )
{
    COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
    TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]收到业务应答消息[%s--%d] (SIP应答消息[%s--%d])\n",
        m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
        OspExtEventDesc(pcOspSipMsg->GetOspEventID()).c_str(), pcOspSipMsg->GetOspEventID(),
        pcOspSipMsg->GetSipEventStr().c_str(), pcOspSipMsg->GetSipEventID());
    u32 dwRet = PROCMSG_DEL;

    switch ( pcMsg->event )
    {
    case OSP_SIP_BYE_REQ:   
        {
            TASKLOG(NORMAL_TASK, EVENT_LEV, "CInviteTask[TaskNO %u]收到InviteBye Req，DialogID[%u]，回复应答后销毁CInviteTask\n",
                GetTaskNO(), pcOspSipMsg->GetSipDlgID() );
            //将消息回调给InviteInterAckTask类
            if ( m_pcInviteInterActTask != NULL )
            {
                m_pcInviteInterActTask->ProcMsg(pcMsg);
            }

            //             CInviteStopNotify cInviteStopNtf;
            //             cInviteStopNtf.playId = m_tCuChn.GetChnNO();
            //             cInviteStopNtf.errorCode = ERR_CUI_DIALOG_TERMINATED;
            //             m_pcCuAgent->m_cNotifyPool.InsertInviteStopNotify(cInviteStopNtf);

            string strNullMsg;
            PostRsp(KDSIP_EVENT_INVITE_BYE_RSP, pcOspSipMsg->GetSipTransID(), strNullMsg);
            //已经删除了OspSip的Dialog绑定，可以直接删除task
            m_tInviteID = INVALID_DIALOG_ID;
            ReleaseVtduResource();
            NextState(NOT_REQUEST_STATE);
            delete this;
        }
        break;
    case OSP_SIP_BYE_RSP:
        {
            if ( m_pcInviteInterActTask != NULL )
            {
                m_pcInviteInterActTask->ProcMsg(pcMsg);
            }

            TASKLOG(NORMAL_TASK, EVENT_LEV, "销毁CInviteTask[TaskNO: %u]，结束Sip Dialog: [%u]\n",
                GetTaskNO(), m_tInviteID);
            m_tInviteID = INVALID_DIALOG_ID;

            ReleaseVtduResource();

            NextState(NOT_REQUEST_STATE);
            delete this;
        }
        break;
    case OSP_SIP_MSG_PROC_FAIL:
    case OSP_SIP_DIALOG_TERMINATE:
        {
            //将消息回调给InviteInterAckTask类
            if ( m_pcInviteInterActTask != NULL )
            {
                m_pcInviteInterActTask->ProcMsg(pcMsg);
            }
            TASKLOG(NORMAL_TASK, ERROR_LEV, "销毁CInviteTask[TaskNO: %u]，结束Sip Dialog: [%u]\n",
                GetTaskNO(), m_tInviteID);
            //? 这块不能直接设置为无效
            //m_tInviteID = INVALID_DIALOG_ID;
            NextState(NOT_REQUEST_STATE);
            delete this;
        }
        break;
    case VCR_CTRL_RSP:
        {
            TASKLOG(NORMAL_TASK, EVENT_LEV, "CInviteTask[TaskNO %u]收到VCR Ctrl Req，DialogID[%u]，自动回复应答\n",
                GetTaskNO(), pcOspSipMsg->GetSipDlgID() );

            CTask* pcTask =FindChildTask(pcOspSipMsg->GetOspSeqNO());
            if(pcTask != NULL)
            {
                pcTask->ProcMsg(pcMsg);
            }
            dwRet = PROCMSG_OK;
        }
        break;
    case PLAYBACK_PROGRESS_NTF_REQ:
        {
            TASKLOG(NORMAL_TASK, EVENT_LEV, "CInviteTask[TaskNO %u]收到PlayBack Process Ntf Req，DialogID[%u]，自动回复应答\n",
                GetTaskNO(), pcOspSipMsg->GetSipDlgID() );
            CPlaybackProgressNtfReq cReq;
            pcOspSipMsg->GetMsgBody(cReq);
            CPlaybackProgressNtfRsp cRsp;
            cRsp.SetHeadFromReq(cReq);
            TASKLOG(NORMAL_TASK, EVENT_LEV, "PlayBack Process Ntf Req--CurTime[%d], TotalTime[%d]\n",
                cReq.GetCurPlayTime(), cReq.GetTotalPlayTime());
            PostRsp(KDSIP_EVENT_MESSAGE_DIALOG_RSP, pcOspSipMsg->GetSipTransID(), cRsp);
            m_tNtfReq = cReq;
            CCMSTime cStartTime(m_tReq.GetSdp().GetTimeRange().GetStartTime());
            m_tNtfReq.SetStartPlayTime((u32)cStartTime.GetTime());
            dwRet =  PROCMSG_OK;
        }
        break;
    default:
        {
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"收到未知消息");
            dwRet = PROCMSG_FAIL;
        }
        break;
    }
    return dwRet;
}

u32 CInviteTask::WaitByeRspTimerPoll()
{
    if ( GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout() * 2 )
    {
        TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户[%s]会话[%s]等待Bye Rsp超时，结束Dialog\n",
            m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str());
        m_tInviteID = INVALID_DIALOG_ID;
        return TIMERPOLL_DEL;
    }
    return TIMERPOLL_DONE;
}

TChannel& CInviteTask::GetDevChn()
{
    return m_tReq.GetSrcChn();
}

void CInviteTask::AddChildTask(int nSeqNo, int nTaskNo)
{
    if(nTaskNo == INVALID_TASKNO)
    {
        TASKLOG(ERROR_LEV, "TaskNo is Invalid");
        return;
    }
    m_cChildTaskList.Insert(nSeqNo, nTaskNo);
}
void CInviteTask::DelChildTask(int nSeqNo)
{
    m_cChildTaskList.Erase(nSeqNo);
}
CTask* CInviteTask::FindChildTask(int nSeqNo)
{
    int nTaskNo = INVALID_TASKNO;
    m_cChildTaskList.Find(nSeqNo, nTaskNo);
    return GetInstance()->FindTask(nTaskNo);
}


void CInviteTask::ReleaseVtduResource()
{
    if( !m_strVtduResourceId.empty() )
    {
        g_cCuiRedisSdk.ReleaseVtduResource( m_strVtduResourceId );
        m_strVtduResourceId = "";
    }
}


CInviteInterAction::CInviteInterAction( CInstExt *pcInst, CCuAgent *pcCuAgent, PTRecvNode ptRecvNode ) : CSipTask(pcInst)
{
    CMS_ASSERT(pcCuAgent != NULL);
    CMS_ASSERT(ptRecvNode != NULL);
    m_pcCuAgent = pcCuAgent;
    m_ptRecvNode = ptRecvNode;
    m_nplayId = INVALID_PLAY_ID;
    m_pcCuAgent->AddSubTask(this);
    m_dwEvent = CU_CUI_INVITE_ACK_REQ;
    m_dwSeqNo = INVALID_SEQ_NO;
}

void CInviteInterAction::TerminateTask()
{
    CMS_ASSERT(NULL != m_pcCuAgent);

	if ( m_pcCuAgent == NULL )
	{
		return;
	}

    m_pcCuAgent->DelSubTask(this);
    if ( GetState() == WAIT_RESPONSE_STATE && m_ptRecvNode != NULL )
    {
        CInviteRsp cRsp;
        cRsp.SetErrorCode(ERR_CUI_TASK_DEL);

        TASKLOG(NORMAL_TASK, ERROR_LEV, "向CU用户[%s]会话[%s]发送InviteReq，交互任务CInviteInterAction被销毁，向CU发送应答，错误码[%d]\n",
            m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(), cRsp.GetErrorCode());
        SendRspToCu<CCuSetupStreamRsp>(cRsp, m_ptRecvNode);
    }

    //将对应的InviteTask的绑定解除
    if ( INVALID_PLAY_ID != m_nplayId )
    {
		if ( m_pcCuAgent->CuiTransStream4Pu() )
		{
			CInviteTaskNatSwitch *pcInviteTask = (CInviteTaskNatSwitch*)GetInviteTask(m_nplayId);
			if ( NULL != pcInviteTask )
			{
				if ( pcInviteTask->m_pcInviteInterActTask == this )
				{
					pcInviteTask->m_pcInviteInterActTask = NULL;
				}
				if (m_dwSeqNo != INVALID_SEQ_NO)
				{
					pcInviteTask->DelChildTask(m_dwSeqNo);
				}
			}
		} 
		else
		{
			CInviteTaskEx *pcInviteTask = (CInviteTaskEx*)GetInviteTask(m_nplayId);
			if ( NULL != pcInviteTask )
			{
				if ( pcInviteTask->m_pcInviteInterActTask == this )
				{
					pcInviteTask->m_pcInviteInterActTask = NULL;
				}
				if (m_dwSeqNo != INVALID_SEQ_NO)
				{
					pcInviteTask->DelChildTask(m_dwSeqNo);
				}
			}
		}
    }
}

void CInviteInterAction::InitStateMachine()
{
    CStateProc cNotRequestProc;
    cNotRequestProc.ProcMsg = (CSipTask::PFProcMsg)&CInviteInterAction::NotReqProcMsg;
    cNotRequestProc.TimerPoll = &CSipTask::TimerPollSkip;
    cNotRequestProc.ProcErrMsg = (CSipTask::PFProcMsg)&CInviteInterAction::NotReqProcMsg;
    AddRuleProc(NOT_REQUEST_STATE, cNotRequestProc);

    CStateProc cWaitRspProc;
    cWaitRspProc.ProcMsg = (CSipTask::PFProcMsg)&CInviteInterAction::WaitRspProcMsg;
    cWaitRspProc.TimerPoll = (CSipTask::PFTimerPoll)&CInviteInterAction::WaitRspTimerPoll;
    cWaitRspProc.ProcErrMsg = (CSipTask::PFProcMsg)&CInviteInterAction::WaitRspProcMsg;
    AddRuleProc(WAIT_RESPONSE_STATE, cWaitRspProc);

    NextState(NOT_REQUEST_STATE);
}

u32 CInviteInterAction::NotReqProcMsg( CMessage *const pcMsg )
{
    m_dwEvent = pcMsg->event;
    switch (pcMsg->event)
    {
    case INVITE_REQ:  // 浏览
    case CU_CUI_INVITE_PLAY_DOWNLOADPURECORD_REQ: // 前端录像下载
    case CU_CUI_INVITE_PLAY_PURECORD_REQ: // 前端放像
        {
            CCuSetupStreamReq *pcReq = (CCuSetupStreamReq *)pcMsg->content;
            pcReq->SetEvent(INVITE_REQ);
			//入口处就将monitorid加进去，后续就都有了
			pcReq->SetUserMonitorId(m_pcCuAgent->m_pcUserAccount->m_tUserID);

			bool bSupportNA = false;/*m_pcCuAgent->m_bIsSupportNA;*/

			/*if (INVITE_REQ != pcMsg->event) 浏览不填这个字段？*/
			pcReq->GetSdp().SetIsNeedPunch(bSupportNA);
            m_nplayId = pcReq->GetDstChn().GetChnNO();
            m_tDevChn = pcReq->GetSrcChn();
            m_strReq = pcReq->ToXml();
            //1 鉴权
            bool bAuthRet = true; //鉴权结果
            do 
            {//hzytodo2 不知道为什么会加上
//                 string strDomainName = GetDomainNameWithoutPreNameFromURI(pcReq->GetSrcChn().GetDevUri());
//                 if ( ! m_pcCuAgent->IsExistDomainInfo(strDomainName))
//                 {
//                     bAuthRet = false;
//                     break;
//                 }

				if (m_pcCuAgent->m_pcUserAccount->IsSuperAdmin())
                {
                    bAuthRet = true;
                    break;
                }

                string uuid = GetNameWithoutDomainFromURI(pcReq->GetSrcChn().GetDevUri());

				CVideosourcePrivilege cPri;
				string errMsg;
				EFDB_ErrorCode er = g_cRedisDb.GetUserVideoPriByEncChn(m_pcCuAgent->m_pcUserAccount->m_tUserID, uuid, pcReq->GetSrcChn().GetChnNO(), cPri, errMsg);
				if ( !IsFdbResultPositive(er) )
				{
					TASKLOG(NORMAL_TASK, ERROR_LEV, "用户name[%s] id[%s]没有视频源[%s]权限 er[%d] errmsg[%s]\n",
						m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_pcUserAccount->m_tUserID.c_str(), pcReq->GetSrcChn().ToXml().c_str(), er, errMsg.c_str());
					bAuthRet = false;
					break;
				}

                if (CU_CUI_INVITE_PLAY_PURECORD_REQ == pcMsg->event)
                {
					CEncoderPrivilege encPri;
					er = g_cRedisDb.GetUserEncoderPrivilege(m_pcCuAgent->m_pcUserAccount->m_tUserID, uuid, encPri);
					if ( !IsFdbResultPositive(er) )
					{
						TASKLOG(NORMAL_TASK, ERROR_LEV, "用户name[%s] id[%s]没有获取到设备[%s]权限[%d]\n",
							m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_pcUserAccount->m_tUserID.c_str(), uuid.c_str(), er);
						bAuthRet = false;
						break;
					}

                    if (!encPri.HasPrivilege(CEncoderPrivilege::en_Vod_Play))
					{
						TASKLOG(NORMAL_TASK, ERROR_LEV, "用户name[%s] id[%s]没有获取到设备[%s]开启前端录像权限n",
							m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_pcUserAccount->m_tUserID.c_str(), uuid.c_str());
                        bAuthRet = false;
                        break;
                    }
                }
                else if (CU_CUI_INVITE_PLAY_DOWNLOADPURECORD_REQ == pcMsg->event)
				{
					CEncoderPrivilege encPri;
					er = g_cRedisDb.GetUserEncoderPrivilege(m_pcCuAgent->m_pcUserAccount->m_tUserID, uuid, encPri);
					if ( !IsFdbResultPositive(er) )
					{
						TASKLOG(NORMAL_TASK, ERROR_LEV, "用户name[%s] id[%s]没有获取到设备[%s]权限[%d]\n",
							m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_pcUserAccount->m_tUserID.c_str(), uuid.c_str(), er);
						bAuthRet = false;
						break;
					}

					if (!encPri.HasPrivilege(CEncoderPrivilege::en_Vod_Download))
					{
						TASKLOG(NORMAL_TASK, ERROR_LEV, "用户name[%s] id[%s]没有获取到设备[%s]下载前端录像权限n",
							m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_pcUserAccount->m_tUserID.c_str(), uuid.c_str());
                        bAuthRet = false;
                        break;
                    }
                }
            } while (0);

            if (!bAuthRet)
            {
                TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户[%s]会话[%s]发送Invite请求，但是对源通道[%s--%d]无权限\n",
                    m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
                    pcReq->GetSrcChn().GetDevUri().c_str(), pcReq->GetSrcChn().GetChnNO());

                //记录日志
                WriteLog(ERR_CUI_CU_NO_PERMISSION, m_dwEvent);

                CCuSetupStreamRsp cRsp;
                cRsp.SetErrorCode(ERR_CUI_CU_NO_PERMISSION);
                SendRspToCu<CCuSetupStreamRsp>(cRsp, m_ptRecvNode);
                m_nplayId = INVALID_PLAY_ID;
                delete this;
                break;
            }

			if (m_pcCuAgent->CuiTransStream4Pu())
			{
				return ProcInviteReqNatSwitch();
			} 
			else
			{
				return ProcInviteReqEx();
			}
        }
        break;
    case CU_CUI_INVITE_ACK_REQ:
        {
			if (m_pcCuAgent->CuiTransStream4Pu())
			{
				ProcInviteAckNatSwitch(pcMsg);
			} 
			else
			{
				ProcInviteAckEx(pcMsg);
			}
            m_nplayId = INVALID_PLAY_ID;
            delete this;
        }
        break;  
    case CU_CUI_INVITE_BYE_REQ:
    case CU_CUI_INVITE_STOP_DOWNLOADPURECORD_REQ:
    case CU_CUI_INVITE_STOP_PURECORD_REQ:
        {
			if (m_pcCuAgent->CuiTransStream4Pu())
			{
				ProcInviteByeNatSwitch(pcMsg);
			}
			else
			{
				ProcInviteByeEx(pcMsg);
			}
        }
        break;
    case VCR_CTRL_REQ:
        {
			if (m_pcCuAgent->CuiTransStream4Pu())
			{
				ProcVcrReqNatSwitch(pcMsg);
			}
			else
			{
				ProcVcrReqEx(pcMsg);
			}
        }
        break;
    default:
        {
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"收到未知消息");
        }
        break;
    }
    return PROCMSG_OK;
}

u32 CInviteInterAction::WaitRspProcMsg( CMessage *const pcMsg )
{
    COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
    switch (pcMsg->event)
    {
    case OSP_SIP_MSG_PROC_FAIL:
    case OSP_SIP_DIALOG_TERMINATE:
        {
            CEventRsp cRsp;
            cRsp.SetErrorCode(ERR_CUI_SIP_MSG_FAIL);
            WriteLog((u32)cRsp.GetErrorCode(), m_dwEvent);

            TASKLOG(NORMAL_TASK, ERROR_LEV, "向CU用户[%s]会话[%s]发送失败应答消息，错误码[%d]\n",
                m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(), cRsp.GetErrorCode());
            SendRspToCu<CEventRsp>(cRsp, m_ptRecvNode);
            //任务完成, 销毁之
            NextState(NOT_REQUEST_STATE);
            delete this;
        }
        break;
    case INVITE_RSP:
        {
            CInviteRsp cRsp;
            pcOspSipMsg->GetMsgBody(cRsp);
            if ( INVALID_DIALOG_ID == pcOspSipMsg->GetSipDlgID())
            {
                cRsp.SetErrorCode(ERR_CUI_DIALOG_NOT_ESTABLISHED);
            }

			if (! m_pcCuAgent->CuiTransStream4Pu())
			{
				//只有走半源半端口和地址伪装才需要更新平台地址
				SwitchPlatAddrToNat(cRsp.GetSdp());
			}

            SendRspToCu<CCuSetupStreamRsp>(cRsp, m_ptRecvNode);
            WriteLog((u32)cRsp.GetErrorCode(), m_dwEvent);
            //任务完成, 销毁之
            NextState(NOT_REQUEST_STATE);
            delete this;
        }
        break;
    case OSP_SIP_BYE_REQ:
        { //服务器端主动结束会话
            CInviteRsp cRsp;
            if(pcOspSipMsg->GetMsgBodyLen() >0)
            {
                CByeReq cByeReq;
                pcOspSipMsg->GetMsgBody(cByeReq);
                cRsp.SetErrorCode(cByeReq.GetReason());
                WriteLog(cByeReq.GetReason(), m_dwEvent);
            }
            else
            {
                cRsp.SetErrorCode(ERR_CUI_DIALOG_TERMINATED);
                WriteLog(ERR_CUI_DIALOG_TERMINATED, m_dwEvent);
            }

            SendRspToCu<CCuSetupStreamRsp>(cRsp, m_ptRecvNode);

            //任务完成, 销毁之
            NextState(NOT_REQUEST_STATE);
            delete this;
        }
        break;
    case OSP_SIP_BYE_RSP:
        {
            CCuStopInviteRsp cRsp;
            cRsp.SetErrorCode(CMS_SUCCESS);

            WriteLog((u32)cRsp.GetErrorCode(), m_dwEvent);
            SendRspToCu<CCuStopInviteRsp>(cRsp, m_ptRecvNode);
            //任务完成, 销毁之
            NextState(NOT_REQUEST_STATE);
            delete this;
        }
        break;
    case VCR_CTRL_RSP:
        {
            CCuVcrCtrlRsp cRsp;
            pcOspSipMsg->GetMsgBody(cRsp);

            SendRspToCu<CCuVcrCtrlRsp>(cRsp, m_ptRecvNode);
            //任务完成, 销毁之
            NextState(NOT_REQUEST_STATE);
            delete this;
        }
        break;
    default:
        {
            TASKLOG(NORMAL_TASK, ERROR_LEV, "收到未知消息[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"收到未知消息");
        }
        break;
    }
    return PROCMSG_OK;
}

u32 CInviteInterAction::WaitRspTimerPoll()
{
    if ( GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout() )
    {
        TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户[%s]会话[%s]等待Invite应答超时\n",
            m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str());

        if(m_dwEvent == INVITE_REQ)
        {
            CInviteRsp cRsp;
            cRsp.SetErrorCode(ERR_CUI_OPR_TIME_OUT);
            SendRspToCu<CCuSetupStreamRsp>(cRsp, m_ptRecvNode);
            WriteLog((u32)cRsp.GetErrorCode(), m_dwEvent);
            NextState(NOT_REQUEST_STATE);
            return TIMERPOLL_DEL;
        }
        else if(m_dwEvent == VCR_CTRL_REQ)
        {
            CCuVcrCtrlRsp cRsp;
            cRsp.SetErrorCode(ERR_CUI_OPR_TIME_OUT);
            SendRspToCu<CCuVcrCtrlRsp>(cRsp, m_ptRecvNode);
            WriteLog((u32)cRsp.GetErrorCode(), m_dwEvent);
            NextState(NOT_REQUEST_STATE);
            return TIMERPOLL_DEL;
        }
        else if (m_dwEvent == CU_CUI_INVITE_BYE_REQ
            || m_dwEvent == CU_CUI_INVITE_STOP_DOWNLOADPURECORD_REQ
            || m_dwEvent == CU_CUI_INVITE_STOP_PURECORD_REQ)
        {
            CCuStopInviteRsp cRsp;
            cRsp.SetErrorCode(ERR_CUI_OPR_TIME_OUT);
            SendRspToCu<CEventRsp>(cRsp, m_ptRecvNode);
            WriteLog((u32)cRsp.GetErrorCode(), m_dwEvent);
            NextState(NOT_REQUEST_STATE);
            return TIMERPOLL_DEL;
        }
    }
    return TIMERPOLL_DONE;
}

/*CInviteTask*/CTask * CInviteInterAction::GetInviteTask( s32 nPlayId )
{
    CMS_ASSERT(NULL != m_pcCuAgent);
	if ( m_pcCuAgent == NULL )
	{
		return NULL;
	}

    u32 dwTaskId = INVALID_TASKNO;
    if ( ! m_pcCuAgent->m_cInviteTaskSet.Find(m_nplayId, dwTaskId) )
    {
        return NULL;
    }

    if ( INVALID_TASKNO == dwTaskId )
    {
        return NULL;
    }

    return /*(CInviteTask *)*/(GetInstance()->FindTask(dwTaskId));
}

u32 CInviteInterAction::ProcInviteReq()
{
	CCuSetupStreamReq cReq;
	cReq.FromXml(m_strReq);

	//查找InviteTask
	CInviteTask *pcInviteTask = (CInviteTask*)GetInviteTask(m_nplayId);
	
	/*
	如果找到了InviteTask，说明此交换CU侧已经拆除，CUI没有拆除。
	拆除原InviteTask，保持和CU侧一致。
	*/
	if (NULL != pcInviteTask)
	{
		delete pcInviteTask;
		pcInviteTask = NULL;
	}

	if ( NULL == pcInviteTask )
	{
		pcInviteTask = new CInviteTask(GetInstance(), m_pcCuAgent, this, m_nplayId);
		pcInviteTask->InitStateMachine();
		//将当前任务登记到CuAgent中
		m_pcCuAgent->m_cInviteTaskSet.Insert(cReq.GetDstChn().GetChnNO(), pcInviteTask->GetTaskNO());
	}

	if ( pcInviteTask->GetState() != CInviteTask::NOT_REQUEST_STATE )
	{
		CInviteRsp cRsp;
		cRsp.SetSession(cReq.GetSession());
		cRsp.SetSeqNum(cReq.GetSeqNum());
		cRsp.SetErrorCode(ERR_CUI_DIALOG_NOT_ESTABLISHED);
		SendRspToCu<CCuSetupStreamRsp>(cRsp, m_ptRecvNode);

		TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户[%s]会话[%s]已经发起过播放窗口[%d]的InviteReq，本次请求被拒绝\n",
			m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(), cReq.GetDstChn().GetChnNO());

		//任务完成, 销毁之
		NextState(NOT_REQUEST_STATE);
		delete this;
		return PROCMSG_DEL;
	}
	else
	{
		u32 dwRet = pcInviteTask->StartSendInviteReq(cReq);
        if( dwRet == PROCMSG_OK )
        {
		    NextState(WAIT_RESPONSE_STATE);
        }
        else
        {
            CInviteRsp cRsp;
            cRsp.SetSession( cReq.GetSession());
            cRsp.SetSeqNum( cReq.GetSeqNum());
            cRsp.SetErrorCode( pcInviteTask->GetErrorCode() );
            SendRspToCu<CInviteRsp>(cRsp, m_ptRecvNode);

            TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户[%s]会话[%s] StartSendInviteReq失败, 错误码[%u] \n",
                m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(), pcInviteTask->GetErrorCode() );

            delete pcInviteTask;
            pcInviteTask = NULL;

            //任务完成, 销毁之
            NextState(NOT_REQUEST_STATE);

            dwRet =  PROCMSG_DEL;
            delete this;
        }
	}
	return PROCMSG_OK;
}

u32 CInviteInterAction::ProcInviteReqEx()
{
	CCuSetupStreamReq cReq;
	cReq.FromXml(m_strReq);

	//查找InviteTask
	CInviteTaskEx *pcInviteTask = (CInviteTaskEx*)GetInviteTask(m_nplayId);
	
	/*
	如果找到了InviteTask，说明此交换CU侧已经拆除，CUI没有拆除。
	拆除原InviteTask，保持和CU侧一致。
	*/
	if (NULL != pcInviteTask)
	{
		delete pcInviteTask;
		pcInviteTask = NULL;
	}

	if ( NULL == pcInviteTask )
	{
		pcInviteTask = new CInviteTaskEx(GetInstance(), m_pcCuAgent, this,m_nplayId);
		pcInviteTask->InitStateMachine();
		//将当前任务登记到CuAgent中
		m_pcCuAgent->m_cInviteTaskSet.Insert(cReq.GetDstChn().GetChnNO(), pcInviteTask->GetTaskNO());

		//将当前事务保存起来，NAT探测回调时需要更改探测包地址

		PushMediaTask(cReq.GetSession(),m_nplayId,pcInviteTask);
	}

	if ( pcInviteTask->GetState() != CInviteTaskEx::NOT_REQUEST_STATE )
	{
		CInviteRsp cRsp;
		cRsp.SetSession(cReq.GetSession());
		cRsp.SetSeqNum(cReq.GetSeqNum());
		cRsp.SetErrorCode(ERR_CUI_DIALOG_NOT_ESTABLISHED);
		SendRspToCu<CCuSetupStreamRsp>(cRsp, m_ptRecvNode);

		TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户[%s]会话[%s]已经发起过播放窗口[%d]的InviteReq，本次请求被拒绝\n",
			m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(), cReq.GetDstChn().GetChnNO());

		//任务完成, 销毁之
		NextState(NOT_REQUEST_STATE);
		delete this;
		return PROCMSG_DEL;
	}
	else
	{
		u32 dwRet = pcInviteTask->StartSendInviteReq(cReq);
        if( dwRet == PROCMSG_OK )
        {
		    NextState(WAIT_RESPONSE_STATE);
        }
        else
        {
            CInviteRsp cRsp;
            cRsp.SetSession( cReq.GetSession());
            cRsp.SetSeqNum( cReq.GetSeqNum());
            cRsp.SetErrorCode( pcInviteTask->GetErrorCode() );
            SendRspToCu<CInviteRsp>(cRsp, m_ptRecvNode);

            TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户[%s]会话[%s] StartSendInviteReq失败, 错误码[%u] \n",
                m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(), pcInviteTask->GetErrorCode() );

            delete pcInviteTask;
            pcInviteTask = NULL;

            //任务完成, 销毁之
            NextState(NOT_REQUEST_STATE);

            dwRet =  PROCMSG_DEL;
            delete this;
        }
	}
	return PROCMSG_OK;
}

u32 CInviteInterAction::ProcInviteReqNatSwitch()
{
	CCuSetupStreamReq cReq;
	cReq.FromXml(m_strReq);

	//查找InviteTask
	CInviteTaskNatSwitch *pcInviteTask = (CInviteTaskNatSwitch*)GetInviteTask(m_nplayId);

	/*
	如果找到了InviteTask，说明此交换CU侧已经拆除，CUI没有拆除。
	拆除原InviteTask，保持和CU侧一致。
	*/
	if (NULL != pcInviteTask)
	{
		delete pcInviteTask;
		pcInviteTask = NULL;
	}

	if ( NULL == pcInviteTask )
	{
		pcInviteTask = new CInviteTaskNatSwitch(GetInstance(), m_pcCuAgent, this,m_nplayId);
		pcInviteTask->InitStateMachine();
		//将当前任务登记到CuAgent中
		m_pcCuAgent->m_cInviteTaskSet.Insert(cReq.GetDstChn().GetChnNO(), pcInviteTask->GetTaskNO());

		//将当前事务保存起来，NAT探测回调时需要更改探测包地址
		PushMediaTask(cReq.GetSession(), m_nplayId, pcInviteTask);
	}

	if ( pcInviteTask->GetState() != CInviteTaskNatSwitch::NOT_REQUEST_STATE )
	{
		CInviteRsp cRsp;
		cRsp.SetSession(cReq.GetSession());
		cRsp.SetSeqNum(cReq.GetSeqNum());
		cRsp.SetErrorCode(ERR_CUI_DIALOG_NOT_ESTABLISHED);
		SendRspToCu<CCuSetupStreamRsp>(cRsp, m_ptRecvNode);

		TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户[%s]会话[%s]已经发起过播放窗口[%d]的InviteReq，本次请求被拒绝\n",
			m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(), cReq.GetDstChn().GetChnNO());

		//任务完成, 销毁之
		NextState(NOT_REQUEST_STATE);
		delete this;
		return PROCMSG_DEL;
	}
	else
	{
		// 向CMU请求建立交换
		u32 dwRet = pcInviteTask->StartSendInviteReqNatSwitch(cReq);
        if( dwRet == PROCMSG_OK )
        {
		    NextState(WAIT_RESPONSE_STATE);
        }
        else
        {
            CInviteRsp cRsp;
            cRsp.SetSession( cReq.GetSession());
            cRsp.SetSeqNum( cReq.GetSeqNum());
            cRsp.SetErrorCode( pcInviteTask->GetErrorCode() );
            SendRspToCu<CInviteRsp>(cRsp, m_ptRecvNode);

            TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户[%s]会话[%s] StartSendInviteReq失败, 错误码[%u] \n",
                m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(), pcInviteTask->GetErrorCode() );

            delete pcInviteTask;
            pcInviteTask = NULL;

            //任务完成, 销毁之
            NextState(NOT_REQUEST_STATE);

            dwRet =  PROCMSG_DEL;
            delete this;
        }
	}
	return PROCMSG_OK;
}

void CInviteInterAction::ProcInviteAck(CMessage *const pcMsg)
{
	CCuPlayStreamReq *pcReq = (CCuPlayStreamReq *)pcMsg->content;
	m_nplayId = pcReq->GetPlayId();

	TASKLOG(NORMAL_TASK, ERROR_LEV, "ProcInviteAck playID is %d\n",pcReq->GetPlayId());

	CInviteTask *pcInviteTask = (CInviteTask*)GetInviteTask(m_nplayId);
	CCuPlayStreamRsp cRsp;
	int nErrorCode = ERR_CUI_DIALOG_NOT_ESTABLISHED;
	if ( NULL == pcInviteTask )
	{ //事务尚未建立
		TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户[%s]会话[%s]的Invite没有对应Task\n",
			m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str());
	}
	else
	{
		if ( pcInviteTask->GetState() == CInviteTask::WAIT_SETUP_ACK_STATE )
		{
			pcInviteTask->StartSendInviteAck();
			pcInviteTask->m_pcInviteInterActTask = NULL;
			nErrorCode = CMS_SUCCESS;
		}
		else
		{
			TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户[%s]会话[%s]的InviteTask[%u-%s]处于[%s]状态\n",
				m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
				pcInviteTask->GetTaskNO(), pcInviteTask->GetObjName(),
				pcInviteTask->GetStateName(pcInviteTask->GetState()));
		}
	}

	TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]发送的InviteAck没有应答消息，CUI直接向CU回复应答\n",
		m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str());
	//ACK消息没有应答，直接回复
	cRsp.SetErrorCode(nErrorCode);
	SendRspToCu<CCuPlayStreamRsp>(cRsp, m_ptRecvNode);
}

void CInviteInterAction::ProcInviteAckEx(CMessage *const pcMsg)
{
	CCuPlayStreamReq *pcReq = (CCuPlayStreamReq *)pcMsg->content;
	pcReq->SetUserMonitorId(m_pcCuAgent->m_pcUserAccount->m_tUserID);
	m_nplayId = pcReq->GetPlayId();

	TASKLOG(NORMAL_TASK, ERROR_LEV, "ProcInviteAckEx playID is %d\n",pcReq->GetPlayId());

	CInviteTaskEx *pcInviteTask = (CInviteTaskEx*)GetInviteTask(m_nplayId);
	CCuPlayStreamRsp cRsp;
	int nErrorCode = ERR_CUI_DIALOG_NOT_ESTABLISHED;
	if ( NULL == pcInviteTask )
	{ //事务尚未建立
		TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户[%s]会话[%s]的Invite没有对应Task\n",
			m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str());
	}
	else
	{
		if ( pcInviteTask->GetState() == CInviteTaskEx::WAIT_SETUP_ACK_STATE )
		{
			pcInviteTask->StartSendInviteAck();
			pcInviteTask->m_pcInviteInterActTask = NULL;
			nErrorCode = CMS_SUCCESS;
		}
		else
		{
			TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户[%s]会话[%s]的InviteTask[%u-%s]处于[%s]状态\n",
				m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
				pcInviteTask->GetTaskNO(), pcInviteTask->GetObjName(),
				pcInviteTask->GetStateName(pcInviteTask->GetState()));
		}
	}

	TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]发送的InviteAck没有应答消息，CUI直接向CU回复应答\n",
		m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str());
	//ACK消息没有应答，直接回复
	cRsp.SetErrorCode(nErrorCode);
	SendRspToCu<CCuPlayStreamRsp>(cRsp, m_ptRecvNode);
}

void CInviteInterAction::ProcInviteAckNatSwitch(CMessage *const pcMsg)
{
	CCuPlayStreamReq *pcReq = (CCuPlayStreamReq *)pcMsg->content;
	pcReq->SetUserMonitorId(m_pcCuAgent->m_pcUserAccount->m_tUserID);
	m_nplayId = pcReq->GetPlayId();

	TASKLOG(NORMAL_TASK, ERROR_LEV, "ProcInviteAckNatSwitch playID is %d\n",pcReq->GetPlayId());

	CInviteTaskNatSwitch *pcInviteTask = (CInviteTaskNatSwitch*)GetInviteTask(m_nplayId);
	CCuPlayStreamRsp cRsp;
	int nErrorCode = ERR_CUI_DIALOG_NOT_ESTABLISHED;
	if ( NULL == pcInviteTask )
	{ //事务尚未建立
		TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户[%s]会话[%s]的Invite没有对应Task\n",
			m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str());
	}
	else
	{
		if ( pcInviteTask->GetState() == CInviteTaskNatSwitch::WAIT_SETUP_ACK_STATE )
		{
			pcInviteTask->StartSendInviteAckNatSwitch();
			pcInviteTask->m_pcInviteInterActTask = NULL;
			nErrorCode = CMS_SUCCESS;
		}
		else
		{
			TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户[%s]会话[%s]的InviteTask[%u-%s]处于[%s]状态\n",
				m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
				pcInviteTask->GetTaskNO(), pcInviteTask->GetObjName(),
				pcInviteTask->GetStateName(pcInviteTask->GetState()));
		}
	}

	TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]发送的InviteAck没有应答消息，CUI直接向CU回复应答\n",
		m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str());
	//ACK消息没有应答，直接回复
	cRsp.SetErrorCode(nErrorCode);
	SendRspToCu<CCuPlayStreamRsp>(cRsp, m_ptRecvNode);
}

void CInviteInterAction::ProcInviteBye(CMessage *const pcMsg)
{
	u32 dwRet = PROCMSG_OK;
	CCuStopStreamReq *pcReq = (CCuStopStreamReq *)pcMsg->content;
	m_nplayId = pcReq->GetPlayId();
	m_strReq = pcReq->ToXml();
	pcReq->SetEvent(CU_CUI_INVITE_BYE_REQ);

	//GetOptLogDesc(GetLanDesc(LAN_DESC_STOP) + GetLanDesc(LAN_DESC_REAL_MEDIA_INVITE), TransInt2String(pcReq->GetPlayId()));    
	CInviteTask *pcInviteTask = (CInviteTask*)GetInviteTask(m_nplayId);
	if ( NULL != pcInviteTask )
	{
		m_tDevChn = pcInviteTask->m_tReq.GetSrcChn();

		if (pcInviteTask->TryBye(this))
		{
			NextState(WAIT_RESPONSE_STATE);
		}
		else
		{
			CCuStopInviteRsp cRsp;
			TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户[%s]会话[%s]的Invite Task TryBye failed\n",
				m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str());
			cRsp.SetErrorCode(ERR_CUI_DIALOG_NOT_ESTABLISHED);
			SendRspToCu<CCuStopInviteRsp>(cRsp, m_ptRecvNode);

			m_nplayId = INVALID_PLAY_ID;
			delete this;
		}
	}
	else
	{
		CCuStopInviteRsp cRsp;
		TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户[%s]会话[%s]的Invite没有对应Task, playId[%d]\n",
			m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(), m_nplayId);
		cRsp.SetErrorCode(ERR_CUI_DIALOG_NOT_ESTABLISHED);
		TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]发送的InviteBye没有应答消息，CUI直接向CU回复应答\n",
			m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str());
		//BYE消息没有应答，直接回复
		SendRspToCu<CCuStopInviteRsp>(cRsp, m_ptRecvNode);
		m_nplayId = INVALID_PLAY_ID;
       delete this;
	}  
}

void CInviteInterAction::ProcInviteByeEx(CMessage *const pcMsg)
{
	CCuStopStreamReq *pcReq = (CCuStopStreamReq *)pcMsg->content;
	pcReq->SetUserMonitorId(m_pcCuAgent->m_pcUserAccount->m_tUserID);
	m_nplayId = pcReq->GetPlayId();
	m_strReq = pcReq->ToXml();
	pcReq->SetEvent(CU_CUI_INVITE_BYE_REQ);

	//GetOptLogDesc(GetLanDesc(LAN_DESC_STOP) + GetLanDesc(LAN_DESC_REAL_MEDIA_INVITE), TransInt2String(pcReq->GetPlayId()));    
	CInviteTaskEx *pcInviteTask = (CInviteTaskEx*)GetInviteTask(m_nplayId);
	if ( NULL != pcInviteTask )
	{
		m_tDevChn = pcInviteTask->m_tReq.GetSrcChn();

		if (pcInviteTask->TryBye(this))
		{
			NextState(WAIT_RESPONSE_STATE);
		}
		else
		{
			CCuStopInviteRsp cRsp;
			TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]的Invite Task TryBye failed\n",
				m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str());
			cRsp.SetErrorCode(ERR_CUI_DIALOG_NOT_ESTABLISHED);
			SendRspToCu<CCuStopInviteRsp>(cRsp, m_ptRecvNode);

			m_nplayId = INVALID_PLAY_ID;
			delete this;
		}
	}
	else
	{
		CCuStopInviteRsp cRsp;
		TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户[%s]会话[%s]的Invite没有对应Task, playId[%d]\n",
			m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(), m_nplayId);
		cRsp.SetErrorCode(ERR_CUI_DIALOG_NOT_ESTABLISHED);
		TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]发送的InviteBye没有应答消息，CUI直接向CU回复应答\n",
			m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str());
		//BYE消息没有应答，直接回复
		SendRspToCu<CCuStopInviteRsp>(cRsp, m_ptRecvNode);
		m_nplayId = INVALID_PLAY_ID;
		delete this;
	}  
}

void CInviteInterAction::ProcInviteByeNatSwitch(CMessage *const pcMsg)
{
	CCuStopStreamReq *pcReq = (CCuStopStreamReq *)pcMsg->content;
	pcReq->SetUserMonitorId(m_pcCuAgent->m_pcUserAccount->m_tUserID);
	m_nplayId = pcReq->GetPlayId();
	m_strReq = pcReq->ToXml();
	pcReq->SetEvent(CU_CUI_INVITE_BYE_REQ);
 
	CInviteTaskNatSwitch *pcInviteTask = (CInviteTaskNatSwitch*)GetInviteTask(m_nplayId);
	if ( NULL != pcInviteTask )
	{
		m_tDevChn = pcInviteTask->m_tReq.GetSrcChn();

		if (pcInviteTask->TryBye(this))
		{
			NextState(WAIT_RESPONSE_STATE);
		}
		else
		{
			CCuStopInviteRsp cRsp;
			TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]的Invite Task TryBye failed\n",
				m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str());
			cRsp.SetErrorCode(ERR_CUI_DIALOG_NOT_ESTABLISHED);
			SendRspToCu<CCuStopInviteRsp>(cRsp, m_ptRecvNode);

			m_nplayId = INVALID_PLAY_ID;
			delete this;
		}
	}
	else
	{
		CCuStopInviteRsp cRsp;
		TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户[%s]会话[%s]的Invite没有对应Task, playId[%d]\n",
			m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(), m_nplayId);
		cRsp.SetErrorCode(ERR_CUI_DIALOG_NOT_ESTABLISHED);
		TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]发送的InviteBye没有应答消息，CUI直接向CU回复应答\n",
			m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str());
		//BYE消息没有应答，直接回复
		SendRspToCu<CCuStopInviteRsp>(cRsp, m_ptRecvNode);
		m_nplayId = INVALID_PLAY_ID;
		delete this;
	}  
}

void CInviteInterAction::ProcVcrReq(CMessage *const pcMsg)
{
	CCuVcrCtrlReq *pcReq = (CCuVcrCtrlReq *)pcMsg->content;
	m_nplayId = pcReq->GetPlayId();
	m_dwSeqNo = pcReq->GetSeqNum();

	CInviteTask *pcInviteTask = (CInviteTask*)GetInviteTask(m_nplayId);
	if ( NULL != pcInviteTask )
	{
		pcInviteTask->AddChildTask(pcReq->GetSeqNum(), GetTaskNO());
		m_tDevChn = pcInviteTask->m_tReq.GetSrcChn();
		//pcInviteTask->m_pcInviteInterActTask = this;
		pcInviteTask->StartVcrCtrl(*pcReq);
		NextState(WAIT_RESPONSE_STATE);
	}
	else
	{
		CCuVcrCtrlRsp cRsp;
		TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户[%s]会话[%s]的Invite没有对应Task\n",
			m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str());
		cRsp.SetErrorCode(ERR_CUI_DIALOG_NOT_ESTABLISHED);

		SendRspToCu<CCuVcrCtrlRsp>(cRsp, m_ptRecvNode);
		m_nplayId = INVALID_PLAY_ID;
		delete this;
	}  
}
void CInviteInterAction::ProcVcrReqEx(CMessage *const pcMsg)
{
	CCuVcrCtrlReq *pcReq = (CCuVcrCtrlReq *)pcMsg->content;
	pcReq->SetUserMonitorId(m_pcCuAgent->m_pcUserAccount->m_tUserID);
	m_nplayId = pcReq->GetPlayId();
	m_dwSeqNo = pcReq->GetSeqNum();

	CInviteTaskEx *pcInviteTask = (CInviteTaskEx*)GetInviteTask(m_nplayId);
	if ( NULL != pcInviteTask )
	{
		pcInviteTask->AddChildTask(pcReq->GetSeqNum(), GetTaskNO());
		m_tDevChn = pcInviteTask->m_tReq.GetSrcChn();
		//pcInviteTask->m_pcInviteInterActTask = this;
		pcInviteTask->StartVcrCtrl(*pcReq);
		NextState(WAIT_RESPONSE_STATE);
	}
	else
	{
		CCuVcrCtrlRsp cRsp;
		TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户[%s]会话[%s]的Invite没有对应Task\n",
			m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str());
		cRsp.SetErrorCode(ERR_CUI_DIALOG_NOT_ESTABLISHED);

		SendRspToCu<CCuVcrCtrlRsp>(cRsp, m_ptRecvNode);
		m_nplayId = INVALID_PLAY_ID;
		delete this;
	} 
}

void CInviteInterAction::ProcVcrReqNatSwitch(CMessage *const pcMsg)
{
	CCuVcrCtrlReq *pcReq = (CCuVcrCtrlReq *)pcMsg->content;
	pcReq->SetUserMonitorId(m_pcCuAgent->m_pcUserAccount->m_tUserID);
	m_nplayId = pcReq->GetPlayId();
	m_dwSeqNo = pcReq->GetSeqNum();

	CInviteTaskNatSwitch *pcInviteTask = (CInviteTaskNatSwitch*)GetInviteTask(m_nplayId);
	if ( NULL != pcInviteTask )
	{
		pcInviteTask->AddChildTask(pcReq->GetSeqNum(), GetTaskNO());
		m_tDevChn = pcInviteTask->m_tReq.GetSrcChn();
		//pcInviteTask->m_pcInviteInterActTask = this;
		pcInviteTask->StartVcrCtrl(*pcReq);
		NextState(WAIT_RESPONSE_STATE);
	}
	else
	{
		CCuVcrCtrlRsp cRsp;
		TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户[%s]会话[%s]的Invite没有对应Task\n",
			m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str());
		cRsp.SetErrorCode(ERR_CUI_DIALOG_NOT_ESTABLISHED);

		SendRspToCu<CCuVcrCtrlRsp>(cRsp, m_ptRecvNode);
		m_nplayId = INVALID_PLAY_ID;
		delete this;
	} 
}

void CInviteInterAction::SwitchPlatAddrToNat(TSdp& tSdp)
{
	TMediaDescList& tMediaDescList = tSdp.GetMediaDescList();

	for(TMediaDescList::iterator itrMediaDesc = tMediaDescList.begin(); 
		itrMediaDesc!=tMediaDescList.end(); itrMediaDesc++)
	{
		TMediaTransChannelList& tMediaTransChnList = itrMediaDesc->GetMediaTransChnList();
		for(TMediaTransChannelList::iterator itrTransChn = tMediaTransChnList.begin(); 
			itrTransChn!=tMediaTransChnList.end(); itrTransChn++)
		{
			if (itrTransChn->GetTransChnType() == MEDIA_TRANS_CHN_RTCP)
			{
				TNetAddrList& tNatAddrList = itrTransChn->GetAddrList();
				for (TNetAddrList::iterator itrNatAddr = tNatAddrList.begin();
					itrNatAddr != tNatAddrList.end();itrNatAddr++)
				{
					string strSrcIp = itrNatAddr->GetNetIp();
					int nSrcPort = itrNatAddr->GetNetPort();
					string strDstIp="";
					int nDstPort=0;
					stringstream ssKey;
					ssKey << NAT_PROTOCOL_UDP << "_" << strSrcIp << "_" << nSrcPort;
					TMappingTable::const_iterator it = g_tMappingTable.find(ssKey.str());
					TASKLOG(NORMAL_TASK, EVENT_LEV, "Key:(%s)\n", ssKey.str().c_str());
					if (it != g_tMappingTable.end())
					{
						TASKLOG(NORMAL_TASK, EVENT_LEV, "PlatIp(%s), HostIP(%s), MappingIP(%s), MappingPort(%d)\n",
							m_pcCuAgent->m_tNatInfo.GetIpAddr().c_str(),
							m_pcCuAgent->m_strHostIp.c_str(),
							it->second.GetMappingIP().c_str(),
							it->second.GetMappingPort());
						//当含有二层NAT时，平台不能同时把一个端口（8001）映射到第一层和第二层
						//这里直接使用cu的登陆ip，即为平台在的最外层映射IP
						//itrNatAddr->SetNetIp(g_cCuiConfig.GetMapedMediaIP());
						itrNatAddr->SetNetIp(m_pcCuAgent->m_strHostIp);
						//当cu登陆ip与平台映射ip相同时需要替换映射出去的端口
						//如果不相同，说明含有二层NAT，平台映射出去的与实际cu不在统一层NAT固无须替换映射后的端口
						if (/*strDstIp*/m_pcCuAgent->m_tNatInfo.GetIpAddr() != m_pcCuAgent->m_strHostIp)
						{
							itrNatAddr->SetNetPort(it->second.GetMappingPort()/*nDstPort*/);
						}
					}
					else if (g_cCuiConfig.GetNatMapAddr(strSrcIp,nSrcPort,strDstIp,nDstPort))
					{
						//当含有二层NAT时，平台不能同时把一个端口（8001）映射到第一层和第二层
						//这里直接使用cu的登陆ip，即为平台在的最外层映射IP
						//itrNatAddr->SetNetIp(g_cCuiConfig.GetMapedMediaIP());
						itrNatAddr->SetNetIp(m_pcCuAgent->m_strHostIp);
						//当cu登陆ip与平台映射ip相同时需要替换映射出去的端口
						//如果不相同，说明含有二层NAT，平台映射出去的与实际cu不在统一层NAT固无须替换映射后的端口
						if (m_pcCuAgent->m_tNatInfo.GetIpAddr() != m_pcCuAgent->m_strHostIp)
						{
							itrNatAddr->SetNetPort(nDstPort);
						}
					}
					else
					{
						TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]切换外网映射地址失败，可能导致浏览失败\n",
							m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str());
					}
				}
			}
		}
	}
}

void CInviteInterAction::WriteLog( const u32 dwErrorCode, const u32 dwEvent)
{
    CGetPuData *pcTask = new CGetPuData(GetInstance(), m_pcCuAgent->m_strSession, m_tDevChn, dwErrorCode, m_strReq, dwEvent);
    pcTask->InitStateMachine();
	if (pcTask->StartWriteLog() != PROCMSG_OK)
	{
		delete pcTask;
		pcTask = NULL;
	}
}
// void CInviteInterAction::GetOptLogDesc(const string& OptDevInfo)
// {
//     m_strLogDesc =  GetUserLogDesc() + GetLanDesc(LAN_DESC_REAL_MEDIA_INVITE) + "(" + OptDevInfo + ")";
// }
// void CInviteInterAction::GetOptLogDesc(const string& oprType, const string& OptDevInfo)
// {
//     m_strLogDesc =  GetUserLogDesc() + GetLanDesc(oprType) + GetLanDesc(LAN_DESC_REAL_MEDIA_INVITE) + "(" + OptDevInfo + ")";
// }
// void CInviteInterAction::WriteLog(u32 dwErrorCode)
// {
//     m_strLogDesc += GetErrorCode(dwErrorCode);
//     CUsrOprLogTask* pcTask = new CUsrOprLogTask(GetInstance(), m_pcCuAgent->m_strSession, 
//         string(DEV_TYPE_UAS) + "@" + GetDomainName(),
//         enUserOpTypeLiveStream,
//         (u32)dwErrorCode,
//         m_strLogDesc
//         );
//     pcTask->InitStateMachine();
//     pcTask->StartSendLog();
// }
// string CInviteInterAction::GetErrorCode(u32 dwErrorCode)
// {
//     if( CMS_SUCCESS == dwErrorCode)
//     {
//         return GetLanDesc(LAN_DESC_SUCCESS);
//     }
//     else if( ERR_CUI_OPR_TIME_OUT == dwErrorCode)
//     {
//         return GetLanDesc(LAN_DESC_TIMEOUT);
//     }
//     else
//     {
//         return GetLanDesc(LAN_DESC_FAIL);
//     }
// }
CInviteInterAction::~CInviteInterAction()
{
    TerminateTask();
}
//////////////////////////////////////////////////////////////////////////
//为了模板OnWaitNotifyMsg，编写各种push_back函数
void CCuQueryUserRsp::push_back(const CUserQryNtfReq &cNtf, const CQueryTask *pcQueryTask)
{
    CMS_ASSERT(NULL != pcQueryTask);
    CMS_ASSERT(NULL != pcQueryTask->m_pc3acAgent);
    int nRecvItemNum = GetActualResultNum();
    const vector<CUserInfo>& tmpVct = cNtf.GetUserInfo();
    vector<CUserInfo>::const_iterator pItem = tmpVct.begin();
    while (pItem != tmpVct.end())
    {
        //能查询到本域用户信息
        CCuUserInfo cCuUserInfo;
        cCuUserInfo = *pItem;
        if(!cCuUserInfo.GetUserID().empty())
        {
            cCuUserInfo.SetUserID(cCuUserInfo.GetUserID() + "@" + GetDomainName());
        }

        if(!cCuUserInfo.GetUsergrpID().empty())
        {
            cCuUserInfo.SetUsergrpID(cCuUserInfo.GetUsergrpID() + "@" + GetDomainName());
        }

        cCuUserInfo.SetDomainID(pcQueryTask->m_pc3acAgent->m_t3ASDomainID);

        if(!pItem->GetUserName().empty())
        {
            string strUserName = pItem->GetUserName() + "@" + GetDomainName();
            cCuUserInfo.SetUserName(strUserName);
        }

        if(!cCuUserInfo.GetCreatorID().empty())
        {
            cCuUserInfo.SetCreatorID(cCuUserInfo.GetCreatorID() + "@" + GetDomainName());
        }

        GetUserInfo().push_back(cCuUserInfo);

        nRecvItemNum++;
        pItem++;
    }
    SetActualResultNum(nRecvItemNum);
}

void CCuUserGroupQryRsp::push_back(CUserGroupQryNtfReq &cNtf, const CQueryTask *pcQueryTask )
{
    CMS_ASSERT(NULL != pcQueryTask);
    CMS_ASSERT(NULL != pcQueryTask->m_pc3acAgent);
    u32 nRecvItemNum = (u32)GetActualResultNum();
    vector<CUserGroupInfo>& tmpVct = cNtf.GetUserGroupInfo();
    vector<CUserGroupInfo>::iterator pItem = tmpVct.begin();
    while (pItem != tmpVct.end())
    {
        CUserGroupInfo cCuUserGroupInfo;
        cCuUserGroupInfo = *pItem;
        pItem->SetGroupNO(cCuUserGroupInfo.GetGroupNO() + "@" + pcQueryTask->m_pc3acAgent->m_str3ASDomainName);
        pItem->SetCreateUser(cCuUserGroupInfo.GetCreateUser() + "@" + pcQueryTask->m_pc3acAgent->m_str3ASDomainName);

        pItem++;
    }

    nRecvItemNum+= tmpVct.size();
    GetUserGroupInfo().insert(GetUserGroupInfo().end(), tmpVct.begin(), tmpVct.end());
    SetActualResultNum(nRecvItemNum);
}

void CCuUserDevGroupQryRsp::push_back( const CDeviceGroupQryNtfReq &cNtf, const CQueryTask *pcQueryTask )
{
    CMS_ASSERT(NULL != pcQueryTask);
    CMS_ASSERT(NULL != pcQueryTask->m_pc3acAgent);
    int nRecvItemNum = GetActualResultNum();
    const vector<CDeviceGroupInfo>& tmpVct = cNtf.GetDeviceGroupInfo();
    vector<CDeviceGroupInfo>::const_iterator pItem = tmpVct.begin();
    u32 nIndex = 0;
    while (pItem != tmpVct.end())
    {
        CCuUserDeviceGroupInfo cCuDeviceGroupInfo;
        cCuDeviceGroupInfo = *pItem;

        C3acAgent *pc3acAgent = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(pcQueryTask->m_pcCuAgent->m_str3asName, pcQueryTask->GetInstance());;
        if(pc3acAgent != NULL)
        {
            if(!cCuDeviceGroupInfo.GetDeviceGroupID().empty())
            {
                cCuDeviceGroupInfo.SetDeviceGroupID( cCuDeviceGroupInfo.GetDeviceGroupID() + "@" + pc3acAgent->GetDomainNameByDomainID(cCuDeviceGroupInfo.GetDomainUUID()));    
            }

            if(!cCuDeviceGroupInfo.GetDeviceParentGroupID().empty())
            {
                cCuDeviceGroupInfo.SetDeviceParentGroupID(cCuDeviceGroupInfo.GetDeviceParentGroupID() + "@" + pc3acAgent->GetDomainNameByDomainID(cCuDeviceGroupInfo.GetParentDomainUUID()));
            }
        }

        cCuDeviceGroupInfo.SetGroupHasDev(cNtf.GetDeviceGroupHasDevice().at(nIndex));
        GetUserDevGroupInfo().push_back(cCuDeviceGroupInfo);

        nRecvItemNum++;
        pItem++;
        nIndex++;
    }
    SetActualResultNum(nRecvItemNum);
}

void CCuUserDevFullInfoQryRsp::push_back( CCuserDevicefullinfoAndPrivilegeQryNtfReq &cNtf, const CQueryTask *pcQueryTask )
{
    CMS_ASSERT(NULL != pcQueryTask);
    CMS_ASSERT(NULL != pcQueryTask->m_pc3acAgent);
    int nRecvItemNum = GetActualResultNum();

    vector<TDevicefullinfoAndPrivilege>& tmpVct = cNtf.GetDevicefullinfoAndPrivileges();
    //将设备的UUID全部转为devURI
    C3acAgent *pc3acAgent = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(pcQueryTask->m_pcCuAgent->m_str3asName, pcQueryTask->GetInstance());;
    if(pc3acAgent != NULL)
    {
        vector<TDevicefullinfoAndPrivilege>::iterator pDevItem = tmpVct.begin();
        while ( pDevItem != tmpVct.end() )
        {
            string strDomainName = pc3acAgent->GetDomainNameByDomainID(pDevItem->deviceInfo.GetDomainUUID());
            if ( ! strDomainName.empty() )
            {
                string strDevURI = pDevItem->deviceInfo.GetDeviceUUID() + "@" + strDomainName;
                pDevItem->deviceInfo.SetDeviceUUID(strDevURI);
            }

            vector<TDevicefullinfoAndPrivilege>::iterator it = GetUserDevFullInfo().begin();
            for (; it != GetUserDevFullInfo().end(); ++it)
            {
                if (it->deviceInfo.GetDeviceUUID() == pDevItem->deviceInfo.GetDeviceUUID())
                {
                    it->vidSrcIndex.insert( it->vidSrcIndex.end(), pDevItem->vidSrcIndex.begin(), pDevItem->vidSrcIndex.end() ); 
                    it->alarminputinfos.insert(pDevItem->alarminputinfos.begin(), pDevItem->alarminputinfos.end());
                    it->videosourceinfos.insert(pDevItem->videosourceinfos.begin(), pDevItem->videosourceinfos.end());

                    break;
                }
            }
            if (it == GetUserDevFullInfo().end())
            {
                GetUserDevFullInfo().push_back(*pDevItem);
                ++nRecvItemNum;
            }
            
            pDevItem++;
        }
    }

    SetActualResultNum(nRecvItemNum);
}

void CCuQryDevGroupOfDevInfoRsp::push_back( CDeviceGroupDeviceQryNtfReq &cNtf, const CQueryTask *pcQueryTask )
{
    CMS_ASSERT(NULL != pcQueryTask);
    CMS_ASSERT(NULL != pcQueryTask->m_pc3acAgent);
    s32 nRecvItemNum = GetActualResultNum();

    vector<CDeviceGroupDeviceInfo>& tmpVct = cNtf.GetDeviceGroupDeviceInfo();
    C3acAgent *pc3acAgent = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(pcQueryTask->m_pcCuAgent->m_str3asName, pcQueryTask->GetInstance());;
    if(pc3acAgent != NULL)
    {
        //将设备的UUID全部转换为devURI
        vector<CDeviceGroupDeviceInfo>::iterator pDevGrpDevInfoItem = tmpVct.begin();
        while (pDevGrpDevInfoItem != tmpVct.end())
        {
            string strDomainName = pc3acAgent->GetDomainNameByDomainID(pDevGrpDevInfoItem->GetDomainId());
            if ( ! strDomainName.empty() )
            {
                string strDevURI = pDevGrpDevInfoItem->GetDeviceID() + "@" + strDomainName;
                pDevGrpDevInfoItem->SetDeviceID(strDevURI);
                string strGroupURI = pDevGrpDevInfoItem->GetDeviceGroupID() + "@" + strDomainName;
                pDevGrpDevInfoItem->SetDeviceGroupID(strGroupURI);
            }

            vector<CDeviceGroupDeviceInfo>::iterator it = GetDeviceGroupDeviceInfo().begin();
            for (;it != GetDeviceGroupDeviceInfo().end(); ++it)
            {
                if (it->GetDeviceGroupID() == pDevGrpDevInfoItem->GetDeviceGroupID() && 
                    it->GetDeviceID() == pDevGrpDevInfoItem->GetDeviceID())
                {
                    it->GetDeviceCapIndexs().insert(it->GetDeviceCapIndexs().end(), 
                        pDevGrpDevInfoItem->GetDeviceCapIndexs().begin(), 
                        pDevGrpDevInfoItem->GetDeviceCapIndexs().end());
                    break;
                }
            }
            if (it == GetDeviceGroupDeviceInfo().end())
            {
                GetDeviceGroupDeviceInfo().push_back(*pDevGrpDevInfoItem);
                ++nRecvItemNum;
            }
            
            pDevGrpDevInfoItem++;
        }
    }
    SetActualResultNum(nRecvItemNum);
}


void CCuGetUserDeviceRsp::push_back( CUserDeviceQryNtfReq &cNtf, const CQueryTask *pcQueryTask )
{
    CMS_ASSERT(NULL != pcQueryTask);
    CMS_ASSERT(NULL != pcQueryTask->m_pc3acAgent);
    s32 nRecvItemNum = GetActualResultNum();

    C3acAgent *pc3acAgent = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(pcQueryTask->m_pcCuAgent->m_str3asName, pcQueryTask->GetInstance());;
    if(pc3acAgent != NULL)
    {
        //将设备的UUID全部转换为devURI
        vector<CUserDeviceInfo>::iterator pUserDevItem = cNtf.GetCuserDevInfo().begin();
        while ( pUserDevItem != cNtf.GetCuserDevInfo().end() )
        {
            string strDomainName = pc3acAgent->GetDomainNameByDomainID(pUserDevItem->GetDevDomainID());
            if ( ! strDomainName.empty() )
            {
                string strDevURI = pUserDevItem->GetDeviceID() + "@" + strDomainName;
                pUserDevItem->SetDeviceID(strDevURI);

                string strUsrID = pUserDevItem->GetUserID() + "@" + strDomainName;
                pUserDevItem->SetUserID(strUsrID);
            }
            
            vector<CUserDeviceInfo>::iterator it = GetCuserDevInfo().begin();
            for (;it != GetCuserDevInfo().end(); ++it)
            {
                if (it->GetUserID() == pUserDevItem->GetUserID() && it->GetDeviceID() == pUserDevItem->GetDeviceID())
                {
                    it->GetDeviceCapPrivilege().insert(it->GetDeviceCapPrivilege().end(), pUserDevItem->GetDeviceCapPrivilege().begin(),
                        pUserDevItem->GetDeviceCapPrivilege().end());

                    break;
                }
            }
            if (it == GetCuserDevInfo().end())
            {
                GetCuserDevInfo().push_back(*pUserDevItem);
                ++nRecvItemNum;
            }
            
            pUserDevItem++;
        }
    }
    SetActualResultNum(nRecvItemNum);
}

void CCuGetUserGroupDeviceRsp::push_back( CUserGroupDeviceQryNtfReq &cNtf, const CQueryTask *pcQueryTask )
{
    CMS_ASSERT(NULL != pcQueryTask);
    CMS_ASSERT(NULL != pcQueryTask->m_pc3acAgent);
    int nRecvItemNum = GetActualResultNum();

    C3acAgent *pc3acAgent = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(pcQueryTask->m_pcCuAgent->m_str3asName, pcQueryTask->GetInstance());;
    if(pc3acAgent != NULL)
    {
        vector<CUserGroupDeviceInfo>::iterator pUserGroupItem = cNtf.GetUserGroupDeviceInfo().begin();
        while ( pUserGroupItem != cNtf.GetUserGroupDeviceInfo().end() )
        {
            string strDomainName = pc3acAgent->GetDomainNameByDomainID(pUserGroupItem->GetDevDomainID());
            if ( ! strDomainName.empty() )
            {
                string strDevURI = pUserGroupItem->GetDeviceID() + "@" + strDomainName;
                pUserGroupItem->SetDeviceID(strDevURI);
                string strGroupURI = pUserGroupItem->GetGroupID() + "@" + strDomainName;
                pUserGroupItem->SetGroupID(strGroupURI);
            }
            GetUserGroupDeviceInfo().push_back(*pUserGroupItem);
            pUserGroupItem++;
        }
    }
    nRecvItemNum += (s32)cNtf.GetUserGroupDeviceInfo().size();
    SetActualResultNum(nRecvItemNum);
}

void CCuGetAuthorizationDeviceRsp::push_back( CTempAuthorizationQryNtfReq &cNtf, const CQueryTask *pcQueryTask )
{
    CMS_ASSERT(NULL != pcQueryTask);
    CMS_ASSERT(NULL != pcQueryTask->m_pc3acAgent);
    int nRecvItemNum = GetActualResultNum();

    C3acAgent *pc3acAgent = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(pcQueryTask->m_pcCuAgent->m_str3asName, pcQueryTask->GetInstance());;
    if(pc3acAgent != NULL)
    {
        vector<CTempAuthorizationInfo>::iterator pTmpAuthDevItem = cNtf.GetTempAuthorizationInfo().begin();
        while ( pTmpAuthDevItem != cNtf.GetTempAuthorizationInfo().end() )
        {
			string strDomainID;
			EFDB_ErrorCode er = g_cRedisDb.GetDevInfo(pTmpAuthDevItem->GetDeviceID(), CDeviceInfo_domainId, strDomainID);
			if ( !IsFdbResultPositive(er) )
			{
				OspPrintf(TRUE, FALSE, "无法获取设备[%s]的域名 er[%d]\n", pTmpAuthDevItem->GetDeviceID().c_str(), er);
				continue;
			}
            string strDevId = pTmpAuthDevItem->GetDeviceID() + "@" + pc3acAgent->GetDomainNameByDomainID(strDomainID);
            pTmpAuthDevItem->SetDeviceID(strDevId);
            pTmpAuthDevItem->SetUsername( pTmpAuthDevItem->GetUsername() + "@" +pTmpAuthDevItem->GetDestinationDomain());
            GetTempAuthorizationInfo().push_back(*pTmpAuthDevItem);
            pTmpAuthDevItem++;
        }
    }
    nRecvItemNum += (s32)cNtf.GetTempAuthorizationInfo().size();
    SetActualResultNum(nRecvItemNum);
}

void CCuGetDeviceBaseInfoRsp::push_back( CDeviceQryNtfReq &cNtf, const CQueryTask *pcQueryTask )
{
    CMS_ASSERT(NULL != pcQueryTask);
    CMS_ASSERT(NULL != pcQueryTask->m_pc3acAgent);
    int nRecvItemNum = GetActualResultNum();

    C3acAgent *pc3acAgent = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(pcQueryTask->m_pcCuAgent->m_str3asName, pcQueryTask->GetInstance());;
    if(pc3acAgent != NULL)
    {
        vector<CDeviceInfo>::iterator pDevItem = cNtf.GetDeviceInfo().begin();
        while ( pDevItem != cNtf.GetDeviceInfo().end() )
        {
            string strDevId = pDevItem->GetDeviceUUID() + "@" + pc3acAgent->GetDomainNameByDomainID(pDevItem->GetDomainUUID());
            pDevItem->SetDeviceUUID(strDevId);
            GetDeviceInfo().push_back(*pDevItem);
            pDevItem++;
        }
    }
    nRecvItemNum += (s32)cNtf.GetDeviceInfo().size();
    SetActualResultNum(nRecvItemNum);
}

void CCuTvwallQryRsp::push_back(CTvwallQryNtfReq &cNtf, const CQueryTask *pcQueryTask)
{
    CMS_ASSERT(NULL != pcQueryTask);
    CMS_ASSERT(NULL != pcQueryTask->m_pc3acAgent);
    int nRecvItemNum = GetActualResultNum();

    C3acAgent *pc3acAgent = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(pcQueryTask->m_pcCuAgent->m_str3asName, pcQueryTask->GetInstance());;
    if(pc3acAgent != NULL)
    {
        vector<CTvwallInfo>::iterator pDevItem = cNtf.GetInfos().begin();
        while(pDevItem != cNtf.GetInfos().end())
        {
            vector<TvDecoderBinder> tvDecoderBinder = pDevItem->GetTvDecoderBinders();
            for(u32 tvBinderNum = 0; tvBinderNum < tvDecoderBinder.size(); tvBinderNum++)
            {
                if(!tvDecoderBinder[tvBinderNum].decoderId.empty())
                {
                    string strEncoder = tvDecoderBinder[tvBinderNum].decoderId + "@" + pc3acAgent->GetDomainNameByDomainID(pDevItem->GetDomainUUID());
                    tvDecoderBinder[tvBinderNum].decoderId = strEncoder;
                }            
            }
            pDevItem->SetTvDecoderBinders(tvDecoderBinder);

            string strDevId = pDevItem->GetDeviceUUID() + "@" + pc3acAgent->GetDomainNameByDomainID(pDevItem->GetDomainUUID());
            pDevItem->SetDeviceUUID(strDevId);

            vector<CTvwallInfo>::iterator it = GetTvwallInfo().begin();
            for (;it != GetTvwallInfo().end(); ++it)
            {
                if (it->GetDeviceUUID() == pDevItem->GetDeviceUUID())
                {
                    it->GetTvDecoderBinders().insert(it->GetTvDecoderBinders().end(), pDevItem->GetTvDecoderBinders().begin(),
                        pDevItem->GetTvDecoderBinders().end());
                    break;
                }
            }
            if (it == GetTvwallInfo().end())
            {
                GetTvwallInfo().push_back(*pDevItem);
                ++nRecvItemNum;
            }

            pDevItem++;
        }
    }
    SetActualResultNum(nRecvItemNum);
}

void CCuDomainRelationQryRsp::push_back(CDomainRelationQryNtfReq &cNtf, const CQueryTask *pcQueryTask)
{
    CMS_ASSERT(NULL != pcQueryTask);
    CMS_ASSERT(NULL != pcQueryTask->m_pc3acAgent);
    int nRecvItemNum = GetActualResultNum();

    vector<CDomainRelationInfo>::iterator pItem = cNtf.GetDomainRelationInfo().begin();
    while ( pItem != cNtf.GetDomainRelationInfo().end() )
    {
        GetDomainRelationInfo().push_back(*pItem);
        pItem++;
    }
    nRecvItemNum += (s32)cNtf.GetDomainRelationInfo().size();
    SetActualResultNum(nRecvItemNum);
}

void CCuOffLineDevQryRsp::push_back(COffLineDevQryNtfReq &cNtf, const CQueryTask *pcQueryTask)
{
	CMS_ASSERT(NULL != pcQueryTask);
	int nRecvItemNum = GetActualResultNum();

	//离线设备列表
	TDeviceVideoSrcIdList::iterator pOfflineItem = cNtf.GetDevVideoSrcList().begin();
	while ( pOfflineItem != cNtf.GetDevVideoSrcList().end() )
	{
		GetDevVideoSrcList().push_back(*pOfflineItem);
		pOfflineItem++;
	}
	nRecvItemNum += (s32)cNtf.GetDevVideoSrcList().size();
	SetActualResultNum(nRecvItemNum);

	// 增加在线总数统计 [3/2/2015 pangubing]
	SetOnlineVideoSrcNum(cNtf.GetOnlineVideoSrcNum());
}

void CCuRcsQueryNruRsp::push_back(CRcsQueryNruNtfReq &cNtf, const CPeQueryTask* pcQueryTask)
{
    CMS_ASSERT(NULL != pcQueryTask);
    int nRecvItemNum = GetActualResultNum();

    TNruInfoList::iterator pItem = cNtf.GetNruInfoList().begin();
    while ( pItem != cNtf.GetNruInfoList().end() )
    {
        GetNruInfo().push_back(*pItem);
        pItem++;
    }
    nRecvItemNum += (s32)cNtf.GetNruInfoList().size();
    SetActualResultNum(nRecvItemNum);
}

void CCuRcsQueryRecordRsp::push_back(CRcsQueryRecordNtfReq &cNtf, const CPeQueryTask *pcQueryTask)
{
    CMS_ASSERT(NULL != pcQueryTask);
    int nRecvItemNum = GetActualResultNum();

    TRecDurationList::iterator pItem = cNtf.GetRecDurationList().begin();
    while ( pItem != cNtf.GetRecDurationList().end() )
    {
        GetDurantionList().push_back(*pItem);
        pItem++;
    }
    nRecvItemNum += (s32)cNtf.GetRecDurationList().size();
    SetActualResultNum(nRecvItemNum);
}

void CCuRcsQueryPuChnBindingRsp::push_back(CRcsQueryPuChnBindingNtfReq &cNtf, const CPeQueryTask* pcQueryTask)
{
    CMS_ASSERT(NULL != pcQueryTask);
    int nRecvItemNum = GetActualResultNum();

    TChannelList::iterator pItem = cNtf.GetPuChnList().begin();
    while ( pItem != cNtf.GetPuChnList().end() )
    {
        GetChnList().push_back(*pItem);
        pItem++;
    }
    nRecvItemNum += (s32)cNtf.GetPuChnList().size();
    SetActualResultNum(nRecvItemNum);
}

void CCuRcsQueryRecordParamRsp::push_back(CRcsQueryRecordParamNtfReq &cNtf, const CPeQueryTask* pcQueryTask)
{
    CMS_ASSERT(NULL != pcQueryTask);
    int nRecvItemNum = GetActualResultNum();

    TRecordParamList::iterator pItem = cNtf.GetRecordParamList().begin();
    while ( pItem != cNtf.GetRecordParamList().end() )
    {
        GetRecordParamList().push_back(*pItem);
        pItem++;
    }
    nRecvItemNum += (s32)cNtf.GetRecordParamList().size();
    SetActualResultNum(nRecvItemNum);
}

void CCuTvWallSchemeQueryRsp::push_back(CTvWallSchemeNtfReq &cNtf, const CPeQueryTask* pcQueryTask)
{
    CMS_ASSERT(NULL != pcQueryTask);
    int nRecvItemNum = GetActualResultNum();

    CTTvWallSchemeList::iterator pItem = cNtf.GetSchemeList().begin();
    while ( pItem != cNtf.GetSchemeList().end() )
    {
        GetTvWallScheme().push_back(*pItem);
        pItem++;
    }
    nRecvItemNum += (s32)cNtf.GetSchemeList().size();
    SetActualResultNum(nRecvItemNum);
}

void CCuMapQryRsp::push_back(CMapQryNtfReq &cNtf, const CPeQueryTask* pcQueryTask)
{
    CMS_ASSERT(NULL != pcQueryTask);
    int nRecvItemNum = GetActualResultNum();

    vector<MapInfo>::iterator pItem = cNtf.GetMapInfos().begin();
    while ( pItem != cNtf.GetMapInfos().end() )
    {
        GetMapInfos().push_back(*pItem);
        pItem++;
    }
    nRecvItemNum += (s32)cNtf.GetMapInfos().size();
    SetActualResultNum(nRecvItemNum);
}

void CCuMapElementQryRsp::push_back(CMapElementQryNtfReq &cNtf, const CPeQueryTask* pcQueryTask)
{
    CMS_ASSERT(NULL != pcQueryTask);
    int nRecvItemNum = GetActualResultNum();

    vector<MapElement>::iterator pItem = cNtf.GetMapElements().begin();
    while ( pItem != cNtf.GetMapElements().end() )
    {
        GetMapElement().push_back(*pItem);
        pItem++;
    }
    nRecvItemNum += (s32)cNtf.GetMapElements().size();
    SetActualResultNum(nRecvItemNum);
}



void CCuMapElementSearchRsp::push_back(CMapElementSearchNtfReq &cNtf, const CPeQueryTask *pcQueryTask)
{
	CMS_ASSERT(NULL != pcQueryTask);
	int nRecvItemNum = GetActualResultNum();

	vector<MapElement>::iterator pItem = cNtf.GetMapElements().begin();
	while (pItem != cNtf.GetMapElements().end())
	{
		GetMapElement().push_back(*pItem);
		pItem++;
	}
	nRecvItemNum += (s32)cNtf.GetMapElements().size();
	SetActualResultNum(nRecvItemNum);
}

void CCuAlarmLinkQueryRsp::push_back(CAlarmLinkNtfReq &cNtf, const CPeQueryTask* pcQueryTask)
{
    CMS_ASSERT(NULL != pcQueryTask);
    int nRecvItemNum = GetActualResultNum();

    CAlarmLinkageList::iterator pItem = cNtf.GetCfgList().begin();
    while ( pItem != cNtf.GetCfgList().end() )
    {
        GetAlarmLink().push_back(*pItem);
        pItem++;
    }
    nRecvItemNum += (s32)cNtf.GetCfgList().size();
    SetActualResultNum(nRecvItemNum);
}

void CCuOperateLogQryRsp::push_back(COperateLogQryNtfReq &cNtf, const CPeQueryTask* pcQueryTask)
{
    CMS_ASSERT(NULL != pcQueryTask);
    int nRecvItemNum = GetActualResultNum();

    vector<TOperateLogInfo>::iterator pItem = cNtf.GetInfos().begin();
    while ( pItem != cNtf.GetInfos().end() )
    {
        GetInfo().push_back(*pItem);
        pItem++;
    }
    nRecvItemNum += (s32)cNtf.GetInfos().size();
    SetActualResultNum(nRecvItemNum);
}

void CCuGpsTraceQryRsp::push_back(CGpsTraceQryNtfReq &cNtf, const CPeQueryTask *pcQueryTask)
{
	CMS_ASSERT(NULL != pcQueryTask);
	int nRecvItemNum = GetActualResultNum();

	vector<TGpsStatus>::iterator pItem = cNtf.GetResults().begin();
	while ( pItem != cNtf.GetResults().end() )
	{
		GetInfo().push_back(*pItem);
		pItem++;
	}
	nRecvItemNum += (s32)cNtf.GetResults().size();
	SetActualResultNum(nRecvItemNum);
}

void CCuDeviceLogQryRsp::push_back(CDeviceLogQryNtfReq &cNtf, const CPeQueryTask* pcQueryTask)
{
    CMS_ASSERT(NULL != pcQueryTask);
    int nRecvItemNum = GetActualResultNum();

    vector<TDeviceLogInfo>::iterator pItem = cNtf.GetInfos().begin();
    while ( pItem != cNtf.GetInfos().end() )
    {
        GetInfo().push_back(*pItem);
        pItem++;
    }
    nRecvItemNum += (s32)cNtf.GetInfos().size();
    SetActualResultNum(nRecvItemNum);
}

void CCuAlarmLogQryRsp::push_back(CAlarmLogQryNtfReq &cNtf, const CPeQueryTask* pcQueryTask)
{
    CMS_ASSERT(NULL != pcQueryTask);
    int nRecvItemNum = GetActualResultNum();

    vector<TAlarmLogInfo>::iterator pItem = cNtf.GetInfos().begin();
    while ( pItem != cNtf.GetInfos().end() )
    {
        GetInfo().push_back(*pItem);
        pItem++;
    }
    nRecvItemNum += (s32)cNtf.GetInfos().size();
    SetActualResultNum(nRecvItemNum);
}

void CCuUserCustomDataQryRsp::push_back( CUserCustomDataQryNtfReq &cNtf, const CPeQueryTask *pcQueryTask )
{
    CMS_ASSERT(NULL != pcQueryTask);
    int nRecvItemNum = GetActualResultNum();

    vector<TUserCustomDataInfo>::iterator pItem = cNtf.GetInfos().begin();
    while ( pItem != cNtf.GetInfos().end() )
    {
        infos.push_back(*pItem);
        pItem++;
    }
    nRecvItemNum += (s32)cNtf.GetInfos().size();
    SetActualResultNum(nRecvItemNum);
}

void CCuRcsQueryDiskInfoRsp::push_back(CRcsQueryDiskInfoNtfReq &cNtf, const CPeQueryTask* pcQueryTask)
{
    CMS_ASSERT(NULL != pcQueryTask);
    int nRecvItemNum = GetActualResultNum();

    TDiskInfoList::iterator pItem = cNtf.GetDiskInfoList().begin();
    while ( pItem != cNtf.GetDiskInfoList().end() )
    {
        GetDiskInfo().push_back(*pItem);
        pItem++;
    }
    nRecvItemNum += (s32)cNtf.GetDiskInfoList().size();
    SetActualResultNum(nRecvItemNum);
}

void CCuMapAreaQryRsp::push_back(CMapAreaQryNtfReq &cNtf, const CPeQueryTask *pcQueryTask)
{
    CMS_ASSERT(NULL != pcQueryTask);
    int nRecvItemNum = GetActualResultNum();

    vector<MapArea>::iterator pItem = cNtf.GetResults().begin();
    while ( pItem != cNtf.GetResults().end() )
    {
        GetResults().push_back(*pItem);
        pItem++;
    }
    nRecvItemNum += (s32)cNtf.GetResults().size();
    SetActualResultNum(nRecvItemNum);
}

void CCuElementImageQryRsp::push_back(CElementImageQryNtfReq &cNtf, const CPeQueryTask *pcQueryTask)
{
    CMS_ASSERT(NULL != pcQueryTask);
    int nRecvItemNum = GetActualResultNum();

    vector<string>::iterator pItem = cNtf.GetImageList().begin();
    while ( pItem != cNtf.GetImageList().end() )
    {
        GetImageList().push_back(*pItem);
        pItem++;
    }
    nRecvItemNum += (s32)cNtf.GetImageList().size();
    SetActualResultNum(nRecvItemNum);
}

void CCuPuRecQryRsp::push_back(CPuRecQryNtfReq &cNtf, const CTask *pcQueryTask)
{
    CMS_ASSERT(NULL != pcQueryTask);
    int nRecvItemNum = GetActualResultNum();

    CPuRecInfoList::iterator pItem = cNtf.GetRecInfoList().begin();
    while ( pItem != cNtf.GetRecInfoList().end() )
    {
        GetRecInfoList().push_back(*pItem);
        pItem++;
    }
    nRecvItemNum += (s32)cNtf.GetRecInfoList().size();
    SetActualResultNum(nRecvItemNum);
}

void CCuVideosourcePresetcfgQryRsp::push_back(CVideosourcePresetcfgQryNtfReq &cNtf, const CPeQueryTask *pcQueryTask)
{
    CMS_ASSERT(NULL != pcQueryTask);
    int nRecvItemNum = GetActualResultNum();

    vector<TVideosourcePresetCfgInfo>::iterator pItem = cNtf.GetInfos().begin();
    while ( pItem != cNtf.GetInfos().end() )
    {
        GetInfos().push_back(*pItem);
        pItem++;
    }
    nRecvItemNum += (s32)cNtf.GetInfos().size();
    SetActualResultNum(nRecvItemNum);
}

void CCuQueryCAPSDeviceListRsp::push_back(CQueryDeviceListNtfReq &cNtf, const CPeQueryTask *pcQueryTask)
{
	CMS_ASSERT(NULL != pcQueryTask);
	int nRecvItemNum = GetActualResultNum();

	CWechatDeviceList::iterator pItem = cNtf.GetDevList().begin();
	while ( pItem != cNtf.GetDevList().end() )
	{
		GetDevList().push_back(*pItem);
		pItem++;
	}
	nRecvItemNum += (s32)cNtf.GetDevList().size();
	SetActualResultNum(nRecvItemNum);
}


void CCuDeviceAutoAssignToUserQryRsp::push_back( CDeviceAutoAssignToUserQryNtfReq &cNtf, const CQueryTask *pcQueryTask)
{
	CMS_ASSERT(NULL != pcQueryTask);
	CMS_ASSERT(NULL != pcQueryTask->m_pc3acAgent);
	int nRecvItemNum = GetActualResultNum();

	C3acAgent *pc3acAgent = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(pcQueryTask->m_pcCuAgent->m_str3asName, pcQueryTask->GetInstance());;
	if (NULL != pc3acAgent)
	{
		vector<CDeviceAutoAssignInfo>::iterator pItem = cNtf.GetDeviceAutoAssignInfo().begin();
		string strDevGrpId;
		while ( pItem != cNtf.GetDeviceAutoAssignInfo().end() )
		{
			strDevGrpId = pItem->GetDevGrpId() + "@" + pc3acAgent->GetDomainNameByDomainID(pItem->GetDomainId());
			pItem->SetDevGrpId(strDevGrpId);
			this->GetDeviceAutoAssignInfo().push_back(*pItem);
			pItem++;
		}
	}
	
	nRecvItemNum += (s32)cNtf.GetDeviceAutoAssignInfo().size();
	SetActualResultNum(nRecvItemNum);
}

void CCuDeviceAutoAssignToUserGroupQryRsp::push_back( CDeviceAutoAssignToUserGroupQryNtfReq &cNtf, const CQueryTask *pcQueryTask)
{
	CMS_ASSERT(NULL != pcQueryTask);
	CMS_ASSERT(NULL != pcQueryTask->m_pc3acAgent);
	int nRecvItemNum = GetActualResultNum();

	C3acAgent *pc3acAgent = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(pcQueryTask->m_pcCuAgent->m_str3asName, pcQueryTask->GetInstance());;
	if (NULL != pc3acAgent)
	{
		vector<CDeviceAutoAssignInfo>::iterator pItem = cNtf.GetDeviceAutoAssignInfo().begin();
		string strDevGrpId;
		while ( pItem != cNtf.GetDeviceAutoAssignInfo().end() )
		{
			strDevGrpId = pItem->GetDevGrpId() + "@" + pc3acAgent->GetDomainNameByDomainID(pItem->GetDomainId());
			pItem->SetDevGrpId(strDevGrpId);
			this->GetDeviceAutoAssignInfo().push_back(*pItem);
			pItem++;
		}
	}
	
	nRecvItemNum += (s32)cNtf.GetDeviceAutoAssignInfo().size();
	SetActualResultNum(nRecvItemNum);
}

void CCuDeviceGrpPathQryRsp::push_back( CDeviceGrpPathQryNtfReq &cNtf, const CQueryTask *pcQueryTask)
{
	CMS_ASSERT(NULL != pcQueryTask);
	CMS_ASSERT(NULL != pcQueryTask->m_pc3acAgent);
	int nRecvItemNum = GetActualResultNum();

	C3acAgent *pc3acAgent = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(pcQueryTask->m_pcCuAgent->m_str3asName, pcQueryTask->GetInstance());
	if (NULL != pc3acAgent)
	{
		vector<CDeviceGroupInfo>::iterator pItem = cNtf.GetInfos().begin();
		
		while (pItem != cNtf.GetInfos().end())
		{
			string strTmp;
			string strDomainName;

			strTmp = pItem->GetDeviceGroupID();
			if ( !strTmp.empty() )
			{
				strDomainName = pc3acAgent->GetDomainNameByDomainID(pItem->GetDomainUUID());
				if ( !strDomainName.empty() )
				{
					pItem->SetDeviceGroupID(strTmp  + "@" + strDomainName);
					strDomainName.clear();
				}
				strTmp.clear();
			}

			strTmp = pItem->GetDeviceParentGroupID();
			if ( !strTmp.empty() )
			{
				strDomainName = pc3acAgent->GetDomainNameByDomainID(pItem->GetParentDomainUUID());
				if ( !strDomainName.empty() )
				{
					pItem->SetDeviceParentGroupID(strTmp  + "@" + strDomainName);
					strDomainName.clear();
				}
				strTmp.clear();
			}

			this->GetDeviceGroupInfo().push_back(*pItem);
			pItem++;
		}
	}

	nRecvItemNum += (s32)cNtf.GetInfos().size();
	SetActualResultNum(nRecvItemNum);
}

void CCuDevTreeQryRsp::push_back(const CDevTreeQryNtfReq &cNtf, const CQueryTask *pcQueryTask)
{
	CMS_ASSERT(NULL != pcQueryTask);
	CMS_ASSERT(NULL != pcQueryTask->m_pc3acAgent);
	int nRecvItemNum = GetActualResultNum();

	this->GetDevTreeInfo() = cNtf.GetInfos();

	nRecvItemNum += (s32)cNtf.GetInfos().size();
	SetActualResultNum(nRecvItemNum);
}

/////////////////////////////////////////////////////////////////////////////////
//第三方呼叫task
CCallPuInterAction::CCallPuInterAction(CInstExt *pcInst, const string& strCuSession, PTRecvNode ptRecvNode) : CSipTask(pcInst)
{
    CMS_ASSERT(!strCuSession.empty());
    CMS_ASSERT(ptRecvNode != NULL);

    m_strCuSession = strCuSession;
    m_ptRecvNode = ptRecvNode;
    m_nplayId = INVALID_PLAY_ID;
	m_bNewInvite = false;
	m_eCallPuTaskType = CALL_PU_TASK_TYPE_PUNCH;
}

CCallPuInterAction::~CCallPuInterAction()
{
    TerminateTask();
}

void CCallPuInterAction::TerminateTask()
{
    //     if ( GetState() == WAIT_RESPONSE_STATE && m_ptRecvNode != NULL )
    //     {
    //         CCuStartThirdPartyCallRsp cRsp;
    //         cRsp.SetErrorCode(ERR_CUI_TASK_DEL);
    // 
    //         TASKLOG(NORMAL_TASK, ERROR_LEV, "向CU用户会话[%s]发送CCuStartThirdPartyCallReq，交互任务CInviteInterAction被销毁，向CU发送应答，错误码[%d]\n",
    //             m_strCuSession.c_str(), cRsp.GetErrorCode());
    //         SendRspToCu<CCuStartThirdPartyCallRsp>(cRsp, m_ptRecvNode);
    //     }

    //将对应的InviteTask的绑定解除
    if ( INVALID_PLAY_ID != m_nplayId )
	{
		if (m_eCallPuTaskType == CALL_PU_TASK_TYPE_PUNCH)
		{
			CCallPuTask *pcInviteTask = (CCallPuTask *)GetCallPuTask(m_nplayId);
			if (NULL != pcInviteTask)
			{
				if (pcInviteTask->m_pcCallPuInterActTask == this)
				{
					pcInviteTask->m_pcCallPuInterActTask = NULL;
				}
			}
		}
		else if (m_eCallPuTaskType == CALL_PU_TASK_TYPE_NAT)
		{
			CCallPuTaskEx *pcInviteTask = (CCallPuTaskEx *)GetCallPuTask(m_nplayId);
			if (NULL != pcInviteTask)
			{
				if (pcInviteTask->m_pcCallPuInterActTask == this)
				{
					pcInviteTask->m_pcCallPuInterActTask = NULL;
				}
			}
		}
		else
		{
			CCallPuTaskNatSwitch *pcInviteTask = (CCallPuTaskNatSwitch *)GetCallPuTask(m_nplayId);
			if (NULL != pcInviteTask)
			{
				if (pcInviteTask->m_pcCallPuInterActTask == this)
				{
					pcInviteTask->m_pcCallPuInterActTask = NULL;
				}
			}
		}
    }
}

CSipTask *CCallPuInterAction::GetCallPuTask(s32 nPlayId)
{
    CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession, GetInstance());
    if(NULL == pcCuAgent)
    {
        return NULL;
    }

    u32 dwTaskId = INVALID_TASKNO;
    if ( ! pcCuAgent->m_cCallPuTaskSet.Find(m_nplayId, dwTaskId) )
    {
        return NULL;
    }

    if ( INVALID_TASKNO == dwTaskId )
    {
        return NULL;
    }

    return (CSipTask *)(GetInstance()->FindTask(dwTaskId));
}
// string CCallPuInterAction::GetErrorCode(u32 dwErrorCode)
// {
//     if( CMS_SUCCESS == dwErrorCode)
//     {
//         return GetLanDesc(LAN_DESC_SUCCESS);
//     }
//     else if( ERR_CUI_OPR_TIME_OUT == dwErrorCode)
//     {
//         return GetLanDesc(LAN_DESC_TIMEOUT);
//     }
//     else
//     {
//         return GetLanDesc(LAN_DESC_FAIL);
//     }
// }
// void CCallPuInterAction::GetOptLogDesc(const string& Opt, const string& OptDevInfo)
// {
//     m_strLogDesc = GetLanDesc(Opt) + "(" + OptDevInfo + ")";
// }
// void CCallPuInterAction::GetOptLogDesc(const string& OptType,const string& Type, const string& OptDevInfo)
// {
//     m_strLogDesc = GetLanDesc(OptType) + GetLanDesc(Type) + "(" + OptDevInfo + ")";
// }

// void CCallPuInterAction::WriteLog(u32 dwErrorCode)
// {
//     m_strLogDesc += GetErrorCode(dwErrorCode);
//     CUsrOprLogTask* pcTask = new CUsrOprLogTask(GetInstance(), m_strCuSession, 
//         string(DEV_TYPE_UAS) + "@" + GetDomainName(),
//         enUserOpTypeCall,
//         (u32)dwErrorCode,
//         m_strLogDesc
//         );
//     pcTask->InitStateMachine();
//     pcTask->StartSendLog();
// }

void CCallPuInterAction::InitStateMachine()
{
    CStateProc cNotRequestProc;
    cNotRequestProc.ProcMsg = (CSipTask::PFProcMsg)&CCallPuInterAction::NotReqProcMsg;
    cNotRequestProc.TimerPoll = (CSipTask::PFTimerPoll)&CCallPuInterAction::NotReqTimerPoll;
    cNotRequestProc.ProcErrMsg = (CSipTask::PFProcMsg)&CCallPuInterAction::NotReqProcMsg;
    AddRuleProc(NOT_REQUEST_STATE, cNotRequestProc);

    CStateProc cWaitRspProc;
    cWaitRspProc.ProcMsg = (CSipTask::PFProcMsg)&CCallPuInterAction::WaitRspProcMsg;
    cWaitRspProc.TimerPoll = (CSipTask::PFTimerPoll)&CCallPuInterAction::WaitRspTimerPoll;
    cWaitRspProc.ProcErrMsg = (CSipTask::PFProcMsg)&CCallPuInterAction::WaitRspProcMsg;
    AddRuleProc(WAIT_RESPONSE_STATE, cWaitRspProc);

    NextState(NOT_REQUEST_STATE);
}

u32 CCallPuInterAction::NotReqProcMsg( CMessage *const pcMsg )
{
    TASKLOG(NORMAL_TASK, CRITICAL_LEV, "在[CCallPuInterAction::NotReqProcMsg]收到消息[%s--%hu]\n",
        OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

	u32 dwRet = PROCMSG_OK;

    m_dwEvent = pcMsg->event;
    switch (pcMsg->event)
    {
    case START_THIRD_PARTY_CALL_REQ:
        {
            CCuStartThirdPartyCallReq *pcReq = (CCuStartThirdPartyCallReq *)pcMsg->content;
            //第三方呼叫的源是pu
            m_nplayId = pcReq->GetSrcChn().GetChnNO();
            //缓存req消息
            m_strSrcReq = pcReq->ToXml();

            //1 鉴权
            bool bAuthRet = true; //鉴权结果
            CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession, GetInstance());
            CMS_ASSERT(NULL != pcCuAgent);
			if (NULL == pcCuAgent)
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户[%s]Agent已不存在\n",m_strCuSession.c_str());

				CCuStartThirdPartyCallRsp cRsp;
				cRsp.SetErrorCode(ERR_CUI_CU_NOT_ONLINE);
				SendRspToCu<CCuStartThirdPartyCallRsp>(cRsp, m_ptRecvNode);
				dwRet = PROCMSG_DEL;
				break;
			}

            do 
            {                             
                string strDomainName = GetDomainNameWithoutPreNameFromURI(pcReq->GetSrcChn().GetDevUri());
                if ( ! pcCuAgent->IsExistDomainInfo(strDomainName))
                {
                    bAuthRet = false;
                    break;
				}

				if (pcCuAgent->m_pcUserAccount->IsSuperAdmin())
                {
                    bAuthRet = true;
                    break;
                }

                CPrivilege cPri = pcCuAgent->m_pcUserAccount->GetUserPri();
                if(!cPri.IsEmpty())
                {
                    if(cPri.HasPri(CUserPrivilege::en_Call_Ctrl))
                    {
                        bAuthRet = true;
                        break;
                    }
                    else
                    {
                        bAuthRet = false;
                    }
                }
                else
                {
                    bAuthRet = false;
                    break;
                }

                string uuid = GetNameWithoutDomainFromURI(pcReq->GetDstChn().GetDevUri());
				if (!g_cRedisDb.IsUserHasDev(pcCuAgent->m_pcUserAccount->m_tUserID, uuid))
				{
					TASKLOG(NORMAL_TASK, ERROR_LEV, "用户[%s]没有划归设备[%s]\n", pcCuAgent->m_pcUserAccount->m_tUserID.c_str(), uuid.c_str());
					bAuthRet = false;
					break;
				}
            } while (0);

            if (!bAuthRet)
            {
                TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户会话[%s]发送Invite请求，但是对源通道[%s--%d]无权限\n",
                    m_strCuSession.c_str(),
                    pcReq->GetSrcChn().GetDevUri().c_str(), pcReq->GetSrcChn().GetChnNO());

                CCuStartThirdPartyCallRsp cRsp;
                cRsp.SetErrorCode(ERR_CUI_CU_NO_PERMISSION);
                SendRspToCu<CCuStartThirdPartyCallRsp>(cRsp, m_ptRecvNode);
                m_nplayId = INVALID_PLAY_ID;

                WriteLog(pcReq->GetDstChn(), ERR_CUI_CU_NO_PERMISSION);
                //                WriteLog(ERR_CUI_CU_NO_PERMISSION);
                //delete this;
                dwRet = PROCMSG_DEL;
				break;
            }

			if (pcCuAgent->CuiTransStream4Pu())
			{
				m_eCallPuTaskType = CALL_PU_TASK_TYPE_CUI_NatSwitch;
				ProcStartCallPuReqNatSwitch(pcMsg);
			}
			else
			{
				m_eCallPuTaskType = CALL_PU_TASK_TYPE_NAT;
				ProcStartCallPuReqEx(pcMsg);
			}

            NextState(WAIT_RESPONSE_STATE);
			dwRet = PROCMSG_OK;
        }
        break;
    case STOP_THIRD_PARTY_CALL_REQ:
        {
            CCuStopThirdPartyCallReq *pcReq = (CCuStopThirdPartyCallReq *)pcMsg->content;
            //第三方呼叫的源是cu
            m_nplayId = pcReq->GetSrcChn().GetChnNO();
            //缓存req消息
            m_strSrcReq = pcReq->ToXml();

			CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession, GetInstance());
			CMS_ASSERT(NULL != pcCuAgent);
			if (NULL == pcCuAgent)
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户[%s]Agent已不存在\n",m_strCuSession.c_str());

				CCuStopThirdPartyCallRsp cRsp;
				cRsp.SetErrorCode(ERR_CUI_CU_NOT_ONLINE);
				SendRspToCu<CCuStopThirdPartyCallRsp>(cRsp, m_ptRecvNode);
				dwRet = PROCMSG_DEL;
				break;
			}

            //停止操作不鉴权
			if (pcCuAgent->CuiTransStream4Pu())
			{
				m_eCallPuTaskType = CALL_PU_TASK_TYPE_CUI_NatSwitch;
				dwRet = ProcStopCallPuReqNatSwitch(pcMsg);
			}
			else
			{
				m_eCallPuTaskType = CALL_PU_TASK_TYPE_NAT;
				dwRet = ProcStopCallPuReqEx(pcMsg);
			}
			
			if (dwRet != PROCMSG_OK)
			{
				dwRet = PROCMSG_DEL;
				break;
			}

			NextState(WAIT_RESPONSE_STATE);
        }
        break;
    default:
		{
			dwRet = PROCMSG_FAIL;
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"收到未知消息");
        }
        break;
    }
    return dwRet;
}

u32 CCallPuInterAction::WaitRspProcMsg(CMessage *const pcMsg)
{
    TASKLOG(NORMAL_TASK, CRITICAL_LEV, "在[CCallPuInterAction::WaitRspProcMsg]收到消息[%s--%hu]\n",
        OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

    u32 dwRet = PROCMSG_OK;

    switch (pcMsg->event)
    {
    case OSP_SIP_MSG_PROC_FAIL:
    case OSP_SIP_DIALOG_TERMINATE:
        {
            CCuStartThirdPartyCallRsp cRsp;
            cRsp.SetErrorCode(ERR_CUI_SIP_MSG_FAIL);

            TASKLOG(NORMAL_TASK, ERROR_LEV, "向CU用户会话[%s]发送失败应答消息，错误码[%d]\n",
                m_strCuSession.c_str(), cRsp.GetErrorCode());
            SendRspToCu<CCuStartThirdPartyCallRsp>(cRsp, m_ptRecvNode);
            //任务完成, 销毁之
            NextState(NOT_REQUEST_STATE);
            switch(m_dwEvent)
            {
            case START_THIRD_PARTY_CALL_REQ:
                {
                    CCuStartThirdPartyCallReq cReq;
                    cReq.FromXml(m_strSrcReq);
                    WriteLog(cReq.GetDstChn(), ERR_CUI_DIALOG_NOT_ESTABLISHED);
                }
                break;
            case STOP_THIRD_PARTY_CALL_REQ:
                {
                    CCuStopThirdPartyCallReq cReq;
                    cReq.FromXml(m_strSrcReq);
                    WriteLog(cReq.GetDstChn(), ERR_CUI_DIALOG_NOT_ESTABLISHED);
                }
                break;
            default:
                break;
            }           
            dwRet =  PROCMSG_DEL;
            delete this;
        }
        break;
    case START_THIRD_PARTY_CALL_RSP:
        {
			CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession, GetInstance());
			CMS_ASSERT(NULL != pcCuAgent);
			if (NULL == pcCuAgent)
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户[%s]Agent已不存在\n",m_strCuSession.c_str());

				CCuStartThirdPartyCallRsp cRsp;
				cRsp.SetErrorCode(ERR_CUI_CU_NOT_ONLINE);
				SendRspToCu<CCuStartThirdPartyCallRsp>(cRsp, m_ptRecvNode);
				dwRet =  PROCMSG_DEL;
				delete this;
				break;
			}

			if (pcCuAgent->CuiTransStream4Pu())
			{
				ProcStartCallPuRspNatSwitch(pcMsg);
			}
			else
			{
				ProcStartCallPuRspEx(pcMsg);
			}

            //任务完成, 销毁之
            NextState(NOT_REQUEST_STATE);
            dwRet =  PROCMSG_DEL;
            delete this;
        }
        break;
    case STOP_THIRD_PARTY_CALL_RSP:
        {
            COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
            CCuStopThirdPartyCallRsp cRsp;
            pcOspSipMsg->GetMsgBody(cRsp);

            SendRspToCu<CCuStopThirdPartyCallRsp>(cRsp, m_ptRecvNode);
            CCuStopThirdPartyCallReq cReq;
            cReq.FromXml(m_strSrcReq);
            WriteLog(cReq.GetDstChn(), (u32)cRsp.GetErrorCode());
            //任务完成, 销毁之
            NextState(NOT_REQUEST_STATE);
            dwRet =  PROCMSG_DEL;
            delete this;
        }
        break;
    default:
        {
            dwRet = PROCMSG_FAIL;
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"收到未知消息");
        }
        break;
    }
    return dwRet;
}

u32 CCallPuInterAction::NotReqTimerPoll()
{
    if ( GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout() )
    {
        TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户会话[%s]等待START_THIRD_PARTY_CALL_REQ应答超时\n",
            m_strCuSession.c_str());
        return TIMERPOLL_DEL;
    }
    return TIMERPOLL_DONE;
}

u32 CCallPuInterAction::WaitRspTimerPoll()
{
    if ( GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout() )
    {
        TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户会话[%s]等待START_THIRD_PARTY_CALL_RSP应答超时\n",
            m_strCuSession.c_str());

        CCuStartThirdPartyCallRsp cRsp;
        cRsp.SetErrorCode(ERR_CUI_OPR_TIME_OUT);
        SendRspToCu<CCuStartThirdPartyCallRsp>(cRsp, m_ptRecvNode);
        //        WriteLog((u32)cRsp.GetErrorCode());
        NextState(NOT_REQUEST_STATE);
        return TIMERPOLL_DEL;
    }
    return TIMERPOLL_DONE;
}
void CCallPuInterAction::WriteLog(const TChannel& tDevChn, const u32 dwErrorCode)
{
    CGetPuData *pcTask = new CGetPuData(GetInstance(), m_strCuSession, tDevChn, dwErrorCode, m_strSrcReq, m_dwEvent);
    pcTask->InitStateMachine(); 
	if (pcTask->StartWriteLog() != PROCMSG_OK)
	{
		delete pcTask;
		pcTask = NULL;
	}
}

void CCallPuInterAction::ProcStartCallPuReq(CMessage *const pcMsg)
{
	TASKLOG(NORMAL_TASK,EVENT_LEV, "ProcStartCallPuReq收到未知消息[%s--%hu]\n",
		OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

	CCuStartThirdPartyCallReq *pcReq = (CCuStartThirdPartyCallReq *)pcMsg->content;
	CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession, GetInstance());
	CMS_ASSERT(NULL!=pcCuAgent&&"当前cu登陆task为空");

	pcReq->GetSdp().SetIsNeedPunch(pcCuAgent->m_bIsSupportNA);

	//查找InviteTask
	CCallPuTask *pcCallPuTask = (CCallPuTask *)GetCallPuTask(m_nplayId);
	if ( NULL == pcCallPuTask )
	{
		pcCallPuTask = new CCallPuTask(GetInstance(), m_strCuSession, this);
		pcCallPuTask->InitStateMachine();
		//将当前任务登记到CuAgent中
		pcCuAgent->m_cCallPuTaskSet.Insert(pcReq->GetSrcChn().GetChnNO(), pcCallPuTask->GetTaskNO());
		m_bNewInvite = true;
	}

    TSipURI tDstUri;
    bool bSuccessful = CStreamDecentrationAdapter::FindPuiUri( tDstUri, pcReq->GetDstChn().GetDevUri(), pcCuAgent->m_tCmuURI );
    pcReq->SetSession( pcReq->GetDstChn().GetDevUri() );
    
    //由action发送3pc msg消息
	//PostMsgReq<CCuStartThirdPartyCallReq>(*pcReq, pcCuAgent->m_tCmuURI);

    PostMsgReq<CCuStartThirdPartyCallReq>(*pcReq, tDstUri);
}

void CCallPuInterAction::ProcStartCallPuReqEx(CMessage *const pcMsg)
{
	TASKLOG(NORMAL_TASK,EVENT_LEV, "ProcStartCallPuReqEx收到未知消息[%s--%hu]\n",
		OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

	CCuStartThirdPartyCallReq *pcReq = (CCuStartThirdPartyCallReq *)pcMsg->content;
	CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession, GetInstance());
	CMS_ASSERT(NULL!=pcCuAgent&&"当前cu登陆task为空");

	if (NATModeInner == pcCuAgent->m_ePlatNat)
	{
		pcReq->GetSdp().SetIsNatPacketMode(true);
		pcReq->GetSdp().SetIsNeedPunch(false);
		SwitchCuAddrToNatAddr(pcReq->GetSdp());
		//更新本地保存的SDP地址
		m_strSrcReq = pcReq->ToXml();
	}

	//查找InviteTask
	CCallPuTaskEx *pcCallPuTask = (CCallPuTaskEx *)GetCallPuTask(m_nplayId);
	if ( NULL == pcCallPuTask )
	{
		pcCallPuTask = new CCallPuTaskEx(GetInstance(), m_strCuSession,m_strSrcReq,this);
		pcCallPuTask->InitStateMachine();
		//将当前任务登记到CuAgent中
		pcCuAgent->m_cCallPuTaskSet.Insert(pcReq->GetSrcChn().GetChnNO(), pcCallPuTask->GetTaskNO());
		PushMediaTask(m_strCuSession, m_nplayId, pcCallPuTask);
		m_bNewInvite = true;
	}

    TSipURI tDstUri;
    CStreamDecentrationAdapter::FindPuiUri( tDstUri, pcReq->GetDstChn().GetDevUri(), pcCuAgent->m_tCmuURI );
    pcReq->SetSession( pcReq->GetDstChn().GetDevUri() );

    //由action发送3pc msg消息
	//PostMsgReq<CCuStartThirdPartyCallReq>(*pcReq, pcCuAgent->m_tCmuURI);
    PostMsgReq<CCuStartThirdPartyCallReq>(*pcReq, tDstUri );
}

void CCallPuInterAction::ProcStartCallPuReqNatSwitch(CMessage *const pcMsg)
{
	TASKLOG(NORMAL_TASK,EVENT_LEV, "收到CU消息[%s--%hu]\n",
		OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

	CCuStartThirdPartyCallReq *pcReq = (CCuStartThirdPartyCallReq *)pcMsg->content;
	CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession, GetInstance());
	CMS_ASSERT(NULL!=pcCuAgent&&"当前cu登陆task为空");

	// CUI转发码流，不需要PUNCH
	pcReq->GetSdp().SetIsNatPacketMode(false);
	pcReq->GetSdp().SetIsNeedPunch(false);

	if (NATModeInner == pcCuAgent->m_eClientNat)
	{
		SwitchCuAddrToNatAddrNatSwitch(pcReq->GetSdp());
	}

	//更新本地保存的SDP地址
	m_strSrcReq = pcReq->ToXml();

	// 将码流的源改为CUI地址
	SwitchToCuiNatSwitchAddressForPlat(pcReq->GetSdp());

	m_strSrcReqNatSwitch = pcReq->ToXml();

	//查找InviteTask
	CCallPuTaskNatSwitch *pcCallPuTask = (CCallPuTaskNatSwitch *)GetCallPuTask(m_nplayId);
	if ( NULL == pcCallPuTask )
	{
		pcCallPuTask = new CCallPuTaskNatSwitch(GetInstance(), m_strCuSession,m_strSrcReq,this);
		pcCallPuTask->InitStateMachine();
		
		//将当前任务登记到CuAgent中
		pcCuAgent->m_cCallPuTaskSet.Insert(pcReq->GetSrcChn().GetChnNO(), pcCallPuTask->GetTaskNO());
		PushMediaTask(m_strCuSession, m_nplayId, pcCallPuTask);
		m_bNewInvite = true;
	}

    TSipURI tDstUri;
    CStreamDecentrationAdapter::FindPuiUri( tDstUri, pcReq->GetDstChn().GetDevUri() ,pcCuAgent->m_tCmuURI );
    pcReq->SetSession( pcReq->GetDstChn().GetDevUri() );

    //由action发送3pc msg消息
	PostMsgReq<CCuStartThirdPartyCallReq>( *pcReq, tDstUri );        
    //由action发送3pc msg消息
    //PostMsgReq<CCuStartThirdPartyCallReq>(*pcReq, pcCuAgent->m_tCmuURI);
}

void CCallPuInterAction::ProcStartCallPuRsp(CMessage *const pcMsg)
{
	COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
	CCuStartThirdPartyCallRsp cRsp;
	cRsp.FromXml(pcOspSipMsg->GetMsgBody());

	CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession, GetInstance());
	CMS_ASSERT(NULL != pcCuAgent&&"当前cu登陆task为空");

	CCuStartEncoderTransNtfReq cuNtfReq;

	CCallPuTask *pcCallBackTask = (CCallPuTask *)GetCallPuTask(m_nplayId);
	if ( NULL == pcCallBackTask )
	{  
		cRsp.SetErrorCode(ERR_CUI_TASK_DEL);
	}
	else if (CMS_SUCCESS != cRsp.GetErrorCode())
	{
		cRsp.SetErrorCode(cRsp.GetErrorCode());

        if (this == pcCallBackTask->m_pcCallPuInterActTask)
        {
            delete pcCallBackTask;
            pcCallBackTask = NULL;
        }
	}
	else
	{
		CInviteReq cReq;
		cReq.FromXml(pcCallBackTask->m_strReq);
		cRsp.SetSdp(cReq.GetSdp());

		if (m_bNewInvite)
		{
			cuNtfReq.SetClientEncChnId(cReq.GetSrcChn().GetChnNO());
			cuNtfReq.SetTMediaDescList(cRsp.GetSdp().GetMediaDescList());
			cuNtfReq.SetTExtInfo(cRsp.GetSdp().GetExtInfo());
		}
	}

	SendRspToCu<CCuStartThirdPartyCallRsp>(cRsp, m_ptRecvNode);

	if (pcCuAgent != NULL
		&& pcCallBackTask != NULL
		&& cRsp.GetErrorCode() == CMS_SUCCESS
		&& m_bNewInvite)
	{
		pcCuAgent->m_cNotifyPool.InsertStartEncoderTransNotify(cuNtfReq);
	}

	m_bNewInvite = false;

	CCuStartThirdPartyCallReq cReq;
	cReq.FromXml(m_strSrcReq);
	WriteLog(cReq.GetDstChn(), (u32)cRsp.GetErrorCode());
}

void CCallPuInterAction::ProcStartCallPuRspEx(CMessage *const pcMsg)
{
	COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
	CCuStartThirdPartyCallRsp cRsp;
	cRsp.FromXml(pcOspSipMsg->GetMsgBody());

	CCuStartEncoderTransNtfReq cuNtfReq;
	CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession, GetInstance());
	CMS_ASSERT(NULL != pcCuAgent&&"当前cu登陆task为空");

	CCallPuTaskEx *pcCallBackTask = (CCallPuTaskEx *)GetCallPuTask(m_nplayId);
	if ( NULL == pcCallBackTask )
	{  
		cRsp.SetErrorCode(ERR_CUI_TASK_DEL);
	}
	else if (CMS_SUCCESS != cRsp.GetErrorCode())
	{
		cRsp.SetErrorCode(cRsp.GetErrorCode());

        if (this == pcCallBackTask->m_pcCallPuInterActTask)
        {
            delete pcCallBackTask;
            pcCallBackTask = NULL;
        }
	}
	else
	{
		CInviteReq cReq;
		cReq.FromXml(pcCallBackTask->m_strReq);
		cRsp.SetSdp(cReq.GetSdp());

		SwitchPlatAddrToNat(cRsp.GetSdp());

		if (m_bNewInvite)
		{
			cuNtfReq.SetClientEncChnId(cReq.GetSrcChn().GetChnNO());
			cuNtfReq.SetTMediaDescList(cRsp.GetSdp().GetMediaDescList());
			cuNtfReq.SetTExtInfo(cRsp.GetSdp().GetExtInfo());
		}
	}

	SendRspToCu<CCuStartThirdPartyCallRsp>(cRsp, m_ptRecvNode);

	if (pcCuAgent != NULL
		&& pcCallBackTask != NULL
		&& cRsp.GetErrorCode() == CMS_SUCCESS
		&& m_bNewInvite)
	{
		pcCuAgent->m_cNotifyPool.InsertStartEncoderTransNotify(cuNtfReq);
	}

	m_bNewInvite = false;

	CCuStartThirdPartyCallReq cReq;
	cReq.FromXml(m_strSrcReq);
	WriteLog(cReq.GetDstChn(), (u32)cRsp.GetErrorCode());
}

void CCallPuInterAction::ProcStartCallPuRspNatSwitch(CMessage *const pcMsg)
{
	COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
	CCuStartThirdPartyCallRsp cRsp;
	cRsp.FromXml(pcOspSipMsg->GetMsgBody());

	CCuStartEncoderTransNtfReq cuNtfReq;

	CCallPuTaskNatSwitch *pcCallBackTask = (CCallPuTaskNatSwitch *)GetCallPuTask(m_nplayId);
	if ( NULL == pcCallBackTask )
	{  
		cRsp.SetErrorCode(ERR_CUI_TASK_DEL);
	}
	else if (CMS_SUCCESS != cRsp.GetErrorCode())
	{
		cRsp.SetErrorCode(cRsp.GetErrorCode());

        if (this == pcCallBackTask->m_pcCallPuInterActTask)
        {
            delete pcCallBackTask;
            pcCallBackTask = NULL;
        }
	}
	else
	{
		CInviteReq cReq;
		cReq.FromXml(pcCallBackTask->m_strCuCallPuReq);

		SwitchToCuiNatSwitchAddressForCu(cReq.GetSdp());

		cReq.GetSdp().SetIsNatPacketMode(false/*true*/);
		cReq.GetSdp().SetIsNeedPunch(false);

		cRsp.SetSdp(cReq.GetSdp());

		if (m_bNewInvite)
		{
			cuNtfReq.SetClientEncChnId(cReq.GetSrcChn().GetChnNO());
			cuNtfReq.SetTMediaDescList(cRsp.GetSdp().GetMediaDescList());
			cuNtfReq.SetTExtInfo(cRsp.GetSdp().GetExtInfo());
		}
	}

	SendRspToCu<CCuStartThirdPartyCallRsp>(cRsp, m_ptRecvNode);

	CCuStartThirdPartyCallReq cReq;
	cReq.FromXml(m_strSrcReq);

	if (pcCallBackTask != NULL
		&& cRsp.GetErrorCode() == CMS_SUCCESS
		&& m_bNewInvite)
	{

		CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession, GetInstance());
		CMS_ASSERT(NULL != pcCuAgent&&"当前cu登陆task为空");
		if (pcCuAgent != NULL)
		{
			pcCuAgent->m_cNotifyPool.InsertStartEncoderTransNotify(cuNtfReq);
		}
	}

	m_bNewInvite = false;

	WriteLog(cReq.GetDstChn(), (u32)cRsp.GetErrorCode());
}

u32 CCallPuInterAction::ProcStopCallPuReq(CMessage *const pcMsg)
{
	CCuStopThirdPartyCallReq *pcReq = (CCuStopThirdPartyCallReq *)pcMsg->content;
	//查找task
	CCallPuTask *pcCallBackTask = (CCallPuTask *)GetCallPuTask(m_nplayId);
	if ( NULL == pcCallBackTask )
	{
		CCuStopThirdPartyCallRsp cRsp;
		cRsp.SetErrorCode(ERR_CUI_TASK_DEL);
		SendRspToCu<CCuStopThirdPartyCallRsp>(cRsp, m_ptRecvNode);
		m_nplayId = INVALID_PLAY_ID;
		WriteLog(pcReq->GetDstChn(), ERR_CUI_TASK_DEL);

		return PROCMSG_DEL;
	}

	//状态不正确，不能取消
	if(CCallPuTask::INVITE_HOLD_STATE != pcCallBackTask->GetState())
	{
		CCuStopThirdPartyCallRsp cRsp;
		cRsp.SetErrorCode(ERR_CUI_DIALOG_NOT_ESTABLISHED);
		SendRspToCu<CCuStopThirdPartyCallRsp>(cRsp, m_ptRecvNode);
		m_nplayId = INVALID_PLAY_ID;
		WriteLog(pcReq->GetDstChn(), ERR_CUI_DIALOG_NOT_ESTABLISHED);

		return PROCMSG_DEL;
	}

	CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession, GetInstance());
	CMS_ASSERT(NULL != pcCuAgent);
	if (NULL == pcCuAgent)
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户[%s]Agent已不存在\n",m_strCuSession.c_str());

		CCuStopThirdPartyCallRsp cRsp;
		cRsp.SetErrorCode(ERR_CUI_CU_NOT_ONLINE);
		SendRspToCu<CCuStopThirdPartyCallRsp>(cRsp, m_ptRecvNode);

		return PROCMSG_DEL;
	}

    TSipURI tDstUri;
    CStreamDecentrationAdapter::FindPuiUri( tDstUri, pcReq->GetDstChn().GetDevUri(), pcCuAgent->m_tCmuURI );
    pcReq->SetSession( pcReq->GetDstChn().GetDevUri() );
    
	if ( PROCMSG_OK != PostMsgReq<CCuStopThirdPartyCallReq>(*pcReq, tDstUri) )
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "发送SIP消息失败[%s]\n", pcReq->ToXml().c_str());

		CCuStopThirdPartyCallRsp cRsp;
		cRsp.SetErrorCode(ERR_CUI_SIP_MSG_FAIL);
		SendRspToCu<CCuStopThirdPartyCallRsp>(cRsp, m_ptRecvNode);

		m_nplayId = INVALID_PLAY_ID;
		WriteLog(pcReq->GetDstChn(), ERR_CUI_SIP_MSG_FAIL);
		return PROCMSG_DEL;
	}

	return PROCMSG_OK;
}

u32 CCallPuInterAction::ProcStopCallPuReqEx(CMessage *const pcMsg)
{
	CCuStopThirdPartyCallReq *pcReq = (CCuStopThirdPartyCallReq *)pcMsg->content;
	//查找task
	CCallPuTaskEx *pcCallBackTask = (CCallPuTaskEx *)GetCallPuTask(m_nplayId);
	if ( NULL == pcCallBackTask )
	{
		CCuStopThirdPartyCallRsp cRsp;
		cRsp.SetErrorCode(ERR_CUI_TASK_DEL);
		SendRspToCu<CCuStopThirdPartyCallRsp>(cRsp, m_ptRecvNode);
		m_nplayId = INVALID_PLAY_ID;
		WriteLog(pcReq->GetDstChn(), ERR_CUI_TASK_DEL);

		return PROCMSG_DEL;
	}

	//状态不正确，不能取消
	if(CCallPuTaskEx::INVITE_HOLD_STATE != pcCallBackTask->GetState())
	{
		CCuStopThirdPartyCallRsp cRsp;
		cRsp.SetErrorCode(ERR_CUI_DIALOG_NOT_ESTABLISHED);
		SendRspToCu<CCuStopThirdPartyCallRsp>(cRsp, m_ptRecvNode);
		m_nplayId = INVALID_PLAY_ID;
		WriteLog(pcReq->GetDstChn(), ERR_CUI_DIALOG_NOT_ESTABLISHED);

		return PROCMSG_DEL;
	}

	CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession, GetInstance());
	CMS_ASSERT(NULL != pcCuAgent);
	if (NULL == pcCuAgent)
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户[%s]Agent已不存在\n",m_strCuSession.c_str());

		CCuStopThirdPartyCallRsp cRsp;
		cRsp.SetErrorCode(ERR_CUI_CU_NOT_ONLINE);
		SendRspToCu<CCuStopThirdPartyCallRsp>(cRsp, m_ptRecvNode);

		return PROCMSG_DEL;
	}
    
    TSipURI tDstUri;
    CStreamDecentrationAdapter::FindPuiUri( tDstUri, pcReq->GetDstChn().GetDevUri(), pcCuAgent->m_tCmuURI );
    pcReq->SetSession( pcReq->GetDstChn().GetDevUri() );
	if (PROCMSG_OK != PostMsgReq<CCuStopThirdPartyCallReq>(*pcReq, tDstUri ))
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "发送SIP消息失败[%s]\n", pcReq->ToXml().c_str());

		CCuStopThirdPartyCallRsp cRsp;
		cRsp.SetErrorCode(ERR_CUI_SIP_MSG_FAIL);
		SendRspToCu<CCuStopThirdPartyCallRsp>(cRsp, m_ptRecvNode);

		m_nplayId = INVALID_PLAY_ID;
		WriteLog(pcReq->GetDstChn(), ERR_CUI_SIP_MSG_FAIL);
		return PROCMSG_DEL;
	}

	return PROCMSG_OK;
}

u32 CCallPuInterAction::ProcStopCallPuReqNatSwitch(CMessage *const pcMsg)
{
	CCuStopThirdPartyCallReq *pcReq = (CCuStopThirdPartyCallReq *)pcMsg->content;
	//查找task
	CCallPuTaskNatSwitch *pcCallBackTask = (CCallPuTaskNatSwitch *)GetCallPuTask(m_nplayId);
	if ( NULL == pcCallBackTask )
	{
		CCuStopThirdPartyCallRsp cRsp;
		cRsp.SetErrorCode(ERR_CUI_TASK_DEL);
		SendRspToCu<CCuStopThirdPartyCallRsp>(cRsp, m_ptRecvNode);
		m_nplayId = INVALID_PLAY_ID;
		WriteLog(pcReq->GetDstChn(), ERR_CUI_TASK_DEL);

		return PROCMSG_DEL;
	}

	//状态不正确，不能取消
	if(CCallPuTaskNatSwitch::INVITE_HOLD_STATE != pcCallBackTask->GetState())
	{
		CCuStopThirdPartyCallRsp cRsp;
		cRsp.SetErrorCode(ERR_CUI_DIALOG_NOT_ESTABLISHED);
		SendRspToCu<CCuStopThirdPartyCallRsp>(cRsp, m_ptRecvNode);
		m_nplayId = INVALID_PLAY_ID;
		WriteLog(pcReq->GetDstChn(), ERR_CUI_DIALOG_NOT_ESTABLISHED);

		return PROCMSG_DEL;
	}

	CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession, GetInstance());
	CMS_ASSERT(NULL != pcCuAgent);
	if ( pcCuAgent == NULL )
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "CU已退出[%s]\n", pcReq->ToXml().c_str());

		CCuStopThirdPartyCallRsp cRsp;
		cRsp.SetErrorCode(ERR_CUI_CU_NOT_ONLINE);
		SendRspToCu<CCuStopThirdPartyCallRsp>(cRsp, m_ptRecvNode);

		return PROCMSG_DEL;
	}

    TSipURI tDstUri;
    CStreamDecentrationAdapter::FindPuiUri( tDstUri, pcReq->GetDstChn().GetDevUri(), pcCuAgent->m_tCmuURI );
    pcReq->SetSession( pcReq->GetDstChn().GetDevUri() );

    if (PROCMSG_OK != PostMsgReq<CCuStopThirdPartyCallReq>(*pcReq, tDstUri))
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "发送SIP消息失败[%s]\n", pcReq->ToXml().c_str());

		CCuStopThirdPartyCallRsp cRsp;
		cRsp.SetErrorCode(ERR_CUI_SIP_MSG_FAIL);
		SendRspToCu<CCuStopThirdPartyCallRsp>(cRsp, m_ptRecvNode);

		m_nplayId = INVALID_PLAY_ID;
		WriteLog(pcReq->GetDstChn(), ERR_CUI_SIP_MSG_FAIL);
		return PROCMSG_DEL;
	}

	return PROCMSG_OK;
}

void CCallPuInterAction::SwitchCuAddrToNatAddr(TSdp& tSdp)
{
	TMediaDescList& tMediaDescList = tSdp.GetMediaDescList();
	CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession, GetInstance());
	CMS_ASSERT(NULL!=pcCuAgent&&"当前cu登陆task为空");

	for(TMediaDescList::iterator itrMediaDesc = tMediaDescList.begin(); 
		itrMediaDesc!=tMediaDescList.end(); itrMediaDesc++)
	{
		if (itrMediaDesc->GetMediaType() == MEDIA_TYPE_AUDIO)
		{
			TMediaTransChannelList& tMediaTransChnList = itrMediaDesc->GetMediaTransChnList();
			for(TMediaTransChannelList::iterator itrTransChn = tMediaTransChnList.begin(); 
				itrTransChn!=tMediaTransChnList.end(); itrTransChn++)
			{
				if (itrTransChn->GetTransChnType() == MEDIA_TRANS_CHN_RTP)
				{
					TNetAddrList& tNatAddrList = itrTransChn->GetAddrList();
					for (TNetAddrList::iterator itrNatAddr = tNatAddrList.begin();
						itrNatAddr != tNatAddrList.end();itrNatAddr++)
					{
						ChangeToNatAddr(pcCuAgent->m_strSession,*itrNatAddr,m_nplayId);
					}
				}
				else if (itrTransChn->GetTransChnType() == MEDIA_TRANS_CHN_RTCP)
				{
					TNetAddrList& tNatAddrList = itrTransChn->GetAddrList();

					for (TNetAddrList::iterator itrNatAddr = tNatAddrList.begin();
						itrNatAddr != tNatAddrList.end();itrNatAddr++)
					{
						ChangeToNatAddr(pcCuAgent->m_strSession,*itrNatAddr,m_nplayId);
					}
					//交换需要地址伪装为NAT探测包地址
					itrTransChn->SetSendBindAddr(GetSendBindAddr());
				}
				else
				{
					TASKLOG(NORMAL_TASK, ERROR_LEV, "音频呼叫不支持的媒体类型[%s]\n",
						itrTransChn->GetTransChnType().c_str());
				}
			}
		}
	}
}

void CCallPuInterAction::SwitchCuAddrToNatAddrNatSwitch(TSdp& tSdp)
{
	TMediaDescList& tMediaDescList = tSdp.GetMediaDescList();
	CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession, GetInstance());
	CMS_ASSERT(NULL!=pcCuAgent&&"当前cu登陆task为空");

	for(TMediaDescList::iterator itrMediaDesc = tMediaDescList.begin(); 
		itrMediaDesc!=tMediaDescList.end(); itrMediaDesc++)
	{
		if (itrMediaDesc->GetMediaType() == MEDIA_TYPE_AUDIO)
		{
			TMediaTransChannelList& tMediaTransChnList = itrMediaDesc->GetMediaTransChnList();
			for(TMediaTransChannelList::iterator itrTransChn = tMediaTransChnList.begin(); 
				itrTransChn!=tMediaTransChnList.end(); itrTransChn++)
			{
				if (itrTransChn->GetTransChnType() == MEDIA_TRANS_CHN_RTP)
				{
					TNetAddrList& tNatAddrList = itrTransChn->GetAddrList();
					for (TNetAddrList::iterator itrNatAddr = tNatAddrList.begin();
						itrNatAddr != tNatAddrList.end();itrNatAddr++)
					{
						ChangeToNatAddr(pcCuAgent->m_strSession,*itrNatAddr,m_nplayId);
					}
				}
				else if (itrTransChn->GetTransChnType() == MEDIA_TRANS_CHN_RTCP)
				{
					TNetAddrList& tNatAddrList = itrTransChn->GetAddrList();

					for (TNetAddrList::iterator itrNatAddr = tNatAddrList.begin();
						itrNatAddr != tNatAddrList.end();itrNatAddr++)
					{
						ChangeToNatAddr(pcCuAgent->m_strSession,*itrNatAddr,m_nplayId);
					}
				}
				else
				{
					TASKLOG(NORMAL_TASK, ERROR_LEV, "音频呼叫不支持的媒体类型[%s]\n",
						itrTransChn->GetTransChnType().c_str());
				}
			}
		}
	}
}

bool CCallPuInterAction::SwitchToCuiNatSwitchAddressForPlat(TSdp& tSdp)
{
	TMediaDescList& tMediaDescList = tSdp.GetMediaDescList();

	TNetAddr tCuiNatAddr;
	if (!GetCuiNatAddrForPlat(tCuiNatAddr))
	{
		return false;
	}

	for(TMediaDescList::iterator itrMediaDesc = tMediaDescList.begin(); 
		itrMediaDesc!=tMediaDescList.end(); itrMediaDesc++)
	{
		TMediaTransChannelList& tMediaTransChnList = itrMediaDesc->GetMediaTransChnList();
		for(TMediaTransChannelList::iterator itrTransChn = tMediaTransChnList.begin(); 
			itrTransChn!=tMediaTransChnList.end(); itrTransChn++)
		{
			if (itrTransChn->GetTransChnType() == MEDIA_TRANS_CHN_RTP
				|| itrTransChn->GetTransChnType() == MEDIA_TRANS_CHN_RTCP)
			{
				TNetAddrList& tNatAddrList = itrTransChn->GetAddrList();
				for (TNetAddrList::iterator itrNatAddr = tNatAddrList.begin();
					itrNatAddr != tNatAddrList.end();itrNatAddr++)
				{
					//ChangeToCuiNatSwitchAddr(*itrNatAddr);
					*itrNatAddr = tCuiNatAddr;
				}
			}
			else
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "不支持的媒体类型[%s]\n",
					itrTransChn->GetTransChnType().c_str());
			}
		}
	}

	return true;
}

TNetAddr CCallPuInterAction::GetSendBindAddr() const
{
	CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession, GetInstance());
	CMS_ASSERT(NULL!=pcCuAgent&&"当前cu登陆task为空");

	TNetAddr tAddr;

	if (pcCuAgent->m_ePlatNat == NATModeInner && g_cCuiConfig.IsEnableMapedNatAddr())     //平台在NAT内
	{
		//当含有二层NAT时，平台不能同时把一个端口（8001）映射到第一层和第二层
		//这里直接使用cu的登陆ip，即为平台在的最外层映射IP
		//tAddr.SetNetIp(g_cCuiConfig.GetMapedNatIP());
		tAddr.SetNetIp(pcCuAgent->m_strHostIp);
		tAddr.SetNetPort(g_cCuiConfig.GetMapedNatPort());
	}
	else                                             //平台在NAT外
	{
		tAddr.SetNetIp(pcCuAgent->m_tNatInfo.GetIpAddr());//GetLocalIPAddress());
		tAddr.SetNetPort(g_cCuiConfig.GetNatPort());
	}       

	TASKLOG(NORMAL_TASK,EVENT_LEV, "音频呼叫获取到的伪装地址[%s-%d]\n",tAddr.GetNetIp().c_str(),tAddr.GetNetPort());
	return tAddr;
}

void CCallPuInterAction::SwitchPlatAddrToNat(TSdp& tSdp)
{
	CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession, GetInstance());
	CMS_ASSERT(NULL!=pcCuAgent&&"当前cu登陆task为空");

	TMediaDescList& tMediaDescList = tSdp.GetMediaDescList();

	for(TMediaDescList::iterator itrMediaDesc = tMediaDescList.begin(); 
			itrMediaDesc!=tMediaDescList.end(); itrMediaDesc++)
	{
		TMediaTransChannelList& tMediaTransChnList = itrMediaDesc->GetMediaTransChnList();
		for(TMediaTransChannelList::iterator itrTransChn = tMediaTransChnList.begin(); 
				itrTransChn!=tMediaTransChnList.end(); itrTransChn++)
		{
			if (itrTransChn->GetTransChnType() == MEDIA_TRANS_CHN_RTP)
			{
				TNetAddrList& tNatAddrList = itrTransChn->GetAddrList();
				for (TNetAddrList::iterator itrNatAddr = tNatAddrList.begin();
					itrNatAddr != tNatAddrList.end();itrNatAddr++)
				{
					string strSrcIp = itrNatAddr->GetNetIp();
					int nSrcPort = itrNatAddr->GetNetPort();
					string strDstIp="";
					int nDstPort=0;
					stringstream ssKey;
					ssKey << NAT_PROTOCOL_UDP << "_" << strSrcIp << "_" << nSrcPort;
					TMappingTable::const_iterator it = g_tMappingTable.find(ssKey.str());
					TASKLOG(NORMAL_TASK, EVENT_LEV, "Key:(%s)\n", ssKey.str().c_str());
					if (it != g_tMappingTable.end())
					{
						TASKLOG(NORMAL_TASK, EVENT_LEV, "PlatIp(%s), HostIP(%s), MappingIP(%s), MappingPort(%d)\n",
							pcCuAgent->m_tNatInfo.GetIpAddr().c_str(),
							pcCuAgent->m_strHostIp.c_str(),
							it->second.GetMappingIP().c_str(),
							it->second.GetMappingPort());
						//当含有二层NAT时，平台不能同时把一个端口（8001）映射到第一层和第二层
						//这里直接使用cu的登陆ip，即为平台在的最外层映射IP
						//itrNatAddr->SetNetIp(g_cCuiConfig.GetMapedMediaIP());
						itrNatAddr->SetNetIp(pcCuAgent->m_strHostIp);
						//当cu登陆ip与平台映射ip相同时需要替换映射出去的端口
						//如果不相同，说明含有二层NAT，平台映射出去的与实际cu不在统一层NAT固无须替换映射后的端口
						if (/*strDstIp*/pcCuAgent->m_tNatInfo.GetIpAddr() != pcCuAgent->m_strHostIp)
						{
							itrNatAddr->SetNetPort(it->second.GetMappingPort()/*nDstPort*/);
						}
					}
					else if (g_cCuiConfig.GetNatMapAddr(strSrcIp,nSrcPort,strDstIp,nDstPort))
					{
						//当含有二层NAT时，平台不能同时把一个端口（8001）映射到第一层和第二层
						//这里直接使用cu的登陆ip，即为平台在的最外层映射IP
						//itrNatAddr->SetNetIp(g_cCuiConfig.GetMapedMediaIP());
						itrNatAddr->SetNetIp(pcCuAgent->m_strHostIp);
						//当cu登陆ip与平台映射ip相同时需要替换映射出去的端口
						//如果不相同，说明含有二层NAT，平台映射出去的与实际cu不在统一层NAT固无须替换映射后的端口
						if (pcCuAgent->m_tNatInfo.GetIpAddr() != pcCuAgent->m_strHostIp)
						{
							itrNatAddr->SetNetPort(nDstPort);
						}
					}
					else
					{
						TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]切换外网映射地址失败，可能导致浏览失败\n",
							pcCuAgent->m_strUserName.c_str(), pcCuAgent->m_strSession.c_str());
					}
				}
			}
		}
	}
}

bool CCallPuInterAction::GetCuiNatAddrForCu(TNetAddr &tCuiNatAddr)
{
	CCuAgent* pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession, GetInstance());
	if ( pcCuAgent == NULL )
	{
		return false;
	}
	
	tCuiNatAddr.Clear();

	string strPlatCuiIp;
	int nPlatCuiPort = 0;

	if ( NATModeInner == pcCuAgent->m_ePlatNat && !g_cCuiConfig.GetMapedNatIP().empty())
	{
		//cCuPunchAddr.SetNetIp(g_cCuiConfig.GetMapedNatIP());
		//当含有二层NAT时，平台不能同时把一个端口（8001）映射到第一层和第二层
		//这里直接使用cu的登陆ip，即为平台在的最外层映射IP
		strPlatCuiIp = pcCuAgent->m_strHostIp;
		if (pcCuAgent->m_tNatInfo.GetIpAddr() != pcCuAgent->m_strHostIp)
		{
			nPlatCuiPort = g_cCuiConfig.GetMapedNatPort();//需要判断hostip是否就是本地ip，若是本地ip，则不能使用natport。
		}
		else
		{
			nPlatCuiPort = g_cCuiConfig.GetNatPort();
		}
	}
	else
	{
		strPlatCuiIp = pcCuAgent->m_tNatInfo.GetIpAddr();
		nPlatCuiPort = g_cCuiConfig.GetNatPort();
	}

	if ( strPlatCuiIp.empty() )
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "strPlatCuiIp is empty!\n");
		return false;
	}

	if ( nPlatCuiPort <= 0 )
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "nPlatCuiPort is 0!\n");
		return false;
	}

	tCuiNatAddr.SetNetIp(strPlatCuiIp);
	tCuiNatAddr.SetNetPort(nPlatCuiPort);

	TASKLOG(NORMAL_TASK, EVENT_LEV, "GetCuiNatAddrForPlat[%s:%d]!\n", IpToStr(tCuiNatAddr.GetNetIpv4()).c_str(), tCuiNatAddr.GetNetPort());
	return true;
}

bool CCallPuInterAction::GetCuiNatAddrForPlat(TNetAddr &tCuiNatAddr)
{
	CCuAgent* pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession, GetInstance());
	if ( pcCuAgent == NULL )
	{
		return false;
	}

	tCuiNatAddr.Clear();
	string strPlatCuiIp = pcCuAgent->m_tNatInfo.GetIpAddr();
	if ( strPlatCuiIp.empty() )
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "m_tNatInfo.GetIpAddr() is empty!\n");
		return false;
	}

	int nPlatCuiPort = g_cCuiConfig.GetNatPort();
	if ( nPlatCuiPort <= 0 )
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "g_cCuiConfig.GetNatPort() is 0!\n");
		return false;
	}

	tCuiNatAddr.SetNetIp(strPlatCuiIp);
	tCuiNatAddr.SetNetPort(nPlatCuiPort);

	TASKLOG(NORMAL_TASK, EVENT_LEV, "GetCuiNatAddrForPlat[%s:%d]!\n", IpToStr(tCuiNatAddr.GetNetIpv4()).c_str(), tCuiNatAddr.GetNetPort());
	return true;
}
bool CCallPuInterAction::SwitchToCuiNatSwitchAddressForCu(TSdp& tSdp)
{
	TMediaDescList& tMediaDescList = tSdp.GetMediaDescList();

	TNetAddr tCuiNatAddr;
	if (!GetCuiNatAddrForCu(tCuiNatAddr))
	{
		return false;
	}

	for(TMediaDescList::iterator itrMediaDesc = tMediaDescList.begin(); 
		itrMediaDesc!=tMediaDescList.end(); itrMediaDesc++)
	{
		TMediaTransChannelList& tMediaTransChnList = itrMediaDesc->GetMediaTransChnList();
		for(TMediaTransChannelList::iterator itrTransChn = tMediaTransChnList.begin(); 
			itrTransChn!=tMediaTransChnList.end(); itrTransChn++)
		{
			if (itrTransChn->GetTransChnType() == MEDIA_TRANS_CHN_RTP
				|| itrTransChn->GetTransChnType() == MEDIA_TRANS_CHN_RTCP)
			{
				TNetAddrList& tNatAddrList = itrTransChn->GetAddrList();
				for (TNetAddrList::iterator itrNatAddr = tNatAddrList.begin();
					itrNatAddr != tNatAddrList.end();itrNatAddr++)
				{
					*itrNatAddr = tCuiNatAddr;
				}
			}
			else
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "不支持的媒体类型[%s]\n",
					itrTransChn->GetTransChnType().c_str());
			}
		}
	}

	return true;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//瞬时事务
CPeSingleInterAction::CPeSingleInterAction( CInstExt *pcInst, const string &strCuSession, const string &strPeUri, PTRecvNode ptRecvNode) : CCallBackTask(pcInst, strCuSession, strPeUri)
{
    CMS_ASSERT(!strCuSession.empty());
    CMS_ASSERT(!strPeUri.empty());
    CMS_ASSERT(ptRecvNode != NULL);

    m_dwEventID = 0;
    m_ptRecvNode = ptRecvNode; 
    m_strReq.clear();
    m_strRspOfErr.clear();
}

CPeSingleInterAction::~CPeSingleInterAction()
{
    TerminateTask();
}

void CPeSingleInterAction::TerminateTask()
{
    //瞬时事务不需要管理
    //     if ( GetState() != WAIT_RESPONSE_STATE && m_ptRecvNode != NULL )
    //     {
    //         CEventRsp cRsp;
    //         cRsp.FromXml(m_strRspOfErr);
    //         cRsp.SetErrorCode(ERR_CUI_TASK_DEL);
    //         SendRspToCu<CEventRsp>(cRsp, m_ptRecvNode);
    //     }
}

void CPeSingleInterAction::InitStateMachine()
{
    CStateProc cNotRequestProc;
    cNotRequestProc.ProcMsg = (CSipTask::PFProcMsg)&CPeSingleInterAction::NotReqProcMsg;
    cNotRequestProc.TimerPoll = (CSipTask::PFTimerPoll)&CPeSingleInterAction::WaitReqTimerPoll;
    cNotRequestProc.ProcErrMsg = (CSipTask::PFProcMsg)&CPeSingleInterAction::NotReqProcMsg;
    AddRuleProc(NOT_REQUEST_STATE, cNotRequestProc);

    CStateProc cWaitRspProc;
    cWaitRspProc.ProcMsg = (CSipTask::PFProcMsg)&CPeSingleInterAction::WaitRspProcMsg;
    cWaitRspProc.TimerPoll = (CSipTask::PFTimerPoll)&CPeSingleInterAction::WaitRspTimerPoll;
    cWaitRspProc.ProcErrMsg = (CSipTask::PFProcMsg)&CPeSingleInterAction::WaitRspProcMsg;
    AddRuleProc(WAIT_RESPONSE_STATE, cWaitRspProc);

    NextState(NOT_REQUEST_STATE);
}

u32 CPeSingleInterAction::NotReqProcMsg(CMessage *const pcMsg)
{
    TASKLOG(NORMAL_TASK, EVENT_LEV, "收到CU用户会话[%s]操作外设[%s]的请求[%s--%hu]\n",
        m_strCuSession.c_str(), m_tPeUri.GetURIString().c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

    u32 dwRet = PROCMSG_OK;

    m_dwEventID = pcMsg->event;
    switch(pcMsg->event)
    {
	case RCS_RECORD_LOCK_OPR_REQ:
		{
			dwRet = OnNotReqMsg<CCuRcsRecordLockOprReq, CCuRcsRecordLockOprRsp>(pcMsg);
		}
		break;
    case RCS_SET_RECORD_TASK_REQ:
        {
            dwRet = OnNotReqMsg<CCuRcsSetRecordTaskReq, CCuRcsSetRecordTaskRsp>(pcMsg);
        }
        break;
    case RCS_GET_SYSPARAM_REQ:
        {
            dwRet =  OnNotReqMsg<CCuRcsGetSysParamReq, CCuRcsGetSysParamRsp>(pcMsg);
        }
        break;
    case RCS_SET_SYSPARAM_REQ:
        {
            dwRet =  OnNotReqMsg<CCuRcsSetSysParamReq, CCuRcsSetSysParamRsp>(pcMsg);
        }
        break;
    case RCS_BINDING_PUCHN_REQ:
        {
            dwRet =  OnNotReqMsg<CCuRcsBindingPuChnReq, CCuRcsBindingPuChnRsp>(pcMsg);
        }
        break;
    case RCS_UNBINDING_PUCHN_REQ:
        {
            dwRet =  OnNotReqMsg<CCuRcsUnbindingPuChnReq,CCuRcsUnbindingPuChnRsp>(pcMsg);
        }
        break;
    case RCS_QUERY_RECORD_DAYS_REQ:
        {
            dwRet = OnNotReqMsg<CRcsQueryRecordDaysReq, CCuRcsQueryRecordDaysRsp>(pcMsg);
        }
        break;
    case RCS_SET_RECORD_PARAM_REQ:
        {
            dwRet = OnNotReqMsg<CCuRcsSetRecordParamReq, CCuRcsSetRecordParamRsp>(pcMsg);
        }
        break;
    case TV_POLL_OPERATE_REQ:
        {
            dwRet = OnNotReqMsg<CCuTvPollOperateReq, CCuTvPollOperateRsp>(pcMsg);
        }
        break;
    case TVWALL_SCHEME_SAVE_REQ:
        {
            dwRet = OnNotReqMsg<CCuTvWallSchemeSaveReq, CCuTvWallSchemeSaveRsp>(pcMsg);
        }
        break;
    case TVWALL_SCHEME_DEL_REQ:
        {
            dwRet = OnNotReqMsg<CCuTvWallSchemeDelReq, CCuTvWallSchemeDelRsp>(pcMsg);
        }
        break;
    case TVWALL_SCHEME_LOAD_REQ:
        {
            dwRet = OnNotReqMsg<CCuTvWallSchemeLoadReq, CCuTvWallSchemeLoadRsp>(pcMsg);
        }
        break;
    case TVWALL_SCHEME_GROUP_SET_REQ:
        {
            dwRet = OnNotReqMsg<CCuTvWallSchemeGroupSetReq, CCuTvWallSchemeGroupSetRsp>(pcMsg);
        }
        break;
    case TVWALL_SCHEME_GROUP_GET_REQ:
        {
            dwRet = OnNotReqMsg<CCuTvWallSchemeGroupGetReq, CCuTvWallSchemeGroupGetRsp>(pcMsg);
        }
        break;
    case TVWALL_SCHEME_POLL_OPERATE_REQ:
        {
            dwRet = OnNotReqMsg<CCuTvWallSchemePollOperateReq, CCuTvWallSchemePollOperateRsp>(pcMsg);
        }
        break;
    case TV_WIN_STYLE_SET_REQ:
        {
            dwRet = OnNotReqMsg<CCuTvWinStyleSetReq, CCuTvWinStyleSetRsp>(pcMsg);
        }
        break;
    case MPS_MAP_CREATE_REQ:
        {
            dwRet = OnNotReqMsg<CCuMapCreateReq, CCuMapCreateRsp>(pcMsg);
        }
        break;
    case MPS_MAP_MOD_REQ:
        {
            dwRet = OnNotReqMsg<CCuMapModReq, CCuMapModRsp>(pcMsg);
        }
        break;
    case MPS_MAP_DEL_REQ:
        {
            dwRet = OnNotReqMsg<CCuMapDelReq, CCuMapDelRsp>(pcMsg);
        }
        break;
    case MPS_MAP_ELEMENT_CREATE_REQ:
        {
            dwRet = OnNotReqMsg<CCuMapElementCreateReq, CCuMapElementCreateRsp>(pcMsg);
        }
        break;
    case MPS_MAP_ELEMENT_MOD_REQ:
        {
            dwRet = OnNotReqMsg<CCuMapElementModReq, CCuMapElementModRsp>(pcMsg);
        }
        break;
    case MPS_MAP_ELEMENT_DEL_REQ:
        {
            dwRet = OnNotReqMsg<CCuMapElementDelReq, CCuMapElementDelRsp>(pcMsg);
        }
        break;
    case MPS_POS_CORRECT_MARS_TO_EARTH_REQ:
        {
            dwRet = OnNotReqMsg<CCuPosCorrectMarsToEarthReq, CCuPosCorrectMarsToEarthRsp>(pcMsg);
        }
        break;
    case MPS_POS_CORRECT_EARTH_TO_MARS_REQ:
        {
            dwRet = OnNotReqMsg<CCuPosCorrectEarthToMarsReq, CCuPosCorrectEarthToMarsRsp>(pcMsg);
        }
        break;
    case MPS_DATA_VERSION_GET_REQ:
        {
            dwRet = OnNotReqMsg<CCuMpsDataVersionGetReq, CCuMpsDataVersionGetRsp>(pcMsg);
        }
        break;
	case MPS_GPS_DATA_BAKEUP_REQ:
		{
			dwRet = OnNotReqMsg<CCuGpsDataBakeUpReq,CCuGpsDataBakeUpRsp>(pcMsg);
		}
		break;
	case MPS_GPS_STORE_CONFIG_GET_REQ:
		{
			dwRet = OnNotReqMsg<CCuMpsGpsStoreCfgGetReq,CCuMpsGpsStoreCfgGetRsp>(pcMsg);
		}
		break;
    case MPS_MAP_UPLOAD_REQ:
        {
            dwRet = OnNotReqMsg<CCuMapUploadReq, CCuMapUploadRsp>(pcMsg);
        }
        break;
    case MPS_ELEMENT_ICON_UPLOAD_REQ:
        {
            dwRet = OnNotReqMsg<CCuElementIconUploadReq, CCuElementIconUploadRsp>(pcMsg);
        }
        break;
    case ALARMLINK_VERSION_GET_REQ:
        {
            dwRet = OnNotReqMsg<CCuAlarmLinkVersionGetReq, CCuAlarmLinkVersionGetRsp>(pcMsg);
        }
        break;
    case ALARMLINK_CFG_GET_REQ:
        {
            dwRet = OnNotReqMsg<CCuAlarmLinkCfgGetReq, CCuAlarmLinkCfgGetRsp>(pcMsg);
        }
        break;
    case ALARMLINK_CFG_SET_REQ:
        {
            CCuAlarmLinkCfgSetReq *pcReq = (CCuAlarmLinkCfgSetReq *)pcMsg->content;
            if (!g_cCuiDataMgr.IsHasDevPrivilege(m_strCuSession, GetInstance(), pcReq->GetCfg().GetAlarmLinkSrc().GetDevUri(), CEncoderPrivilege::en_Defence_Ctrl))
            {
                SendUniformRsp(*pcReq, ERR_CUI_CU_NO_PERMISSION);
                return PROCMSG_DEL;
            }

            dwRet = OnNotReqMsg<CCuAlarmLinkCfgSetReq, CCuAlarmLinkCfgSetRsp>(pcMsg);
        }
        break;
    case UAS_CUSER_CUSTOMDATA_ADD_REQ:
        {
            CCuUserCustomDataAddReq *pcReq = (CCuUserCustomDataAddReq*)pcMsg->content;
            const string &strUserId = g_cCuiDataMgr.GetCuserIdByCuserSession(pcReq->GetSession(), GetInstance());
            if (strUserId.empty())
            {
                SendUniformRsp(*pcReq, ERR_CUI_QUERYING_INNER_DATA);
                return PROCMSG_DEL;
            }
			
			//添加操作日志
			CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession, GetInstance());
			if (NULL != pcCuAgent)
			{
				string strLogDesc = string( GetLanDesc(LAN_DESC_ADD) + GetLanDesc(LAN_DESC_CUSTOM) + GetLanDesc(LAN_DESC_SCHEME)
					+ "{ T[" + pcReq->GetDataTag() + "], " 
					+ " F[" + (pcReq->GetLastFrag()?"Y":"N") + "], "
					+ " D[" + Num2Str(pcReq->GetInfo().dataType) + "#" + pcReq->GetInfo().dataId + "#" 
					+ Num2Str(pcReq->GetInfo().currentFragNo) + "#" + Num2Str(pcReq->GetInfo().fragData.size()) + "] }, "
					+ GetLanDesc(LAN_DESC_USER) + "[" + pcCuAgent->m_strUserName +":"+pcCuAgent->m_strClientLocalIp + "] "
					+ GetLanDesc(LAN_DESC_DATE) + "[" + CCMSTime().GetXSDDateTime()+ "]");
					
				WriteLog(enUserOpTypeOther, CMS_SUCCESS, strLogDesc, strUserId);
			}
            
            pcReq->GetInfo().userId = strUserId;
            dwRet = OnNotReqMsg<CCuUserCustomDataAddReq, CCuUserCustomDataAddRsp>(pcMsg);
        }
        break;
    case UAS_CUSER_CUSTOMDATA_DEL_REQ:
        {
            CCuUserCustomDataDelReq *pcReq = (CCuUserCustomDataDelReq*)pcMsg->content;
            const string &strUserId = g_cCuiDataMgr.GetCuserIdByCuserSession(pcReq->GetSession(), GetInstance());
            if (strUserId.empty())
            {
                SendUniformRsp(*pcReq, ERR_CUI_QUERYING_INNER_DATA);
                return PROCMSG_DEL;
            }
			
			//添加操作日志
			CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession, GetInstance());
			if (NULL != pcCuAgent)
			{
				string strLogDesc = string( GetLanDesc(LAN_DESC_DEL) + GetLanDesc(LAN_DESC_CUSTOM) + GetLanDesc(LAN_DESC_SCHEME)
					+ "{ D[" + Num2Str(pcReq->GetKey().GetDataType()) + "#" + pcReq->GetKey().GetDataId() + "] }, "
					+ GetLanDesc(LAN_DESC_USER) + "[" + pcCuAgent->m_strUserName+":"+pcCuAgent->m_strClientLocalIp + "] "
					+ GetLanDesc(LAN_DESC_DATE) + "[" + CCMSTime().GetXSDDateTime()+ "]");
					
				WriteLog(enUserOpTypeOther, CMS_SUCCESS, strLogDesc, strUserId);
			}

            pcReq->GetKey().SetUserId(strUserId);
            dwRet = OnNotReqMsg<CCuUserCustomDataDelReq, CCuUserCustomDataDelRsp>(pcMsg);
        }
        break;
    case UAS_CONFIG_UAS_GET_REQ:
        {
            dwRet = OnNotReqMsg<CCuConfigUasGetReq, CCuConfigUasGetRsp>(pcMsg);
        }
        break;
    case UAS_CONFIG_UAS_SET_REQ:
        {
            dwRet = OnNotReqMsg<CCuConfigUasSetReq, CCuConfigUasSetRsp>(pcMsg);
        }
        break;
	case GBS_GBDOMAIN_PARENT_PRIVILEGE_GET_REQ:
		{
			dwRet = OnNotReqMsg<CCuGbsGbdomainParentPrivilegeGetReq,CCuGbsGbdomainParentPrivilegeGetRsp>(pcMsg);
		}
		break;
	case GBS_GBDOMAIN_PARENT_PRIVILEGE_SET_REQ:
		{
			dwRet = OnNotReqMsg<CCuGbsGbdomainParentPrivilegeSetReq,CCuGbsGbdomainParentPrivilegeSetRsp>(pcMsg);
		}
		break;
	case GBS_OPTION_CONF_GET_REQ:
		{
			dwRet = OnNotReqMsg<CCuGbsOptionConfGetReq, CCuGbsOptionConfGetRsp>(pcMsg);
		}
		break;
	case GBS_OPTION_CONF_SET_REQ:
		{
			dwRet = OnNotReqMsg<CCuGbsOptionConfSetReq, CCuGbsOptionConfSetRsp>(pcMsg);
		}
		break;
	case GBS_CERTIFICATE_UPLOAD_REQ:
		{
			dwRet = OnNotReqMsg<CCuGbsCertificateUploadReq, CCuGbsCertificateUploadRsp>(pcMsg);
		}
		break;
	case GBS_CERTIFICATE_GET_REQ:
		{
			dwRet = OnNotReqMsg<CCuGbsCertificateGetReq, CCuGbsCertificateGetRsp>(pcMsg);
		}
		break;
	case GBS_DEV_GBID_GET_REQ:
		{
			dwRet = OnNotReqMsg<CCuGbsDevGbidGetReq,CCuGbsDevGbidGetRsp>(pcMsg);
		}
		break;
	case GBS_GBPARENTDOMAIN_GET_REQ:
		{
			dwRet = OnNotReqMsg<CCuGbsGbParentDomainGetReq,CCuGbsGbParentDomainGetRsp>(pcMsg);
		}
		break;
	case GBS_GBPARENTDOMAIN_CATALOG_LOAD_REQ:
		{
			dwRet = OnNotReqMsg<CCuGbsGbPDCatalogLoadReq,CCuGbsGbPDCatalogLoadRsp>(pcMsg);
		}
		break;
	case GBS_DEVGRP_CIVILCODE_GET_REQ:
		{
			dwRet = OnNotReqMsg<CCuGbsDevGrpCivilCodeGetReq,CCuGbsDevGrpCivilCodeGetRsp>(pcMsg);
		}
		break;
	case GBS_DEV_KDID_GET_REQ:
		{
			dwRet = OnNotReqMsg<CCuGbsDevKdidGetReq,CCuGbsDevKdidGetRsp>(pcMsg);
		}
		break;
	case GBS_CATALOG_SELECT_REPORT_START_REQ:
		{
			dwRet = OnNotReqMsg<CCuGbsCatalogSelectReportStartReq, CCuGbsCatalogSelectReportStartRsp>(pcMsg);
		}
		break;
	case GBS_GBDEV_TELEBOOT_REQ:
		{
			dwRet = OnNotReqMsg<CCuGbsGbDevTeleBootReq,CCuGbsGbDevTeleBootRsp>(pcMsg);
		}
		break;
	case CU_CUI_SECURITY_AUTHORIZATION_FIRST_REQ:
		{
			const CSecurityAuthorizationFirstReq *pcCuReq = NULL;
			pcCuReq = (const CSecurityAuthorizationFirstReq *)pcMsg->content;

			string strUserId = g_cCuiDataMgr.GetCuserIdByCuserSession( m_strCuSession, GetInstance() );

			CGbsSafeAuthInfoFirstReq  cGbsReq;
			cGbsReq.SetCapabilityAlgorithm( pcCuReq->GetAlgorithm() );

			cGbsReq.SetUserId( strUserId );

			CMessage cMsg;
			cMsg.content = (u8 *)&cGbsReq;
			dwRet = OnNotReqMsg< CGbsSafeAuthInfoFirstReq, CGbsSafeAuthInfoFirstRsp>( &cMsg );
		}
		break;
	case CU_CUI_SECURITY_AUTHORIZATION_SECOND_REQ:
		{
			const CSecurityAuthorizationSecondReq *pcCuReq = NULL;
			pcCuReq = (const CSecurityAuthorizationSecondReq *)pcMsg->content;

			CGbsSafeAuthInfoSecondReq  cGbsReq;
			cGbsReq.SetRandom1( pcCuReq->GetRandom1() );
			cGbsReq.SetRandom2( pcCuReq->GetRandom2() );
			cGbsReq.SetSign1(   pcCuReq->GetSign1()   );
			cGbsReq.SetServerId(pcCuReq->GetServerId());
			cGbsReq.SetAlgorithm(pcCuReq->GetAlgorithm() );

			CMessage cMsg;
			cMsg.content  = (u8*)&cGbsReq;
			dwRet = OnNotReqMsg< CGbsSafeAuthInfoSecondReq, CGbsSafeAuthInfoSecondRsp>( &cMsg );
		}
		break;
	case RECORD_ON_TV_REQ:
		{
			dwRet = OnNotReqMsg<CCuRecordOnTvReq, CCuRecordOnTvRsp>(pcMsg);
		}
		break;
	case RECORD_OFF_TV_REQ:
		{
			dwRet = OnNotReqMsg<CCuRecordOffTvReq, CCuRecordOffTvRsp>(pcMsg);
		}
		break;
    case REAL_MEDIA_ON_TV_REQ:
        {
            dwRet = OnNotReqMsg<CCuRealMediaOnTvReq, CCuRealMediaOnTvRsp>(pcMsg);
        }
        break;
	case REAL_MEDIA_SWITCH_OCCUR_TYPE_REQ:
		{
			dwRet = OnNotReqMsg<CCuRealMediaSwitchOccurTypeReq, CCuRealMediaSwitchOccurTypeRsp>(pcMsg);
		}
		break;
    case REAL_MEDIA_OFF_TV_REQ:
        {
            dwRet = OnNotReqMsg<CCuRealMediaOffTvReq, CCuRealMediaOffTvRsp>(pcMsg);
        }
        break;
    case VCR_CTRL_REQ:
        {
            dwRet = OnNotReqMsg<CCuVcrCtrlReq, CCuVcrCtrlRsp>(pcMsg);
        }
        break;
    case PLAYBACK_PROGRESS_QUERY_REQ:
        {
            dwRet = OnNotReqMsg<CCuPlaybackProgressQueryReq, CCuPlaybackProgressQueryRsp>(pcMsg);
        }
        break;
    case PLAT_REC_ON_TV_REQ:
        {
            dwRet = OnNotReqMsg<CCuPlatRecOnTvReq, CCuPlatRecOnTvRsp>(pcMsg);
        }
        break;
    case MPS_GET_SUPPORT_MAP_TYPES_REQ:
        {
            dwRet = OnNotReqMsg<CCuGetSupportMapTypesReq, CCuGetSupportMapTypesRsp>(pcMsg);
        }
        break;
    case MPS_MAP_AREA_CREATE_REQ:
        {
            dwRet = OnNotReqMsg<CCuMapAreaCreateReq, CCuMapAreaCreateRsp>(pcMsg);
        }
        break;
    case MPS_MAP_AREA_MOD_REQ:
        {
            dwRet = OnNotReqMsg<CCuMapAreaModReq, CCuMapAreaModRsp>(pcMsg);
        }
        break;
    case UAS_VIDEOSOURCE_PRESETCFG_ADD_REQ:
        {
            dwRet = OnNotReqMsg<CCuVideosourcePresetcfgAddReq, CCuVideosourcePresetcfgAddRsp>(pcMsg);
        }
        break;
    case UAS_VIDEOSOURCE_PRESETCFG_DEL_REQ:
        {
            dwRet = OnNotReqMsg<CCuVideosourcePresetcfgDelReq, CCuVideosourcePresetcfgDelRsp>(pcMsg);
        }
        break;
	case CAPS_GET_CONFIG_REQ:
		{
			dwRet = OnNotReqMsg<CCuGetCAPSConfigReq, CCuGetCAPSConfigRsp>(pcMsg);
		}
		break;
	case CAPS_SET_CONFIG_REQ:
		{
			dwRet = OnNotReqMsg<CCuSetCAPSConfigReq, CCuSetCAPSConfigRsp>(pcMsg);
		}
		break;
	case CAPS_ADD_DEVICE_REQ:
		{
			dwRet = OnNotReqMsg<CCuCAPSAddDeviceReq, CCuCAPSAddDeviceRsp>(pcMsg);
		}
		break;
	case CAPS_MODIFY_DEVICE_REQ:
		{
			dwRet = OnNotReqMsg<CCuCAPSModifyDeviceReq, CCuCAPSModifyDeviceRsp>(pcMsg);
		}
		break;
	case CAPS_DEL_DEVICE_REQ:
		{
			dwRet = OnNotReqMsg<CCuCAPSDelDeviceReq, CCuCAPSDelDeviceRsp>(pcMsg);
		}
		break;
	case DPS_GET_DEVICE_STATUS_REQ:
		{
			dwRet = OnNotReqMsg<CCuDPSGetDeviceStatusReq, CCuDPSGetDeviceStatusRsp>(pcMsg);
		}
		break;
    default:
        {
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"收到未知消息");
        }
        break;
    }
    return dwRet;
}

u32 CPeSingleInterAction::WaitReqTimerPoll()
{
    if ( GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout() )
    {
        TASKLOG(NORMAL_TASK, ERROR_LEV, "请求[%s--%d]超时，直接操作外设[%s]CU用户会话[%s]发送应答消息[%s--%d]\n",
            OspExtEventDesc(m_dwEventID + 1).c_str(), m_dwEventID + 1,
            m_tPeUri.GetURIString().c_str(), m_strCuSession.c_str(), 
            OspExtEventDesc(m_dwEventID).c_str(), m_dwEventID );

        CMS_ASSERT(NULL != m_ptRecvNode);
        SendRspToCu(m_strRspOfErr, m_ptRecvNode);
        NextState(NOT_REQUEST_STATE);
        return TIMERPOLL_DEL;
    }
    return TIMERPOLL_DONE;
}

u32 CPeSingleInterAction::WaitRspProcMsg(CMessage *const pcMsg)
{
    COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
    TASKLOG(NORMAL_TASK, EVENT_LEV, "CPeSingleInterAction[TaskNO %u]收到服务器[%s]的应答消息[%s--%hu]\n",
        GetTaskNO(), pcOspSipMsg->GetSipToUri().GetURIString().c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

    u32 dwRet = PROCMSG_OK;

    switch (pcMsg->event)
    {
    case OSP_SIP_MSG_PROC_FAIL:
        {
            CMS_ASSERT(NULL != m_ptRecvNode);
            m_strRspOfErr = ReplaceXmlKeyText(m_strRspOfErr, "13012","<errorCode>", "</errorCode>");
            TASKLOG(NORMAL_TASK, EVENT_LEV, "CPeSingleInterAction[TaskNO %u]发送消息给CU[%s]\n",
                GetTaskNO(), m_strRspOfErr.c_str());
            SendRspToCu(m_strRspOfErr, m_ptRecvNode);
            NextState(NOT_REQUEST_STATE);
            dwRet = PROCMSG_DEL;
        }
        break;
    case OSP_SIP_DIALOG_TERMINATE:
        {
            //不会有Dlg消息
            CMS_ASSERT( 0 && "CSingleInterAction::WaitRspProcMsg不可能出现会话内消息" );
        }
        break;
	case RCS_RECORD_LOCK_OPR_RSP:
		{
			CCuRcsRecordLockOprRsp cRsp;
			GetRspMsg(cRsp, pcMsg);
			dwRet = OnWaitRspMsg<CCuRcsRecordLockOprRsp>(cRsp);
		}
		break;
    case RCS_SET_RECORD_TASK_RSP:
        {
            CCuRcsSetRecordTaskRsp cRsp;
            GetRspMsg(cRsp, pcMsg);
            dwRet = OnWaitRspMsg<CCuRcsSetRecordTaskRsp>(cRsp);
            CCuRcsSetRecordTaskReq cReq;
            cReq.FromXml(m_strReq);

            //             string strLogDesc;
            //             if(cReq.GetIsStart())
            //             {
            //                 strLogDesc = GetOptLogDesc(LAN_DESC_START , GetLanDesc(LAN_DESC_PLAT_REC), GetLanDesc(LAN_DESC_DEVICE_ID) + ":"
            //                     + cReq.GetPuChn().GetDevUri() + " " + GetLanDesc(LAN_DESC_DEV_CHN) + ":" 
            //                     + TransInt2String(cReq.GetPuChn().GetChnNO()+1), (u32)cRsp.GetErrorCode());
            //             }
            //             else
            //             {
            //                 strLogDesc = GetOptLogDesc(LAN_DESC_STOP , GetLanDesc(LAN_DESC_PLAT_REC), GetLanDesc(LAN_DESC_DEVICE_ID) + ":"
            //                     + cReq.GetPuChn().GetDevUri() + " " + GetLanDesc(LAN_DESC_DEV_CHN) + ":"
            //                     + TransInt2String(cReq.GetPuChn().GetChnNO() + 1), (u32)cRsp.GetErrorCode());
            //             }
            //             
            //             WriteLog(enUserOpTypePlatformRecord, (u32)cRsp.GetErrorCode(), strLogDesc);
            WriteLog(cReq.GetPuChn(), cRsp.GetErrorCode(), m_dwEventID, GetDomainName());
        }
        break;
    case RCS_GET_SYSPARAM_RSP:
        {
            CCuRcsGetSysParamRsp cRsp;
            GetRspMsg(cRsp, pcMsg);
            dwRet = OnWaitRspMsg<CCuRcsGetSysParamRsp>(cRsp);
        }
        break;
    case RCS_SET_SYSPARAM_RSP:
        {
            CCuRcsSetSysParamRsp cRsp;
            GetRspMsg(cRsp, pcMsg);
            dwRet = OnWaitRspMsg<CCuRcsSetSysParamRsp>(cRsp);
            CCuRcsSetSysParamReq cReq;
            cReq.FromXml(m_strReq);
            string strLogDesc = GetOptLogDesc(LAN_DESC_SET, GetLanDesc(LAN_DESC_RECORDSYS_PARAM), cReq.GetCoverPolicy(), (u32)cRsp.GetErrorCode());
            WriteLog(enUserOpTypeNRUManage, (u32)cRsp.GetErrorCode(), strLogDesc);
        }
        break;
    case RCS_BINDING_PUCHN_RSP:
        {
            CCuRcsBindingPuChnRsp cRsp;
            GetRspMsg(cRsp, pcMsg);
            dwRet = OnWaitRspMsg<CCuRcsBindingPuChnRsp>(cRsp);
            CCuRcsBindingPuChnReq cReq;
            cReq.FromXml(m_strReq);
            //             string strLogDesc = GetOptLogDesc(GetLanDesc(LAN_DESC_PUCHN_BINDING), GetLanDesc(LAN_DESC_NRU_ID) + ":"
            //                 + cReq.GetPuChnBinding().GetNruID() + " " 
            //                 + GetLanDesc(LAN_DESC_DISK_MOUNT_PATH) + ":"
            //                 + cReq.GetPuChnBinding().GetDiskMountPath()
            //                 , (u32)cRsp.GetErrorCode());
            //             WriteLog(enUserOpTypeNRUManage, (u32)cRsp.GetErrorCode(), strLogDesc);

            TChannelList& tPuChnList = cReq.GetPuChnBinding().GetPuChnList();
            TChannelList::iterator itor = tPuChnList.begin();
            while( itor != tPuChnList.end())
            {
                WriteLog(*itor, cRsp.GetErrorCode(), m_dwEventID, GetDomainName());
                itor++;
            }         
        }
        break;
    case RCS_UNBINDING_PUCHN_RSP:
        {
            CCuRcsUnbindingPuChnRsp cRsp;
            GetRspMsg(cRsp, pcMsg);
            dwRet = OnWaitRspMsg<CCuRcsUnbindingPuChnRsp>(cRsp);

            CCuRcsUnbindingPuChnReq cReq;
            cReq.FromXml(m_strReq);
            TChannelList& tPuChnList = cReq.GetPuChnBinding().GetPuChnList();
            TChannelList::iterator itor = tPuChnList.begin();
            while( itor != tPuChnList.end())
            {
                WriteLog(*itor, cRsp.GetErrorCode(), m_dwEventID, GetDomainName());
                itor++;
            }   

            //             string strLogDesc = GetOptLogDesc(GetLanDesc(LAN_DESC_PUCHN_UNBINDING), GetLanDesc(LAN_DESC_NRU_ID) + ":"
            //                 + cReq.GetPuChnBinding().GetNruID() + " " 
            //                 + GetLanDesc(LAN_DESC_DISK_MOUNT_PATH) + ":"
            //                 + cReq.GetPuChnBinding().GetDiskMountPath()
            //                 , (u32)cRsp.GetErrorCode());
            //             WriteLog(enUserOpTypeNRUManage, (u32)cRsp.GetErrorCode(), strLogDesc);
        }
        break;
    case RCS_QUERY_RECORD_DAYS_RSP:
        {
            CCuRcsQueryRecordDaysRsp cRsp;
            GetRspMsg(cRsp, pcMsg);
            dwRet = OnWaitRspMsg<CCuRcsQueryRecordDaysRsp>(cRsp);
        }
        break;
    case RCS_SET_RECORD_PARAM_RSP:
        {
            CRcsSetRecordParamRsp cRsp;
            GetRspMsg(cRsp, pcMsg);
            dwRet = OnWaitRspMsg<CRcsSetRecordParamRsp>(cRsp);
            CRcsSetRecordParamReq cReq;
            cReq.FromXml(m_strReq);
            string strLogDesc = GetOptLogDesc(LAN_DESC_SET , LAN_DESC_RECORD_PARAM, GetLanDesc(LAN_DESC_DEVICE_ID) + ":"
                + cReq.GetRecordParam().GetPuChn().GetDevUri() + " " + GetLanDesc(LAN_DESC_DEV_CHN) + ":" 
                + TransInt2String(cReq.GetRecordParam().GetPuChn().GetChnNO())+ " " + GetLanDesc(LAN_DESC_RECORD_STREAM_TYPE) + ":" + TransInt2String(cReq.GetRecordParam().GetRecStreamType()) + " " + GetLanDesc(LAN_DESC_RECORD_MODE) + ":" +  cReq.GetRecordParam().GetRecMode() 
                , (u32)cRsp.GetErrorCode());
            WriteLog(enUserOpTypeOther, (u32)cRsp.GetErrorCode(), strLogDesc);
        }
        break;
    case TV_POLL_OPERATE_RSP:
        {
            CCuTvPollOperateRsp cRsp;
            GetRspMsg(cRsp, pcMsg);
            dwRet = OnWaitRspMsg<CCuTvPollOperateRsp>(cRsp);
            CCuTvPollOperateReq cReq;
            cReq.FromXml(m_strReq);
            string strLogDesc = GetOptLogDesc(GetLanDesc(LAN_DESC_TVWIN_POLL), GetLanDesc(LAN_DESC_TVWALLID) + ":" 
                + cReq.GetTvChn().GetTvWallId() + " " + GetLanDesc(LAN_DESC_TVID) + ":" + TransInt2String(cReq.GetTvChn().GetTvId())
                + " " + GetLanDesc(LAN_DESC_TVDIV_ID)  + ":" + TransInt2String(cReq.GetTvChn().GetTvDivId())
                ,(u32)cRsp.GetErrorCode());
            WriteLog(enUserOpTypeTVWall, (u32)cRsp.GetErrorCode(), strLogDesc);
        }
        break;
    case TVWALL_SCHEME_SAVE_RSP:
        {
            CCuTvWallSchemeSaveRsp cRsp;
            GetRspMsg(cRsp, pcMsg);
            dwRet = OnWaitRspMsg<CCuTvWallSchemeSaveRsp>(cRsp);
            CCuTvWallSchemeSaveReq cReq;
            cReq.FromXml(m_strReq);

			const string& strScheme = cReq.GetScheme();
			string strZipString = Base64Decode(strScheme);
			u8 abyBuff[60*1024] = {0};
			uLongf dwBuffLen = sizeof(abyBuff);
			if (Z_OK != uncompress(abyBuff, &dwBuffLen, (u8*)strZipString.c_str(),strZipString.length()))
			{
				break;
			}
			string strUnzip((char*)abyBuff,dwBuffLen);
			TTvWallScheme tScheme;
			tScheme.FromXml(strUnzip);

            string strLogDesc = GetOptLogDesc(GetLanDesc(LAN_DESC_SAVE), GetLanDesc(LAN_DESC_SCHEME), GetLanDesc(LAN_DESC_SCHEME_NAME) + ":"
                + tScheme.GetSchemeName() + "" + GetLanDesc(LAN_DESC_TVWALLID) + ":" + tScheme.GetTvWallId()
                , (u32)cRsp.GetErrorCode());
            WriteLog(enUserOpTypeTVWall, (u32)cRsp.GetErrorCode(), strLogDesc);
        }
        break;
    case TVWALL_SCHEME_DEL_RSP:
        {
            CCuTvWallSchemeDelRsp cRsp;
            GetRspMsg(cRsp, pcMsg);
            dwRet = OnWaitRspMsg<CCuTvWallSchemeDelRsp>(cRsp);
            CCuTvWallSchemeDelReq cReq;
            cReq.FromXml(m_strReq);
            string strLogDesc = GetOptLogDesc(GetLanDesc(LAN_DESC_DEL), GetLanDesc(LAN_DESC_SCHEME), GetLanDesc(LAN_DESC_TVWALLID) + ":"
                + cReq.GetTvWallId()
                , (u32)cRsp.GetErrorCode());
            WriteLog(enUserOpTypeTVWall, (u32)cRsp.GetErrorCode(), strLogDesc);
        }
        break;
    case TVWALL_SCHEME_LOAD_RSP:
        {
            CCuTvWallSchemeLoadRsp cRsp;
            GetRspMsg(cRsp, pcMsg);
            dwRet = OnWaitRspMsg<CCuTvWallSchemeLoadRsp>(cRsp);
            CCuTvWallSchemeLoadReq cReq;
            cReq.FromXml(m_strReq);
            string strLogDesc = GetOptLogDesc(GetLanDesc(LAN_DESC_LOAD), GetLanDesc(LAN_DESC_SCHEME), GetLanDesc(LAN_DESC_SCHEME_NAME) + ":"
                + cReq.GetSchemeName() + "" + GetLanDesc(LAN_DESC_TVWALLID) + ":" + cReq.GetTvWallId()
                , (u32)cRsp.GetErrorCode());
            WriteLog(enUserOpTypeTVWall, (u32)cRsp.GetErrorCode(), strLogDesc);
        }
        break;
    case TVWALL_SCHEME_GROUP_SET_RSP:
        {
            CCuTvWallSchemeGroupSetRsp cRsp;
            GetRspMsg(cRsp, pcMsg);
            dwRet = OnWaitRspMsg<CCuTvWallSchemeGroupSetRsp>(cRsp);
            CCuTvWallSchemeGroupSetReq cReq;
            cReq.FromXml(m_strReq);
            string strLogDesc = GetOptLogDesc(GetLanDesc(LAN_DESC_SET), GetLanDesc(LAN_DESC_TVWALL_SCHEME_GROUP), GetLanDesc(LAN_DESC_TVWALLID)
                + ":" + cReq.GetSchemeGroup().GetTvWallId()
                , (u32)cRsp.GetErrorCode());
            WriteLog(enUserOpTypeTVWall, (u32)cRsp.GetErrorCode(), strLogDesc);
        }
        break;
    case TVWALL_SCHEME_GROUP_GET_RSP:
        {
            CCuTvWallSchemeGroupGetRsp cRsp;
            GetRspMsg(cRsp, pcMsg);
            dwRet = OnWaitRspMsg<CCuTvWallSchemeGroupGetRsp>(cRsp);
            //dwRet = OnWaitRspMsg<CCuTvWallSchemeGroupGetRsp>(pcMsg);
        }
        break;
    case TVWALL_SCHEME_POLL_OPERATE_RSP:
        {
            CCuTvWallSchemePollOperateRsp cRsp;
            GetRspMsg(cRsp, pcMsg);
            dwRet = OnWaitRspMsg<CCuTvWallSchemePollOperateRsp>(cRsp);
            CCuTvWallSchemePollOperateReq cReq;
            cReq.FromXml(m_strReq);
            string strLogDesc = GetOptLogDesc(GetLanDesc(LAN_DESC_SCHEME_POLL), GetLanDesc(LAN_DESC_TVWALLID)
                + ":" + cReq.GetTvWallId() + " " + GetLanDesc(LAN_DESC_POLL_STATE) + ":" + cReq.GetOprType()
                , (u32)cRsp.GetErrorCode());
            WriteLog(enUserOpTypeTVWall, (u32)cRsp.GetErrorCode(), strLogDesc);
        }
        break;
    case TV_WIN_STYLE_SET_RSP:
        {
            CCuTvWinStyleSetRsp cRsp;
            GetRspMsg(cRsp, pcMsg);
            dwRet = OnWaitRspMsg<CCuTvWinStyleSetRsp>(cRsp);
            CCuTvWinStyleSetReq cReq;
            cReq.FromXml(m_strReq);
            string strLogDesc = GetOptLogDesc(GetLanDesc(LAN_DESC_SET), GetLanDesc(LAN_DESC_TVWALL_STYLE), 
                GetLanDesc(LAN_DESC_TVWALLID) + ":" + cReq.GetTvWallId() + " " +  GetLanDesc(LAN_DESC_TVID)
                + ":" + TransInt2String(cReq.GetTvId()) + " " + GetLanDesc(LAN_DESC_TVWIN_STYLE) + ":" 
                + TransInt2String(cReq.GetStyle())
                , (u32)cRsp.GetErrorCode());
            WriteLog(enUserOpTypeTVWall, (u32)cRsp.GetErrorCode(), strLogDesc);
        }
        break;
    case MPS_MAP_CREATE_RSP:
        {
            CCuMapCreateRsp cRsp;
            GetRspMsg(cRsp, pcMsg);
            dwRet = OnWaitRspMsg<CCuMapCreateRsp>(cRsp);
            CCuMapCreateReq cReq;
            cReq.FromXml(m_strReq);
            string strLogDesc = GetOptLogDesc(GetLanDesc(LAN_DESC_CREATE), GetLanDesc(LAN_DESC_E_MAP), 
                GetLanDesc(LAN_DESC_MAP_ID) + ":" + cRsp.GetMapId() + " " + GetLanDesc(LAN_DESC_MAP_NAME) + ":"
                + cReq.GetMapInfo().GetMapName() + " " + GetLanDesc(LAN_DESC_MAP_TYPE) + ":" +  cReq.GetMapInfo().GetMapType(),
                (u32)cRsp.GetErrorCode());
            WriteLog(enUserOpTypeMapConfig, (u32)cRsp.GetErrorCode(), strLogDesc);
        }
        break;
    case MPS_MAP_MOD_RSP:
        {
            CCuMapModRsp cRsp;
            GetRspMsg(cRsp, pcMsg);
            dwRet = OnWaitRspMsg<CCuMapModRsp>(cRsp);
            CCuMapModReq cReq;
            cReq.FromXml(m_strReq);
            string strLogDesc = GetOptLogDesc(GetLanDesc(LAN_DESC_MOD), GetLanDesc(LAN_DESC_E_MAP), 
                GetLanDesc(LAN_DESC_MAP_ID) + ":" + cReq.GetMapInfo().GetMapId() + " " + GetLanDesc(LAN_DESC_MAP_NAME) + ":"
                + cReq.GetMapInfo().GetMapName() + " " + GetLanDesc(LAN_DESC_MAP_TYPE) + ":" +  cReq.GetMapInfo().GetMapType(),
                (u32)cRsp.GetErrorCode());
            WriteLog(enUserOpTypeMapConfig, (u32)cRsp.GetErrorCode(), strLogDesc);
        }
        break;
    case MPS_MAP_DEL_RSP:
        {
            CCuMapDelRsp cRsp;
            GetRspMsg(cRsp, pcMsg);
            dwRet = OnWaitRspMsg<CCuMapDelRsp>(cRsp);
            CCuMapDelReq cReq;
            cReq.FromXml(m_strReq);
            string strLogDesc = GetOptLogDesc(GetLanDesc(LAN_DESC_DEL), GetLanDesc(LAN_DESC_E_MAP), 
                GetLanDesc(LAN_DESC_MAP_ID) + ":" + cReq.GetMapId(),
                (u32)cRsp.GetErrorCode());
            WriteLog(enUserOpTypeMapConfig, (u32)cRsp.GetErrorCode(), strLogDesc);
        }
        break;
    case MPS_MAP_ELEMENT_CREATE_RSP:
        {
            CCuMapElementCreateRsp cRsp;
            GetRspMsg(cRsp, pcMsg);
            dwRet = OnWaitRspMsg<CCuMapElementCreateRsp>(cRsp);
            CCuMapElementCreateReq cReq;
            cReq.FromXml(m_strReq);
            string strLogDesc = GetOptLogDesc(GetLanDesc(LAN_DESC_CREATE), GetLanDesc(LAN_DESC_MAP_ELEMENT), 
                GetLanDesc(LAN_DESC_MAP_ELEMENT_ID) + ":" + cRsp.GetElementId() + " " + GetLanDesc(LAN_DESC_MAP_ELEMENT_NAME) + ":"
                + cReq.GetMapElement().GetName() + " " + GetLanDesc(LAN_DESC_MAP_ELEMENT_TYPE) + ":" +  cReq.GetMapElement().GetType(),
                (u32)cRsp.GetErrorCode());
            WriteLog(enUserOpTypeMapConfig, (u32)cRsp.GetErrorCode(), strLogDesc);
        }
        break;
    case MPS_MAP_ELEMENT_MOD_RSP:
        {
            CCuMapElementModRsp cRsp;
            GetRspMsg(cRsp, pcMsg);
            dwRet = OnWaitRspMsg<CCuMapElementModRsp>(cRsp);
            CCuMapElementModReq cReq;
            cReq.FromXml(m_strReq);
            string strLogDesc = GetOptLogDesc(GetLanDesc(LAN_DESC_MOD), GetLanDesc(LAN_DESC_MAP_ELEMENT), 
                GetLanDesc(LAN_DESC_MAP_ELEMENT_ID) + ":" + cReq.GetMapElement().GetId() + " " + GetLanDesc(LAN_DESC_MAP_ELEMENT_NAME) + ":"
                + cReq.GetMapElement().GetName() + " " + GetLanDesc(LAN_DESC_MAP_ELEMENT_TYPE) + ":" +  cReq.GetMapElement().GetType(),
                (u32)cRsp.GetErrorCode());
            WriteLog(enUserOpTypeMapConfig, (u32)cRsp.GetErrorCode(), strLogDesc);
        }
        break;
    case MPS_MAP_ELEMENT_DEL_RSP:
        {
            CCuMapElementDelRsp cRsp;
            GetRspMsg(cRsp, pcMsg);
            dwRet = OnWaitRspMsg<CCuMapElementDelRsp>(cRsp);
            CCuMapElementDelReq cReq;
            cReq.FromXml(m_strReq);
            string strLogDesc = GetOptLogDesc(GetLanDesc(LAN_DESC_DEL), GetLanDesc(LAN_DESC_MAP_ELEMENT), 
                GetLanDesc(LAN_DESC_MAP_ELEMENT_ID) + ":" + cReq.GetElementId() ,
                (u32)cRsp.GetErrorCode());
            WriteLog(enUserOpTypeMapConfig, (u32)cRsp.GetErrorCode(), strLogDesc);
        }
        break;
    case MPS_POS_CORRECT_MARS_TO_EARTH_RSP:
        {
            CCuPosCorrectMarsToEarthRsp cRsp;
            GetRspMsg(cRsp, pcMsg);
            dwRet = OnWaitRspMsg<CCuPosCorrectMarsToEarthRsp>(cRsp);
            CCuPosCorrectMarsToEarthReq cReq;
            cReq.FromXml(m_strReq);
            string strLogDesc = GetOptLogDesc(GetLanDesc(LAN_DESC_COORDINATES), GetLanDesc(LAN_DESC_TRANSFORMATION), 
                GetLanDesc(LAN_DESC_MARS_COORDINATES) + GetLanDesc(LAN_DESC_LAT) + ":" +  TransDouble2String(cReq.GetMars_lat()) + " " + GetLanDesc(LAN_DESC_LNG) 
                + ":" +   TransDouble2String(cReq.GetMars_lng()) + " " + GetLanDesc(LAN_DESC_EARTH_COORDINATES) + GetLanDesc(LAN_DESC_LAT) + ":" 
                + TransDouble2String(cRsp.GetEarth_lat()) + " " + GetLanDesc(LAN_DESC_LNG) 
                + ":" + TransDouble2String(cRsp.GetEarth_lng()),
                (u32)cRsp.GetErrorCode());
            WriteLog(enUserOpTypeMapConfig, (u32)cRsp.GetErrorCode(), strLogDesc);
        }
        break;
    case MPS_POS_CORRECT_EARTH_TO_MARS_RSP:
        {
            CCuPosCorrectEarthToMarsRsp cRsp;
            GetRspMsg(cRsp, pcMsg);
            dwRet = OnWaitRspMsg<CCuPosCorrectEarthToMarsRsp>(cRsp);
            CCuPosCorrectEarthToMarsReq cReq;
            cReq.FromXml(m_strReq);
            string strLogDesc = GetOptLogDesc(GetLanDesc(LAN_DESC_COORDINATES), GetLanDesc(LAN_DESC_TRANSFORMATION), 
                GetLanDesc(LAN_DESC_EARTH_COORDINATES) + GetLanDesc(LAN_DESC_LAT) + ":" 
                + TransDouble2String(cReq.GetEarth_lat()) + " " + GetLanDesc(LAN_DESC_LNG) 
                + ":" +   TransDouble2String(cReq.GetEarth_lng()) + " " 
                + GetLanDesc(LAN_DESC_MARS_COORDINATES)  + GetLanDesc(LAN_DESC_LAT) 
                + ":" +  TransDouble2String(cRsp.GetMars_lat()) + " " + GetLanDesc(LAN_DESC_LNG) 
                + ":" +   TransDouble2String(cRsp.GetMars_lng()),
                (u32)cRsp.GetErrorCode());
            WriteLog(enUserOpTypeMapConfig, (u32)cRsp.GetErrorCode(), strLogDesc);
        }
        break;
    case MPS_DATA_VERSION_GET_RSP:
        {
            CCuMpsDataVersionGetRsp cRsp;
            GetRspMsg(cRsp, pcMsg);
            dwRet = OnWaitRspMsg<CCuMpsDataVersionGetRsp>(cRsp);
        }
        break;
	case MPS_GPS_DATA_BAKEUP_RSP:
		{
			CCuGpsDataBakeUpRsp cRsp;
			GetRspMsg(cRsp, pcMsg);
			dwRet = OnWaitRspMsg<CCuGpsDataBakeUpRsp>(cRsp);
		}
		break;
	case MPS_GPS_STORE_CONFIG_GET_RSP:
		{
			CCuMpsGpsStoreCfgGetRsp cRsp;
			GetRspMsg(cRsp, pcMsg);
			dwRet = OnWaitRspMsg<CCuMpsGpsStoreCfgGetRsp>(cRsp);
		}
		break;
    case MPS_MAP_UPLOAD_RSP:
        {
            CCuMapUploadRsp cRsp;
            GetRspMsg(cRsp, pcMsg);
            dwRet = OnWaitRspMsg<CCuMapUploadRsp>(cRsp);
        }
        break;
    case MPS_ELEMENT_ICON_UPLOAD_RSP:
        {
            CCuElementIconUploadRsp cRsp;
            GetRspMsg(cRsp, pcMsg);
            dwRet = OnWaitRspMsg<CCuElementIconUploadRsp>(cRsp);
        }
        break;
    case ALARMLINK_CFG_GET_RSP:
        {
            CCuAlarmLinkCfgGetRsp cRsp;
            GetRspMsg(cRsp, pcMsg);
            dwRet = OnWaitRspMsg<CCuAlarmLinkCfgGetRsp>(cRsp);
        }
        break;
    case ALARMLINK_VERSION_GET_RSP:
        {
            CCuAlarmLinkVersionGetRsp cRsp;
            GetRspMsg(cRsp, pcMsg);
            dwRet = OnWaitRspMsg<CCuAlarmLinkVersionGetRsp>(cRsp);
        }
        break;
    case ALARMLINK_CFG_SET_RSP:
        {
            CCuAlarmLinkCfgSetRsp cRsp;
            GetRspMsg(cRsp, pcMsg);
            dwRet = OnWaitRspMsg<CCuAlarmLinkCfgSetRsp>(cRsp);
            CCuAlarmLinkCfgSetReq cReq;
            cReq.FromXml(m_strReq);
            string strLogDesc = GetOptLogDesc(GetLanDesc(LAN_DESC_SET), GetLanDesc(LAN_DESC_ALARM_LINK_CFG), 
                GetLanDesc(LAN_DESC_ALARM_SRC) + GetLanDesc(LAN_DESC_DEVICE_ID) + ":" 
                + cReq.GetCfg().GetAlarmLinkSrc().GetDevUri() + " " + GetLanDesc(LAN_DESC_ALARM_INPUT_ID) + ":" + TransInt2String(cReq.GetCfg().GetAlarmLinkSrc().GetInputId())
                + " " + GetLanDesc(LAN_DESC_ALARM_TYPE) + ":" + cReq.GetCfg().GetAlarmLinkSrc().GetAlarmType()
                , (u32)cRsp.GetErrorCode());
            WriteLog(enUserOpTypeAlarmConfig, (u32)cRsp.GetErrorCode(), strLogDesc, cReq.GetCfg().GetAlarmLinkSrc().GetDevUri());
        }
        break;
    case UAS_CUSER_CUSTOMDATA_ADD_RSP:
        {
            CCuUserCustomDataAddRsp cRsp;
            GetRspMsg(cRsp, pcMsg);
            dwRet = OnWaitRspMsg<CCuUserCustomDataAddRsp>(cRsp);
            // hzytodo2 日志后续需要重构，到时一起加——
        }
        break;
    case UAS_CUSER_CUSTOMDATA_DEL_RSP:
        {
            CCuUserCustomDataDelRsp cRsp;
            GetRspMsg(cRsp, pcMsg);
            dwRet = OnWaitRspMsg<CCuUserCustomDataDelRsp>(cRsp);
            // hzytodo2 日志后续需要重构，到时一起加——
        }
        break;
    case UAS_CONFIG_UAS_GET_RSP:
        {
            CCuConfigUasGetRsp cRsp;
            GetRspMsg(cRsp, pcMsg);
            dwRet = OnWaitRspMsg<CCuConfigUasGetRsp>(cRsp);
        }
        break;
    case UAS_CONFIG_UAS_SET_RSP:
        {
            CCuConfigUasSetRsp cRsp;
            GetRspMsg(cRsp, pcMsg);
            dwRet = OnWaitRspMsg<CCuConfigUasSetRsp>(cRsp);
            CCuConfigUasSetReq cReq;
            cReq.FromXml(m_strReq);
            string strLogDesc = GetOptLogDesc(GetLanDesc(LAN_DESC_SET), GetLanDesc(LAN_DESC_UAS_CFG)
                , (u32)cRsp.GetErrorCode());
            WriteLog(enUserOpTypeSysParamConfig, (u32)cRsp.GetErrorCode(), strLogDesc);
        }
        break;
	case GBS_GBDOMAIN_PARENT_PRIVILEGE_GET_RSP:
		{
			CCuGbsGbdomainParentPrivilegeGetRsp cRsp;
			GetRspMsg(cRsp, pcMsg);
			dwRet = OnWaitRspMsg<CCuGbsGbdomainParentPrivilegeGetRsp>(cRsp);
		}
		break;
	case GBS_GBDOMAIN_PARENT_PRIVILEGE_SET_RSP:
		{
			CCuGbsGbdomainParentPrivilegeSetRsp cRsp;
			GetRspMsg(cRsp, pcMsg);
			dwRet = OnWaitRspMsg<CCuGbsGbdomainParentPrivilegeSetRsp>(cRsp);
		}
		break;
	case GBS_OPTION_CONF_GET_RSP:
		{
			CCuGbsOptionConfGetRsp cRsp;
			GetRspMsg(cRsp, pcMsg);
			dwRet = OnWaitRspMsg<CCuGbsOptionConfGetRsp>(cRsp);
		}
		break;
	case GBS_OPTION_CONF_SET_RSP:
		{
			CCuGbsOptionConfSetRsp cRsp;
			GetRspMsg(cRsp, pcMsg);
			dwRet = OnWaitRspMsg<CCuGbsOptionConfSetRsp>(cRsp);
		}
		break;
	case GBS_CERTIFICATE_UPLOAD_RSP:
		{
			CCuGbsCertificateUploadRsp cRsp;
			GetRspMsg(cRsp, pcMsg);
			dwRet = OnWaitRspMsg<CCuGbsCertificateUploadRsp>(cRsp);
		}
		break;
	case GBS_CERTIFICATE_GET_RSP:
		{
			CCuGbsCertificateGetRsp cRsp;
			GetRspMsg(cRsp, pcMsg);
			dwRet = OnWaitRspMsg<CCuGbsCertificateGetRsp>(cRsp);
		}
		break;
	case GBS_DEV_GBID_GET_RSP:
		{
			CCuGbsDevGbidGetRsp cRsp;
			GetRspMsg(cRsp, pcMsg);
			dwRet = OnWaitRspMsg<CCuGbsDevGbidGetRsp>(cRsp);
		}
		break;
	case GBS_GBPARENTDOMAIN_GET_RSP:
		{
			CCuGbsGbParentDomainGetRsp cRsp;
			GetRspMsg(cRsp, pcMsg);
			dwRet = OnWaitRspMsg<CCuGbsGbParentDomainGetRsp>(cRsp);
		}
		break;
	case GBS_GBPARENTDOMAIN_CATALOG_LOAD_RSP:
		{
			CCuGbsGbPDCatalogLoadRsp cRsp;
			GetRspMsg(cRsp, pcMsg);
			dwRet = OnWaitRspMsg<CCuGbsGbPDCatalogLoadRsp>(cRsp);
		}
		break;
	case GBS_DEVGRP_CIVILCODE_GET_RSP:
		{
			CCuGbsDevGrpCivilCodeGetRsp  cRsp;
			GetRspMsg(cRsp, pcMsg);
			dwRet = OnWaitRspMsg<CCuGbsDevGrpCivilCodeGetRsp >(cRsp);
		}
		break;
	case GBS_DEV_KDID_GET_RSP:
		{
			CCuGbsDevKdidGetRsp  cRsp;
			GetRspMsg(cRsp, pcMsg);
			dwRet = OnWaitRspMsg<CCuGbsDevKdidGetRsp>(cRsp);
		}
		break;
	case GBS_GBDEV_TELEBOOT_RSP:
		{
			CCuGbsGbDevTeleBootRsp cRsp;
			GetRspMsg(cRsp, pcMsg);
			dwRet = OnWaitRspMsg<CCuGbsGbDevTeleBootRsp>(cRsp);
		}
		break;
	case GBS_CATALOG_SELECT_REPORT_START_RSP:
		{
			CCuGbsCatalogSelectReportStartRsp cRsp;
			GetRspMsg(cRsp, pcMsg);
			dwRet = OnWaitRspMsg<CCuGbsCatalogSelectReportStartRsp>(cRsp);
		}
		break;
	case GBS_SAFE_AUTHINFO_CHECK_FIRST_RSP:
		{
			CGbsSafeAuthInfoFirstRsp cGbsRsp;
			GetRspMsg( cGbsRsp, pcMsg );

			CCuSecurityAuthorizationFirstRsp cCuRsp;
			cCuRsp.SetErrorCode ( cGbsRsp.GetErrorCode() );
			cCuRsp.SetAlgorithm ( cGbsRsp.GetBidirectionAlgorithm() );
			cCuRsp.SetRandom1   ( cGbsRsp.GetRandom1() );               

			dwRet = OnWaitRspMsg<CCuSecurityAuthorizationFirstRsp>(cCuRsp);
		}
		break;
	case GBS_SAFE_AUTHINFO_CHECK_SECOND_RSP:
		{
			CGbsSafeAuthInfoSecondRsp cGbsRsp;
			GetRspMsg( cGbsRsp, pcMsg );

			CCuSecurityAuthorizationSecondRsp cCuRsp;
			cCuRsp.SetErrorCode ( cGbsRsp.GetErrorCode() );
			cCuRsp.SetAlgorithm ( cGbsRsp.GetAlgorithm() );
			cCuRsp.SetCrptKey   ( cGbsRsp.GetCryptKey() );
			cCuRsp.SetRandom1   ( cGbsRsp.GetRandom1()  );
			cCuRsp.SetRandom2   ( cGbsRsp.GetRandom2()  );
			cCuRsp.SetSign2     ( cGbsRsp.GetSign2()    );

			dwRet = OnWaitRspMsg<CCuSecurityAuthorizationSecondRsp>(cCuRsp);
		}
		break;
	case RECORD_OFF_TV_RSP:
		{
			CCuRecordOffTvRsp cRsp;
			GetRspMsg(cRsp, pcMsg);
			dwRet = OnWaitRspMsg<CCuRecordOffTvRsp>(cRsp);
			CCuRecordOffTvReq cReq;
			cReq.FromXml(m_strReq);
			string strLogDesc = GetOptLogDesc(GetLanDesc(LAN_DESC_RECORD_OFF_TV), GetLanDesc(LAN_DESC_TVWALLID) + ":" 
				+ cReq.GetTvChn().GetTvWallId() + " " + GetLanDesc(LAN_DESC_TVID) + ":" + TransInt2String(cReq.GetTvChn().GetTvId())
				+ " " + GetLanDesc(LAN_DESC_TVDIV_ID) + ":"+  TransInt2String(cReq.GetTvChn().GetTvDivId())
				, (u32)cRsp.GetErrorCode());
			WriteLog(enUserOpTypeTVWall, (u32)cRsp.GetErrorCode(), strLogDesc);
		}
		break;
	case RECORD_ON_TV_RSP:
		{
			CCuRecordOnTvRsp cRsp;
			GetRspMsg(cRsp, pcMsg);
			dwRet = OnWaitRspMsg<CCuRecordOnTvRsp>(cRsp);
			CCuRecordOnTvReq cReq;
			cReq.FromXml(m_strReq);
			string strLogDesc = GetOptLogDesc(GetLanDesc(LAN_DESC_RECORD_ON_TV), GetLanDesc(LAN_DESC_TVWALLID) + ":" 
				+ cReq.GetTvChn().GetTvWallId() + " " + GetLanDesc(LAN_DESC_TVID) + ":" + TransInt2String(cReq.GetTvChn().GetTvId())
				+ " " + GetLanDesc(LAN_DESC_TVDIV_ID) + ":"+  TransInt2String(cReq.GetTvChn().GetTvDivId())
				, (u32)cRsp.GetErrorCode());
			WriteLog(enUserOpTypeTVWall, (u32)cRsp.GetErrorCode(), strLogDesc);
		}
		break;
	case REAL_MEDIA_SWITCH_OCCUR_TYPE_RSP:
		{
			CCuRealMediaSwitchOccurTypeRsp cRsp;
			GetRspMsg(cRsp, pcMsg);
			dwRet = OnWaitRspMsg<CCuRealMediaSwitchOccurTypeRsp>(cRsp);
		}
		break;
    case REAL_MEDIA_ON_TV_RSP:
        {
            CCuRealMediaOnTvRsp cRsp;
            GetRspMsg(cRsp, pcMsg);
            dwRet = OnWaitRspMsg<CCuRealMediaOnTvRsp>(cRsp);
            CCuRealMediaOnTvReq cReq;
            cReq.FromXml(m_strReq);
            string strLogDesc = GetOptLogDesc(GetLanDesc(LAN_DESC_REAL_MEDIA_ON_TV), GetLanDesc(LAN_DESC_TVWALLID) + ":" 
                + cReq.GetTvChn().GetTvWallId() + " " + GetLanDesc(LAN_DESC_TVID) + ":" + TransInt2String(cReq.GetTvChn().GetTvId())
                + " " + GetLanDesc(LAN_DESC_TVDIV_ID) + ":"+  TransInt2String(cReq.GetTvChn().GetTvDivId()) + " " 
                + GetLanDesc(LAN_DESC_TVWALL_STYLE) + ":" + cReq.GetTvOccurType()
                , (u32)cRsp.GetErrorCode());
            WriteLog(enUserOpTypeTVWall, (u32)cRsp.GetErrorCode(), strLogDesc);
        }
        break;
    case REAL_MEDIA_OFF_TV_RSP:
        {
            CCuRealMediaOffTvRsp cRsp;
            GetRspMsg(cRsp, pcMsg);
            dwRet = OnWaitRspMsg<CCuRealMediaOffTvRsp>(cRsp);
            CCuRealMediaOffTvReq cReq;
            cReq.FromXml(m_strReq);     
            string strLogDesc = GetOptLogDesc(GetLanDesc(LAN_DESC_REAL_MEDIA_OFF_TV), GetLanDesc(LAN_DESC_TVWALLID) + ":" 
                + cReq.GetTvChn().GetTvWallId() + " " + GetLanDesc(LAN_DESC_TVID) + ":" + TransInt2String(cReq.GetTvChn().GetTvId())
                + " " + GetLanDesc(LAN_DESC_TVDIV_ID) + ":"+  TransInt2String(cReq.GetTvChn().GetTvDivId()) + " " + GetLanDesc(LAN_DESC_TVWALL_STYLE) + ":" + cReq.GetTvOccurType()
                , (u32)cRsp.GetErrorCode());
            WriteLog(enUserOpTypeTVWall, (u32)cRsp.GetErrorCode(), strLogDesc);
        }
        break;
    case VCR_CTRL_RSP:
        {
            CCuVcrCtrlRsp cRsp;
            GetRspMsg(cRsp, pcMsg);
            dwRet = OnWaitRspMsg<CCuVcrCtrlRsp>(cRsp);
            CCuVcrCtrlReq cReq;
            cReq.FromXml(m_strReq);
            string strLogDesc = GetOptLogDesc(GetLanDesc(LAN_DESC_VCR_CTRL), GetLanDesc(LAN_DESC_VCR_CMD) + ":" 
                + cReq.GetVcrCmd() + " " + GetLanDesc(LAN_DESC_PLAY_RATE) + ":" + TransInt2String(cReq.GetPlayRate())
                + " " + GetLanDesc(LAN_DESC_SEEK_TIME) + ":"+  TransInt2String(cReq.GetSeekTime())
                , (u32)cRsp.GetErrorCode());
            WriteLog(enUserOpTypeTVWall, (u32)cRsp.GetErrorCode(), strLogDesc);
        }
        break;
    case PLAYBACK_PROGRESS_QUERY_RSP:
        {
            CCuPlaybackProgressQueryRsp cRsp;
            GetRspMsg(cRsp, pcMsg);
            dwRet = OnWaitRspMsg<CCuPlaybackProgressQueryRsp>(cRsp);
        }
        break;
    case PLAT_REC_ON_TV_RSP:
        {
            CCuPlatRecOnTvRsp cRsp;
            GetRspMsg(cRsp, pcMsg);
            dwRet = OnWaitRspMsg<CCuPlatRecOnTvRsp>(cRsp);
            CCuPlatRecOnTvReq cReq;
            cReq.FromXml(m_strReq);
            string strLogDesc = GetOptLogDesc(GetLanDesc(LAN_DESC_PLAT_REC_ON_TV), GetLanDesc(LAN_DESC_REC_TOKEN) + ":" 
                + cReq.GetRecDurationToken() + " " + GetLanDesc(LAN_DESC_TVWALLID) + ":" 
                + cReq.GetTvChn().GetTvWallId() + " " + GetLanDesc(LAN_DESC_TVID) + ":" + TransInt2String(cReq.GetTvChn().GetTvId())
                + " " + GetLanDesc(LAN_DESC_TVDIV_ID) + ":"+  TransInt2String(cReq.GetTvChn().GetTvDivId())
                , (u32)cRsp.GetErrorCode());
            WriteLog(enUserOpTypeTVWall, (u32)cRsp.GetErrorCode(), strLogDesc);
        }
        break;
    case MPS_GET_SUPPORT_MAP_TYPES_RSP:
        {
            CCuGetSupportMapTypesRsp cRsp;
            GetRspMsg(cRsp, pcMsg);
            dwRet = OnWaitRspMsg<CCuGetSupportMapTypesRsp>(cRsp);
        }
        break;
    case MPS_MAP_AREA_CREATE_RSP:
        {
            CCuMapAreaCreateRsp cRsp;
            GetRspMsg(cRsp, pcMsg);
            dwRet = OnWaitRspMsg<CCuMapAreaCreateRsp>(cRsp);
        }
        break;
    case MPS_MAP_AREA_MOD_RSP:
        {
            CCuMapAreaModRsp cRsp;
            GetRspMsg(cRsp, pcMsg);
            dwRet = OnWaitRspMsg<CCuMapAreaModRsp>(cRsp);
        }
        break;
    case UAS_VIDEOSOURCE_PRESETCFG_ADD_RSP:
        {
            CCuVideosourcePresetcfgAddRsp cRsp;
            GetRspMsg(cRsp, pcMsg);
            dwRet = OnWaitRspMsg<CCuVideosourcePresetcfgAddRsp>(cRsp);
        }
        break;
    case UAS_VIDEOSOURCE_PRESETCFG_DEL_RSP:
        {
            CCuVideosourcePresetcfgDelRsp cRsp;
            GetRspMsg(cRsp, pcMsg);
            dwRet = OnWaitRspMsg<CCuVideosourcePresetcfgDelRsp>(cRsp);
        }
        break;
	case CAPS_GET_CONFIG_RSP:
		{
			CCuGetCAPSConfigRsp cRsp;
			GetRspMsg(cRsp, pcMsg);
			dwRet = OnWaitRspMsg<CCuGetCAPSConfigRsp>(cRsp);
		}
		break;
	case CAPS_SET_CONFIG_RSP:
		{
			CCuSetCAPSConfigRsp cRsp;
			GetRspMsg(cRsp, pcMsg);
			dwRet = OnWaitRspMsg<CCuSetCAPSConfigRsp>(cRsp);
		}
		break;
	case CAPS_ADD_DEVICE_RSP:
		{
			CCuCAPSAddDeviceRsp cRsp;
			GetRspMsg(cRsp, pcMsg);
			dwRet = OnWaitRspMsg<CCuCAPSAddDeviceRsp>(cRsp);
		}
		break;
	case CAPS_MODIFY_DEVICE_RSP:
		{
			CCuCAPSModifyDeviceRsp cRsp;
			GetRspMsg(cRsp, pcMsg);
			dwRet = OnWaitRspMsg<CCuCAPSModifyDeviceRsp>(cRsp);
		}
		break;
	case CAPS_DEL_DEVICE_RSP:
		{
			CCuCAPSDelDeviceRsp cRsp;
			GetRspMsg(cRsp, pcMsg);
			dwRet = OnWaitRspMsg<CCuCAPSDelDeviceRsp>(cRsp);
		}
		break;
	case DPS_GET_DEVICE_STATUS_RSP:
		{
			CCuDPSGetDeviceStatusRsp cRsp;
			GetRspMsg(cRsp, pcMsg);
			dwRet = OnWaitRspMsg<CCuDPSGetDeviceStatusRsp>(cRsp);
		}
		break;
    default:
        {
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"收到未知消息");
        }
        break;
    }

    if(PROCMSG_DEL == dwRet)
    {
        delete this;
    }

    return dwRet;
}

u32 CPeSingleInterAction::WaitRspTimerPoll()
{
    if ( GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout() )
    {
        TASKLOG(NORMAL_TASK, ERROR_LEV, "请求[%s--%d]超时，直接操作外设[%s]CU用户会话[%s]发送应答消息[%s--%d]\n",
            OspExtEventDesc(m_dwEventID + 1).c_str(), m_dwEventID + 1,
            m_tPeUri.GetURIString().c_str(), m_strCuSession.c_str(), 
            OspExtEventDesc(m_dwEventID).c_str(), m_dwEventID );

        CMS_ASSERT(NULL != m_ptRecvNode);
        SendRspToCu(m_strRspOfErr, m_ptRecvNode);
        NextState(NOT_REQUEST_STATE);
        return TIMERPOLL_DEL;
    }
    return TIMERPOLL_DONE;
}

void CPeSingleInterAction::WriteTimeOutLog()
{
    u16 wEvent = OspExtEventID(GetXmlKeyText(m_strReq, EVENT_BEGIN, EVENT_END).c_str());
    switch (wEvent)
    {
	case RCS_RECORD_LOCK_OPR_REQ:
		{
			CCuRcsRecordLockOprReq cReq;
			cReq.FromXml(m_strReq);

			string strLogDesc = GetOptLogDesc(GetLanDesc(LAN_DESC_LOCK), cReq.GetRecToken(), ERR_CUI_OPR_TIME_OUT);
			WriteLog(enUserOpTypeOther, ERR_CUI_OPR_TIME_OUT, strLogDesc);
		}
		break;
    case RCS_SET_RECORD_TASK_REQ:
        {
            string strLogDesc;
            CCuRcsSetRecordTaskReq cReq;
            cReq.FromXml(m_strReq);

            WriteLog(cReq.GetPuChn(), ERR_CUI_OPR_TIME_OUT, m_dwEventID, GetDomainName());
            //             if(cReq.GetIsStart())
            //             {
            //                 strLogDesc = GetOptLogDesc(LAN_DESC_START , GetLanDesc(LAN_DESC_PLAT_REC), GetLanDesc(LAN_DESC_DEVICE_ID) + ":"
            //                     + cReq.GetPuChn().GetDevUri() + " " + GetLanDesc(LAN_DESC_DEV_CHN) + ":" 
            //                     + TransInt2String(cReq.GetPuChn().GetChnNO()+1), ERR_CUI_OPR_TIME_OUT);
            //             }
            //             else
            //             {
            //                 strLogDesc = GetOptLogDesc(LAN_DESC_STOP , GetLanDesc(LAN_DESC_PLAT_REC), GetLanDesc(LAN_DESC_DEVICE_ID) + ": "
            //                     + cReq.GetPuChn().GetDevUri()+ " " + GetLanDesc(LAN_DESC_DEV_CHN) + ":"  
            //                     + TransInt2String(cReq.GetPuChn().GetChnNO()+1), ERR_CUI_OPR_TIME_OUT);
            //             }
            // 
            //             WriteLog(enUserOpTypeNRUManage, ERR_CUI_OPR_TIME_OUT, strLogDesc);
        }
        break;
    case RCS_GET_SYSPARAM_REQ:
        {
        }
        break;
    case RCS_SET_SYSPARAM_REQ:
        {
            CCuRcsSetSysParamReq cReq;
            cReq.FromXml(m_strReq);
            string strLogDesc = GetOptLogDesc(LAN_DESC_SET, GetLanDesc(LAN_DESC_RECORDSYS_PARAM), cReq.GetCoverPolicy(), ERR_CUI_OPR_TIME_OUT);
            WriteLog(enUserOpTypeNRUManage, ERR_CUI_OPR_TIME_OUT, strLogDesc);
        }
        break;
    case RCS_BINDING_PUCHN_REQ:
        {
            CCuRcsBindingPuChnReq cReq;
            cReq.FromXml(m_strReq);

            TChannelList& tPuChnList = cReq.GetPuChnBinding().GetPuChnList();
            TChannelList::iterator itor = tPuChnList.begin();
            while( itor != tPuChnList.end())
            {
                WriteLog(*itor, ERR_CUI_OPR_TIME_OUT, m_dwEventID, GetDomainName());
                itor++;
            }         
        }
        break;
    case RCS_UNBINDING_PUCHN_REQ:
        {
            CCuRcsUnbindingPuChnReq cReq;
            cReq.FromXml(m_strReq);
            TChannelList& tPuChnList = cReq.GetPuChnBinding().GetPuChnList();
            TChannelList::iterator itor = tPuChnList.begin();
            while( itor != tPuChnList.end())
            {
                WriteLog(*itor, ERR_CUI_OPR_TIME_OUT, m_dwEventID, GetDomainName());
                itor++;
            }   
        }
        break;
        //     case RCS_BINDING_PUCHN_REQ:
        //         {
        //             CCuRcsBindingPuChnReq cReq;
        //             cReq.FromXml(m_strReq);
        //             string strLogDesc = GetOptLogDesc(GetLanDesc(LAN_DESC_PUCHN_BINDING), GetLanDesc(LAN_DESC_NRU_ID) + ":"
        //                 + cReq.GetPuChnBinding().GetNruID() + " " 
        //                 + GetLanDesc(LAN_DESC_DISK_MOUNT_PATH) + ":"
        //                 + cReq.GetPuChnBinding().GetDiskMountPath()
        //                 , ERR_CUI_OPR_TIME_OUT);
        //             WriteLog(enUserOpTypeNRUManage, ERR_CUI_OPR_TIME_OUT, strLogDesc);
        //         }
        //         break;
        //     case RCS_UNBINDING_PUCHN_REQ:
        //         {
        //             CCuRcsUnbindingPuChnReq cReq;
        //             cReq.FromXml(m_strReq);
        //             string strLogDesc = GetOptLogDesc(GetLanDesc(LAN_DESC_PUCHN_UNBINDING), GetLanDesc(LAN_DESC_NRU_ID) + ":"
        //                 + cReq.GetPuChnBinding().GetNruID() + " " 
        //                 + GetLanDesc(LAN_DESC_DISK_MOUNT_PATH) + ":"
        //                 + cReq.GetPuChnBinding().GetDiskMountPath()
        //                 , ERR_CUI_OPR_TIME_OUT);
        //             WriteLog(enUserOpTypeNRUManage, ERR_CUI_OPR_TIME_OUT, strLogDesc);
        //         }
        //         break;
    case RCS_QUERY_RECORD_DAYS_REQ:
        {
        }
        break;
    case RCS_SET_RECORD_PARAM_REQ:
        {
            CRcsSetRecordParamReq cReq;
            cReq.FromXml(m_strReq);
            string strLogDesc = GetOptLogDesc(LAN_DESC_SET , GetLanDesc(LAN_DESC_RECORD_PARAM), GetLanDesc(LAN_DESC_DEVICE_ID) + ":"
                + cReq.GetRecordParam().GetPuChn().GetDevUri() + " " + GetLanDesc(LAN_DESC_DEV_CHN) + ":"
                + TransInt2String(cReq.GetRecordParam().GetPuChn().GetChnNO()+1) + " " + GetLanDesc(LAN_DESC_RECORD_MODE) + ":" +  cReq.GetRecordParam().GetRecMode() 
                + " " + GetLanDesc(LAN_DESC_RECORD_STREAM_TYPE) + ":" +  TransInt2String(cReq.GetRecordParam().GetRecStreamType())
                + " " + GetLanDesc(LAN_DESC_RECODE_SUPPORT_CHANGE_STREAM_type) + ":" +  TransInt2String(cReq.GetRecordParam().GetIsSupportChangeStreamType()), ERR_CUI_OPR_TIME_OUT);
            WriteLog(enUserOpTypeNRUManage, ERR_CUI_OPR_TIME_OUT, strLogDesc);
        }
        break;
    case TV_POLL_OPERATE_REQ:
        {
            CCuTvPollOperateReq cReq;
            cReq.FromXml(m_strReq);
            string strLogDesc = GetOptLogDesc(GetLanDesc(LAN_DESC_TVWIN_POLL), GetLanDesc(LAN_DESC_TVWALLID) + ":" 
                + cReq.GetTvChn().GetTvWallId() + " " + GetLanDesc(LAN_DESC_TVID) + ":" + TransInt2String(cReq.GetTvChn().GetTvId())
                + " " + GetLanDesc(LAN_DESC_TVDIV_ID) + ":" + TransInt2String(cReq.GetTvChn().GetTvDivId())
                ,ERR_CUI_OPR_TIME_OUT);
            WriteLog(enUserOpTypeTVWall, ERR_CUI_OPR_TIME_OUT, strLogDesc);
        }
        break;
    case TVWALL_SCHEME_SAVE_REQ:
        {
            CCuTvWallSchemeSaveReq cReq;
            cReq.FromXml(m_strReq);

			const string& strScheme = cReq.GetScheme();
			string strZipString = Base64Decode(strScheme);
			u8 abyBuff[60*1024] = {0};
			uLongf dwBuffLen = sizeof(abyBuff);
			if (Z_OK != uncompress(abyBuff, &dwBuffLen, (u8*)strZipString.c_str(),strZipString.length()))
			{
				break;
			}
			string strUnzip((char*)abyBuff,dwBuffLen);
			TTvWallScheme tScheme;
			tScheme.FromXml(strUnzip);

            string strLogDesc = GetOptLogDesc(GetLanDesc(LAN_DESC_SAVE), GetLanDesc(LAN_DESC_SCHEME), GetLanDesc(LAN_DESC_SCHEME_NAME) + ":"
                + tScheme.GetSchemeName() + " " + GetLanDesc(LAN_DESC_TVWALLID) + ":" + tScheme.GetTvWallId()
                , ERR_CUI_OPR_TIME_OUT);
            WriteLog(enUserOpTypeTVWall, ERR_CUI_OPR_TIME_OUT, strLogDesc);
        }
        break;
    case TVWALL_SCHEME_DEL_REQ:
        {
            CCuTvWallSchemeDelReq cReq;
            cReq.FromXml(m_strReq);
            string strLogDesc = GetOptLogDesc(GetLanDesc(LAN_DESC_DEL), GetLanDesc(LAN_DESC_SCHEME), GetLanDesc(LAN_DESC_TVWALLID) + ":"
                + cReq.GetTvWallId()
                , ERR_CUI_OPR_TIME_OUT);
            WriteLog(enUserOpTypeTVWall, ERR_CUI_OPR_TIME_OUT, strLogDesc);
        }
        break;
    case TVWALL_SCHEME_LOAD_REQ:
        {
            CCuTvWallSchemeLoadReq cReq;
            cReq.FromXml(m_strReq);
            string strLogDesc = GetOptLogDesc(GetLanDesc(LAN_DESC_LOAD), GetLanDesc(LAN_DESC_SCHEME), GetLanDesc(LAN_DESC_SCHEME_NAME) + ":"
                + cReq.GetSchemeName() + " " + GetLanDesc(LAN_DESC_TVWALLID) + ":" + cReq.GetTvWallId()
                , ERR_CUI_OPR_TIME_OUT);
            WriteLog(enUserOpTypeTVWall, ERR_CUI_OPR_TIME_OUT, strLogDesc);
        }
        break;
    case TVWALL_SCHEME_GROUP_SET_REQ:
        {
            CCuTvWallSchemeGroupSetReq cReq;
            cReq.FromXml(m_strReq);
            string strLogDesc = GetOptLogDesc(GetLanDesc(LAN_DESC_SET), GetLanDesc(LAN_DESC_TVWALL_SCHEME_GROUP), GetLanDesc(LAN_DESC_TVWALLID)
                + ":" + cReq.GetSchemeGroup().GetTvWallId()
                , ERR_CUI_OPR_TIME_OUT);
            WriteLog(enUserOpTypeTVWall, ERR_CUI_OPR_TIME_OUT, strLogDesc);
        }
        break;
    case TVWALL_SCHEME_GROUP_GET_REQ:
        {
        }
        break;
    case TVWALL_SCHEME_POLL_OPERATE_REQ:
        {
            CCuTvWallSchemePollOperateReq cReq;
            cReq.FromXml(m_strReq);
            string strLogDesc = GetOptLogDesc(GetLanDesc(LAN_DESC_SCHEME_POLL), GetLanDesc(LAN_DESC_TVWALLID)
                + ":" + cReq.GetTvWallId() + " " + GetLanDesc(LAN_DESC_POLL_STATE) + ":" + cReq.GetOprType()
                , ERR_CUI_OPR_TIME_OUT);
            WriteLog(enUserOpTypeTVWall, ERR_CUI_OPR_TIME_OUT, strLogDesc);
        }
        break;
    case TV_WIN_STYLE_SET_REQ:
        {
            CCuTvWinStyleSetReq cReq;
            cReq.FromXml(m_strReq);
            string strLogDesc = GetOptLogDesc(GetLanDesc(LAN_DESC_SET), GetLanDesc(LAN_DESC_TVWALL_STYLE), 
                GetLanDesc(LAN_DESC_TVWALLID) + ":" + cReq.GetTvWallId() + " " +  GetLanDesc(LAN_DESC_TVID)
                + ":" + TransInt2String(cReq.GetTvId()) + " " + GetLanDesc(LAN_DESC_TVWIN_STYLE) + ":" 
                + TransInt2String(cReq.GetStyle())
                , ERR_CUI_OPR_TIME_OUT);
            WriteLog(enUserOpTypeTVWall, ERR_CUI_OPR_TIME_OUT, strLogDesc);
        }
        break;
    case MPS_MAP_CREATE_REQ:
        {
            CCuMapCreateReq cReq;
            cReq.FromXml(m_strReq);
            string strLogDesc = GetOptLogDesc(GetLanDesc(LAN_DESC_CREATE), GetLanDesc(LAN_DESC_E_MAP), 
                GetLanDesc(LAN_DESC_MAP_NAME) + ":"+ cReq.GetMapInfo().GetMapName() + " " +
                GetLanDesc(LAN_DESC_MAP_TYPE) + ":" +  cReq.GetMapInfo().GetMapType(),
                ERR_CUI_OPR_TIME_OUT);
            WriteLog(enUserOpTypeMapConfig, ERR_CUI_OPR_TIME_OUT, strLogDesc);
        }
        break;
    case MPS_MAP_MOD_REQ:
        {
            CCuMapModReq cReq;
            cReq.FromXml(m_strReq);
            string strLogDesc = GetOptLogDesc(GetLanDesc(LAN_DESC_MOD), GetLanDesc(LAN_DESC_E_MAP), 
                GetLanDesc(LAN_DESC_MAP_ID) + ":" + cReq.GetMapInfo().GetMapId() + " " + GetLanDesc(LAN_DESC_MAP_NAME) + ":"
                + cReq.GetMapInfo().GetMapName() + " " + GetLanDesc(LAN_DESC_MAP_TYPE) + ":" +  cReq.GetMapInfo().GetMapType(),
                ERR_CUI_OPR_TIME_OUT);
            WriteLog(enUserOpTypeMapConfig, ERR_CUI_OPR_TIME_OUT, strLogDesc);
        }
        break;
    case MPS_MAP_DEL_REQ:
        {
            CCuMapDelReq cReq;
            cReq.FromXml(m_strReq);
            string strLogDesc = GetOptLogDesc(GetLanDesc(LAN_DESC_DEL), GetLanDesc(LAN_DESC_E_MAP), 
                GetLanDesc(LAN_DESC_MAP_ID) + ":" + cReq.GetMapId(),
                ERR_CUI_OPR_TIME_OUT);
            WriteLog(enUserOpTypeMapConfig, ERR_CUI_OPR_TIME_OUT, strLogDesc);
        }
        break;
    case MPS_MAP_ELEMENT_CREATE_REQ:
        {
            CCuMapElementCreateReq cReq;
            cReq.FromXml(m_strReq);
            string strLogDesc = GetOptLogDesc(GetLanDesc(LAN_DESC_CREATE), GetLanDesc(LAN_DESC_MAP_ELEMENT), 
                GetLanDesc(LAN_DESC_MAP_ELEMENT_NAME) + ":" + 
                cReq.GetMapElement().GetName() + " " + GetLanDesc(LAN_DESC_MAP_ELEMENT_TYPE) + ":"
                +  cReq.GetMapElement().GetType(),
                ERR_CUI_OPR_TIME_OUT);
            WriteLog(enUserOpTypeMapConfig, ERR_CUI_OPR_TIME_OUT, strLogDesc);
        }
        break;
    case MPS_MAP_ELEMENT_MOD_REQ:
        {
            CCuMapElementModReq cReq;
            cReq.FromXml(m_strReq);
            string strLogDesc = GetOptLogDesc(GetLanDesc(LAN_DESC_MOD), GetLanDesc(LAN_DESC_MAP_ELEMENT), 
                GetLanDesc(LAN_DESC_MAP_ELEMENT_ID) + ":" + cReq.GetMapElement().GetId() + " " + GetLanDesc(LAN_DESC_MAP_ELEMENT_NAME) + ":"
                + cReq.GetMapElement().GetName() + " " + GetLanDesc(LAN_DESC_MAP_ELEMENT_TYPE) + ":" +  cReq.GetMapElement().GetType(),
                ERR_CUI_OPR_TIME_OUT);
            WriteLog(enUserOpTypeMapConfig, ERR_CUI_OPR_TIME_OUT, strLogDesc);
        }
        break;
    case MPS_MAP_ELEMENT_DEL_REQ:
        {   
            CCuMapElementDelReq cReq;
            cReq.FromXml(m_strReq);
            string strLogDesc = GetOptLogDesc(GetLanDesc(LAN_DESC_DEL), GetLanDesc(LAN_DESC_MAP_ELEMENT), 
                GetLanDesc(LAN_DESC_MAP_ELEMENT_ID) + ":" + cReq.GetElementId() ,
                ERR_CUI_OPR_TIME_OUT);
            WriteLog(enUserOpTypeMapConfig, ERR_CUI_OPR_TIME_OUT, strLogDesc);
        }
        break;
    case MPS_POS_CORRECT_MARS_TO_EARTH_REQ:
        {
            CCuPosCorrectMarsToEarthReq cReq;
            cReq.FromXml(m_strReq);
            string strLogDesc = GetOptLogDesc(GetLanDesc(LAN_DESC_COORDINATES), GetLanDesc(LAN_DESC_TRANSFORMATION), 
                GetLanDesc(LAN_DESC_MARS_COORDINATES) + GetLanDesc(LAN_DESC_LAT) + ":" +  
                TransDouble2String(cReq.GetMars_lat()) + " " + GetLanDesc(LAN_DESC_LNG) 
                + ":" +  TransDouble2String(cReq.GetMars_lng()),
                ERR_CUI_OPR_TIME_OUT);
            WriteLog(enUserOpTypeMapConfig, ERR_CUI_OPR_TIME_OUT, strLogDesc);
        }
        break;
    case MPS_POS_CORRECT_EARTH_TO_MARS_REQ:
        {
            CCuPosCorrectEarthToMarsReq cReq;
            cReq.FromXml(m_strReq);
            string strLogDesc = GetOptLogDesc(GetLanDesc(LAN_DESC_COORDINATES), GetLanDesc(LAN_DESC_TRANSFORMATION), 
                GetLanDesc(LAN_DESC_EARTH_COORDINATES) + GetLanDesc(LAN_DESC_LAT) + ":" 
                + TransDouble2String(cReq.GetEarth_lat()) + " " + GetLanDesc(LAN_DESC_LNG) 
                + ":" +   TransDouble2String(cReq.GetEarth_lng()),
                ERR_CUI_OPR_TIME_OUT);
            WriteLog(enUserOpTypeMapConfig, ERR_CUI_OPR_TIME_OUT, strLogDesc);
        }
        break;
    case MPS_DATA_VERSION_GET_REQ:
        {

        }
        break;
	case MPS_GPS_DATA_BAKEUP_REQ:
		{
		}
		break;
    case ALARMLINK_CFG_GET_REQ:
        {
        }
        break;
    case ALARMLINK_VERSION_GET_REQ:
        {
        }
        break;
    case ALARMLINK_CFG_SET_REQ:
        {
            CCuAlarmLinkCfgSetReq cReq;
            cReq.FromXml(m_strReq);
            string strLogDesc = GetOptLogDesc(GetLanDesc(LAN_DESC_SET), GetLanDesc(LAN_DESC_ALARM_LINK_CFG), 
                GetLanDesc(LAN_DESC_ALARM_SRC) + GetLanDesc(LAN_DESC_DEVICE_ID) + ":" 
                + cReq.GetCfg().GetAlarmLinkSrc().GetDevUri() + " " + GetLanDesc(LAN_DESC_ALARM_INPUT_ID) + TransInt2String(cReq.GetCfg().GetAlarmLinkSrc().GetInputId())
                + " " + GetLanDesc(LAN_DESC_ALARM_TYPE) + cReq.GetCfg().GetAlarmLinkSrc().GetAlarmType()
                ,ERR_CUI_OPR_TIME_OUT);
            WriteLog(enUserOpTypeAlarmConfig, ERR_CUI_OPR_TIME_OUT, strLogDesc, cReq.GetCfg().GetAlarmLinkSrc().GetDevUri());
        }
        break;
    case UAS_CONFIG_UAS_GET_REQ:
        {
        }
        break;
    case UAS_CONFIG_UAS_SET_REQ:
        {
            CCuConfigUasSetReq cReq;
            cReq.FromXml(m_strReq);
            string strLogDesc = GetOptLogDesc(GetLanDesc(LAN_DESC_SET), GetLanDesc(LAN_DESC_UAS_CFG)
                ,ERR_CUI_OPR_TIME_OUT);
            WriteLog(enUserOpTypeSysParamConfig, ERR_CUI_OPR_TIME_OUT, strLogDesc);
        }
        break;
	case RECORD_ON_TV_REQ:
		{
			CCuRecordOnTvReq cReq;
			cReq.FromXml(m_strReq);
			string strLogDesc = GetOptLogDesc(GetLanDesc(LAN_DESC_RECORD_ON_TV), GetLanDesc(LAN_DESC_TVWALLID) + ":" 
				+ cReq.GetTvChn().GetTvWallId() + " " + GetLanDesc(LAN_DESC_TVID) + ":" + TransInt2String(cReq.GetTvChn().GetTvId())
				+ " " + GetLanDesc(LAN_DESC_TVDIV_ID) + ":"+  TransInt2String(cReq.GetTvChn().GetTvDivId())
				, ERR_CUI_OPR_TIME_OUT);
			WriteLog(enUserOpTypeTVWall, ERR_CUI_OPR_TIME_OUT, strLogDesc);
		}
		break;
	case RECORD_OFF_TV_REQ:
		{
			CCuRecordOffTvReq cReq;
			cReq.FromXml(m_strReq);
			string strLogDesc = GetOptLogDesc(GetLanDesc(LAN_DESC_RECORD_OFF_TV), GetLanDesc(LAN_DESC_TVWALLID) + ":" 
				+ cReq.GetTvChn().GetTvWallId() + " " + GetLanDesc(LAN_DESC_TVID) + ":" + TransInt2String(cReq.GetTvChn().GetTvId())
				+ " " + GetLanDesc(LAN_DESC_TVDIV_ID) + ":"+  TransInt2String(cReq.GetTvChn().GetTvDivId())
				, ERR_CUI_OPR_TIME_OUT);
			WriteLog(enUserOpTypeTVWall, ERR_CUI_OPR_TIME_OUT, strLogDesc);
		}
		break;
	case REAL_MEDIA_SWITCH_OCCUR_TYPE_REQ:
		{
			CCuRealMediaSwitchOccurTypeReq cReq;
			cReq.FromXml(m_strReq);
			string strLogDesc = GetOptLogDesc(GetLanDesc(LAN_DESC_ALARM_LINK), GetLanDesc(LAN_DESC_TVWALLID) + ":" 
				+ cReq.GetTvChn().GetTvWallId() + " " + GetLanDesc(LAN_DESC_TVID) + ":" + TransInt2String(cReq.GetTvChn().GetTvId())
				+ " " + GetLanDesc(LAN_DESC_TVDIV_ID) + ":"+  TransInt2String(cReq.GetTvChn().GetTvDivId()), ERR_CUI_OPR_TIME_OUT);
		}
		break;
    case REAL_MEDIA_ON_TV_REQ:
        {
            CCuRealMediaOnTvReq cReq;
            cReq.FromXml(m_strReq);
            string strLogDesc = GetOptLogDesc(GetLanDesc(LAN_DESC_REAL_MEDIA_ON_TV), GetLanDesc(LAN_DESC_TVWALLID) + ":" 
                + cReq.GetTvChn().GetTvWallId() + " " + GetLanDesc(LAN_DESC_TVID) + ":" + TransInt2String(cReq.GetTvChn().GetTvId())
                + " " + GetLanDesc(LAN_DESC_TVDIV_ID) + ":"+  TransInt2String(cReq.GetTvChn().GetTvDivId()) + " " 
                + GetLanDesc(LAN_DESC_TVWALL_STYLE) + ":" + cReq.GetTvOccurType()
                , ERR_CUI_OPR_TIME_OUT);
            WriteLog(enUserOpTypeTVWall, ERR_CUI_OPR_TIME_OUT, strLogDesc);
        }
        break;
    case REAL_MEDIA_OFF_TV_REQ:
        {
            CCuRealMediaOffTvReq cReq;
            cReq.FromXml(m_strReq);
            string strLogDesc = GetOptLogDesc(GetLanDesc(LAN_DESC_REAL_MEDIA_OFF_TV), GetLanDesc(LAN_DESC_TVWALLID) + ":" 
                + cReq.GetTvChn().GetTvWallId() + " " + GetLanDesc(LAN_DESC_TVID) + ":" + TransInt2String(cReq.GetTvChn().GetTvId())
                + " " + GetLanDesc(LAN_DESC_TVDIV_ID) + ":"+  TransInt2String(cReq.GetTvChn().GetTvDivId()) + " " + 
                GetLanDesc(LAN_DESC_TVWALL_STYLE) + ":" + cReq.GetTvOccurType()
                , ERR_CUI_OPR_TIME_OUT);
            WriteLog(enUserOpTypeTVWall, ERR_CUI_OPR_TIME_OUT, strLogDesc);
        }
        break;
    case VCR_CTRL_REQ:
        {
            CCuVcrCtrlReq cReq;
            cReq.FromXml(m_strReq);
            string strLogDesc = GetOptLogDesc(GetLanDesc(LAN_DESC_VCR_CTRL), GetLanDesc(LAN_DESC_VCR_CMD) + ":" 
                + cReq.GetVcrCmd() + " " + GetLanDesc(LAN_DESC_PLAY_RATE) + ":" + TransInt2String(cReq.GetPlayRate())
                + " " + GetLanDesc(LAN_DESC_SEEK_TIME) + ":"+  TransInt2String(cReq.GetSeekTime())
                , ERR_CUI_OPR_TIME_OUT);
            WriteLog(enUserOpTypeTVWall, ERR_CUI_OPR_TIME_OUT, strLogDesc);
        }
        break;
    case PLAT_REC_ON_TV_REQ:
        {
            CCuPlatRecOnTvReq cReq;
            cReq.FromXml(m_strReq);
            string strLogDesc = GetOptLogDesc(GetLanDesc(LAN_DESC_PLAT_REC_ON_TV), GetLanDesc(LAN_DESC_REC_TOKEN) + ":" 
                + cReq.GetRecDurationToken() + " " + GetLanDesc(LAN_DESC_TVWALLID) + ":" 
                + cReq.GetTvChn().GetTvWallId() + " " + GetLanDesc(LAN_DESC_TVID) + ":" + TransInt2String(cReq.GetTvChn().GetTvId())
                + " " + GetLanDesc(LAN_DESC_TVDIV_ID) + ":"+  TransInt2String(cReq.GetTvChn().GetTvDivId())
                , ERR_CUI_OPR_TIME_OUT);
            WriteLog(enUserOpTypeTVWall, ERR_CUI_OPR_TIME_OUT, strLogDesc);
        }
        break;
    case PLAYBACK_PROGRESS_QUERY_REQ:
    default:
        break;
    }
}

string CPeSingleInterAction::GetOptLogDesc(const string& Opt, const string& OptDevInfo, u32 dwErrorCode)
{
    return GetLanDesc(Opt) + GetErrorCode(dwErrorCode) + " " + OptDevInfo;
}

string CPeSingleInterAction::GetOptLogDesc(const string& OptType,const string& Type, const string& OptDevInfo, u32 dwErrorCode)
{
    return GetLanDesc(OptType) + GetLanDesc(Type) + GetErrorCode(dwErrorCode) + " " + OptDevInfo ;
}

void CPeSingleInterAction::WriteLog(TOperateType enOprType, u32 dwErrorCode, const string& strLogDesc, const string &strDevUri)
{
    CUsrOprLogTask* pcTask = new CUsrOprLogTask(GetInstance(), m_strCuSession, 
        string(DEV_TYPE_UAS) + "@" + m_tPeUri.GetDomain(),
        enOprType,
        dwErrorCode,
        strLogDesc,
        strDevUri
        );
    pcTask->InitStateMachine();
    pcTask->StartSendLog();
}

void CPeSingleInterAction::WriteLog(const TChannel& tDevChn, const u32 dwErrorCode, const u32 dwEvent, const string& strDomainName)
{
    CGetPuData *pcTask = new CGetPuData(GetInstance(), m_strCuSession, tDevChn, dwErrorCode, m_strReq, dwEvent, strDomainName);
    pcTask->InitStateMachine(); 
	if (pcTask->StartWriteLog() != PROCMSG_OK)
	{
		delete pcTask;
		pcTask = NULL;
	}
}

string CPeSingleInterAction::GetErrorCode(u32 dwErrorCode)
{
    if( CMS_SUCCESS == dwErrorCode)
    {
        return GetLanDesc(LAN_DESC_SUCCESS);
    }
    else
        //             OspExtEventDesc(cRsp.GetEventId() - 1).c_str(), cRsp.GetEventId() - 1,
        //             m_tPeUri.GetURIString().c_str(), m_strCuSession.c_str(), 
        //             OspExtEventDesc(cRsp.GetEventId()).c_str(), cRsp.GetEventId() );
    {
        return GetLanDesc(LAN_DESC_FAIL);
        //         SendRspToCu(m_strRspOfErr, m_ptRecvNode);
        //         NextState(NOT_REQUEST_STATE);
        //         return TIMERPOLL_DEL;
    }
    //     return TIMERPOLL_DONE;
}
string CPeSingleInterAction::GetPeSession()
{
    CCuiInst *pcInst = (CCuiInst *)GetInstance();
    return pcInst->IsProvideServer(m_tPeUri, GetTaskNO());
}

u32 CPeSingleInterAction::StartSendReq()
{
    u32 dwRet = PROCMSG_OK;

    CPeAgent *pcPeAgent = (CPeAgent *)g_cCuiDataMgr.GetPeAgentPtrByPeName(m_tPeUri.GetURIString(), GetInstance());
    CMS_ASSERT(NULL != pcPeAgent );

    //如果当前CPeAgent不在服务态，则直接删除该task
    if(CPeAgent::Service != pcPeAgent->GetState())
    {
        TASKLOG(NORMAL_TASK, EVENT_LEV, "不在服务态，立马回复消息时间！\n");

        string strRsp = ReplaceXmlKeyText(m_strRspOfErr, "13019", "<errorCode>", "</errorCode>");      
        SendRspToCu(strRsp, m_ptRecvNode);
        NextState(NOT_REQUEST_STATE);

        return PROCMSG_DEL; 
    }

    if( pcPeAgent )
    {
        m_strReq = ReplaceXmlKeyText(m_strReq, pcPeAgent->m_strSessionID, "<session>", "</session>");
    }

    CMS_ASSERT(!m_strReq.empty());
    dwRet = PostMsgReq(m_strReq, m_tPeUri);
    if(dwRet != PROCMSG_OK)
    {
        return dwRet;
    }
    NextState(WAIT_RESPONSE_STATE);

    return PROCMSG_OK;
}


//查询事务
CPeQueryTask::CPeQueryTask(CInstExt *pcInst, const string& strCuSession, const string& strPeUri, PTRecvNode ptRecvNode) : CCallBackTask(pcInst, strCuSession, strPeUri)
{
    CMS_ASSERT(!strCuSession.empty());
    CMS_ASSERT(!strPeUri.empty());
    CMS_ASSERT(ptRecvNode != NULL);

    m_ptRecvNode = ptRecvNode;
    m_pRsp = NULL;
    m_strQueryTag.clear();
    m_VecNtfReqList.clear();
}

CPeQueryTask::~CPeQueryTask()
{
    if ( ! m_strQueryTag.empty() )
    {
        g_cCuiDataMgr.m_mapSessToTaskMap.Erase(m_strQueryTag);
        m_strQueryTag.clear();
    }

    TerminateTask(); 

    if(m_pRsp != NULL)
    {
        delete m_pRsp;
        m_pRsp = NULL;
    }
}

void CPeQueryTask::TerminateTask()
{
    //     //瞬时事务不需要管理
    //     if ( GetState() == WAIT_RESPONSE_STATE && m_ptRecvNode != NULL )
    //     {
    //         if ( NULL != m_pRsp )
    //         {
    //             m_pRsp->SetErrorCode(ERR_CUI_TASK_DEL);
    //             string strRsp = m_pRsp->ToXml();
    //             SendRspToCu(strRsp, m_ptRecvNode);          
    //         }
    //     }
}

void CPeQueryTask::InitStateMachine()
{
    CStateProc cNotQueryProc;
    cNotQueryProc.ProcMsg = (CSipTask::PFProcMsg)&CPeQueryTask::NotQueryProcMsg;
    cNotQueryProc.TimerPoll = (CSipTask::PFTimerPoll)&CPeQueryTask::WaitReqTimerPoll;
    cNotQueryProc.ProcErrMsg = (CSipTask::PFProcMsg)&CPeQueryTask::NotQueryProcMsg;
    AddRuleProc(NOT_QUERY_STATE, cNotQueryProc);

    CStateProc cWaitRspProc;
    cWaitRspProc.ProcMsg = (CSipTask::PFProcMsg)&CPeQueryTask::WaitRspProcMsg;
    cWaitRspProc.TimerPoll = (CSipTask::PFTimerPoll)&CPeQueryTask::WaitRspTimerPoll;
    cWaitRspProc.ProcErrMsg = (CSipTask::PFProcMsg)&CPeQueryTask::WaitRspProcMsg;
    AddRuleProc(WAIT_RESPONSE_STATE, cWaitRspProc);

    CStateProc cWaitNotifyProc;
    cWaitNotifyProc.ProcMsg = (CSipTask::PFProcMsg)&CPeQueryTask::WaitNotifyProcMsg;
    cWaitNotifyProc.TimerPoll = (CSipTask::PFTimerPoll)&CPeQueryTask::WaitNotifyTimerPoll;
    cWaitNotifyProc.ProcErrMsg = (CSipTask::PFProcMsg)&CPeQueryTask::WaitNotifyProcMsg;
    AddRuleProc(WAIT_NOTIFY_STATE, cWaitNotifyProc);

    NextState(NOT_QUERY_STATE);
}

u32 CPeQueryTask::NotQueryProcMsg(CMessage *const pcMsg)
{
    TASKLOG(NORMAL_TASK, EVENT_LEV, "收到CU用户会话[%s]操作外设[%s]的请求[%s--%hu]\n",
        m_strCuSession.c_str(), m_tPeUri.GetURIString().c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

    u32 dwRet = PROCMSG_OK;
    switch(pcMsg->event)
    {
    case RCS_QUERY_RECORD_REQ:
        {
            /*dwRet = OnNotQueryMsg<CRcsQueryRecordReq,CCuRcsQueryRecordRsp>(pcMsg);*/
            CRcsQueryRecordReq *pcReq = (CRcsQueryRecordReq *)pcMsg->content;
            //缓存错误消息，如果失败可以返回消息
            //存储相应的信息
            m_pRsp = new CCuRcsQueryRecordRsp;
            m_pRsp->SetSeqNum(pcReq->GetSeqNum());
            m_pRsp->SetSession(pcReq->GetSession());

            CCuRcsQueryRecordRsp *pRsp = (CCuRcsQueryRecordRsp *)m_pRsp;

            string strDomainName = GetDomainNameWithoutPreNameFromURI(m_tPeUri.GetURIString());
            if(strDomainName.empty())
            {
                pRsp->SetDomainName(GetDomainName());
            }
            else
            {
                pRsp->SetDomainName(strDomainName);
            }

            //存储QueryTag
            g_cCuiDataMgr.m_mapSessToTaskMap.Insert(pcReq->GetQueryTag(), GetTaskNO());
            m_strQueryTag = pcReq->GetQueryTag();

            string strPeSession = GetPeSession();
            if(strPeSession.empty())
            {
                //缓存req消息
                m_strReq = pcReq->ToXml();
            }
            else
            {
                pcReq->SetSession(strPeSession);
                dwRet = PostMsgReqToPe(*pcReq);
                if(dwRet != PROCMSG_OK)
                {
                    return dwRet;
                }

                NextState(WAIT_RESPONSE_STATE);
            }
            return dwRet;
        }
        break;
    case RCS_QUERY_NRU_REQ:
        {
            dwRet = OnNotQueryMsg<CCuRcsQueryNruReq, CCuRcsQueryNruRsp>(pcMsg);
        }
        break;
    case RCS_QUERY_PUCHN_BINDING_REQ:
        {
            dwRet = OnNotQueryMsg<CCuRcsQueryPuChnBindingReq, CCuRcsQueryPuChnBindingRsp>(pcMsg);
        }
        break;
    case RCS_QUERY_RECORD_PARAM_REQ:
        {
            dwRet = OnNotQueryMsg<CCuRcsQueryRecordParamReq, CCuRcsQueryRecordParamRsp>(pcMsg);
        }
        break;
    case TVWALL_SCHEME_QUERY_REQ:
        {
            dwRet = OnNotQueryMsg<CCuTvWallSchemeQueryReq, CCuTvWallSchemeQueryRsp>(pcMsg);
        }
        break;
    case MPS_MAP_QRY_REQ:
        {
            dwRet = OnNotQueryMsg<CCuMapQryReq, CCuMapQryRsp>(pcMsg);
        }
        break;
    case MPS_MAP_ELEMENT_QRY_REQ:
        {
            dwRet = OnNotQueryMsg<CCuMapElementQryReq, CCuMapElementQryRsp>(pcMsg);
        }
		break;
	case MPS_MAP_ELEMENT_SEARCH_REQ:
		{
			dwRet = OnNotQueryMsg<CCuMapElementSearchReq, CCuMapElementSearchRsp>(pcMsg);
		}
		break;
    case MPS_MAP_AREA_QRY_REQ:
        {
            dwRet = OnNotQueryMsg<CCuMapAreaQryReq, CCuMapAreaQryRsp>(pcMsg);
        }
        break;
    case MPS_ELEMENT_IMAGE_QRY_REQ:
        {
            dwRet = OnNotQueryMsg<CCuElementImageQryReq, CCuElementImageQryRsp>(pcMsg);
        }
        break;
	case MPS_GPS_TRACE_QRY_REQ:
		{
			dwRet = OnNotQueryMsg<CCuGpsTraceQryReq,CCuGpsTraceQryRsp>(pcMsg);
		}
		break;
    case ALARMLINK_QUERY_REQ:
        {
            dwRet = OnNotQueryMsg<CCuAlarmLinkQueryReq, CCuAlarmLinkQueryRsp>(pcMsg);
        }
        break;
    case UAS_CUSER_CUSTOMDATA_QRY_REQ:
        {
			CCuUserCustomDataQryReq *pcReq = (CCuUserCustomDataQryReq*)pcMsg->content;
			const string &strUserId = g_cCuiDataMgr.GetCuserIdByCuserSession(pcReq->GetSession(), GetInstance());

			//添加操作日志
			CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession, GetInstance());
			if (NULL != pcCuAgent)
			{
				s32 nFragNo = 0;
				pcReq->GetCondition().GetFragNo(nFragNo);
				if (0 == nFragNo || -1 == nFragNo) //只记录第一分片的日志
				{
					string strUserId;
					s32 nDataType = 0;
					string strDataId;
					pcReq->GetCondition().GetUserId(strUserId);
					pcReq->GetCondition().GetDataType(nDataType);
					pcReq->GetCondition().GetDataId(strDataId);
					string strLogDesc = string( GetLanDesc(LAN_DESC_QRY) + GetLanDesc(LAN_DESC_CUSTOM) + GetLanDesc(LAN_DESC_SCHEME)
						+ GetLanDesc(LAN_DESC_TAG) + "[" + pcReq->GetQueryTag() + "] "
						+ GetLanDesc(LAN_DESC_DATA) + "[" + Num2Str(nDataType) + "#" + strDataId + "] "
						+ GetLanDesc(LAN_DESC_USER) + "[" + pcCuAgent->m_strUserName +":"+pcCuAgent->m_strClientLocalIp + "] "
						+ GetLanDesc(LAN_DESC_DATE) + "[" + CCMSTime().GetXSDDateTime()+ "]");

					WriteLog(enUserOpTypeOther, CMS_SUCCESS, strLogDesc, strUserId);
				}
			}

			if (strUserId.empty())
			{
				m_pRsp = new CCuUserCustomDataQryRsp;
				m_pRsp->SetSeqNum(pcReq->GetSeqNum());
				m_pRsp->SetSession(pcReq->GetSession());

				CCuUserCustomDataQryRsp *pRsp = (CCuUserCustomDataQryRsp *)m_pRsp;

				CMS_ASSERT(NULL != m_ptRecvNode);
				m_pRsp->SetErrorCode(ERR_CUI_QUERYING_INNER_DATA);
				string strRsp = m_pRsp->ToXml();
				SendRspToCu(strRsp, m_ptRecvNode);
				NextState(NOT_QUERY_STATE);

				return PROCMSG_DEL;
			}
			pcReq->GetCondition().SetUserId(strUserId);
            dwRet = OnNotQueryMsg<CCuUserCustomDataQryReq, CCuUserCustomDataQryRsp>(pcMsg);
        }
        break;
    case UAS_OPERATE_LOG_QRY_REQ:
        {
            dwRet = OnNotQueryMsg<CCuOperateLogQryReq, CCuOperateLogQryRsp>(pcMsg);          
        }
        break;
    case UAS_DEVICE_LOG_QRY_REQ:
        {
            dwRet = OnNotQueryMsg<CCuDeviceLogQryReq, CCuDeviceLogQryRsp>(pcMsg); 
        }
        break;
    case UAS_VIDEOSOURCE_PRESETCFG_QRY_REQ:
        {
            dwRet = OnNotQueryMsg<CCuVideosourcePresetcfgQryReq, CCuVideosourcePresetcfgQryRsp>(pcMsg); 
        }
        break;
    case UAS_ALARM_LOG_QRY_REQ:
        {
            dwRet = OnNotQueryMsg<CCuAlarmLogQryReq, CCuAlarmLogQryRsp>(pcMsg); 
        }
        break;
    case RCS_QUERY_DISK_INFO_REQ:
        {
            dwRet = OnNotQueryMsg<CCuRcsQueryDiskInfoReq, CCuRcsQueryDiskInfoRsp>(pcMsg); 
        }
        break;
	case  CAPS_QUERY_DEVICE_LIST_REQ:
		{
			dwRet = OnNotQueryMsg<CCuQueryCAPSDeviceListReq, CCuQueryCAPSDeviceListRsp>(pcMsg); 
		}
		break;
    default:
        {
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"收到未知消息");
        }
        break;
    }  
    return dwRet;
}

u32 CPeQueryTask::WaitReqTimerPoll()
{
    if ( GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout() )
    {
        TASKLOG(NORMAL_TASK, ERROR_LEV, "在处理[%s--%d]消息时注册外设模块[%s]CU用户会话[%s]发送应答消息[%s--%d]\n",
            OspExtEventDesc((u16)m_pRsp->GetEventId() - 1).c_str(), m_pRsp->GetEventId() - 1,
            m_tPeUri.GetURIString().c_str(), m_strCuSession.c_str(), 
            OspExtEventDesc((u16)m_pRsp->GetEventId()).c_str(), m_pRsp->GetEventId() );

        CMS_ASSERT(NULL != m_ptRecvNode);

        m_pRsp->SetErrorCode(ERR_CUI_OPR_TIME_OUT);
        string strRsp = m_pRsp->ToXml();
        SendRspToCu(strRsp, m_ptRecvNode);
        NextState(NOT_QUERY_STATE);

        return TIMERPOLL_DEL;
    }
    return TIMERPOLL_DONE;
}

u32 CPeQueryTask::WaitRspProcMsg(CMessage *const pcMsg)
{
    COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
    TASKLOG(NORMAL_TASK, EVENT_LEV, "CPeQueryTask[TaskNO %u]收到服务器[%s]的应答消息[%s--%hu]\n",
        GetTaskNO(), pcOspSipMsg->GetSipToUri().GetURIString().c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

    u32 dwRet = PROCMSG_OK;

    switch (pcMsg->event)
    {
    case OSP_SIP_MSG_PROC_FAIL:
        {
            CMS_ASSERT(NULL != m_ptRecvNode);

            m_pRsp->SetErrorCode(ERR_CUI_SIP_MSG_FAIL);
            string strRsp = m_pRsp->ToXml();
            SendRspToCu(strRsp, m_ptRecvNode);
            NextState(NOT_QUERY_STATE);
            dwRet = PROCMSG_DEL;
        }
        break;
    case OSP_SIP_DIALOG_TERMINATE:
        {
            //不会有Dlg消息
            CMS_ASSERT( 0 && "CSingleInterAction::WaitRspProcMsg不可能出现会话内消息" );
        }
        break;
    case RCS_QUERY_RECORD_RSP:
        {
            dwRet = OnWaitRspMsg<CRcsQueryRecordRsp, CCuRcsQueryRecordRsp>(pcMsg);
        }
        break;
    case RCS_QUERY_NRU_RSP:
        {
            dwRet = OnWaitRspMsg<CRcsQueryNruRsp, CCuRcsQueryNruRsp>(pcMsg);
        }
        break;
    case RCS_QUERY_PUCHN_BINDING_RSP:
        {
            dwRet = OnWaitRspMsg<CRcsQueryPuChnBindingRsp, CCuRcsQueryPuChnBindingRsp>(pcMsg);
        }
        break;
    case RCS_QUERY_RECORD_PARAM_RSP:
        {
            dwRet = OnWaitRspMsg<CRcsQueryRecordParamRsp, CCuRcsQueryRecordParamRsp>(pcMsg);
        }
        break;
    case TVWALL_SCHEME_QUERY_RSP:
        {
            dwRet = OnWaitRspMsg<CTvWallSchemeQueryRsp, CCuTvWallSchemeQueryRsp>(pcMsg);
        }
        break;
    case MPS_MAP_QRY_RSP:
        {
            dwRet = OnWaitRspMsg<CMapQryRsp, CCuMapQryRsp>(pcMsg);
        }
        break;
    case MPS_MAP_ELEMENT_QRY_RSP:
        {
            dwRet = OnWaitRspMsg<CMapElementQryRsp, CCuMapElementQryRsp>(pcMsg);
        }
		break;
	case MPS_MAP_ELEMENT_SEARCH_RSP:
		{
			dwRet = OnWaitRspMsg<CMapElementSearchRsp, CCuMapElementSearchRsp>(pcMsg);
		}
		break;
    case MPS_MAP_AREA_QRY_RSP:
        {
            dwRet = OnWaitRspMsg<CMapAreaQryRsp, CCuMapAreaQryRsp>(pcMsg);
        }
        break;
    case MPS_ELEMENT_IMAGE_QRY_RSP:
        {
            dwRet = OnWaitRspMsg<CElementImageQryRsp, CCuElementImageQryRsp>(pcMsg);
        }
        break;
	case MPS_GPS_TRACE_QRY_RSP:
		{
			dwRet = OnWaitRspMsg<CGpsTraceQryRsp,CCuGpsTraceQryRsp>(pcMsg);
		}
		break;
    case ALARMLINK_QUERY_RSP:
        {
            dwRet = OnWaitRspMsg<CAlarmLinkQueryRsp, CCuAlarmLinkQueryRsp>(pcMsg);
        }
        break;
    case UAS_OPERATE_LOG_QRY_RSP:
        {
            dwRet = OnWaitRspMsg<COperateLogQryRsp, CCuOperateLogQryRsp>(pcMsg);
        }
        break;
    case UAS_DEVICE_LOG_QRY_RSP:
        {
            dwRet = OnWaitRspMsg<CDeviceLogQryRsp, CCuDeviceLogQryRsp>(pcMsg); 
        }
        break;
    case UAS_VIDEOSOURCE_PRESETCFG_QRY_RSP:
        {
            dwRet = OnWaitRspMsg<CVideosourcePresetcfgQryRsp, CCuVideosourcePresetcfgQryRsp>(pcMsg); 
        }
        break;
    case UAS_ALARM_LOG_QRY_RSP:
        {
            dwRet = OnWaitRspMsg<CAlarmLogQryRsp, CCuAlarmLogQryRsp>(pcMsg);
        }
        break;
    case UAS_CUSER_CUSTOMDATA_QRY_RSP:
        {
            dwRet = OnWaitRspMsg<CUserCustomDataQryRsp, CCuUserCustomDataQryRsp>(pcMsg);
        }
        break;
    case RCS_QUERY_DISK_INFO_RSP:
        {
            dwRet = OnWaitRspMsg<CRcsQueryDiskInfoRsp, CCuRcsQueryDiskInfoRsp>(pcMsg);
        }
        break; 
	case CAPS_QUERY_DEVICE_LIST_RSP:
		{
			dwRet = OnWaitRspMsg<CQueryDeviceListRsp, CCuQueryCAPSDeviceListRsp>(pcMsg);
		}
		break;
    case RCS_QUERY_RECORD_NTF_REQ:
    case RCS_QUERY_NRU_NTF_REQ:
    case RCS_QUERY_PUCHN_BINDING_NTF_REQ:
    case RCS_QUERY_RECORD_PARAM_NTF_REQ:
    case TVWALL_SCHEME_NTF_REQ:
    case MPS_MAP_QRY_NTF_REQ:
	case MPS_MAP_ELEMENT_QRY_NTF_REQ:
	case MPS_MAP_ELEMENT_SEARCH_NTF_REQ:
    case MPS_MAP_AREA_QRY_NTF_REQ:
    case MPS_ELEMENT_IMAGE_QRY_NTF_REQ:
	case MPS_GPS_TRACE_QRY_NTF_REQ:
    case ALARMLINK_NTF_REQ:
    case UAS_OPERATE_LOG_QRY_NTF_REQ:
    case UAS_DEVICE_LOG_QRY_NTF_REQ:
    case UAS_VIDEOSOURCE_PRESETCFG_QRY_NTF_REQ:
    case UAS_ALARM_LOG_QRY_NTF_REQ:
    case UAS_CUSER_CUSTOMDATA_QRY_NTF_REQ:
    case RCS_QUERY_DISK_INFO_NTF_REQ:
	case CAPS_QUERY_DEVICE_LIST_NTF_REQ:
        {
            if((m_pRsp->GetEventId()+1) == pcMsg->event)
            {
                if(pcMsg->content != NULL)
                {
                    string strContect((char *)pcMsg->content, pcMsg->length);
                    m_VecNtfReqList.push_back(strContect);
                    TASKLOG(NORMAL_TASK, CRITICAL_LEV, "在[Wait Rsp State]收到相应的Ntf消息[%s--%hu]\n",
                        OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
                }              
            }
            else
            {
                TASKLOG(NORMAL_TASK, CRITICAL_LEV, "在[Wait Rsp State]收到不对应的Ntf消息[%s--%hu]\n",
                    OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            }
        }
        break;
    default:
        {
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"收到未知消息");
        }
        break;
    }

    if(PROCMSG_DEL == dwRet)
    {
        delete this;
    }

    return dwRet;
}

u32 CPeQueryTask::WaitRspTimerPoll()
{
    if ( GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout() )
    {
        TASKLOG(NORMAL_TASK, ERROR_LEV, "请求[%s--%d]超时，直接操作外设[%s]CU用户会话[%s]发送应答消息[%s--%d]\n",
            OspExtEventDesc((u16)m_pRsp->GetEventId() - 1).c_str(), m_pRsp->GetEventId() - 1,
            m_tPeUri.GetURIString().c_str(), m_strCuSession.c_str(), 
            OspExtEventDesc((u16)m_pRsp->GetEventId()).c_str(), m_pRsp->GetEventId() );

        CMS_ASSERT(NULL != m_ptRecvNode);
        m_pRsp->SetErrorCode(ERR_CUI_OPR_TIME_OUT);
        string strRsp = m_pRsp->ToXml();
        SendRspToCu(strRsp, m_ptRecvNode);
        NextState(NOT_QUERY_STATE);
        return TIMERPOLL_DEL;
    }
    return TIMERPOLL_DONE;
}

u32 CPeQueryTask::WaitNotifyProcMsg(CMessage *const pcMsg)
{
    //用于处理rsp状态收到的ntf消息
    u32 dwRet = PROCMSG_OK;
    while(m_VecNtfReqList.size() != 0)
    {
        CMessage cMsg;
        cMsg.event = (u16)m_pRsp->GetEventId()+1;
        string content = m_VecNtfReqList.front();
        m_VecNtfReqList.pop_front();
        cMsg.content = (u8 *)content.c_str();

        //这块处理不论死活
        ProcNotifyMsg(&cMsg);
    }
    dwRet = ProcNotifyMsg(pcMsg);

    return dwRet;
}

u32 CPeQueryTask::ProcNotifyMsg(CMessage *const pcMsg)
{
    COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
    TASKLOG(NORMAL_TASK, EVENT_LEV, "CPeSingleInterAction[TaskNO %u]收到服务器[%s]的应答消息[%s--%hu]\n",
        GetTaskNO(), pcOspSipMsg->GetSipToUri().GetURIString().c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

    u32 dwRet = PROCMSG_OK;

    switch (pcMsg->event)
    {
    case OSP_SIP_MSG_PROC_FAIL:
        {
            CMS_ASSERT(NULL != m_ptRecvNode);

            m_pRsp->SetErrorCode(ERR_CUI_SIP_MSG_FAIL);
            string strRsp = m_pRsp->ToXml();
            SendRspToCu(strRsp, m_ptRecvNode);
            NextState(NOT_QUERY_STATE);
            dwRet = PROCMSG_DEL;
        }
        break;
    case OSP_SIP_DIALOG_TERMINATE:
        {
            //不会有Dlg消息
            CMS_ASSERT( 0 && "CSingleInterAction::WaitRspProcMsg不可能出现会话内消息" );
        }
        break;
    case RCS_QUERY_RECORD_NTF_REQ:
        {
            dwRet = OnWaitNotifyMsg<CRcsQueryRecordNtfReq, CCuRcsQueryRecordRsp>(pcMsg);
        }
        break;
    case RCS_QUERY_NRU_NTF_REQ:
        {
            dwRet = OnWaitNotifyMsg<CRcsQueryNruNtfReq, CCuRcsQueryNruRsp>(pcMsg);
        }
        break;
    case RCS_QUERY_PUCHN_BINDING_NTF_REQ:
        {
            dwRet = OnWaitNotifyMsg<CRcsQueryPuChnBindingNtfReq, CCuRcsQueryPuChnBindingRsp>(pcMsg);
        }
        break;
    case RCS_QUERY_RECORD_PARAM_NTF_REQ:
        {
            dwRet = OnWaitNotifyMsg<CRcsQueryRecordParamNtfReq, CCuRcsQueryRecordParamRsp>(pcMsg);
        }
        break;
    case TVWALL_SCHEME_NTF_REQ:
        {
            dwRet = OnWaitNotifyMsg<CTvWallSchemeNtfReq, CCuTvWallSchemeQueryRsp>(pcMsg);
        }
        break;
    case MPS_MAP_QRY_NTF_REQ:
        {
            dwRet = OnWaitNotifyMsg<CMapQryNtfReq, CCuMapQryRsp>(pcMsg);
        }
        break;
    case MPS_MAP_ELEMENT_QRY_NTF_REQ:
        {
            dwRet = OnWaitNotifyMsg<CMapElementQryNtfReq, CCuMapElementQryRsp>(pcMsg);
        }
		break;
	case MPS_MAP_ELEMENT_SEARCH_NTF_REQ:
		{
			dwRet = OnWaitNotifyMsg<CMapElementSearchNtfReq, CCuMapElementSearchRsp>(pcMsg);
		}
		break;
    case MPS_MAP_AREA_QRY_NTF_REQ:
        {
            dwRet = OnWaitNotifyMsg<CMapAreaQryNtfReq, CCuMapAreaQryRsp>(pcMsg);
        }
        break;
    case MPS_ELEMENT_IMAGE_QRY_NTF_REQ:
        {
            dwRet = OnWaitNotifyMsg<CElementImageQryNtfReq, CCuElementImageQryRsp>(pcMsg);
        }
        break;
	case MPS_GPS_TRACE_QRY_NTF_REQ:
		{
			dwRet = OnWaitNotifyMsg<CGpsTraceQryNtfReq,CCuGpsTraceQryRsp>(pcMsg);
		}
		break;
    case ALARMLINK_NTF_REQ:
        {
            dwRet = OnWaitNotifyMsg<CAlarmLinkNtfReq, CCuAlarmLinkQueryRsp>(pcMsg);
        }
        break;
    case UAS_OPERATE_LOG_QRY_NTF_REQ:
        {
            dwRet = OnWaitNotifyMsg<COperateLogQryNtfReq, CCuOperateLogQryRsp>(pcMsg);          
        }
        break;
    case UAS_DEVICE_LOG_QRY_NTF_REQ:
        {
            dwRet = OnWaitNotifyMsg<CDeviceLogQryNtfReq, CCuDeviceLogQryRsp>(pcMsg); 
        }
        break;
    case UAS_VIDEOSOURCE_PRESETCFG_QRY_NTF_REQ:
        {
            dwRet = OnWaitNotifyMsg<CVideosourcePresetcfgQryNtfReq, CCuVideosourcePresetcfgQryRsp>(pcMsg); 
        }
        break;
    case UAS_ALARM_LOG_QRY_NTF_REQ:
        {
            dwRet = OnWaitNotifyMsg<CAlarmLogQryNtfReq, CCuAlarmLogQryRsp>(pcMsg); 
        }
        break;
    case UAS_CUSER_CUSTOMDATA_QRY_NTF_REQ:
        {
            dwRet = OnWaitNotifyMsg<CUserCustomDataQryNtfReq, CCuUserCustomDataQryRsp>(pcMsg); 

			//添加操作日志
			CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession, GetInstance());
			if (NULL != pcCuAgent)
			{
				COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
				CUserCustomDataQryNtfReq cNtf;
				pcOspSipMsg->GetMsgBody(cNtf);
				if (cNtf.IsLastNtf()) //只记录最后一条通知
				{
					string strLogDesc = string( GetLanDesc(LAN_DESC_QRY) + GetLanDesc(LAN_DESC_CUSTOM) + GetLanDesc(LAN_DESC_SCHEME)
						+ GetLanDesc(LAN_DESC_NTF)
						+ GetLanDesc(LAN_DESC_TAG) + "[" + cNtf.GetQueryTag() + "] "
						+ GetLanDesc(LAN_DESC_USER) + "[" + pcCuAgent->m_strUserName +":"+pcCuAgent->m_strClientLocalIp + "] "
						+ GetLanDesc(LAN_DESC_DATE) + "[" + CCMSTime().GetXSDDateTime()+ "]");

					const string &strUserId = g_cCuiDataMgr.GetCuserIdByCuserSession(m_strCuSession, GetInstance());
					WriteLog(enUserOpTypeOther, CMS_SUCCESS, strLogDesc, strUserId);
				}
			}
        }
        break;
    case RCS_QUERY_DISK_INFO_NTF_REQ:
        {
            //过滤非数据分区返回给cu
            CRcsQueryDiskInfoNtfReq cNty;
            pcOspSipMsg->GetMsgBody(cNty);
            TDiskInfoList &tDiskList = cNty.GetDiskInfoList();
            for(TDiskInfoList::iterator it = tDiskList.begin(); it != tDiskList.end(); ++it)
            {
                TPartInfoList &tPartList = it->GetPartList();
                for(TPartInfoList::iterator partIt = tPartList.begin(); partIt != tPartList.end();)
                {
                    if((partIt->GetPartUsage() != RCS_DISK_USAGE_DATA) && !partIt->GetIsReportByNru())
                    {
                        tPartList.erase(partIt);
                    }
                    else
                    {
                        ++partIt;
                    }
                }
            }
            pcOspSipMsg->SetMsgBody(cNty.ToXml().c_str(), cNty.ToXml().length() + 1);

            dwRet = OnWaitNotifyMsg<CRcsQueryDiskInfoNtfReq, CCuRcsQueryDiskInfoRsp>(pcMsg); 
        }
        break;
	case CAPS_QUERY_DEVICE_LIST_NTF_REQ:
		{
			dwRet = OnWaitNotifyMsg<CQueryDeviceListNtfReq, CCuQueryCAPSDeviceListRsp>(pcMsg); 
		}
		break;
    default:
        {
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"收到未知消息");
        }
        break;
    }

    if(PROCMSG_DEL == dwRet)
    {
        delete this;
    }

    return dwRet;
}

u32 CPeQueryTask::WaitNotifyTimerPoll()
{
    if ( GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout() )
    {
        TASKLOG(NORMAL_TASK, ERROR_LEV, "请求[%s--%d]超时，直接操作外设[%s]CU用户会话[%s]发送应答消息[%s--%d]\n",
            OspExtEventDesc((u16)m_pRsp->GetEventId() - 1).c_str(), m_pRsp->GetEventId() - 1,
            m_tPeUri.GetURIString().c_str(), m_strCuSession.c_str(), 
            OspExtEventDesc((u16)m_pRsp->GetEventId()).c_str(), m_pRsp->GetEventId() );

        CMS_ASSERT(NULL != m_ptRecvNode);
        m_pRsp->SetErrorCode(ERR_CUI_OPR_TIME_OUT);
        string strRsp = m_pRsp->ToXml();
        SendRspToCu(strRsp, m_ptRecvNode);
        NextState(NOT_QUERY_STATE);
        return TIMERPOLL_DEL;
    }
    return TIMERPOLL_DONE;
}

string CPeQueryTask::GetPeSession()
{
    CCuiInst *pcInst = (CCuiInst *)GetInstance();
    return pcInst->IsProvideServer(m_tPeUri, GetTaskNO());
}

void CPeQueryTask::WriteLog(TOperateType enOprType, u32 dwErrorCode, const string& strLogDesc, const string &strDevUri)
{
	CUsrOprLogTask* pcTask = new CUsrOprLogTask(GetInstance(), m_strCuSession, 
		string(DEV_TYPE_UAS) + "@" + m_tPeUri.GetDomain(),
		enOprType,
		dwErrorCode,
		strLogDesc,
		strDevUri
		);
	pcTask->InitStateMachine();
	pcTask->StartSendLog();
}

u32 CPeQueryTask::StartSendReq()
{
    u32 dwRet = PROCMSG_OK;

    CPeAgent *pcPeAgent = (CPeAgent *)g_cCuiDataMgr.GetPeAgentPtrByPeName(m_tPeUri.GetURIString(), GetInstance());
    CMS_ASSERT(NULL != pcPeAgent);

    //如果当前CPeAgent不在服务态，则直接删除该task
    if(CPeAgent::Service != pcPeAgent->GetState())
    {
        m_pRsp->SetErrorCode(ERR_CUI_PE_OFFLINE);
        string strRsp = m_pRsp->ToXml();
        SendRspToCu(strRsp, m_ptRecvNode);
        NextState(NOT_QUERY_STATE);

        return PROCMSG_DEL; 
    }

    //CMS_ASSERT(CPeAgent::Service == pcPeAgent->GetState());
    if( pcPeAgent )
    {
        m_strReq = ReplaceXmlKeyText(m_strReq, pcPeAgent->m_strSessionID, "<session>", "</session>");
    }

    dwRet = PostMsgReq(m_strReq, m_tPeUri);
    if(dwRet != PROCMSG_OK)
    {
        return dwRet;
    }
    NextState(WAIT_RESPONSE_STATE);

    return PROCMSG_OK;
}

// 订阅事务
// CPeSubscribeTask::CPeSubscribeTask(CInstExt *pcInst, const string &strCuSession, CSipTask *pcSubInterActTask, const string &strPeUri) : CCallBackTask(pcInst, strPeUri, strCuSession)
// {
//     CMS_ASSERT(!strCuSession.empty());
//     CMS_ASSERT(!strPeUri.empty());
//     CMS_ASSERT(NULL != pcSubInterActTask);
// 
//     m_tSubscribeID = INVALID_DIALOG_ID;
//     m_pcBatchTask = pcSubInterActTask;
//     m_strDevUri.clear();
// }
// 
// CPeSubscribeTask::~CPeSubscribeTask()
// {
//     TerminateTask();
// }
// 
// void CPeSubscribeTask::InitStateMachine()
// {
//     CStateProc cNotSubscribeProc;
//     cNotSubscribeProc.ProcMsg = (CSipTask::PFProcMsg)&CPeSubscribeTask::NotSubscribeProcMsg;
//     cNotSubscribeProc.TimerPoll = &CSipTask::TimerPollSkip;
//     cNotSubscribeProc.ProcErrMsg = (CSipTask::PFProcMsg)&CPeSubscribeTask::NotSubscribeProcMsg;
//     AddRuleProc(NOT_SUBSCRIBE_STATE, cNotSubscribeProc);
// 
//     CStateProc cSubscribeFinishedProc;
//     cSubscribeFinishedProc.ProcMsg = (CSipTask::PFProcMsg)&CPeSubscribeTask::SubscribeFinishedProcMsg;
//     cSubscribeFinishedProc.TimerPoll = &CSipTask::TimerPollSkip;
//     cSubscribeFinishedProc.ProcErrMsg = (CSipTask::PFProcMsg)&CPeSubscribeTask::SubscribeFinishedProcMsg;
//     AddRuleProc(SUBSCRIBE_FINISH_STATE, cSubscribeFinishedProc);
// 
//     CStateProc cWaitUnSubProc;
//     cWaitUnSubProc.ProcMsg = (CSipTask::PFProcMsg)&CPeSubscribeTask::WaitUnsubscribeRspProcMsg;
//     cWaitUnSubProc.TimerPoll = (CSipTask::PFTimerPoll)&CPeSubscribeTask::WaitUnsubscribeRspTimerPoll;
//     cWaitUnSubProc.ProcErrMsg = (CSipTask::PFProcMsg)&CPeSubscribeTask::WaitUnsubscribeRspProcMsg;
//     AddRuleProc(WAIT_UNSUB_STATE, cWaitUnSubProc);
// 
//     NextState(NOT_SUBSCRIBE_STATE);
// }
// 
// void CPeSubscribeTask::TerminateTask()
// {
//     if(m_tSubscribeID != INVALID_DIALOG_ID)
//     {
//         OspSipReleaseDlg(m_tSubscribeID);
//         m_tSubscribeID = INVALID_DIALOG_ID;
//     }
// 
//     if( !m_strDevUri.empty())
//     {
//         CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession, GetInstance());
//         if (NULL != pcCuAgent)
//         {
//             pcCuAgent->m_cPeSubscribeTaskSet.Erase(m_strDevUri);
//         }
//     }   
// 
//     CPeAgentBase *pcPeBaseAgent  = (CPeAgentBase *)g_cCuiDataMgr.GetPeAgentPtrByPeName(m_tPeUri.GetURIString(), GetInstance());
//     if(NULL != pcPeBaseAgent)
//     {
//         pcPeBaseAgent->m_cSubscribeTaskSet.Erase(m_strDevUri);
//     }
// 
//     m_strDevUri.clear();
//     m_pcBatchTask = NULL;
// }
// 
// u32 CPeSubscribeTask::NotSubscribeProcMsg( CMessage *const pcMsg )
// {
//     TASKLOG(NORMAL_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
//         OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
//     CMS_ASSERT(0&&"NotSubscribeProcMsg收到未知消息");
//     return PROCMSG_OK;
// }
// 
// s32 CPeSubscribeTask::StartSubscribePeStatus(const string& strDevUri)
// {
//     m_strDevUri = strDevUri;
// 
//     CTvWallStatusSsReq cReq;
//     //订阅的对象是cui
//     cReq.SetUserUri(g_cCuiConfig.GetLocalURI().GetURIString());
//     cReq.SetDevUri(strDevUri);
// 
// 
//     u32 dwRet = PROCMSG_OK;
//     CCuiInst *pcInst = (CCuiInst *)GetInstance();
//     m_strPeSession = pcInst->IsProvideServer(m_tPeUri, GetTaskNO());
// 
//     if(m_strPeSession.empty())
//     {
//         m_strReq = cReq.ToXml();
//     }
//     else
//     {
//         CPeAgentBase *pcPeBaseAgent = (CPeAgentBase *)g_cCuiDataMgr.GetPeAgentPtrByPeName(m_tPeUri.GetURIString(), GetInstance());
//         CMS_ASSERT(NULL != pcPeBaseAgent);
//         if(pcPeBaseAgent != NULL)
//         {
//             pcPeBaseAgent->m_cSubscribeTaskSet.Insert(m_strDevUri, GetTaskNO());
// 
//             cReq.SetSession(m_strPeSession);
//             dwRet = PostReq<CTvWallStatusSsReq>(KDSIP_EVENT_INVITE_REQ, cReq, m_tPeUri);
//             if(dwRet != PROCMSG_OK)
//             {
//                 return dwRet;
//             }
//             NextState(SUBSCRIBE_FINISH_STATE);
//         }
//         dwRet = PROCMSG_FAIL; 
//     }
// 
//     return dwRet;
// }

//////////////////////////////////////////////////////////////
CPeInviteTask::CPeInviteTask(CInstExt *pcInst, const string& strCuSession, CSipTask *pcInviteInterActTask, const string& strPeUri, CCuAgent *pcCuAgent, s32 nPlayId) : CCallBackTask(pcInst, strCuSession, strPeUri), m_cStrmNumRestorer(pcInst)
{
    CMS_ASSERT(!strCuSession.empty());
    CMS_ASSERT(!strPeUri.empty());
    CMS_ASSERT(NULL != pcInviteInterActTask);

    m_pcInviteInterActTask = pcInviteInterActTask;
    m_tInviteID = INVALID_DIALOG_ID;
    m_bByeWhileWaitInviteRsp = false;

	m_pcCuAgent = pcCuAgent;
	m_nPlayId = nPlayId;

    m_cStrmNumRestorer.SetUserLoginSession( pcCuAgent->m_strSession );
    m_dwErrorCode = CMS_SUCCESS;

}

CPeInviteTask::~CPeInviteTask()
{
    TerminateTask();
}

void CPeInviteTask::TerminateTask()
{
    Iterator pPos;
    int nSeqNo = 0;
    int nTaskNo = INVALID_TASKNO;
    while(!pPos.End())
    {
        if (m_cChildTaskList.Iterate(pPos, nSeqNo, nTaskNo) )
        {
            CTask* pcTask = NULL;
            pcTask = GetInstance()->FindTask(nTaskNo);
            if(pcTask != NULL)
            {
                delete pcTask;
                pcTask = NULL;
            }        
        }
    }
    if ( m_tInviteID != INVALID_DIALOG_ID )
    { //释放会话
		PostInDlgReq(KDSIP_EVENT_INVITE_BYE_REQ, CByeReq(), m_tInviteID);
        //OspSipReleaseDlg(m_tInviteID);
        m_tInviteID = INVALID_DIALOG_ID;
    }

    //注销CuAgent中的浏览事务
    if ( INVALID_PLAY_ID != m_tReq.GetDstChn().GetChnNO() )
    {
        //m_pcCuAgent->m_cInviteTaskSet.Erase(m_tCuChn.GetChnNO());
        CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession, GetInstance());
        //当调用CuAgent析构后，Invite等待bye响应时，CuAgent已经被干掉了，所以去掉CMS_ASSERT
        //CMS_ASSERT(pcCuAgent != NULL);
        if(NULL != pcCuAgent)
        {
            pcCuAgent->m_cPeInviteTaskSet.Erase(m_tReq.GetDstChn().GetChnNO());
        }

        CPeAgent *pcPeAgent = (CPeAgent *)g_cCuiDataMgr.GetPeAgentPtrByPeName(m_tPeUri.GetURIString(), GetInstance());
        if(NULL != pcPeAgent)
        {
            pcPeAgent->m_cInviteTaskSet.Erase(m_tReq.GetDstChn().GetChnNO());
        }

        m_tReq.GetDstChn().SetChnNO(INVALID_PLAY_ID);
    }

    m_pcInviteInterActTask = NULL;
}


bool CPeInviteTask::IsNotNeedPollState()
{
    if (GetState() == INVITE_HOLD_STATE)
    {
        return true;
    }

    return false;
}

void CPeInviteTask::InitStateMachine()
{
    CStateProc cNotReqState;
    cNotReqState.ProcMsg = (CSipTask::PFProcMsg)&CPeInviteTask::NotReqProcMsg;
    cNotReqState.TimerPoll = (CSipTask::PFTimerPoll)&CPeInviteTask::WaitSetupReqTimerPoll;
    cNotReqState.ProcErrMsg = (CSipTask::PFProcMsg)&CPeInviteTask::NotReqProcMsg;
    AddRuleProc(NOT_REQUEST_STATE, cNotReqState);

    CStateProc cWaitSetupRspState;
    cWaitSetupRspState.ProcMsg = (CSipTask::PFProcMsg)&CPeInviteTask::WaitSetupRspProcMsg;
    cWaitSetupRspState.TimerPoll = (CSipTask::PFTimerPoll)&CPeInviteTask::WaitSetupRspTimerPoll;
    cWaitSetupRspState.ProcErrMsg = (CSipTask::PFProcMsg)&CPeInviteTask::WaitSetupRspProcMsg;
    AddRuleProc(WAIT_SETUP_RSP_STATE, cWaitSetupRspState);

    CStateProc cWaitSetupAckState;
    cWaitSetupAckState.ProcMsg = (CSipTask::PFProcMsg)&CPeInviteTask::WaitSetupAckProcMsg;
    cWaitSetupAckState.TimerPoll = (CSipTask::PFTimerPoll)&CPeInviteTask::WaitAckTimerPoll;
    cWaitSetupAckState.ProcErrMsg = (CSipTask::PFProcMsg)&CPeInviteTask::WaitSetupAckProcMsg;
    AddRuleProc(WAIT_SETUP_ACK_STATE, cWaitSetupAckState);

    CStateProc cInviteHoldState;
    cInviteHoldState.ProcMsg = (CSipTask::PFProcMsg)&CPeInviteTask::InviteHoldProcMsg;
    cInviteHoldState.TimerPoll = &CSipTask::TimerPollSkip;
    cInviteHoldState.ProcErrMsg = (CSipTask::PFProcMsg)&CPeInviteTask::InviteHoldProcMsg;
    AddRuleProc(INVITE_HOLD_STATE, cInviteHoldState);

    CStateProc cWaitByeRspState;
    cWaitByeRspState.ProcMsg = (CSipTask::PFProcMsg)&CPeInviteTask::WaitByeRspProcMsg;
    cWaitByeRspState.TimerPoll = (CSipTask::PFTimerPoll)&CPeInviteTask::WaitByeRspTimerPoll;
    cWaitByeRspState.ProcErrMsg = (CSipTask::PFProcMsg)&CPeInviteTask::WaitByeRspProcMsg;
    AddRuleProc(WAIT_BYE_RSP_STATE, cWaitByeRspState);

    NextState(NOT_REQUEST_STATE);
}

u32 CPeInviteTask::NotReqProcMsg( CMessage *const pcMsg )
{
    TASKLOG(NORMAL_TASK, CRITICAL_LEV, "在[CInviteTask::NotReqProcMsg]收到未知消息[%s--%hu]，不处理\n",
        OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
    CMS_ASSERT(0&&"CInviteTask::NotReqProcMsg收到未知消息");
    return PROCMSG_OK;
}

TNetAddr CPeInviteTask::GetSendBindAddr() const
{
	TNetAddr tAddr;
	if (m_pcCuAgent->m_ePlatNat == NATModeInner && g_cCuiConfig.IsEnableMapedNatAddr())     //平台在NAT内
	{
		//当含有二层NAT时，平台不能同时把一个端口（8001）映射到第一层和第二层
		//这里直接使用cu的登陆ip，即为平台在的最外层映射IP
		//tAddr.SetNetIp(g_cCuiConfig.GetMapedNatIP());
		tAddr.SetNetIp(m_pcCuAgent->m_strHostIp);
		tAddr.SetNetPort(g_cCuiConfig.GetMapedNatPort());
	}
	else                                             //平台在NAT外
	{
		tAddr.SetNetIp(m_pcCuAgent->m_tNatInfo.GetIpAddr());
		tAddr.SetNetPort(g_cCuiConfig.GetNatPort());
	}  

	TASKLOG(NORMAL_TASK, EVENT_LEV, "获取到的伪装地址[%s-%d]\n",tAddr.GetNetIp().c_str(),tAddr.GetNetPort());
	return tAddr;
}

void CPeInviteTask::SwitchToNatAddress(TSdp &tSdp)
{
	TMediaDescList& tMediaDescList = tSdp.GetMediaDescList();

	for(TMediaDescList::iterator itrMediaDesc = tMediaDescList.begin(); 
		itrMediaDesc!=tMediaDescList.end(); itrMediaDesc++)
	{
		//if (itrMediaDesc->GetMediaType() == MEDIA_TYPE_VIDEO)
		//{
		TMediaTransChannelList& tMediaTransChnList = itrMediaDesc->GetMediaTransChnList();
		for(TMediaTransChannelList::iterator itrTransChn = tMediaTransChnList.begin(); 
			itrTransChn!=tMediaTransChnList.end(); itrTransChn++)
		{
			if (itrTransChn->GetTransChnType() == MEDIA_TRANS_CHN_RTP)
			{
				TNetAddrList& tNatAddrList = itrTransChn->GetAddrList();
				for (TNetAddrList::iterator itrNatAddr = tNatAddrList.begin();
					itrNatAddr != tNatAddrList.end();itrNatAddr++)
				{
					ChangeToNatAddr(m_pcCuAgent->m_strSession,*itrNatAddr,m_nPlayId);
				}

				//交换需要地址伪装为NAT探测包地址
				itrTransChn->SetSendBindAddr(GetSendBindAddr());
			}
			else if (itrTransChn->GetTransChnType() == MEDIA_TRANS_CHN_RTCP)
			{
				TNetAddrList& tNatAddrList = itrTransChn->GetAddrList();
				for (TNetAddrList::iterator itrNatAddr = tNatAddrList.begin();
					itrNatAddr != tNatAddrList.end();itrNatAddr++)
				{
					ChangeToNatAddr(m_pcCuAgent->m_strSession,*itrNatAddr,m_nPlayId);
				}
			}
			else
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "不支持的媒体类型[%s]\n",
					itrTransChn->GetTransChnType().c_str());
			}
		}
		//}
	}
}

u32 CPeInviteTask::StartSendInviteReq(CInviteReq &cReq)
{
	m_tReq = cReq;//2018/8/22 zsy
    if( !m_cStrmNumRestorer.Start() )
    {
        TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户[%s]会话[%s] 已达到最大并发数，本次请求被拒绝\n",
            m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str() );
        
        m_dwErrorCode = ERR_CUI_USER_EXCEEDED_MAX_STREAM;
        return PROCMSG_FAIL;
    }

    CMS_ASSERT( NOT_REQUEST_STATE == GetState() );

    TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户会话[%s]发送Invite请求，建立源通道[%s--%d]到目的通道[%s--%d]的交换\n",
        m_strCuSession.c_str(),
        cReq.GetSrcChn().GetDevUri().c_str(), cReq.GetSrcChn().GetChnNO(),
        cReq.GetDstChn().GetDevUri().c_str(), cReq.GetDstChn().GetChnNO());

    //     m_tCuChn = cReq.GetDstChn();
    //     m_tDevChn = cReq.GetSrcChn();

    u32 dwRet = PROCMSG_OK;
    CCuiInst *pcInst = (CCuiInst *)GetInstance();
    m_strPeSession = pcInst->IsProvideServer(m_tPeUri, GetTaskNO());

    //m_tReq = cReq;//2018/8/22 zsy 需要提前到第一个可能的return 之前，否则前面return了，后面销毁本对象的时候就无法释放invite了。
    //m_strReq = cReq.ToXml();

    TASKLOG( EVENT_LEV," cu在NAT內，更换碼流地址為其NAT外地址\n");
	if (m_pcCuAgent->m_eClientNat == NATModeInner) 
	{
		SwitchToNatAddress(m_tReq.GetSdp());
	}

    if(!m_strPeSession.empty())
    {
        //         m_strReq = cReq.ToXml();
        //     }
        //     else
        //     {
        CPeAgentBase *pcPeBaseAgent = (CPeAgentBase *)g_cCuiDataMgr.GetPeAgentPtrByPeName(m_tPeUri.GetURIString(), GetInstance());
        CMS_ASSERT(NULL != pcPeBaseAgent);
        if(pcPeBaseAgent != NULL)
        {
            pcPeBaseAgent->m_cInviteTaskSet.Insert(m_tReq.GetDstChn().GetChnNO(), GetTaskNO());

            m_tReq.SetSession(m_strPeSession);
            dwRet = PostReq<CInviteReq>(KDSIP_EVENT_INVITE_REQ, m_tReq, m_tPeUri);
            if(dwRet != PROCMSG_OK)
            {
                TASKLOG( ERROR_LEV,"发送invite 到 [%s] 失败 \n",m_tPeUri.GetURIString().c_str());
                return dwRet;
            }
            NextState(WAIT_SETUP_RSP_STATE);
        }
        //dwRet = PROCMSG_FAIL; 
    }

    return dwRet;
}

u32 CPeInviteTask::StartSendInviteAck()
{ 
    CMS_ASSERT(WAIT_SETUP_ACK_STATE == GetState());
    CMS_ASSERT(INVALID_DIALOG_ID != m_tInviteID);
    TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户会话[%s]通过TaskNO[%u]发送InviteACK请求, Sip DialogID为 %u\n",
        m_strCuSession.c_str(), GetTaskNO(), m_tInviteID);

    CInviteACK cAck;
    cAck.SetSession(m_strPeSession);

#ifdef _UNIT_TEST_
    g_cTaskNOQue.push(GetTaskNO());
    cout << "Post InviteAck To " << m_pcCuAgent->m_tCmuURI.GetURIString().c_str() << endl;
    cout << cAck.ToXml().c_str() << endl;
#else
    u32 dwRet = PROCMSG_OK;
    dwRet = PostInDlgReq<CInviteACK>(KDSIP_EVENT_INVITE_ACK, cAck, m_tInviteID);
#endif

    NextState(INVITE_HOLD_STATE);
    return dwRet;
}

u32 CPeInviteTask::StartSendInviteBye()
{
    u32 dwRet = PROCMSG_OK;
    if ( m_tInviteID != INVALID_DIALOG_ID )
    {
        TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户会话[%s]通过TaskNO[%u]发送InviteBye请求, Sip DialogID为 %u\n",
            m_strCuSession.c_str(), GetTaskNO(), m_tInviteID);

        CByeReq cReq;
        cReq.SetReason( ERR_CMU_SWITCH_STOP_BY_BYE );
        dwRet = PostInDlgReq(KDSIP_EVENT_INVITE_BYE_REQ, cReq, m_tInviteID);
    }

    NextState(WAIT_BYE_RSP_STATE);
    //BYE Rsp由对端SIP协议栈直接回复，等待回复后delete task
    return dwRet;
}


bool CPeInviteTask::TryBye( CSipTask *pcActTask )
{
    if (GetState() != WAIT_SETUP_RSP_STATE)
    {
        m_pcInviteInterActTask = pcActTask;
        StartSendInviteBye();
        return true;
    }
    else
    {
        m_bByeWhileWaitInviteRsp = true;
        return false;
    }
}


s32 CPeInviteTask::StartVcrCtrl(const CCuVcrCtrlReq& cCuReq)
{
    if( m_tInviteID != INVALID_DIALOG_ID)
    {
        TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户会话[%s]通过TaskNO[%u]发送VcrCtrl请求, Sip DialogID为 %u\n",
            m_strCuSession.c_str(), GetTaskNO(), m_tInviteID);

        CVcrCtrlReq cReq;
        cReq.SetSeqNum(cCuReq.GetSeqNum());
        cReq.SetSession(m_strPeSession);
        cReq.SetPlaybackSessUri(m_tRsp.GetSdp().GetPlaybackSessUri());
        cReq.SetPlayRate(cCuReq.GetPlayRate());
        cReq.SetSeekTime(cCuReq.GetSeekTime());
        cReq.SetVcrCmd(cCuReq.GetVcrCmd());
        PostInDlgReq(KDSIP_EVENT_INFO_REQ, cReq, m_tInviteID);
    }

    return PROCMSG_OK;
}
u32 CPeInviteTask::StartTask()
{
    u32 dwRet = PROCMSG_FAIL;
    if( m_tInviteID != INVALID_DIALOG_ID)
    {
        TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户会话[%s]通过TaskNO[%u]发送StartTask请求, Sip DialogID为 %u\n",
            m_strCuSession.c_str(), GetTaskNO(), m_tInviteID);
        CRcsTaskStartReq cReq;
        cReq.SetSession(m_strPeSession);
        
        dwRet = PostInDlgReq(KDSIP_EVENT_INFO_REQ, cReq, m_tInviteID);
        NextState(INVITE_HOLD_STATE);
    }
    return dwRet;
}
CPlaybackProgressNtfReq &CPeInviteTask::GetPlayBackProgressNtf()
{
    return m_tNtfReq;
}
const CPlaybackProgressNtfReq &CPeInviteTask::GetPlayBackProgressNtf() const
{
    return m_tNtfReq;
}
void CPeInviteTask::AddChildTask(int nSeqNo, int nTaskNo)
{
    if(nTaskNo == INVALID_TASKNO)
    {
        TASKLOG(ERROR_LEV, "TaskNo is Invalid");
        return;
    }
    m_cChildTaskList.Insert(nSeqNo, nTaskNo);
}
void CPeInviteTask::DelChildTask(int nSeqNo)
{
    m_cChildTaskList.Erase(nSeqNo);
}
CTask* CPeInviteTask::FindChildTask(int nSeqNo)
{
    int nTaskNo = INVALID_TASKNO;
    m_cChildTaskList.Find(nSeqNo, nTaskNo);
    return GetInstance()->FindTask(nTaskNo);
}

void CPeInviteTask::NotifyStop2Cu(s32 dwErrorCode)
{
    CInviteStopNotify cInviteStopNtf;
    cInviteStopNtf.playId = m_tReq.GetDstChn().GetChnNO();
    cInviteStopNtf.errorCode = dwErrorCode;

    CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession,GetInstance());
    CMS_ASSERT(pcCuAgent != NULL);
    if(pcCuAgent != NULL)
    {
        pcCuAgent->m_cNotifyPool.InsertInviteStopNotify(cInviteStopNtf);
    }
}

TChannel& CPeInviteTask::GetDevChn()
{
    return m_tReq.GetSrcChn();
}

u32 CPeInviteTask::WaitSetupReqTimerPoll()
{
    if ( GetCurStateHoldTime() > g_cCuiConfig.GetDlgWaitRspTimeout() )
    {
        TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户会话[%s]等待外设模块建链超时，销毁CInviteTask[TaskNO: %u]\n",
            m_strCuSession.c_str(), GetTaskNO());

        /*
        这里不用通知CInviteInterAction操作超时
        只是取消m_pcCuAgent->m_cInviteTaskSet中的登记
        等CInviteInterAction消息自己超时好了

        因为此时还未建立会话，所以也无需发送Bye，以及ReleaseDialog
        */
        return TIMERPOLL_DEL;
    }
    return TIMERPOLL_DONE;
}

u32 CPeInviteTask::WaitSetupRspTimerPoll()
{
    if ( GetCurStateHoldTime() > g_cCuiConfig.GetDlgWaitRspTimeout() )
    {
        TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户会话[%s]等待InviteRsp超时，销毁CInviteTask[TaskNO: %u]\n",
            m_strCuSession.c_str(), GetTaskNO());

        /*
        这里不用通知CInviteInterAction操作超时
        只是取消m_pcCuAgent->m_cInviteTaskSet中的登记
        等CInviteInterAction消息自己超时好了

        因为此时还未建立会话，所以也无需发送Bye，以及ReleaseDialog
        */
        return TIMERPOLL_DEL;
    }
    return TIMERPOLL_DONE;
}

u32 CPeInviteTask::WaitByeRspTimerPoll()
{
    if ( GetCurStateHoldTime() > g_cCuiConfig.GetDlgWaitRspTimeout())
    {
        TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户会话[%s]等待Bye Rsp超时，结束Dialog\n",
            m_strCuSession.c_str());
        m_tInviteID = INVALID_DIALOG_ID;
        return TIMERPOLL_DEL;
    }
    return TIMERPOLL_DONE;
}

u32 CPeInviteTask::StartSendReq()
{
    u32 dwRet = PROCMSG_OK;

    CPeAgent *pcPeAgent = (CPeAgent *)g_cCuiDataMgr.GetPeAgentPtrByPeName(m_tPeUri.GetURIString(), GetInstance());
    CMS_ASSERT(NULL != pcPeAgent);

    if(pcPeAgent->GetState() != CPeAgent::Service)
    {
        if(m_pcInviteInterActTask != NULL)
        {
            CPeInviteInterAction *pcInviteInterActTask= (CPeInviteInterAction *)m_pcInviteInterActTask;
            pcInviteInterActTask->PostErrMsg2Cu(ERR_CUI_PE_OFFLINE);
            delete pcInviteInterActTask;
            pcInviteInterActTask = NULL;

            return PROCMSG_DEL;
        }
    }


    if( pcPeAgent )
    {
        pcPeAgent->m_cInviteTaskSet.Insert(m_tReq.GetDstChn().GetChnNO(), GetTaskNO());
        m_strPeSession = pcPeAgent->m_strSessionID;
        //m_strReq = ReplaceXmlKeyText(m_strReq, m_strPeSession, "<session>", "</session>");
        m_tReq.SetSession(m_strPeSession);

        dwRet = PostReq(KDSIP_EVENT_INVITE_REQ, m_tReq, m_tPeUri);
        //dwRet = PostMsgReq(m_strReq, m_tPeUri);
        if(dwRet != PROCMSG_OK)
        {
            TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户会话[%s]发送[INVITE_REQ]消息失败\n",
                m_strCuSession.c_str());
            return dwRet;
        }
        NextState(WAIT_SETUP_RSP_STATE);
    }  

    return dwRet;
}

u32 CPeInviteTask::WaitSetupRspProcMsg(CMessage *const pcMsg)
{
    COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
    TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户会话[%s]收到业务应答消息[%s--%d] (SIP应答消息[%s--%d])\n",
        m_strCuSession.c_str(),
        OspExtEventDesc(pcOspSipMsg->GetOspEventID()).c_str(), pcOspSipMsg->GetOspEventID(),
        pcOspSipMsg->GetSipEventStr().c_str(), pcOspSipMsg->GetSipEventID());

    u32 dwRet = PROCMSG_OK;

    switch ( pcMsg->event )
    {
    case OSP_SIP_BYE_REQ:   //其实这里不可能会收到Bye，因为呼叫还未建立，要么成功要么拒绝，收到403错误码
        {
            CMS_ASSERT(0&&"不可能在WaitSetupRsp状态下收到OSP_SIP_BYE_REQ消息");
        }
        break;
    case OSP_SIP_MSG_PROC_FAIL:
    case OSP_SIP_DIALOG_TERMINATE:
        {
            //将消息回调给InviteInterAckTask类
            if ( m_pcInviteInterActTask != NULL )
            {
                dwRet = m_pcInviteInterActTask->ProcMsg(pcMsg);
                if(PROCMSG_DEL ==  dwRet)
                {
                    delete m_pcInviteInterActTask;
                    m_pcInviteInterActTask = NULL;
                }
            }
            TASKLOG(NORMAL_TASK, ERROR_LEV, "收到InviteRsp失败消息，销毁CInviteTask[TaskNO: %u]\n",
                GetTaskNO());
            m_tInviteID = INVALID_DIALOG_ID;
            NextState(NOT_REQUEST_STATE);

            delete this;
            dwRet = PROCMSG_DEL;
        }
        break;
    case INVITE_RSP:
        {
            //判断是否业务层失败
            CInviteRsp cRsp;
            pcOspSipMsg->GetMsgBody(cRsp);

            //保存DlgID
            m_tInviteID = pcOspSipMsg->GetSipDlgID();
            m_tRsp = cRsp;

            if (cRsp.GetErrorCode() == CMS_SUCCESS && m_bByeWhileWaitInviteRsp)
            {
                cRsp.SetErrorCode(ERR_CUI_DIALOG_TERMINATED_BY_CMU);
            }

            //将消息回调给InviteInterAckTask类
            if ( m_pcInviteInterActTask != NULL )
            {
                dwRet = m_pcInviteInterActTask->ProcMsg(pcMsg);
                if(PROCMSG_DEL == dwRet)
                {
                    delete m_pcInviteInterActTask;
                    m_pcInviteInterActTask = NULL;
                }

				dwRet = PROCMSG_OK;//InviteInterAckTask处理后恢复正常返回值
            }

            if ( INVALID_DIALOG_ID == m_tInviteID )
            {
                TASKLOG(NORMAL_TASK, ERROR_LEV, "收到的InviteRsp含带DialogID为 0 !!!!直接销毁CInviteTask[TaskNO %u]\n",
                    GetTaskNO());
                m_tInviteID = INVALID_DIALOG_ID;
                NextState(NOT_REQUEST_STATE);
                dwRet = PROCMSG_DEL;
                delete this;
                break;
            }

            if ( CMS_SUCCESS != cRsp.GetErrorCode() )
            {
                TASKLOG(NORMAL_TASK, ERROR_LEV, "Invite请求失败，错误码[%d]，销毁CInviteTask[TaskNO: %u]，结束Sip Dialog: [%u]\n",
                    cRsp.GetErrorCode(), GetTaskNO(), m_tInviteID);

                //dwRet = StartSendInviteBye();
                //OspSipReleaseDlg(m_tInviteID); 发送了Bye就不需要再主动释放
                //等待Bye应答之后再销毁本Task
                delete this;
                return PROCMSG_DEL;
            }
            else
            {
                CCMSTime cStartTime(m_tReq.GetSdp().GetTimeRange().GetStartTime());
                CCMSTime cEndTime(m_tReq.GetSdp().GetTimeRange().GetEndTime());

                m_tNtfReq.SetCurPlayTime((u32)cStartTime.GetTime());
                m_tNtfReq.SetStartPlayTime((u32)cStartTime.GetTime());
                if(cEndTime.GetTime() >= cStartTime.GetTime())
                {
                    m_tNtfReq.SetTotalPlayTime((u32)(cEndTime.GetTime() - cStartTime.GetTime()));
                }
                else
                {
                    m_tNtfReq.SetTotalPlayTime(0);
                }

                m_tNtfReq.SetIsPlayFinished(false);

                TASKLOG(NORMAL_TASK, EVENT_LEV, "Invite请求成功，将CInviteTask[TaskNO: %u, PlayId: %d]登记到CCuAgent的任务列表中\n",
                    GetTaskNO(), m_tReq.GetDstChn().GetChnNO());
                NextState(WAIT_SETUP_ACK_STATE);
                StartSendInviteAck();
            }
        }
        break;
    default:
        {
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"收到未知消息");
        }
        break;
    }
    return dwRet;
}

u32 CPeInviteTask::WaitSetupAckProcMsg( CMessage *const pcMsg )
{
    COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
    TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户会话[%s]收到业务应答消息[%s--%d] (SIP应答消息[%s--%d])\n",
        m_strCuSession.c_str(),
        OspExtEventDesc(pcOspSipMsg->GetOspEventID()).c_str(), pcOspSipMsg->GetOspEventID(),
        pcOspSipMsg->GetSipEventStr().c_str(), pcOspSipMsg->GetSipEventID());

    switch ( pcMsg->event )
    {
    case OSP_SIP_BYE_REQ:
        {
            TASKLOG(NORMAL_TASK, EVENT_LEV, "CInviteTask[TaskNO %u]收到InviteBye Req，DialogID[%u]，自动回复应答\n",
                GetTaskNO(), pcOspSipMsg->GetSipDlgID() );
            //将消息回调给InviteInterAckTask类
            if ( m_pcInviteInterActTask != NULL )
            {
                u32 dwRet = PROCMSG_OK;
                dwRet = m_pcInviteInterActTask->ProcMsg(pcMsg);
                if(PROCMSG_DEL == dwRet)
                {
                    delete m_pcInviteInterActTask;
                    m_pcInviteInterActTask = NULL;
                }
            }

            //播放任务没有成功建立至少cu认为没有建立
            if(pcOspSipMsg->GetMsgBodyLen() >0)
            {
                CByeReq cByeReq;
                pcOspSipMsg->GetMsgBody(cByeReq);
                NotifyStop2Cu(cByeReq.GetReason());
            }
            else
            {
                NotifyStop2Cu(ERR_CUI_DIALOG_TERMINATED);
            }

            string strNullMsg;
            PostRsp(KDSIP_EVENT_INVITE_BYE_RSP, pcOspSipMsg->GetSipTransID(), strNullMsg);
            m_tInviteID = INVALID_DIALOG_ID;

            NextState(NOT_REQUEST_STATE);
            delete this;
            return PROCMSG_DEL;
        }
        break;
    case OSP_SIP_MSG_PROC_FAIL:
    case OSP_SIP_DIALOG_TERMINATE:
        {
            //将消息回调给InviteInterAckTask类
            if ( m_pcInviteInterActTask != NULL )
            {
                u32 dwRet = m_pcInviteInterActTask->ProcMsg(pcMsg);
                if(PROCMSG_DEL == dwRet)
                {
                    delete m_pcInviteInterActTask;
                    m_pcInviteInterActTask = NULL;
                }
            }
            TASKLOG(NORMAL_TASK, ERROR_LEV, "销毁CInviteTask[TaskNO: %u]，结束Sip Dialog: [%u]\n",
                GetTaskNO(), m_tInviteID);
            delete this;
            return PROCMSG_DEL;
        }
        break;
    default:
        {
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"收到未知消息");
        }
        break;
    }
    return PROCMSG_OK;
}

u32 CPeInviteTask::WaitAckTimerPoll()
{
    if ( GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout() * 2 )
    {
        TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户[%s]会话等待InviteAck超时，直接发送InviteBye结束Dlg\n",
            m_strCuSession.c_str());
        StartSendInviteBye();
        //return TIMERPOLL_DEL;
        return TIMERPOLL_DONE;  //发送ByeReq之后切换到WaitByeRsp状态了，不用删除
    }
    return TIMERPOLL_DONE;
}

u32 CPeInviteTask::InviteHoldProcMsg( CMessage *const pcMsg )
{
    COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
    TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户会话[%s]收到业务应答消息[%s--%d] (SIP应答消息[%s--%d])\n",
        m_strCuSession.c_str(),
        OspExtEventDesc(pcOspSipMsg->GetOspEventID()).c_str(), pcOspSipMsg->GetOspEventID(),
        pcOspSipMsg->GetSipEventStr().c_str(), pcOspSipMsg->GetSipEventID());

    u32 dwRet = PROCMSG_OK;
    switch ( pcMsg->event )
    {
    case OSP_SIP_BYE_REQ:
        {
            TASKLOG(NORMAL_TASK, EVENT_LEV, "CInviteTask[TaskNO %u]收到InviteBye Req，DialogID[%u]，自动回复应答\n",
                GetTaskNO(), pcOspSipMsg->GetSipDlgID() );
            //将消息回调给InviteInterAckTask类
            if ( m_pcInviteInterActTask != NULL )
            {
                u32 dwRet = m_pcInviteInterActTask->ProcMsg(pcMsg);
                if(PROCMSG_DEL == dwRet)
                {
                    delete m_pcInviteInterActTask;
                    m_pcInviteInterActTask = NULL;
                }
            }

            if(pcOspSipMsg->GetMsgBodyLen() > 0)
            {
                CByeReq cByeReq;
                pcOspSipMsg->GetMsgBody(cByeReq);
                NotifyStop2Cu(cByeReq.GetReason());
            }
            else
            {
                NotifyStop2Cu(ERR_CUI_DIALOG_TERMINATED);
            }

            CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession,GetInstance());
            CMS_ASSERT(pcCuAgent != NULL);
            if(pcCuAgent != NULL)
            {
                string strNullMsg;
                PostRsp(KDSIP_EVENT_INVITE_BYE_RSP, pcOspSipMsg->GetSipTransID(), strNullMsg);
            }

            NextState(NOT_REQUEST_STATE);
            delete this;
            dwRet =  PROCMSG_DEL;
        }
        break;
    case OSP_SIP_MSG_PROC_FAIL:
    case OSP_SIP_DIALOG_TERMINATE:
        {
            //将消息回调给InviteInterAckTask类
            if ( m_pcInviteInterActTask != NULL )
            {
                u32 dwRet = m_pcInviteInterActTask->ProcMsg(pcMsg);
                if(dwRet == PROCMSG_DEL)
                {
                    delete m_pcInviteInterActTask;
                    m_pcInviteInterActTask = NULL;
                } 
            }
            if (OSP_SIP_MSG_PROC_FAIL == pcMsg->event)
            {
                TASKLOG(NORMAL_TASK, ERROR_LEV, "recv OSP_SIP_MSG_PROC_FAIL, sipErrorCode=%d, invite req:%s\n", 
                    pcOspSipMsg->GetSipErrorCode(), m_tReq.ToXml().c_str());
                StartSendInviteBye();
            }

            TASKLOG(NORMAL_TASK, ERROR_LEV, "销毁CInviteTask[TaskNO: %u]，结束Sip Dialog: [%u]\n",
                GetTaskNO(), m_tInviteID);
            NextState(NOT_REQUEST_STATE);
            delete this;
            dwRet =  PROCMSG_DEL;
        }
        break;
    case END_OF_FILE_NTF_REQ:
        {
            CEndOfFileNtfReq cReq;
            pcOspSipMsg->GetMsgBody(cReq);

            CEndOfFileNtfRsp cRsp;
            cRsp.SetHeadFromReq(cReq);

            PostRsp(KDSIP_EVENT_MESSAGE_DIALOG_RSP, pcOspSipMsg->GetSipTransID(), cRsp);

            NotifyStop2Cu(ERR_CUI_RECORD_END);

            StartSendInviteBye();
        }
        break;
    case PLAYBACK_PROGRESS_QUERY_RSP:
        {
            TASKLOG(NORMAL_TASK, EVENT_LEV, "CInviteTask[TaskNO %u]收到QueryProcess Rsp，DialogID[%u]，自动回复应答\n",
                GetTaskNO(), pcOspSipMsg->GetSipDlgID() );

            //将消息回调给InviteInterAckTask类
            if ( m_pcInviteInterActTask != NULL )
            {
                m_pcInviteInterActTask->ProcMsg(pcMsg);
            }
        }
        break;
    case PLAYBACK_PROGRESS_NTF_REQ:
        {
            TASKLOG(NORMAL_TASK, EVENT_LEV, "CInviteTask[TaskNO %u]收到PlayBack Process Ntf Req，DialogID[%u]，自动回复应答\n",
                GetTaskNO(), pcOspSipMsg->GetSipDlgID() );
            CPlaybackProgressNtfReq cReq;
            pcOspSipMsg->GetMsgBody(cReq);
            CPlaybackProgressNtfRsp cRsp;
            cRsp.SetHeadFromReq(cReq);
            PostRsp(KDSIP_EVENT_MESSAGE_DIALOG_RSP, pcOspSipMsg->GetSipTransID(), cRsp);
            m_tNtfReq = cReq;
            /*CCMSTime cStartTime(m_tReq.GetSdp().GetTimeRange().GetStartTime());
            m_tNtfReq.SetStartPlayTime((u32)cStartTime.GetTime());*/
        }
        break;
    case VCR_CTRL_RSP:
        {
            TASKLOG(NORMAL_TASK, EVENT_LEV, "CInviteTask[TaskNO %u]收到VCR Ctrl Req，DialogID[%u]，自动回复应答\n",
                GetTaskNO(), pcOspSipMsg->GetSipDlgID() );
            CTask* pcTask =FindChildTask(pcOspSipMsg->GetOspSeqNO());
            if(pcTask != NULL)
            {
                pcTask->ProcMsg(pcMsg);
            }         
        }
        break;
    case RCS_TASK_START_RSP:
        {
            TASKLOG(NORMAL_TASK, EVENT_LEV, "CInviteTask[TaskNO %u]收到TaskStart Rsp，DialogID[%u]，自动回复应答\n",
                GetTaskNO(), pcOspSipMsg->GetSipDlgID() );
            if ( m_pcInviteInterActTask != NULL )
            {
                m_pcInviteInterActTask->ProcMsg(pcMsg);
            }
        }
        break;
    default:
        {
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"收到未知消息");
            dwRet = PROCMSG_FAIL;
        }
        break;
    }
    return dwRet;
}

u32 CPeInviteTask::WaitByeRspProcMsg(CMessage *const pcMsg)
{
    COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
    TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户会话[%s]收到业务应答消息[%s--%d]\n",
        m_strCuSession.c_str(),
        pcOspSipMsg->GetSipEventStr().c_str(), pcOspSipMsg->GetSipEventID());

    u32 dwRet = PROCMSG_OK;
    switch ( pcMsg->event )
    {
    case OSP_SIP_BYE_REQ:   
        {
            TASKLOG(NORMAL_TASK, EVENT_LEV, "CInviteTask[TaskNO %u]收到InviteBye Req，DialogID[%u]，回复应答后销毁CInviteTask\n",
                GetTaskNO(), pcOspSipMsg->GetSipDlgID() );
            //将消息回调给InviteInterAckTask类
            if ( m_pcInviteInterActTask != NULL )
            {
                u32 dwRet = m_pcInviteInterActTask->ProcMsg(pcMsg);
                if(PROCMSG_DEL == dwRet)
                {
                    delete m_pcInviteInterActTask;
                    m_pcInviteInterActTask = NULL;
                }
            }

            string strNullMsg;
            PostRsp(KDSIP_EVENT_INVITE_BYE_RSP, pcOspSipMsg->GetSipTransID(), strNullMsg);
            //已经删除了OspSip的Dialog绑定，可以直接删除task
            m_tInviteID = INVALID_DIALOG_ID;
            NextState(NOT_REQUEST_STATE);
            delete this;
            dwRet = PROCMSG_DEL;
        }
        break;
    case OSP_SIP_BYE_RSP:
        {
            if ( m_pcInviteInterActTask != NULL )
            {
                u32 dwRet = m_pcInviteInterActTask->ProcMsg(pcMsg);
                if(PROCMSG_DEL == dwRet)
                {
                    delete m_pcInviteInterActTask;
                    m_pcInviteInterActTask = NULL;
                }
            }

            TASKLOG(NORMAL_TASK, EVENT_LEV, "销毁CInviteTask[TaskNO: %u]，结束Sip Dialog: [%u]\n",
                GetTaskNO(), m_tInviteID);
            m_tInviteID = INVALID_DIALOG_ID;
            NextState(NOT_REQUEST_STATE);
            delete this;
            dwRet = PROCMSG_DEL;
        }
        break;
    case OSP_SIP_MSG_PROC_FAIL:
    case OSP_SIP_DIALOG_TERMINATE:
        {
            //将消息回调给InviteInterAckTask类
            if ( m_pcInviteInterActTask != NULL )
            {
                u32 dwRet = m_pcInviteInterActTask->ProcMsg(pcMsg);
                if(PROCMSG_DEL == dwRet)
                {
                    delete m_pcInviteInterActTask;
                    m_pcInviteInterActTask = NULL;
                }
            }
            TASKLOG(NORMAL_TASK, ERROR_LEV, "销毁CInviteTask[TaskNO: %u]，结束Sip Dialog: [%u]\n",
                GetTaskNO(), m_tInviteID);
            //m_tInviteID = INVALID_DIALOG_ID;
            NextState(NOT_REQUEST_STATE);
            delete this;
            dwRet = PROCMSG_DEL;
        }
        break;
    case END_OF_FILE_NTF_REQ:
        {
            CEndOfFileNtfReq cReq;
            pcOspSipMsg->GetMsgBody(cReq);

            CEndOfFileNtfRsp cRsp;
            cRsp.SetHeadFromReq(cReq);

            PostRsp(KDSIP_EVENT_MESSAGE_DIALOG_RSP, pcOspSipMsg->GetSipTransID(), cRsp);

            TASKLOG(NORMAL_TASK, ERROR_LEV, "在CInviteTask[TaskNO: %u][WaitByeRspProcMsg]收到消息[%s--%hu]\n",
                GetTaskNO(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    case PLAYBACK_PROGRESS_QUERY_RSP:
        {
            TASKLOG(NORMAL_TASK, EVENT_LEV, "CInviteTask[TaskNO %u]收到QueryProcess Rsp，DialogID[%u]，自动回复应答\n",
                GetTaskNO(), pcOspSipMsg->GetSipDlgID() );

            //将消息回调给InviteInterAckTask类
            if ( m_pcInviteInterActTask != NULL )
            {
                m_pcInviteInterActTask->ProcMsg(pcMsg);
            }
        }
        break;
    case PLAYBACK_PROGRESS_NTF_REQ:
        {
            TASKLOG(NORMAL_TASK, EVENT_LEV, "CInviteTask[TaskNO %u]收到PlayBack Process Ntf Req，DialogID[%u]，自动回复应答\n",
                GetTaskNO(), pcOspSipMsg->GetSipDlgID() );
            CPlaybackProgressNtfReq cReq;
            pcOspSipMsg->GetMsgBody(cReq);
            CPlaybackProgressNtfRsp cRsp;
            cRsp.SetHeadFromReq(cReq);
            PostRsp(KDSIP_EVENT_MESSAGE_DIALOG_RSP, pcOspSipMsg->GetSipTransID(), cRsp);
            m_tNtfReq = cReq;
            TASKLOG(NORMAL_TASK, EVENT_LEV, "PlayBack Process Ntf Req--CurTime[%d], TotalTime[%d]\n",
                cReq.GetCurPlayTime(), cReq.GetTotalPlayTime());
            CCMSTime cStartTime(m_tReq.GetSdp().GetTimeRange().GetStartTime());
            m_tNtfReq.SetStartPlayTime((u32)cStartTime.GetTime());
        }
        break;
    case VCR_CTRL_RSP:
        {
            TASKLOG(NORMAL_TASK, EVENT_LEV, "CInviteTask[TaskNO %u]收到VCR Ctrl Req，DialogID[%u]，自动回复应答\n",
                GetTaskNO(), pcOspSipMsg->GetSipDlgID() );
            CTask* pcTask =FindChildTask(pcOspSipMsg->GetOspSeqNO());
            if(pcTask != NULL)
            {
                pcTask->ProcMsg(pcMsg);
            }  
        }
        break;
    default:
        {
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"收到未知消息");
            dwRet = PROCMSG_FAIL;
        }
        break;
    }
    return dwRet;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CPeInviteInterAction::CPeInviteInterAction(CInstExt *pcInst, const string &strCuSession, PTRecvNode ptRecvNode) : CSipTask(pcInst)
{
    CMS_ASSERT(!strCuSession.empty());
    CMS_ASSERT(ptRecvNode != NULL);

    m_ptRecvNode = ptRecvNode;
    m_strRecordPlaySession.clear();
    m_strCuSession = strCuSession;
    m_dwEventID = 0;
    m_nplayId = INVALID_PLAY_ID;
    m_dwSeqNum = INVALID_SEQ_NO;
}

CPeInviteInterAction::~CPeInviteInterAction()
{
    TerminateTask();
}

void CPeInviteInterAction::TerminateTask()
{
    //将对应的InviteTask的绑定解除
    if ( INVALID_PLAY_ID != m_nplayId )
    {
        CPeInviteTask *pcInviteTask = (CPeInviteTask *)GetInviteTask(m_nplayId);
        if ( NULL != pcInviteTask )
        {
            if ( pcInviteTask->m_pcInviteInterActTask == this )
            {
                pcInviteTask->m_pcInviteInterActTask = NULL;
            }
            if(m_dwSeqNum != INVALID_SEQ_NO)
            {
                pcInviteTask->DelChildTask(m_dwSeqNum);
            }
        }
    }
}

void CPeInviteInterAction::InitStateMachine()
{
    CStateProc cNotRequestProc;
    cNotRequestProc.ProcMsg = (CSipTask::PFProcMsg)&CPeInviteInterAction::NotReqProcMsg;
    cNotRequestProc.TimerPoll = (CSipTask::PFTimerPoll)&CPeInviteInterAction::WaitReqTimerPoll;
    cNotRequestProc.ProcErrMsg = (CSipTask::PFProcMsg)&CPeInviteInterAction::NotReqProcMsg;
    AddRuleProc(NOT_REQUEST_STATE, cNotRequestProc);

    CStateProc cWaitRspProc;
    cWaitRspProc.ProcMsg = (CSipTask::PFProcMsg)&CPeInviteInterAction::WaitRspProcMsg;
    cWaitRspProc.TimerPoll = (CSipTask::PFTimerPoll)&CPeInviteInterAction::WaitRspTimerPoll;
    cWaitRspProc.ProcErrMsg = (CSipTask::PFProcMsg)&CPeInviteInterAction::WaitRspProcMsg;
    AddRuleProc(WAIT_RESPONSE_STATE, cWaitRspProc);

    NextState(NOT_REQUEST_STATE);
}
// string CPeInviteInterAction::GetUserLogDesc()
// {
//     CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession, GetInstance()); 
//     if(pcCuAgent != NULL)
//     {
//         return string("(") + GetLanDesc(LAN_DESC_USERNAME)  + ":" + 
//             pcCuAgent->m_pcUserAccount->m_strUserName + ")";
//     } 
//     string strEmpty;
//     return strEmpty;
// }
// void CPeInviteInterAction::GetOptLogDesc(const string& oprType, const string& OptDevInfo, const u32 optType)
// {
//     m_optType = optType;
//     m_strLogDesc =  GetUserLogDesc() + GetLanDesc(oprType) + "(" + OptDevInfo + ")";
// }
void CPeInviteInterAction::WriteLog(u32 dwErrorCode, const u32 dwEvent)
{
    CGetPuData *pcTask = new CGetPuData(GetInstance(), m_strCuSession, m_tDevChn, dwErrorCode, m_strReq, dwEvent);
    pcTask->InitStateMachine();
	if (pcTask->StartWriteLog() != PROCMSG_OK)
	{
		delete pcTask;
		pcTask = NULL;
	}
}
// string CPeInviteInterAction::GetErrorCode(u32 dwErrorCode)
// {
//     if( CMS_SUCCESS == dwErrorCode)
//     {
//         return GetLanDesc(LAN_DESC_SUCCCESS);
//     }
//     else if( ERR_CUI_OPR_TIME_OUT == dwErrorCode)
//     {
//         return GetLanDesc(LAN_DESC_TIMEOUT);
//     }
//     else
//     {
//         return GetLanDesc(LAN_DESC_FAIL);
//     }
// }

u32 CPeInviteInterAction::NotReqProcMsg( CMessage *const pcMsg )
{
    COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
    TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户会话[%s]收到业务消息[%s--%d] (SIP消息[%s--%d])\n",
        m_strCuSession.c_str(),
        OspExtEventDesc(pcOspSipMsg->GetOspEventID()).c_str(), pcOspSipMsg->GetOspEventID(),
        pcOspSipMsg->GetSipEventStr().c_str(), pcOspSipMsg->GetSipEventID());

    u32 dwRet = PROCMSG_OK;

    m_dwEventID = pcMsg->event;
    switch (pcMsg->event)
    {
    case CU_CUI_INVITE_PLAY_PLATFORMRECORD_REQ:
        {
            CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession, GetInstance());
            CMS_ASSERT(NULL != pcCuAgent);
			if (NULL == pcCuAgent)
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户[%s]Agent已不存在\n",m_strCuSession.c_str());

				CInviteRsp cRsp;
				cRsp.SetErrorCode(ERR_CUI_CU_NOT_ONLINE);
				SendRspToCu<CInviteRsp>(cRsp, m_ptRecvNode);
				dwRet =  PROCMSG_DEL;
				delete this;
				break;
			}

            CCuSetupPlayPlatformRecordReq *pcReq = (CCuSetupPlayPlatformRecordReq *)pcMsg->content;
            CInviteReq cInviteReq = pcReq->GetReq();
            cInviteReq.GetSdp().SetIsNeedPunch(pcCuAgent->m_bIsSupportNA);

            m_nplayId = cInviteReq.GetDstChn().GetChnNO();
            m_tDevChn = cInviteReq.GetSrcChn();
            m_strReq = cInviteReq.ToXml();

            //1 鉴权
            bool bAuthRet = true; //鉴权结果
            do 
            {                
                string strDomainName = GetDomainNameWithoutPreNameFromURI(cInviteReq.GetSrcChn().GetDevUri());
                if ( ! pcCuAgent->IsExistDomainInfo(strDomainName))
                {
                    bAuthRet = false;
                    break;
                }

				if (pcCuAgent->m_pcUserAccount->IsSuperAdmin())
                {
                    bAuthRet = true;
                    break;
                }

                //判断用户是否有平台放像的权限
                CPrivilege cPri = pcCuAgent->m_pcUserAccount->GetUserPri();
                if(!cPri.IsEmpty())
                {
                    if(cPri.HasPri(CUserPrivilege::en_Vod_Play))
                    {
                        bAuthRet = true;
                        break;
                    }
                    else
                    {
                        bAuthRet = false;
                    }
                }
                else
                {
                    bAuthRet = false;
                    break;
                }

                string uuid = GetNameWithoutDomainFromURI(cInviteReq.GetSrcChn().GetDevUri());
                // 此时请求是通道而不是视频源
				CVideosourcePrivilege vsPri;
				string strErrMsg;
				if( !g_cRedisDb.IsUserHasVideoSource(pcCuAgent->m_pcUserAccount->m_tUserID, uuid, cInviteReq.GetSrcChn().GetChnNO(), strErrMsg) )
				{
					TASKLOG(NORMAL_TASK, ERROR_LEV, "用户name[%s] id[%s]没有通道的[%s]权限 errMsg[%s]\n",
						pcCuAgent->m_strUserName.c_str(), pcCuAgent->m_pcUserAccount->m_tUserID.c_str(), cInviteReq.GetSrcChn().ToXml().c_str(), strErrMsg.c_str());
                    bAuthRet = false;
                    break;
                }
            } while (0);

            if (!bAuthRet)
            {
                TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户[%s]会话[%s]发送Invite请求，但是对源通道[%s--%d]无权限\n",
                    pcCuAgent->m_strUserName.c_str(), pcCuAgent->m_strSession.c_str(), cInviteReq.GetSrcChn().GetDevUri().c_str(), 
                    cInviteReq.GetSrcChn().GetChnNO());

                CInviteRsp cRsp;
                cRsp.SetErrorCode(ERR_CUI_CU_NO_PERMISSION);
                SendRspToCu<CInviteRsp>(cRsp, m_ptRecvNode);
                dwRet =  PROCMSG_DEL;
                delete this;
                break;
            }

            //查找InviteTask
            CPeInviteTask *pcInviteTask = (CPeInviteTask *)GetInviteTask(m_nplayId);
            if ( NULL == pcInviteTask )
            {
                if(pcReq->GetDomainName().empty())
                {
                    TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户[%s]会话[%s]发送Invite请求，但是DomainName为空\n",
                        pcCuAgent->m_strUserName.c_str(), pcCuAgent->m_strSession.c_str());

                    CInviteRsp cRsp;
                    cRsp.SetErrorCode(ERR_CUI_INVALID_PARAM);
                    SendRspToCu<CInviteRsp>(cRsp, m_ptRecvNode);
                    delete this;
                    return PROCMSG_DEL;
                }

                TSipURI tPeUri;
                CStreamDecentrationAdapter::FindRcsUri( tPeUri, pcReq->GetDomainName() );
                pcInviteTask = new CPeInviteTask(GetInstance(), m_strCuSession, this, tPeUri.GetURIString(), pcCuAgent, m_nplayId);
                pcInviteTask->InitStateMachine();
                //将当前任务登记到CuAgent中,由于不知道是否连上外设模块，暂时不能加入到外设模块的表中
                pcCuAgent->m_cPeInviteTaskSet.Insert(cInviteReq.GetDstChn().GetChnNO(), pcInviteTask->GetTaskNO());
            }

            if ( pcInviteTask->GetState() != CPeInviteTask::NOT_REQUEST_STATE )
            {
                CInviteRsp cRsp;
                cRsp.SetSession(pcReq->GetSession());
                cRsp.SetSeqNum(pcReq->GetSeqNum());
                cRsp.SetErrorCode(ERR_CUI_DIALOG_NOT_ESTABLISHED);
                SendRspToCu<CInviteRsp>(cRsp, m_ptRecvNode);

                TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户[%s]会话[%s]已经发起过播放窗口[%d]的InviteReq，本次请求被拒绝\n",
                    pcCuAgent->m_strUserName.c_str(), pcCuAgent->m_strSession.c_str(), cInviteReq.GetDstChn().GetChnNO());

                //任务完成, 销毁之
                NextState(NOT_REQUEST_STATE);

                dwRet =  PROCMSG_DEL;
                delete this;
            }
            else
            {
                //这块不会有删除task操作，所以不需要判断
                dwRet = pcInviteTask->StartSendInviteReq(cInviteReq);
                if( dwRet == PROCMSG_OK )
                {
                    NextState(WAIT_RESPONSE_STATE);
                }
                else
                {
                    CInviteRsp cRsp;
                    cRsp.SetSession(pcReq->GetSession());
                    cRsp.SetSeqNum(pcReq->GetSeqNum());
                    cRsp.SetErrorCode( pcInviteTask->GetErrorCode() );
                    SendRspToCu<CInviteRsp>(cRsp, m_ptRecvNode);

                    TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户[%s]会话[%s] StartSendInviteReq失败, 错误码[%u] \n",
                        pcCuAgent->m_strUserName.c_str(), pcCuAgent->m_strSession.c_str(), pcInviteTask->GetErrorCode() );

                    delete pcInviteTask;
                    pcInviteTask = NULL;

                    //任务完成, 销毁之
                    NextState(NOT_REQUEST_STATE);

                    dwRet =  PROCMSG_DEL;
                    delete this;
                }
            }
        }
        break;
    case CU_CUI_INVITE_ACK_PLAY_PLATFORMRECORD_REQ:
        {
            CCuPlayPlatformRecordReq *pcReq = (CCuPlayPlatformRecordReq *)pcMsg->content;
            m_nplayId = pcReq->GetPlayId();

            TASKLOG(NORMAL_TASK, ERROR_LEV, "playID is %d\n",pcReq->GetPlayId());

            CPeInviteTask *pcInviteTask = (CPeInviteTask *)GetInviteTask(m_nplayId);
            CCuPlayPlatformRecordRsp cRsp;
            int nErrorCode = ERR_CUI_DIALOG_NOT_ESTABLISHED;
            if ( NULL == pcInviteTask )
            { //事务尚未建立
                TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户会话[%s]的Invite没有对应Task\n",
                    m_strCuSession.c_str());
                //cRsp.SetErrorCode(nErrorCode);
            }
            else
            {
                if ( pcInviteTask->GetState() == CInviteTask::WAIT_SETUP_ACK_STATE )
                {
                    //这块不会有删除task操作，所以不需要判断
                    pcInviteTask->StartSendInviteAck();
                    pcInviteTask->m_pcInviteInterActTask = NULL;
                    nErrorCode = CMS_SUCCESS;
                }
                //pcInviteTask->m_pcInviteInterActTask = NULL;
            }

            TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户会话[%s]发送的InviteAck没有应答消息，CUI直接向CU回复应答\n",
                m_strCuSession.c_str());
            //ACK消息没有应答，直接回复
            cRsp.SetErrorCode(nErrorCode);
            SendRspToCu<CCuPlayPlatformRecordRsp>(cRsp, m_ptRecvNode);
            m_nplayId = INVALID_PLAY_ID;
            dwRet = PROCMSG_DEL;
            delete this;
        }
        break;
    case RCS_TASK_START_REQ:
        {
            CCuTaskStartReq *pcReq = (CCuTaskStartReq *)pcMsg->content;
            m_nplayId = pcReq->GetPlayId();
            TASKLOG(NORMAL_TASK, ERROR_LEV, "playID is %d\n",pcReq->GetPlayId());
            CPeInviteTask *pcInviteTask = (CPeInviteTask *)GetInviteTask(m_nplayId);
            CCuPlayPlatformRecordRsp cRsp;
            int nErrorCode = ERR_CUI_DIALOG_NOT_ESTABLISHED;
            if ( NULL == pcInviteTask )
            { 
                TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户会话[%s]的Invite没有对应Task\n",
                    m_strCuSession.c_str());
                cRsp.SetErrorCode(nErrorCode);
                SendRspToCu<CCuPlayPlatformRecordRsp>(cRsp, m_ptRecvNode);
                m_nplayId = INVALID_PLAY_ID;
                dwRet = PROCMSG_DEL;
                delete this;
            }
            else
            {
                if ( pcInviteTask->GetState() == CInviteTask::INVITE_HOLD_STATE )
                {
                    pcInviteTask->StartTask();
                    pcInviteTask->m_pcInviteInterActTask = this;
                    nErrorCode = CMS_SUCCESS;
                    NextState(WAIT_RESPONSE_STATE);
                }
                else
                {
                    TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户会话[%s]的Invite Task不在正确的状态，当前状态[]\n",
                        m_strCuSession.c_str());
                    cRsp.SetErrorCode(ERR_CUI_STATE_ERROR);
                    SendRspToCu<CCuPlayPlatformRecordRsp>(cRsp, m_ptRecvNode);
                    m_nplayId = INVALID_PLAY_ID;
                    dwRet = PROCMSG_DEL;
                    delete this;
                }
            }
        }
        break;
    case CU_CUI_INVITE_BYE_PLAY_PLATFORMRECORD_REQ:
        {
            CCuStopPlatformRecordReq *pcReq = (CCuStopPlatformRecordReq *)pcMsg->content;
            m_nplayId = pcReq->GetPlayId();
            m_strReq = pcReq->ToXml();

            CPeInviteTask *pcInviteTask = (CPeInviteTask *)GetInviteTask(m_nplayId);

            if ( NULL != pcInviteTask )
            {
                if (pcInviteTask->TryBye(this))
                {
                    m_tDevChn = pcInviteTask->m_tReq.GetSrcChn();
                    NextState(WAIT_RESPONSE_STATE);
                }
                else
                {
                    CCuStopPlatformRecordRsp cRsp;
                    TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户会话[%s]的Invite Task Trybye failed\n", m_strCuSession.c_str());
                    cRsp.SetErrorCode(ERR_CUI_DIALOG_NOT_ESTABLISHED);
                    SendRspToCu<CCuStopPlatformRecordRsp>(cRsp, m_ptRecvNode);
                    WriteLog(ERR_CUI_DIALOG_NOT_ESTABLISHED, m_dwEventID);
                    m_nplayId = INVALID_PLAY_ID;
                    dwRet = PROCMSG_DEL;
                    delete this;
                }
            }
            else
            {
                CCuStopPlatformRecordRsp cRsp;
                TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户会话[%s]的Invite没有对应Task\n",
                    m_strCuSession.c_str());
                cRsp.SetErrorCode(ERR_CUI_DIALOG_NOT_ESTABLISHED);
                TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户会话[%s]发送的InviteBye没有应答消息，CUI直接向CU回复应答\n",
                    m_strCuSession.c_str());
                //BYE消息没有应答，直接回复
                SendRspToCu<CCuStopPlatformRecordRsp>(cRsp, m_ptRecvNode);
                WriteLog(ERR_CUI_DIALOG_NOT_ESTABLISHED, m_dwEventID);
                m_nplayId = INVALID_PLAY_ID;
                dwRet = PROCMSG_DEL;
                delete this;
            }            
        }
        break;
    case CU_CUI_INVITE_PLAY_DOWNLOADPLATFORMRECORD_REQ:
        { 
            CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession, GetInstance());
            CMS_ASSERT(NULL != pcCuAgent);
			if (NULL == pcCuAgent)
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户[%s]Agent已不存在\n",m_strCuSession.c_str());

				CInviteRsp cRsp;
				cRsp.SetErrorCode(ERR_CUI_CU_NOT_ONLINE);
				SendRspToCu<CInviteRsp>(cRsp, m_ptRecvNode);
				dwRet =  PROCMSG_DEL;
				delete this;
				break;
			}

            CCuDownloadPlatformRecordReq *pcReq = (CCuDownloadPlatformRecordReq *)pcMsg->content;
            CInviteReq cInviteReq = pcReq->GetReq();
            cInviteReq.GetSdp().SetIsNeedPunch(pcCuAgent->m_bIsSupportNA);
            m_nplayId = cInviteReq.GetDstChn().GetChnNO();
            m_tDevChn = cInviteReq.GetSrcChn();
            m_strReq = cInviteReq.ToXml();

            bool bAuthRet = true; //鉴权结果
            do 
            {                
                string strDomainName = GetDomainNameWithoutPreNameFromURI(cInviteReq.GetSrcChn().GetDevUri());
                if ( ! pcCuAgent->IsExistDomainInfo(strDomainName))
                {
                    bAuthRet = false;
                    break;
                }

				if (pcCuAgent->m_pcUserAccount->IsSuperAdmin())
                {
                    bAuthRet = true;
                    break;
                }

                CPrivilege cPri = pcCuAgent->m_pcUserAccount->GetUserPri();
                if(!cPri.IsEmpty())
                {
                    if(cPri.HasPri(CUserPrivilege::en_Vod_Download))
                    {
                        bAuthRet = true;
                        break;
                    }
                    else
                    {
                        bAuthRet = false;
                    }
                }
                else
                {
                    bAuthRet = true;
                    break;
                }
            }while(0);

            if (!bAuthRet)
            {
                TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户[%s]会话[%s]发送Invite请求，但是对源通道[%s--%d]无权限\n",
                    pcCuAgent->m_strUserName.c_str(), pcCuAgent->m_strSession.c_str(), cInviteReq.GetSrcChn().GetDevUri().c_str(), 
                    cInviteReq.GetSrcChn().GetChnNO());

                CInviteRsp cRsp;
                cRsp.SetErrorCode(ERR_CUI_CU_NO_PERMISSION);
                SendRspToCu<CInviteRsp>(cRsp, m_ptRecvNode);
                dwRet =  PROCMSG_DEL;
                delete this;
                break;
            }

            //查找InviteTask
            CPeInviteTask *pcInviteTask = (CPeInviteTask *)GetInviteTask(m_nplayId);
            if ( NULL == pcInviteTask )
            {
                TSipURI tRcsUri;
                CStreamDecentrationAdapter::FindRcsUri( tRcsUri, pcReq->GetDomainName() );

                pcInviteTask = new CPeInviteTask(GetInstance(), m_strCuSession, this, tRcsUri.GetURIString(), pcCuAgent, m_nplayId);
                pcInviteTask->InitStateMachine();
                //将当前任务登记到CuAgent中
                pcCuAgent->m_cPeInviteTaskSet.Insert(cInviteReq.GetDstChn().GetChnNO(), pcInviteTask->GetTaskNO());
            }

            if ( pcInviteTask->GetState() != CPeInviteTask::NOT_REQUEST_STATE )
            {
                CInviteRsp cRsp;
                cRsp.SetSession(pcReq->GetSession());
                cRsp.SetSeqNum(pcReq->GetSeqNum());
                cRsp.SetErrorCode(ERR_CUI_DIALOG_NOT_ESTABLISHED);
                SendRspToCu<CInviteRsp>(cRsp, m_ptRecvNode);

                TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户[%s]会话[%s]已经发起过播放窗口[%d]的InviteReq，本次请求被拒绝\n",
                    pcCuAgent->m_strUserName.c_str(), pcCuAgent->m_strSession.c_str(), cInviteReq.GetDstChn().GetChnNO());

                //任务完成, 销毁之
                NextState(NOT_REQUEST_STATE);

                dwRet =  PROCMSG_DEL;
                delete this;
            }
            else
            {
                //这块不会有删除task操作，所以不需要判断
                dwRet = pcInviteTask->StartSendInviteReq(cInviteReq);
                if( dwRet == PROCMSG_OK )
                {
                    NextState(WAIT_RESPONSE_STATE);
                }
                else
                {
                    CInviteRsp cRsp;
                    cRsp.SetSession(pcReq->GetSession());
                    cRsp.SetSeqNum(pcReq->GetSeqNum());
                    cRsp.SetErrorCode( pcInviteTask->GetErrorCode() );
                    SendRspToCu<CInviteRsp>(cRsp, m_ptRecvNode);

                    TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户[%s]会话[%s] StartSendInviteReq失败, 错误码[%u] \n",
                        pcCuAgent->m_strUserName.c_str(), pcCuAgent->m_strSession.c_str(), pcInviteTask->GetErrorCode() );

                    delete pcInviteTask;
                    pcInviteTask = NULL;

                    //任务完成, 销毁之
                    NextState(NOT_REQUEST_STATE);

                    dwRet =  PROCMSG_DEL;
                    delete this;

                }

            }
        }
        break;
    case CU_CUI_INVITE_ACK_DOWNLOADPLATFORMRECORD_REQ:
        {            
            CCuStartDownloadPlatformRecordReq *pcReq = (CCuStartDownloadPlatformRecordReq *)pcMsg->content;
            m_nplayId = pcReq->GetPlayId();

            TASKLOG(NORMAL_TASK, ERROR_LEV, "playID is %d\n",pcReq->GetPlayId());

            CPeInviteTask *pcInviteTask = (CPeInviteTask *)GetInviteTask(m_nplayId);
            CCuStartDownloadPlatformRecordRsp cRsp;
            int nErrorCode = ERR_CUI_DIALOG_NOT_ESTABLISHED;
            if ( NULL == pcInviteTask )
            { //事务尚未建立
                TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户会话[%s]的Invite没有对应Task\n",
                    m_strCuSession.c_str());
                cRsp.SetErrorCode(nErrorCode);
            }
            else
            {
                if ( pcInviteTask->GetState() == CInviteTask::WAIT_SETUP_ACK_STATE )
                {
                    //这块不会有删除task操作，所以不需要判断
                    pcInviteTask->StartSendInviteAck();
                    pcInviteTask->m_pcInviteInterActTask = NULL;
                    nErrorCode = CMS_SUCCESS;
                }
                //pcInviteTask->m_pcInviteInterActTask = NULL;
            }

            TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户会话[%s]发送的InviteAck没有应答消息，CUI直接向CU回复应答\n",
                m_strCuSession.c_str());
            //ACK消息没有应答，直接回复
            cRsp.SetErrorCode(nErrorCode);
            SendRspToCu<CCuStartDownloadPlatformRecordRsp>(cRsp, m_ptRecvNode);
            m_nplayId = INVALID_PLAY_ID;
            dwRet = PROCMSG_DEL;
            delete this;
        }
        break;
    case CU_CUI_INVITE_BYE_DOWNLOADPLATFORMRECORD_REQ:
        {
            CCuStopDownloadPlatformRecordReq *pcReq = (CCuStopDownloadPlatformRecordReq *)pcMsg->content;
            m_nplayId = pcReq->GetPlayId();
            m_strReq = pcReq->ToXml();

            CPeInviteTask *pcInviteTask = (CPeInviteTask *)GetInviteTask(m_nplayId);
            if ( NULL != pcInviteTask )
            {
                if (pcInviteTask->TryBye(this))
                {
                    m_tDevChn = pcInviteTask->m_tReq.GetSrcChn();
                    NextState(WAIT_RESPONSE_STATE);
                }
                else
                {
                    CCuStopDownloadPlatformRecordRsp cRsp;
                    TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户会话[%s]的Invite Task TryBye failed\n", m_strCuSession.c_str());
                    cRsp.SetErrorCode(ERR_CUI_DIALOG_NOT_ESTABLISHED);
                    SendRspToCu<CCuStopDownloadPlatformRecordRsp>(cRsp, m_ptRecvNode);
                    WriteLog(ERR_CUI_DIALOG_NOT_ESTABLISHED, m_dwEventID);
                    m_nplayId = INVALID_PLAY_ID;
                    dwRet = PROCMSG_DEL;
                    delete this;
                }
            }
            else
            {
                CCuStopDownloadPlatformRecordRsp cRsp;
                TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户会话[%s]的Invite没有对应Task\n",
                    m_strCuSession.c_str());
                cRsp.SetErrorCode(ERR_CUI_DIALOG_NOT_ESTABLISHED);
                TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户会话[%s]发送的InviteBye没有应答消息，CUI直接向CU回复应答\n",
                    m_strCuSession.c_str());
                //BYE消息没有应答，直接回复
                SendRspToCu<CCuStopDownloadPlatformRecordRsp>(cRsp, m_ptRecvNode);
                WriteLog(ERR_CUI_DIALOG_NOT_ESTABLISHED, m_dwEventID);
                m_nplayId = INVALID_PLAY_ID;
                dwRet = PROCMSG_DEL;
                delete this;
            } 
        }
        break;
    case VCR_CTRL_REQ:
        {
            CCuVcrCtrlReq *pcReq = (CCuVcrCtrlReq *)pcMsg->content;
            m_nplayId = pcReq->GetPlayId();
            m_dwSeqNum = pcReq->GetSeqNum();

            CPeInviteTask *pcInviteTask = (CPeInviteTask *)GetInviteTask(m_nplayId);
            if ( NULL != pcInviteTask )
            {
                pcInviteTask->AddChildTask(pcReq->GetSeqNum(), GetTaskNO());
                //pcInviteTask->m_pcInviteInterActTask = this;
                pcInviteTask->StartVcrCtrl(*pcReq);
                NextState(WAIT_RESPONSE_STATE);
            }
            else
            {
                CCuVcrCtrlRsp cRsp;
                TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户会话[%s]的Invite没有对应Task\n",
                    m_strCuSession.c_str());
                cRsp.SetErrorCode(ERR_CUI_DIALOG_NOT_ESTABLISHED);

                SendRspToCu<CCuVcrCtrlRsp>(cRsp, m_ptRecvNode);
                m_nplayId = INVALID_PLAY_ID;
                delete this;
            }  
        }
        break;
    default:
        {
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"收到未知消息");
        }
        break;
    }
    return dwRet;
}

u32 CPeInviteInterAction::WaitRspProcMsg( CMessage *const pcMsg )
{
    COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
    TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户会话[%s]收到业务应答消息[%s--%d] (SIP应答消息[%s--%d])\n",
        m_strCuSession.c_str(),
        OspExtEventDesc(pcOspSipMsg->GetOspEventID()).c_str(), pcOspSipMsg->GetOspEventID(),
        pcOspSipMsg->GetSipEventStr().c_str(), pcOspSipMsg->GetSipEventID());

    u32 dwRet = PROCMSG_OK;

    switch (pcMsg->event)
    {
    case OSP_SIP_MSG_PROC_FAIL:
    case OSP_SIP_DIALOG_TERMINATE:
        {
            PostErrMsg2Cu(ERR_CUI_SIP_MSG_FAIL);
            WriteLog(ERR_CUI_SIP_MSG_FAIL, m_dwEventID);
            TASKLOG(NORMAL_TASK, ERROR_LEV, "向CU用户会话[%s]发送失败应答消息，错误码[ERR_CUI_SIP_MSG_FAIL]\n",
                m_strCuSession.c_str());
            //任务完成, 销毁之
            NextState(NOT_REQUEST_STATE);
            dwRet =  PROCMSG_DEL;
        }
        break;
    case INVITE_RSP:
        {
            CInviteRsp cRsp;
            pcOspSipMsg->GetMsgBody(cRsp);
            if ( INVALID_DIALOG_ID == pcOspSipMsg->GetSipDlgID() )
            {
                cRsp.SetErrorCode(ERR_CUI_DIALOG_NOT_ESTABLISHED);
            }

            SendRspToCu<CInviteRsp>(cRsp, m_ptRecvNode);
            WriteLog((u32)cRsp.GetErrorCode(), m_dwEventID);
            //任务完成, 销毁之
            NextState(NOT_REQUEST_STATE);
            dwRet = PROCMSG_DEL;
        }
        break;
    case OSP_SIP_BYE_REQ:
        { //服务器端主动结束会话
            if(pcOspSipMsg->GetMsgBodyLen() > 0)
            {
                CByeReq cByeReq;
                pcOspSipMsg->GetMsgBody(cByeReq);

                PostErrMsg2Cu((u32)cByeReq.GetReason());
                WriteLog(cByeReq.GetReason(), m_dwEventID);
            }
            else
            {
                PostErrMsg2Cu(ERR_CUI_DIALOG_TERMINATED);
                WriteLog(ERR_CUI_DIALOG_TERMINATED, m_dwEventID);
            }        


            //任务完成, 销毁之
            NextState(NOT_REQUEST_STATE);
            dwRet = PROCMSG_DEL;
        }
        break;
    case OSP_SIP_BYE_RSP:
        {
            CCuStopInviteRsp cRsp;
            WriteLog((u32)cRsp.GetErrorCode(), m_dwEventID);
            SendRspToCu<CCuStopInviteRsp>(cRsp, m_ptRecvNode);

            dwRet = PROCMSG_DEL;
        }
        break;
    case PLAYBACK_PROGRESS_QUERY_RSP:
        {
            CCuPlaybackProgressQueryRsp cRsp;
            pcOspSipMsg->GetMsgBody(cRsp);

            SendRspToCu<CCuPlaybackProgressQueryRsp>(cRsp, m_ptRecvNode);
            //任务完成, 销毁之
            NextState(NOT_REQUEST_STATE);
            delete this;
        }
        break;
    case VCR_CTRL_RSP:
        {
            CCuVcrCtrlRsp cRsp;
            pcOspSipMsg->GetMsgBody(cRsp);
            SendRspToCu<CCuVcrCtrlRsp>(cRsp, m_ptRecvNode);
            NextState(NOT_REQUEST_STATE);
            delete this;
        }
        break;
    case RCS_TASK_START_RSP:
        {
            CRcsTaskStartRsp cRcsRsp;
            pcOspSipMsg->GetMsgBody(cRcsRsp);
            CCuTaskStartRsp cCuRsp;
            cCuRsp.SetErrorCode(cRcsRsp.GetErrorCode());
            SendRspToCu<CCuTaskStartRsp>(cCuRsp, m_ptRecvNode);
            NextState(NOT_REQUEST_STATE);
            delete this;
        }
        break;
    default:
        {
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"收到未知消息");
        }
        break;
    }
    return dwRet;
}

u32 CPeInviteInterAction::WaitReqTimerPoll()
{
    if ( GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout() )
    {
        TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户会话[%s]等待Invite应答超时\n",
            m_strCuSession.c_str());

        PostErrMsg2Cu(ERR_CUI_OPR_TIME_OUT);
        WriteLog(ERR_CUI_OPR_TIME_OUT, m_dwEventID);

        NextState(NOT_REQUEST_STATE);
        return TIMERPOLL_DEL;
    }
    return TIMERPOLL_DONE;
}

u32 CPeInviteInterAction::WaitRspTimerPoll()
{
    if ( GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout() )
    {
        TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户会话[%s]等待Invite应答超时\n",
            m_strCuSession.c_str());

        PostErrMsg2Cu(ERR_CUI_OPR_TIME_OUT);
        WriteLog(ERR_CUI_OPR_TIME_OUT, m_dwEventID);

        NextState(NOT_REQUEST_STATE);
        return TIMERPOLL_DEL;
    }
    return TIMERPOLL_DONE;
}

CTask *CPeInviteInterAction::GetInviteTask(s32 nPlayId)
{
    CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession, GetInstance());
    //由于用户下线不删除 PeInviteInterAciton，只删除CPeInvite，当用户先退出后，CPeInviteInterAction超时，导致找不到用户task，产生该断言
    //CMS_ASSERT(NULL != pcCuAgent);
    u32 dwTaskId = INVALID_TASKNO;
    if(pcCuAgent != NULL)
    {
        if ( ! pcCuAgent->m_cPeInviteTaskSet.Find(m_nplayId, dwTaskId) )
        {
            return NULL;
        }

        if ( INVALID_TASKNO == dwTaskId )
        {
            return NULL;
        }

        return GetInstance()->FindTask(dwTaskId);
    }

    return NULL;
}

void CPeInviteInterAction::PostErrMsg2Cu(u32 dwErrorCode)
{
    switch(m_dwEventID)
    {
    case CU_CUI_INVITE_PLAY_PLATFORMRECORD_REQ:
        {
            SendErrorMsg2Cu<CCuSetupPlayPlatformRecordRsp>(dwErrorCode);
        }
        break;
    case CU_CUI_INVITE_ACK_PLAY_PLATFORMRECORD_REQ:
        {
            SendErrorMsg2Cu<CCuPlayPlatformRecordRsp>(dwErrorCode);
        }
        break;
    case CU_CUI_INVITE_BYE_PLAY_PLATFORMRECORD_REQ:
        {
            SendErrorMsg2Cu<CCuStopPlatformRecordRsp>(dwErrorCode);
        }
        break;
    case CU_CUI_INVITE_PLAY_DOWNLOADPLATFORMRECORD_REQ:
        {            
            SendErrorMsg2Cu<CCuDownloadPlatformRecordRsp>(dwErrorCode);
        }
        break;
    case CU_CUI_INVITE_ACK_DOWNLOADPLATFORMRECORD_REQ:
        {            
            SendErrorMsg2Cu<CCuStartDownloadPlatformRecordRsp>(dwErrorCode);
        }
        break;
    case CU_CUI_INVITE_BYE_DOWNLOADPLATFORMRECORD_REQ:
        {
            SendErrorMsg2Cu<CCuStopDownloadPlatformRecordRsp>(dwErrorCode);
        }
        break;
    case PLAYBACK_PROGRESS_QUERY_REQ:
        {
            SendErrorMsg2Cu<CCuPlaybackProgressQueryRsp>(dwErrorCode);
        }
        break;
    case  VCR_CTRL_REQ:
        {
            SendErrorMsg2Cu<CCuVcrCtrlRsp>(dwErrorCode);
        }
        break;
    case RCS_TASK_START_REQ:
        {
            SendErrorMsg2Cu<CCuTaskStartRsp>(dwErrorCode);
        }
        break;
    default:
        {
            SendErrorMsg2Cu<CEventRsp>(dwErrorCode);
        }
        break;
    }
}

/////////////////////////////////////////////////////////////////////////
//第三方呼叫task 支持新的穿NAT方案
/////////////////////////////////////////////////////////////////////////
CCallPuTaskEx::CCallPuTaskEx(CInstExt *pcInst, const string &strCuSession,string strCallPuReq, CSipTask *pcTask) : CSipTask(pcInst)//, m_cStrmNumRestorer(pcInst)
{
	CMS_ASSERT(!strCuSession.empty());
	CMS_ASSERT(NULL != pcTask);

	m_pcCallPuInterActTask = pcTask;
	m_strCuSession = strCuSession;
	m_strCallPuReq = strCallPuReq;
	m_tDialogID = INVALID_DIALOG_ID;

    //m_cStrmNumRestorer.SetUserLoginSession( strCuSession );
    m_dwErrorCode = CMS_SUCCESS;
}

CCallPuTaskEx::~CCallPuTaskEx()
{
	TerminateTask();
	//m_tPuChnIDList.clear();
    if (!m_strModuleId.empty())
	{
		CRedisSsChannel cSsChn = g_cCuiRedisSdk.GetModuleSsChannelById(m_strModuleId);
		CRedisSsMng::GetInstance()->DelSs(cSsChn, GetTaskNO());
	}
}

void CCallPuTaskEx::TerminateTask()
{
	if ( m_tDialogID != INVALID_DIALOG_ID )
	{ //释放会话
		PostInDlgReq(KDSIP_EVENT_INVITE_BYE_REQ, CByeReq(), m_tDialogID);
		//OspSipReleaseDlg(m_tDialogID);
		m_tDialogID = INVALID_DIALOG_ID;
	}

	//注销CuAgent中的浏览事务
	if ( INVALID_PLAY_ID != m_tCuChn.GetChnNO() )
	{
		//m_pcCuAgent->m_cInviteTaskSet.Erase(m_tCuChn.GetChnNO());
		CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession, GetInstance());
		//当调用CuAgent析构后，Invite等待bye响应时，CuAgent已经被干掉了，所以去掉CMS_ASSERT
		//CMS_ASSERT(pcCuAgent != NULL);
		if(NULL != pcCuAgent)
		{
			pcCuAgent->m_cCallPuTaskSet.Erase(m_tCuChn.GetChnNO());
		}

		PopMediaTask(m_strCuSession,m_tCuChn.GetChnNO());

		m_tCuChn.SetChnNO(INVALID_PLAY_ID);
	}

	m_pcCallPuInterActTask = NULL;
}

bool CCallPuTaskEx::IsNotNeedPollState()
{
	if (GetState() == INVITE_HOLD_STATE)
	{
		return true;
	}

	return false;
}

void CCallPuTaskEx::InitStateMachine()
{
	CStateProc cWaitSetUpReqProc;
	cWaitSetUpReqProc.ProcMsg = (CSipTask::PFProcMsg)&CCallPuTaskEx::WaitSetUpReqProcMsg;
	cWaitSetUpReqProc.TimerPoll = (CSipTask::PFTimerPoll)&CCallPuTaskEx::WaitSetUpReqTimerPoll;
	cWaitSetUpReqProc.ProcErrMsg = (CSipTask::PFProcMsg)&CCallPuTaskEx::WaitSetUpReqProcMsg;
	AddRuleProc(WAIT_SETUP_REQ_STATE, cWaitSetUpReqProc);

	CStateProc cWaitAckState;
	cWaitAckState.ProcMsg = (CSipTask::PFProcMsg)&CCallPuTaskEx::WaitAckProcMsg;
	cWaitAckState.TimerPoll = (CSipTask::PFTimerPoll)&CCallPuTaskEx::WaitAckTimerPoll;
	cWaitAckState.ProcErrMsg = (CSipTask::PFProcMsg)&CCallPuTaskEx::WaitAckProcMsg;
	AddRuleProc(WAIT_ACK_STATE, cWaitAckState);

	CStateProc cInviteHoldState;
	cInviteHoldState.ProcMsg = (CSipTask::PFProcMsg)&CCallPuTaskEx::InviteHoldProcMsg;
	cInviteHoldState.TimerPoll = &CSipTask::TimerPollSkip;
	cInviteHoldState.ProcErrMsg = (CSipTask::PFProcMsg)&CCallPuTaskEx::InviteHoldProcMsg;
	AddRuleProc(INVITE_HOLD_STATE, cInviteHoldState);

	NextState(WAIT_SETUP_REQ_STATE);
}

u32 CCallPuTaskEx::WaitSetUpReqProcMsg(CMessage *const pcMsg)
{
	TASKLOG(NORMAL_TASK, EVENT_LEV, "CCallPuTaskEx收到CU用户会话[%s]的请求[%s--%hu]\n",
		m_strCuSession.c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

	u32 dwRet = PROCMSG_OK;

	switch(pcMsg->event)
	{
	case INVITE_REQ:
		{
			COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
			CMS_ASSERT(pcOspSipMsg && "ospsip 消息为空\n");

			CInviteReq cReq;
			pcOspSipMsg->GetMsgBody(cReq);

			m_strReq = cReq.ToXml();

			m_tDialogID = pcOspSipMsg->GetSipDlgID();
			//1 鉴权
			bool bAuthRet = true; //鉴权结果
			CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession, GetInstance());
			CMS_ASSERT(NULL != pcCuAgent);
			if(NULL == pcCuAgent)
			{
				CInviteRsp cRsp;
				cRsp.SetErrorCode(ERR_CUI_CU_NOT_ONLINE);
				cRsp.SetSeqNum(cReq.GetSeqNum());
				cRsp.SetSession(cReq.GetSession());

				PostRsp<CInviteRsp>(KDSIP_EVENT_INVITE_RSP, pcOspSipMsg->GetSipTransID(), cRsp);
				TASKLOG(NORMAL_TASK, ERROR_LEV, "reply ERR_CUI_CU_NOT_ONLINE\n");
				//delete this;
				return PROCMSG_DEL;
			}

            //if( !m_cStrmNumRestorer.Start() )
            //{
            //    TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户会话[%s] 已达到最大并发数，本次请求被拒绝\n", m_strCuSession.c_str() );
            //    m_dwErrorCode = ERR_CUI_USER_EXCEEDED_MAX_STREAM;

            //    CInviteRsp cRsp;
            //    cRsp.SetSession( cReq.GetSession());
            //    cRsp.SetSeqNum( cReq.GetSeqNum());
            //    cRsp.SetErrorCode( GetErrorCode() );

            //    PostRsp<CInviteRsp>(KDSIP_EVENT_INVITE_RSP, pcOspSipMsg->GetSipTransID(), cRsp);                   

            //    return PROCMSG_DEL;
            //}

			do 
			{                             
				string strDomainName = GetDomainNameWithoutPreNameFromURI(cReq.GetSrcChn().GetDevUri());
				if ( ! pcCuAgent->IsExistDomainInfo(strDomainName))
				{
					bAuthRet = false;
					break;
				}

				if (pcCuAgent->m_pcUserAccount->IsSuperAdmin())
				{
					bAuthRet = true;
					break;
				}

				CPrivilege cPri = pcCuAgent->m_pcUserAccount->GetUserPri();
				if(!cPri.IsEmpty())
				{
					if(cPri.HasPri(CUserPrivilege::en_Call_Ctrl))
					{
						bAuthRet = true;
						break;
					}
					else
					{
						bAuthRet = false;
					}
				}
				else
				{
					bAuthRet = false;
					break;
				}

				string uuid = GetNameWithoutDomainFromURI(cReq.GetDstChn().GetDevUri());
				if (!g_cRedisDb.IsUserHasDev(pcCuAgent->m_pcUserAccount->m_tUserID, uuid))
				{
					TASKLOG(NORMAL_TASK, ERROR_LEV, "用户[%s]没有划归这个设备 [%s]\n", pcCuAgent->m_pcUserAccount->m_tUserID.c_str(), uuid.c_str());
					bAuthRet = false;
					break;
				}
			} while (0);

			if (!bAuthRet)
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户会话[%s]发送Invite请求，但是对源通道[%s--%d]无权限\n",
					m_strCuSession.c_str(),
					cReq.GetSrcChn().GetDevUri().c_str(), cReq.GetSrcChn().GetChnNO());

				CInviteRsp cRsp;
				cRsp.SetErrorCode(ERR_CUI_CU_NO_PERMISSION);
				cRsp.SetSeqNum(cReq.GetSeqNum());
				cRsp.SetSession(cReq.GetSession());

				PostRsp<CInviteRsp>(KDSIP_EVENT_INVITE_RSP, pcOspSipMsg->GetSipTransID(), cRsp);
				//delete this;
				return PROCMSG_DEL;
			}

			if(INVALID_TRANS_ID ==  m_tDialogID )
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户会话[%s]发送Invite请求，但是TransID无效,DevUri[%s],SrcChnNo[%d]\n",
					m_strCuSession.c_str(),
					cReq.GetSrcChn().GetDevUri().c_str(), cReq.GetSrcChn().GetChnNO());

				CInviteRsp cRsp;
				cRsp.SetErrorCode(ERR_CUI_INVALID_PARAM);
				cRsp.SetSeqNum(cReq.GetSeqNum());
				cRsp.SetSession(cReq.GetSession());

				PostRsp<CInviteRsp>(KDSIP_EVENT_INVITE_RSP, pcOspSipMsg->GetSipTransID(), cRsp);
				//delete this;
				dwRet = PROCMSG_DEL;
			}

			//m_tPuChnIDList.push_back(cReq.GetDstChn());

			CCallPuInterAction *pcCallPuInterActTask= (CCallPuInterAction *)m_pcCallPuInterActTask;
			CMS_ASSERT(NULL != pcCallPuInterActTask);
			if(pcCallPuInterActTask != NULL)
			{
				CCuStartThirdPartyCallReq cCuStartThirdPartyCallReq;
				cCuStartThirdPartyCallReq.FromXml(pcCallPuInterActTask->m_strSrcReq);

				m_tCuChn = cCuStartThirdPartyCallReq.GetSrcChn();               

				CInviteRsp cRsp;

				cRsp.SetSeqNum(cReq.GetSeqNum());
				cRsp.SetSession(cReq.GetSession());
				cRsp.GetSdp().SetOwner(m_tCuChn);
				cRsp.GetSdp().SetSessionName(cReq.GetSdp().GetSessionName());
				cRsp.GetSdp().SetMediaDescList(cCuStartThirdPartyCallReq.GetSdp().GetMediaDescList());
				cRsp.GetSdp().CopyMediaChnKeyFrom(cReq.GetSdp());
				if (NATModeInner == pcCuAgent->m_ePlatNat)
				{
				    cRsp.GetSdp().SetIsNatPacketMode(true);
				}

				PostRsp<CInviteRsp>(KDSIP_EVENT_INVITE_RSP, pcOspSipMsg->GetSipTransID(), cRsp);

                m_strModuleId = pcOspSipMsg->GetSipFromUri().GetUser();
                CRedisSsChannel cSsChn = g_cCuiRedisSdk.GetModuleSsChannelById(m_strModuleId);
		        CRedisSsMng::GetInstance()->AddSs(cSsChn, GetTaskNO());

				NextState(WAIT_ACK_STATE);
			}
			else
			{
				dwRet = PROCMSG_DEL;
			}
		}
		break;
	default:
		{
			dwRet = PROCMSG_FAIL;
			TASKLOG(NORMAL_TASK, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
		}
		break;
	}
	return dwRet;
}

u32 CCallPuTaskEx::WaitSetUpReqTimerPoll()
{
	if ( GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout() )
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户会话[%s]等待Invite请求超时\n",
			m_strCuSession.c_str());

		return TIMERPOLL_DEL;
	}
	return TIMERPOLL_DONE;
}

u32 CCallPuTaskEx::WaitAckProcMsg(CMessage *const pcMsg)
{
	TASKLOG(NORMAL_TASK, EVENT_LEV, "收到CU用户会话[%s]的消息[%s--%hu]\n",
		m_strCuSession.c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

	u32 dwRet = PROCMSG_OK;

	switch(pcMsg->event)
	{
	case INVITE_ACK:
		{
			COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
            CInviteACK cAck;
			if (NULL != pcOspSipMsg)
			{
				
				pcOspSipMsg->GetMsgBody(cAck);

				CInviteReq cReq;
				cReq.FromXml(m_strReq);
				cReq.SetSdp(cAck.GetSdp());

				//m_strReq = cReq.ToXml();
			}
			//ACK无失败的时候,直接状态切换

            TASKLOG( NORMAL_TASK, EVENT_LEV, "Ack content [%s] \n", cAck.ToXml().c_str() );
			NextState(INVITE_HOLD_STATE);
		}
		break;
	case OSP_SIP_MSG_PROC_FAIL:
	case OSP_SIP_DIALOG_TERMINATE:
		{
			COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
			TASKLOG(NORMAL_TASK, EVENT_LEV, "CCallPuTask[TaskNO %u]收到OSP_SIP_MGS_PROC_FAIL，DialogID[%u]，自动回复应答\n",
				GetTaskNO(), pcOspSipMsg->GetSipDlgID() );

			CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession, GetInstance());
			CMS_ASSERT(pcCuAgent != NULL);
			if(pcCuAgent != NULL)
			{
				CInviteStopNotify cInviteStopNtf;
				cInviteStopNtf.playId = m_tCuChn.GetChnNO();
				cInviteStopNtf.errorCode = ERR_CUI_DIALOG_TERMINATED;

				pcCuAgent->m_cNotifyPool.InsertInviteStopNotify(cInviteStopNtf);

				string strNullMsg;
				PostRsp(KDSIP_EVENT_INVITE_BYE_RSP, pcOspSipMsg->GetSipTransID(), strNullMsg);
				m_tDialogID = INVALID_DIALOG_ID;
			}           

			NextState(WAIT_SETUP_REQ_STATE);
			delete this;
			dwRet =  PROCMSG_DEL;
		}
		break;
	default:
		{
			dwRet = PROCMSG_FAIL;
			TASKLOG(NORMAL_TASK, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
		}
		break;
	}
	return PROCMSG_OK;
}

u32 CCallPuTaskEx::WaitAckTimerPoll()
{
	if ( GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout() )
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户会话[%s]等待Invite请求超时\n",
			m_strCuSession.c_str());

		return TIMERPOLL_DEL;
	}
	return TIMERPOLL_DONE;
}

u32 CCallPuTaskEx::InviteHoldProcMsg(CMessage *const pcMsg)
{
	COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
	TASKLOG(NORMAL_TASK, EVENT_LEV, "收到CU用户会话[%s]的请求[%s--%hu]\n",
		m_strCuSession.c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

	u32 dwRet = PROCMSG_OK;
	switch ( pcMsg->event )
	{
	case CUI_NATDETECT_CALLBACK:
		{
			ProcNatCBMsg(pcMsg);
		}
		break;
	case UPDATE_SWITCH_ADDRESS_RSP:
		{
			CUpdateSwitchAddressRsp cRsp;
			pcOspSipMsg->GetMsgBody(cRsp);

			if (CMS_SUCCESS != cRsp.GetErrorCode())
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "CCallPuTaskEx[TaskNO %u]更新交换地址失败，DialogID[%u],错误码[%d].\n",
					GetTaskNO(), pcOspSipMsg->GetSipDlgID(),cRsp.GetErrorCode());
			}
			else
			{
				TASKLOG(NORMAL_TASK, EVENT_LEV, "CCallPuTaskEx[TaskNO %u]更新交换地址成功，DialogID[%u].\n",
					GetTaskNO(), pcOspSipMsg->GetSipDlgID());
			}
		}
		break;
	case OSP_SIP_MSG_PROC_FAIL:
	case OSP_SIP_DIALOG_TERMINATE:
		{
			COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
			TASKLOG(NORMAL_TASK, EVENT_LEV, "CCallPuTask[TaskNO %u]收到OSP_SIP_MGS_PROC_FAIL，DialogID[%u]，自动回复应答\n",
				GetTaskNO(), pcOspSipMsg->GetSipDlgID() );

			CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession, GetInstance());
			CMS_ASSERT(pcCuAgent != NULL);
			if(pcCuAgent != NULL)
			{
				CInviteStopNotify cInviteStopNtf;
				cInviteStopNtf.playId = m_tCuChn.GetChnNO();
				cInviteStopNtf.errorCode = ERR_CUI_DIALOG_TERMINATED;

				pcCuAgent->m_cNotifyPool.InsertInviteStopNotify(cInviteStopNtf);
				string strNullMsg;

				m_tDialogID = INVALID_DIALOG_ID;
			}           

			NextState(WAIT_SETUP_REQ_STATE);
			delete this;
			dwRet =  PROCMSG_DEL;
		}
		break;
	case OSP_SIP_BYE_REQ:
		{
			COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
			TASKLOG(NORMAL_TASK, EVENT_LEV, "CCallPuTask[TaskNO %u]收到OSP_SIP_MGS_PROC_FAIL，DialogID[%u]，自动回复应答\n",
				GetTaskNO(), pcOspSipMsg->GetSipDlgID() );

			CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession, GetInstance());
			CMS_ASSERT(pcCuAgent != NULL);
			if(pcCuAgent != NULL)
			{                
				CInviteStopNotify cInviteStopNtf;
				cInviteStopNtf.playId = m_tCuChn.GetChnNO();

				if(pcOspSipMsg->GetMsgBodyLen() > 0)
				{
					CByeReq cByeReq;
					pcOspSipMsg->GetMsgBody(cByeReq);

					cInviteStopNtf.errorCode = cByeReq.GetReason();
				}
				else
				{
					cInviteStopNtf.errorCode = ERR_CUI_DIALOG_TERMINATED;
				}

				pcCuAgent->m_cNotifyPool.InsertInviteStopNotify(cInviteStopNtf);
				string strNullMsg;

				PostRsp(KDSIP_EVENT_INVITE_BYE_RSP, pcOspSipMsg->GetSipTransID(), strNullMsg);
				m_tDialogID = INVALID_DIALOG_ID;
			}           

			NextState(WAIT_SETUP_REQ_STATE);
			delete this;
			dwRet =  PROCMSG_DEL;
		}
		break;
	default:
		{
			TASKLOG(NORMAL_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
				OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
			CMS_ASSERT(0&&"收到未知消息");
		}
		break;
	}
	return dwRet;
}

void CCallPuTaskEx::PrintData()const
{
    CSipTask::PrintData();

    OspPrintf(TRUE, FALSE, "m_strReq[%s]\n\n", m_strReq.c_str());
}

//NAT探测包回调地址，如果地址有变化需要通知VTDU 更改交换地址
void CCallPuTaskEx::ProcNatCBMsg(CMessage *const pcMsg)
{
	CCuStartThirdPartyCallReq tReq;
	tReq.FromXml(m_strCallPuReq);

	TNatPacketInfo tNatPacket = *(reinterpret_cast<TNatPacketInfo*>(pcMsg->content));

	TMediaDescList& tMediaDescList = tReq.GetSdp().GetMediaDescList();

	//nat探测包的reserve无效，走老的替换流程：源地址和交换地址匹配才替换nat地址；
	//reserve有效，走（sdk）新的替换流程：nat地址和当前交换地址不匹配就替换nat地址。
	TASKLOG(NORMAL_TASK, EVENT_LEV, "Reserver[%d]\n", tNatPacket.m_byReserve);
	if (tNatPacket.IsReserveInvalid())//老流程
	{
		for(TMediaDescList::iterator itrMediaDesc = tMediaDescList.begin(); 
			itrMediaDesc!=tMediaDescList.end(); itrMediaDesc++)
		{
			if (itrMediaDesc->GetMediaType() == MEDIA_TYPE_AUDIO)
			{
				TMediaTransChannelList& tMediaTransChnList = itrMediaDesc->GetMediaTransChnList();
				for(TMediaTransChannelList::iterator itrTransChn = tMediaTransChnList.begin(); 
					itrTransChn!=tMediaTransChnList.end(); itrTransChn++)
				{
					if (itrTransChn->GetTransChnType() == MEDIA_TRANS_CHN_RTP)
					{
						TNetAddrList& tNatAddrList = itrTransChn->GetAddrList();
						for (TNetAddrList::iterator itrNatAddr = tNatAddrList.begin();
							itrNatAddr != tNatAddrList.end();itrNatAddr++)
						{
							if (itrNatAddr->GetNetIpv4() == tNatPacket.m_dwSrcIp && itrNatAddr->GetNetPort() == tNatPacket.m_wSrcPort)
							{
								CUpdateSwitchAddressReq cReq;
								cReq.SetMediaType(itrMediaDesc->GetMediaType());
								cReq.SetTransChannel(MEDIA_TRANS_CHN_RTP);
								cReq.SetPreviousAddr(*itrNatAddr);
								cReq.SetPresentAddr(tNatPacket.m_dwNatIp,tNatPacket.m_wNatPort);

								PostInDlgReq<CUpdateSwitchAddressReq>(KDSIP_EVENT_MESSAGE_DIALOG_REQ, cReq, m_tDialogID);

								itrNatAddr->SetNetIpv4(tNatPacket.m_dwNatIp);
								itrNatAddr->SetNetPort(tNatPacket.m_wNatPort);
							}
						}
					}
					else if (itrTransChn->GetTransChnType() == MEDIA_TRANS_CHN_RTCP)
					{
						TNetAddrList& tNatAddrList = itrTransChn->GetAddrList();
						for (TNetAddrList::iterator itrNatAddr = tNatAddrList.begin();
							itrNatAddr != tNatAddrList.end();itrNatAddr++)
						{
							if (itrNatAddr->GetNetIpv4() == tNatPacket.m_dwSrcIp && itrNatAddr->GetNetPort() == tNatPacket.m_wSrcPort)
							{
								CUpdateSwitchAddressReq cReq;
								cReq.SetMediaType(itrMediaDesc->GetMediaType());
								cReq.SetTransChannel(MEDIA_TRANS_CHN_RTCP);
								cReq.SetPreviousAddr(*itrNatAddr);
								cReq.SetPresentAddr(tNatPacket.m_dwNatIp,tNatPacket.m_wNatPort);

								PostInDlgReq<CUpdateSwitchAddressReq>(KDSIP_EVENT_MESSAGE_DIALOG_REQ, cReq, m_tDialogID);

								itrNatAddr->SetNetIpv4(tNatPacket.m_dwNatIp);
								itrNatAddr->SetNetPort(tNatPacket.m_wNatPort);
							}
						}
					}
					else
					{
						TASKLOG(NORMAL_TASK, ERROR_LEV, "音频呼叫不支持的媒体类型[%s]\n",
							itrTransChn->GetTransChnType().c_str());
					}
				}
			}
		}
	} 
	else//新流程
	{
		for(TMediaDescList::iterator itrMediaDesc = tMediaDescList.begin(); 
			itrMediaDesc!=tMediaDescList.end(); itrMediaDesc++)
		{
			if (MEDIA_TYPE_AUDIO == itrMediaDesc->GetMediaType()
				&& (NAT_PACK_TYPE_AUDIO_RTP == tNatPacket.m_byReserve || NAT_PACK_TYPE_AUDIO_RTCP == tNatPacket.m_byReserve))
			{
				TMediaTransChannelList& tMediaTransChnList = itrMediaDesc->GetMediaTransChnList();
				for(TMediaTransChannelList::iterator itrTransChn = tMediaTransChnList.begin(); 
					itrTransChn!=tMediaTransChnList.end(); itrTransChn++)
				{
					if (MEDIA_TRANS_CHN_RTP == itrTransChn->GetTransChnType() && NAT_PACK_TYPE_AUDIO_RTP == tNatPacket.m_byReserve)
					{
						TNetAddrList& tNatAddrList = itrTransChn->GetAddrList();
						for (TNetAddrList::iterator itrNatAddr = tNatAddrList.begin();
							itrNatAddr != tNatAddrList.end();itrNatAddr++)
						{
							if (itrNatAddr->GetNetIpv4() != tNatPacket.m_dwNatIp || itrNatAddr->GetNetPort() != tNatPacket.m_wNatPort)
							{
								CUpdateSwitchAddressReq cReq;
								cReq.SetMediaType(itrMediaDesc->GetMediaType());
								cReq.SetTransChannel(MEDIA_TRANS_CHN_RTP);
								cReq.SetPreviousAddr(*itrNatAddr);
								cReq.SetPresentAddr(tNatPacket.m_dwNatIp,tNatPacket.m_wNatPort);

								PostInDlgReq<CUpdateSwitchAddressReq>(KDSIP_EVENT_MESSAGE_DIALOG_REQ, cReq, m_tDialogID);

								TASKLOG(NORMAL_TASK, EVENT_LEV, "Bef LocalAddr[%u:%d], NatAddr[%u:%d], NatPacketNatAddr[%u:%d]\n",
									tNatPacket.m_dwNatIp, tNatPacket.m_wSrcPort,
									itrNatAddr->GetNetIpv4(), itrNatAddr->GetNetPort(),
									tNatPacket.m_dwNatIp, tNatPacket.m_wNatPort);
								itrNatAddr->SetNetIpv4(tNatPacket.m_dwNatIp);
								itrNatAddr->SetNetPort(tNatPacket.m_wNatPort);
								TASKLOG(NORMAL_TASK, EVENT_LEV, "Aft NatAddr[%u:%d]\n",
									itrNatAddr->GetNetIpv4(), itrNatAddr->GetNetPort());
							}
						}
					}
					else if (MEDIA_TRANS_CHN_RTCP == itrTransChn->GetTransChnType() && NAT_PACK_TYPE_AUDIO_RTCP == tNatPacket.m_byReserve)
					{
						TNetAddrList& tNatAddrList = itrTransChn->GetAddrList();
						for (TNetAddrList::iterator itrNatAddr = tNatAddrList.begin();
							itrNatAddr != tNatAddrList.end();itrNatAddr++)
						{
							if (itrNatAddr->GetNetIpv4() != tNatPacket.m_dwNatIp || itrNatAddr->GetNetPort() != tNatPacket.m_wNatPort)
							{
								CUpdateSwitchAddressReq cReq;
								cReq.SetMediaType(itrMediaDesc->GetMediaType());
								cReq.SetTransChannel(MEDIA_TRANS_CHN_RTCP);
								cReq.SetPreviousAddr(*itrNatAddr);
								cReq.SetPresentAddr(tNatPacket.m_dwNatIp,tNatPacket.m_wNatPort);

								PostInDlgReq<CUpdateSwitchAddressReq>(KDSIP_EVENT_MESSAGE_DIALOG_REQ, cReq, m_tDialogID);

								TASKLOG(NORMAL_TASK, EVENT_LEV, "Bef LocalAddr[%u:%d], NatAddr[%u:%d], NatPacketNatAddr[%u:%d]\n",
									tNatPacket.m_dwNatIp, tNatPacket.m_wSrcPort,
									itrNatAddr->GetNetIpv4(), itrNatAddr->GetNetPort(),
									tNatPacket.m_dwNatIp, tNatPacket.m_wNatPort);
								itrNatAddr->SetNetIpv4(tNatPacket.m_dwNatIp);
								itrNatAddr->SetNetPort(tNatPacket.m_wNatPort);
								TASKLOG(NORMAL_TASK, EVENT_LEV, "Aft NatAddr[%u:%d]\n",
									itrNatAddr->GetNetIpv4(), itrNatAddr->GetNetPort());
							}
						}
					}
				}
			}
		}
	}	

	m_strCallPuReq = tReq.ToXml();
}

/////////////////////////////////////////////////////////////////////////
//第三方呼叫task
/////////////////////////////////////////////////////////////////////////
CCallPuTask::CCallPuTask(CInstExt *pcInst, const string &strCuSession, CSipTask *pcTask) : CSipTask(pcInst)//, m_cStrmNumRestorer(pcInst)
{
    CMS_ASSERT(!strCuSession.empty());
    CMS_ASSERT(NULL != pcTask);

    m_pcCallPuInterActTask = pcTask;
    m_strCuSession = strCuSession;
    m_tDialogID = INVALID_DIALOG_ID;

    //m_cStrmNumRestorer.SetUserLoginSession( strCuSession );
    m_dwErrorCode = CMS_SUCCESS;
}

CCallPuTask::~CCallPuTask()
{
    TerminateTask();
    //m_tPuChnIDList.clear();
    if (!m_strModuleId.empty())
	{
		CRedisSsChannel cSsChn = g_cCuiRedisSdk.GetModuleSsChannelById(m_strModuleId);
		CRedisSsMng::GetInstance()->DelSs(cSsChn, GetTaskNO());
	}
}

void CCallPuTask::TerminateTask()
{
    if ( m_tDialogID != INVALID_DIALOG_ID )
    { //释放会话
		PostInDlgReq(KDSIP_EVENT_INVITE_BYE_REQ, CByeReq(), m_tDialogID);
        //OspSipReleaseDlg(m_tDialogID);
        m_tDialogID = INVALID_DIALOG_ID;
    }

    //注销CuAgent中的浏览事务
    if ( INVALID_PLAY_ID != m_tCuChn.GetChnNO() )
    {
        //m_pcCuAgent->m_cInviteTaskSet.Erase(m_tCuChn.GetChnNO());
        CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession, GetInstance());
        //当调用CuAgent析构后，Invite等待bye响应时，CuAgent已经被干掉了，所以去掉CMS_ASSERT
        //CMS_ASSERT(pcCuAgent != NULL);
        if(NULL != pcCuAgent)
        {
            pcCuAgent->m_cCallPuTaskSet.Erase(m_tCuChn.GetChnNO());
        }
        m_tCuChn.SetChnNO(INVALID_PLAY_ID);
    }

    m_pcCallPuInterActTask = NULL;
}

bool CCallPuTask::IsNotNeedPollState()
{
    if (GetState() == INVITE_HOLD_STATE)
    {
        return true;
    }

    return false;
}

void CCallPuTask::InitStateMachine()
{
    CStateProc cWaitSetUpReqProc;
    cWaitSetUpReqProc.ProcMsg = (CSipTask::PFProcMsg)&CCallPuTask::WaitSetUpReqProcMsg;
    cWaitSetUpReqProc.TimerPoll = (CSipTask::PFTimerPoll)&CCallPuTask::WaitSetUpReqTimerPoll;
    cWaitSetUpReqProc.ProcErrMsg = (CSipTask::PFProcMsg)&CCallPuTask::WaitSetUpReqProcMsg;
    AddRuleProc(WAIT_SETUP_REQ_STATE, cWaitSetUpReqProc);

    CStateProc cWaitAckState;
    cWaitAckState.ProcMsg = (CSipTask::PFProcMsg)&CCallPuTask::WaitAckProcMsg;
    cWaitAckState.TimerPoll = (CSipTask::PFTimerPoll)&CCallPuTask::WaitAckTimerPoll;
    cWaitAckState.ProcErrMsg = (CSipTask::PFProcMsg)&CCallPuTask::WaitAckProcMsg;
    AddRuleProc(WAIT_ACK_STATE, cWaitAckState);

    CStateProc cInviteHoldState;
    cInviteHoldState.ProcMsg = (CSipTask::PFProcMsg)&CCallPuTask::InviteHoldProcMsg;
    cInviteHoldState.TimerPoll = &CSipTask::TimerPollSkip;
    cInviteHoldState.ProcErrMsg = (CSipTask::PFProcMsg)&CCallPuTask::InviteHoldProcMsg;
    AddRuleProc(INVITE_HOLD_STATE, cInviteHoldState);

    NextState(WAIT_SETUP_REQ_STATE);
}

u32 CCallPuTask::WaitSetUpReqProcMsg(CMessage *const pcMsg)
{
    TASKLOG(NORMAL_TASK, EVENT_LEV, "CCallPuTask收到CU用户会话[%s]的请求[%s--%hu]\n",
        m_strCuSession.c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

    u32 dwRet = PROCMSG_OK;

    switch(pcMsg->event)
    {
    case INVITE_REQ:
        {
            COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
            CMS_ASSERT(pcOspSipMsg && "ospsip 消息为空\n");

            CInviteReq cReq;
            pcOspSipMsg->GetMsgBody(cReq);

            m_strReq = cReq.ToXml();
            
            m_tDialogID = pcOspSipMsg->GetSipDlgID();
            //1 鉴权
            bool bAuthRet = true; //鉴权结果
            CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession, GetInstance());
            CMS_ASSERT(NULL != pcCuAgent);
            if(NULL == pcCuAgent)
            {
                CInviteRsp cRsp;
                cRsp.SetErrorCode(ERR_CUI_CU_NOT_ONLINE);
                cRsp.SetSeqNum(cReq.GetSeqNum());
                cRsp.SetSession(cReq.GetSession());

                PostRsp<CInviteRsp>(KDSIP_EVENT_INVITE_RSP, pcOspSipMsg->GetSipTransID(), cRsp);
                TASKLOG(NORMAL_TASK, ERROR_LEV, "reply ERR_CUI_CU_NOT_ONLINE\n");
                //delete this;
                return PROCMSG_DEL;
            }

            //if( !m_cStrmNumRestorer.Start() )
            //{
            //    TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户会话[%s] 已达到最大并发数，本次请求被拒绝\n", m_strCuSession.c_str() );

            //    m_dwErrorCode = ERR_CUI_USER_EXCEEDED_MAX_STREAM;
            //    CInviteRsp cRsp;
            //    cRsp.SetErrorCode( GetErrorCode() );
            //    cRsp.SetSeqNum(cReq.GetSeqNum());
            //    cRsp.SetSession(cReq.GetSession());

            //    return PROCMSG_DEL;
            //}

            do 
            {                             
                string strDomainName = GetDomainNameWithoutPreNameFromURI(cReq.GetSrcChn().GetDevUri());
                if ( ! pcCuAgent->IsExistDomainInfo(strDomainName))
                {
                    bAuthRet = false;
                    break;
                }

				if (pcCuAgent->m_pcUserAccount->IsSuperAdmin())
                {
                    bAuthRet = true;
                    break;
                }

                CPrivilege cPri = pcCuAgent->m_pcUserAccount->GetUserPri();
                if(!cPri.IsEmpty())
                {
                    if(cPri.HasPri(CUserPrivilege::en_Call_Ctrl))
                    {
                        bAuthRet = true;
                        break;
                    }
                    else
                    {
                        bAuthRet = false;
                    }
                }
                else
                {
                    bAuthRet = false;
                    break;
                }

                string uuid = GetNameWithoutDomainFromURI(cReq.GetDstChn().GetDevUri());
				if (!g_cRedisDb.IsUserHasDev(pcCuAgent->m_pcUserAccount->m_tUserID, uuid))
				{
					TASKLOG(NORMAL_TASK, ERROR_LEV, "pcDevInfo is null:uuid  is [%s]\n", uuid.c_str());
					bAuthRet = false;
					break;
				}
            } while (0);

            if (!bAuthRet)
            {
                TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户会话[%s]发送Invite请求，但是对源通道[%s--%d]无权限\n",
                    m_strCuSession.c_str(),
                    cReq.GetSrcChn().GetDevUri().c_str(), cReq.GetSrcChn().GetChnNO());

                CInviteRsp cRsp;
                cRsp.SetErrorCode(ERR_CUI_CU_NO_PERMISSION);
                cRsp.SetSeqNum(cReq.GetSeqNum());
                cRsp.SetSession(cReq.GetSession());

                PostRsp<CInviteRsp>(KDSIP_EVENT_INVITE_RSP, pcOspSipMsg->GetSipTransID(), cRsp);
                //delete this;
                return PROCMSG_DEL;
            }

            if(INVALID_TRANS_ID ==  m_tDialogID )
            {
                TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户会话[%s]发送Invite请求，但是TransID无效,DevUri[%s],SrcChnNo[%d]\n",
                    m_strCuSession.c_str(),
                    cReq.GetSrcChn().GetDevUri().c_str(), cReq.GetSrcChn().GetChnNO());

                CInviteRsp cRsp;
                cRsp.SetErrorCode(ERR_CUI_INVALID_PARAM);
                cRsp.SetSeqNum(cReq.GetSeqNum());
                cRsp.SetSession(cReq.GetSession());

                PostRsp<CInviteRsp>(KDSIP_EVENT_INVITE_RSP, pcOspSipMsg->GetSipTransID(), cRsp);
                //delete this;
                dwRet = PROCMSG_DEL;
            }

            //m_tPuChnIDList.push_back(cReq.GetDstChn());

            CCallPuInterAction *pcCallPuInterActTask= (CCallPuInterAction *)m_pcCallPuInterActTask;
            CMS_ASSERT(NULL != pcCallPuInterActTask);
            if(pcCallPuInterActTask != NULL)
            {
                CCuStartThirdPartyCallReq cCuStartThirdPartyCallReq;
                cCuStartThirdPartyCallReq.FromXml(pcCallPuInterActTask->m_strSrcReq);

                m_tCuChn = cCuStartThirdPartyCallReq.GetSrcChn();               

                CInviteRsp cRsp;

                cRsp.SetSeqNum(cReq.GetSeqNum());
                cRsp.SetSession(cReq.GetSession());
                cRsp.GetSdp().SetOwner(m_tCuChn);
                cRsp.GetSdp().SetSessionName(cReq.GetSdp().GetSessionName());
                cRsp.GetSdp().SetMediaDescList(cCuStartThirdPartyCallReq.GetSdp().GetMediaDescList());
                cRsp.GetSdp().CopyMediaChnKeyFrom(cReq.GetSdp());

                PostRsp<CInviteRsp>(KDSIP_EVENT_INVITE_RSP, pcOspSipMsg->GetSipTransID(), cRsp);

                
                m_strModuleId = pcOspSipMsg->GetSipFromUri().GetUser();
                CRedisSsChannel cSsChn = g_cCuiRedisSdk.GetModuleSsChannelById(m_strModuleId);
		        CRedisSsMng::GetInstance()->AddSs(cSsChn, GetTaskNO());

                NextState(WAIT_ACK_STATE);
            }
            else
            {
                dwRet = PROCMSG_DEL;
            }
        }
        break;
    default:
        {
            dwRet = PROCMSG_FAIL;
            TASKLOG(NORMAL_TASK, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }
    return dwRet;
}

u32 CCallPuTask::WaitSetUpReqTimerPoll()
{
    if ( GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout() )
    {
        TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户会话[%s]等待Invite请求超时\n",
            m_strCuSession.c_str());

        return TIMERPOLL_DEL;
    }
    return TIMERPOLL_DONE;
}

u32 CCallPuTask::WaitAckProcMsg(CMessage *const pcMsg)
{
    TASKLOG(NORMAL_TASK, EVENT_LEV, "收到CU用户会话[%s]的消息[%s--%hu]\n",
        m_strCuSession.c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

    u32 dwRet = PROCMSG_OK;

    switch(pcMsg->event)
    {
    case INVITE_ACK:
        {
            //ACK无失败的时候,直接状态切换
            NextState(INVITE_HOLD_STATE);
        }
        break;
    case OSP_SIP_MSG_PROC_FAIL:
    case OSP_SIP_DIALOG_TERMINATE:
        {
            COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
            TASKLOG(NORMAL_TASK, EVENT_LEV, "CCallPuTask[TaskNO %u]收到OSP_SIP_MGS_PROC_FAIL，DialogID[%u]，自动回复应答\n",
                GetTaskNO(), pcOspSipMsg->GetSipDlgID() );

            CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession, GetInstance());
            CMS_ASSERT(pcCuAgent != NULL);
            if(pcCuAgent != NULL)
            {
                CInviteStopNotify cInviteStopNtf;
                cInviteStopNtf.playId = m_tCuChn.GetChnNO();
                cInviteStopNtf.errorCode = ERR_CUI_DIALOG_TERMINATED;

                pcCuAgent->m_cNotifyPool.InsertInviteStopNotify(cInviteStopNtf);

                string strNullMsg;
                PostRsp(KDSIP_EVENT_INVITE_BYE_RSP, pcOspSipMsg->GetSipTransID(), strNullMsg);
                m_tDialogID = INVALID_DIALOG_ID;
            }           

            NextState(WAIT_SETUP_REQ_STATE);
            delete this;
            dwRet =  PROCMSG_DEL;
        }
        break;
    default:
        {
            dwRet = PROCMSG_FAIL;
            TASKLOG(NORMAL_TASK, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }
    return PROCMSG_OK;
}

u32 CCallPuTask::WaitAckTimerPoll()
{
    if ( GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout() )
    {
        TASKLOG(NORMAL_TASK, ERROR_LEV, "CU用户会话[%s]等待Invite请求超时\n",
            m_strCuSession.c_str());

        return TIMERPOLL_DEL;
    }
    return TIMERPOLL_DONE;
}

u32 CCallPuTask::InviteHoldProcMsg(CMessage *const pcMsg)
{
    TASKLOG(NORMAL_TASK, EVENT_LEV, "收到CU用户会话[%s]的请求[%s--%hu]\n",
        m_strCuSession.c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

    u32 dwRet = PROCMSG_OK;
    switch ( pcMsg->event )
    {
    case OSP_SIP_MSG_PROC_FAIL:
    case OSP_SIP_DIALOG_TERMINATE:
        {
            COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
            TASKLOG(NORMAL_TASK, EVENT_LEV, "CCallPuTask[TaskNO %u]收到OSP_SIP_MGS_PROC_FAIL，DialogID[%u]，自动回复应答\n",
                GetTaskNO(), pcOspSipMsg->GetSipDlgID() );

            CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession, GetInstance());
            CMS_ASSERT(pcCuAgent != NULL);
            if(pcCuAgent != NULL)
            {
                CInviteStopNotify cInviteStopNtf;
                cInviteStopNtf.playId = m_tCuChn.GetChnNO();
                cInviteStopNtf.errorCode = ERR_CUI_DIALOG_TERMINATED;

                pcCuAgent->m_cNotifyPool.InsertInviteStopNotify(cInviteStopNtf);
                string strNullMsg;

                m_tDialogID = INVALID_DIALOG_ID;
            }           

            NextState(WAIT_SETUP_REQ_STATE);
            delete this;
            dwRet =  PROCMSG_DEL;
        }
        break;
    case OSP_SIP_BYE_REQ:
        {
            COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
            TASKLOG(NORMAL_TASK, EVENT_LEV, "CCallPuTask[TaskNO %u]收到OSP_SIP_MGS_PROC_FAIL，DialogID[%u]，自动回复应答\n",
                GetTaskNO(), pcOspSipMsg->GetSipDlgID() );

            CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession, GetInstance());
            CMS_ASSERT(pcCuAgent != NULL);
            if(pcCuAgent != NULL)
            {                
                CInviteStopNotify cInviteStopNtf;
                cInviteStopNtf.playId = m_tCuChn.GetChnNO();

                if(pcOspSipMsg->GetMsgBodyLen() > 0)
                {
                    CByeReq cByeReq;
                    pcOspSipMsg->GetMsgBody(cByeReq);

                    cInviteStopNtf.errorCode = cByeReq.GetReason();
                }
                else
                {
                    cInviteStopNtf.errorCode = ERR_CUI_DIALOG_TERMINATED;
                }

                pcCuAgent->m_cNotifyPool.InsertInviteStopNotify(cInviteStopNtf);
                string strNullMsg;

                PostRsp(KDSIP_EVENT_INVITE_BYE_RSP, pcOspSipMsg->GetSipTransID(), strNullMsg);
                m_tDialogID = INVALID_DIALOG_ID;
            }           

            NextState(WAIT_SETUP_REQ_STATE);
            delete this;
            dwRet =  PROCMSG_DEL;
        }
        break;
    default:
        {
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"收到未知消息");
        }
        break;
    }
    return dwRet;
}

CUsrOprLogTask::CUsrOprLogTask(CInstExt *pcInst, const string& strCuSession, const string& strPeUri,
    const int operateType,const u32 operateResult, const string& operateDesc, const string& strDevUri) : CCallBackTask(pcInst, strCuSession, strPeUri)
{
    SetLogInfo(operateType, operateResult, operateDesc, strDevUri);
    m_nTrySendNum = 0;
}

CUsrOprLogTask::~CUsrOprLogTask()
{
    TerminateTask();
}

void CUsrOprLogTask::TerminateTask()
{
    return;
}

string CUsrOprLogTask::GetPeSession()
{
    CCuiInst *pcInst = (CCuiInst *)GetInstance();
    return pcInst->IsProvideServer(m_tPeUri, GetTaskNO());
}

void CUsrOprLogTask::InitStateMachine()
{
    CStateProc cReqProc;
    cReqProc.ProcMsg = (CSipTask::PFProcMsg)&CUsrOprLogTask::ReqProcMsg;
    cReqProc.TimerPoll = (CSipTask::PFTimerPoll)&CUsrOprLogTask::ReqTimerPoll;
    cReqProc.ProcErrMsg = (CSipTask::PFProcMsg)&CUsrOprLogTask::ReqProcMsg;
    AddRuleProc(Req, cReqProc);

    CStateProc cWaitRspProc;
    cWaitRspProc.ProcMsg = (CSipTask::PFProcMsg)&CUsrOprLogTask::WaitRspProcMsg;
    cWaitRspProc.TimerPoll = (CSipTask::PFTimerPoll)&CUsrOprLogTask::WaitRspTimerPoll;
    cWaitRspProc.ProcErrMsg = (CSipTask::PFProcMsg)&CUsrOprLogTask::WaitRspProcMsg;
    AddRuleProc(WaitRsp, cWaitRspProc);

    NextState(Req);
}

u32 CUsrOprLogTask::ReqProcMsg(CMessage *const pcMsg)
{
    //SIP层错误处理
    if(pcMsg->event == OSP_SIP_MSG_PROC_FAIL)
    {
        TASKLOG(NORMAL_TASK, ERROR_LEV, "[CUsrOprLogTask::ReqProcMsg]在rsp状态收到错误消息[%s--%hu]\n",
            OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

        return PROCMSG_FAIL;
    }

    TASKLOG(NORMAL_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
        OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

    CMS_ASSERT(0&&"ReqProcMsg收到未知消息");
    return PROCMSG_OK;
}

u32 CUsrOprLogTask::ReqTimerPoll()
{
    if ( GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout() )
    {
        TASKLOG(NORMAL_TASK, ERROR_LEV, "[CUsrOprLogTask::ReqTimerPoll]发送日志请求超时[TaskNO %u]\n",
            GetTaskNO());
        if(m_nTrySendNum >= NTF_TRY_SEND_NUM)
        {
            return TIMERPOLL_DEL;
        }

        StartSendLog();       
    }

    return TIMERPOLL_DONE;
}

u32 CUsrOprLogTask::WaitRspProcMsg( CMessage *const pcMsg )
{
    COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
    TASKLOG(NORMAL_TASK, EVENT_LEV, "CUsrOprLogTask[TaskNO %u]收到服务器[%s]的应答消息[%s--%hu]\n",
        GetTaskNO(), pcOspSipMsg->GetSipToUri().GetURIString().c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

    u32 dwRet = PROCMSG_FAIL;
    switch (pcMsg->event)
    {
    case OSP_SIP_MSG_PROC_FAIL:
        {
            TASKLOG(NORMAL_TASK, EVENT_LEV, "收到[OSP_SIP_MSG_PROC_FAIL]消息\n");
            NextState(Req);
            dwRet = PROCMSG_FAIL;
        }
        break;
    case OSP_SIP_DIALOG_TERMINATE:
        {
            CMS_ASSERT( 0 && "CQueryTask::WaitRspProcMsg不可能出现会话内消息" );
            dwRet = PROCMSG_FAIL;
        }
        break;
    case UAS_OPERATE_LOG_ADD_RSP:
        {
            COperateLogAddRsp cRsp;
            pcOspSipMsg->GetMsgBody(cRsp);

            if(CMS_SUCCESS == cRsp.GetErrorCode())
            {
                TASKLOG(NORMAL_TASK, EVENT_LEV, "向uas写日志成功 Msg[%s-%d]\n", 
                    OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            }
            else
            {
                TASKLOG(NORMAL_TASK, EVENT_LEV, "向uas写日志失败 Msg[%s-%d]\n", 
                    OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            }

            dwRet = PROCMSG_DEL;
        }
        break;
    default:
        {
            TASKLOG(NORMAL_TASK, CRITICAL_LEV, "收到未知消息[%s--%hu]\n",
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            CMS_ASSERT(0&&"收到未知消息");
        }
        break; 
    }

    if(dwRet = PROCMSG_DEL)
    {
        delete this;
    }

    return dwRet;
}

u32 CUsrOprLogTask::StartSendLog()
{
    if (!g_cCuiConfig.IsCuOperateLogEnable(m_tLogInfo.operateType))
    {
        delete this;
        return PROCMSG_OK;
    }

    CMS_ASSERT( Req == GetState() );

    TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户会话[%s]发送记录日志请求给UAS[%s]\n",
        m_strCuSession.c_str(),
        m_tPeUri.GetURIString().c_str());

    u32 dwRet = PROCMSG_OK;
    CCuiInst *pcInst = (CCuiInst *)GetInstance();
    m_strPeSession = pcInst->IsProvideServer(m_tPeUri, GetTaskNO());

    if(!m_strPeSession.empty())
    {
        CPeAgent *pcPeAgent = (CPeAgent *)g_cCuiDataMgr.GetPeAgentPtrByPeName(m_tPeUri.GetURIString(), GetInstance());
        CMS_ASSERT(NULL != pcPeAgent);
        if(pcPeAgent != NULL)
        {
            COperateLogAddReq cReq;
            cReq.SetSession(pcPeAgent->m_strSessionID);
            cReq.SetInfo(m_tLogInfo);
            dwRet = PostMsgReq<COperateLogAddReq>(cReq, m_tPeUri);
            if(dwRet != PROCMSG_OK)
            {
                return dwRet;
            }

            NextState(WaitRsp);
        } 
    }

    return dwRet;
}

u32 CUsrOprLogTask::StartSendReq()
{
    u32 dwRet = PROCMSG_OK;
    m_nTrySendNum++;

    CPeAgent *pcPeAgent = (CPeAgent *)g_cCuiDataMgr.GetPeAgentPtrByPeName(m_tPeUri.GetURIString(), GetInstance());
    CMS_ASSERT(pcPeAgent != NULL);

    //如果当前CPeAgent不在服务态，则直接删除该task
    if(CPeAgent::Service != pcPeAgent->GetState())
    {
        TASKLOG(NORMAL_TASK, CRITICAL_LEV, "CPeAgent[%s]不在服务态\n",
            m_tPeUri.GetURIString().c_str());

        return PROCMSG_DEL; 
    }

    if( pcPeAgent )
    {
        COperateLogAddReq cReq;
        cReq.SetSession(pcPeAgent->m_strSessionID);
        cReq.SetInfo(m_tLogInfo);

        dwRet = PostMsgReq<COperateLogAddReq>(cReq, m_tPeUri);
        if(dwRet != PROCMSG_OK)
        {
            return dwRet;
        }

        NextState(WaitRsp);
    }

    return dwRet;
}

u32 CUsrOprLogTask::WaitRspTimerPoll()
{
    if ( GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout() )
    {
        TASKLOG(NORMAL_TASK, ERROR_LEV, "[CUsrOprLogTask::WaitRspTimerPoll]发送日志请求超时[TaskNO %u]\n",
            GetTaskNO());

        return TIMERPOLL_DEL;   
    }

    return TIMERPOLL_DONE;
}

void CUsrOprLogTask::SetLogInfo(const int operateType,const u32 operateResult, const string& operateDesc, const string& strDevUri)
{
    m_tLogInfo.logTag = m_strCuSession;
    CCuAgent *pcCuAgent = (CCuAgent *)g_cCuiDataMgr.GetTaskPtrBySession(m_strCuSession, GetInstance());
    //登陆失败没有用户信息
    //CMS_ASSERT(pcCuAgent != NULL);
    if ( NULL != pcCuAgent && CCuAgent::ONLINE_STATE  == pcCuAgent->GetState())
    {
        m_tLogInfo.userUri = pcCuAgent->m_pcUserAccount->m_tUserID + "@" + GetDomainNameWithoutPreNameFromURI(pcCuAgent->m_str3asName);
        m_tLogInfo.userName = pcCuAgent->m_pcUserAccount->m_strUserName;
        m_tLogInfo.netAddr = pcCuAgent->m_strClientIp;
    }
    else
    {
        m_tLogInfo.userUri = string("@");
        m_tLogInfo.userName = string("Unknown(")+GetLanDesc(LAN_DESC_USER) + GetLanDesc(LAN_DESC_OFFLINE)+string(")");
        m_tLogInfo.netAddr = string("Unknown(")+GetLanDesc(LAN_DESC_USER) + GetLanDesc(LAN_DESC_OFFLINE)+string(")");
    }

    CCMSTime cCurtime;
    m_tLogInfo.happenedTime = cCurtime.ToString();
    m_tLogInfo.operateType = operateType;
    m_tLogInfo.operateResult = operateResult;
    m_tLogInfo.operateDesc = operateDesc;
    m_tLogInfo.devUri = strDevUri;
}


CGetPuData::~CGetPuData()
{
    if(!m_strQueryTag.empty())
    {
        g_cCuiDataMgr.m_mapSessToTaskMap.Erase(m_strQueryTag);
    }
}

string CGetPuData::WriteLogDesc(const string& strOpt, const string& strDevName, const string& strVidSrcAlaias)
{
    string strResult;
    if(CMS_SUCCESS != m_dwErrorCode)
    {
        strResult  = LAN_DESC_FAIL;
    }
    else
    {
        strResult = LAN_DESC_SUCCESS;
    }
    return GetLanDesc(strOpt) + GetLanDesc(strResult) + " "
        + GetLanDesc(LAN_DESC_DEV_NAME) + ":" + strDevName 
        + " " + GetLanDesc(LAN_DESC_VIDSRCALIAS) + ":" + strVidSrcAlaias + " " 
        +  GetLanDesc(LAN_DESC_DEVICE_ID) + ":" + m_tDevChn.GetDevUri() + "_" + TransInt2String(m_tDevChn.GetChnNO()+1);
}
string CGetPuData::WriteLogDesc(const string& strAction, const string& strOpt, const string& strDevName, const string& strVidSrcAlaias)
{
    string strResult;
    if(CMS_SUCCESS != m_dwErrorCode)
    {
        strResult  = LAN_DESC_FAIL;
    }
    else
    {
        strResult = LAN_DESC_SUCCESS;
    }
    return GetLanDesc(strAction) + GetLanDesc(strOpt) + GetLanDesc(strResult) + " " 
        + GetLanDesc(LAN_DESC_DEV_NAME) + ":" + strDevName + " "
        + GetLanDesc(LAN_DESC_VIDSRCALIAS) + ":" + strVidSrcAlaias + " "
        +  GetLanDesc(LAN_DESC_DEVICE_ID) + ":" + m_tDevChn.GetDevUri() + " " 
		+ GetLanDesc(LAN_DESC_DEV_CHN) + ":" + TransInt2String(m_tDevChn.GetChnNO()+1);
}
string CGetPuData::GetLogDesc(const string& strDevName, const string& strVidSrcAlaias)
{
    string strResult;
    if(CMS_SUCCESS != m_dwErrorCode)
    {
        strResult  = LAN_DESC_FAIL;
    }
    else
    {
        strResult = LAN_DESC_SUCCESS;
    }

    switch (m_dwEvent)
    {
    case INVITE_REQ:
        {
            return WriteLogDesc(LAN_DESC_START, LAN_DESC_REAL_MEDIA_INVITE, strDevName, strVidSrcAlaias);
        }
        break;
    case CU_CUI_INVITE_BYE_REQ:
        {
            return WriteLogDesc(LAN_DESC_STOP, LAN_DESC_REAL_MEDIA_INVITE, strDevName, strVidSrcAlaias);
        }
        break;
    case CU_CUI_INVITE_PLAY_PLATFORMRECORD_REQ:
        {
            CInviteReq cReq;
            cReq.FromXml(m_strReq);
            return GetLanDesc(LAN_DESC_START) + GetLanDesc(LAN_DESC_PLATFORM_PLAYBACK) + GetLanDesc(strResult) + " " 
                + GetLanDesc(LAN_DESC_DEV_NAME) + ":" + strDevName 
                + " " + GetLanDesc(LAN_DESC_VIDSRCALIAS) + ":" + strVidSrcAlaias + " " 
                +  GetLanDesc(LAN_DESC_DEVICE_ID) + ":" + m_tDevChn.GetDevUri() + "_" + TransInt2String(m_tDevChn.GetChnNO()+1) + " "
                + GetLanDesc(LAN_DESC_CUR_PLAYTIME) + ":" +cReq.GetSdp().GetTimeRange().GetStartTime();
        }
        break;
    case CU_CUI_INVITE_BYE_PLAY_PLATFORMRECORD_REQ:
        {
            return WriteLogDesc(LAN_DESC_STOP, LAN_DESC_PLATFORM_PLAYBACK, strDevName, strVidSrcAlaias);
        }
        break;
    case CU_CUI_INVITE_PLAY_DOWNLOADPLATFORMRECORD_REQ:
        {
            CInviteReq cReq;
            cReq.FromXml(m_strReq);
            return GetLanDesc(LAN_DESC_START) + GetLanDesc(LAN_DESC_DOWNLOAD_PLATFORMRECORD) + GetLanDesc(strResult) + " " 
                + GetLanDesc(LAN_DESC_DEV_NAME) + ":" + strDevName 
                + " " + GetLanDesc(LAN_DESC_VIDSRCALIAS) + ":" + strVidSrcAlaias + " " 
                +  GetLanDesc(LAN_DESC_DEVICE_ID) + ":" + m_tDevChn.GetDevUri() + "_" + TransInt2String(m_tDevChn.GetChnNO()+1) 
                + GetLanDesc(LAN_DESC_START_PLAYTIME) + ":" + cReq.GetSdp().GetTimeRange().GetStartTime() + " "
                + GetLanDesc(LAN_DESC_STOP_PLAYTIME) + ":" + cReq.GetSdp().GetTimeRange().GetEndTime();
        }
        break;
    case CU_CUI_INVITE_BYE_DOWNLOADPLATFORMRECORD_REQ:
        {
            return WriteLogDesc(LAN_DESC_STOP, LAN_DESC_DOWNLOAD_PLATFORMRECORD, strDevName, strVidSrcAlaias);
        }
        break;
    case CU_CUI_INVITE_PLAY_DOWNLOADPURECORD_REQ:
        {
            CInviteReq cReq;
            cReq.FromXml(m_strReq);
            return GetLanDesc(LAN_DESC_START) + GetLanDesc(LAN_DESC_DOWNLOAD_PURECORD) + GetLanDesc(strResult) + " " 
                + GetLanDesc(LAN_DESC_DEV_NAME) + ":" + strDevName 
                + " " + GetLanDesc(LAN_DESC_VIDSRCALIAS) + ":" + strVidSrcAlaias + " " 
                +  GetLanDesc(LAN_DESC_DEVICE_ID) + ":" + m_tDevChn.GetDevUri() + "_" + TransInt2String(m_tDevChn.GetChnNO()+1) 
                + GetLanDesc(LAN_DESC_START_PLAYTIME) + ":" + cReq.GetSdp().GetTimeRange().GetStartTime() + " "
                + GetLanDesc(LAN_DESC_STOP_PLAYTIME) + ":" + cReq.GetSdp().GetTimeRange().GetEndTime();
        }
        break;
    case CU_CUI_INVITE_STOP_DOWNLOADPURECORD_REQ:
        {
            return WriteLogDesc(LAN_DESC_STOP, LAN_DESC_DOWNLOAD_PURECORD, strDevName, strVidSrcAlaias);
        }
        break;
    case CU_CUI_INVITE_PLAY_PURECORD_REQ:
        {
            CInviteReq cReq;
            cReq.FromXml(m_strReq);
            return GetLanDesc(LAN_DESC_START) + GetLanDesc(LAN_DESC_PU_PLAYBACK) + GetLanDesc(strResult) + " " 
                + GetLanDesc(LAN_DESC_DEV_NAME) + ":" + strDevName 
                + " " + GetLanDesc(LAN_DESC_VIDSRCALIAS) + ":" + strVidSrcAlaias + " " 
                +  GetLanDesc(LAN_DESC_DEVICE_ID) + ":" + m_tDevChn.GetDevUri() + "_" + TransInt2String(m_tDevChn.GetChnNO()+1) 
                + GetLanDesc(LAN_DESC_START_PLAYTIME) + ":" + cReq.GetSdp().GetTimeRange().GetStartTime() + " "
                + GetLanDesc(LAN_DESC_STOP_PLAYTIME) + ":" + cReq.GetSdp().GetTimeRange().GetEndTime();
        }
        break;
    case CU_CUI_INVITE_STOP_PURECORD_REQ:
        {
            return WriteLogDesc(LAN_DESC_STOP, LAN_DESC_PU_PLAYBACK, strDevName, strVidSrcAlaias);
        }
        break;
    case PU_PTZ_CTRL_REQ:
        {
            return WriteLogDesc(LAN_DESC_PTZ_CTRL, strDevName, strVidSrcAlaias);
        }
        break;
	case TAS_PTZLOCK_ADD_REQ:
		{
			CCuPtzLockAddReq cReq;
			cReq.FromXml(m_strReq);
			return GetLanDesc(LAN_DESC_PTZ_LOCK) + GetLanDesc(strResult) + " "
				+ GetLanDesc(LAN_DESC_DEV_NAME) + ":" + strDevName
				+ " " + GetLanDesc(LAN_DESC_VIDSRCALIAS) + ":" + strVidSrcAlaias + " "
				+ GetLanDesc(LAN_DESC_DEVICE_ID) + ":" + m_tDevChn.GetDevUri() + "_" + TransInt2String(m_tDevChn.GetChnNO() + 1);
		}
		break;
	case TAS_PTZLOCK_DEL_REQ:
		{
			CCuPtzLockDelReq cReq;
			cReq.FromXml(m_strReq);

			return GetLanDesc(LAN_DESC_PTZ_UNLOCK) + GetLanDesc(strResult) + " "
				+ GetLanDesc(LAN_DESC_DEV_NAME) + ":" + strDevName
				+ " " + GetLanDesc(LAN_DESC_VIDSRCALIAS) + ":" + strVidSrcAlaias + " "
				+ GetLanDesc(LAN_DESC_DEVICE_ID) + ":" + m_tDevChn.GetDevUri() + "_" + TransInt2String(m_tDevChn.GetChnNO() + 1);
		}
		break;
    case START_THIRD_PARTY_CALL_REQ:
        {
            return WriteLogDesc(LAN_DESC_START, LAN_DESC_THIRD_PARTY_CALL, strDevName, strVidSrcAlaias);
        }
        break;
    case STOP_THIRD_PARTY_CALL_REQ:
        {
            return WriteLogDesc(LAN_DESC_STOP, LAN_DESC_THIRD_PARTY_CALL, strDevName, strVidSrcAlaias);
        }
        break;
	case RCS_RECORD_LOCK_OPR_REQ:
		{
			CCuRcsRecordLockOprReq cReq;
			cReq.FromXml(m_strReq);

			return WriteLogDesc(LAN_DESC_START, LAN_DESC_LOCK, strDevName, strVidSrcAlaias);
		}
		break;
    case RCS_SET_RECORD_TASK_REQ:
        {
            CCuRcsSetRecordTaskReq cReq;
            cReq.FromXml(m_strReq);
            if(cReq.GetIsStart())
            {
                return WriteLogDesc(LAN_DESC_START, LAN_DESC_PLAT_REC, strDevName, strVidSrcAlaias);
            }
            else
            {
                return WriteLogDesc(LAN_DESC_STOP, LAN_DESC_PLAT_REC, strDevName, strVidSrcAlaias);
            } 
        }
        break;
	case PU_RECORD_START_REQ:
		{
			return WriteLogDesc(LAN_DESC_START, LAN_DESC_PU_REC, strDevName, strVidSrcAlaias);
		}
		break;
	case PU_RECORD_STOP_REQ:
		{
			  return WriteLogDesc(LAN_DESC_STOP, LAN_DESC_PU_REC, strDevName, strVidSrcAlaias);
		}
		break;
    case RCS_BINDING_PUCHN_REQ:
        {
            CCuRcsBindingPuChnReq cReq;
            cReq.FromXml(m_strReq);


            return GetLanDesc(LAN_DESC_PUCHN_BINDING) + GetLanDesc(strResult) + " "
                + GetLanDesc(LAN_DESC_DEV_NAME) + ":" + strDevName 
                + " " + GetLanDesc(LAN_DESC_VIDSRCALIAS) + ":" + strVidSrcAlaias
                + " "+ GetLanDesc(LAN_DESC_NRU_ID) + ":"
                + cReq.GetPuChnBinding().GetNruID() + " " 
                + GetLanDesc(LAN_DESC_DISK_MOUNT_PATH) + ":"
                + cReq.GetPuChnBinding().GetDiskMountPath();
        }
        break;
    case RCS_UNBINDING_PUCHN_REQ:
        {
            CCuRcsUnbindingPuChnReq cReq;
            cReq.FromXml(m_strReq);

            return GetLanDesc(LAN_DESC_PUCHN_UNBINDING) + GetLanDesc(strResult) + " "
                + GetLanDesc(LAN_DESC_DEV_NAME) + ":" + strDevName 
                + " " + GetLanDesc(LAN_DESC_VIDSRCALIAS) + ":" + strVidSrcAlaias 
                + " " + GetLanDesc(LAN_DESC_NRU_ID) + ":"
                + cReq.GetPuChnBinding().GetNruID() + " " 
                + GetLanDesc(LAN_DESC_DISK_MOUNT_PATH) + ":"
                + cReq.GetPuChnBinding().GetDiskMountPath();
        }
        break;
    default:
        break;
    }

    string strEmpty;
    return strEmpty;
}
u32 CGetPuData::StartWriteLog()
{
	string strErrMsg;
	EFDB_ErrorCode er = g_cRedisDb.GetDevNameAndVSAlaias(GetNameWithoutDomainFromURI(m_tDevChn.GetDevUri()), m_tDevChn.GetChnNO(), m_strDevName, m_strVSname, strErrMsg);
	if ( !IsFdbResultPositive(er) )
	{
		TASKLOG(NORMAL_TASK, ERROR_LEV, "获取设备名称和视频源名称 DevChn[%s]失败! errMsg[%s] er[%d]\n", m_tDevChn.ToString().c_str(), strErrMsg.c_str(), er);
		return PROCMSG_DEL;
	}

	string strLogDesc = GetLogDesc(m_strDevName, m_strVSname);
	WriteLog(strLogDesc);

	return PROCMSG_OK;
}

u32 CGetPuData::WriteLog(const string &strLogDesc)
{
    //当日志为空时，表示用户已经不在线了，无法记录日志
    if(!strLogDesc.empty())
    {
        TOperateType tType;
        switch(m_dwEvent)
        {
        case INVITE_REQ:
			{
				tType = enUserOpTypeLiveStream;
			}
			break;
        case CU_CUI_INVITE_BYE_REQ:
            {
                tType = enUserOpTypeLiveStream;
            }
            break;
        case CU_CUI_INVITE_PLAY_PLATFORMRECORD_REQ:
            {
                tType = enUserOpTypeRecordPlayback;
            }
            break;
        case CU_CUI_INVITE_BYE_PLAY_PLATFORMRECORD_REQ:
            {
                tType = enUserOpTypeRecordPlayback;
            }
            break;
        case CU_CUI_INVITE_PLAY_DOWNLOADPLATFORMRECORD_REQ:
        case CU_CUI_INVITE_BYE_DOWNLOADPLATFORMRECORD_REQ:
            {
                tType = enUserOpTypeRecordDownload;
            }
            break;
        case RCS_SET_RECORD_TASK_REQ:
            {
                CCuRcsSetRecordTaskReq cReq;
                cReq.FromXml(m_strReq);
                if(cReq.GetIsStart())
                {
                    tType = enUserOpTypeRecordStart;
                }
                else
                {
                    tType = enUserOpTypeRecordStop;
                } 
            }
            break;
		case PU_RECORD_START_REQ:
			{
				tType = enUserOpTypeRecordStart;
			}
			break;
		case PU_RECORD_STOP_REQ:
			{
				tType = enUserOpTypeRecordStop;
			}
			break;
        case PU_PTZ_CTRL_REQ:
		case TAS_PTZLOCK_ADD_REQ:
		case TAS_PTZLOCK_DEL_REQ:
            {
                tType = enUserOpTypePtzCtrl;
            }
            break;
        case STOP_THIRD_PARTY_CALL_REQ:
        case START_THIRD_PARTY_CALL_REQ:
            {
                tType = enUserOpTypeCall;
            }
            break;
        case CU_CUI_INVITE_PLAY_DOWNLOADPURECORD_REQ:
        case CU_CUI_INVITE_STOP_DOWNLOADPURECORD_REQ:
            {
                tType = enUserOpTypeRecordDownload;
            }
            break;
        case CU_CUI_INVITE_PLAY_PURECORD_REQ:
        case CU_CUI_INVITE_STOP_PURECORD_REQ:
            {
                tType = enUserOpTypeRecordPlayback;
            }
            break;
        case RCS_BINDING_PUCHN_REQ:
        case RCS_UNBINDING_PUCHN_REQ:
            {
                tType = enUserOpTypeNRUManage;
            }
            break;
        default:
            break;
        }
        CUsrOprLogTask* pcTask = new CUsrOprLogTask(GetInstance(), m_strCuSession, 
            string(DEV_TYPE_UAS) + "@" + GetDomainName(),
            tType,
            (u32)m_dwErrorCode,
            strLogDesc,
            m_tDevChn.GetDevUri()
            );
        pcTask->InitStateMachine();
        return pcTask->StartSendLog();
    }
    return PROCMSG_FAIL;
}
void CGetPuData::InitStateMachine()
{
    CStateProc cGetNameProc;
    cGetNameProc.ProcMsg = (CTask::PFProcMsg)&CGetPuData::OnWaitGetName;
    cGetNameProc.ProcErrMsg = (CTask::PFProcMsg)&CGetPuData::OnWaitGetName;
    cGetNameProc.TimerPoll = (CTask::PFTimerPoll)&CGetPuData::OnWaitGetNameTimer;
    AddRuleProc(WaitGetName, cGetNameProc);
    CStateProc cRspNameProc;
    cRspNameProc.ProcMsg = (CTask::PFProcMsg)&CGetPuData::OnWaitRspName;
    cRspNameProc.ProcErrMsg = (CTask::PFProcMsg)&CGetPuData::OnWaitRspName;
    cRspNameProc.TimerPoll = (CTask::PFTimerPoll)&CGetPuData::OnWaitRspNameTimer;
    AddRuleProc(WaitRspName, cRspNameProc);
    CStateProc cNtfNameProc;
    cNtfNameProc.ProcMsg = (CTask::PFProcMsg)&CGetPuData::OnWaitNtfName;
    cNtfNameProc.ProcErrMsg = (CTask::PFProcMsg)&CGetPuData::OnWaitNtfName;
    cNtfNameProc.TimerPoll = (CTask::PFTimerPoll)&CGetPuData::OnWaitNtfNameTimer;
    AddRuleProc(WaitNtfName, cNtfNameProc);
    CStateProc cGetVidSrcAlias;
    cGetVidSrcAlias.ProcMsg = (CTask::PFProcMsg)&CGetPuData::OnWaitGetVidSrcAlias;
    cGetVidSrcAlias.ProcErrMsg = (CTask::PFProcMsg)&CGetPuData::OnWaitGetVidSrcAlias;
    cGetVidSrcAlias.TimerPoll = (CTask::PFTimerPoll)&CGetPuData::OnWaitGetVidSrcAliasTimer;
    AddRuleProc(WaitGetVidSrcAlias, cGetVidSrcAlias);
    CStateProc cRspVidSrcAlias;
    cRspVidSrcAlias.ProcMsg = (CTask::PFProcMsg)&CGetPuData::OnWaitRspVidSrcAlias;
    cRspVidSrcAlias.ProcErrMsg = (CTask::PFProcMsg)&CGetPuData::OnWaitRspVidSrcAlias;
    cRspVidSrcAlias.TimerPoll = (CTask::PFTimerPoll)&CGetPuData::OnWaitRspVidSrcAliasTimer;
    AddRuleProc(WaitRspVidSrcAlias, cRspVidSrcAlias);
    NextState(WaitGetName);
}
u32 CGetPuData::OnWaitGetName(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_FAIL;
    TASKLOG(NORMAL_TASK, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
    return dwRet;
}
u32 CGetPuData::OnWaitGetNameTimer()//日志里有太多的重新获取打印，延长重新获取间隔
{
    if (GetCurStateHoldTime() > g_cCuiConfig.GetDlgWaitRspTimeout())
    {
        TASKLOG(NORMAL_TASK, ERROR_LEV, "获取失败，重试......\n");
        StartQryName();
    }
    return TIMERPOLL_DONE;
}
u32 CGetPuData::StartQryName()
{
    NextState(WaitGetName);
    u32 dwRet = PROCMSG_FAIL;
    C3acAgent *pc3acAgentTask = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(m_tTasURI.GetURIString(), GetInstance());
    if (NULL == pc3acAgentTask)
    {
        TASKLOG(LOGIN_TASK, ERROR_LEV, "[CGetPuData]3acAgent已经不存在了!\n");       
        //delete this;
        return PROCMSG_DEL;
    }
    CMS_ASSERT( 0 == strcmp(pc3acAgentTask->GetObjName(), "C3acAgent") );
    CDeviceQryReq cReq;
    cReq.SetSession(pc3acAgentTask->m_strSessionID);
    cReq.GetDeviceInfoQueryCondition().SetDeviceUUID(GetNameWithoutDomainFromURI(m_tDevChn.GetDevUri()));
    TASKLOG(LOGIN_TASK, EVENT_LEV, "以会话ID[%s]向TAS[%s]获取设备[%s]信息\n",
        pc3acAgentTask->m_strSessionID.c_str(), pc3acAgentTask->m_t3asURI.GetURIString().c_str(), m_tDevChn.GetDevUri().c_str());
    PostMsgReq(cReq, pc3acAgentTask->m_t3asURI);
    g_cCuiDataMgr.m_mapSessToTaskMap.Insert(cReq.GetQueryTag(), GetTaskNO());
    m_strQueryTag = cReq.GetQueryTag();
    NextState(WaitRspName);
    return dwRet;
}
u32 CGetPuData::StartQryVidSrcAlias()
{
    NextState(WaitGetVidSrcAlias);
    u32 dwRet = PROCMSG_FAIL;
    C3acAgent *pc3acAgentTask = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(m_tTasURI.GetURIString(), GetInstance());
    if (NULL == pc3acAgentTask)
    {
        TASKLOG(LOGIN_TASK, ERROR_LEV, "[CGetPuData]3acAgent已经不存在了!\n");       
        //delete this;
        return PROCMSG_DEL;
    }
    CMS_ASSERT( 0 == strcmp(pc3acAgentTask->GetObjName(), "C3acAgent") );
    CDeviceCapIndexNameGetReq cReq;
    cReq.SetSession(pc3acAgentTask->m_strSessionID);
    cReq.SetDeviceId(GetNameWithoutDomainFromURI(m_tDevChn.GetDevUri()));
    cReq.SetCapId(DEVICE_CAP_VIDEOSOURCE);
    TASKLOG(LOGIN_TASK, EVENT_LEV, "以会话ID[%s]向TAS[%s]获取设备[%s]信息视频源信息\n",
        pc3acAgentTask->m_strSessionID.c_str(), pc3acAgentTask->m_t3asURI.GetURIString().c_str(), m_tDevChn.GetDevUri().c_str());
    PostMsgReq(cReq, pc3acAgentTask->m_t3asURI);
    NextState(WaitRspVidSrcAlias);
    return dwRet;
}

u32 CGetPuData::OnWaitRspName(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_FAIL;
    switch(pcMsg->event)
    {
    case TAS_DEVICE_QRY_RSP:
        {
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            CDeviceQryRsp cRsp;
            pcOspSipMsg->GetMsgBody(cRsp);
            if (cRsp.GetErrorCode() == CMS_SUCCESS)
            {
                TASKLOG(NORMAL_TASK, CRITICAL_LEV, "查询设备名成功!\n");
                if (cRsp.GetTotalEntryNum() == 0)
                {
                    TASKLOG(NORMAL_TASK, WARNING_LEV, "出错,查询设备名为零\n");
                    delete this;
                    dwRet = PROCMSG_DEL;
                }
                else
                {
                    NextState(WaitNtfName);
                    dwRet = PROCMSG_OK;
                }
            }
            else
            {
                TASKLOG(NORMAL_TASK, ERROR_LEV, "查询设备名失败\n");
                delete this;
                dwRet = PROCMSG_DEL;
            }
        }
        break;
    case OSP_SIP_MSG_PROC_FAIL:
        {
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            TASKLOG(NORMAL_TASK, ERROR_LEV, "对设备[%s]请求失败, 发生SIP层错误, 等待定时器重发，sip status code[%d]\n",
                m_tTasURI.GetURIString().c_str(), pcOspSipMsg->GetSipErrorCode());
        }
        break;
    default:
        {
            TASKLOG(NORMAL_TASK, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }
    return dwRet;
}
u32 CGetPuData::OnWaitRspNameTimer()
{
    if ( GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout() )
    {   
        TASKLOG(LOGIN_TASK, ERROR_LEV, "Tas[%s]等待应答消息[TAS_DEVICE_QRY_RSP]超时\n", 
            m_tTasURI.GetURIString().c_str());
        return TIMERPOLL_DEL;
    }
    return TIMERPOLL_DONE;
}
u32 CGetPuData::OnWaitNtfName(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_FAIL;
    switch(pcMsg->event)
    {
    case TAS_DEVICE_QRY_NTF_REQ:
        {
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            CDeviceQryNtfReq cReq;
            pcOspSipMsg->GetMsgBody(cReq);
            CNtfRsp cRsp;
            cRsp.SetHeadFromReq(cReq);
            cRsp.SetErrorCode(CMS_SUCCESS);
            cRsp.SetEvent(TAS_DEVICE_QRY_NTF_RSP);
            PostMsgRsp(pcOspSipMsg->GetSipTransID(), cRsp);
            if( 0 == cReq.GetDeviceInfo().size())
            {
                TASKLOG(NORMAL_TASK, ERROR_LEV, "出错，通知数目为0\n");
                dwRet = PROCMSG_DEL;
                break;
            }
            else
            {
                TASKLOG(NORMAL_TASK, EVENT_LEV, "本次通知的设备数目为[%d]\n", cReq.GetDeviceInfo().size());
            }
            if( 1 != cReq.GetDeviceInfo().size() || !cReq.IsLastNtf())
            {
                TASKLOG(NORMAL_TASK, ERROR_LEV, "tas通知数目[%d]出错\n", cReq.GetDeviceInfo().size());
            }
            CDeviceInfo cDevInfo = *cReq.GetDeviceInfo().begin();  
            StartQryVidSrcAlias();
            dwRet = PROCMSG_OK;
        }
        break;
    default:
        {
            TASKLOG(NORMAL_TASK, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }
    if(PROCMSG_DEL == dwRet)
    {
        delete this;
    }
    return dwRet;
}
u32 CGetPuData::OnWaitNtfNameTimer()
{
    if ( GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout() )
    {   
        TASKLOG(LOGIN_TASK, ERROR_LEV, "Tas[%s]等待应答消息[TAS_DEVICE_QRY_NTF_REQ]超时\n", 
            m_tTasURI.GetURIString().c_str());
        return TIMERPOLL_DEL;
    }
    return TIMERPOLL_DONE;
}
u32 CGetPuData::OnWaitGetVidSrcAlias(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_FAIL;
    TASKLOG(NORMAL_TASK, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
    return dwRet;
}
u32 CGetPuData::OnWaitGetVidSrcAliasTimer()
{
    if (GetCurStateHoldTime() > g_cCuiConfig.GetDlgWaitRspTimeout())//日志里有太多的重新获取打印，延长重新获取间隔
    {
        TASKLOG(NORMAL_TASK, ERROR_LEV, "获取失败，重试......\n");
        StartQryVidSrcAlias();
    }
    return TIMERPOLL_DONE;
}
u32 CGetPuData::OnWaitRspVidSrcAlias(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_FAIL;
    switch(pcMsg->event)
    {
    case TAS_DEVICE_CAP_INDEX_NAME_GET_RSP:
        {
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            CDeviceCapIndexNameGetRsp cRsp;
            pcOspSipMsg->GetMsgBody(cRsp);
            if(cRsp.GetErrorCode() == CMS_SUCCESS)
            {     
                //CPuData* pcPuData = g_cPuList.FindPuData(m_tDevChn.GetDevUri());
                //if(pcPuData != NULL)
                //{
                //    pcPuData->InsertVidSrcAlias(cRsp.GetDeviceCapName());
                //    string strLogDesc = GetLogDesc(pcPuData->GetDevName(), pcPuData->GetVidSrcAlias(m_tDevChn.GetChnNO()));
                //    WriteLog(strLogDesc);
                //}
                //else
                //{
                //    TASKLOG(NORMAL_TASK, ERROR_LEV, "设备[%s]不存在\n", m_tDevChn.GetDevUri().c_str());
                //}
            }
            else
            {
                TASKLOG(NORMAL_TASK, ERROR_LEV, "查询别名列表失败\n");
            }
            dwRet = PROCMSG_DEL;
        }
        break;
    case OSP_SIP_MSG_PROC_FAIL:
        {
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            TASKLOG(NORMAL_TASK, ERROR_LEV, "对设备[%s]请求失败, 发生SIP层错误, 等待定时器重发，sip status code[%d]\n",
                m_tTasURI.GetURIString().c_str(), pcOspSipMsg->GetSipErrorCode());
        }
        break;
    default:
        {
            TASKLOG(NORMAL_TASK, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }
        break;
    }
    if(dwRet == PROCMSG_DEL)
    {
        delete this;
    }
    return dwRet;
}
u32 CGetPuData::OnWaitRspVidSrcAliasTimer()
{
    if ( GetCurStateHoldTime() > g_cCuiConfig.GetSipWaitRspTimeout() )
    {   
        TASKLOG(LOGIN_TASK, ERROR_LEV, "Tas[%s]等待应答消息[TAS_DEVICE_CAP_INDEX_NAME_GET_RSP]超时\n", 
            m_tTasURI.GetURIString().c_str());
        return TIMERPOLL_DEL;
    }
    return TIMERPOLL_DONE;
}


void CCuDPSGetResultRsp::push_back(const CDPSGetResultNtfReq &cNtf, const CQueryTask *pcQueryTask)
{
	CMS_ASSERT(NULL != pcQueryTask);
	CMS_ASSERT(NULL != pcQueryTask->m_pc3acAgent);
	u32 nRecvItemNum = (u32)GetActualResultNum();

	const CDPSSearchResultList &tmpVct = cNtf.Result();

	nRecvItemNum += tmpVct.size();

	resultList.insert(resultList.end(), tmpVct.begin(), tmpVct.end());
	SetActualResultNum(nRecvItemNum);
}

void CCuVidsrcInfoQryRsp::push_back(CVidsrcInfoQryNtfReq &cNtf, const CQueryTask *pcQueryTask)
{
	CMS_ASSERT(NULL != pcQueryTask);
	CMS_ASSERT(NULL != pcQueryTask->m_pc3acAgent);

	vector<CDevVidSrcInfo> &tmpVct = cNtf.GetDevVidsrcInfo();
	vector<CDevVidSrcInfo>::iterator pItem = tmpVct.begin();

	string groupId;
	string groupDomainId;
	string groupDomainName;

	while (pItem != tmpVct.end())
	{
		groupDomainId = pItem->GetDevGrpdomainId();
		if ( groupDomainId.empty() )
		{
			pItem++;
			continue;
		}

		groupDomainName = pcQueryTask->m_pc3acAgent->GetDomainNameByDomainID(groupDomainId);
		if ( groupDomainName.empty() )
		{
			pItem++;
			continue;
		}

		groupId = pItem->GetDevGrpId();
		pItem->SetDevGrpId(groupId + "@" + groupDomainName);

		pItem++;
	}

	resultList.insert(resultList.end(), tmpVct.begin(), tmpVct.end());

	if ( cNtf.IsLastNtf() )
	{
		SetActualResultNum(resultList.size());
	}
}

void CCuDeviceGroupDeviceQry4GbRsp::push_back(CDeviceGroupDeviceQry4GbNtfReq &cNtf, const CQueryTask *pcQueryTask)
{
	CMS_ASSERT(NULL != pcQueryTask);
	CMS_ASSERT(NULL != pcQueryTask->m_pc3acAgent);
	s32 nRecvItemNum = GetActualResultNum();

	vector<CDeviceGroupDeviceInfo4Gb>& tmpVct = cNtf.GetInfos();
	C3acAgent *pc3acAgent = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(pcQueryTask->m_pcCuAgent->m_str3asName, pcQueryTask->GetInstance());;
	if (pc3acAgent != NULL)
	{
		//将设备的UUID全部转换为devURI
		vector<CDeviceGroupDeviceInfo4Gb>::iterator pDevGrpDevInfoItem = tmpVct.begin();
		while (pDevGrpDevInfoItem != tmpVct.end())
		{
			string strDomainName = pc3acAgent->GetDomainNameByDomainID(pDevGrpDevInfoItem->GetDevDomainId());
			if (!strDomainName.empty())
			{
				string strTmp = pDevGrpDevInfoItem->GetDeviceId() + "@" + strDomainName;
				pDevGrpDevInfoItem->SetDeviceId(strTmp);
				strTmp = pDevGrpDevInfoItem->GetDevGrpId() + "@" + strDomainName;
				pDevGrpDevInfoItem->SetDevGrpId(strTmp);
			}

			vector<CDeviceGroupDeviceInfo4Gb>::iterator it = GetDeviceGroupGBDeviceInfo().begin();
			for (; it != GetDeviceGroupGBDeviceInfo().end(); ++it)
			{
				if (it->GetDevDevTreeId() == pDevGrpDevInfoItem->GetDevDevTreeId()
					&& it->GetDevGrpId() == pDevGrpDevInfoItem->GetDevGrpId()
					&& it->GetDeviceId() == pDevGrpDevInfoItem->GetDeviceId()
					&& it->GetDevDomainId() == pDevGrpDevInfoItem->GetDevDomainId())
				{
					it->GetDevinfo().insert(it->GetDevinfo().end(),
						pDevGrpDevInfoItem->GetDevinfo().begin(),
						pDevGrpDevInfoItem->GetDevinfo().end());
					break;
				}
			}

			if (it == GetDeviceGroupGBDeviceInfo().end())
			{
				GetDeviceGroupGBDeviceInfo().push_back(*pDevGrpDevInfoItem);
				++nRecvItemNum;
			}

			pDevGrpDevInfoItem++;
		}
	}
	SetActualResultNum(nRecvItemNum);
}


void CCuDeviceGbinfoQryRsp::push_back(CDeviceGbinfoQryNtfReq &cNtf, const CQueryTask *pcQueryTask)
{
	CMS_ASSERT(NULL != pcQueryTask);
	CMS_ASSERT(NULL != pcQueryTask->m_pc3acAgent);
	s32 nRecvItemNum = GetActualResultNum();

	vector<CDeviceFullGbInfo>& tmpVct = cNtf.GetDevGbinfos();
	C3acAgent *pc3acAgent = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(pcQueryTask->m_pcCuAgent->m_str3asName, pcQueryTask->GetInstance());;
	if (pc3acAgent != NULL)
	{
		//将设备的UUID全部转换为devURI
		vector<CDeviceFullGbInfo>::iterator pDevInfoItem = tmpVct.begin();
		while (pDevInfoItem != tmpVct.end())
		{
			string strTmp = pc3acAgent->GetDomainNameByDomainID(pDevInfoItem->GetDevDomainId());
			if (!strTmp.empty())
			{
				pDevInfoItem->SetDevId(pDevInfoItem->GetDevId() + "@" + strTmp);
			}

			vector<CDeviceGroupInfo>::iterator groupInfoIter = pDevInfoItem->GetDevGrpInfos().begin();
			while (groupInfoIter != pDevInfoItem->GetDevGrpInfos().end())
			{
				strTmp = pc3acAgent->GetDomainNameByDomainID(groupInfoIter->GetDomainUUID());
				if ( !strTmp.empty() )
				{
					groupInfoIter->SetDeviceGroupID(groupInfoIter->GetDeviceGroupID() + "@" + strTmp);
				}

				strTmp = pc3acAgent->GetDomainNameByDomainID(groupInfoIter->GetParentDomainUUID());
				if (!strTmp.empty())
				{
					groupInfoIter->SetDeviceParentGroupID(groupInfoIter->GetDeviceParentGroupID() + "@" + strTmp);
				}
				groupInfoIter++;
			}

			GetInfos().push_back(*pDevInfoItem);
			++nRecvItemNum;

			pDevInfoItem++;
		}
	}
	SetActualResultNum(nRecvItemNum);
}

void CCuDevgrpGbinfoQryRsp::push_back(CDevgrpGbinfoQryNtfReq &cNtf, const CQueryTask *pcQueryTask)
{
	CMS_ASSERT(NULL != pcQueryTask);
	CMS_ASSERT(NULL != pcQueryTask->m_pc3acAgent);
	s32 nRecvItemNum = GetActualResultNum();

	vector<TDevGrpFullGbInfos>& tmpVct = cNtf.GetDevGbinfos();
	C3acAgent *pc3acAgent = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(pcQueryTask->m_pcCuAgent->m_str3asName, pcQueryTask->GetInstance());;
	if (pc3acAgent != NULL)
	{
		//将设备的UUID全部转换为devURI
		vector<TDevGrpFullGbInfos>::iterator pDevGrpFullGbInfoItem = tmpVct.begin();
		while (pDevGrpFullGbInfoItem != tmpVct.end())
		{
			vector<CDeviceGroupInfo>::iterator pDevGrpInfoIter = pDevGrpFullGbInfoItem->begin();
			while (pDevGrpInfoIter != pDevGrpFullGbInfoItem->end())
			{
				string strTmp = pc3acAgent->GetDomainNameByDomainID(pDevGrpInfoIter->GetDomainUUID());
				if (!strTmp.empty())
				{
					pDevGrpInfoIter->SetDeviceGroupID(pDevGrpInfoIter->GetDeviceGroupID() + "@" + strTmp);
				}

				strTmp = pc3acAgent->GetDomainNameByDomainID(pDevGrpInfoIter->GetParentDomainUUID());
				if (!strTmp.empty())
				{
					pDevGrpInfoIter->SetParentDomainUUID(pDevGrpInfoIter->GetParentDomainUUID() + "@" + strTmp);
				}

				pDevGrpInfoIter++;
			}

			GetInfos().push_back(*pDevGrpFullGbInfoItem);
			++nRecvItemNum;

			pDevGrpFullGbInfoItem++;
		}
	}
	SetActualResultNum(nRecvItemNum);
}

//end of normal task impl

