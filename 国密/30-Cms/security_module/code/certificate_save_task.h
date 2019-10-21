#ifndef _CERTIFICATE_SAVE_TASK_H
#define _CERTIFICATE_SAVE_TASK_H

#include "cms/ospsip/ospsip.h"
#include "cms/ospext/ospext.h"
#include "cms/ospext/osptask.h"
#include "cms/security_module/security_module_proto.h"
#include "cms/cms_errorcode.h"
#include "cbb/kdssl-ext/kdssl-ext_api.h"

#define BUFF_LEN (1024 * 1024)
class CCertificateSaveTask :public CSipTask
{
public:
	enum
	{
		REQ = UNKNOWN_STATE + 1,	//保存证书请求
		WaitResult,	                //等待操作结果
	};
public:
	CCertificateSaveTask(CInstExt *pcInst);
	~CCertificateSaveTask();
public:
	virtual const char* GetObjName() const
	{
		return "CCertificateSaveTask";
	}
	virtual const char* GetStateName(u32 dwState) const
	{
		switch (dwState)
		{
		case REQ:
			return "Req State";
		case WaitResult:
			return "WaitResult State";
		default:
			return "Unknown State";
		}
		return "Unknown State";
	}
	virtual void InitStateMachine();
public:
	u32 ProcReqMsg(CMessage *const pcMsg);
	u32 ProcReqMsgTimerPoll();
	u32 ProcWaitResult(CMessage *const pcMsg);
	u32 ProcWaitResultTimerPoll();
	u32 ProcErrMsg(CMessage *const pcMsg);
	bool CheckSign(const CSecurityModuleCertificateSaveReq &cReq);//验签
	void SaveCertificateReq(const CSecurityModuleCertificateSaveReq &cReq);
	void QueryPubKeyForCheckSign(const string& strId,string& strPubKey);//在数据库中查找验签公钥
	void ProcCertBySm3(string& strCert4Sm3);//将证书内容进行杂凑(SM3加密)
	void ProcCertBySm4(const string& strSm4Key, string& strCert4Sm4);//将证书内容进行加密（SM4加密）
	u32 PostRspToClient(u32 dwErrorCode);

private:
	u32 m_dwSeqNum;
	u32 m_dwEvent;
	string m_strSession;
	TKDSIP_TRANS_ID m_tTransId;
	string m_strReqMsg;	//存储请求消息的ToXml串
	string m_strCertContent;//证书内容

};
#endif