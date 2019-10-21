#ifndef _CERTIFICATE_GET_TASK_H
#define _CERTIFICATE_GET_TASK_H

#include "cms/ospsip/ospsip.h"
#include "cms/ospext/ospext.h"
#include "cms/ospext/osptask.h"
#include "cms/security_module/security_module_proto.h"
#include "cms/cms_errorcode.h"
#include "cbb/kdssl-ext/kdssl-ext_api.h"
#include "dbixface.h"

class CCertificateGetTask :public CSipTask
{
public:
	enum
	{
		REQ = UNKNOWN_STATE + 1,	//��ѯ֤������
		WaitResult,	                //�ȴ��������
	};
public:
	CCertificateGetTask(CInstExt *pcInst);
	~CCertificateGetTask();
public:
	virtual const char* GetObjName() const
	{
		return "CCertificateGetTask";
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
	u32 PostRspToClient(u32 dwErrorCode);
public:
	bool GetDbCertInfo(const string& strOwnerid);//��ȡ���ݿ���֤����Ϣ
	void GetUsbKeySm4Passwd(string& strUsbKeyPasswd);//��ȡusbkey�еļ�����Կ
	void DecryptCertInfoBySm4(const string& strUsbKeyPasswd);//ʹ��usbkey�е���Կ��֤�����sm4����
	bool CompareCertHashAndSrcHash();//�Խ��ܺ��֤������Ӵգ�Ȼ�������ݿ��е��Ӵ�ֵ�Ƚ�
private:
	u32 m_dwSeqNum;
	u32 m_dwEvent;
	string m_strSession;
	string m_strCertContent;        //֤����ܺ�����
	string m_strSrcCertContent;  //���ݿ��е�֤������
	string m_strSrcCertHash;      //���ݿ��е�֤���Ӵ�����
	TKDSIP_TRANS_ID m_tTransId;
};


#endif // !_CERTIFICATE_GET_TASK_H
