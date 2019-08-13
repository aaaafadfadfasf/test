/*****************************************************************************
模块名      : cmu 
文件名      : cmutask.h 
相关文件    : 
文件实现功能: 任务基类。提供Req的预处理方法，以保存Sip头信息/req/rsp等信息，
             简化task模式，提供统一处理模型，便于调试，以及可靠性。
作者        : 黄至春
版本        : vs_phoenix  Copyright(C) 2009-2015 KDDACOM, All rights reserved.
-----------------------------------------------------------------------------
修改记录:
日  期         版本        修改人      修改内容
2012/08/14     1.0        黄至春        创建
******************************************************************************/

#ifndef _CMU_TASK_H
#define _CMU_TASK_H

#include "cms/ospsip/ospsip.h"
#include "cms/cms_errorcode.h"
#include "cms/cmu/cmu_proto.h"
#include "cmuconst.h"
#include <sstream>
#include "cmuconf.h"
#include "cmudata.h"
#include "cmuconnecttask.h"
#include "cmucoreapp.h"
#include "cuiregtask.h"
#include "puiregtask.h"
#include "vtduregtask.h"

class CCmuTask : public CSipTask
{
public:
    CCmuTask(CInstExt *pcInst):CSipTask(pcInst)
    {
        m_pcReq = NULL;
        m_pcRsp = NULL;

        m_tFromUri.Clear();
        m_tSipTransId = INVALID_TRANS_ID;
        m_tSipDlgID = INVALID_DIALOG_ID;

        m_nModuleID = CMU_MOD_CMU;
        m_nErrorCode = CMS_SUCCESS;
        m_ptReqCmuSsn = NULL;
    }
    virtual ~CCmuTask()
    {
		//任务删除时，如果还没有应答过，则发送应答
		if(INVALID_TRANS_ID != m_tSipTransId && NULL != m_pcRsp)
		{
			DoPostMsgRsp(*m_pcRsp);
		}

        if (NULL != m_ptReqCmuSsn)
        {
            m_ptReqCmuSsn->DelTask(this);
        }

        SAFE_DEL_PTR(m_pcReq);
        SAFE_DEL_PTR(m_pcRsp);
    }

	/////////////////////////////////////////////////////////////
	//重载函数
public:
    virtual const char* GetObjName() const
    {
        return "CCmuTask";
    }

    virtual const char* GetStateName(u32 dwState) const
    {
        return "CCmuTask_Virtual";
    }

    virtual void PrintData() const
    {
        CTask::PrintData();

        OspPrintf(TRUE, FALSE, "m_tFromUri[%s] m_tSipTransId[%lu] m_tSipDlgID[%lu] m_nModuleID[%d] m_nErrorCode[%d]\n", 
            m_tFromUri.GetURIString().c_str(), m_tSipTransId, m_tSipDlgID, m_nModuleID, m_nErrorCode);

        if (m_pcReq)
        {
            OspPrintf(TRUE, FALSE, "Req:\n%s\n", m_pcReq->ToXml().c_str());
        }
    }

    void ReleaseResource()
    {
        if (CMS_SUCCESS != m_nErrorCode)
        {
            TASKLOG(this->GetModuleID(), EVENT_LEV, "//////////////事务[%s]准备销毁, ErrorCode[%d]///////////////\n", 
                this->GetObjName(), m_nErrorCode);
        }
    }
	/////////////////////////////////////////////////////////////
	//业务逻辑
public:
    template<class CReq, class CRsp>
	CReq& ProcSipReq2(CMessage *const pcMsg)
    {
        if (m_pcReq)
        {
            SAFE_DEL_PTR(m_pcReq);
        }
        m_pcReq = new CReq();

        if (m_pcRsp)
        {
            SAFE_DEL_PTR(m_pcRsp);
        }
        m_pcRsp = new CRsp();

        COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
        if (pcOspSipMsg == NULL)
        {
            CMS_ASSERT(pcOspSipMsg && "ospsip 消息为空\n");
            //TASKLOG(this->GetModuleID(), ERROR_LEV, "ospsip 消息为空\n");
            //nErrorCode = ERR_CMU_SIP_BODY_NULL;

            return *((CReq*)m_pcReq);
        }

        m_tFromUri = pcOspSipMsg->GetSipFromUri();
        m_pcReq->FromXml(pcOspSipMsg->GetMsgBody());
        m_tSipTransId = pcOspSipMsg->GetSipTransID();
        m_tSipDlgID = pcOspSipMsg->GetSipDlgID();

        TASKLOG(this->GetModuleID(), CRITICAL_LEV, "ReqTransId[%lu] DlgId[%lu]\n", m_tSipTransId, m_tSipDlgID);

        return *((CReq*)m_pcReq);
    }

