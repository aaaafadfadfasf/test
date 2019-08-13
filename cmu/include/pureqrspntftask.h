/*========================================================================================
ģ����    ��cmu
�ļ���    ��pureqrspntftask.h
����ļ�  ��
ʵ�ֹ���  ��pu����-Ӧ��-֪ͨ����(��ѯ��)����ģ��
����      ��fanxg
��Ȩ      ��<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
�޸ļ�¼��
����               �汾              �޸���             �߶���              �޸ļ�¼
2012/09/18         V1.0              fanxg                                  �½��ļ�
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

        //ֱ�Ӵ�ӡxml����
        OspPrintf(TRUE, FALSE, "  %s\n", m_tReq.ToXml().c_str());
    }

public:
    //״̬����
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
                TASKLOG(CMU_MOD_PU, PROGRESS_LEV, "֪ͨ�ɹ� Msg[%s-%d]\n", 
                    OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            }
            else
            {
                TASKLOG(CMU_MOD_PU, PROGRESS_LEV, "֪ͨʧ�ܣ�errCode[%d], Msg[%s-%d]\n", 
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
                //�������κ�ʧ�ܣ�ֱ��ɾ��֪ͨ����
                TASKLOG(CMU_MOD_PU, ERROR_LEV, "֪ͨ[%d]�ζ�ʧ�ܣ�ֱ��ɾ��������!\n",
                    m_nTrySendNum);

                return TIMERPOLL_DEL;
            }

            TASKLOG(CMU_MOD_PU, ERROR_LEV, "֪ͨ��ʱ������...!\n");
            StartNtf();
            return TIMERPOLL_DONE;
        }

        return TIMERPOLL_DONE;
    }


public:
    //ҵ���� 
    void StartNtf()
    {
        NextState(Rsp);
        PostMsgReq(m_tReq, m_tDstUri);

        m_nTrySendNum++;
    }

public:

    //���ݻ�ȡ   
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
        //�Ӳ�ѯ�����б���ɾ��
        g_cPuQueryTaskList.Erase(m_tReq.GetQueryTag());

        if (IsLocalDomainByDevUri(m_tReq.GetDevUri()))
        {
            return;
        }

        //���������Ҫɾ����¼����
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

        //ֱ�Ӵ�ӡxml����
        OspPrintf(TRUE, FALSE, "  %s\n", m_tReq.ToXml().c_str());
    }

    //״̬����
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

            //ȷ��pu·��,ת����Ϣ
            TSipURI tPuRouterUri = GetPuRouter(cReq.GetDevUri());
            if (tPuRouterUri.IsNull())
            {
                TASKLOG(CMU_MOD_PU, WARNING_LEV, "Dev[%s]�޷�·��, Msg[%s-%d]����ʧ��\n", 
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
                    TASKLOG(CMU_MOD_PU, ERROR_LEV, "Dev[%s]������, Msg[%s-%d]����ʧ��\n", 
                        cReq.GetDevUri().c_str(), m_tReq.GetEventStr().c_str(), m_tReq.GetEventId());

                    SendRsp(ERR_CMU_PU_NOT_IN_TOP);
                    dwRet = PROCMSG_DEL;
                    return dwRet;
                }

                CPuRegBaseTask *pcPuRegTask = pcPuData->GetPuRegTask();
                if (pcPuRegTask == NULL)
                {
                    TASKLOG(CMU_MOD_PU, ERROR_LEV, "Dev[%s]������, Msg[%s-%d]����ʧ��\n", 
                        cReq.GetDevUri().c_str(), m_tReq.GetEventStr().c_str(), m_tReq.GetEventId());

                    SendRsp(ERR_CMU_PU_NOT_ONLINE);
                    dwRet = PROCMSG_DEL;
                    return dwRet;
                }

                //����pu����Ϣ��Ҫ��session��дpu�ĵ�¼session
                cReq.SetSession(pcPuRegTask->GetSession());
            }
            else
            {
                if (!IsDomainInTopByDevUri(tPuRouterUri))
                {
                    TASKLOG(CMU_MOD_PU, WARNING_LEV, "Dev[%s]������δ����, Msg[%s-%d]����ʧ��\n", 
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
            //����cmu��¼�ɹ�, ���Է�������
            TSipURI tPuRouterUri = GetPuRouter(m_tReq.GetDevUri());
            if (!IsLocalDomainByDevUri(tPuRouterUri))  //У��һ��Ŀ���豸�Ƿ�������
            {
                SendReq(m_tReq, tPuRouterUri);
            }
            else
            {
                TASKLOG(CMU_MOD_PU, ERROR_LEV, "�������豸[%s], Receive Msg[%s-%d]\n", 
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
        //�ö�ʱ���������ʱ���õ�
        if (GetCurStateHoldTime() > CMU_CONNECT_CMU_TIMEOUT)
        {
            TSipURI tCmuUri = GetCmuUriFromDevUri(m_tReq.GetDevUri());
            TASKLOG(CMU_MOD_PU, WARNING_LEV, "��¼����cmu[%s]��ʱ, Msg[%s-%d]����ʧ��\n", 
                tCmuUri.GetURIString().c_str(), m_tReq.GetEventStr().c_str(), m_tReq.GetEventId());
            SendRsp(ERR_CMU_CMU_NOT_CONN);
            return TIMERPOLL_DEL;
        }

        return TIMERPOLL_DONE;
    }

    u32 OnWaitRsp(CMessage *const pcMsg)
    {
        u32 dwRet = PROCMSG_DEL;

        //SIP�������
        if (pcMsg->event == OSP_SIP_MSG_PROC_FAIL)
        {
            //����������յ�SIP��Ĵ���Ӧ��
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

            TASKLOG(CMU_MOD_PU, ERROR_LEV, "���豸[%s]����ʧ��, ����SIP�����sip status code[%d]\n",
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
                TASKLOG(CMU_MOD_PU, ERROR_LEV, "��ѯʧ�ܣ�errCode[%d], Msg[%s-%d]\n", 
                    cRsp.GetErrorCode(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            }
            
        }
        else
        {
            TASKLOG(CMU_MOD_PU, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            dwRet = PROCMSG_FAIL;
        }

        //�����������ɾ��֮
        return dwRet;
    }

    u32 OnWaitRspTimer()
    {
        //Ӧ��ʱ
        if (GetCurStateHoldTime() > CMU_REQ_RSP_TASK_TIMEOUT)
        {
            TASKLOG(CMU_MOD_PU, WARNING_LEV, "Dev[%s]Ӧ��ʱ, Msg[%s-%d]����ʧ��\n", 
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

            //Ӧ��pui֪ͨ�յ�
            CNtfRsp cNtfRsp;
            cNtfRsp.SetHeadFromReq(cNtfReq);
            cNtfRsp.SetErrorCode(CMS_SUCCESS);
            PostMsgRsp(pcOspSipMsg->GetSipTransID(), cNtfRsp);

            //֪ͨ������ɾ������
            if (cNtfReq.IsLastNtf())
            {
                TASKLOG(CMU_MOD_PU, CRITICAL_LEV, "Dev[%s]��ѯMsg[%s-%d]����ȫ��֪ͨ����\n", 
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
            CMS_ASSERT(NULL != ptCmuTask && "ptCmuTask = NULL!!!");
            //cmu���ӳɹ���ŷ�����
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
    TKDSIP_TRANS_ID m_tSipTransId;   //���ڲ�ѯӦ��·��
    TSipURI m_tFromUri;              //���ڲ�ѯ֪ͨ·��
    CReq m_tReq;
};


//ǰ��¼���ѯ
typedef CPuReqRspNtfTask<CPuRecQryReq, CPuRecQryRsp, CPuRecQryNtfReq, CPuRecQryNtfRsp> CPuRecordQryTask;

//ǰ��ͼƬ��ѯ
typedef CPuReqRspNtfTask<CPuPicQryReq, CPuPicQryRsp, CPuPicQryNtfReq, CPuPicQryNtfRsp> CPuPictureQryTask;

//ǰ��͸�����ݲ�ѯ
typedef CPuReqRspNtfTask<CPuTransparentQryReq, CPuTransparentQryRsp, CPuTransparentQryNtfReq, CPuTransparentQryNtfRsp> CPuTransparentQryTask;


#endif  //#ifndef _PU_REQ_RSP_NTF_TASK_H