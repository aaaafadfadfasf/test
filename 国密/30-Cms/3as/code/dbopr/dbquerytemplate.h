/*
功能：查询数据库数据组装NTF和RSP，并且投递至CCtrl的模板实现。
      仅供dbopr.h中的CDbOpr包含。
作者：胡志云
*/
#ifndef _DBOPR_H_
#error "only can be used in dbopr.h"
#endif

/* 业务记录只由一条数据库记录组成 */
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
        // 算法： 循环里面一条一条的获取DB记录 --> 1条DB记录为一条完整的业务记录 --> 组装N条业务记录为一个NTF消息; 循环之外处理最后一条消息
        while (this->IsNotEof())
        {
			cInfo.Clear();

            // 获取一条DB记录
            (this->*pFuncFetchQryRecord)(cInfo);
            if (!this->MoveNext())
            {
                cRsp.SetErrorCode(ERR_TAS_DBERROR);
                break;
            }

            // 组装一个NTF消息
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

        // 发送组装好尚未发送的最后一个NTF消息
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
        // 算法： 循环里面一条一条的获取DB记录 --> 1条DB记录为一条完整的业务记录 --> 组装N条业务记录为一个NTF消息; 循环之外处理最后一条消息
        while (this->IsNotEof())
        {
			cInfo.Clear();

            // 获取一条DB记录
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

            // 组装一个NTF消息
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

        // 发送组装好尚未发送的最后一个NTF消息
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

/* 业务记录可能由多条数据库记录组成 */
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
        // 算法： 循环里面一条一条的获取DB记录 --> 组装N条DB记录为一条完整的业务记录(超过消息大小限制时拆分业务记录) --> 组装N条业务记录为一个NTF消息; 循环之外处理最后一条消息
        while (IsNotEof())
        {
			cInfoTmp.Clear();

            // 获取一条DB记录
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
				if (dwNtfContentSize + dwInfoSize + dwInfoTmpSize >= SIP_MSG_LEN_MAX)	// 该记录处需要拆包，先将前面的数据发送出去
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
			else				// 发生业务记录切换
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
		// 算法： 循环里面一条一条的获取DB记录 --> 组装N条DB记录为一条完整的业务记录(超过消息大小限制时拆分业务记录) --> 组装N条业务记录为一个NTF消息; 循环之外处理最后一条消息
		while (IsNotEof())
		{
			cInfoTmp.Clear();

			// 获取一条DB记录
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
				if (dwNtfContentSize + dwInfoSize + dwInfoTmpSize >= SIP_MSG_LEN_MAX)	// 该记录处需要拆包，先将前面的数据发送出去
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
			else				// 发生业务记录切换
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

		//分页查询返回值需要控制
		cRsp.SetEntryNum(cRsp.GetTotalEntryNum()!=-1? cRsp.GetTotalEntryNum():(IsNotEof()?2000000:0));
		
		
		PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
		bHasRsped = true;

		u32 dwRecordcount = 0;
		u32 RecordNumInNtf = 0;
		u32 dwNtfContentSize = 0;
		RecordInfo cInfo, cInfoTmp;
		// 算法： 循环里面一条一条的获取DB记录 --> 组装N条DB记录为一条完整的业务记录(超过消息大小限制时拆分业务记录) --> 组装N条业务记录为一个NTF消息; 循环之外处理最后一条消息
		while (IsNotEof())
		{
			cInfoTmp.Clear();

			// 获取一条DB记录
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
				if (dwNtfContentSize + dwInfoSize + dwInfoTmpSize >= SIP_MSG_LEN_MAX)	// 该记录处需要拆包，先将前面的数据发送出去
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
			else				// 发生业务记录切换
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

