#include "certificate_save_task.h"
#include "dbixface.h"

CCertificateSaveTask::CCertificateSaveTask(CInstExt *pcInst) : CSipTask(pcInst)
{

}


CCertificateSaveTask::~CCertificateSaveTask()
{

}


void CCertificateSaveTask::InitStateMachine()
{

	CStateProc cReqProc;
	cReqProc.ProcMsg = (CSipTask::PFProcMsg)&CCertificateSaveTask::ProcReqMsg;
	cReqProc.ProcErrMsg = (CSipTask::PFProcMsg)&CCertificateSaveTask::ProcErrMsg;
	cReqProc.TimerPoll = (CSipTask::PFTimerPoll)&CCertificateSaveTask::ProcReqMsgTimerPoll;
	AddRuleProc(REQ, cReqProc);


	CStateProc cResultProc;
	cResultProc.ProcMsg = (CSipTask::PFProcMsg)&CCertificateSaveTask::ProcWaitResult;
	cResultProc.ProcErrMsg = (CSipTask::PFProcMsg)&CCertificateSaveTask::ProcErrMsg;
	cResultProc.TimerPoll = (CSipTask::PFTimerPoll)&CCertificateSaveTask::ProcWaitResultTimerPoll;
	AddRuleProc(WaitResult, cResultProc);

	NextState(REQ);
}

