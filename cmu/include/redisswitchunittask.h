
#ifndef _REDIS_SWITCH_UNIT_TASK_H
#define _REDIS_SWITCH_UNIT_TASK_H

#include <set>
using namespace std;

#include "cms/ospsip/ospsip.h"
#include "cms/cms_struct.h"
#include "cms/cmu/cmu_struct.h"
#include "cms/cmu/cmu_proto.h"
#include "sdk/redissdk/switch_struct.h"
#include "loginsession.h"
#include "cmuconst.h"
#include "cmucoreapp.h"

struct TDstSwitchKey
{
public:
    TDstSwitchKey() {}
    TDstSwitchKey(const TSdpSessionName &tSession, const TChannel &tChn) : tSessionName(tSession), tDstChn(tChn) {}

    bool operator<(const TDstSwitchKey &tKey) const
    {
        if (this == &tKey)
        {
            return false;
        }
        else
        {
            if (tSessionName < tKey.tSessionName)
            {
                return true;
            }
            else if (tKey.tSessionName < tSessionName)
            {
                return false;
            }
            else
            {
                return tDstChn < tKey.tDstChn;
            }
        }
    }

    TSdpSessionName tSessionName;
    TChannel tDstChn;
};

class CRedisSwitchUnitTask : public CSipTask
{
public:
    enum TSwitchUnitType
    {
        SwitchUnitType_Src, 
        SwitchUnitType_Dst
    };

public:
    CRedisSwitchUnitTask(CInstExt *pcInst, const TSdpSessionName &tSessionName, 
        const TChannel &tStreamChn, const TChannel &tCmdChn, 
        CLoginSession *pcStreamSession, CLoginSession *pcCmdSession);
    virtual ~CRedisSwitchUnitTask();

public:
    virtual const char* GetObjName() const
    {
        return "CRedisSwitchUnitTask";
    }

public:
    // �����������Ҫʵ�ֵ��麯��
    virtual TSwitchUnitType GetSwitchUnitType() const = 0;
    virtual CRedisSwitchUnitTask *GetRelativeTask() = 0;

public:
    // ���ݷ��ʽӿ�
    const TSdpSessionName &GetSessionName() const { return m_tSessionName; }
    const TChannel &GetStreamChn() const { return m_tStreamChn; }
    const TChannel &GetCmdChn() const { return m_tCmdChn; }
    CLoginSession *GetStreamSession() const { return m_pcStreamSession; }
    CLoginSession *GetCmdSession() const { return m_pcCmdSession; }
	const TSdp &GetSdp() const { return m_tSdp; }
	TSdp &GetSdp(){ return m_tSdp; }

    CMS_RESULT GetErrorCode() const { return m_nErrorCode; }

public:
    // ����ͨ����Ϣ����
    bool       ProcDisconnect(const TDevOnOffCBMsg* ptDevOnOffCBMsg);
    bool       ProcOspSipException(CMessage *const pcMsg);
    bool       ProcByeReq(CMessage *const pcMsg);

private:
    // ����һ��SwitchUnit�Ļ���Ҫ�أ���Щ��������ֻ����ͨ����������ȡ�������޸�
    TSdpSessionName        m_tSessionName;          // ��������
    TChannel               m_tStreamChn;            // ����ͨ������cu/pu��
    TChannel               m_tCmdChn;               // ����ͨ������cui/pui/vtdu��
    CLoginSession         *m_pcStreamSession;       // ����session
    CLoginSession         *m_pcCmdSession;          // ����session

protected:
    // ���������ֱ�ӷſ����������
    TKDSIP_DIALOG_ID       m_tDlgID;
    TSdp                   m_tSdp;                  // pu/cu/vtdu��sdp
    CMS_RESULT             m_nErrorCode;            // ����״̬
    
    // ��������е���Ϣ��
    CInviteReq		       m_cInviteReq;
    CInviteRsp		       m_cInviteRsp;
    CInviteACK             m_cInviteAck;
};

//============================================================================================================================
class CRedisSrcSwitchUnitTask;
class CRedisDstSwitchUnitTask : public CRedisSwitchUnitTask
{
public:
    CRedisDstSwitchUnitTask(CInstExt *pcInst, const TSdpSessionName &tSessionName, 
        const TChannel &tStreamChn, const TChannel &tCmdChn, 
        CLoginSession *pcStreamSession, CLoginSession *pcCmdSession, CRedisSrcSwitchUnitTask *pcInput);
    virtual ~CRedisDstSwitchUnitTask();

