#ifndef _EXPORT_TASK_H_
#define _EXPORT_TASK_H_
#include "common.h"
#include "cms/cmu/cmu_proto.h"
#include "culogintask.h"

/* �ļ�������ҵ�� */
class CExportTask : public CCallBackTask
{
public:
    enum
    {
        NotReq = UNKNOWN_STATE + 1,      //����״̬��δ��������
        WaitRsp,
        WaitAck,   //��״̬û��Ӧ��
        Hold,
        WaitBye,
    };
public:
    CExportTask(CInstExt *pcInst, const string &strCuSession, CSipTask *pcExportActTask, const string &strPeUri);
    ~CExportTask();
public:
    virtual const char* GetObjName() const //�������صķ���
    {
        return "CExportTask";
    }
    virtual const char* GetStateName(u32 dwState) const //�������صķ���
    {
        switch (dwState)
        {
        case NotReq:
            {
                return "Not Req";
            }
            break;
        case WaitRsp:
            {
                return "Wait Rsp";
            }
            break;
        case WaitAck:
            {
                return "Wait Ack";
            }
            break;
        case Hold:
            {
                return "Hold";
            }
            break;
        case WaitBye:
            {
                return "Wait Bye";
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
    u32 NotReqProcMsg(CMessage *const pcMsg);
    u32 NotReqTimerPoll();
    u32 WaitRspProcMsg(CMessage *const pcMsg);
    u32 WaitRspTimerPoll();
    u32 WaitAckProcMsg(CMessage *const pcMsg);
    u32 WaitAckTimerPoll();
    u32 HoldProcMsg(CMessage *const pcMsg);
    u32 WaitByeProcMsg(CMessage *const pcMsg);
    u32 WaitByeTimerPoll();
public:
    u32 StartSendReq();
    u32 StartSendExportReq(CFileExportReq &cReq);
    u32 StartSendExportAck();
    u32 StartSendExportBye();
public:
    void NotifyStop2Cu(s32 dwErrorCode);
public:
    CFileCollectProgressNtfReq &GetProcessNtfReq();
public:
    CSipTask *m_pcExportActTask;
    TKDSIP_DIALOG_ID m_tInviteID;
    string m_strExportFileUrl;
    CFileCollectProgressNtfReq m_tNtfReq;
public:
    string m_strPeSession;
public:
    CFileExportReq m_tReq;
    CFileExportRsp m_tRsp;
};

class CExportTaskInterAction : public CSipTask
{
public:
    enum
    {
        Req = UNKNOWN_STATE + 1,
        Rsp,
    };
public:
    CExportTaskInterAction(CInstExt *pcInst, const string &strCuSession, PTRecvNode ptRecvNode);
    virtual ~CExportTaskInterAction();
    virtual const char* GetObjName() const //�������صķ���
    {
        return "CExportTaskInterAction";
    }
    virtual const char* GetStateName(u32 dwState) const //�������صķ���
    {
        switch (dwState)
        {
        case Req:
            {
                return "Req";
            }
            break;
        case Rsp:
            {
                return "Rsp";
            }
            break;
        default:
            break;
        }
        return "Unknown State";
    }
    virtual void TerminateTask();
    virtual void InitStateMachine();
public:
    u32 ReqProcMsg( CMessage *const pcMsg );
    u32 ReqTimerPoll();
    u32 RspProcMsg( CMessage *const pcMsg );
    u32 RspTimerPoll();
public:
    CTask *GetExportTask(const string& strFileUrl);
public:
    void PostErrorMsg2Cu(u32 dwErrorCode);
    template<typename CRspToCu>
    u32 SendErrorMsg2Cu(u32 dwErrorCode = ERR_CUI_OPR_TIME_OUT)
    {
        CRspToCu cRsp;
        cRsp.SetErrorCode(dwErrorCode);
        return SendRspToCu<CRspToCu>(cRsp, m_ptRecvNode);
    }
private:
    PTRecvNode m_ptRecvNode;
    string m_strCuSession;
    string m_stFileUrl;
private:
    u32 m_dwReqEventID;
};

class CExportTask4Tas : public CCallBackTask
{
public:
    enum
    {
        NotReq = UNKNOWN_STATE + 1,      //����״̬��δ��������
        WaitRsp,
        WaitAck,   //��״̬û��Ӧ��
        Hold,
        WaitBye,
    };
public:
    CExportTask4Tas(CInstExt *pcInst, const string &strCuSession, CSipTask *pcExportActTask, const string &strTasUri);
    ~CExportTask4Tas();
public:
    virtual const char* GetObjName() const //�������صķ���
    {
        return "CExportTask4Tas";
    }
    virtual const char* GetStateName(u32 dwState) const //�������صķ���
    {
        switch (dwState)
        {
        case NotReq:
            {
                return "Not Req";
            }
            break;
        case WaitRsp:
            {
                return "Wait Rsp";
            }
            break;
        case WaitAck:
            {
                return "Wait Ack";
            }
            break;
        case Hold:
            {
                return "Hold";
            }
            break;
        case WaitBye:
            {
                return "Wait Bye";
            }
            break;
        default:
            break;
        }
        return "Unknown State";
    }
    void TerminateTask();
    virtual void InitStateMachine();
    void PrintData() const;

public:
    u32 NotReqProcMsg(CMessage *const pcMsg);
    u32 NotReqTimerPoll();
    u32 WaitRspProcMsg(CMessage *const pcMsg);
    u32 WaitRspTimerPoll();
    u32 WaitAckProcMsg(CMessage *const pcMsg);
    u32 WaitAckTimerPoll();
    u32 HoldProcMsg(CMessage *const pcMsg);
    u32 HoldTimerPoll();
    u32 WaitByeProcMsg(CMessage *const pcMsg);
    u32 WaitByeTimerPoll();
public:
    u32 StartSendReq();
    u32 StartSendExportReq(CFileExportReq &cReq);
    u32 StartSendExportAck();
    u32 StartSendExportBye();
public:
    void NotifyStop2Cu(s32 dwErrorCode);
public:
    CFileCollectProgressNtfReq &GetProcessNtfReq();
public:
    CSipTask *m_pcExportActTask;
    TKDSIP_DIALOG_ID m_tInviteID;
    string m_strExportFileUrl;
    CFileCollectProgressNtfReq m_tNtfReq;

private:
    string m_strCuSessionInTas;
public:
    CFileExportReq m_tReq;
    CFileExportRsp m_tRsp;
};

class CExportTask4TasInterAction : public CSipTask
{
public:
    enum
    {
        Req = UNKNOWN_STATE + 1,
        Rsp,
    };
public:
    CExportTask4TasInterAction(CInstExt *pcInst, const string &strCuSession, PTRecvNode ptRecvNode);
    virtual ~CExportTask4TasInterAction();
    virtual const char* GetObjName() const //�������صķ���
    {
        return "CExportTask4TasInterAction";
    }
    virtual const char* GetStateName(u32 dwState) const //�������صķ���
    {
        switch (dwState)
        {
        case Req:
            {
                return "Req";
            }
            break;
        case Rsp:
            {
                return "Rsp";
            }
            break;
        default:
            break;
        }
        return "Unknown State";
    }
    virtual void TerminateTask();
    virtual void InitStateMachine();
public:
    u32 ReqProcMsg( CMessage *const pcMsg );
    u32 ReqTimerPoll();
    u32 RspProcMsg( CMessage *const pcMsg );
    u32 RspTimerPoll();
public:
    CTask *GetExportTask(const string& strFileUrl);
public:
    void PostErrorMsg2Cu(u32 dwErrorCode);
    template<typename CRspToCu>
    u32 SendErrorMsg2Cu(u32 dwErrorCode = ERR_CUI_OPR_TIME_OUT)
    {
        CRspToCu cRsp;
        cRsp.SetErrorCode(dwErrorCode);
        return SendRspToCu<CRspToCu>(cRsp, m_ptRecvNode);
    }
private:
    PTRecvNode m_ptRecvNode;
    string m_strCuSession;
    CCuFileExportRsp m_cExportRsp;
private:
    u32 m_dwReqEventID;
};

class CExportTaskExInterAction : public CSipTask
{
public:
	enum
	{
		Req = UNKNOWN_STATE + 1,
		Rsp,
	};
public:
	CExportTaskExInterAction(CInstExt *pcInst, const string &strCuSession, PTRecvNode ptRecvNode);
	virtual ~CExportTaskExInterAction();
	virtual const char* GetObjName() const //�������صķ���
	{
		return "CExportTaskExInterAction";
	}
	virtual const char* GetStateName(u32 dwState) const //�������صķ���
	{
		switch (dwState)
		{
		case Req:
			{
				return "Req";
			}
			break;
		case Rsp:
			{
				return "Rsp";
			}
			break;
		default:
			break;
		}
		return "Unknown State";
	}
	virtual void TerminateTask();
	virtual void InitStateMachine();
public:
	u32 ReqProcMsg( CMessage *const pcMsg );
	u32 ReqTimerPoll();
	u32 RspProcMsg( CMessage *const pcMsg );
	u32 RspTimerPoll();
public:
	CTask *GetExportTask(const string& strFileUrl);
public:
	void PostErrorMsg2Cu(u32 dwErrorCode);
	template<typename CRspToCu>
	u32 SendErrorMsg2Cu(u32 dwErrorCode = ERR_CUI_OPR_TIME_OUT)
	{
		CRspToCu cRsp;
		cRsp.SetErrorCode(dwErrorCode);
		return SendRspToCu<CRspToCu>(cRsp, m_ptRecvNode);
	}
private:
	PTRecvNode m_ptRecvNode;
	string m_strCuSession;
	string m_stFileUrl;
	u32 m_dwInviteReqTaskNo;
private:
	u32 m_dwReqEventID;
};

/* �ļ�������ҵ�� */
class CExportTaskEx : public CCallBackTask
{
public:
	enum
	{
		NotReq = UNKNOWN_STATE + 1,      //����״̬��δ��������
		WaitRsp,
		WaitAck,   //��״̬û��Ӧ��
		Hold,
		WaitBye,
	};
public:
	CExportTaskEx(CInstExt *pcInst, const string &strCuSession, CSipTask *pcExportActTask, const string &strPeUri);
	~CExportTaskEx();
public:
	virtual const char* GetObjName() const //�������صķ���
	{
		return "CExportTaskEx";
	}
	virtual const char* GetStateName(u32 dwState) const //�������صķ���
	{
		switch (dwState)
		{
		case NotReq:
			{
				return "Not Req";
			}
			break;
		case WaitRsp:
			{
				return "Wait Rsp";
			}
			break;
		case WaitAck:
			{
				return "Wait Ack";
			}
			break;
		case Hold:
			{
				return "Hold";
			}
			break;
		case WaitBye:
			{
				return "Wait Bye";
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
	u32 NotReqProcMsg(CMessage *const pcMsg);
	u32 NotReqTimerPoll();
	u32 WaitRspProcMsg(CMessage *const pcMsg);
	u32 WaitRspTimerPoll();
	u32 WaitAckProcMsg(CMessage *const pcMsg);
	u32 WaitAckTimerPoll();
	u32 HoldProcMsg(CMessage *const pcMsg);
	u32 WaitByeProcMsg(CMessage *const pcMsg);
	u32 WaitByeTimerPoll();
public:
	u32 StartSendReq();
	u32 StartSendExportReq(CFileExportReq &cReq);
	u32 StartSendExportAck();
	u32 StartSendExportBye();
public:
	void NotifyStop2Cu(s32 dwErrorCode);
public:
	CFileCollectProgressNtfReq &GetProcessNtfReq();
public:
	CSipTask *m_pcExportActTask;
	TKDSIP_DIALOG_ID m_tInviteID;
	string m_strExportFileUrl;
	CFileCollectProgressNtfReq m_tNtfReq;
public:
	string m_strPeSession;
public:
	CFileExportReq m_tReq;
	CFileExportRsp m_tRsp;
};

#endif // _EXPORT_TASK_H_
