/*
查询PTZ锁信息中的用户名和用户所属设备的PTZ控制级别
*/
#ifndef _GET_PTZ_LOCK_USER_INFO_TASK_H_
#define _GET_PTZ_LOCK_USER_INFO_TASK_H_

#include "cms/ospext/ospext.h"
#include "cms/ospsip/ospsip.h"
#include "cuidata.h"
#include "cms/commonconfig/cmcfg.h"
#include "culogintask.h"
#include "cms/utility/digital_string.h"
#include "normalcutask.h"

class CGetPuPtzLockUserInfoTask : public CSipTask
{
public:
	enum
	{
		NOT_QUERY_STATE = UNKNOWN_STATE + 1,        //空闲状态，未发送查询请求
		WAIT_RESPONSE_STATE,                        //等待查询Rsp
		WAIT_NOTIFY_STATE,                          //等待查询Ntf
	};
public:
	CGetPuPtzLockUserInfoTask(CInstExt *pcInst, CCuAgent *pcCuAgent, PTRecvNode ptRecvNode);
	~CGetPuPtzLockUserInfoTask();

	void SetPuPtzCtrlRsp(const CPuPtzCtrlRsp& cRsp);
	void SetPuTransOperateRsp(const CCuPuTransparentOperateRsp& cRsp);
public:
	//子类需要重写的方法
	virtual const char* GetObjName() const //必须重载的方法
	{
		return "CGetPuPtzLockUserInfoTask";
	}
	virtual const char* GetStateName(u32 dwState) const //必须重载的方法
	{
		switch (dwState)
		{
		case NOT_QUERY_STATE:
		{
			return "Not Query State";
		}
		break;
		case WAIT_RESPONSE_STATE:
		{
			return "Wait Response State";
		}
		break;
		case WAIT_NOTIFY_STATE:
		{
			return "Wait Notify State";
		}
		break;
		default:
			break;
		}
		return "Unknown State";
	}
	void TerminateTask();
	virtual void InitStateMachine();
public:
	template <typename CReq>
	u32 FakePostMsgReq(CReq &cReq, const TSipURI &tDstURI)
	{
		TASKLOG(NORMAL_TASK, EVENT_LEV, "CGetPuPtzLockUserInfoTask[TaskNO %u]发送消息[%s--%d]给服务器[%s]\n",
			GetTaskNO(), cReq.GetEventStr().c_str(), cReq.GetEventId(), tDstURI.GetURIString().c_str());

#ifdef _UNIT_TEST_
		g_cTaskNOQue.push(GetTaskNO());
		cout << "Post To " << tDstURI.GetURIString().c_str() << endl;
		cout << cReq.ToXml().c_str() << endl;
		return PROCMSG_OK;
#else
		return PostMsgReq<CReq>(cReq, tDstURI);
#endif
	}
public:
	//状态函数
	u32 NotQueryProcMsg(CMessage *const pcMsg);
	u32 WaitRspProcMsg(CMessage *const pcMsg);
	u32 WaitRspTimerPoll();
	u32 WaitNotifyProcMsg(CMessage *const pcMsg);
	u32 ProcNotifyMsg(CMessage *const pcMsg);
	u32 WaitNotifyTimerPoll();

	bool GetUserNameInfo(const string &strUserId, const TChannel &tDevVsChn);
	bool GetUserDevVsPtzPri();