    template<class CReq>
    CReq& ProcSipReq(CMessage *const pcMsg)
    {
        if (m_pcReq)
        {
            SAFE_DEL_PTR(m_pcReq);
        }
        m_pcReq = new CReq();

        COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
        if (pcOspSipMsg == NULL)
        {
            CMS_ASSERT(pcOspSipMsg && "ospsip 消息为空\n");
            //TASKLOG(this->GetModuleID(), ERROR_LEV, "ospsip 消息为空\n");
            //nErrorCode = ERR_CMU_SIP_BODY_NULL;

            return *((CReq*)m_pcReq);
        }

        m_tFromUri = pcOspSipMsg->GetSipFromUri();
        m_pcReq->FromXml(pcOspSipMsg->GetMsgBody());
        m_tSipTransId = pcOspSipMsg->GetSipTransID();
        m_tSipDlgID = pcOspSipMsg->GetSipDlgID();

        return *((CReq*)m_pcReq);
    }

    bool DoPostMsgRsp(CEventRsp& cRsp)
    {
        cRsp.SetHeadFromReq(*m_pcReq);
        cRsp.SetErrorCode(m_nErrorCode);

        if( PROCMSG_OK != PostMsgRsp(m_tSipTransId, cRsp))
        {
            TASKLOG(this->GetModuleID(), ERROR_LEV, "发送%s失败！\n", cRsp.GetEventStr().c_str());
            //nErrorCode = ERR_POST_MSG_FAILED;
            return false;
        }

        //发送完成记得要重置m_tReqTransId,避免重复发送
        m_tSipTransId = INVALID_TRANS_ID;

        return true;
    }

	bool DoPostMsgRsp(const char* pstrXmlVar)
	{
        if (NULL == pstrXmlVar)
        {
            CMS_ASSERT(pstrXmlVar);
            return false;
        }

        string strXmlVar = pstrXmlVar;

		CEventRsp::SetHeadFromReq(strXmlVar, *m_pcReq);
        ReplaceXmlKeyText(strXmlVar, m_nErrorCode, "<errorCode>", "</errorCode>");
		
		if( PROCMSG_OK != PostMsgRsp(m_tSipTransId, strXmlVar))
		{
			TASKLOG(this->GetModuleID(), ERROR_LEV, "发送%s失败！\n", GetXmlNodeText(strXmlVar, "event").c_str());   //cRsp.GetEventStr()
			//nErrorCode = ERR_POST_MSG_FAILED;
			return false;
		}

		//发送完成记得要重置m_tReqTransId,避免重复发送
		m_tSipTransId = INVALID_TRANS_ID;

		return true;
	}

    bool DoPostRsp(EKDSIP_EVENT eSIPMethod, CEventRsp& cRsp)
    {
        cRsp.SetHeadFromReq(*m_pcReq);
        cRsp.SetErrorCode(m_nErrorCode);

        if( PROCMSG_OK != PostRsp(eSIPMethod, m_tSipTransId, cRsp))
        {
            TASKLOG(this->GetModuleID(), ERROR_LEV, "发送event[%s-%d]失败！\n", cRsp.GetEventStr().c_str(), cRsp.GetEventId());
            //nErrorCode = ERR_POST_MSG_FAILED;
            return false;
        }

        //发送完成记得要重置m_tReqTransId,避免重复发送
        m_tSipTransId = INVALID_TRANS_ID;

        return true;
    }

