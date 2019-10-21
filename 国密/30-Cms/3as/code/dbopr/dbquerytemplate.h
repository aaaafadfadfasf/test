/*
���ܣ���ѯ���ݿ�������װNTF��RSP������Ͷ����CCtrl��ģ��ʵ�֡�
      ����dbopr.h�е�CDbOpr������
���ߣ���־��
*/
#ifndef _DBOPR_H_
#error "only can be used in dbopr.h"
#endif

/* ҵ���¼ֻ��һ�����ݿ��¼��� */
template<typename QryReq, typename QryRsp, typename NtfReq, typename RecordInfo>
void DatabaseQueryTemplate( const CTaskMsgTransfer<QryReq> *pcMsg, 
    ENTasErrno (CDbOpr::*pFuncPrepareQrySql)(const QryReq &cReq, TSqlString &tSqlSelect, TSqlString &tSqlCount),
    void (CDbOpr::*pFuncFetchQryRecord)(RecordInfo &),
    vector<RecordInfo>& (NtfReq::*pFuncNtfContainer)(),
    const string& (RecordInfo::*pFuncRecordKey)() const)
{
    CTaskMsgTransfer<QryRsp> cTrans;
    QryRsp &cRsp = cTrans.GetMsgObj();
    CTaskMsgTransfer<NtfReq> cTransNtf;
    NtfReq &cNtf = cTransNtf.GetMsgObj();

    const QryReq &cReq = pcMsg->GetMsgObj();
    TSqlString tSql, tSqlCount;

    do 
    {
        ENTasErrno enErr = (this->*pFuncPrepareQrySql)(cReq, tSql, tSqlCount);
        if (CMS_SUCCESS != enErr)
        {
            cRsp.SetErrorCode(enErr);
            break;
        }

        if (cReq.GetStartEntryIndex() == 0)
        {
            if (this->ExecSql(tSqlCount.GetBuf(), CDbOpr::enExecMustRet))
            {
                cRsp.SetTotalEntryNum(this->GetIntResult());
            }
            else
            {
                cRsp.SetErrorCode(ERR_TAS_DBERROR);
                break;
            }
        }

        if (!this->ExecSql(tSql.GetBuf(), CDbOpr::enExecMayRet))
        {
            cRsp.SetErrorCode(ERR_TAS_DBERROR);
            break;
        }

        u32 dwRecordcount = 0;
        u32 RecordNumInNtf = 0;
        u32 dwNtfContentSize = 0;
        RecordInfo cInfo;
        // �㷨�� ѭ������һ��һ���Ļ�ȡDB��¼ --> 1��DB��¼Ϊһ��������ҵ���¼ --> ��װN��ҵ���¼Ϊһ��NTF��Ϣ; ѭ��֮�⴦�����һ����Ϣ
        while (this->IsNotEof())
        {
			cInfo.Clear();

            // ��ȡһ��DB��¼
            (this->*pFuncFetchQryRecord)(cInfo);
            if (!this->MoveNext())
            {
                cRsp.SetErrorCode(ERR_TAS_DBERROR);
                break;
            }

            // ��װһ��NTF��Ϣ
            u32 dwRecordContentSize = cInfo.ToXml().size();
            if (dwNtfContentSize + dwRecordContentSize < SIP_MSG_LEN_MAX)
            {
                dwNtfContentSize += dwRecordContentSize;
                ++RecordNumInNtf;
                (cNtf.*pFuncNtfContainer)().push_back(cInfo);
            }
            else
            {
                cNtf.SetEntryNum(RecordNumInNtf);
                cNtf.SetLastNtf(false);
                PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cNtf, cTransNtf);

                dwRecordcount += RecordNumInNtf;

                (cNtf.*pFuncNtfContainer)().clear();
                (cNtf.*pFuncNtfContainer)().push_back(cInfo);
                dwNtfContentSize = dwRecordContentSize;
                RecordNumInNtf = 1;
            }
        }

        // ������װ����δ���͵����һ��NTF��Ϣ
        if (cRsp.GetErrorCode() == CMS_SUCCESS && !(cNtf.*pFuncNtfContainer)().empty())
        {
            cNtf.SetEntryNum(RecordNumInNtf);
            cNtf.SetLastNtf(true);
            PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cNtf, cTransNtf);

            dwRecordcount += RecordNumInNtf;
        }

        cRsp.SetEntryNum(dwRecordcount);
    } while (false);

    PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
}

