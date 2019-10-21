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
		REQ = UNKNOWN_STATE + 1,	//����֤������
		WaitResult,	                //�ȴ��������
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
	bool CheckSign(const CSecurityModuleCertificateSaveReq &cReq);//��ǩ
	void SaveCertificateReq(const CSecurityModuleCertificateSaveReq &cReq);
	void QueryPubKeyForCheckSign(const string& strId,string& strPubKey);//�����ݿ��в�����ǩ��Կ
	void ProcCertBySm3(string& strCert4Sm3);//��֤�����ݽ����Ӵ�(SM3����)
	void ProcCertBySm4(const string& strSm4Key, string& strCert4Sm4);//��֤�����ݽ��м��ܣ�SM4���ܣ�
	u32 PostRspToClient(u32 dwErrorCode);

private:
	u32 m_dwSeqNum;
	u32 m_dwEvent;
	string m_strSession;
	TKDSIP_TRANS_ID m_tTransId;
	string m_strReqMsg;	//�洢������Ϣ��ToXml��
	string m_strCertContent;//֤������

};
#endif