    template <class CReq>
    bool DoPostMsgReq(CReq &cReq, const TSipURI &tDstURI, bool bRouteFollowTopo= false, LPCSTR szDirectUAAddr = NULL)
    {
        TSipURI tRouteUri;
        if (CMS_SUCCESS != CheckDevRouter(tDstURI.GetURIString(), bRouteFollowTopo, tRouteUri))
        {
            return false;
        }

        u32 dwResult = PostMsgReq(cReq, tRouteUri, szDirectUAAddr);
        if(PROCMSG_OK != dwResult)
        {
            TASKLOG(this->GetModuleID(), ERROR_LEV, "发送event[%s-%d]失败！\n", cReq.GetEventStr().c_str(), cReq.GetEventId());

            //nErrorCode = ERR_POST_MSG_FAILED;
            return false;
        }

        return true;
    }

    template <class CReq>
    bool DoPostInDlgReq(EKDSIP_EVENT eSIPMethod, CReq &cReq, TKDSIP_DIALOG_ID tSipDlgID=INVALID_DIALOG_ID)
    {
        if (INVALID_DIALOG_ID == tSipDlgID)
        {
            tSipDlgID = m_tSipDlgID;
        }

        u32 dwResult = PostInDlgReq(eSIPMethod, cReq, tSipDlgID);
        if(PROCMSG_OK != dwResult)
        {
            TASKLOG(this->GetModuleID(), CRITICAL_LEV, "发送SIPMethod[%ld]到设备(DlgID[%ld]--DevUri[%s])失败：Result[%lu].\n", 
                eSIPMethod, m_tSipDlgID, this->GetFromUri().GetURIString().c_str(), dwResult);

            //nErrorCode = ERR_POST_MSG_FAILED;
            return false;
        }

        //m_tSipDlgID = INVALID_DIALOG_ID;

        return true;
    }

	void SetDialogTerminateErrocodeBySender(const TSipURI &tSipUri)
	{
		if( tSipUri.GetUser().empty() )
		{
			return ;
		}

		if ( tSipUri.GetUser() == DEV_TYPE_CMU )
		{
			if ( tSipUri.GetDomain() != GetDomainName() )
			{
				this->SetErrorCode(ERR_CMU_DIALOG_TERMINATE_BY_SUBDOMAIN_CMU );
			}
			return;
		}
		else if ( tSipUri.GetUser() == DEV_TYPE_GBS )
		{
			this->SetErrorCode(ERR_CMU_DIALOG_TERMINATE_BY_GBS );
			return;
		}

		string uriString = tSipUri.GetURIString();
		CMS_RESULT ret = CMS_SUCCESS;

		CLoginSession *pTask = NULL;
		if ( g_cCuiList.Find(uriString, pTask) )
		{
			TASKLOG(this->GetModuleID(), ERROR_LEV, "收到[cui@%s]Dialog异常终止\n", tSipUri.GetDomain().c_str());
			ret = ERR_CMU_DIALOG_TERMINATE_BY_CUI;
		}
		else if ( g_cVtduList.Find(uriString, pTask) )
		{
			TASKLOG(this->GetModuleID(), ERROR_LEV, "收到[vtdu@%s]Dialog异常终止\n", tSipUri.GetDomain().c_str());
			ret = ERR_CMU_DIALOG_TERMINATE_BY_VTDU;
		}
		else if ( g_cPuiList.Find(uriString, pTask) )
		{
			TASKLOG(this->GetModuleID(), ERROR_LEV, "收到[pui@%s]Dialog异常终止\n", tSipUri.GetDomain().c_str());
			ret = ERR_CMU_DIALOG_TERMINATE_BY_PUI;
		}

		if ( ret != CMS_SUCCESS )
		{
			this->SetErrorCode(ret);
		}

		return;
	}