template<typename QryReq, typename QryRsp, typename NtfReq, typename RecordInfo>
void DatabaseQueryTemplate( const CTaskMsgTransfer<QryReq> *pcMsg, 
    ENTasErrno (CDbOpr::*pFuncPrepareQrySql)(const QryReq &cReq, TSqlString &tSqlSelect, TSqlString &tSqlCount),
    ENTasErrno (CDbOpr::*pFuncFetchQryRecord)(RecordInfo &),
    vector<RecordInfo>& (NtfReq::*pFuncNtfContainer)(),
    const string& (RecordInfo::*pFuncRecordKey)() const)
{
    CTaskMsgTransfer<QryRsp> cTrans;
    QryRsp &cRsp = cTrans.GetMsgObj();
    CTaskMsgTransfer<NtfReq> cTransNtf;
    NtfReq &cNtf = cTransNtf.GetMsgObj();

    const QryReq &cReq = pcMsg->GetMsgObj();
    TSqlString tSql, tSqlCount;

    do 
    {
        ENTasErrno enErr = (this->*pFuncPrepareQrySql)(cReq, tSql, tSqlCount);
        if (CMS_SUCCESS != enErr)
        {
            cRsp.SetErrorCode(enErr);
            break;
        }

        if (cReq.GetStartEntryIndex() == 0)
        {
            if (this->ExecSql(tSqlCount.GetBuf(), CDbOpr::enExecMustRet))
            {
                cRsp.SetTotalEntryNum(this->GetIntResult());
            }
            else
            {
                cRsp.SetErrorCode(ERR_TAS_DBERROR);
                break;
            }
        }

        if (!this->ExecSql(tSql.GetBuf(), CDbOpr::enExecMayRet))
        {
            cRsp.SetErrorCode(ERR_TAS_DBERROR);
            break;
        }

        u32 dwRecordcount = 0;
        u32 RecordNumInNtf = 0;
        u32 dwNtfContentSize = 0;
        RecordInfo cInfo;
        // �㷨�� ѭ������һ��һ���Ļ�ȡDB��¼ --> 1��DB��¼Ϊһ��������ҵ���¼ --> ��װN��ҵ���¼Ϊһ��NTF��Ϣ; ѭ��֮�⴦�����һ����Ϣ
        while (this->IsNotEof())
        {
			cInfo.Clear();

            // ��ȡһ��DB��¼
            enErr = (this->*pFuncFetchQryRecord)(cInfo);
            if (CMS_SUCCESS != enErr)
            {
                cRsp.SetErrorCode(enErr);
                break;
            }
            if (!this->MoveNext())
            {
                cRsp.SetErrorCode(ERR_TAS_DBERROR);
                break;
            }

            // ��װһ��NTF��Ϣ
            u32 dwRecordContentSize = cInfo.ToXml().size();
            if (dwNtfContentSize + dwRecordContentSize < SIP_MSG_LEN_MAX)
            {
                dwNtfContentSize += dwRecordContentSize;
                ++RecordNumInNtf;
                (cNtf.*pFuncNtfContainer)().push_back(cInfo);
            }
            else
            {
                cNtf.SetEntryNum(RecordNumInNtf);
                cNtf.SetLastNtf(false);
                PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cNtf, cTransNtf);

                dwRecordcount += RecordNumInNtf;

                (cNtf.*pFuncNtfContainer)().clear();
                (cNtf.*pFuncNtfContainer)().push_back(cInfo);
                dwNtfContentSize = dwRecordContentSize;
                RecordNumInNtf = 1;
            }
        }

        // ������װ����δ���͵����һ��NTF��Ϣ
        if (cRsp.GetErrorCode() == CMS_SUCCESS && !(cNtf.*pFuncNtfContainer)().empty())
        {
            cNtf.SetEntryNum(RecordNumInNtf);
            cNtf.SetLastNtf(true);
            PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cNtf, cTransNtf);

            dwRecordcount += RecordNumInNtf;
        }

        cRsp.SetEntryNum(dwRecordcount);
    } while (false);

    PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
}

