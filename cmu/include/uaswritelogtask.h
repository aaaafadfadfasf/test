/*========================================================================================
ģ����    ��cmu
�ļ���    ��uaswritelogtask.h
����ļ�  ��
ʵ�ֹ���  ����uasд��־������ģ�壬�����豸��־�͸澯��־
����      ��fanxg
��Ȩ      ��<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
�޸ļ�¼��
����               �汾              �޸���             �߶���              �޸ļ�¼
2012/11/01         V1.0              fanxg                                  �½��ļ�
=========================================================================================*/
#ifndef _UAS_WRITE_LOG_TASK_H
#define _UAS_WRITE_LOG_TASK_H

#include "cms/ospsip/ospsip.h"
#include "cms/uas/uas_proto.h"
#include "cmuconst.h"
#include "cmudata.h"
#include "cmucoreapp.h"

template<class CReq, class CRsp>
class CUasWriteLogTask : public CSipTask
{
public:
    enum
    {
        Wait = UNKNOWN_STATE+1,
        Rsp,
    };

public:
    CUasWriteLogTask(CInstExt* pcInst) : CSipTask(pcInst)
    {
        m_nTrySendNum = 0;
    }
    virtual ~CUasWriteLogTask()
    {

    }

public:
    virtual const char* GetObjName() const
    {
        return "CUasWriteLogTask";
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
        cWaitProc.ProcMsg = (CTask::PFProcMsg)&CUasWriteLogTask::OnWait;
        cWaitProc.ProcErrMsg = (CTask::PFProcMsg)&CUasWriteLogTask::OnWait;
        cWaitProc.TimerPoll = (CTask::PFTimerPoll)&CUasWriteLogTask::OnWaitTimer;
        AddRuleProc(Wait, cWaitProc);

        CStateProc cRspProc;
        cRspProc.ProcMsg = (CTask::PFProcMsg)&CUasWriteLogTask::OnRsp;
        cRspProc.ProcErrMsg = (CTask::PFProcMsg)&CUasWriteLogTask::OnRsp;
        cRspProc.TimerPoll = (CTask::PFTimerPoll)&CUasWriteLogTask::OnRspTimer;
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

        TASKLOG(CMU_MOD_UAS, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

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
                TASKLOG(CMU_MOD_UAS, EVENT_LEV, "��uasд��־�ɹ� Msg[%s-%d]\n", 
                    OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            }
            else
            {
                TASKLOG(CMU_MOD_UAS, EVENT_LEV, "��uasд��־ʧ�ܣ�errCode[%d], Msg[%s-%d]\n", 
                    m_tRsp.GetErrorCode(), OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            }
            
            dwRet = PROCMSG_DEL;
        }
        else
        {
            TASKLOG(CMU_MOD_UAS, WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
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
                TASKLOG(CMU_MOD_UAS, ERROR_LEV, "��uasд��־[%d]�ζ�ʧ�ܣ�ֱ��ɾ��������!\n",
                    m_nTrySendNum);

                return TIMERPOLL_DEL;
            }

            TASKLOG(CMU_MOD_UAS, ERROR_LEV, "��uasд��־��ʱ������...!\n");
            StartWrite();
            return TIMERPOLL_DONE;
        }

        return TIMERPOLL_DONE;
    }


public:
    //ҵ���� 
    void StartWrite()
    {
        NextState(Rsp);
        PostMsgReq(m_tReq, m_tDstUri);

        m_nTrySendNum++;
    }

public:

    //���ݻ�ȡ   
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
    CReq m_tReq;
    CRsp m_tRsp;
    TSipURI m_tDstUri;
    s32 m_nTrySendNum;
};

//д�豸��־
typedef CUasWriteLogTask<CDeviceLogAddReq, CDeviceLogAddRsp> CUasWriteDevLogTask;

//д�澯��־
typedef CUasWriteLogTask<CAlarmLogAddReq, CAlarmLogAddRsp> CUasWriteAlarmLogTask;

//д�û�������־
typedef CUasWriteLogTask<COperateLogAddReq, COperateLogAddRsp> CUasWriteUserOptLogTask;

#endif  //#ifndef _UAS_WRITE_LOG_TASK_H