	void SendOptFailedToUas( const string &sSessinName )
	{
		TOperateLogInfo tOptLogInfo;

		if ( this->GetErrorCode() == ERR_CMU_DIALOG_TERMINATE
			|| this->GetErrorCode() == ERR_CMU_DIALOG_TERMINATE_BY_CUI
			|| this->GetErrorCode() == ERR_CMU_DIALOG_TERMINATE_BY_VTDU
			|| this->GetErrorCode() == ERR_CMU_DIALOG_TERMINATE_BY_PUI
			|| this->GetErrorCode() == ERR_CMU_DIALOG_TERMINATE_BY_GBS
			|| this->GetErrorCode() == ERR_CMU_DIALOG_TERMINATE_BY_SUBDOMAIN_CMU)
		{
			std::ostringstream oss;
			oss << GetLanDesc( LAN_DESC_SWITCH_STOP_BY_BYE )
				<< " "
				<< GetLanDesc( LAN_DESC_ERROR_CODE )
				<< ":"
			    << (int)this->GetErrorCode();

			tOptLogInfo.operateDesc = oss.str();
		}
		else
		{
			return ;
		}

		if ( sSessinName == SDP_SESSION_PLAY)
		{
			tOptLogInfo.operateType = enUserOpTypeLiveStream;
		}
		else if ( sSessinName == SDP_SESSION_PLAYBACK)
		{
			tOptLogInfo.operateType = enUserOpTypeRecordPlayback;
		}
		else if ( sSessinName == SDP_SESSION_DOWNLOAD)
		{
			tOptLogInfo.operateType = enUserOpTypeRecordDownload ;
		}
		else if ( sSessinName == SDP_SESSION_AUDIOCALL)
		{
			tOptLogInfo.operateType = enUserOpTypeCall ;
		}
		else
		{
			return;
		}

		tOptLogInfo.userUri = "admin@" + GetDomainName();
		tOptLogInfo.userName = "admin@" + GetDomainName();
		tOptLogInfo.netAddr.empty();

		CCMSTime cCurtime;
		tOptLogInfo.happenedTime = cCurtime.ToString();

		tOptLogInfo.operateResult = (int)this->GetErrorCode();
		
		tOptLogInfo.netAddr = g_cCmuConf.m_cCmCfg.GetLocalIP();

		((CCmuCoreInst*)GetInstance())->GetUasLogTask()->AddOperateLog(tOptLogInfo);

		return;
	}

