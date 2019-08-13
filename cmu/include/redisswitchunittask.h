
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
    // 几个子类必须要实现的虚函数
    virtual TSwitchUnitType GetSwitchUnitType() const = 0;
    virtual CRedisSwitchUnitTask *GetRelativeTask() = 0;

public:
    // 数据访问接口
    const TSdpSessionName &GetSessionName() const { return m_tSessionName; }
    const TChannel &GetStreamChn() const { return m_tStreamChn; }
    const TChannel &GetCmdChn() const { return m_tCmdChn; }
    CLoginSession *GetStreamSession() const { return m_pcStreamSession; }
    CLoginSession *GetCmdSession() const { return m_pcCmdSession; }
	const TSdp &GetSdp() const { return m_tSdp; }
	TSdp &GetSdp(){ return m_tSdp; }

    CMS_RESULT GetErrorCode() const { return m_nErrorCode; }

public:
    // 部分通用消息处理
    bool       ProcDisconnect(const TDevOnOffCBMsg* ptDevOnOffCBMsg);
    bool       ProcOspSipException(CMessage *const pcMsg);
    bool       ProcByeReq(CMessage *const pcMsg);

private:
    // 构成一个SwitchUnit的基础要素，这些数据子类只允许通过函数来读取，不能修改
    TSdpSessionName        m_tSessionName;          // 交换类型
    TChannel               m_tStreamChn;            // 码流通道（如cu/pu）
    TChannel               m_tCmdChn;               // 信令通道（如cui/pui/vtdu）
    CLoginSession         *m_pcStreamSession;       // 码流session
    CLoginSession         *m_pcCmdSession;          // 信令session

protected:
    // 下面的数据直接放开给子类访问
    TKDSIP_DIALOG_ID       m_tDlgID;
    TSdp                   m_tSdp;                  // pu/cu/vtdu的sdp
    CMS_RESULT             m_nErrorCode;            // 交换状态
    
    // 信令过程中的消息体
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
    // 几个重要的虚函数
    virtual CMS_RESULT Start(CMessage *const pcMsg) = 0;
    virtual CMS_RESULT ForwardInviteRsp(CRedisSrcSwitchUnitTask *pcSrc) = 0;
    virtual CMS_RESULT ForwardSrcInDlgMsg(CMessage *const pcMsg) = 0;

    // 事件
    void ProcSrcSwitchUnitExit(CRedisSrcSwitchUnitTask *pcSrc);            // 源交换task退出时通知目的交换task

protected:
    CRedisSrcSwitchUnitTask    *m_pcInput;          // 输入端（对SwitchUnitType_Dst来说就是对应的SwitchUnitType_Src）
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
    //状态处理
    u32 OnState(CMessage *const pcMsg);
    u32 OnStateTimer();

    u32 OnWaitReq(CMessage *const pcMsg);
    u32 OnWaitRsp(CMessage *const pcMsg);
    u32 OnWaitAck(CMessage *const pcMsg);
    u32 OnService(CMessage *const pcMsg);

public:
    // 推动交换业务的几个关键方法
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
    //状态处理
    u32 OnState(CMessage *const pcMsg);
    u32 OnStateTimer();

    u32 OnWaitReq(CMessage *const pcMsg);
    u32 OnWaitRsp(CMessage *const pcMsg);
    u32 OnWaitSrc(CMessage *const pcMsg);
    u32 OnService(CMessage *const pcMsg);

public:
    // 推动交换业务的几个关键方法
    virtual CMS_RESULT Start(CMessage *const pcMsg);
    virtual CMS_RESULT ForwardInviteRsp(CRedisSrcSwitchUnitTask *pcSrc);
    virtual CMS_RESULT ForwardSrcInDlgMsg(CMessage *const pcMsg);

    // 事件
    virtual void ProcSwitchFinished(C3PCSwitchEntryTask *pcCreator);
    virtual void ProcCreatorExit(C3PCSwitchEntryTask *pcCreator) = 0;            // 3pc switch task退出时通知目的交换task

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
    //状态处理
    u32 OnState(CMessage *const pcMsg);
    u32 OnStateTimer();

    u32 OnWaitReq(CMessage *const pcMsg);
    u32 OnWaitRsp(CMessage *const pcMsg);
    u32 OnWaitAck(CMessage *const pcMsg);
    u32 OnService(CMessage *const pcMsg);

public:
    // 推动交换业务的几个关键方法
    CMS_RESULT ForwardInviteReq(CRedisDstSwitchUnitTask *pcDst);
    CMS_RESULT ForwardInviteAck(CRedisDstSwitchUnitTask *pcDst);
    CMS_RESULT ForwardDstInDlgMsg(CMessage *const pcMsg);

    void       ProcDstSwitchUnitCreate(CRedisDstSwitchUnitTask *pcDst);
    void       ProcDstSwitchUnitExit(CRedisDstSwitchUnitTask *pcDst);            // 目的交换task退出时通知源交换task

	void       SetNatPacketMode(const bool var){m_bNatPacketMode = var;};

private:
    TRedisModuleResourceId         m_tResourceId;
    CRedisDstSwitchUnitTask       *m_pcOutput;               // 输出端（对SwitchUnitType_Src来说是分发对应的SwitchUnitType_Dst）
	bool                           m_bNatPacketMode;         // 是否使用nat探测包模式，即接收代理
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
                TASKLOG(WARNING_LEV, "获取关联交换事务失败 Msg[%s-%d]\n", 
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

        //SIP层错误处理
        if (pcMsg->event == OSP_SIP_MSG_PROC_FAIL)
        {
            //发起的请求收到SIP层的错误应答
            COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

            TASKLOG(ERROR_LEV, "发生SIP层错误，sip status code[%d]\n",
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

        //事务处理结束，删除之
        return dwRet;
    }

    u32 OnWaitRspTimer()
    {
        //应答超时
        if (GetCurStateHoldTime() > CMU_REQ_RSP_TASK_TIMEOUT)
        {
            TASKLOG(WARNING_LEV, "对话内控制应答超时, Msg[%s-%d]请求失败\n", 
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
