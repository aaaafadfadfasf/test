#include "certificate_get_task.h"

#define BUFF_LEN (1024 * 1024)
CCertificateGetTask::CCertificateGetTask(CInstExt *pcInst) : CSipTask(pcInst)
{

}


CCertificateGetTask::~CCertificateGetTask()
{

}


void CCertificateGetTask::InitStateMachine()
{

	CStateProc cReqProc;
	cReqProc.ProcMsg = (CSipTask::PFProcMsg)&CCertificateGetTask::ProcReqMsg;
	cReqProc.ProcErrMsg = (CSipTask::PFProcMsg)&CCertificateGetTask::ProcErrMsg;
	cReqProc.TimerPoll = (CSipTask::PFTimerPoll)&CCertificateGetTask::ProcReqMsgTimerPoll;
	AddRuleProc(REQ, cReqProc);


	CStateProc cResultProc;
	cResultProc.ProcMsg = (CSipTask::PFProcMsg)&CCertificateGetTask::ProcWaitResult;
	cResultProc.ProcErrMsg = (CSipTask::PFProcMsg)&CCertificateGetTask::ProcErrMsg;
	cResultProc.TimerPoll = (CSipTask::PFTimerPoll)&CCertificateGetTask::ProcWaitResultTimerPoll;
	AddRuleProc(WaitResult, cResultProc);

	NextState(REQ);
}