u32 CCertificateSaveTask::ProcReqMsg(CMessage *const pcMsg)
{
	TASKLOG(EVENT_LEV, "�յ���Ϣ,event[%s]\n", OspExtEventDesc(pcMsg->event).c_str());

	COspSipMsg *pcOspSipMsg = (COspSipMsg*)pcMsg->content;
	if (NULL == pcOspSipMsg)
	{
		TASKLOG(ERROR_LEV, "��Ϣ��Ϊ��! ��������\n");
		return PROCMSG_DEL;
	}

	u32 dwRet = PROCMSG_OK;

	if (pcMsg->event == SECURITY_MODULE_CERTIFICATE_SAVE_REQ)
	{
		CSecurityModuleCertificateSaveReq cReq;
		pcOspSipMsg->GetMsgBody(cReq);

		TASKLOG(EVENT_LEV, "�յ�REQ��Ϣ,event[%s]\n", cReq.GetEventStr().c_str());

		m_strReqMsg = cReq.ToXml();

		m_tTransId = pcOspSipMsg->GetSipTransID();
		m_strSession = cReq.GetSession();
		m_dwSeqNum = cReq.GetSeqNum();
		m_dwEvent = cReq.GetEventId();
		//����id���ļ����ҵ���Ӧ�Ĺ�Կ������ǩ��ͨ�����ٽ�����һ��,����ֱ�ӷ���
		if (CheckSign(cReq))
		{
			SaveCertificateReq(cReq);
		}
		else
		{
			dwRet = PostRspToClient(ERR_SECURITY_CRT_SIGN_FAIL);
		}
	}
	else
	{
		TASKLOG(WARNING_LEV, "�յ�δ֪��Ϣ[%s--%hu]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
	}
	NextState(WaitResult);
	return dwRet;
}

u32 CCertificateSaveTask::ProcReqMsgTimerPoll()
{
	if (GetCurStateHoldTime() > SECURITY_DBIX_PROC_TIMEOUT)
	{
		TASKLOG(ERROR_LEV, "�������ݿⳬʱ,��������!\n");

		return TIMERPOLL_DEL;
	}

	return TIMERPOLL_DONE;
}

u32 CCertificateSaveTask::ProcWaitResult(CMessage *const pcMsg)
{
	u32 dwRet = PROCMSG_OK;

	switch (pcMsg->event)
	{
	case DBIX_RESULT_CB:
	{
		CDbixMsg* pDbMsg = (CDbixMsg*)pcMsg->content;
		dwRet = PROCMSG_DEL;

		u32 dwErrorCode = CMS_SUCCESS;

		if (pDbMsg->GetDBResult() != enDBResultSuccess)
		{
			TASKLOG(ERROR_LEV, "�������ݱ�[VKEK_INFO_TABLE], db operate fail, db-errCode[%d]!\n",
				pDbMsg->GetDBResult());
			dwErrorCode = ERR_SECURITY_DB_OPR_FAIL;
		}
		else
		{
			TASKLOG(EVENT_LEV, "�������ݱ�[VKEK_INFO_TABLE], db operate success!\n");

		}

		dwRet = PostRspToClient(dwErrorCode);
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

u32 CCertificateSaveTask::ProcWaitResultTimerPoll()
{
	if (GetCurStateHoldTime() > SECURITY_DBIX_PROC_TIMEOUT)
	{
		TASKLOG(ERROR_LEV, "�ȴ����ݿ⴦��ʱ����������!\n");
		return TIMERPOLL_DEL;
	}
	return TIMERPOLL_DONE;
}

u32 CCertificateSaveTask::ProcErrMsg(CMessage *const pcMsg)
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

u32 CCertificateSaveTask::PostRspToClient(u32 dwErrorCode)
{
	CSecurityModuleCertificateSaveRsp cRsp;
	cRsp.SetSeqNum(m_dwSeqNum);
	cRsp.SetSession(m_strSession);
	cRsp.SetErrorCode(dwErrorCode);

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

bool CCertificateSaveTask::CheckSign(const CSecurityModuleCertificateSaveReq &cReq)
{
	string strPubKey;
	QueryPubKeyForCheckSign(cReq.GetOwnerid(),strPubKey);
	if (strPubKey.empty())
	{
		TASKLOG(ERROR_LEV, "�����ݿ���δ�ҵ�id:%s�Ĺ�Կ!\n", cReq.GetOwnerid().c_str());
		return false;
	}
	else
	{
		TASKLOG(EVENT_LEV, "�ҵ�id:%s�Ĺ�Կ!\n", cReq.GetOwnerid().c_str());

		//ֱ���ù�Կ��֤˽Կ�����ͨ������ǩ�ɹ�
		int ret;
		int engine_id = 0;
		int dwEncryCertLen = strlen(cReq.GetCert4encry().c_str());
		int dwSignCertLen = strlen(cReq.GetCert4sign().c_str());

		KDSSL_PKEY_CTX md_handle;
		TPKey pub_key;
		memset(&pub_key, 0, sizeof(pub_key));

		pub_key.m_pbyPKeyBuff = (u8 *)strPubKey.c_str();
		pub_key.m_dwPKeyLen = 0;//if KDC_EXT_KEY_FILE_NAME, not care
		pub_key.m_eKeyType = KDC_EXT_KEY_CERT;
		pub_key.m_eDataType = KDC_EXT_KEY_FILE_DATA;
		pub_key.m_pbyPassword = NULL;

		md_handle = KdSSL_MD_CTX_create();
		ret = KdSSL_DigestVerifyInit(md_handle, &pub_key, KDC_EXT_SM3, engine_id);
		assert(!ret);
		ret = KdSSL_DigestVerifyUpdate(md_handle, cReq.GetCert4encry().c_str(), dwEncryCertLen);
		assert(!ret);
		/* ��֤ǩ�����������ǩ�����壬��ǩ������*/
		ret = KdSSL_DigestVerifyFinal(md_handle, (u8*)cReq.GetCert4sign().c_str(), (u32)dwSignCertLen);

		if (!ret)
		{
			TASKLOG(ERROR_LEV, "��ǩid:%sʧ��!\n", cReq.GetOwnerid().c_str());
			return false;
		}
		TASKLOG(EVENT_LEV, "��ǩid:%s�ɹ�!\n", cReq.GetOwnerid().c_str());
	}
	return true;
}

void CCertificateSaveTask::QueryPubKeyForCheckSign(const string& strId,string& strPubKey)
{
	/*�˴��ݶ�Ϊ���ļ��в���*/
}

void CCertificateSaveTask::SaveCertificateReq(const CSecurityModuleCertificateSaveReq &cReq)
{
	//�����ݽ��м��ܺ��Ӵ�
	//�˴��Ƚ��Ժ����������ȵõ���ȡusbkey�е�˽Կ�������ٸ�ֵ
	string strPriKey = "��usbkey�л�ȡ��˽Կ";
	string strSm4Key = "��usbkey�л�ȡ�ĶԳ���Կ";
	/*��ȡ��Կ����*/

	string strCert4Sm3;
	string strCert4encry;

	ProcCertBySm3(strCert4Sm3);
	ProcCertBySm4(strSm4Key,strCert4encry);
	CDBEntity cEntity(g_cCertificateInfoTable);

	u64 lwDbCb = GetOspTransId_u64(GetInstance()->GetOspIID(), GetTaskNO());
	cEntity.SetCBParm(lwDbCb);
	cEntity.SetAddFlag();

	cEntity.SetColumnVal(CRTIT_TYPE,cReq.GetType());
	cEntity.SetColumnVal(CRTIT_OWNERID,cReq.GetOwnerid());
	cEntity.SetColumnVal(CRTIT_SIGN, strCert4Sm3);
	cEntity.SetColumnVal(CRTIT_ENCRY, strCert4encry);

	OperateDBEntity(cEntity);
	NextState(WaitResult);
}

void CCertificateSaveTask::ProcCertBySm3(string& strCert4Sm3)
{
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

	strCert4Sm3 = (char*)out;

	free(in);
	free(out);
	in = NULL;
	out = NULL;
}

void CCertificateSaveTask::ProcCertBySm4(const string& strSm4Key,string& strCert4Sm4)
{
	unsigned char iv[16] = {
		0x66, 0x03, 0x54, 0x92, 0x78, 0x00, 0x00, 0x00,
		0x66, 0x03, 0x54, 0x92, 0x78, 0x00, 0x00, 0x00,
	};//sm4���ܵ�����Կ����������д��

	int ret;
	u8 *out = (u8*)malloc(BUFF_LEN);
	s32 count;
	KDSSL_CIPHER_CTX enc_cipher;
	int engine_id = 0;
	enc_cipher = KdSSL_CIPHER_CTX_new();
	/* sm4 ����*/
	ret = KdSSL_CipherInit_ex(enc_cipher, KDC_EXT_SM4_OFB, engine_id, (u8*)strSm4Key.c_str(), iv, 1);
	assert(!ret);
	KdSSL_EncryptUpdate(enc_cipher, out, &count, (const u8*)m_strCertContent.c_str(), strlen(m_strCertContent.c_str()));
	assert(!ret);
	KdSSL_EncryptFinal_ex(enc_cipher, out + count, &count);
	assert(!ret);
	KdSSL_CIPHER_CTX_free(enc_cipher);

	strCert4Sm4 = (char*)out;
	free(out);
	out = NULL;
}