	void ClearChildTask();

    virtual TSwitchUnitType GetSwitchUnitType() const
    {
        return CRedisSwitchUnitTask::SwitchUnitType_Dst;
    }

    virtual CRedisSwitchUnitTask *GetRelativeTask();

public:
    CRedisSrcSwitchUnitTask *GetInput() const { return m_pcInput; }

    void AddChildTask(s32 nSeqNo, CTask* pTask)
    {
        m_cChildTasks.insert(map<s32, u32>::value_type(nSeqNo, pTask->GetTaskNO()));
    }
    void DelChildTask(s32 nSeqNo)
    {
        m_cChildTasks.erase(nSeqNo);
    }

public:
    // ������Ҫ���麯��
    virtual CMS_RESULT Start(CMessage *const pcMsg) = 0;
    virtual CMS_RESULT ForwardInviteRsp(CRedisSrcSwitchUnitTask *pcSrc) = 0;
    virtual CMS_RESULT ForwardSrcInDlgMsg(CMessage *const pcMsg) = 0;

    // �¼�
    void ProcSrcSwitchUnitExit(CRedisSrcSwitchUnitTask *pcSrc);            // Դ����task�˳�ʱ֪ͨĿ�Ľ���task

protected:
    CRedisSrcSwitchUnitTask    *m_pcInput;          // ����ˣ���SwitchUnitType_Dst��˵���Ƕ�Ӧ��SwitchUnitType_Src��
    map<s32, u32> m_cChildTasks;                    //s32 SeqNO, u32 TaskNO
};

class CRedisCallDstSwitchUnitTask : public CRedisDstSwitchUnitTask
{
public:
    enum 
    {
        WaitReq,
        WaitRsp,
        WaitAck,
        Service
    };

public:
    CRedisCallDstSwitchUnitTask(CInstExt *pcInst, const TSdpSessionName &tSessionName, 
        const TChannel &tStreamChn, const TChannel &tCmdChn, 
        CLoginSession *pcStreamSession, CLoginSession *pcCmdSession, CRedisSrcSwitchUnitTask *pcInput);
    virtual ~CRedisCallDstSwitchUnitTask();

public:
    virtual const char* GetObjName() const
    {
        return "CRedisCallDstSwitchUnitTask";
    }

    virtual const char* GetStateName(u32 dwState) const
    {
        switch (dwState)
        {
        case WaitReq:
            return "WaitReq";
        case WaitRsp:
            return "WaitRsp";
        case WaitAck:
            return "WaitAck";
        case Service:
            return "Service";

        default:
            break;
        }
        return "Unknown State";
    }

public:
    virtual void InitStateMachine();
    virtual bool IsNotNeedPollState() { return (Service == GetState()); }

public:
    //״̬����
    u32 OnState(CMessage *const pcMsg);
    u32 OnStateTimer();

    u32 OnWaitReq(CMessage *const pcMsg);
    u32 OnWaitRsp(CMessage *const pcMsg);
    u32 OnWaitAck(CMessage *const pcMsg);
    u32 OnService(CMessage *const pcMsg);

public:
    // �ƶ�����ҵ��ļ����ؼ�����
    virtual CMS_RESULT Start(CMessage *const pcMsg);
    virtual CMS_RESULT ForwardInviteRsp(CRedisSrcSwitchUnitTask *pcSrc);
    virtual CMS_RESULT ForwardSrcInDlgMsg(CMessage *const pcMsg);

private:
    TKDSIP_TRANS_ID             m_tTransID;
};

class C3PCSwitchEntryTask;
class CRedis3PCDstSwitchUnitTask : public CRedisDstSwitchUnitTask
{
public:
    enum 
    {
        WaitReq,
        WaitRsp,
        WaitSrc,
        Service
    };

public:
    CRedis3PCDstSwitchUnitTask(CInstExt *pcInst, const TSdpSessionName &tSessionName, 
        const TChannel &tStreamChn, const TChannel &tCmdChn, 
        CLoginSession *pcStreamSession, CLoginSession *pcCmdSession, CRedisSrcSwitchUnitTask *pcInput, C3PCSwitchEntryTask *pcCreator);
    virtual ~CRedis3PCDstSwitchUnitTask();

public:
    virtual const char* GetObjName() const
    {
        return "CRedis3PCDstSwitchUnitTask";
    }