u32 CCertificateGetTask::ProcReqMsg(CMessage *const pcMsg)
{
	TASKLOG(EVENT_LEV, "�յ���Ϣ,event[%s]\n", OspExtEventDesc(pcMsg->event).c_str());

	COspSipMsg *pcOspSipMsg = (COspSipMsg*)pcMsg->content;
	if (NULL == pcOspSipMsg)
	{
		TASKLOG(ERROR_LEV, "��Ϣ��Ϊ��! ��������\n");
		return PROCMSG_DEL;
	}

	u32 dwRet = PROCMSG_OK;

	if (pcMsg->event == SECURITY_MODULE_CERTIFICATE_GET_REQ)
	{
		CSecurityModuleCertificateGetReq cReq;
		pcOspSipMsg->GetMsgBody(cReq);

		TASKLOG(EVENT_LEV, "�յ�REQ��Ϣ,event[%s]\n", cReq.GetEventStr().c_str());

		m_tTransId = pcOspSipMsg->GetSipTransID();
		m_strSession = cReq.GetSession();
		m_dwSeqNum = cReq.GetSeqNum();
		m_dwEvent = cReq.GetEventId();
		/*1.����id�����ݿ�ȡ��֤����������
		   2.ʹ��usbkey�еĶԳ���Կ����sm4����
		   3.�Խ��ܵ����ݽ���sm3�Ӵ�
		   4.�Ա��Ӵ�ֵ�����ݿ��е��Ӵ�ֵ�Ƿ�һ��
		   һ���򷵻ؽ���ֵ�����򷵻ش������*/

		if (!GetDbCertInfo(cReq.GetOwnerid()))
		{
			dwRet = PROCMSG_DEL;
		}
	}
	else
	{
		TASKLOG(WARNING_LEV, "�յ�δ֪��Ϣ[%s--%hu]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
	}
	NextState(WaitResult);
	return dwRet;
}

u32 CCertificateGetTask::ProcReqMsgTimerPoll()
{
	if (GetCurStateHoldTime() > SECURITY_DBIX_PROC_TIMEOUT)
	{
		TASKLOG(ERROR_LEV, "�������ݿⳬʱ,��������!\n");

		return TIMERPOLL_DEL;
	}

	return TIMERPOLL_DONE;
}

u32 CCertificateGetTask::ProcWaitResult(CMessage *const pcMsg)
{
	u32 dwRet = PROCMSG_OK;

	switch (pcMsg->event)
	{
	case DBIX_RESULT_CB:
	{
		CDbixMsg* pDbMsg = (CDbixMsg*)pcMsg->content;
		dwRet = PROCMSG_DEL;

		u32 dwErrorCode = CMS_SUCCESS;
		if (NULL == pDbMsg)
		{
			dwErrorCode = ERR_SECURITY_GET_CRT_EMPTY;
			TASKLOG(ERROR_LEV, "��ѯ���ݿ�֤��Ϊ��!\n");
			dwRet = PostRspToClient(dwErrorCode);
			return dwRet;
		}
		if (pDbMsg->GetDBResult() != enDBResultSuccess)
		{
			TASKLOG(ERROR_LEV, "�������ݱ�[CERT_INFO_TABLE], db operate fail, db-errCode[%d]!\n",
				pDbMsg->GetDBResult());
			dwErrorCode = ERR_SECURITY_DB_OPR_FAIL;
			dwRet = PostRspToClient(dwErrorCode);
			return dwRet;
		}
		else
		{
			TASKLOG(EVENT_LEV, "�������ݱ�[CERT_INFO_TABLE], db operate success!\n");
		}
		dwRet = ProcQeryData(pcMsg);
	}
	break;
	default:
	{
		TASKLOG(WARNING_LEV, "Receive unkown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
	}
	break;
	}
	return dwRet;
}

u32 CCertificateGetTask::ProcWaitResultTimerPoll()
{
	if (GetCurStateHoldTime() > SECURITY_DBIX_PROC_TIMEOUT)
	{
		TASKLOG(ERROR_LEV, "�ȴ����ݿ⴦��ʱ����������!\n");
		return TIMERPOLL_DEL;
	}
	return TIMERPOLL_DONE;
}

u32 CCertificateGetTask::ProcErrMsg(CMessage *const pcMsg)
{
	switch (pcMsg->event)
	{
	case OSP_SIP_MSG_PROC_FAIL:
	{
		//����������յ�SIP��Ĵ���Ӧ��
		COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
		if (NULL == pcOspSipMsg)
		{
			TASKLOG(ERROR_LEV, "��Ϣ��ΪNULL!\n");
		}

		TASKLOG(ERROR_LEV, "����SIP�����sip status code[%d]\n"
			, pcOspSipMsg->GetSipErrorCode());
	}
	break;
	default:
	{
		TASKLOG(WARNING_LEV, "Receive unkown Msg[%s-%d]\n",
			OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
	}
	break;
	}
	return PROCMSG_FAIL;
}

u32 CCertificateGetTask::PostRspToClient(u32 dwErrorCode)
{
	CSecurityModuleCertificateGetRsp cRsp;
	cRsp.SetSeqNum(m_dwSeqNum);
	cRsp.SetSession(m_strSession);
	cRsp.SetErrorCode(dwErrorCode);
	if (dwErrorCode == CMS_SUCCESS)
	{
		cRsp.SetCertContent(m_strCertContent);
	}
	if (PROCMSG_OK != PostMsgRsp(m_tTransId, cRsp))
	{
		TASKLOG(ERROR_LEV, "����Ӧ��ʧ��!\n");
	}
	else
	{
		TASKLOG(ERROR_LEV, "����Ӧ��ɹ�!\n");
	}

	return PROCMSG_DEL;
}

bool CCertificateGetTask::GetDbCertInfo(const string& strOwnerid)
{
	if (strOwnerid.empty())
	{
		TASKLOG(ERROR_LEV, "��ѯ�û�/�豸idΪ��\n");
		return false;
	}
	CDBEntity cEntity(g_cCertificateInfoTable);
	u64 lwDbCb = GetOspTransId_u64(GetInstance()->GetOspIID(), GetTaskNO());
	cEntity.SetCBParm(lwDbCb);

	CDBCriteria OwneridCond;
	CDBCriteria Condition;

	OwneridCond = eq(CRTIT_OWNERID, strOwnerid);
	Condition = OwneridCond;
	cEntity.SetQryFlag(0, 1, &Condition);
	OperateDBEntity(cEntity);
	NextState(WaitResult);
	return true;
}
u32 CCertificateGetTask::ProcQeryData(CMessage *const pcMsg)
{
	CDbixMsg* pDbMsg = (CDbixMsg*)pcMsg->content;
	u32 dwErrorCode = CMS_SUCCESS;

	CDBEntityValue* pDbValList = (CDBEntityValue*)pDbMsg->GetDBData();
	if (NULL != pDbValList)
	{
		pDbValList->NextRow(0);
		pDbValList->GetColumnVal(CRTIT_ENCRY, m_strSrcCertContent);
		TASKLOG(EVENT_LEV, "CertContent = %s\n", m_strSrcCertContent.c_str());
		pDbValList->GetColumnVal(CRTIT_SIGN, m_strSrcCertHash);
		TASKLOG(EVENT_LEV,"CerHash = %s\n", m_strSrcCertHash.c_str());

		string strUsbKeyPasswd;
		GetUsbKeySm4Passwd(strUsbKeyPasswd);
		if (!strUsbKeyPasswd.empty())
		{
			DecryptCertInfoBySm4(strUsbKeyPasswd);
			if (!CompareCertHashAndSrcHash())
			{
				dwErrorCode = ERR_SECURITY_GET_CRT_ERROR;
			}
		}
		else
		{
			TASKLOG(ERROR_LEV, "��ȡ�ԳƼ�����Կʧ��!\n");
		}
	}
	else
	{
		TASKLOG(ERROR_LEV, "��ѯ���Ϊ��!\n");
	}
	u32 dwRet = PostRspToClient(dwErrorCode);
	return dwRet;
}

void CCertificateGetTask::GetUsbKeySm4Passwd(string& strUsbKeyPasswd)
{

}
void CCertificateGetTask::DecryptCertInfoBySm4(const string& strUsbKeyPasswd)
{
	int ret;
	s32 count;
	u8 *out = (u8*)malloc(BUFF_LEN);
	KDSSL_CIPHER_CTX dec_cipher;
	dec_cipher = KdSSL_CIPHER_CTX_new();
	ret = KdSSL_CipherInit_ex(dec_cipher, KDC_EXT_SM4_ECB, 0, (u8*)strUsbKeyPasswd.c_str(), NULL, 0);
	assert(!ret);
	KdSSL_EncryptUpdate(dec_cipher, out, &count, (u8*)m_strSrcCertContent.c_str(), strlen(m_strSrcCertContent.c_str()));
	assert(!ret);
	/* sm4 ���� */
	KdSSL_EncryptFinal_ex(dec_cipher, out + count, &count);
	assert(!ret);
	KdSSL_CIPHER_CTX_free(dec_cipher);

	m_strCertContent = (char*)out;
	free(out);
	out = NULL;
}
bool CCertificateGetTask::CompareCertHashAndSrcHash()
{
	bool bRet = true;
	/*�ȶԽ��ܺ�����ݽ����Ӵգ�Ȼ���Ӵ����������ݿ��е��Ӵ����ݱȽ�*/
	int ret;
	s32 count;
	int engine_id = 0;
	u8 *in = (u8*)malloc(BUFF_LEN);
	int inlen = strlen(m_strCertContent.c_str());
	u8 *out = (u8*)malloc(BUFF_LEN);

	KDSSL_MD_CTX md_handle;
	md_handle = KdSSL_MD_CTX_create();
	ret = KdSSL_DigestInit(md_handle, KDC_EXT_SM3, engine_id);
	assert(!ret);
	memcpy(in, m_strCertContent.c_str(), inlen);
	ret = KdSSL_DigestUpdate(md_handle, in, inlen);
	assert(!ret);
	ret = KdSSL_DigestFinal(md_handle, out, (u32 *)&count);
	assert(!ret);
	KdSSL_MD_CTX_destroy(md_handle);

	if (m_strSrcCertHash == static_cast<string>((char*)out))
	{
		TASKLOG(EVENT_LEV, "���ݿ���Կ����û�б��Ķ�!\n");
		bRet = true;
	}
	else
	{
		TASKLOG(ERROR_LEV, "���ݿ���Կ���ݱ��Ķ�!\n");
		bRet = false;
	}

	free(in);
	free(out);
	in = NULL;
	out = NULL;

	return bRet;
}