/*========================================================================================
模块名    ：cmu
文件名    ：pureqrspntftask.h
相关文件  ：
实现功能  ：pu请求-应答-通知类型(查询类)事务模板
作者      ：fanxg
版权      ：<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2012/09/18         V1.0              fanxg                                  新建文件
=========================================================================================*/
#ifndef _PU_REQ_RSP_NTF_TASK_H
#define _PU_REQ_RSP_NTF_TASK_H

#include "cms/ospsip/ospsip.h"
#include "cmuconst.h"
#include "cmudata.h"
#include "puregtask.h"
#include "cmuconnecttask.h"
#include "cmucoreapp.h"

template<class CNtfReq, class CNtfRsp>
class CPuNtfTask : public CSipTask
{
public:
    enum
    {
        Wait = UNKNOWN_STATE+1,
        Rsp,
    };

public:
    CPuNtfTask(CInstExt* pcInst) : CSipTask(pcInst)
    {
        m_nTrySendNum = 0;
    }
    virtual ~CPuNtfTask()
    {

    }

public:
    virtual const char* GetObjName() const
    {
        return "CPuNtfTask";
    }

    virtual const char* GetStateName(u32 dwState) const
    {
        switch (dwState)
        {
        case Wait:
            return "Wait";
        case Rsp:
            return "Rsp";
        default:
            break;
        }
        return "Unknown State";
    }

public:
    virtual void InitStateMachine()
    {
        CStateProc cWaitProc;
        cWaitProc.ProcMsg = (CTask::PFProcMsg)&CPuNtfTask::OnWait;
        cWaitProc.ProcErrMsg = (CTask::PFProcMsg)&CPuNtfTask::OnWait;
        cWaitProc.TimerPoll = (CTask::PFTimerPoll)&CPuNtfTask::OnWaitTimer;
        AddRuleProc(Wait, cWaitProc);

        CStateProc cRspProc;
        cRspProc.ProcMsg = (CTask::PFProcMsg)&CPuNtfTask::OnRsp;
        cRspProc.ProcErrMsg = (CTask::PFProcMsg)&CPuNtfTask::OnRsp;
        cRspProc.TimerPoll = (CTask::PFTimerPoll)&CPuNtfTask::OnRspTimer;
        AddRuleProc(Rsp, cRspProc);

        NextState(Wait);
    }
    virtual void PrintData() const
    {
        CTask::PrintData();

        //直接打印xml数据
        OspPrintf(TRUE, FALSE, "  %s\n", m_tReq.ToXml().c_str());
    }

public:
    //状态处理
    u32 OnWait(CMessage *const pcMsg)
    {
        u32 dwRet = PROCMSG_DEL;

        TASKLOG(CMU_MOD_PU, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

        return dwRet;
    }
    u32 OnWaitTimer()
    {
        return TIMERPOLL_DONE;
    }

    u32 OnRsp(CMessage *const pcMsg)
    {
        u32 dwRet = PROCMSG_FAIL;
        if ((u16)m_tRsp.GetEventId() == pcMsg->event)
        {
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            pcOspSipMsg->GetMsgBody(m_tRsp);

            if (m_tRsp.GetErrorCode() == CMS_SUCCESS)
            {
                TASKLOG(CMU_MOD_PU, PROGRESS_LEV, "通知成功 Msg[%s-%d]\n", 
                    OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            }
            else
            {
                TASKLOG(CMU_MOD_PU, PROGRESS_LEV, "通知失败，errCode[%d], Msg[%s-%d]\n", 
                    m_tRsp.GetErrorCode(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            }
            
            dwRet = PROCMSG_DEL;
        }
        else
        {
            TASKLOG(CMU_MOD_PU, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }

        return dwRet;
    }
    u32 OnRspTimer()
    {
        if (GetCurStateHoldTime() > CMU_REQ_RSP_TASK_TIMEOUT)
        {
            if (m_nTrySendNum >= NTF_TRY_SEND_NUM)
            {
                //尝试三次后还失败，直接删除通知事务
                TASKLOG(CMU_MOD_PU, ERROR_LEV, "通知[%d]次都失败，直接删除该事务!\n",
                    m_nTrySendNum);

                return TIMERPOLL_DEL;
            }

            TASKLOG(CMU_MOD_PU, ERROR_LEV, "通知超时，重试...!\n");
            StartNtf();
            return TIMERPOLL_DONE;
        }

        return TIMERPOLL_DONE;
    }


public:
    //业务处理 
    void StartNtf()
    {
        NextState(Rsp);
        PostMsgReq(m_tReq, m_tDstUri);

        m_nTrySendNum++;
    }

public:

    //数据获取   
    const CNtfReq& GetReq() const
    {
        return m_tReq;
    }
    CNtfReq& GetReq()
    {
        return m_tReq;
    }
    void SetReq(const CNtfReq& tReq)
    {
        m_tReq = tReq;
    }

    const TSipURI& GetDstUri() const
    {
        return m_tDstUri;
    }
    TSipURI& GetDstUri()
    {
        return m_tDstUri;
    }
    void SetDstUri(const TSipURI& tUri)
    {
        m_tDstUri = tUri;
    }

private:
    CNtfReq m_tReq;
    CNtfRsp m_tRsp;
    TSipURI m_tDstUri;
    s32 m_nTrySendNum;
};

template<class CReq, class CRsp, class CNtfReq, class CNtfRsp>
class CPuReqRspNtfTask : public CSipTask
{
public:
    enum
    {
        Req = UNKNOWN_STATE+1,
        WaitRsp,
        Ntf,
    };

    typedef CPuNtfTask<CNtfReq, CNtfRsp>  CNtfTask;

public:
    CPuReqRspNtfTask(CInstExt* pInst) : CSipTask(pInst)
    {
        m_tSipTransId = INVALID_TRANS_ID;
    }

    ~CPuReqRspNtfTask()
    {	
        ReleaseResource();
    }

    void ReleaseResource()
    {
        //从查询事务列表中删除
        g_cPuQueryTaskList.Erase(m_tReq.GetQueryTag());

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
        return "CPuReqRspNtfTask";
    }

    virtual const char* GetStateName(u32 dwState) const
    {
        switch (dwState)
        {
        case Req:
            return "Req";
        case WaitRsp:
            return "WaitRsp";
        case Ntf:
            return "Ntf";
        default:
            break;
        }
        return "Unknown State";
    }


    virtual void InitStateMachine()
    {
        CStateProc cReqProc;
        cReqProc.ProcMsg = (CTask::PFProcMsg)&CPuReqRspNtfTask::OnReq;
        cReqProc.ProcErrMsg = (CTask::PFProcMsg)&CPuReqRspNtfTask::OnReq;
        cReqProc.TimerPoll = (CTask::PFTimerPoll)&CPuReqRspNtfTask::OnReqTimer;
        AddRuleProc(Req, cReqProc);

        CStateProc cWaitRspProc;
        cWaitRspProc.ProcMsg = (CTask::PFProcMsg)&CPuReqRspNtfTask::OnWaitRsp;
        cWaitRspProc.ProcErrMsg = (CTask::PFProcMsg)&CPuReqRspNtfTask::OnWaitRsp;
        cWaitRspProc.TimerPoll = (CTask::PFTimerPoll)&CPuReqRspNtfTask::OnWaitRspTimer;
        AddRuleProc(WaitRsp, cWaitRspProc);

        CStateProc cNtfProc;
        cNtfProc.ProcMsg = (CTask::PFProcMsg)&CPuReqRspNtfTask::OnNtf;
        cNtfProc.ProcErrMsg = (CTask::PFProcMsg)&CPuReqRspNtfTask::OnNtf;
        cNtfProc.TimerPoll = (CTask::PFTimerPoll)&CPuReqRspNtfTask::OnNtfTimer;
        AddRuleProc(Ntf, cNtfProc);

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
            m_tFromUri = pcOspSipMsg->GetSipFromUri();

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
                CPuData *pcPuData = CCmuData::GetInstance()->GetPuDataByUri(cReq.GetDevUri());
                if (NULL == pcPuData)
                {
                    TASKLOG(CMU_MOD_PU, ERROR_LEV, "Dev[%s]不在线, Msg[%s-%d]请求失败\n", 
                        cReq.GetDevUri().c_str(), m_tReq.GetEventStr().c_str(), m_tReq.GetEventId());

                    SendRsp(ERR_CMU_PU_NOT_IN_TOP);
                    dwRet = PROCMSG_DEL;
                    return dwRet;
                }

                CPuRegBaseTask *pcPuRegTask = pcPuData->GetPuRegTask();
                if (pcPuRegTask == NULL)
                {
                    TASKLOG(CMU_MOD_PU, ERROR_LEV, "Dev[%s]不在线, Msg[%s-%d]请求失败\n", 
                        cReq.GetDevUri().c_str(), m_tReq.GetEventStr().c_str(), m_tReq.GetEventId());

                    SendRsp(ERR_CMU_PU_NOT_ONLINE);
                    dwRet = PROCMSG_DEL;
                    return dwRet;
                }

                //发往pu的消息需要将session改写pu的登录session
                cReq.SetSession(pcPuRegTask->GetSession());
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

            g_cPuQueryTaskList.Insert(m_tReq.GetQueryTag(), this);
        }
        else if (pcMsg->event == CMU_CMU_CONNECT_SUCCESS)
        {
            //异域cmu登录成功, 可以发送请求
            TSipURI tPuRouterUri = GetPuRouter(m_tReq.GetDevUri());
            if (!IsLocalDomainByDevUri(tPuRouterUri))  //校验一下目的设备是否在异域
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

            TASKLOG(CMU_MOD_PU, ERROR_LEV, "对设备[%s]请求失败, 发生SIP层错误，sip status code[%d]\n",
                m_tReq.GetDevUri().c_str(), pcOspSipMsg->GetSipErrorCode());

            SendRsp((u32)pcOspSipMsg->GetSipErrorCode());          
            return dwRet;
        }

        CRsp cRsp;
        if ((u16)cRsp.GetEventId() == pcMsg->event)
        {
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            pcOspSipMsg->GetMsgBody(cRsp);
            cRsp.SetSession(m_tReq.GetSession());
            SendRsp(cRsp);

            if (cRsp.GetErrorCode() == CMS_SUCCESS)
            {
                TASKLOG(CMU_MOD_PU, CRITICAL_LEV, "QryTotalNum[%d], Msg[%s-%d]\n", 
                    cRsp.GetTotalEntryNum(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

                if (cRsp.GetTotalEntryNum() > 0)
                {
                    NextState(Ntf);
                    dwRet = PROCMSG_OK;
                }
            }
            else
            {
                TASKLOG(CMU_MOD_PU, ERROR_LEV, "查询失败，errCode[%d], Msg[%s-%d]\n", 
                    cRsp.GetErrorCode(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            }
            
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

    u32 OnNtf(CMessage *const pcMsg)
    {
        u32 dwRet = PROCMSG_OK;
        CNtfReq cNtfReq;
        if ((u16)cNtfReq.GetEventId() == pcMsg->event)
        {
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
            pcOspSipMsg->GetMsgBody(cNtfReq);
            cNtfReq.SetSession(m_tReq.GetSession());
            SendNtfReq(cNtfReq);

            //应答pui通知收到
            CNtfRsp cNtfRsp;
            cNtfRsp.SetHeadFromReq(cNtfReq);
            cNtfRsp.SetErrorCode(CMS_SUCCESS);
            PostMsgRsp(pcOspSipMsg->GetSipTransID(), cNtfRsp);

            //通知结束，删除事务
            if (cNtfReq.IsLastNtf())
            {
                TASKLOG(CMU_MOD_PU, CRITICAL_LEV, "Dev[%s]查询Msg[%s-%d]请求，全部通知结束\n", 
                    m_tReq.GetDevUri().c_str(), m_tReq.GetEventStr().c_str(), m_tReq.GetEventId());

                dwRet = PROCMSG_DEL;
            }
        }
        else
        {
            TASKLOG(CMU_MOD_PU, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            dwRet = PROCMSG_FAIL;
        }

        return dwRet;
    }

    u32 OnNtfTimer()
    {
        return TIMERPOLL_DONE;
    }

    void SendReq(const CReq& cReq, const TSipURI& tDstUri)
    {
        if (IsLocalDomainByDevUri(tDstUri))
        {
            //本域直接投递
            PostMsgReq(cReq, tDstUri);
            NextState(WaitRsp);
        }
        else
        {
            //异域处理，如果没有登陆过异域需要先登录，登录成功后再发起操作请求
            TSipURI tCmuUri = GetCmuUriFromDevUri(tDstUri);
            CLoginSession* ptCmuTask = NULL;
            g_cCmuList.Find(tCmuUri.GetURIString(), ptCmuTask);
            CMS_ASSERT(NULL != ptCmuTask && "ptCmuTask = NULL!!!");
            //cmu连接成功后才发起订阅
            if (ptCmuTask->GetState() == CCmuConnecterTask::Service)
            {
                PostMsgReq(cReq, tCmuUri);
                NextState(WaitRsp);
            }

            //跨域事务都需要登记到cmu登录会话中
            //对于长期事务，添加是为了域断链时释放长期事务的资源
            //对于瞬时事务，添加是为了cmu登录成功后即时通知到本事务
            //事务删除时，析构函数中需要去除本事务在cmu登录会话中的登记
            ptCmuTask->AddTask(this);
        }
    }

    bool SendRsp(u32 dwError)
    {
        CRsp cRsp;
        cRsp.SetSeqNum(m_tReq.GetSeqNum());
        cRsp.SetSession(m_tReq.GetSession());
        cRsp.SetErrorCode(dwError);

        return (PROCMSG_OK == PostMsgRsp(m_tSipTransId, cRsp));
    }

    bool SendRsp(const CRsp& cRsp)
    {
        return (PROCMSG_OK == PostMsgRsp(m_tSipTransId, cRsp));
    }

    bool SendNtfReq(const CNtfReq& cNtfReq)
    {
        CNtfTask* pcNtfTask = new CNtfTask(GetInstance());
        pcNtfTask->InitStateMachine();
        pcNtfTask->SetReq(cNtfReq);
        pcNtfTask->SetDstUri(m_tFromUri);
        pcNtfTask->StartNtf();  

        return true;
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

private:
    TKDSIP_TRANS_ID m_tSipTransId;   //用于查询应答路由
    TSipURI m_tFromUri;              //用于查询通知路由
    CReq m_tReq;
};


//前端录像查询
typedef CPuReqRspNtfTask<CPuRecQryReq, CPuRecQryRsp, CPuRecQryNtfReq, CPuRecQryNtfRsp> CPuRecordQryTask;

//前端图片查询
typedef CPuReqRspNtfTask<CPuPicQryReq, CPuPicQryRsp, CPuPicQryNtfReq, CPuPicQryNtfRsp> CPuPictureQryTask;

//前端透明数据查询
typedef CPuReqRspNtfTask<CPuTransparentQryReq, CPuTransparentQryRsp, CPuTransparentQryNtfReq, CPuTransparentQryNtfRsp> CPuTransparentQryTask;


#endif  //#ifndef _PU_REQ_RSP_NTF_TASK_H