/* ҵ���¼�����ɶ������ݿ��¼��� */
template<typename QryReq, typename QryRsp, typename NtfReq, typename RecordInfo, typename SubRecordInfo>
void DatabaseQueryTemplate( const CTaskMsgTransfer<QryReq> *pcMsg, 
    ENTasErrno (CDbOpr::*pFuncPrepareQrySql)(const QryReq &cReq, TSqlString &tSqlSelect, TSqlString &tSqlCount),
    void (CDbOpr::*pFuncFetchQryRecord)(RecordInfo &), 
    vector<RecordInfo>& (NtfReq::*pFuncNtfContainer)(),
    vector<SubRecordInfo>& (RecordInfo::*pFuncRecordContainer)(),
    const string& (RecordInfo::*pFuncRecordKey)() const)
{
    CTaskMsgTransfer<QryRsp> cTrans;
    QryRsp &cRsp = cTrans.GetMsgObj();
    CTaskMsgTransfer<NtfReq> cTransNtf;
    NtfReq &cNtf = cTransNtf.GetMsgObj();

    const QryReq &cReq = pcMsg->GetMsgObj();
    TSqlString tSql, tSqlCount;

    do 
    {
        ENTasErrno enErr = (this->*pFuncPrepareQrySql)(cReq, tSql, tSqlCount);
        if (CMS_SUCCESS != enErr)
        {
            cRsp.SetErrorCode(enErr);
            break;
        }
        if (cReq.GetStartEntryIndex() == 0)
        {
            if (ExecSql(tSqlCount, enExecMustRet))
            {
                cRsp.SetTotalEntryNum(GetIntResult());
            }
            else
            {
                cRsp.SetErrorCode(ERR_TAS_DBERROR);
                break;
            }
        }

        if (!ExecSql(tSql, enExecMayRet))
        {
            cRsp.SetErrorCode(ERR_TAS_DBERROR);
            break;
        }

        u32 dwRecordcount = 0;
        u32 RecordNumInNtf = 0;
        u32 dwNtfContentSize = 0;
        RecordInfo cInfo, cInfoTmp;
        // �㷨�� ѭ������һ��һ���Ļ�ȡDB��¼ --> ��װN��DB��¼Ϊһ��������ҵ���¼(������Ϣ��С����ʱ���ҵ���¼) --> ��װN��ҵ���¼Ϊһ��NTF��Ϣ; ѭ��֮�⴦�����һ����Ϣ
        while (IsNotEof())
        {
			cInfoTmp.Clear();

            // ��ȡһ��DB��¼
            (this->*pFuncFetchQryRecord)(cInfoTmp);
            if (!MoveNext())
            {
                cRsp.SetErrorCode(ERR_TAS_DBERROR);
                break;
            }

			u32 dwInfoSize = cInfo.ToXml().size();
			u32 dwInfoTmpSize = cInfoTmp.ToXml().size();
			if ((cInfo.*pFuncRecordKey)() == (cInfoTmp.*pFuncRecordKey)())
			{
				if (dwNtfContentSize + dwInfoSize + dwInfoTmpSize >= SIP_MSG_LEN_MAX)	// �ü�¼����Ҫ������Ƚ�ǰ������ݷ��ͳ�ȥ
				{
					++RecordNumInNtf;
					(cNtf.*pFuncNtfContainer)().push_back(cInfo);
					cNtf.SetEntryNum(RecordNumInNtf);
					PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cNtf, cTransNtf);
					(cNtf.*pFuncNtfContainer)().clear();
					dwNtfContentSize = 0;
					RecordNumInNtf   = 0;

					(cInfo.*pFuncRecordContainer)().clear();
				}

				(cInfo.*pFuncRecordContainer)().push_back((cInfoTmp.*pFuncRecordContainer)().front());
			}
			else				// ����ҵ���¼�л�
			{
				if (0 < (cInfo.*pFuncRecordContainer)().size())
				{
					++RecordNumInNtf;
					dwNtfContentSize += dwInfoSize;
					(cNtf.*pFuncNtfContainer)().push_back(cInfo);
				}

				++dwRecordcount;
				cInfo = cInfoTmp;

				if (dwNtfContentSize + dwInfoTmpSize >= SIP_MSG_LEN_MAX)
				{
					cNtf.SetEntryNum(RecordNumInNtf);
					PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cNtf, cTransNtf);
					(cNtf.*pFuncNtfContainer)().clear();
					dwNtfContentSize = 0;
					RecordNumInNtf   = 0;
				}
			}            
        }

		if (cRsp.GetErrorCode() == CMS_SUCCESS && (!(cNtf.*pFuncNtfContainer)().empty() || !(cInfo.*pFuncRecordContainer)().empty()))
		{
			++RecordNumInNtf;
			(cNtf.*pFuncNtfContainer)().push_back(cInfo);
			cNtf.SetEntryNum(RecordNumInNtf);
			cNtf.SetLastNtf(true);
			PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cNtf, cTransNtf);
			(cNtf.*pFuncNtfContainer)().clear();
			dwNtfContentSize = 0;
			RecordNumInNtf   = 0;
		}

        cRsp.SetEntryNum(dwRecordcount);
    } while (false);

    PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
}