    virtual const char* GetStateName(u32 dwState) const
    {
        switch (dwState)
        {
        case WaitReq:
            return "WaitReq";
        case WaitRsp:
            return "WaitRsp";
        case WaitSrc:
            return "WaitSrc";
        case Service:
            return "Service";

        default:
            break;
        }
        return "Unknown State";
    }

public:
    virtual void InitStateMachine();
    virtual bool IsNotNeedPollState() { return (Service == GetState()); }

public:
    //״̬����
    u32 OnState(CMessage *const pcMsg);
    u32 OnStateTimer();

    u32 OnWaitReq(CMessage *const pcMsg);
    u32 OnWaitRsp(CMessage *const pcMsg);
    u32 OnWaitSrc(CMessage *const pcMsg);
    u32 OnService(CMessage *const pcMsg);

public:
    // �ƶ�����ҵ��ļ����ؼ�����
    virtual CMS_RESULT Start(CMessage *const pcMsg);
    virtual CMS_RESULT ForwardInviteRsp(CRedisSrcSwitchUnitTask *pcSrc);
    virtual CMS_RESULT ForwardSrcInDlgMsg(CMessage *const pcMsg);

    // �¼�
    virtual void ProcSwitchFinished(C3PCSwitchEntryTask *pcCreator);
    virtual void ProcCreatorExit(C3PCSwitchEntryTask *pcCreator) = 0;            // 3pc switch task�˳�ʱ֪ͨĿ�Ľ���task

protected:    
    C3PCSwitchEntryTask        *m_pcCreator;        // 3pc switch task
};

class CRedis3PCMsgDstSwitchUnitTask : public CRedis3PCDstSwitchUnitTask
{
public:
    CRedis3PCMsgDstSwitchUnitTask(CInstExt *pcInst, const TSdpSessionName &tSessionName, 
        const TChannel &tStreamChn, const TChannel &tCmdChn, 
        CLoginSession *pcStreamSession, CLoginSession *pcCmdSession, CRedisSrcSwitchUnitTask *pcInput, C3PCSwitchEntryTask *pcCreator);
    virtual ~CRedis3PCMsgDstSwitchUnitTask();

public:
    virtual const char* GetObjName() const
    {
        return "CRedis3PCMsgDstSwitchUnitTask";
    }

    void PublishState(bool bStart) const;

public:
    virtual void ProcSwitchFinished(C3PCSwitchEntryTask *pcCreator);
    virtual void ProcCreatorExit(C3PCSwitchEntryTask *pcCreator);
};

class CRedis3PCInviteDstSwitchUnitTask : public CRedis3PCDstSwitchUnitTask
{
public:
    CRedis3PCInviteDstSwitchUnitTask(CInstExt *pcInst, const TSdpSessionName &tSessionName, 
        const TChannel &tStreamChn, const TChannel &tCmdChn, 
        CLoginSession *pcStreamSession, CLoginSession *pcCmdSession, CRedisSrcSwitchUnitTask *pcInput, C3PCSwitchEntryTask *pcCreator);
    virtual ~CRedis3PCInviteDstSwitchUnitTask();

public:
    virtual const char* GetObjName() const
    {
        return "CRedis3PCInviteDstSwitchUnitTask";
    }

public:
    virtual void ProcCreatorExit(C3PCSwitchEntryTask *pcCreator);
};

//============================================================================================================================

class CRedisSrcSwitchUnitTask : public CRedisSwitchUnitTask
{
public:
    enum 
    {
        WaitReq,
        WaitRsp,
        WaitAck,
        Service
    };

public:
    CRedisSrcSwitchUnitTask(CInstExt *pcInst, const TSdpSessionName &tSessionName, 
        const TChannel &tStreamChn, const TChannel &tCmdChn, 
        CLoginSession *pcStreamSession, CLoginSession *pcCmdSession, 
        const TRedisModuleResourceId &tResourceId);
    virtual ~CRedisSrcSwitchUnitTask();

public:
    virtual const char* GetObjName() const
    {
        return "CRedisSrcSwitchUnitTask";
    }

