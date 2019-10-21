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
		REQ = UNKNOWN_STATE + 1,	//查询证书请求
		WaitResult,	                //等待操作结果
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
	bool GetDbCertInfo(const string& strOwnerid);//获取数据库中证书信息
	void GetUsbKeySm4Passwd(string& strUsbKeyPasswd);//获取usbkey中的加密密钥
	void DecryptCertInfoBySm4(const string& strUsbKeyPasswd);//使用usbkey中的密钥对证书进行sm4解密
	bool CompareCertHashAndSrcHash();//对解密后的证书进行杂凑，然后与数据库中的杂凑值比较
private:
	u32 m_dwSeqNum;
	u32 m_dwEvent;
	string m_strSession;
	string m_strCertContent;        //证书解密后数据
	string m_strSrcCertContent;  //数据库中的证书数据
	string m_strSrcCertHash;      //数据库中的证书杂凑数据
	TKDSIP_TRANS_ID m_tTransId;
};


#endif // !_CERTIFICATE_GET_TASK_H