template<typename QryReq, typename QryRsp, typename NtfReq, typename RecordInfo, typename SubRecordInfo>
void DatabaseQueryTemplate( const CTaskMsgTransfer<QryReq> *pcMsg, 
    ENTasErrno (CDbOpr::*pFuncPrepareQrySql)(const QryReq &cReq, TSqlString &tSqlSelect, TSqlString &tSqlCount),
    void (CDbOpr::*pFuncFetchQryRecord)(RecordInfo &), 
    vector<RecordInfo>& (NtfReq::*pFuncNtfContainer)(),
    vector<SubRecordInfo>& (RecordInfo::*pFuncRecordContainer)(),
    const string& (RecordInfo::*pFuncRecordKey)() const,
    const string& (RecordInfo::*pFuncRecordKey2)() const)
{
	CTaskMsgTransfer<QryRsp> cTrans;
	QryRsp &cRsp = cTrans.GetMsgObj();
	CTaskMsgTransfer<NtfReq> cTransNtf;
	NtfReq &cNtf = cTransNtf.GetMsgObj();

	const QryReq &cReq = pcMsg->GetMsgObj();
	TSqlString tSql, tSqlCount;

	do 
	{
		ENTasErrno enErr = (this->*pFuncPrepareQrySql)(cReq, tSql, tSqlCount);
		if (CMS_SUCCESS != enErr)
		{
			cRsp.SetErrorCode(enErr);
			break;
		}
		if (cReq.GetStartEntryIndex() == 0)
		{
			if (ExecSql(tSqlCount, enExecMustRet))
			{
				cRsp.SetTotalEntryNum(GetIntResult());
			}
			else
			{
				cRsp.SetErrorCode(ERR_TAS_DBERROR);
				break;
			}
		}

		if (!ExecSql(tSql, enExecMayRet))
		{
			cRsp.SetErrorCode(ERR_TAS_DBERROR);
			break;
		}

		u32 dwRecordcount = 0;
		u32 RecordNumInNtf = 0;
		u32 dwNtfContentSize = 0;
		RecordInfo cInfo, cInfoTmp;
		// �㷨�� ѭ������һ��һ���Ļ�ȡDB��¼ --> ��װN��DB��¼Ϊһ��������ҵ���¼(������Ϣ��С����ʱ���ҵ���¼) --> ��װN��ҵ���¼Ϊһ��NTF��Ϣ; ѭ��֮�⴦�����һ����Ϣ
		while (IsNotEof())
		{
			cInfoTmp.Clear();

			// ��ȡһ��DB��¼
			(this->*pFuncFetchQryRecord)(cInfoTmp);
			if (!MoveNext())
			{
				cRsp.SetErrorCode(ERR_TAS_DBERROR);
				break;
			}

			u32 dwInfoSize = cInfo.ToXml().size();
			u32 dwInfoTmpSize = cInfoTmp.ToXml().size();
			if ((cInfo.*pFuncRecordKey)() == (cInfoTmp.*pFuncRecordKey)() && (cInfo.*pFuncRecordKey2)() == (cInfoTmp.*pFuncRecordKey2)())
			{
				if (dwNtfContentSize + dwInfoSize + dwInfoTmpSize >= SIP_MSG_LEN_MAX)	// �ü�¼����Ҫ������Ƚ�ǰ������ݷ��ͳ�ȥ
				{
					++RecordNumInNtf;
					(cNtf.*pFuncNtfContainer)().push_back(cInfo);
					cNtf.SetEntryNum(RecordNumInNtf);
					PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cNtf, cTransNtf);
					(cNtf.*pFuncNtfContainer)().clear();
					dwNtfContentSize = 0;
					RecordNumInNtf   = 0;

					(cInfo.*pFuncRecordContainer)().clear();
				}

				(cInfo.*pFuncRecordContainer)().push_back((cInfoTmp.*pFuncRecordContainer)().front());
			}
			else				// ����ҵ���¼�л�
			{
				if (0 < (cInfo.*pFuncRecordContainer)().size())
				{
					++RecordNumInNtf;
					dwNtfContentSize += dwInfoSize;
					(cNtf.*pFuncNtfContainer)().push_back(cInfo);
				}

				++dwRecordcount;
				cInfo = cInfoTmp;

				if (dwNtfContentSize + dwInfoTmpSize >= SIP_MSG_LEN_MAX)
				{
					cNtf.SetEntryNum(RecordNumInNtf);
					PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cNtf, cTransNtf);
					(cNtf.*pFuncNtfContainer)().clear();
					dwNtfContentSize = 0;
					RecordNumInNtf   = 0;
				}
			}            
		}

		if (cRsp.GetErrorCode() == CMS_SUCCESS && (!(cNtf.*pFuncNtfContainer)().empty() || !(cInfo.*pFuncRecordContainer)().empty()))
		{
			++RecordNumInNtf;
			(cNtf.*pFuncNtfContainer)().push_back(cInfo);
			cNtf.SetEntryNum(RecordNumInNtf);
			cNtf.SetLastNtf(true);
			PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cNtf, cTransNtf);
			(cNtf.*pFuncNtfContainer)().clear();
			dwNtfContentSize = 0;
			RecordNumInNtf   = 0;
		}

		cRsp.SetEntryNum(dwRecordcount);
	} while (false);

	PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
}