    virtual const char* GetStateName(u32 dwState) const
    {
        switch (dwState)
        {
        case WaitReq:
            return "WaitReq";
        case WaitRsp:
            return "WaitRsp";
        case WaitAck:
            return "WaitAck";
        case Service:
            return "Service";

        default:
            break;
        }
        return "Unknown State";
    }

    virtual TSwitchUnitType GetSwitchUnitType() const
    {
        return CRedisSwitchUnitTask::SwitchUnitType_Src;
    }

    virtual CRedisSwitchUnitTask *GetRelativeTask();

public:
    virtual void InitStateMachine();
    virtual bool IsNotNeedPollState() { return (Service == GetState()); }

public:
    //״̬����
    u32 OnState(CMessage *const pcMsg);
    u32 OnStateTimer();

    u32 OnWaitReq(CMessage *const pcMsg);
    u32 OnWaitRsp(CMessage *const pcMsg);
    u32 OnWaitAck(CMessage *const pcMsg);
    u32 OnService(CMessage *const pcMsg);

public:
    // �ƶ�����ҵ��ļ����ؼ�����
    CMS_RESULT ForwardInviteReq(CRedisDstSwitchUnitTask *pcDst);
    CMS_RESULT ForwardInviteAck(CRedisDstSwitchUnitTask *pcDst);
    CMS_RESULT ForwardDstInDlgMsg(CMessage *const pcMsg);

    void       ProcDstSwitchUnitCreate(CRedisDstSwitchUnitTask *pcDst);
    void       ProcDstSwitchUnitExit(CRedisDstSwitchUnitTask *pcDst);            // Ŀ�Ľ���task�˳�ʱ֪ͨԴ����task

	void       SetNatPacketMode(const bool var){m_bNatPacketMode = var;};

private:
    TRedisModuleResourceId         m_tResourceId;
    CRedisDstSwitchUnitTask       *m_pcOutput;               // ����ˣ���SwitchUnitType_Src��˵�Ƿַ���Ӧ��SwitchUnitType_Dst��
	bool                           m_bNatPacketMode;         // �Ƿ�ʹ��nat̽���ģʽ�������մ���
};

template<class CReq, class CRsp>
class CRedisSwitchUnitReqRspTask : public CSipTask
{
public:
    enum
    {
        Req = UNKNOWN_STATE+1,
        WaitRsp,
    };

public:

    CRedisSwitchUnitReqRspTask(CInstExt* pcInst, CRedisDstSwitchUnitTask* pcSwitchTask) : 
      CSipTask(pcInst), m_dwSwitchTaskNO(pcSwitchTask->GetTaskNO()), m_tSipTransId(INVALID_TRANS_ID), m_eSipRspEvent(KDSIP_EVENT_UNKNOWN)
    {
    }

    ~CRedisSwitchUnitReqRspTask()
    {	
        ReleaseResource();
    }