    bool ProcOspSipException(CMessage *const pcMsg)
    {
        switch(pcMsg->event)
        {
            //Invite会话终止
        case OSP_SIP_MSG_PROC_FAIL:
            {
                //发起的请求收到SIP层的错误应答
                COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
                this->SetErrorCode(ERR_CMU_DEV_NO_ROUTER);

                TASKLOG(this->GetModuleID(), ERROR_LEV, "收到SipMethod[%s]SIP层错误应答, SipErrorCode[%d]\n",
                    pcOspSipMsg->GetSipEventStr().c_str(), pcOspSipMsg->GetSipErrorCode());

                //this->PrintData();
            }
            break;
        case OSP_SIP_DIALOG_TERMINATE:
            {
                //SIP层Dialog异常终止通知
                COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
                this->SetErrorCode(ERR_CMU_DIALOG_TERMINATE);

                TASKLOG(this->GetModuleID(), ERROR_LEV, "收到SIP层Dialog异常终止, SipFromUri[%s], SipErrorCode[%d], StatName[%s], ErrorCode[%d]\n",
					pcOspSipMsg->GetSipFromUri().GetURIString().c_str(), pcOspSipMsg->GetSipErrorCode(),
					this->GetStateName(this->GetState()), this->GetErrorCode());

                m_tSipDlgID = INVALID_DIALOG_ID;
            }
            break;

        default:
            {
                TASKLOG(this->GetModuleID(), WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

                return false;
            }
        }

        return true;
    }

    bool ProcByeReq(CMessage *const pcMsg)
    {
        COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

        s32 nByeReason = CMS_SUCCESS;
        if (pcOspSipMsg->GetMsgBodyLen() != 0)
        {
            CByeReq cByeReq;
            pcOspSipMsg->GetMsgBody(cByeReq);

            nByeReason = cByeReq.GetReason();
        }

        if (CMS_SUCCESS != nByeReason)
        {
            this->SetErrorCode(nByeReason);
        }
        else
        {
            this->SetErrorCode(ERR_CMU_STOP_BY_BYE);
        }

        TKDSIP_DIALOG_ID tReqDlgId = pcOspSipMsg->GetSipDlgID();
        
        TASKLOG(this->GetModuleID(), CRITICAL_LEV, "收到设备[%s]DlgID[%lu]的ByeReq, Reason[%ld]！\n", 
            pcOspSipMsg->GetSipFromUri().GetURIString().c_str(), tReqDlgId, nByeReason);

        m_tSipDlgID = INVALID_DIALOG_ID;

        ////////////////////////////////////////////////////////////
        //向请求方回应应答
        CByeRsp cRsp;

        u32 dwResult = PostRsp(KDSIP_EVENT_INVITE_BYE_RSP, pcOspSipMsg->GetSipTransID(), cRsp);
        TASKLOG(this->GetModuleID(), CRITICAL_LEV, "发送KDSIP_EVENT_INVITE_BYE_RSP, ResultCode[%lu].\n", dwResult);

        ////////////////////////////////////////////////////////////
        //停止交换任务并且释放资源
        //ReleaseResource();    //析构时会调用该函数，这里不再调用
        
        return true;
    }

    bool ProcByeRsp(CMessage *const pcMsg)
    {
        COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

        TKDSIP_DIALOG_ID tReqDlgId = pcOspSipMsg->GetSipDlgID();

        TASKLOG(this->GetModuleID(), CRITICAL_LEV, "收到设备[%s]DlgID[%lu]的ByeRsp！\n", 
            pcOspSipMsg->GetSipFromUri().GetURIString().c_str(), tReqDlgId);

        return true;
    }

    CMS_RESULT CheckDevRouter(IN const string& strDevUri, IN bool bRouteFollowTopo, 
        OUT TSipURI& tRouteUri, OUT CLoginSession** ppLoginSsn = NULL)
    {
        tRouteUri = GetDevRouter(strDevUri, bRouteFollowTopo);

        if (!tRouteUri.IsValidURI())
        {
            this->SetErrorCode(ERR_CMU_DEV_NO_ROUTER);

            return this->GetErrorCode();
        }

        //异域设备检测域之间的连通性
        if (!IsLocalDomainByDevUri(strDevUri))
        {
            CLoginSession* ptCmuTask = NULL;
            g_cCmuList.Find(tRouteUri.GetURIString(), ptCmuTask);
            if (NULL == ptCmuTask)
            {
                TASKLOG(this->GetModuleID(), ERROR_LEV, "Dev[%s]对应的RouteUri[%s]未注册!\n", 
                    strDevUri.c_str(), tRouteUri.GetURIString().c_str());

                this->SetErrorCode(ERR_CMU_DEV_NO_ROUTER);

                return this->GetErrorCode();
            }

            TASKLOG(this->GetModuleID(), EVENT_LEV, "CmuLoginSess[%s], TaskNO[%u], State[%s]\n",
                tRouteUri.GetURIString().c_str(), ptCmuTask->GetTaskNO(), ptCmuTask->GetStateName(ptCmuTask->GetState()));

            //跨域事务都需要登记到cmu登录会话中
            //对于长期事务，添加是为了域断链时释放长期事务的资源
            //对于瞬时事务，添加是为了cmu登录成功后即时通知到本事务
            //事务删除时，析构函数中需要去除本事务在cmu登录会话中的登记
            ptCmuTask->AddTask(this);

            if (NULL != m_ptReqCmuSsn)
            {
                m_ptReqCmuSsn->DelTask(this);
            }
            m_ptReqCmuSsn = ptCmuTask;

            //cmu未连接成功，则跳出处理，等待连接成功通知
            if (ptCmuTask->GetState() != CCmuConnecterTask::Service)
            {
                this->SetErrorCode(ERR_CMU_CMU_NOT_CONN);
            }

            if (NULL != ppLoginSsn)
            {
                *ppLoginSsn = ptCmuTask;
            }
        }
        else
        {
            if (NULL != ppLoginSsn)
            {
                //本域的设备，直接取设备本身的登录session，如pu的路由是pui但是loginsession取pu自己的.
                *ppLoginSsn = GetLoginSession(strDevUri);

                if(NULL == *ppLoginSsn)
                {
                    TASKLOG(this->GetModuleID(), ERROR_LEV, "Dev[%s] not login!\n", strDevUri.c_str());

                    this->SetErrorCode(ERR_CMU_SWITCH_DEV_NOT_AVAILABLE);
                }
            }
        }

        return this->GetErrorCode();
    }

    void CallbackToTask(CMessage *const pcMsg, CTask* ptTask)
    {
        if (ptTask != NULL)
        {
            if (ptTask->ProcMsg(pcMsg) == PROCMSG_DEL)
            {
                delete ptTask;
                ptTask = NULL;
            }
        }
    }

    void CallbackToTask(u16 event, const void* content, u16 length, CTask* ptTask)
    {
        CMessage cOspMsg;
        cOspMsg.event = event;
        cOspMsg.content = (u8*)content;
        cOspMsg.length = length;

        CallbackToTask(&cOspMsg, ptTask);
    }


    //void CallbackToTasks(CMessage *const pcMsg)
    //{
    //    CTask* ptTask = NULL;
    //    Iterator pPos;
    //    while (!pPos.End())
    //    {
    //        if (m_tTaskList.Iterate(pPos, ptTask) && ptTask != NULL)
    //        {
    //            if (ptTask->ProcMsg(pcMsg) == PROCMSG_DEL)
    //            {
    //                delete ptTask;
    //                ptTask = NULL;
    //            }
    //        }
    //    }
    //}

    //void CallbackToChildTasks(u16 event, const void* content, u16 length)
    //{
    //    CMessage cOspMsg;
    //    cOspMsg.event = event;
    //    cOspMsg.content = (u8*)content;
    //    cOspMsg.length = length;

    //    CallbackToChildTask(&cOspMsg);
    //}

	/////////////////////////////////////////////////////////////
	//成员属性
public:

    //m_pcReq
    inline CEventReq* GetReq()
    {
        return m_pcReq;
    }
    template<class CReq>
    CReq* GetRealReq()
    {
        return (CReq*)m_pcReq;
    }

    //m_pcRsp
    inline CEventRsp* GetRsp()
    {
        return m_pcRsp;
    }
	template<class CRsp>
	CRsp* GetRealRsp()
	{
		return (CRsp*)m_pcRsp;
	}

    //m_tFromUri
    inline const TSipURI& GetFromUri() const
    {
        return m_tFromUri;
    }

    //m_tSipDlgID
    inline TKDSIP_DIALOG_ID GetSipDlgID() const
    {
        return m_tSipDlgID;
    }
    inline void SetSipDlgID(TKDSIP_DIALOG_ID var)
    {
        m_tSipDlgID = var;
    }

    //m_tSipTransId
    inline TKDSIP_TRANS_ID GetSipTransID() const
    {
        return m_tSipTransId;
    }

    //m_nModuleID
    inline u8 GetModuleID() const
    {
        return (u8)m_nModuleID;
    }
    inline void SetModuleID(int var)
    {
        m_nModuleID = var;
    }

    //m_nErrorCode
    inline CMS_RESULT GetErrorCode() const
    {
        return m_nErrorCode;
    }
    inline void SetErrorCode(CMS_RESULT var)
    {
        m_nErrorCode = var;
    }

    virtual void Clear()
    {
        if (m_pcReq)
        {
            SAFE_DEL_PTR(m_pcReq);
        }

        m_tFromUri.Clear();
        m_tSipTransId = INVALID_TRANS_ID;
        m_tSipDlgID = INVALID_DIALOG_ID;

        m_nErrorCode = CMS_SUCCESS;
    }

    //用private限定，不允许继承类直接修改成员
private:    
    CEventReq*              m_pcReq;
    CEventRsp*              m_pcRsp;

    //TOspSipMsgHead          m_tOspSipMsgHead;
    TSipURI                 m_tFromUri;
    TKDSIP_TRANS_ID         m_tSipTransId;
    TKDSIP_DIALOG_ID        m_tSipDlgID;

    int                     m_nModuleID;    //目前主要用于日志的打印控制
    CMS_RESULT              m_nErrorCode;
    CLoginSession*          m_ptReqCmuSsn;
};

#endif  //#ifndef _CMU_TASK_H