template<typename QryReq, typename QryRsp, typename NtfReq, typename RecordInfo, typename SubRecordInfo>
void DatabaseLongQueryTemplate( const CTaskMsgTransfer<QryReq> *pcMsg, 
	ENTasErrno (CDbOpr::*pFuncPrepareQrySql)(const QryReq &cReq, TSqlString &tSqlSelect, TSqlString &tSqlCount),
	void (CDbOpr::*pFuncFetchQryRecord)(RecordInfo &), 
	vector<RecordInfo>& (NtfReq::*pFuncNtfContainer)(),
	vector<SubRecordInfo>& (RecordInfo::*pFuncRecordContainer)(),
	const string& (RecordInfo::*pFuncRecordKey)() const,
	const string& (RecordInfo::*pFuncRecordKey2)() const)
{
	CTaskMsgTransfer<QryRsp> cTrans;
	QryRsp &cRsp = cTrans.GetMsgObj();
	CTaskMsgTransfer<NtfReq> cTransNtf;
	NtfReq &cNtf = cTransNtf.GetMsgObj();

	const QryReq &cReq = pcMsg->GetMsgObj();
	TSqlString tSql, tSqlCount;
	bool bHasRsped = false;
	do 
	{
		ENTasErrno enErr = (this->*pFuncPrepareQrySql)(cReq, tSql, tSqlCount);
		if (CMS_SUCCESS != enErr)
		{
			cRsp.SetErrorCode(enErr);
			break;
		}
		if (cReq.GetStartEntryIndex() == 0)
		{
			if (ExecSql(tSqlCount, enExecMustRet))
			{
				cRsp.SetTotalEntryNum(GetIntResult());
			}
			else
			{
				cRsp.SetErrorCode(ERR_TAS_DBERROR);
				break;
			}
		}

		if (!ExecSql(tSql, enExecMayRet))
		{
			cRsp.SetErrorCode(ERR_TAS_DBERROR);
			break;
		}

		//��ҳ��ѯ����ֵ��Ҫ����
		cRsp.SetEntryNum(cRsp.GetTotalEntryNum()!=-1? cRsp.GetTotalEntryNum():(IsNotEof()?2000000:0));
		
		
		PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
		bHasRsped = true;

		u32 dwRecordcount = 0;
		u32 RecordNumInNtf = 0;
		u32 dwNtfContentSize = 0;
		RecordInfo cInfo, cInfoTmp;
		// �㷨�� ѭ������һ��һ���Ļ�ȡDB��¼ --> ��װN��DB��¼Ϊһ��������ҵ���¼(������Ϣ��С����ʱ���ҵ���¼) --> ��װN��ҵ���¼Ϊһ��NTF��Ϣ; ѭ��֮�⴦�����һ����Ϣ
		while (IsNotEof())
		{
			cInfoTmp.Clear();

			// ��ȡһ��DB��¼
			(this->*pFuncFetchQryRecord)(cInfoTmp);
			if (!MoveNext())
			{
				cRsp.SetErrorCode(ERR_TAS_DBERROR);
				break;
			}

			u32 dwInfoSize = cInfo.ToXml().size();
			u32 dwInfoTmpSize = cInfoTmp.ToXml().size();
			if ((cInfo.*pFuncRecordKey)() == (cInfoTmp.*pFuncRecordKey)() && (cInfo.*pFuncRecordKey2)() == (cInfoTmp.*pFuncRecordKey2)())
			{
				if (dwNtfContentSize + dwInfoSize + dwInfoTmpSize >= SIP_MSG_LEN_MAX)	// �ü�¼����Ҫ������Ƚ�ǰ������ݷ��ͳ�ȥ
				{
					++RecordNumInNtf;
					(cNtf.*pFuncNtfContainer)().push_back(cInfo);
					cNtf.SetEntryNum(RecordNumInNtf);
					PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cNtf, cTransNtf);
					(cNtf.*pFuncNtfContainer)().clear();
					dwNtfContentSize = 0;
					RecordNumInNtf   = 0;

					(cInfo.*pFuncRecordContainer)().clear();
				}

				(cInfo.*pFuncRecordContainer)().push_back((cInfoTmp.*pFuncRecordContainer)().front());
			}
			else				// ����ҵ���¼�л�
			{
				if (0 < (cInfo.*pFuncRecordContainer)().size())
				{
					++RecordNumInNtf;
					dwNtfContentSize += dwInfoSize;
					(cNtf.*pFuncNtfContainer)().push_back(cInfo);
				}

				++dwRecordcount;
				cInfo = cInfoTmp;

				if (dwNtfContentSize + dwInfoTmpSize >= SIP_MSG_LEN_MAX)
				{
					cNtf.SetEntryNum(RecordNumInNtf);
					PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cNtf, cTransNtf);
					(cNtf.*pFuncNtfContainer)().clear();
					dwNtfContentSize = 0;
					RecordNumInNtf   = 0;
				}
			}            
		}

		if (cRsp.GetErrorCode() == CMS_SUCCESS && (!(cNtf.*pFuncNtfContainer)().empty() || !(cInfo.*pFuncRecordContainer)().empty()))
		{
			++RecordNumInNtf;
			(cNtf.*pFuncNtfContainer)().push_back(cInfo);
			cNtf.SetEntryNum(RecordNumInNtf);
			cNtf.SetLastNtf(true);
			PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cNtf, cTransNtf);
			(cNtf.*pFuncNtfContainer)().clear();
			dwNtfContentSize = 0;
			RecordNumInNtf   = 0;
		}
	} while (false);

	if (!bHasRsped)
	{
		PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
	}

}