    void ReleaseResource()
    {
		CTask* pcTask = ((CCmuCoreInst*)GetInstance())->FindTask(m_dwSwitchTaskNO);
        if (pcTask != NULL)
        {
			CRedisDstSwitchUnitTask* pcDstSwitchTask = (CRedisDstSwitchUnitTask*)pcTask;
            pcDstSwitchTask->DelChildTask(GetSeqNo());
        }
    }

public:
    virtual const char* GetObjName() const
    {
        return "CInDlgReqRspTask";
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
        cReqProc.ProcMsg = (CTask::PFProcMsg)&CRedisSwitchUnitReqRspTask::OnReq;
        cReqProc.ProcErrMsg = (CTask::PFProcMsg)&CRedisSwitchUnitReqRspTask::OnReq;
        cReqProc.TimerPoll = (CTask::PFTimerPoll)&CRedisSwitchUnitReqRspTask::OnReqTimer;
        AddRuleProc(Req, cReqProc);

        CStateProc cWaitRspProc;
        cWaitRspProc.ProcMsg = (CTask::PFProcMsg)&CRedisSwitchUnitReqRspTask::OnWaitRsp;
        cWaitRspProc.ProcErrMsg = (CTask::PFProcMsg)&CRedisSwitchUnitReqRspTask::OnWaitRsp;
        cWaitRspProc.TimerPoll = (CTask::PFTimerPoll)&CRedisSwitchUnitReqRspTask::OnWaitRspTimer;
        AddRuleProc(WaitRsp, cWaitRspProc);

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
            m_eSipRspEvent = pcOspSipMsg->GetSipRsp();

            CReq cReq;
            pcOspSipMsg->GetMsgBody(cReq);
            m_tReq = cReq; 

			CTask* pcFindTask = ((CCmuCoreInst*)GetInstance())->FindTask(m_dwSwitchTaskNO);
			if (NULL == pcFindTask)
			{
				TASKLOG(ERROR_LEV, "TaskNO[%u] Not Find CRedisDstSwitchUnitTask\n", 
					m_dwSwitchTaskNO);
				return PROCMSG_DEL;
			}
			CRedisDstSwitchUnitTask* pcDstSwitchTask = (CRedisDstSwitchUnitTask*)pcFindTask;
            pcDstSwitchTask->AddChildTask(GetSeqNo(), this);

            CRedisSwitchUnitTask * pcTask = pcDstSwitchTask->GetRelativeTask();
            if (pcTask != NULL)
            {
                pcTask->ProcMsg(pcMsg);
            }
            else
            {             
                TASKLOG(WARNING_LEV, "��ȡ������������ʧ�� Msg[%s-%d]\n", 
                    OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
                dwRet = PROCMSG_FAIL;
            }
        }
        else
        {
            TASKLOG(WARNING_LEV, "Receive unknown Msg[%s-%d]\n", 
                OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
            dwRet = PROCMSG_FAIL;
        }

        if (dwRet != PROCMSG_OK)
        {
            SendRsp(ERR_CMU_SWITCH_VCR_FAILED);
        }
        else
        {
            NextState(WaitRsp);
        }

        return dwRet;
    }

    u32 OnReqTimer()
    {
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

            TASKLOG(ERROR_LEV, "����SIP�����sip status code[%d]\n",
                pcOspSipMsg->GetSipErrorCode());

            SendRsp(ERR_CMU_SWITCH_VCR_FAILED);

            return dwRet;
        }

        CRsp cRsp;
        if ((u16)cRsp.GetEventId() == pcMsg->event)
        {
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

            pcOspSipMsg->GetMsgBody(cRsp);
            cRsp.SetSession(m_tReq.GetSession());
            SendRsp(cRsp);
        }
        else
        {
            TASKLOG(WARNING_LEV, "Receive unknown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
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
            TASKLOG(WARNING_LEV, "�Ի��ڿ���Ӧ��ʱ, Msg[%s-%d]����ʧ��\n", 
                m_tReq.GetEventStr().c_str(), m_tReq.GetEventId());
            SendRsp(ERR_CMU_DEV_RSP_TIMEOUT);
            return TIMERPOLL_DEL;
        }

        return TIMERPOLL_DONE;
    }

    bool SendRsp(u32 dwError)
    {
        CRsp cRsp;
        cRsp.SetSeqNum(m_tReq.GetSeqNum());
        cRsp.SetSession(m_tReq.GetSession());
        cRsp.SetErrorCode(dwError);

        return (PROCMSG_OK == PostRsp(m_eSipRspEvent, m_tSipTransId, cRsp));
    }

    bool SendRsp(const CRsp& cRsp)
    {
        return (PROCMSG_OK == PostRsp(m_eSipRspEvent, m_tSipTransId, cRsp));
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
    int GetSeqNo() const
    {
        return m_tReq.GetSeqNum();
    }

private:
    u32 m_dwSwitchTaskNO;
    TKDSIP_TRANS_ID m_tSipTransId;
    EKDSIP_EVENT m_eSipRspEvent;

    CReq m_tReq;
};

typedef CRedisSwitchUnitReqRspTask<CVcrCtrlReq, CVcrCtrlRsp> CRedisVcrCtrlTask;
typedef CRedisSwitchUnitReqRspTask<CUpdateSwitchAddressReq, CUpdateSwitchAddressRsp> CRedisUpdateSwitchAddressTask;

extern map<TDstSwitchKey, CRedisDstSwitchUnitTask *> g_cRedisDstSwitchList;

#endif  //#ifndef _REDIS_SWITCH_UNIT_TASK_H
