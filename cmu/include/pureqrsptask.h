/*========================================================================================
模块名    ：cmu
文件名    ：pureqrsptask.h
相关文件  ：
实现功能  ：pu请求-应答类型事务模板
作者      ：fanxg
版权      ：<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2012/01/15         V1.0              fanxg                                  新建文件
=========================================================================================*/
#ifndef _PU_REQ_RSP_TASK_H
#define _PU_REQ_RSP_TASK_H

#include "cms/ospsip/ospsip.h"
#include "cms/cms_errorcode.h"
#include "cmuconst.h"
#include "cmudata.h"
#include "puregtask.h"
#include "cmuconnecttask.h"
#include "cmucoreapp.h"

template<class CReq, class CRsp>
class CPuReqRspTask : public CSipTask
{
public:
    enum
    {
        Req = UNKNOWN_STATE+1,
        WaitRsp,
    };

public:
	CPuReqRspTask(CInstExt* pInst) : CSipTask(pInst)
	{
        m_tSipTransId = INVALID_TRANS_ID;
	}
	
	~CPuReqRspTask()
	{	
        ReleaseResource();
	}

    void ReleaseResource()
    {
        if (IsLocalDomainByDevUri(m_tReq.GetDevUri()))
        {
            return;
        }

        //异域操作需要删除登录依赖
        TSipURI tCmuUri = GetCmuUriFromDevUri(m_tReq.GetDevUri());
        CLoginSession* ptCmuTask = NULL;
        g_cCmuList.Find(tCmuUri.GetURIString(), ptCmuTask);
        if (ptCmuTask != NULL)
        {
            ptCmuTask->DelTask(this);
        }
    }

public:
	virtual const char* GetObjName() const
	{
		return "CPuReqRspTask";
	}

    virtual const char* GetStateName(u32 dwState) const
    {
        switch (dwState)
        {
        case Req:
            return "Req";
        case WaitRsp:
            return "WaitRsp";
        default:
            break;
        }
        return "Unknown State";
    }


    virtual void InitStateMachine()
    {
        CStateProc cReqProc;
        cReqProc.ProcMsg = (CTask::PFProcMsg)&CPuReqRspTask::OnReq;
        cReqProc.ProcErrMsg = (CTask::PFProcMsg)&CPuReqRspTask::OnReq;
        cReqProc.TimerPoll = (CTask::PFTimerPoll)&CPuReqRspTask::OnReqTimer;
        AddRuleProc(Req, cReqProc);

        CStateProc cWaitRspProc;
        cWaitRspProc.ProcMsg = (CTask::PFProcMsg)&CPuReqRspTask::OnWaitRsp;
        cWaitRspProc.ProcErrMsg = (CTask::PFProcMsg)&CPuReqRspTask::OnWaitRsp;
        cWaitRspProc.TimerPoll = (CTask::PFTimerPoll)&CPuReqRspTask::OnWaitRspTimer;
        AddRuleProc(WaitRsp, cWaitRspProc);

        NextState(Req);
    }

    virtual void PrintData() const
    {
        CTask::PrintData();

        //直接打印xml数据
        OspPrintf(TRUE, FALSE, "  %s\n", m_tReq.ToXml().c_str());
    }

