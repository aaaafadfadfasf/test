/*
wanghui
功能：gbs选择上报xml文件生成、导出
*/

#ifndef _GB_REPORT_CATALOG_FILE_EXPORT_H_
#define _GB_REPORT_CATALOG_FILE_EXPORT_H_

#include "inner_common.h"
class CGbReportCatalogFileExportTask: public CSipTask
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

	CGbReportCatalogFileExportTask(CInstExt *pcInst);
	~CGbReportCatalogFileExportTask();

	virtual const char* GetObjName() const
	{
		return "CGbReportCatalogFileExportTask";
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

	u32 m_dwProgress;
};


#endif // _GB_REPORT_CATALOG_FILE_EXPORT_H_