	//NotReq状态下的泛型处理
	//因为CReq不能透传，CUI需要转换CU和3AS之间的数据，所以这里不直接处理CMessage
	//数据转换放到外面完成
	/*====================================================================
	函数名      : OnNotQueryMsg
	功能        : NotReq状态下的泛型处理
	因为CReq不能透传，CUI需要转换CU和3AS之间的数据，所以这里不直接处理CMessage
	数据转换放到外面完成
	算法实现    :
	引用全局变量:
	输入参数说明: CReqToSvr &cReq            请求类型必须是服务器定义的接口类型
	EServerType eSendToDst     服务器是3AS或者CMU，以后还有可能是RecMgr
	返回值说明  : 成功:true，失败:false
	----------------------------------------------------------------------
	修改记录    :
	日  期      版本        修改人        修改内容
	2012/2/3/             liangli
	====================================================================*/
	template<typename CReqToSvr, typename CRspToCu>
	u32 OnNotQueryMsg(CReqToSvr &cReq, EServerType eSendToDst = SERVER_3AS)
	{
		CMS_ASSERT(GetState() == NOT_QUERY_STATE);
		//缓存req消息，如果在wait rsp state收到ntf消息好处理
		m_dwReqEvent = cReq.GetEventId();
		//1 向服务器发送请求, 请求类型必须是服务器定义的接口类型
		u32 dwRet = PROCMSG_OK;
		switch (eSendToDst)
		{
		case SERVER_3AS:
		{
			dwRet = PostMsgReqTo3AS(cReq);
		}
		break;
		case SERVER_CMU:
		{
			PostMsgReqToCMU(cReq);
		}
		break;
		default:
		{
			return PROCMSG_FAIL;
		}
		break;
		}
		if (PROCMSG_DEL == dwRet)
		{
			TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]发送请求[%s--%d]，TAS不在线，直接拒绝请求\n",
				m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
				cReq.GetEventStr().c_str(), cReq.GetEventId());
			CEventRsp cRsp;
			cRsp.SetSeqNum(cReq.GetSeqNum());
			cRsp.SetSession(cReq.GetSession());
			cRsp.SetErrorCode(ERR_CUI_TAS_OFFLINE);
			m_strRspOfErr = cRsp.ToXml();
			SendRspToCu(m_strRspOfErr, m_ptRecvNode);
			NextState(NOT_QUERY_STATE);
			delete this;
			return dwRet;
		}

		//将QueryTag插入到会话表中，等待NTF
		g_cCuiDataMgr.m_mapSessToTaskMap.Insert(cReq.GetQueryTag(), GetTaskNO());
		m_strQueryTag = cReq.GetQueryTag();

		//2 转换状态，等待应答
		NextState(WAIT_RESPONSE_STATE);

		//3 创建结果保存数据，超时时复用此结果数据
		m_pRsp = new CRspToCu;
		m_pRsp->SetSeqNum(cReq.GetSeqNum());
		m_pRsp->SetSession(cReq.GetSession());
		return PROCMSG_OK;
	}
	template<typename CRspFromSvr, typename CRspToCu>
	u32 OnWaitRspMsg(CMessage *const pcMsg)
	{
		CMS_ASSERT(GetState() == WAIT_RESPONSE_STATE);

		COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
		CRspFromSvr cRsp;
		pcOspSipMsg->GetMsgBody(cRsp);
		if (cRsp.GetErrorCode() != CMS_SUCCESS || cRsp.GetEntryNum() == 0)
		{ //查询失败 或者 无结果
			if (cRsp.GetErrorCode() != CMS_SUCCESS)
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "CGetPuPtzLockUserInfoTask[NO: %u]查询应答[%s--%d]返回失败，错误码[%d]\n",
					GetTaskNO(), cRsp.GetEventStr().c_str(), cRsp.GetEventId(), cRsp.GetErrorCode());
			}
			else
			{
				TASKLOG(NORMAL_TASK, EVENT_LEV, "CGetPuPtzLockUserInfoTask[NO: %u]查询应答[%s--%d]成功，结果为空\n",
					GetTaskNO(), cRsp.GetEventStr().c_str(), cRsp.GetEventId());
			}

			m_pRsp->SetErrorCode(cRsp.GetErrorCode());
			m_strRspOfErr = m_pRsp->ToXml();
			SendRspToCu(m_strRspOfErr, m_ptRecvNode);
			NextState(NOT_QUERY_STATE);
			delete this;
			return PROCMSG_OK;
		}
		NextState(WAIT_NOTIFY_STATE);
		return PROCMSG_OK;
	}

public:
	PTRecvNode m_ptRecvNode;
	CCuAgent *m_pcCuAgent;      //该指针一定不为空
	C3acAgent *m_pc3acAgent;    //只有当查询成功后(说明TAS在线)，该指针才有效，为了效率缓存
	string m_strRspOfErr;
	CPuPtzCtrlWithLockRsp *m_pRsp;
	u32 m_dwRspEventId;
	string m_strQueryTag;       //当其不为空时，表面在m_mapSessToTaskMap中登记过
	std::list<string> m_VecNtfReqList; //缓存在rsp消息中接受到得ntf消息
	u16 m_dwReqEvent;
	string m_strUserId;
	TChannel m_tVsChn;
	bool m_bIsSuperAdmin;
};

#endif // _GET_PTZ_LOCK_USER_INFO_TASK_H_