    //状态处理
    u32 OnReq(CMessage *const pcMsg)
    {
       u32 dwRet = PROCMSG_OK;
       if ((u16)m_tReq.GetEventId() == pcMsg->event)
       {
           COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
           m_tSipTransId = pcOspSipMsg->GetSipTransID();

           CReq cReq;
           pcOspSipMsg->GetMsgBody(cReq);
           m_tReq = cReq;

           //确定pu路由,转发消息
           TSipURI tPuRouterUri = GetPuRouter(cReq.GetDevUri());
           if (tPuRouterUri.IsNull())
           {
               TASKLOG(CMU_MOD_PU, WARNING_LEV, "Dev[%s]无法路由, Msg[%s-%d]请求失败\n", 
                   m_tReq.GetDevUri().c_str(), m_tReq.GetEventStr().c_str(), m_tReq.GetEventId());

               SendRsp(ERR_CMU_DEV_NO_ROUTER);
               dwRet = PROCMSG_DEL;
               return dwRet;
           }

           if (IsLocalDomainByDevUri(tPuRouterUri))
           {
               TSipURI tDevUri;
               tDevUri.SetURIString(cReq.GetDevUri());
               CPuData *pcPuData = CCmuData::GetInstance()->GetPuData(tDevUri.GetUser());
               if (NULL == pcPuData)
               {
                   TASKLOG(CMU_MOD_PU, ERROR_LEV, "Dev[%s]未入网, Msg[%s-%d]请求失败\n", 
                       cReq.GetDevUri().c_str(), m_tReq.GetEventStr().c_str(), m_tReq.GetEventId());

                   SendRsp(ERR_CMU_PU_NOT_IN_TOP);
                   dwRet = PROCMSG_DEL;
                   return dwRet;
               }

               CPuRegBaseTask *ptPuRegTask = pcPuData->GetPuRegTask();
               if (ptPuRegTask == NULL)
               {
                   TASKLOG(CMU_MOD_PU, ERROR_LEV, "Dev[%s]不在线, Msg[%s-%d]请求失败\n", 
                       cReq.GetDevUri().c_str(), m_tReq.GetEventStr().c_str(), m_tReq.GetEventId());

                   SendRsp(ERR_CMU_PU_NOT_ONLINE);
                   dwRet = PROCMSG_DEL;
                   return dwRet;
               }

               //发往pu的消息需要将session改写pu的登录session
               cReq.SetSession(ptPuRegTask->GetSession());
           }
           else
           {
               if (!IsDomainInTopByDevUri(tPuRouterUri))
               {
                   TASKLOG(CMU_MOD_PU, WARNING_LEV, "Dev[%s]所在域未组网, Msg[%s-%d]请求失败\n", 
                       m_tReq.GetDevUri().c_str(), m_tReq.GetEventStr().c_str(), m_tReq.GetEventId());

                   SendRsp(ERR_CMU_DOMAIN_NOT_IN_TOP);
                   dwRet = PROCMSG_DEL;
                   return dwRet;
               }   
           }

           SendReq(cReq, tPuRouterUri);
       }
       else if (pcMsg->event == CMU_CMU_CONNECT_SUCCESS)
       {
           //异域cmu登录成功, 可以发送请求
           TSipURI tPuRouterUri = GetPuRouter(m_tReq.GetDevUri());
           if (!IsLocalDomainByDevUri(tPuRouterUri))
           {
               SendReq(m_tReq, tPuRouterUri);
           }
           else
           {
               TASKLOG(CMU_MOD_PU, ERROR_LEV, "非异域设备[%s], Receive Msg[%s-%d]\n", 
                   m_tReq.GetDevUri().c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
           }
       }
       else
       {
           TASKLOG(CMU_MOD_PU, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
           dwRet = PROCMSG_FAIL;
       }

       return dwRet;
    }

    u32 OnReqTimer()
    {
        //该定时器异域操作时会用到
        if (GetCurStateHoldTime() > CMU_CONNECT_CMU_TIMEOUT)
        {
            TSipURI tCmuUri = GetCmuUriFromDevUri(m_tReq.GetDevUri());
            TASKLOG(CMU_MOD_PU, WARNING_LEV, "登录异域cmu[%s]超时, Msg[%s-%d]请求失败\n", 
                tCmuUri.GetURIString().c_str(), m_tReq.GetEventStr().c_str(), m_tReq.GetEventId());
            SendRsp(ERR_CMU_CMU_NOT_CONN);
            return TIMERPOLL_DEL;
        }

        return TIMERPOLL_DONE;
    }

    u32 OnWaitRsp(CMessage *const pcMsg)
    {
        u32 dwRet = PROCMSG_DEL;

        //SIP层错误处理
        if (pcMsg->event == OSP_SIP_MSG_PROC_FAIL)
        {
            //发起的请求收到SIP层的错误应答
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            if (pcOspSipMsg == NULL)
            {
                TASKLOG(CMU_MOD_PU, ERROR_LEV, "ospsip 消息为空\n");
                dwRet = PROCMSG_FAIL;
                return dwRet;
            }


            TASKLOG(CMU_MOD_PU, ERROR_LEV, "对设备[%s]请求失败, 发生SIP层错误，sip status code[%d]\n",
                m_tReq.GetDevUri().c_str(), pcOspSipMsg->GetSipErrorCode());

            SendRsp((u32)pcOspSipMsg->GetSipErrorCode());
            return dwRet;
        }

        CRsp cRsp;
        if ((u16)cRsp.GetEventId() == pcMsg->event)
        {
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            if (pcOspSipMsg == NULL)
            {
                TASKLOG(CMU_MOD_PU, ERROR_LEV, "ospsip 消息为空\n");
                SendRsp(ERR_CMU_SIP_BODY_NULL);
                return dwRet;
            }

            pcOspSipMsg->GetMsgBody(cRsp);
            cRsp.SetSession(m_tReq.GetSession());
            SendRsp(cRsp);
        }
        else
        {
            TASKLOG(CMU_MOD_PU, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            dwRet = PROCMSG_FAIL;
        }

        //事务处理结束，删除之
        return dwRet;
    }

    u32 OnWaitRspTimer()
    {
        //应答超时
        if (GetCurStateHoldTime() > CMU_REQ_RSP_TASK_TIMEOUT)
        {
            TASKLOG(CMU_MOD_PU, WARNING_LEV, "Dev[%s]应答超时, Msg[%s-%d]请求失败\n", 
                m_tReq.GetDevUri().c_str(), m_tReq.GetEventStr().c_str(), m_tReq.GetEventId());
            SendRsp(ERR_CMU_DEV_RSP_TIMEOUT);
            return TIMERPOLL_DEL;
        }

        return TIMERPOLL_DONE;
    }

    void SendReq(const CReq& cReq, const TSipURI& tDstUri)
    {
        if (IsLocalDomainByDevUri(tDstUri))
//        if (IsLocalDomain(tDstUri))
        {
            //本域直接投递
            PostMsgReq(cReq, tDstUri);
            NextState(WaitRsp);
        }
        else
        {
            //异域处理，如果没有登陆过异域需要先登录，登录成功后再发起操作请求
            CCmuConnecterTask *pcCmuTask = GetConnectTaskByDomainName(tDstUri.GetDomain());
            CMS_ASSERT(NULL != pcCmuTask && "pcCmuTask = NULL!!!");
            //cmu连接成功后才发起订阅
            if (pcCmuTask->GetState() == CCmuConnecterTask::Service)
            {
                PostMsgReq(cReq, GetCmuUriFromDomainName(tDstUri.GetDomain()));
                NextState(WaitRsp);
            }

            //跨域事务都需要登记到cmu登录会话中
            //对于长期事务，添加是为了域断链时释放长期事务的资源
            //对于瞬时事务，添加是为了cmu登录成功后即时通知到本事务
            //事务删除时，析构函数中需要去除本事务在cmu登录会话中的登记
            pcCmuTask->AddTask(this);
        }
    }

    bool SendRsp(u32 dwError)
    {
        CRsp cRsp;
        cRsp.SetSeqNum(m_tReq.GetSeqNum());
        cRsp.SetSession(m_tReq.GetSession());
        cRsp.SetErrorCode(dwError);

        if (m_tSipTransId != INVALID_TRANS_ID)
        {
            return (PROCMSG_OK == PostMsgRsp(m_tSipTransId, cRsp));
        }
        else
        {
            return true;
        }   
    }

    bool SendRsp(const CRsp& cRsp)
    {
        if (m_tSipTransId != INVALID_TRANS_ID)
        {
            return (PROCMSG_OK == PostMsgRsp(m_tSipTransId, cRsp));
        }
        else
        {
            return true;
        }
    }

public: 
    const TKDSIP_TRANS_ID GetSipTransID() const
    {
        return m_tSipTransId;
    }
    void SetSipTransID(const TKDSIP_TRANS_ID tSipTransID)
    {
        m_tSipTransId = tSipTransID;
    }

    const CReq& GetReq() const
    {
        return m_tReq;
    }
    CReq& GetReq()
    {
        return m_tReq;
    }
    void SetReq(const CReq& tReq)
    {
        m_tReq = tReq;
    }
	
private:
    TKDSIP_TRANS_ID m_tSipTransId;
	CReq m_tReq;
};

template<class CReq, class CRsp>
class CPuReqRspTask2 : public CSipTask
{
public:
	enum
	{
		Req = UNKNOWN_STATE+1,
		WaitRsp,
	};

public:
	CPuReqRspTask2(CInstExt* pInst) : CSipTask(pInst)
	{
		m_tSipTransId = INVALID_TRANS_ID;
	}

	~CPuReqRspTask2()
	{	
		ReleaseResource();
	}

	void ReleaseResource()
	{
		if (IsLocalDomainByDevUri(m_tReq.GetDevUri()))
		{
			return;
		}

		//异域操作需要删除登录依赖
		TSipURI tCmuUri = GetCmuUriFromDevUri(m_tReq.GetDevUri());
		CLoginSession* ptCmuTask = NULL;
		g_cCmuList.Find(tCmuUri.GetURIString(), ptCmuTask);
		if (ptCmuTask != NULL)
		{
			ptCmuTask->DelTask(this);
		}
	}

public:
	virtual const char* GetObjName() const
	{
		return "CPuReqRspTask2";
	}

	virtual const char* GetStateName(u32 dwState) const
	{
		switch (dwState)
		{
		case Req:
			return "Req";
		case WaitRsp:
			return "WaitRsp";
		default:
			break;
		}
		return "Unknown State";
	}


	virtual void InitStateMachine()
	{
		CStateProc cReqProc;
		cReqProc.ProcMsg = (CTask::PFProcMsg)&CPuReqRspTask2::OnReq;
		cReqProc.ProcErrMsg = (CTask::PFProcMsg)&CPuReqRspTask2::OnReq;
		cReqProc.TimerPoll = (CTask::PFTimerPoll)&CPuReqRspTask2::OnReqTimer;
		AddRuleProc(Req, cReqProc);

		CStateProc cWaitRspProc;
		cWaitRspProc.ProcMsg = (CTask::PFProcMsg)&CPuReqRspTask2::OnWaitRsp;
		cWaitRspProc.ProcErrMsg = (CTask::PFProcMsg)&CPuReqRspTask2::OnWaitRsp;
		cWaitRspProc.TimerPoll = (CTask::PFTimerPoll)&CPuReqRspTask2::OnWaitRspTimer;
		AddRuleProc(WaitRsp, cWaitRspProc);

		NextState(Req);
	}

	virtual void PrintData() const
	{
		CTask::PrintData();

		//直接打印xml数据
		OspPrintf(TRUE, FALSE, "  %s\n", m_tReq.ToXml().c_str());
	}

	//状态处理
	u32 OnReq(CMessage *const pcMsg)
	{
		u32 dwRet = PROCMSG_OK;
		if ((u16)m_tReq.GetEventId() == pcMsg->event)
		{
			COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
			m_tSipTransId = pcOspSipMsg->GetSipTransID();

			CReq cReq;
			pcOspSipMsg->GetMsgBody(cReq);
			m_tReq = cReq;

			//确定pu路由,转发消息
			TSipURI tPuRouterUri = GetPuRouter(cReq.GetDevUri());
			if (tPuRouterUri.IsNull())
			{
				TASKLOG(CMU_MOD_PU, WARNING_LEV, "Dev[%s]无法路由, Msg[%s-%d]请求失败\n", 
					m_tReq.GetDevUri().c_str(), m_tReq.GetEventStr().c_str(), m_tReq.GetEventId());

				SendRsp(ERR_CMU_DEV_NO_ROUTER);
				dwRet = PROCMSG_DEL;
				return dwRet;
			}

			if (IsLocalDomainByDevUri(tPuRouterUri))
			{
				TSipURI tDevUri;
				tDevUri.SetURIString(cReq.GetDevUri());
				CPuData *pcPuData = CCmuData::GetInstance()->GetPuData(tDevUri.GetUser());
				if (NULL == pcPuData)
				{
					TASKLOG(CMU_MOD_PU, ERROR_LEV, "Dev[%s]未入网, Msg[%s-%d]请求失败\n", 
						cReq.GetDevUri().c_str(), m_tReq.GetEventStr().c_str(), m_tReq.GetEventId());

					SendRsp(ERR_CMU_PU_NOT_IN_TOP);
					dwRet = PROCMSG_DEL;
					return dwRet;
				}

				CPuRegBaseTask *ptPuRegTask = pcPuData->GetPuRegTask();
				if (ptPuRegTask == NULL)
				{
					TASKLOG(CMU_MOD_PU, ERROR_LEV, "Dev[%s]不在线, Msg[%s-%d]请求失败\n", 
						cReq.GetDevUri().c_str(), m_tReq.GetEventStr().c_str(), m_tReq.GetEventId());

					SendRsp(ERR_CMU_PU_NOT_ONLINE);
					dwRet = PROCMSG_DEL;
					return dwRet;
				}

				//发往pu的消息需要将session改写pu的登录session
				cReq.SetSession(ptPuRegTask->GetSession());
			}
			else
			{
				if (!IsDomainInTopByDevUri(tPuRouterUri))
				{
					TASKLOG(CMU_MOD_PU, WARNING_LEV, "Dev[%s]所在域未组网, Msg[%s-%d]请求失败\n", 
						m_tReq.GetDevUri().c_str(), m_tReq.GetEventStr().c_str(), m_tReq.GetEventId());

					SendRsp(ERR_CMU_DOMAIN_NOT_IN_TOP);
					dwRet = PROCMSG_DEL;
					return dwRet;
				}   
			}

			SendReq(cReq, tPuRouterUri);
		}
		else if (pcMsg->event == CMU_CMU_CONNECT_SUCCESS)
		{
			//异域cmu登录成功, 可以发送请求
			TSipURI tPuRouterUri = GetPuRouter(m_tReq.GetDevUri());
			if (!IsLocalDomainByDevUri(tPuRouterUri))
			{
				SendReq(m_tReq, tPuRouterUri);
			}
			else
			{
				TASKLOG(CMU_MOD_PU, ERROR_LEV, "非异域设备[%s], Receive Msg[%s-%d]\n", 
					m_tReq.GetDevUri().c_str(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
			}
		}
		else
		{
			TASKLOG(CMU_MOD_PU, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
			dwRet = PROCMSG_FAIL;
		}

		return dwRet;
	}

	u32 OnReqTimer()
	{
		//该定时器异域操作时会用到
		if (GetCurStateHoldTime() > CMU_CONNECT_CMU_TIMEOUT)
		{
			TSipURI tCmuUri = GetCmuUriFromDevUri(m_tReq.GetDevUri());
			TASKLOG(CMU_MOD_PU, WARNING_LEV, "登录异域cmu[%s]超时, Msg[%s-%d]请求失败\n", 
				tCmuUri.GetURIString().c_str(), m_tReq.GetEventStr().c_str(), m_tReq.GetEventId());
			SendRsp(ERR_CMU_CMU_NOT_CONN);
			return TIMERPOLL_DEL;
		}

		return TIMERPOLL_DONE;
	}

	u32 OnWaitRsp(CMessage *const pcMsg)
	{
		u32 dwRet = PROCMSG_DEL;

		//SIP层错误处理
		if (pcMsg->event == OSP_SIP_MSG_PROC_FAIL)
		{
			//发起的请求收到SIP层的错误应答
			COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
			if (pcOspSipMsg == NULL)
			{
				TASKLOG(CMU_MOD_PU, ERROR_LEV, "ospsip 消息为空\n");
				dwRet = PROCMSG_FAIL;
				return dwRet;
			}


			TASKLOG(CMU_MOD_PU, ERROR_LEV, "对设备[%s]请求失败, 发生SIP层错误，sip status code[%d]\n",
				m_tReq.GetDevUri().c_str(), pcOspSipMsg->GetSipErrorCode());

			SendRsp((u32)pcOspSipMsg->GetSipErrorCode());
			return dwRet;
		}

		CRsp cRsp;
		if ((u16)cRsp.GetEventId() == pcMsg->event)
		{
			COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
			if (pcOspSipMsg == NULL)
			{
				TASKLOG(CMU_MOD_PU, ERROR_LEV, "ospsip 消息为空\n");
				SendRsp(ERR_CMU_SIP_BODY_NULL);
				return dwRet;
			}

			pcOspSipMsg->GetMsgBody(cRsp);
			cRsp.SetSession(m_tReq.GetSession());
			SendRsp(cRsp);
		}
		else
		{
			TASKLOG(CMU_MOD_PU, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
			dwRet = PROCMSG_FAIL;
		}

		//事务处理结束，删除之
		return dwRet;
	}

	u32 OnWaitRspTimer()
	{
		//应答超时
		if (GetCurStateHoldTime() > CMU_REQ_RSP_TASK_TIMEOUT)
		{
			TASKLOG(CMU_MOD_PU, WARNING_LEV, "Dev[%s]应答超时, Msg[%s-%d]请求失败\n", 
				m_tReq.GetDevUri().c_str(), m_tReq.GetEventStr().c_str(), m_tReq.GetEventId());
			SendRsp(ERR_CMU_DEV_RSP_TIMEOUT);
			return TIMERPOLL_DEL;
		}

		return TIMERPOLL_DONE;
	}

	void SendReq(const CReq& cReq, const TSipURI& tDstUri)
	{
		if (IsLocalDomainByDevUri(tDstUri))
			//        if (IsLocalDomain(tDstUri))
		{
			//本域直接投递
			PostMsgReq(cReq, tDstUri);
			NextState(WaitRsp);
		}
		else
		{
			//异域处理，如果没有登陆过异域需要先登录，登录成功后再发起操作请求
			CCmuConnecterTask *pcCmuTask = GetConnectTaskByDomainName(tDstUri.GetDomain());
			CMS_ASSERT(NULL != pcCmuTask && "pcCmuTask = NULL!!!");
			//cmu连接成功后才发起订阅
			if (pcCmuTask->GetState() == CCmuConnecterTask::Service)
			{
				PostMsgReq(cReq, GetCmuUriFromDomainName(tDstUri.GetDomain()));
				NextState(WaitRsp);
			}

			//跨域事务都需要登记到cmu登录会话中
			//对于长期事务，添加是为了域断链时释放长期事务的资源
			//对于瞬时事务，添加是为了cmu登录成功后即时通知到本事务
			//事务删除时，析构函数中需要去除本事务在cmu登录会话中的登记
			pcCmuTask->AddTask(this);
		}
	}

	bool SendRsp(u32 dwError)
	{
		CRsp cRsp;
		cRsp.SetSeqNum(m_tReq.GetSeqNum());
		cRsp.SetSession(m_tReq.GetSession());
		cRsp.SetErrorCode(dwError);

		if (m_tSipTransId != INVALID_TRANS_ID)
		{
			return (PROCMSG_OK == PostMsgRsp(m_tSipTransId, cRsp));
		}
		else
		{
			return true;
		}   
	}

	bool SendRsp(const CRsp& cRsp)
	{
		if (m_tSipTransId != INVALID_TRANS_ID)
		{
			return (PROCMSG_OK == PostMsgRsp(m_tSipTransId, cRsp));
		}
		else
		{
			return true;
		}
	}

public: 
	const TKDSIP_TRANS_ID GetSipTransID() const
	{
		return m_tSipTransId;
	}
	void SetSipTransID(const TKDSIP_TRANS_ID tSipTransID)
	{
		m_tSipTransId = tSipTransID;
	}

	const CReq& GetReq() const
	{
		return m_tReq;
	}
	CReq& GetReq()
	{
		return m_tReq;
	}
	void SetReq(const CReq& tReq)
	{
		m_tReq = tReq;
	}

private:
	TKDSIP_TRANS_ID m_tSipTransId;
	CReq m_tReq;
};


//pu系统参数获取、配置
typedef CPuReqRspTask<CPuSysParamGetReq, CPuSysParamGetRsp> CGetPuSysParamTask;
typedef CPuReqRspTask<CPuSysParamSetReq, CPuSysParamSetRsp> CSetPuSysParamTask;

//保存pu配置
typedef CPuReqRspTask<CPuSaveCfgReq, CPuSaveCfgRsp> CSavePuCfgTask;

//pu视频编码参数获取、配置
typedef CPuReqRspTask<CPuVidEncParamGetReq, CPuVidEncParamGetRsp> CGetPuVidEncParamTask;
typedef CPuReqRspTask<CPuVidEncParamSetReq, CPuVidEncParamSetRsp> CSetPuVidEncParamTask;

//pu告警输出
typedef CPuReqRspTask<CPuIoOutReq, CPuIoOutRsp> CPuIoOutTask;

//pu告警读秒
typedef CPuReqRspTask<CPuReadSecReq, CPuReadSecRsp> CPuReadSecTask;

//pu告警复位
typedef CPuReqRspTask2<CPuAlarmResetReq, CPuAlarmResetRsp> CPuAlarmResetTask;

//摄像头类型
typedef CPuReqRspTask<CPuCameraTypeSetReq, CPuCameraTypeSetRsp> CSetPuCameraTypeTask;

//移动侦测
typedef CPuReqRspTask<CPuMotionDetectSetReq, CPuMotionDetectSetRsp> CSetPuMotionDetectTask;
typedef CPuReqRspTask<CPuMotionDetectGetReq, CPuMotionDetectGetRsp> CGetPuMotionDetectTask;

//图像遮蔽
typedef CPuReqRspTask<CPuAreaShadeSetReq, CPuAreaShadeSetRsp> CSetPuAreaShadeTask;
typedef CPuReqRspTask<CPuAreaShadeGetReq, CPuAreaShadeGetRsp> CGetPuAreaShadeTask;

//解码器风格配置、获取
typedef CPuReqRspTask<CPuDecStyleSetReq, CPuDecStyleSetRsp> CSetPuDecStyleTask;
typedef CPuReqRspTask<CPuDecStyleGetReq, CPuDecStyleGetRsp> CGetPuDecStyleTask;

//视频源字幕配置、获取
typedef CPuReqRspTask<CPuVidSrcOsdSetReq, CPuVidSrcOsdSetRsp> CSetPuVidSrcOsdTask;
typedef CPuReqRspTask<CPuVidSrcOsdGetReq, CPuVidSrcOsdGetRsp> CGetPuVidSrcOsdTask;

//前端VSIP数据操作
typedef CPuReqRspTask<CPuVsipOperateReq, CPuVsipOperateRsp> CPuVsipOperateTask;

//前端透明数据操作
typedef CPuReqRspTask<CPuTransparentOperateReq, CPuTransparentOperateRsp> CPuTransparentOperateTask;

//请求关键帧
typedef CPuReqRspTask<CKeyFrameReq, CKeyFrameRsp> CKeyFrameTask;

//前端图片抓拍
typedef CPuReqRspTask<CPuPicSnapshotReq, CPuPicSnapshotRsp> CPuPicSnapshotTask;

//前端图片下载
typedef CPuReqRspTask<CPuPicDownloadReq, CPuPicDownloadRsp> CPuPicDownloadTask;

//前端录像日历查询
typedef CPuReqRspTask<CPuRecDaysQryReq, CPuRecDaysQryRsp> CPuRecDaysQryTask;

//GPS参数配置
typedef CPuReqRspTask<CPuGpsCfgSetReq, CPuGpsCfgSetRsp> CPuGpsCfgSetTask;
typedef CPuReqRspTask<CPuGpsCfgGetReq, CPuGpsCfgGetRsp> CPuGpsCfgGetTask;

//GPS订阅配置
typedef CPuReqRspTask<CPuGpsSsSetReq, CPuGpsSsSetRsp> CPuGpsSsSetTask;
typedef CPuReqRspTask<CPuGpsSsGetReq, CPuGpsSsGetRsp> CPuGpsSsGetTask;

//前端录像
typedef CPuReqRspTask<CPuRecordStartReq, CPuRecordStartRsp> CPuRecordStartTask;
typedef CPuReqRspTask<CPuRecordStopReq, CPuRecordStopRsp> CPuRecordStopTask;
typedef CPuReqRspTask<CPuRecordDelReq, CPuRecordDelRsp> CPuRecordDelTask;

//透明数据配置的设置、获取
typedef CPuReqRspTask<CPuTransdataCfgSetReq, CPuTransdataCfgSetRsp> CPuTransdataCfgSetTask;
typedef CPuReqRspTask<CPuTransdataCfgGetReq, CPuTransdataCfgGetRsp> CPuTransdataCfgGetTask;

//传输台标
typedef CPuReqRspTask<CPuLogoBmpOsdTransmitReq, CPuLogoBmpOsdTransmitRsp> CPuLogoBmpOsdTransmitTask;

//长字幕配置
typedef CPuReqRspTask<CPuLongOsdSetReq, CPuLongOsdSetRsp> CPuLongOsdSetTask;
typedef CPuReqRspTask<CPuLongOsdGetReq, CPuLongOsdGetRsp> CPuLongOsdGetTask;

//{{{{{{{{{{{{{{{ add by Royan 20140418 多行字幕
//多行字幕配置
typedef CPuReqRspTask<CPuMultiOsdSetReq, CPuMultiOsdSetRsp> CPuMultiOsdSetTask;
typedef CPuReqRspTask<CPuMultiOsdGetReq, CPuMultiOsdGetRsp> CPuMultiOsdGetTask;
//}}}}}}}}}}}}}}} add by Royan 20140418 多行字幕

//音频参数配置
typedef CPuReqRspTask<CPuAudEncParamSetReq, CPuAudEncParamSetRsp> CPuAudEncParamSetTask;
typedef CPuReqRspTask<CPuAudEncParamGetReq, CPuAudEncParamGetRsp> CPuAudEncParamGetTask;



//前端抓拍参数设置
typedef CPuReqRspTask<CPuSnapShotCfgGetReq, CPuSnapShotCfgGetRsp> CPuSnapShotCfgGetTask;
typedef CPuReqRspTask<CPuSnapShotCfgSetReq, CPuSnapShotCfgSetRsp> CPuSnapShotCfgSetTask;

//串口参数设置
typedef CPuReqRspTask<CPuSerialParamCfgGetReq, CPuSerialParamCfgGetRsp> CPuSerialParamCfgGetTask;
typedef CPuReqRspTask<CPuSerialParamCfgSetReq, CPuSerialParamCfgSetRsp> CPuSerialParamCfgSetTask;



#endif  //#ifndef _PU_REQ_RSP_TASK_H