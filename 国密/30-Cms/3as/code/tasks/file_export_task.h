/*
功能：CU设备数据导出任务
*/

#ifndef _FILE_EXPORT_TASK_H_
#define _FILE_EXPORT_TASK_H_

#include "inner_common.h"
class CFileExportTask: public CSipTask
{
public:
    enum
    {
        REQ  = UNKNOWN_STATE + 1,
        ACK,
        BYE,    // 我方bye
        EXPORT,
        TRANSFER
    };

    CFileExportTask(CInstExt *pcInst);
    ~CFileExportTask();

    virtual const char* GetObjName() const
    {
        return "CFileExportTask";
    }
    virtual const char* GetStateName(u32 dwState) const
    {
        switch(dwState)
        {
        case REQ:
            {
                return "REQ";
            }
            break;
        case ACK:
            {
                return "ACK";
            }
            break;
        case BYE:
            {
                return "BYE";
            }
            break;
        case EXPORT:
            {
                return "EXPORT";
            }
            break;
        case TRANSFER:
            {
                return "TRANSFER";
            }
            break;
        default:
            {
                return "unknown state";
            }
            break;
        }
    }
    virtual void InitStateMachine();
    void TryTerminate();
    void PrintData() const;

private:
    u32 ProcReqMsg(CMessage *const pcMsg);
    u32 WaitReqTimerPoll();
    u32 ProcAckMsg(CMessage *const pcMsg);
    u32 WaitAckTimerPoll();
    u32 ProcByeMsg(CMessage *const pcMsg);
    u32 WaitByeTimerPoll();
    u32 ProcExportMsg(CMessage *const pcMsg);
    u32 WaitExportTimerPoll();
    u32 ProcTransferMsg(CMessage *const pcMsg);
    u32 WaitTransferTimerPoll();
    u32 ProcErrMsg(CMessage *const pcMsg);

private:
    void PostProcessNtf(s32 nErrorCode, s32 nPercent);

private:
    KDSIP_DIALOG_ID m_tInvId;
    CFileExportReq m_cReq;
    bool m_bAbortExport;
};

void TryTerminateFileExportTask(const string &strSessionId);
bool IsFileExportTaskExisted(const string &strSessionId);

#endif // _FILE_EXPORT_TASK_H_
