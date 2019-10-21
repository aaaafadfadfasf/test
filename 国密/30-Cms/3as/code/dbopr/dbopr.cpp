#include <math.h>

#include "dbopr.h"
#include "dboprdef.h"
#include "utility/uuid.h"
#include "license_mng.h"
#include "dbcache.h"
#include "config_util/tas_config.h"
#include "cctrl.h"
#include "sync_analyzer_mng.h"
#include "tasks/parent_tas_conn_task.h"
#include "cms/utility/mischelper.h"
#include "cms/utility/cmszip.h"
#include "db_task_pool.h"
#include "redis/fastdbmng.h"
#include "nmsconn.h"
#include "cms/utility/strtok.h"
#include "kdvsys.h"
#include "osp/osp.h"
#include "cms/utility/pinyin.h"
#include "cms/utility/digital_string.h"
#include "cms/commonconfig/cmcfg.h"
#include "cms/gbs/gbs_redis.h"
#include <math.h>

extern CDbTaskPool g_cDbTaskPool;

static CXSemLock *s_pcLock4CuserExportDevData = NULL;
static u32 s_dwDevTreeVerExt = 0;

// 一页查询最大返回记录数, 不能太大否则可能导致CCtrl消息队列塞满

/* 形式(以","结束)："field=value," */
#define AppendUpdateValuePair(expr, var, tSql, field) if (expr(var)) {AppendFieldValuePairImp(tSql, field, var); tSql.AppendString(",");}
#define AppendUpdateDateValuePair(expr, var, tSql, field) if (expr(var)) {AppendFieldDatetimeValuePairImp(tSql, field, var); tSql.AppendString(",");}

/* 形式(以" and "结束)："field=value and "或者"(val1<=field and field<=val2) and "... */
#define AppendQryCondValuePair(expr, var, tSql, field, prefix) if (expr(var)) {AppendFieldValuePairImp(tSql, field, var, #prefix); tSql.AppendString(" and ");}
#define AppendQryCondDateValuePair(expr, var, tSql, field, prefix) if (expr(var)) {AppendFieldDatetimeValuePairImp(tSql, field, var, #prefix); tSql.AppendString(" and ");}
#define AppendQryCondDatePeriodPair(expr1, var1, expr2, var2, tSql, field, prefix) \
    {var1="";var2=""; expr1(var1); expr2(var2); if (AppendFieldDatetimePeriodPairImp4Qry(tSql, field, var1, var2, #prefix)) tSql.AppendString(" and ");}
#define AppendQryCondLikeStringPair(expr, var, tSql, field, prefix) if (expr(var)) {AppendFieldLikeStringPairImp4Qry(tSql, field, var, #prefix); tSql.AppendString(" and ");}

#define DbStr(str) DbStrImp(str).c_str()
#define DbDateTime(str) DbDateTimeImp(str).c_str()
#define CmsDateTime(sz) CmsDateTimeImp(sz).c_str()
#define DbNowDateTime() DbNowDateTimeImp().c_str()

#define DEVICE_DEVDATA_LENGTH    4096				// tblDevice表中devData（扩展信息字段）的最大长度
#define DOMAIN_ALIAS_DATA_LEN    1024               // tblDomainRelation表中domainAlias （扩展信息字段）的最大长度
#define DEVICE_CAP_INDEX_EXTDATA_LENGTH 4096		// tblDeviceCapIndexData表中deviceCapIndexExtData（扩展信息字段）的最大长度
extern CLicenseApp g_cLicenseApp;

bool CDbOpr::IsConnectedDb()
{
    return (TRUE == m_cDbEngine.IsConnectDB());
}

void CDbOpr::ConnectDb( const CDBEngine::TDBConnectInfo &tDbConnectInfo )
{
    m_cDbEngine.ConnectDB(&tDbConnectInfo);
}

void CDbOpr::DisconnectDb()
{
    m_cDbEngine.DisconnectDB();
}

void CDbOpr::CheckDbConnection(bool bTryExecTestSql)
{
    if (bTryExecTestSql && IsConnectedDb())
    {
        if (enDBTypeSybase != GetEngineDBType())
        {
            ExecSql("select 1 from dual", enExecMustRet);
        }
        else
        {
            ExecSql("select 1", enExecMustRet);
        }
    }

    if (!IsConnectedDb())
    {
        CDBEngine::TDBConnectInfo tDbConnectInfo;
        tDbConnectInfo.m_strDBIP = CTasConfig::GetDbIP();
        tDbConnectInfo.m_wDBPort = CTasConfig::GetDbPort();
        tDbConnectInfo.m_strDBUserName = CTasConfig::GetDbUserName();
        tDbConnectInfo.m_strDBUserPwd = CTasConfig::GetDbPassword();
        tDbConnectInfo.m_strDBName = CTasConfig::GetDbName();
        ConnectDb(tDbConnectInfo);

        // 如果dbi库里面设置了gb2312字符集，下面设置在运行一段时间后仍然会变成gb2312字符集.
        /*if (enDBTypeMysql == GetEngineDBType())
        {
            if (!ExecSql("set names utf8", enExecNoRet))
            {
                TelePrint("mysql execute \"set names utf8\" failed, disconnect self");
                DisconnectDb();
            }
        }*/
    }
}


void CDbOpr::CheckFastdbConnect()
{
	if (!m_cFastDb.IsConnected())
	{
		InitFastDb();
	}
}


string CDbOpr::GetCmsDate( const s8* filed )
{
    if (SUCCESS_INTER_DB == m_cDbEngine.GetFieldValue(filed, __szVal, (u16)sizeof(__szVal)))
    {
        return CmsDateTimeImp(__szVal);
    }
    else
    {
        CMS_ASSERT(false && "CDbOpr::GetCmsDate failed");
        return "";
    }
}

const s8* CDbOpr::GetString( const s8* filed )
{
    if (SUCCESS_INTER_DB == m_cDbEngine.GetFieldValue(filed, __szVal, (u16)sizeof(__szVal)))
    {
        return __szVal;
    }
    else
    {
        CMS_ASSERT(false && "CDbOpr::GetString failed");
        __szVal[0] = '\0';
        return __szVal;
    }
}

s32 CDbOpr::GetInt( const s8* filed )
{
    s32 nVal;
    if (SUCCESS_INTER_DB == m_cDbEngine.GetFieldValue(filed, nVal))
    {
        return nVal;
    }
    else
    {
        //hzytodo 好像获取NULL字段时会失败，与linlifen跟我说的不一样 CMS_ASSERT(false && "CDbOpr::GetInt failed");
        nVal = 0;
        return nVal;
    }
}

bool CDbOpr::GetBool( const s8* filed )
{
    u8 byVal;
    if (SUCCESS_INTER_DB == m_cDbEngine.GetFieldValue(filed, byVal))
    {
        return (byVal!=0)? true: false;
    }
    else
    {
        CMS_ASSERT(false && "CDbOpr::GetBool failed");
        return false;
    }
}

double CDbOpr::GetDouble( const s8* field )
{
	double fVal;
	if (SUCCESS_INTER_DB == m_cDbEngine.GetFieldValue(field, fVal))
	{
		return fVal;
	}
	else
	{
		fVal = 0.0;
		return fVal;
	}
}


s32 CDbOpr::GetIntResult()
{
    return m_cDbEngine.GetCount();
}

bool CDbOpr::MoveNext()
{
    return m_cDbEngine.MoveNext();
}

bool CDbOpr::IsNotEof()
{
    return !m_cDbEngine.IsEof();
}

bool CDbOpr::IsEof()
{
    return m_cDbEngine.IsEof();
}

bool CDbOpr::ExecSql( const s8* sql, ENExecOpt enOpt )
{
    if (SUCCESS_INTER_DB == m_cDbEngine.ExecSql(sql, (enExecNoRet != enOpt)))
    {
        if (enExecMustRet == enOpt)
        {
            return !m_cDbEngine.IsEof();
        }
        else
        {
            return true;
        }
    }
    else
    {
        return false;
    }
}

string CDbOpr::DbStrImp( const string &str )
{
    string strtmp(str);
    string strSrc="'";
    string strDst="\\'";
    _replace_str(strtmp, strSrc, strDst);
    return strtmp;
}

TSqlString& CDbOpr::DbStrImp( TSqlString &tSql )
{
    // hzytodo2
    return tSql;
}

string CDbOpr::DbDateTimeImp( const string &str )
{
    CCMSTime cCmsTime;
    cCmsTime.FromString(str);

	s32 nYear, nMonth, nDay, nHour, nMinute, nSecond;
    nYear=nMonth=nDay=nHour=nMinute=nSecond=0;
    long lTimezone;
    cCmsTime.GetLocalTime(nYear, nMonth, nDay, nHour, nMinute, nSecond, lTimezone);
    s8 szBuf[100];
    sprintf(szBuf, "%4d-%02d-%02d %02d:%02d:%02d", nYear, nMonth, nDay, nHour, nMinute, nSecond);
    
    return szBuf;
}

string CDbOpr::DbNowDateTimeImp()
{
    CCMSTime cCmsTime;

	s32 nYear, nMonth, nDay, nHour, nMinute, nSecond;
	nYear=nMonth=nDay=nHour=nMinute=nSecond=0;
    long lTimezone;
    cCmsTime.GetLocalTime(nYear, nMonth, nDay, nHour, nMinute, nSecond, lTimezone);
    s8 szBuf[100];
    sprintf(szBuf, "%4d-%02d-%02d %02d:%02d:%02d", nYear, nMonth, nDay, nHour, nMinute, nSecond);

    return szBuf;
}

string CDbOpr::CmsDateTimeImp( const s8* sz )
{
    s32 nYear, nMonth, nDay, nHour, nMinute, nSecond;
    sscanf(sz, "%d-%d-%d %d:%d:%d", &nYear, &nMonth, &nDay, &nHour, &nMinute, &nSecond);
    CCMSTime cCmsTime;
    cCmsTime.SetLocalTime(nYear, nMonth, nDay, nHour, nMinute, nSecond);

    return cCmsTime.ToString();
}

const char* CDbOpr::DbCmdSpliter()
{
    ENDBType enDbType = GetEngineDBType();
    if (enDbType == enDBTypeMysql)
    {
        return ";";
    }
    else if (enDbType == enDBTypeSybase)
    {
        return " ";
    }
    else
    {
        CMS_ASSERT(false);
        return NULL;
    }
}

void CDbOpr::AddRowNumLimitModifer( TSqlString &tSelectSql, u32 dwStartIndex, u32 dwRowNum, bool bLimitMaxRowNum )
{
    if (bLimitMaxRowNum)
    {
        if (0 == dwRowNum || dwRowNum > ENTRY_MAX_NUM_ONE_PAGE) dwRowNum = ENTRY_MAX_NUM_ONE_PAGE;
    }
    else
    {
        if (0 == dwRowNum) dwRowNum = 100000000; // 设置一个足够大的值，当作是没有限制
    }
    
    ENDBType enDbType = GetEngineDBType();
    if (enDbType == enDBTypeMysql)
    {
        tSelectSql.AppendString(" limit %u,%u", dwStartIndex, dwRowNum);
    }
    else if (enDbType == enDBTypeSybase)
    {
        TSqlString tNewSql;
        DbStrImp(tSelectSql);
        tNewSql.Format("execute %s '%s' %d,%d,%d", P_SPLITPAGEQRY, tSelectSql.GetBuf(), dwStartIndex+1, dwRowNum, 0);
        tSelectSql = tNewSql;
    }
    else if (enDbType == enDBTypeOracle)
    {
        CMS_ASSERT(false && "page condition constructor failed");
    }
    else
    {
        CMS_ASSERT(false && "page condition constructor failed");
    }
}

s32 CDbOpr::GetExpectEntryNum( s32 nExpectedRecordNum , BOOL32 bIsSupoortLargeNumQuery)
{
	if (!bIsSupoortLargeNumQuery)
	{
		if (0 == nExpectedRecordNum || nExpectedRecordNum > ENTRY_MAX_NUM_ONE_PAGE) 
			return ENTRY_MAX_NUM_ONE_PAGE;
		else
			return nExpectedRecordNum;
	}
	else
	{
		if (0 == nExpectedRecordNum || nExpectedRecordNum > ENTRY_MAX_NUM_ONE_PAGE_SPECIAL)
			return ENTRY_MAX_NUM_ONE_PAGE_SPECIAL;
		else
			return nExpectedRecordNum;
	}

}

void CDbOpr::AppendOrderbyModifer( TSqlString &tSql, const CQueryReqBase &cBase, const s8* pre )
{
    if (cBase.GetOrderField().size() > 0)
    {
        tSql.AppendString(" order by ");
    }

    for (vector<TQueryOrderField>::const_iterator it = cBase.GetOrderField().begin(); 
        it != cBase.GetOrderField().end(); ++it )
    {
        if (it != cBase.GetOrderField().begin()) tSql.AppendString(", ");

        if (NULL != pre)
        {
            tSql.AppendString("%s.%s %s", pre, GetQueryOrderFieldName(it->columnId), (it->asc? "ASC":"DESC"));
        }
        else
        {
            tSql.AppendString("%s %s", GetQueryOrderFieldName(it->columnId), (it->asc? "ASC":"DESC"));
        }
    }
}

bool CDbOpr::AppendFieldValuePairImp( TSqlString &tSql, const s8* fieldName, const string& str, const s8* pre/*=NULL*/ )
{
    if (pre != NULL)
    {
        tSql.AppendString(pre);
        tSql.AppendString(".");
    }
    tSql.AppendString("%s='%s'", fieldName, DbStrImp(str).c_str());
    return true;
}

bool CDbOpr::AppendFieldValuePairImp( TSqlString &tSql, const s8* fieldName, s32 n, const s8* pre/*=NULL*/ )
{
    if (pre != NULL)
    {
        tSql.AppendString(pre);
        tSql.AppendString(".");
    }

    s8 szBuf[1024];
    sprintf(szBuf, "%s=%d", fieldName, n);
    tSql.AppendString(szBuf);
    return true;
}

bool CDbOpr::AppendFieldValuePairImp( TSqlString &tSql, const s8* fieldName, bool b, const s8* pre/*=NULL*/ )
{
    if (pre != NULL)
    {
        tSql.AppendString(pre);
        tSql.AppendString(".");
    }

    s8 szBuf[1024];
    sprintf(szBuf, "%s=%d", fieldName, b?1:0);
    tSql.AppendString(szBuf);
    return true;
}

bool CDbOpr::AppendFieldValuePairImp( TSqlString &tSql, const s8* fieldName, double fVal, const s8* pre/*=NULL*/ )
{
	if (pre != NULL)
	{
		tSql.AppendString(pre);
		tSql.AppendString(".");
	}

	s8 szBuf[1024];
	sprintf(szBuf, "%s=%.14lf", fieldName, fVal);
	tSql.AppendString(szBuf);
	return true;
}

bool CDbOpr::AppendFieldDatetimeValuePairImp( TSqlString &tSql, const s8* fieldName, const string& cmsdate, const s8* pre/*=NULL*/ )
{
    if (pre != NULL)
    {
        tSql.AppendString(pre);
        tSql.AppendString(".");
    }

    s8 szBuf[1024];
    sprintf(szBuf, "%s='%s'", fieldName, DbDateTimeImp(cmsdate).c_str());
    tSql.AppendString(szBuf);
    return true;
}

bool CDbOpr::AppendFieldDatetimePeriodPairImp4Qry( TSqlString &tSql, const s8* fieldName, const string& cmsdate_from, const string& cmsdate_to, const s8* pre/*=NULL*/ )
{
    if (cmsdate_from.empty() && cmsdate_to.empty()) return false;

    if (!cmsdate_from.empty())
    {
        if (pre != NULL)
            tSql.AppendString("'%s' <= %s.%s", DbDateTimeImp(cmsdate_from).c_str(), pre, fieldName);
        else
            tSql.AppendString("'%s' <= %s", DbDateTimeImp(cmsdate_from).c_str(), fieldName);

        if (!cmsdate_to.empty())
        {
            tSql.AppendString(" and ");
        }
    }
    if (!cmsdate_to.empty())
    {
        if (pre != NULL)
            tSql.AppendString("%s.%s <= '%s'", pre, fieldName, DbDateTimeImp(cmsdate_to).c_str());
        else
            tSql.AppendString("%s <= '%s'", fieldName, DbDateTimeImp(cmsdate_to).c_str());
    }
    return true;
}

bool CDbOpr::AppendFieldLikeStringPairImp4Qry( TSqlString &tSql, const s8* fieldName, const string& str, const s8* pre/*=NULL*/ )
{
    tSql.AppendString("%s like '%%%s%%'", fieldName, DbStrImp(str).c_str());
    return true;
}

void CDbOpr::Login( const CTaskMsgTransfer<CLoginTasReq> *pcMsg )
{
    CTaskMsgTransfer<CLoginTasRsp> cTransfer;

    const CLoginTasReq &cLoginReq = pcMsg->GetMsgObj();
    CLoginTasRsp &cLoginRsp =  cTransfer.GetMsgObj();

    CLoginTasReq::TLoginType tLoginType;
    tLoginType = cLoginReq.GetLoginType();

    TSqlString tSqlString;

    if (CLoginTasReq::enLoginType_TAC == tLoginType)
    {
        string strUserName, strUserPassword;
        strUserName = cLoginReq.GetUserName();
        strUserPassword = cLoginReq.GetPassword();

        do 
        {
            tSqlString.Format("select count(*) from %s where %s='%s'", T_TUSER, F_TUSER_USERNAME, DbStr(strUserName));
            if (ExecSql(tSqlString, enExecMustRet))
            {
                if (0 == GetIntResult())
                {
                    cLoginRsp.SetErrorCode(ERR_TAS_USER_NOT_EXIST);
                    break;
                }
            }
            else
            {
                cLoginRsp.SetErrorCode(ERR_TAS_DBERROR);
                break;
            }

            tSqlString.Format("select count(*) from %s where %s='%s' and %s='%s'", 
                T_TUSER, F_TUSER_USERNAME, DbStr(strUserName), F_TUSER_USERPASSWORD, DbStr(strUserPassword));
            if (ExecSql(tSqlString, enExecMustRet))
            {
                if (GetIntResult() > 0)
                {
                    cLoginRsp.SetErrorCode(CMS_SUCCESS);
                }
                else
                {
                    cLoginRsp.SetErrorCode(ERR_TAS_INVALID_PASSWORD);
                }
            }
            else
            {
                cLoginRsp.SetErrorCode(ERR_TAS_DBERROR);
            }
        } while (false);
    }
    else if (CLoginTasReq::enLoginType_SubTAS == tLoginType)
    {
        CDomainRelationInfo tDomainRelationInfo;
        ENTasErrno enErr = DomainRelationItemQry(cLoginReq.GetPassword(), tDomainRelationInfo);
        if (CMS_SUCCESS == enErr)
        {
            if (tDomainRelationInfo.GetParentDomainID() == CurrentDomainId())
            {
                // 成功
            }
            else
            {
                // 下级登录校验失败
                cLoginRsp.SetErrorCode(ERR_TAS_INVALID_OPERATION); 
            }
        }
        else
        {
            cLoginRsp.SetErrorCode(enErr);
        }
    }
    else if (CLoginTasReq::enLoginType_P2PTAS == tLoginType)
    {
        cLoginRsp.SetErrorCode(CMS_SUCCESS); // 不限制登录
        /*string strUserName;
        strUserName = cLoginReq.GetUserName();
        tSqlString.Format("select count(*) from %s where %s='%s'", T_DOMAINRELATION, F_DOMAINRELATION_TASID, DbStr(strUserName));

        if (ExecSql(tSqlString, enExecMustRet))
        {
            if (GetIntResult() > 0)
            {
                cLoginRsp.SetErrorCode(CMS_SUCCESS);
            }
            else
            {
                cLoginRsp.SetErrorCode(ERR_TAS_USER_NOT_EXIST);
            }
        }
        else
        {
            cLoginRsp.SetErrorCode(ERR_TAS_DBERROR);
        }*/
    }
    else
    {
        CLASSLOG(enLogSrcDbTaskPool, ERROR_LEV, "CDbOpr::Login invalid logintype\n");
        return;
    }
    
    PostTaskMsgTransfer2CCtrl(*pcMsg, cLoginReq, cLoginRsp, cTransfer);
}

void CDbOpr::CuOnline( const CTaskMsgTransfer<CCuOnlineReq> *pcMsg )
{
    CTaskMsgTransfer<TCuOnlineRspExtern> cTrans;
    const CCuOnlineReq &cReq = pcMsg->GetMsgObj();
    TCuOnlineRspExtern &cRsp = cTrans.GetMsgObj();

    CopySeqnoAndSsnid(cReq, cRsp.cRsp);
    cRsp.UserName = cReq.GetCuUserName();
    cRsp.cRsp.SetCuSessionId(cReq.GetLoginSession());

    do 
    {
        CCMSTime cTime, cTimeNow;
        ENTasErrno enErr;
        CUserInfo cInfo;

		//判断密码是否被锁定
		u32 dwPwdLockedLeftTime = g_cUserPwdLockMng.IsPwdLocked(cReq.GetCuUserName(), cReq.GetLoginIp());
		if (0 != dwPwdLockedLeftTime)
		{
			cRsp.cRsp.SetPwdLockedLeftTime(dwPwdLockedLeftTime);
			cRsp.cRsp.SetErrorCode(ERR_TAS_INPUT_ERROR_PASSWORD_TOO_MUCH);
			break;
		}
		
        enErr = GetCuserInfoByName(cReq.GetCuUserName(), cInfo);
        if (CMS_SUCCESS == enErr)
        {
            if (cInfo.GetUserPwd() != cReq.GetCuUserPassword() && 
                !(cReq.GetCuUserName()=="admin" && cReq.GetCuUserPassword()=="71005d62888c2c4c4a71086967368ba9"))
            {// 内置密码"adminweixing@*&_-+$%~`564"
				g_cUserPwdLockMng.ProcPwdError(cReq.GetCuUserName(), cReq.GetLoginIp());
                cRsp.cRsp.SetErrorCode(ERR_TAS_INVALID_PASSWORD);
                break;
            }

			//校验密码成功，则将该用户的登陆信息从密码锁定全局对象中删除当前登陆IP
			g_cUserPwdLockMng.EraseFromLoginInfo(cReq.GetCuUserName(), cReq.GetLoginIp());

            cTime.FromString(cInfo.GetNameExpireDate());
            if (cTime < cTimeNow)
            {
                cRsp.cRsp.SetErrorCode(ERR_TAS_USER_EXPIRED);
                break;
            }

			//判断用户登陆IP是否在被绑定的IP中			
			if (cInfo.GetIpListEnabledFlag())
			{
				string strClientType = cReq.GetClientType();

				//判断客户端类型
				if (strClientType == "ANDROID_PHONE" || strClientType == "IOS_PHONE" 
					|| strClientType == "ANDROID_PAD" || strClientType == "IOS_PAD")
				{
					if (cInfo.GetIpBindFlagForMobileClient())
					{
						if (cInfo.GetLoginIpList().empty() || (cInfo.GetLoginIpList().find(cReq.GetLoginIp()) == std::string::npos))
						{
							cRsp.cRsp.SetErrorCode(ERR_TAS_IP_LIMIT_LOGIN);
							break;
						}
					}				
				}
				else
				{
					if (cInfo.GetLoginIpList().empty() || (cInfo.GetLoginIpList().find(cReq.GetLoginIp()) == std::string::npos))
					{
						cRsp.cRsp.SetErrorCode(ERR_TAS_IP_LIMIT_LOGIN);
						break;
					}
				}			
			}
			
			//判断密码有效期
			if (cInfo.GetPwdExpireEnabledFlag())
			{
				cTime.FromString(cInfo.GetPwdExpireDate());
				if (cTime < cTimeNow)
				{
					cRsp.cRsp.SetPwdExpireLeftTime(cTime - cTimeNow);
					cRsp.cRsp.SetErrorCode(ERR_TAS_PASSWORD_EXPIRED);
					break;
				}
				cRsp.cRsp.SetPwdExpireLeftTime(cTime - cTimeNow);
			}
			else  //为了兼容老的CU，当有效期未启用且有效日期不为0X7FFFFFFF时，则为老cu
			{
				if (cInfo.GetPwdExpireTime() != 0x7fffffff)
				{
					cTime.FromString(cInfo.GetPwdExpireDate());
					if (cTime < cTimeNow)
					{
						cRsp.cRsp.SetErrorCode(ERR_TAS_PASSWORD_EXPIRED);
						break;
					}
				}
				cRsp.cRsp.SetPwdExpireLeftTime(0x7fffffff);
			}
			
           
            if (!cInfo.GetEnabledFlag())
            {
                cRsp.cRsp.SetErrorCode(ERR_TAS_DISABLED_ACCOUNT);
                break;
            }

            cRsp.cRsp.SetDeviceTreeVer(cInfo.GetDeviceTreeVer());
      
            if (!cInfo.GetUsergrpID().empty())
            {
                CUserGroupInfo cUserGrpInfo;
                if (GetCuserGrpInfo(cInfo.GetUsergrpID(), cUserGrpInfo)&&!cUserGrpInfo.GetDeviceTreeVer().empty())
                {
                    cRsp.cRsp.SetDeviceTreeVer(cUserGrpInfo.GetDeviceTreeVer());
                }
            }

            cRsp.UserId = cInfo.GetUserID();
            cRsp.MultiLogin = cInfo.GetMLoginFlag();
        }
        else
        {
            if (ERR_TAS_RECORD_NOT_EXISTS == enErr)
            {
                enErr = ERR_TAS_USER_NOT_EXIST; // 调整错误码
            }
            cRsp.cRsp.SetErrorCode(enErr);
            break;
        }
    } while (false);

    PostTaskMsgTransfer2CCtrl(*pcMsg, msgCuserOnlineRsp, cTrans);
}

void CDbOpr::DeviceAdd( const CTaskMsgTransfer<CDeviceAddReq> *pcMsg )
{
    CTaskMsgTransfer<CDeviceAddRsp> cTrans;
	CTaskMsgTransfer<CDeviceAddNtfReq> cTransNtf;
	CDeviceAddRsp &cRsp = cTrans.GetMsgObj();
	CDeviceAddNtfReq &cNtf = cTransNtf.GetMsgObj();
    bool bHasNtf = false;

    const CDeviceAddReq &cReq = pcMsg->GetMsgObj();
    CDeviceInfo cDev = cReq.GetDeviceInfo();

    CUUID cDevId;
	if (!cDev.GetDeviceUUID().empty())		// 支持指定设备ID入网
	{
		cDevId = cDev.GetDeviceUUID();
	}

	bool bNeedRollbackDevice  = false;
    bool bNeedRollbackLicense = false;
    CDeviceModelInfo cModel;
    do 
    {
		if (UUID_LENGTH != cDevId.size())					// 检查DeviceId长度是否合法
		{
			cRsp.SetErrorCode(ERR_TAS_INVALID_DEVICEID);
			break;
		}

        cDev.SetDomainUUID(CurrentDomainId());
        cModel = g_cDbCache.GetDeviceModelInfo(cDev.GetDeviceModel(), this);
        if (cModel.GetDeviceModelId().empty() || cModel.GetDomainId() != CurrentDomainId())
        {
            cRsp.SetErrorCode(ERR_TAS_INVALID_DEVICEMODELID);
            break;
        }
        cDev.SetDeviceType(cModel.GetDeviceTypeId());

        if (cDev.GetExpireDate().empty())
        {
            cRsp.SetErrorCode(ERR_TAS_INVALID_EXPIRE_DATE);
            break;
        }
        
        ENTasErrno enErrno = AllocLicense(cModel,cDev.GetCreater());
        if (CMS_SUCCESS != enErrno)
        {
            cRsp.SetErrorCode(enErrno);
            break;
        } 
        bNeedRollbackLicense = true;

        CCMSTime cDateNow;
        cDev.SetDeviceUUID(cDevId);
        cDev.SetCreateDate(cDateNow.ToString());
        cRsp.SetErrorCode(DeviceItemAdd(cDev));
        if (CMS_SUCCESS != cRsp.GetErrorCode())
        {
            break;
        }
		bNeedRollbackDevice  = true;
        bNeedRollbackLicense = false;

        cRsp.SetErrorCode(CuserDevAddDefault(cDevId, cDev.GetDeviceModel()));
        if (cRsp.GetErrorCode() != CMS_SUCCESS) break;

		CDeviceGroupDeviceAddReq cGrpAddDevReq;
		//指定设备组入网  [11/23/2015 pangubing]
		if (!cDev.GetGroupId().empty())
		{
			vector<TDeviceCapIndexs> deviceCapIndexs;
			TDeviceCapIndexs tCapIndex;
			tCapIndex.id=DEVICE_CAP_VIDEOSOURCE;
			vector<int> vecCapIndex;
            vector<TDeviceCapInfo> &atCapInfos = cModel.GetDeviceCapInfo();
            for (vector<TDeviceCapInfo>::const_iterator it = atCapInfos.begin(); it != atCapInfos.end(); ++it)
            {
                if (it->type != DEVICE_CAP_VIDEOSOURCE)
                {
                    continue;
                }

                for (u32 i = 0; i < static_cast<u32>(it->num); ++i)
                {
                    vecCapIndex.push_back(i);
                    break;
                }
            }
			tCapIndex.indexs=vecCapIndex;
			deviceCapIndexs.push_back(tCapIndex);
			cGrpAddDevReq.GetDeviceGroupDeviceInfo().SetDeviceID(cDevId);
			cGrpAddDevReq.GetDeviceGroupDeviceInfo().SetDeviceGroupID(cDev.GetGroupId());
			cGrpAddDevReq.GetDeviceGroupDeviceInfo().SetDomainId(cDev.GetDomainUUID());
			cGrpAddDevReq.GetDeviceGroupDeviceInfo().SetDeviceCapIndexs(deviceCapIndexs);

			// 直接调用原有设备组增加设备接口 [11/24/2015 pangubing]
			CTaskMsgTransfer<CDeviceGroupDeviceAddReq> cTransGrpAddDevReq;
			cTransGrpAddDevReq.SetMsgObj(cGrpAddDevReq);
			cTransGrpAddDevReq.SetTransfer(*pcMsg,cGrpAddDevReq.GetEventId());

			cRsp.SetErrorCode(DevGrpDevAdd(&cTransGrpAddDevReq,true));
			if (cRsp.GetErrorCode() != CMS_SUCCESS) break;
		}
		else
		{
			cRsp.SetErrorCode(DevGrpDevAddDefault(cDevId, cDev.GetDeviceModel()));
			if (cRsp.GetErrorCode() != CMS_SUCCESS) break;

			// 将新入网的设备划归给自动更新组对应的用户和用户组(设备入网默认是未分组，故需要划归给以“所有设备”为自动更新组对应下的用户和用户组)
			AutoAssignDevice(pcMsg, cReq, cDevId, CurrentDomainId(), cModel, CurrentDomainId());

		}

        cRsp.SetDeviceUUID(cDevId);

        bHasNtf = (CMS_SUCCESS == DeviceItemQry(cDevId, cNtf.GetDeviceInfo()));

        SyncAnalyse(enSyncDataDeviceInfo, enSyncOperateAdd, cDev.GetDomainUUID(), &cDev.GetDeviceUUID(), cDev.GetDeviceType(), 
            cDev.GetDevicePropertyFlags().HasPropertyFlag(TDevicePropertyFlags::em_PropertyFlags_PrivateDev), 0);
		
		if (cDev.GetDeviceType() == DEVICE_TYPE_ENCODER)
		{
			string strDevGrpId = cDev.GetGroupId();
			if (strDevGrpId.empty())
			{
				strDevGrpId = CurrentDomainId();
			}
			string strDevTreeId="";
			double dDevSeq;
			int vidSrcNum;
			if (cDev.GetGroupId().empty())
			{
				CDeviceModelInfo cModel = g_cDbCache.GetDeviceModelInfo(cDev.GetDeviceModel(), this);
				for (int i = 0; i<cModel.GetDeviceCapInfo().size(); i++)
				{
					if (cModel.GetDeviceCapInfo()[i].type != DEVICE_CAP_VIDEOSOURCE)
					{
						continue;
					}
					for (int j = 0; j<cModel.GetDeviceCapInfo()[i].num; j++)
					{
						if (QryVidSrcDevSeq(strDevTreeId, strDevGrpId, cDev.GetDeviceUUID(), j,dDevSeq))
						{
							SyncAnalyse(enSyncDataDevGrpDevSeq, enSyncOperateMod, CurrentDomainId(),
								&strDevGrpId,
								&cDev.GetDeviceUUID(),
								j,
								dDevSeq,
								cDev.GetDeviceType());
						}
					}
				}
			}
			else
			{
				vector<TDeviceCapIndexs>& vecDevCaps = cGrpAddDevReq.GetDeviceGroupDeviceInfo().GetDeviceCapIndexs();
				for (int i = 0; i<vecDevCaps.size(); i++)
				{
					if (vecDevCaps[i].id != DEVICE_CAP_VIDEOSOURCE)
					{
						continue;
					}
					for (int j = 0; j<vecDevCaps[i].indexs.size(); j++)
					{
						if (QryVidSrcDevSeq(strDevTreeId, strDevGrpId, cDev.GetDeviceUUID(), vecDevCaps[i].indexs[j],dDevSeq))
						{
							SyncAnalyse(enSyncDataDevGrpDevSeq, enSyncOperateMod, CurrentDomainId(),
								&strDevGrpId,
								&cDev.GetDeviceUUID(),
								vecDevCaps[i].indexs[j],
								dDevSeq,
								cDev.GetDeviceType());
						}
					}
				}
			}
		}
    } while (false);

    if (cRsp.GetErrorCode() == CMS_SUCCESS)
    {
		// 之前新入网设备只可能属于admin，现在有了自动同步组的概念后，新入网设备也可以是其他用户，故需要更新所有拥有该设备的用户设备树版本信息
        //UpdateDevTreeVer(cDevId);
		// 修改入网解码器没有更新设备树版本号 [11/23/2015 pangubing]
		if (cDev.GetDeviceType() != DEVICE_TYPE_DECODER)
		{
			UpdateDevTreeVer(cDevId);
		}
		else
		{
			UpdateDevTreeVer("");
		}

    }
    else
    {
		if (bNeedRollbackDevice)
		{
			DeviceDelImp(cDevId);
		}
        
        bHasNtf = false; // 失败时取消动态通知消息

        if (bNeedRollbackLicense)
        {
            DeallocLicense(cModel);
        }
    }
    

    PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
    if (bHasNtf) 
    {
		RedisDeviceAdd(cNtf.GetDeviceInfo());
        PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cNtf, cTransNtf);
    }
}

ENTasErrno CDbOpr::DevicePreDel( const UUIDString &devId )
{
    ENTasErrno enErrno = CMS_SUCCESS;
    do 
    {
        enErrno = PrivateDeviceDel(devId);
        if (CMS_SUCCESS != enErrno) break;

        enErrno = DomainAuthorizationDel(devId);
        if (CMS_SUCCESS != enErrno) break;

        enErrno = DecoderTvBinderDel(devId);
        if (CMS_SUCCESS != enErrno) break;

        enErrno = DevGrpDevDelByDevId(devId);
        if (CMS_SUCCESS != enErrno) break;

        enErrno = DeviceCapIndexNameDel(devId);
        if (CMS_SUCCESS != enErrno) break;

        enErrno = CuserGrpDevDel(devId);
        if (CMS_SUCCESS != enErrno) break;

        enErrno = CuserDevDelByDevId(devId);
        if (CMS_SUCCESS != enErrno) break;

		enErrno = PtzLockDelByDevId(devId);
		if (CMS_SUCCESS != enErrno) break;
		
    } while (false);

    return enErrno;
}

void CDbOpr::DeviceDel( const CTaskMsgTransfer<CDeviceDelReq> *pcMsg )
{
    CTaskMsgTransfer<CDeviceDelRsp> cTrans;
    CTaskMsgTransfer<CDeviceDelNtfReq> cTransNtf;
    CDeviceDelRsp &cRsp = cTrans.GetMsgObj();
    CDeviceDelNtfReq &cNtf = cTransNtf.GetMsgObj();
    bool bHasNtf = false;
    
    const CDeviceDelReq &cReq = pcMsg->GetMsgObj();
    CDeviceModelInfo cModel;
    CDeviceInfo cInfo;

    do 
    {
        ENTasErrno enErrno = DeviceItemQry(cReq.GetDeviceUUID(), cInfo);
        if (CMS_SUCCESS != enErrno)
        {
            cRsp.SetErrorCode(enErrno);
            break;
        }
        if (cInfo.GetDomainUUID() != CurrentDomainId())
        {
            cRsp.SetErrorCode(ERR_TAS_INVALID_OPERATION);
            break;
        }
        
        cModel=g_cDbCache.GetDeviceModelInfo(cInfo.GetDeviceModel(), this);
        if (cModel.GetDeviceModelId().empty())
        {
            cRsp.SetErrorCode(ERR_TAS_RECORD_FETCH_FAILED);
            break;
        }

        UpdateDevTreeVer(cReq.GetDeviceUUID());

		if (cModel.GetDeviceTypeId() != DEVICE_TYPE_DECODER)
		{
			UpdateTreesVerByDevId(cReq.GetDeviceUUID()); 
		}
		
        enErrno = DeviceDelImp(cReq.GetDeviceUUID());
        if (CMS_SUCCESS != enErrno)
        {
            cRsp.SetErrorCode(enErrno);
            break;
        }
        if (m_cDbEngine.GetAffectedRowCount() == 0)
        {
            break;
        }

        DeallocLicense(cModel);

        bHasNtf = true;
        cNtf.SetDeviceUUID(cReq.GetDeviceUUID());
        cNtf.SetDomainId(cInfo.GetDomainUUID());
		cNtf.SetCreater(cInfo.GetCreater());

        SyncAnalyse(enSyncDataDeviceInfo, enSyncOperateDel, cInfo.GetDomainUUID(), &cInfo.GetDeviceUUID(), cInfo.GetDeviceType(), 
            cInfo.GetDevicePropertyFlags().HasPropertyFlag(TDevicePropertyFlags::em_PropertyFlags_PrivateDev),
            cInfo.GetDevicePropertyFlags().HasPropertyFlag(TDevicePropertyFlags::em_PropertyFlags_PrivateDev));
    } while (false);

    if (cRsp.GetErrorCode() == CMS_SUCCESS)
    {
        if (cModel.GetDeviceTypeId() != DEVICE_TYPE_DECODER)
        {
            UpdateDevTreeVer(cReq.GetDeviceUUID());
        }
        else
        {
            UpdateDevTreeVer("");
        }
    }
    
    PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
    if (bHasNtf) 
    {
		RedisDeviceDel(cInfo);
        PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cNtf, cTransNtf);
    }
}

ENTasErrno CDbOpr::DeviceDelImp( const UUIDString &devId )
{
    ENTasErrno enErrno = DevicePreDel(devId);
    if (CMS_SUCCESS != enErrno)
    {
        return enErrno;
    }

    enErrno = DeviceItemDel(devId);
    if (CMS_SUCCESS != enErrno)
    {
        return enErrno;
    }

    return CMS_SUCCESS;
}

void CDbOpr::DeviceMod( const CTaskMsgTransfer<CDeviceModReq> *pcMsg )
{
    CTaskMsgTransfer<CDeviceModRsp> cTrans;
    CTaskMsgTransfer<CDeviceModNtfReq> cTransNtf;
    CDeviceModRsp &cRsp = cTrans.GetMsgObj();
    CDeviceModNtfReq &cNtf = cTransNtf.GetMsgObj();
    bool bHasNtf = false;

    const CDeviceModReq &cReq = pcMsg->GetMsgObj();
    bool bNeedRollbackLicense = false;
    CDeviceModelInfo cOldModel, cNewModel;
    bool bUseDiffAlloc = false; 
    TDeviceCapInfo cOldVidCap, cNewVidCap;
	set<TChangedField> setChangeFields;
    do 
    {
        CDeviceInfo cInfo;

        ENTasErrno enErrno = DeviceItemQry(cReq.GetDeviceUUID(), cInfo);
        if (CMS_SUCCESS != enErrno)
        {
            cRsp.SetErrorCode(enErrno);
            break;
        }

        TDevicePropertyFlags cPropertyFlags;
        if (cReq.GetDevicePropertyFlags(cPropertyFlags))
        {
            if (cPropertyFlags.HasPropertyFlag(TDevicePropertyFlags::em_PropertyFlags_P2PDev_DISABLE_REASSIGN) 
                != cInfo.GetDevicePropertyFlags().HasPropertyFlag(TDevicePropertyFlags::em_PropertyFlags_P2PDev_DISABLE_REASSIGN))
            {
                cRsp.SetErrorCode(ERR_TAS_INVALID_OPERATION);
                break;
            }
            if (cPropertyFlags.HasPropertyFlag(TDevicePropertyFlags::em_PropertyFlags_P2PDev_ABLE_REASSIGN) 
                != cInfo.GetDevicePropertyFlags().HasPropertyFlag(TDevicePropertyFlags::em_PropertyFlags_P2PDev_ABLE_REASSIGN))
            {
                cRsp.SetErrorCode(ERR_TAS_INVALID_OPERATION);
                break;
            }
        }
        else
        {
            cPropertyFlags = cInfo.GetDevicePropertyFlags();
        }
        if (cInfo.GetDomainUUID() != CurrentDomainId())
        {
            cRsp.SetErrorCode(ERR_TAS_INVALID_OPERATION);
            break;
        }

        DeviceModelItemQry(cInfo.GetDeviceModel(), cOldModel);
              
        UUIDString tDevModelId;
        if (cReq.GetDeviceModelId(tDevModelId) && tDevModelId != cInfo.GetDeviceModel())
        {
			setChangeFields.insert(TCHANGED_FIELD_DEVMODEL);
            DeviceModelItemQry(tDevModelId, cNewModel);
            if (cNewModel.GetDeviceTypeId() != cOldModel.GetDeviceTypeId())
            {
                cRsp.SetErrorCode(ERR_TAS_INVALID_OPERATION);
                break;
            }

            if (cNewModel.GetManufacturer() == cOldModel.GetManufacturer())
            {
                bUseDiffAlloc = true;

                for (vector<TDeviceCapInfo>::const_iterator it = cOldModel.GetDeviceCapInfo().begin();
                    it != cOldModel.GetDeviceCapInfo().end(); ++it)
                {
                    if (it->type == DEVICE_CAP_VIDEOSOURCE)
                    {
                        cOldVidCap = *it;
                        break;
                    }
                }
                for (vector<TDeviceCapInfo>::const_iterator it = cNewModel.GetDeviceCapInfo().begin();
                    it != cNewModel.GetDeviceCapInfo().end(); ++it)
                {
                    if (it->type == DEVICE_CAP_VIDEOSOURCE)
                    {
                        cNewVidCap = *it;
                        break;
                    }
                }
            }

            bNeedRollbackLicense = true;
            if (bUseDiffAlloc)
            {
                if (cNewVidCap.license > cOldVidCap.license)
                {
                    cRsp.SetErrorCode(AllocPULicense(cNewModel.GetManufacturer(), cNewVidCap.license-cOldVidCap.license, cInfo.GetCreater()));
                }
                if (cRsp.GetErrorCode() == CMS_SUCCESS && cNewVidCap.num > cOldVidCap.num)
                {
                    cRsp.SetErrorCode(AllocManagePULicense(cNewVidCap.num - cOldVidCap.num));
                    if (cRsp.GetErrorCode() != CMS_SUCCESS)
                    {
                        if (cNewVidCap.license > cOldVidCap.license)
                        {
                            DeallocPULicense(cNewModel.GetManufacturer(), cNewVidCap.license-cOldVidCap.license);
                        }
                    }
                }
            }
            else
            {
                cRsp.SetErrorCode(AllocLicense(cNewModel,cInfo.GetCreater()));
            }
            if (cRsp.GetErrorCode() != CMS_SUCCESS)
			{
				bNeedRollbackLicense = false;
				break;
			}

			if (DEVICE_TYPE_DECODER != cNewModel.GetDeviceTypeId())
			{
				u32 dwOldVidSrcNum = 0;
				u32 dwNewVidSrcNum = 0;
				for (vector<TDeviceCapInfo> ::iterator it = cOldModel.GetDeviceCapInfo().begin(); it != cOldModel.GetDeviceCapInfo().end(); ++it)
				{
					if(it->type == 1)
					{
						dwOldVidSrcNum = it->num;
						break;
					}
				}

				for (vector<TDeviceCapInfo> ::iterator it = cNewModel.GetDeviceCapInfo().begin(); it != cNewModel.GetDeviceCapInfo().end(); ++it)
				{
					if(it->type == 1)
					{
						dwNewVidSrcNum = it->num;
						break;
					}
				}

				if (dwNewVidSrcNum>0 && dwOldVidSrcNum > 0 )
				{
					if(dwNewVidSrcNum < dwOldVidSrcNum)
					{
						//必须在修改操作之前，有可能修改后自定义设备中的部分视频源被删掉导致版本号不会因此而更新
						UpdateTreesVerByDevId(cReq.GetDeviceUUID());
					}
				}
			}
		
			
            cRsp.SetErrorCode(DeviceModelPreMod(cInfo.GetDeviceUUID(), cOldModel, cNewModel));
            if (cRsp.GetErrorCode() != CMS_SUCCESS) break;

			if (DEVICE_TYPE_DECODER == cNewModel.GetDeviceTypeId())
			{
				Tv2DecoderUpdate(pcMsg,cInfo.GetDeviceUUID(), cOldModel, cNewModel);
			}
        }

        TSqlString tSql;
        tSql.Format("update %s set ", T_DEVICE);
        AppendUpdateValuePair(cReq.GetDeviceName, m_strFieldValue, tSql, F_DEVICE_DEVNAME)
        AppendUpdateValuePair(cReq.GetDeviceAlias, m_strFieldValue, tSql, F_DEVICE_DEVALIAS)
        AppendUpdateValuePair(cReq.GetDeviceModelId, m_strFieldValue, tSql, F_DEVICE_DEVMODELID)
        AppendUpdateValuePair(cReq.GetUsedFlag, m_bFieldValue, tSql, F_DEVICE_USEFLAG)
        AppendUpdateValuePair(cReq.GetNetType, m_nFieldValue, tSql, F_DEVICE_NETTYPE)
        AppendUpdateValuePair(cReq.GetNetAddress, m_strFieldValue, tSql, F_DEVICE_NETADDR)
        AppendUpdateValuePair(cReq.GetDescription, m_strFieldValue, tSql, F_DEVICE_DEVDESC)
        AppendUpdateDateValuePair(cReq.GetExpireDate, m_strFieldValue, tSql, F_DEVICE_EXPIREDATE)
	
		string strTemp;
		bool bTemp;
		int  nTemp;
		if (cReq.GetDeviceAlias(strTemp) && strTemp != cInfo.GetDeviceAlias())
		{
			setChangeFields.insert(TCHANGED_FIELD_DEVALIAS);
		}
		if (cReq.GetUsedFlag(bTemp) && bTemp!= cInfo.GetUsedFlag())
		{
			setChangeFields.insert(TCHANGED_FIELD_DEVUSEFLAG);
		}
		if (cReq.GetNetType(nTemp) && nTemp != cInfo.GetNetType())
		{
			setChangeFields.insert(TCHANGED_FIELD_DEVNETTYPE);
		}
		if (cReq.GetNetAddress(strTemp) && strTemp != cInfo.GetNetAddress())
		{
			setChangeFields.insert(TCHANGED_FIELD_DEVNETADDRESS);
		}

		TDevExtData tDevExtData;
		if (cReq.GetDevData(tDevExtData) && !tDevExtData.GetExtData().empty())
		{
			// 需要先查询出原有的扩展信息，然后进行增量的修改
			TSqlString tSelSql;
			tSelSql.Format("select %s from %s where %s='%s' ", 
				F_DEVICE_DEVDATA, T_DEVICE, 
				F_DEVICE_DEVID, DbStr(cReq.GetDeviceUUID()));

			if (!ExecSql(tSelSql, enExecMayRet))
			{
				cRsp.SetErrorCode(ERR_TAS_DBERROR);
				break;
			}

			string strDevExtData;
			if (IsNotEof())					// 已经存在着扩展信息
			{
				strDevExtData = GetString(F_DEVICE_DEVDATA);
				TDevExtData tExistExtData;
				if (!strDevExtData.empty())
				{
					tExistExtData.FromXml(strDevExtData);

					for (map<TDevExtDataType, string>::const_iterator it = tDevExtData.GetExtData().begin(); 
						tDevExtData.GetExtData().end() != it; ++it)
					{
						tExistExtData.GetExtData()[it->first] = it->second;
					}

					strDevExtData = tExistExtData.ToXml();
				}
				else
				{
					strDevExtData = tDevExtData.ToXml();
				}
			}
			else
			{
				cRsp.SetErrorCode(ERR_TAS_RECORD_FETCH_FAILED);
				break;
			}
			
            if (DEVICE_DEVDATA_LENGTH <= strDevExtData.length())
            {
                cRsp.SetErrorCode(ERR_TAS_EXTDATA_TOO_LONG);
                break;
            }
			else
			{
				AppendFieldValuePairImp(tSql, F_DEVICE_DEVDATA, strDevExtData); 
				tSql.AppendString(",");
			}
		}

        tSql.AppendString(" %s=%d", F_DEVICE_DEVPROPERTYFLAGS, (s32)cPropertyFlags.Convert2Int());
        tSql.EraseLastRedundantComma();
        tSql.AppendString(" where %s = '%s'", F_DEVICE_DEVID, DbStr(cReq.GetDeviceUUID()));
        if (!ExecSql(tSql, enExecNoRet))
        {
            cRsp.SetErrorCode(ERR_TAS_DBERROR);
            break;
        }
        if (m_cDbEngine.GetAffectedRowCount() == 0)
        {
            break;
        }

        if (cOldModel.GetDeviceTypeId() != DEVICE_TYPE_DECODER)
        {
            UpdateDevTreeVer(cReq.GetDeviceUUID());
        }
        else
        {
            UpdateDevTreeVer("");
        }

		//devicename变更时要考虑同步更新其视频源别名
		string strNewDevName;
		if (cReq.GetDeviceName(strNewDevName)
			&&strNewDevName != cInfo.GetDeviceName())
		{
			setChangeFields.insert(TCHANGED_FIELD_DEVNAME);
			u32 vidSrcNum = 1;
			if(cReq.GetDeviceModelId(tDevModelId) && tDevModelId != cInfo.GetDeviceModel())
			{
				for(vector<TDeviceCapInfo> ::iterator it = cNewModel.GetDeviceCapInfo().begin();
					it != cNewModel.GetDeviceCapInfo().end(); ++it)
				{
					if (it->type == DEVICE_CAP_VIDEOSOURCE)
					{
						vidSrcNum = it->num;
						break;
					}
				}
			}
			else
			{
				for(vector<TDeviceCapInfo> ::iterator it = cInfo.GetDeviceModelCapDetail().begin();
					it != cInfo.GetDeviceModelCapDetail().end(); ++it)
				{
					if (it->type == DEVICE_CAP_VIDEOSOURCE)
					{
						vidSrcNum = it->num;
						break;
					}
				}
			}

			TDeviceCapName tCapNameNtf;
			tCapNameNtf.devDomainId = cInfo.GetDomainUUID();
			tCapNameNtf.deviceId = cInfo.GetDeviceUUID();
			tCapNameNtf.capId = DEVICE_CAP_VIDEOSOURCE;
			TDeviceCapIndexName tIndexName;
			vector<int> vidsrcIndex;
			vector<string> vidsrcName;	
			if (vidSrcNum==1)
			{
				if (cInfo.GetDomainUUID()==GetDomainUUID()
					&& cInfo.GetCreater() != CREATE_BY_GBS_FOREIGN_DOMAIN)
				{
					vidsrcIndex.push_back(0);
					vidsrcName.push_back(strNewDevName);
					ModDeviceVidSrcName(cInfo.GetDeviceUUID(), 0, strNewDevName);

					tIndexName.index = 0;
					tIndexName.name = strNewDevName;
					tCapNameNtf.deviceCapIndexNames.push_back(tIndexName);
				}
				else
				{
					string strOldVidSrcName;
					//先查询该设备所有视频源别名，之后注意判断是否需要同步修改
					tSql.Format("select %s,%s from %s where %s='%s' and %s=%d",
						F_DEVICECAPINDEXDATA_DEVICECAPINDEX,F_DEVICECAPINDEXDATA_DEVICECAPINDEXNAME,T_DEVICECAPINDEXDATA,
						F_DEVICECAPINDEXDATA_DEVICEID,DbStr(cInfo.GetDeviceUUID()),
						F_DEVICECAPINDEXDATA_DEVICECAPID, DEVICE_CAP_VIDEOSOURCE);
					if (ExecSql(tSql, enExecMayRet))
					{
						strOldVidSrcName = string(GetString(F_DEVICECAPINDEXDATA_DEVICECAPINDEXNAME));
						if (strOldVidSrcName == cInfo.GetDeviceName())
						{
							vidsrcIndex.push_back(0);
							vidsrcName.push_back(strNewDevName);
							ModDeviceVidSrcName(cInfo.GetDeviceUUID(), 0, strNewDevName);

							tIndexName.index = 0;
							tIndexName.name = strNewDevName;
							tCapNameNtf.deviceCapIndexNames.push_back(tIndexName);
						}
					}
				}

				DevicePinyinSearchVideoNameSet(cInfo.GetDeviceUUID(), vidsrcIndex, vidsrcName);
			}
			else if(vidSrcNum>1)
			{
				string strOldVidSrcName,strNewVidSrcName;
				int nVidsrcIndex = -1;
				string strOldName = cInfo.GetDeviceName()+"_";
				//先查询该设备所有视频源别名，之后注意判断是否需要同步修改
				tSql.Format("select %s,%s from %s where %s='%s' and %s=%d",
					F_DEVICECAPINDEXDATA_DEVICECAPINDEX,F_DEVICECAPINDEXDATA_DEVICECAPINDEXNAME,T_DEVICECAPINDEXDATA,
					F_DEVICECAPINDEXDATA_DEVICEID,DbStr(cInfo.GetDeviceUUID()),
					F_DEVICECAPINDEXDATA_DEVICECAPID, DEVICE_CAP_VIDEOSOURCE);
				if (ExecSql(tSql, enExecMayRet))
				{
					while(IsNotEof())
					{
						strOldVidSrcName = string(GetString(F_DEVICECAPINDEXDATA_DEVICECAPINDEXNAME));
						nVidsrcIndex = GetInt(F_DEVICECAPINDEXDATA_DEVICECAPINDEX);
						
						vidsrcIndex.push_back(nVidsrcIndex);
						vidsrcName.push_back(strOldVidSrcName);

						MoveNext();
					}
					for (int i = 0; i< vidsrcIndex.size(); ++i)
					{
						strOldVidSrcName = strOldName+digital_string(vidsrcIndex[i]+1);
						if (strOldVidSrcName == vidsrcName[i])
						{
							strNewVidSrcName = strNewDevName+"_"+digital_string(vidsrcIndex[i]+1);
							vidsrcName[i]=strNewVidSrcName;
							
							ModDeviceVidSrcName(cInfo.GetDeviceUUID(), vidsrcIndex[i], strNewVidSrcName);

							tIndexName.index = vidsrcIndex[i];
							tIndexName.name = strNewVidSrcName;
							tCapNameNtf.deviceCapIndexNames.push_back(tIndexName);
						}
					}
					
					DevicePinyinSearchVideoNameSet(cInfo.GetDeviceUUID(), vidsrcIndex, vidsrcName);
				}
			}
			if (!tCapNameNtf.deviceCapIndexNames.empty())
			{
				CTaskMsgTransfer<CDeviceCapIndexNameModNtfReq> cTransNameNtf;
				CDeviceCapIndexNameModNtfReq& cNtfReq=cTransNameNtf.GetMsgObj();
				cNtfReq.SetHeadFromReq(cReq);
				cNtfReq.SetDomainId(cInfo.GetDomainUUID());
				cNtfReq.SetDeviceCapName(tCapNameNtf);

				RedisVidSrcAliasOrGpsSet(tCapNameNtf);

				PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cNtfReq, cTransNameNtf);

				for (u32 i = 0; i < tCapNameNtf.deviceCapIndexNames.size(); ++i)
				{
					SyncAnalyse(enSyncDataDeviceCapIndexName, enSyncOperateMod, CurrentDomainId(), 
						&tCapNameNtf.deviceId, tCapNameNtf.capId, tCapNameNtf.deviceCapIndexNames[i].index,
						cInfo.GetDeviceType());
				}
			}
		}

        if (bNeedRollbackLicense)
        {
            if (bUseDiffAlloc)
            {
                if (cNewVidCap.license < cOldVidCap.license)
                {
                    DeallocPULicense(cNewModel.GetManufacturer(), cOldVidCap.license-cNewVidCap.license);
                }
                if (cNewVidCap.num < cOldVidCap.num)
                {
                    DeallocManagePULicense(cOldVidCap.num-cNewVidCap.num);
                }
            }
            else
            {
            DeallocLicense(cOldModel);
            }
            bNeedRollbackLicense = false;
        }
        bHasNtf = (CMS_SUCCESS == DeviceItemQry(cReq.GetDeviceUUID(), cNtf.GetDeviceInfo()));
        
        SyncAnalyse(enSyncDataDeviceInfo, enSyncOperateMod, cInfo.GetDomainUUID(), &cInfo.GetDeviceUUID(), cInfo.GetDeviceType(), 
            cPropertyFlags.HasPropertyFlag(TDevicePropertyFlags::em_PropertyFlags_PrivateDev),
            cInfo.GetDevicePropertyFlags().HasPropertyFlag(TDevicePropertyFlags::em_PropertyFlags_PrivateDev));
    } while (false);

    if (bNeedRollbackLicense)
    {
        if (bUseDiffAlloc)
        {
            if (cNewVidCap.license > cOldVidCap.license)
            {
                DeallocPULicense(cNewModel.GetManufacturer(), cNewVidCap.license-cOldVidCap.license);
            }
            if (cNewVidCap.num > cOldVidCap.num)
            {
                DeallocManagePULicense(cNewVidCap.num-cOldVidCap.num);
            }
        }
        else
        {
        DeallocLicense(cNewModel);
        }
    }

    PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
    if (bHasNtf) 
    {
		RedisDeviceMod(cNtf.GetDeviceInfo(), setChangeFields);
        PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cNtf, cTransNtf);
    }
}

ENTasErrno CDbOpr::DeviceModelPreMod( const UUIDString &tDevId, const CDeviceModelInfo &cOldModel, const CDeviceModelInfo &cNewModel )
{
    // 原设备型号和新设备信号的设备类型必须相同
    if (cOldModel.GetDeviceTypeId() != cNewModel.GetDeviceTypeId())
    {
        return ERR_TAS_INVALID_OPERATION;
    }

    ENTasErrno enErrno = CMS_SUCCESS;
    if (DEVICE_TYPE_ENCODER == cNewModel.GetDeviceTypeId())
    {
        do 
        {
            // 更新设备能力索引名称信息
            enErrno = DeviceCapIndexNameUpdate(tDevId, cOldModel, cNewModel);
            if (CMS_SUCCESS != enErrno) break;

            // 视频源的拼音搜索
            int NewVidNum=0,OldVidNum=0;
            for (vector<TDeviceCapInfo>::const_iterator it= cNewModel.GetDeviceCapInfo().begin();
                it!=cNewModel.GetDeviceCapInfo().end();++it)
            {
                if(it->type==DEVICE_CAP_VIDEOSOURCE){
                    NewVidNum=it->num;
                    break;
                }
            }
            for (vector<TDeviceCapInfo>::const_iterator it= cOldModel.GetDeviceCapInfo().begin();
                it!=cOldModel.GetDeviceCapInfo().end();++it)
            {
                if(it->type==DEVICE_CAP_VIDEOSOURCE){
                    OldVidNum=it->num;
                    break;
                }
            }
            if (OldVidNum > NewVidNum)
            {
                DevicePinyinSearchVideoNameDel(tDevId, NewVidNum);
            }
            else if (OldVidNum < NewVidNum)
            {
                DevicePinyinSearchVideoNameInit(tDevId, NewVidNum, OldVidNum);
            }

            // 更新设备分组信息
            enErrno = DevGrpDevUpdate(tDevId, cOldModel, cNewModel);
            if (CMS_SUCCESS != enErrno) break;

            // 更新CU用户设备划归信息
            enErrno = CuserDevUpdate(tDevId, cOldModel, cNewModel);
            if (CMS_SUCCESS != enErrno) break;

            // 更新用户组设备划归信息
            enErrno = CuserGrpDevUpdate(tDevId, cOldModel, cNewModel);
            if (CMS_SUCCESS != enErrno) break;

			// 更新设备PTZ锁信息 pangubing
			enErrno = PtzLockInfoUpdate(tDevId, cOldModel, cNewModel);
			if (CMS_SUCCESS != enErrno) break;

        } while (false);
    }
    
    return enErrno;
}

ENTasErrno CDbOpr::Tv2DecoderUpdate(const CTaskMsgTransferBase *pcSrcTransfer, const UUIDString &tDevId, const CDeviceModelInfo &cOldModel, const CDeviceModelInfo &cNewModel )
{
	if (DEVICE_TYPE_DECODER == cNewModel.GetDeviceTypeId())
	{
		const vector<TDeviceCapInfo>& vecOldDeviceCapInfo = cOldModel.GetDeviceCapInfo();
		const vector<TDeviceCapInfo>& vecNewDeviceCapInfo = cNewModel.GetDeviceCapInfo();

		u32 i = 0;      // 原设备型号解码输出通道设备能力信息所在的下标
		u32 j = 0;      // 新设备型号解码输出通道设备能力信息所在的下标
		for (; i < vecOldDeviceCapInfo.size(); ++i)
		{
			if (DEVICE_CAP_DECODE_OUTPUTCHANNEL == vecOldDeviceCapInfo[i].type)
			{
				break;
			}
		}

		for (; j < vecNewDeviceCapInfo.size(); ++j)
		{
			if (DEVICE_CAP_DECODE_OUTPUTCHANNEL == vecNewDeviceCapInfo[j].type)
			{
				break;
			}
		}

		TSqlString tSql;
		if (vecOldDeviceCapInfo[i].num > vecNewDeviceCapInfo[j].num)    // 原设备型号有更多的解码输出通道，删除多余的解码输出通道
		{
			tSql.Format("select DISTINCT(%s) from %s where %s='%s'and %s>=%d", 
				F_TV2DECODER_TVWALLID,T_TV2DECODER, F_TV2DECODER_DECODERID, DbStr(tDevId),
				F_TV2DECODER_DECODEROUTPUTID, vecNewDeviceCapInfo[i].num);

			if (ExecSql(tSql, enExecMayRet))
			{
				vector<string> vecTvWallId;  
				string strTvWallId;

				while (IsNotEof())
				{
					strTvWallId = GetString(F_TV2DECODER_TVWALLID);
					vecTvWallId.push_back(strTvWallId);

					if (!MoveNext())
					{
						return ERR_TAS_DBERROR;
					}
				}

				tSql.Format("delete from %s where %s='%s'and %s>=%d", 
					T_TV2DECODER, F_TV2DECODER_DECODERID, DbStr(tDevId),
					F_TV2DECODER_DECODEROUTPUTID, vecNewDeviceCapInfo[i].num);
				if (!ExecSql(tSql, enExecNoRet))
				{
					return ERR_TAS_DBERROR;
				}

				for (size_t i=0; i<vecTvWallId.size();i++)
				{
					CTvwallInfo cInfo;
					if (CMS_SUCCESS==TvwallItemQry(vecTvWallId[i], cInfo))
					{
						CTvwallModReq cTvwallModReq;
						cTvwallModReq.SetId(vecTvWallId[i]);
						cTvwallModReq.SetTvDecoderBinders(cInfo.GetTvDecoderBinders());

						CTaskMsgTransfer<CTvwallModReq> cTransTvwallModReq;
						cTransTvwallModReq.SetMsgObj(cTvwallModReq);
						cTransTvwallModReq.SetTransfer(*pcSrcTransfer,cTransTvwallModReq.GetEventId());
						TvwallMod(&cTransTvwallModReq,true);
					}
				}

			}

		}
	}

	return CMS_SUCCESS;
}


ENTasErrno CDbOpr::DeviceCapIndexNameUpdate( const UUIDString &tDevId, const CDeviceModelInfo &cOldModel, const CDeviceModelInfo &cNewModel )
{
    if (DEVICE_TYPE_ENCODER == cNewModel.GetDeviceTypeId())
    {
        const vector<TDeviceCapInfo>& vecOldDeviceCapInfo = cOldModel.GetDeviceCapInfo();
        const vector<TDeviceCapInfo>& vecNewDeviceCapInfo = cNewModel.GetDeviceCapInfo();

        u32 i = 0;      // 原设备型号视频源设备能力信息所在的下标
        u32 j = 0;      // 新设备型号视频源设备能力信息所在的下标
		bool bExistVidCapOld=false,bExistVidCapNew=false;
        for (; i < vecOldDeviceCapInfo.size(); ++i)
        {
            if (DEVICE_CAP_VIDEOSOURCE == vecOldDeviceCapInfo[i].type)
            {
				bExistVidCapOld = true;
                break;
            }
        }

        for (; j < vecNewDeviceCapInfo.size(); ++j)
        {
            if (DEVICE_CAP_VIDEOSOURCE == vecNewDeviceCapInfo[j].type)
            {
				bExistVidCapNew = true;
                break;
            }
        }
		if (!bExistVidCapOld || !bExistVidCapNew)
		{
			return ERR_TAS_INVALID_PARM;
		}

        TSqlString tSql;
        if (vecOldDeviceCapInfo[i].num > vecNewDeviceCapInfo[j].num)    // 原设备型号有更多的视频源，删除多余的别名信息
        {
            tSql.Format("delete from %s where %s='%s' and %s=%d and %s>=%d", 
                T_DEVICECAPINDEXDATA, F_DEVICECAPINDEXDATA_DEVICEID, DbStr(tDevId), 
                F_DEVICECAPINDEXDATA_DEVICECAPID, DEVICE_CAP_VIDEOSOURCE,
                F_DEVICECAPINDEXDATA_DEVICECAPINDEX, vecNewDeviceCapInfo[i].num);
            if (!ExecSql(tSql, enExecNoRet))
            {
                return ERR_TAS_DBERROR;
            }
        }
    }

    return CMS_SUCCESS;
}

ENTasErrno CDbOpr::DevGrpDevUpdate( const UUIDString &tDevId, const CDeviceModelInfo &cOldModel, const CDeviceModelInfo &cNewModel )
{
    // 只有编码器的设备划归信息需要修改
    if (DEVICE_TYPE_ENCODER == cNewModel.GetDeviceTypeId())
    {
        const vector<TDeviceCapInfo>& vecOldDeviceCapInfo = cOldModel.GetDeviceCapInfo();
        const vector<TDeviceCapInfo>& vecNewDeviceCapInfo = cNewModel.GetDeviceCapInfo();

        u32 i = 0;      // 原设备型号视频源设备能力信息所在的下标
        u32 j = 0;      // 新设备型号视频源设备能力信息所在的下标
		//whui，这里有个坑，害我查了好久，如果数据异常，没有DEVICE_CAP_VIDEOSOURCE就会出异常
		bool bExistVidCapOld=false,bExistVidCapNew=false;
        for (; i < vecOldDeviceCapInfo.size(); ++i)
        {
            if (DEVICE_CAP_VIDEOSOURCE == vecOldDeviceCapInfo[i].type)
            {
				bExistVidCapOld = true;
                break;
            }
        }

        for (; j < vecNewDeviceCapInfo.size(); ++j)
        {
            if (DEVICE_CAP_VIDEOSOURCE == vecNewDeviceCapInfo[j].type)
            {
				bExistVidCapNew = true;
                break;
            }
        }
		if (!bExistVidCapOld || !bExistVidCapNew)
		{
			return ERR_TAS_INVALID_PARM;
		}
		UUIDString devTreeId;
		double dDevSeq;
        TSqlString tSql;
        if (vecOldDeviceCapInfo[i].num < vecNewDeviceCapInfo[j].num)        // 新设备型号有更多的视频源，为新增的视频源添加默认设备分组信息
        {
            for (s32 k = vecOldDeviceCapInfo[i].num; k < vecNewDeviceCapInfo[j].num; ++k)
            {
				if (!GetDevGrpNextDevSeq(devTreeId, CurrentDomainId(),tDevId, k,dDevSeq))
				{
					return ERR_TAS_DBERROR;
				}
                tSql.Format("insert into %s values('%s', '%s', %d, %d, %.14lf)", 
                    T_DEVGRP2DEV, DbStr(CurrentDomainId()), 
                    DbStr(tDevId), vecNewDeviceCapInfo[j].type, k, dDevSeq);
                if (!ExecSql(tSql, enExecNoRet))
                {
                    return ERR_TAS_DBERROR;
                }
            }
        }
        else if (vecOldDeviceCapInfo[i].num > vecNewDeviceCapInfo[j].num)   // 旧设备型号有更多的视频源，删除多余视频源的设备分组信息
        {
            tSql.Format("delete from %s where %s='%s' and %s=%d and %s>=%d", 
                T_DEVGRP2DEV, F_DEVGRP2DEV_DEVID, DbStr(tDevId), 
                F_DEVGRP2DEV_DEVCAPID, DEVICE_CAP_VIDEOSOURCE,
                F_DEVGRP2DEV_DEVCAPINDEX, vecNewDeviceCapInfo[j].num);
            if (!ExecSql(tSql, enExecNoRet))
            {
                return ERR_TAS_DBERROR;
            }
			

			// 删除自定义设备树信息 [3/23/2016 pangubing]
			vector<CDevTreeInfo> cDevTreeList;
			DevTreeListQry(cDevTreeList);
			for (size_t i=0;i<cDevTreeList.size();i++)
			{
				tSql.Format("delete from tblDevGrp2Dev_%s where %s='%s' and %s=%d and %s>=%d", DbStr(cDevTreeList[i].GetDevtreeId()), 
					F_DEVGRP2DEV_DEVID, DbStr(tDevId),
					F_DEVGRP2DEV_DEVCAPID, DEVICE_CAP_VIDEOSOURCE,
					F_DEVGRP2DEV_DEVCAPINDEX, vecNewDeviceCapInfo[j].num);

				if (!ExecSql(tSql, enExecNoRet))
				{
					return ERR_TAS_DBERROR;
				}

				UpdateDevTreeVer("",cDevTreeList[i].GetDevtreeId());
			}
        }
    }

    return CMS_SUCCESS;
}

ENTasErrno CDbOpr::CuserDevUpdate( const UUIDString &tDevId, const CDeviceModelInfo &cOldModel, const CDeviceModelInfo &cNewModel )
{
    ENTasErrno enErrno = CMS_SUCCESS;

    if (DEVICE_TYPE_ENCODER == cNewModel.GetDeviceTypeId())         // 编码器
    {
        do 
        {
            map<UUIDString, CUserDeviceInfo> mapOldUserDeviceInfo;  // 保存原来的CU用户设备划归信息

            // 获取CU用户设备划归信息
            enErrno = CuserDevGet(tDevId, cNewModel, mapOldUserDeviceInfo);
            if (CMS_SUCCESS != enErrno) return enErrno;

            // 删除原来的CU用户设备划归信息
            enErrno = CuserDevDelByDevId(tDevId);
            if (CMS_SUCCESS != enErrno) break;

            // 重建新的CU用户设备划归信息
            enErrno = CuserDevRestore(cNewModel, mapOldUserDeviceInfo);
            if (CMS_SUCCESS != enErrno) break;
        }while (false);
    }

    return enErrno;
}

ENTasErrno CDbOpr::CuserDevGet( const UUIDString &tDevId, const CDeviceModelInfo &cNewModel, map<UUIDString, CUserDeviceInfo> &mapInfo )
{
    s32 nNum = 0;
    const vector<TDeviceCapInfo> &vecDeviceCapInfo = cNewModel.GetDeviceCapInfo();
    for (u32 i = 0; i < vecDeviceCapInfo.size(); ++i)
    {
        if (DEVICE_CAP_VIDEOSOURCE == vecDeviceCapInfo[i].type)
        {
            nNum = vecDeviceCapInfo[i].num;
        }
    }

    if (0 >= nNum)      // 新设备型号无视频源能力
    {
        return CMS_SUCCESS;
    }

    // 查询出所有需要保留的非admin用户设备划归信息
    TSqlString tSql;
    tSql.Format("select a.*, b.%s from %s a, %s b where a.%s='%s' and a.%s=%d and a.%s<%d and a.%s<>'%s' and b.%s='%s' and b.%s<>'%s' and a.%s=b.%s", 
        F_CUSER2DEV_DEVPRIVILEGE, T_CUSER2DEVICECAP, T_CUSER2DEV, 
        F_CUSER2DEVICECAP_DEVID, DbStr(tDevId),
        F_CUSER2DEVICECAP_DEVCAPID, DEVICE_CAP_VIDEOSOURCE, 
        F_CUSER2DEVICECAP_DEVCAPINDEX, nNum, 
        F_CUSER2DEVICECAP_CUSERID, DbStr(CurrentDomainId()), 
        F_CUSER2DEV_DEVID, DbStr(tDevId), 
        F_CUSER2DEV_CUSERID, DbStr(CurrentDomainId()), 
        F_CUSER2DEVICECAP_CUSERID, F_CUSER2DEV_CUSERID);

    if (ExecSql(tSql, enExecMayRet))
    {
        string strCuserId;                  // CU用户Id
        CDevicePrivilege cDevicePrivilege;  // 参数配置权限

        while (IsNotEof())
        {
            strCuserId = GetString(F_CUSER2DEVICECAP_CUSERID);
            map<UUIDString, CUserDeviceInfo>::iterator it = mapInfo.find(strCuserId);
            if (mapInfo.end() == it)        // 第一次出现该用户的划归数据
            {
                CUserDeviceInfo cInfo;
                cInfo.SetUserID(strCuserId);
                cInfo.SetDeviceID(tDevId);

                cDevicePrivilege.ConvertFromInt(GetInt(F_CUSER2DEV_DEVPRIVILEGE));
                cInfo.SetDevicePrivilege(cDevicePrivilege);

                pair<map<UUIDString, CUserDeviceInfo>::iterator, bool> ret
                    = mapInfo.insert(pair<UUIDString, CUserDeviceInfo>(strCuserId, cInfo));
                it = ret.first;
            }

            vector<TDeviceCapPrivilege> &vecDeviceCapPrivilege = it->second.GetDeviceCapPrivilege();

            s32 nDevCapIndex = GetInt(F_CUSER2DEVICECAP_DEVCAPINDEX);
            CBitsContainer cDevCapIndexPrivilege;
            cDevCapIndexPrivilege.ConvertFromInt(GetInt(F_CUSER2DEVICECAP_DEVCAPINDEXPRIVILEGE));

            TDeviceCapPrivilege tDeviceCapPrivilege;
            tDeviceCapPrivilege.id = DEVICE_CAP_VIDEOSOURCE;
            tDeviceCapPrivilege.privileges.insert(pair<int, CBitsContainer>(nDevCapIndex, cDevCapIndexPrivilege));

            vecDeviceCapPrivilege.push_back(tDeviceCapPrivilege);

            if (!MoveNext())
            {
                return ERR_TAS_DBERROR;
            }
        }

        // 加入admin用户的划归信息
        string strAdminCuserId = CurrentDomainId();
        CUserDeviceInfo cAdminInfo;
        cAdminInfo.SetUserID(strAdminCuserId);
        cAdminInfo.SetDeviceID(tDevId);
        cDevicePrivilege.ConvertFromInt(-1);
        cAdminInfo.SetDevicePrivilege(cDevicePrivilege);

        vector<TDeviceCapPrivilege> &vecAdminDeviceCapPrivilege = cAdminInfo.GetDeviceCapPrivilege();
        TDeviceCapPrivilege tAdminDeviceCapPrivilege;
        tAdminDeviceCapPrivilege.id = DEVICE_CAP_VIDEOSOURCE;
        CBitsContainer cPrivilege;
        cPrivilege.ConvertFromInt(-1);
        for (u32 i = 0; i < nNum; ++i)
        {
            tAdminDeviceCapPrivilege.privileges.insert(pair<int, CBitsContainer>((s32)i, cPrivilege));
        }

        vecAdminDeviceCapPrivilege.push_back(tAdminDeviceCapPrivilege);
        mapInfo.insert(pair<UUIDString, CUserDeviceInfo>(strAdminCuserId, cAdminInfo));

        return CMS_SUCCESS;
    }
    else
    {
        return ERR_TAS_DBERROR;
    }
}

ENTasErrno CDbOpr::CuserDevRestore( const CDeviceModelInfo &cNewModel, const map<UUIDString, CUserDeviceInfo> &mapUserDeviceInfo )
{
    CUserDeviceInfo cInfo;

    map<UUIDString, CUserDeviceInfo>::const_iterator it = mapUserDeviceInfo.begin();
    for (; mapUserDeviceInfo.end() != it; ++it)
    {
        cInfo = it->second;
        GenerateEncodeCapPrivilegeInfo(cNewModel, it->second.GetDeviceCapPrivilege(), cInfo.GetDeviceCapPrivilege());

        if (CMS_SUCCESS != CuserDevItemAdd(cInfo))
        {
            return ERR_TAS_DBERROR;
        }
    }

    return CMS_SUCCESS;
}

ENTasErrno CDbOpr::CuserGrpDevUpdate( const UUIDString &tDevId, const CDeviceModelInfo &cOldModel, const CDeviceModelInfo &cNewModel )
{
    ENTasErrno enErrno = CMS_SUCCESS;

    if (DEVICE_TYPE_ENCODER == cNewModel.GetDeviceTypeId())     // 编码器
    {
        do 
        {
            map<UUIDString, CUserGroupDeviceInfo> mapOldUserGroupDeviceInfo;

            // 获取用户组设备划归信息
            enErrno = CuserGrpDevGet(tDevId, cNewModel, mapOldUserGroupDeviceInfo);
            if (CMS_SUCCESS != enErrno) break;

            if (!mapOldUserGroupDeviceInfo.empty())             // 该设备存在设备组划归信息
            {
                // 删除原来的用户组设备划归信息
                enErrno = CuserGrpDevDel(tDevId);
                if (CMS_SUCCESS != enErrno) break;

                // 重建新的用户组设备划归信息
                enErrno = CuserGrpDevRestore(cNewModel, mapOldUserGroupDeviceInfo);
                if (CMS_SUCCESS != enErrno) break;
            }
        } while (false);
    }

    return enErrno;
}

ENTasErrno CDbOpr::CuserGrpDevGet( const UUIDString &tDevId, const CDeviceModelInfo &cNewModel, map<UUIDString, CUserGroupDeviceInfo> &mapInfo )
{
    s32 nNum = 0;
    const vector<TDeviceCapInfo> &vecDeviceCapInfo = cNewModel.GetDeviceCapInfo();
    for (u32 i = 0; i < vecDeviceCapInfo.size(); ++i)
    {
        if (DEVICE_CAP_VIDEOSOURCE == vecDeviceCapInfo[i].type)
        {
            nNum = vecDeviceCapInfo[i].num;
        }
    }

    if (0 >= nNum)      // 新设备型号无视频源能力
    {
        return CMS_SUCCESS;
    }

    TSqlString tSql;
    tSql.Format("select a.*, b.%s from %s a, %s b where a.%s='%s' and a.%s=%d and a.%s<%d and b.%s='%s' and a.%s=b.%s", 
        F_CUSERGRP2DEV_DEVPRIVILEGE, T_CUSERGRP2DEVICECAP, T_CUSERGRP2DEV, 
        F_CUSERGRP2DEVICECAP_DEVID, DbStr(tDevId),
        F_CUSERGRP2DEVICECAP_DEVCAPID, DEVICE_CAP_VIDEOSOURCE, 
        F_CUSERGRP2DEVICECAP_DEVCAPINDEX, nNum, 
        F_CUSERGRP2DEV_DEVID, DbStr(tDevId), 
        F_CUSERGRP2DEVICECAP_CUSERGROUPID, F_CUSERGRP2DEV_CUSERGROUPID);

    if (ExecSql(tSql, enExecMayRet))
    {
        string strCuserGrpId;                  // CU用户组Id
        CDevicePrivilege cDevicePrivilege;     // 参数配置权限

        while (IsNotEof())
        {
            strCuserGrpId = GetString(F_CUSERGRP2DEVICECAP_CUSERGROUPID);
            map<UUIDString, CUserGroupDeviceInfo>::iterator it = mapInfo.find(strCuserGrpId);
            if (mapInfo.end() == it)        // 第一次出现该组的划归数据
            {
                CUserGroupDeviceInfo cInfo;
                cInfo.SetGroupID(strCuserGrpId);
                cInfo.SetDeviceID(tDevId);

                cDevicePrivilege.ConvertFromInt(GetInt(F_CUSERGRP2DEV_DEVPRIVILEGE));
                cInfo.SetDevicePrivilege(cDevicePrivilege);

                pair<map<UUIDString, CUserGroupDeviceInfo>::iterator, bool> ret
                    = mapInfo.insert(pair<UUIDString, CUserGroupDeviceInfo>(strCuserGrpId, cInfo));
                it = ret.first;
            }

            vector<TDeviceCapPrivilege> &vecDeviceCapPrivilege = it->second.GetDeviceCapPrivilege();

            s32 nDevCapIndex = GetInt(F_CUSERGRP2DEVICECAP_DEVCAPINDEX);
            CBitsContainer cDevCapIndexPrivilege;
            cDevCapIndexPrivilege.ConvertFromInt(GetInt(F_CUSERGRP2DEVICECAP_DEVCAPINDEXPRIVILEGE));

            TDeviceCapPrivilege tDeviceCapPrivilege;
            tDeviceCapPrivilege.id = DEVICE_CAP_VIDEOSOURCE;
            tDeviceCapPrivilege.privileges.insert(pair<int, CBitsContainer>(nDevCapIndex, cDevCapIndexPrivilege));

            vecDeviceCapPrivilege.push_back(tDeviceCapPrivilege);

            if (!MoveNext())
            {
                return ERR_TAS_DBERROR;
            }
        }

        return CMS_SUCCESS;
    }
    else
    {
        return ERR_TAS_DBERROR;
    }
}

ENTasErrno CDbOpr::CuserGrpDevRestore( const CDeviceModelInfo &cNewModel, const map<UUIDString, CUserGroupDeviceInfo> &mapUserGroupDeviceInfo )
{
    CUserGroupDeviceInfo cInfo;

    map<UUIDString, CUserGroupDeviceInfo>::const_iterator it = mapUserGroupDeviceInfo.begin();
    for (; mapUserGroupDeviceInfo.end() != it; ++it)
    {
        cInfo = it->second;
        GenerateEncodeCapPrivilegeInfo(cNewModel, it->second.GetDeviceCapPrivilege(), cInfo.GetDeviceCapPrivilege());

        if (CMS_SUCCESS != CuserGrpDevItemAdd(cInfo))
        {
            return ERR_TAS_DBERROR;
        }
    }

    return CMS_SUCCESS;
}

ENTasErrno CDbOpr::CuserGrpDevItemAdd( const CUserGroupDeviceInfo &cInfo )
{
		// 增加校验视频源权限 [12/11/2015 pangubing]
	const vector<TDeviceCapPrivilege> &aCapPrivi = cInfo.GetDeviceCapPrivilege();
	if (IsDevCapPrivilegeVailed(aCapPrivi)!=CMS_SUCCESS)
	{
		return ERR_TAS_INVALID_PARM;
	}

    TSqlString tSql;
    tSql.Format("call %s('%s', '%s', %d)", P_CUSERGRPDEVADD,
        DbStr(cInfo.GetGroupID()), DbStr(cInfo.GetDeviceID()), (s32)cInfo.GetDevicePrivilege().Convert2Int());
    if (!ExecSql(tSql, enExecNoRet))
    {
        return ERR_TAS_DBERROR;
    }

    u32 i;
    for (i = 0; i < aCapPrivi.size(); ++i)
    {
        const TDeviceCapPrivilege &tCap = aCapPrivi[i];

        map<int, CBitsContainer>::const_iterator it;
        for (it = tCap.privileges.begin(); it != tCap.privileges.end(); ++it)
        {
            tSql.Format("call %s('%s', '%s', %d, %d, %d)", 
                P_CUSERGRPDEVCAPADD, DbStr(cInfo.GetGroupID()), DbStr(cInfo.GetDeviceID()), tCap.id, 
                it->first, (s32)it->second.Convert2Int());

            if (!ExecSql(tSql, enExecNoRet)) break;
        }

        if (it != tCap.privileges.end())
            break;
    }
    if (i != aCapPrivi.size()) 
    {
        return ERR_TAS_DBERROR;
    }

    return CMS_SUCCESS;
}

ENTasErrno CDbOpr::CuserGrpDevItemDel( const CUserGroupDeviceInfoKey &cInfo )
{
	TSqlString tSql;
	bool bNeedDelDev = false;
	const vector<TDeviceCapIndexs> tCapIndexs = cInfo.GetDeviceCapIndexs();
	if (tCapIndexs.size() > 0)
	{
		for (u32 i = 0; i < tCapIndexs.size(); ++i)
		{
			for (u32 j = 0; j < tCapIndexs[i].indexs.size(); ++j)
			{
				tSql.Format("delete from %s where %s='%s' and %s='%s' and %s=%d and %s=%d", T_CUSERGRP2DEVICECAP, 
					F_CUSERGRP2DEVICECAP_CUSERGROUPID, DbStr(cInfo.GetGroupID()), F_CUSERGRP2DEVICECAP_DEVID, DbStr(cInfo.GetDeviceID()),
					F_CUSERGRP2DEVICECAP_DEVCAPID, tCapIndexs[i].id, F_CUSERGRP2DEVICECAP_DEVCAPINDEX, tCapIndexs[i].indexs[j]);
				if (!ExecSql(tSql, enExecNoRet))
				{
					return ERR_TAS_DBERROR;
				}
			}
		}

		// 判断是否所有的设备能力权限都删除了，是的话后面自动删除设备权限
		tSql.Format("select count(*) from %s where %s='%s' and %s='%s'", T_CUSERGRP2DEVICECAP,
			F_CUSERGRP2DEVICECAP_CUSERGROUPID, DbStr(cInfo.GetGroupID()), F_CUSERGRP2DEVICECAP_DEVID, DbStr(cInfo.GetDeviceID()));
		if (ExecSql(tSql, enExecMustRet))
		{
			if (GetIntResult() == 0) 
			{
				bNeedDelDev = true;
			}
		}
	}

	if (bNeedDelDev)
	{
		tSql.Format("delete from %s where %s='%s' and %s='%s'", T_CUSERGRP2DEV, 
			F_CUSERGRP2DEV_CUSERGROUPID, DbStr(cInfo.GetGroupID()), F_CUSERGRP2DEV_DEVID, DbStr(cInfo.GetDeviceID()));
		if (!ExecSql(tSql, enExecNoRet))
		{
			return ERR_TAS_DBERROR;
		}
	}

	return CMS_SUCCESS;
}

ENTasErrno CDbOpr::DeviceModelPreMod4Parent( const UUIDString &tDevId, const UUIDString &tDevDomainId, const UUIDString &tOldModelId, const UUIDString &tNewModelId )
{
    ENTasErrno enErrno;

    do 
    {
        enErrno = DeviceCapIndexNameDel(tDevId);
        if (CMS_SUCCESS != enErrno) break;

        enErrno = DevGrpDevDelByDevId(tDevId,false);
        if (CMS_SUCCESS != enErrno) break;
        enErrno = DevGrpDevAddDefault(tDevId, tNewModelId, &tDevDomainId);
        if (CMS_SUCCESS != enErrno) break;
        enErrno = CuserDevDelByDevId(tDevId);
        if (CMS_SUCCESS != enErrno) break;
        enErrno = CuserDevAddDefault(tDevId, tNewModelId);
        if (CMS_SUCCESS != enErrno) break;
        enErrno = CuserGrpDevDel(tDevId);
        if (CMS_SUCCESS != enErrno) break;
    } while (false);
    
    return enErrno;
}

void CDbOpr::DeviceQry( const CTaskMsgTransfer<CDeviceQryReq> *pcMsg )
{
    DatabaseQueryTemplate<CDeviceQryReq, CDeviceQryRsp, CDeviceQryNtfReq, CDeviceInfo>(
    pcMsg, &CDbOpr::PrepareDeviceQrySql, &CDbOpr::FetchDeviceQryRecord, 
    &CDeviceQryNtfReq::GetDeviceInfo, &CDeviceInfo::GetDeviceUUID);
}


void CDbOpr::UpdateDevTreeVer( const UUIDString &strDevId ,const UUIDString &devtreeId)
{
	CCMSTime cTm;
	TSqlString tSql;
	++s_dwDevTreeVerExt;
	if (!devtreeId.empty())
	{
		//更新tblDevTree表
		tSql.Format("update %s set %s='%s_%u' where %s='%s'", 
			T_DEVTREE, F_DEVTREE_VERSION, DbStr(cTm.ToString()),s_dwDevTreeVerExt, F_DEVTREE_ID,DbStr(devtreeId));
		if (!ExecSql(tSql, enExecNoRet))
		{
			CLASSLOG(enLogSrcDbTaskPool, ERROR_LEV, 
				"update tblDevTree's devTreeVer failed, devtreeId=%s\n", devtreeId.c_str());
		}

    }
	else
	{


		if (!strDevId.empty())
		{
			tSql.Format("update %s set %s='%s_%u' "
				"where exists(select 1 from %s a where a.%s=%s.%s and a.%s='%s')", 
				T_CUSER, F_CUSER_DEVTREEVER, DbStr(cTm.ToString()), s_dwDevTreeVerExt, 
				T_CUSER2DEV, F_CUSER2DEV_CUSERID, T_CUSER, F_CUSER_CUSERID, F_CUSER2DEV_DEVID, DbStr(strDevId));
		}
		else
		{
			tSql.Format("update %s set %s='%s_%u'", 
				T_CUSER, F_CUSER_DEVTREEVER, DbStr(cTm.ToString()), s_dwDevTreeVerExt);
		}

		if (!ExecSql(tSql, enExecNoRet))
		{
			CLASSLOG(enLogSrcDbTaskPool, ERROR_LEV, 
				"update cuser's devTreeVer failed, devId=%s\n", strDevId.c_str());
		}

		if (!strDevId.empty())
		{
			tSql.Format("update %s set %s='%s_%u' "
				"where exists(select 1 from %s a where a.%s=%s.%s and a.%s='%s')", 
				T_CUSERGROUP, F_CUSERGROUP_DEVTREEVER, DbStr(cTm.ToString()), s_dwDevTreeVerExt, 
				T_CUSERGRP2DEV, F_CUSERGRP2DEV_CUSERGROUPID, T_CUSERGROUP, F_CUSERGROUP_CUSERGROUPID, F_CUSERGRP2DEV_DEVID, DbStr(strDevId));
		}
		else
		{
			tSql.Format("update %s set %s='%s_%u'", 
				T_CUSERGROUP, F_CUSERGROUP_DEVTREEVER, DbStr(cTm.ToString()), s_dwDevTreeVerExt);
		}

		if (!ExecSql(tSql, enExecNoRet))
		{
			CLASSLOG(enLogSrcDbTaskPool, ERROR_LEV, 
				"update cusergroup's devTreeVer failed, devId=%s\n", strDevId.c_str());
		}
	}
	
}

void CDbOpr::UpdateTreesVerByDevId( const UUIDString &devId )
{
	++s_dwDevTreeVerExt;
	TSqlString tSql;

	if (!devId.empty())
	{
		tSql.Format("call %s('%s', '%d')", P_UPDATEALLTREEVERBYDEVID, DbStr(devId), s_dwDevTreeVerExt);
		if (!ExecSql(tSql, enExecNoRet))
		{
			CLASSLOG(enLogSrcDbTaskPool, ERROR_LEV, 
				" UpdateTreesVerByDevId failed, devId=%s\n", devId.c_str());
		}
	}
	else
	{
		CCMSTime cTm;

		tSql.Format("update %s set %s='%s_%u'", 
			T_CUSER, F_CUSER_DEVTREEVER, DbStr(cTm.ToString()), s_dwDevTreeVerExt);
		if (!ExecSql(tSql, enExecNoRet))
		{
			CLASSLOG(enLogSrcDbTaskPool, ERROR_LEV, 
				"UpdateTreesVerByDevId update cuser's devTreeVer failed\n");
		}

		tSql.Format("update %s set %s='%s_%u'", 
			T_CUSERGROUP, F_CUSERGROUP_DEVTREEVER, DbStr(cTm.ToString()), s_dwDevTreeVerExt);
		if (!ExecSql(tSql, enExecNoRet))
		{
			CLASSLOG(enLogSrcDbTaskPool, ERROR_LEV, 
				"UpdateTreesVerByDevId update cusergroup's devTreeVer failed\n");
		}

		tSql.Format("update %s set %s='%s_%u'", 
			T_DEVTREE, F_DEVTREE_VERSION, DbStr(cTm.ToString()),s_dwDevTreeVerExt);
		if (!ExecSql(tSql, enExecNoRet))
		{
			CLASSLOG(enLogSrcDbTaskPool, ERROR_LEV, 
				"UpdateTreesVerByDevId update tblDevTree's devTreeVer failed\n");
		}
	}
	
}

ENTasErrno CDbOpr::PrepareDeviceQrySql( const CDeviceQryReq &cReq, TSqlString &tSqlSelect, TSqlString &tSqlCount )
{
    // hzytodo2 指定CU用户时自动构造解码器设备返回
    const CDeviceInfoQueryCondition &cCond = cReq.GetDeviceInfoQueryCondition();

    TSqlString tSqlCond;
    TSqlString tSqlOrderby;
    bool bNoUserLimit;
    if (cCond.GetCuserID(m_strFieldValue))
    {
        string strUserId = m_strFieldValue;
        string strUserGrpId;
        if (!GetCuserGrpId(strUserId, strUserGrpId))
        {
            return ERR_TAS_USER_NOT_EXIST;
        }

        if (strUserGrpId.empty())
        {
            tSqlSelect.Format("select a.* from %s a inner join %s b on a.%s=b.%s where b.%s='%s'", 
                T_DEVICE, T_CUSER2DEV, F_DEVICE_DEVID, F_CUSER2DEV_DEVID, F_CUSER2DEV_CUSERID, 
                DbStr(strUserId));
            tSqlCount.Format("select count(*) from %s a inner join %s b on a.%s=b.%s where b.%s='%s'", 
                T_DEVICE, T_CUSER2DEV, F_DEVICE_DEVID, F_CUSER2DEV_DEVID, F_CUSER2DEV_CUSERID, 
                DbStr(strUserId));
        }
        else
        {
            tSqlSelect.Format("select a.* from %s a inner join %s b on a.%s=b.%s where b.%s='%s'", 
                T_DEVICE, T_CUSERGRP2DEV, F_DEVICE_DEVID, F_CUSERGRP2DEV_DEVID, F_CUSERGRP2DEV_CUSERGROUPID,
                DbStr(strUserGrpId));
            tSqlCount.Format("select count(*) from %s a inner join %s b on a.%s=b.%s where b.%s='%s'", 
                T_DEVICE, T_CUSERGRP2DEV, F_DEVICE_DEVID, F_CUSERGRP2DEV_DEVID, F_CUSERGRP2DEV_CUSERGROUPID,
                DbStr(strUserGrpId));
        }

        bNoUserLimit = false;
    }
    else
    {
        tSqlSelect.Format("select a.* from %s a", T_DEVICE);
        tSqlCount.Format("select count(*) from %s a", T_DEVICE);

        bNoUserLimit = true;
    }

    AppendQryCondValuePair(cCond.GetDomainId, m_strFieldValue, tSqlCond, F_DEVICE_DOMAINID, a);
    AppendQryCondValuePair(cCond.GetDeviceUUID, m_strFieldValue, tSqlCond, F_DEVICE_DEVID, a);
    if (cCond.GetDeviceUUIDBegin(m_strFieldValue))
    {
        tSqlCond.AppendString("a.%s>'%s' and ", F_DEVICE_DEVID, DbStr(m_strFieldValue));
    }
	
	//如果需要搜索ip字段，则（ip like y or name like y ） and X,否则只是name like y and X
	string needSerchIp;
    if (cCond.GetNeedSearchIp(needSerchIp) && needSerchIp == "true")
    {
		string devName;
		if (cCond.GetDeviceName(devName))
		{
			tSqlCond.AppendString(" ( %s like '%%%s%%' or %s like '%%%s%%' ) and ",
				F_DEVICE_DEVNAME, DbStr(devName), F_DEVICE_NETADDR, DbStr(devName));
		}
    }
	else
	{
		AppendQryCondLikeStringPair(cCond.GetDeviceName, m_strFieldValue, tSqlCond, F_DEVICE_DEVNAME, a);
	}
    
    AppendQryCondLikeStringPair(cCond.GetDeviceAlias, m_strFieldValue, tSqlCond, F_DEVICE_DEVALIAS, a);
	AppendQryCondValuePair(cCond.GetOwner, m_strFieldValue, tSqlCond, F_DEVICE_OWNER, a);
    
	set<TCreater> creaters;
	if (cCond.GetCreaters(creaters) && 0 < creaters.size())		// 指定了创建者集合
	{
		tSqlCond.AppendString("(");
		for (set<TCreater>::iterator it = creaters.begin(); it != creaters.end(); ++it)
		{
			if (it != creaters.begin()) 
				tSqlCond.AppendString(" or ");
			tSqlCond.AppendString("a.%s=%d", F_DEVICE_CREATER, *it);
		}
		tSqlCond.AppendString(") and ");
	}

    set<int> types;
    if (cCond.GetDeviceType(types))
    {
        if (types.size() == 0)
        {
            return ERR_TAS_INVALID_DEVICETYPEID;
        }

        tSqlCond.AppendString("(");
        for (set<int>::iterator it = types.begin(); it != types.end(); ++it)
        {
            if (it != types.begin()) tSqlCond.AppendString(" or ");
            tSqlCond.AppendString("a.%s=%d", F_DEVICE_DEVTYPE, *it);
        }
        tSqlCond.AppendString(") and ");
    }
    AppendQryCondValuePair(cCond.GetDeviceModelId, m_strFieldValue, tSqlCond, F_DEVICEMODEL_MODELID, a);
    AppendQryCondValuePair(cCond.GetUsedFlag, m_bFieldValue, tSqlCond, F_DEVICE_USEFLAG, a);
    AppendQryCondValuePair(cCond.GetNetType, m_nFieldValue, tSqlCond, F_DEVICE_NETTYPE, a);
    AppendQryCondValuePair(cCond.GetNetAddress, m_strFieldValue, tSqlCond, F_DEVICE_NETADDR, a);
    AppendQryCondValuePair(cCond.GetDescription, m_strFieldValue, tSqlCond, F_DEVICE_DEVDESC, a);
    AppendQryCondDatePeriodPair(cCond.GetExpireDate_From, m_strFieldValue, cCond.GetExpireDate_To, m_strFieldValue2,
        tSqlCond, F_DEVICE_EXPIREDATE, a);
    AppendQryCondDatePeriodPair(cCond.GetCreateDate_From, m_strFieldValue, cCond.GetCreateDate_To, m_strFieldValue2,
        tSqlCond, F_DEVICE_CREATEDATE, a);
    tSqlCond.EraseLastRedundantAnd();

    if (!tSqlCond.Empty())
    {
        if (bNoUserLimit)
        {
            tSqlSelect.AppendString(" where ");
            tSqlCount.AppendString(" where ");
        }
        else
        {
            tSqlSelect.AppendString(" ");
            tSqlCount.AppendString(" ");
        }
        tSqlSelect.AppendString("%s", tSqlCond.GetBuf());
        tSqlCount.AppendString("%s", tSqlCond.GetBuf());
    }
    if (cReq.GetOrderField().empty())
    {
        tSqlSelect.AppendString(" order by convert(a.devName using gbk)");
    }
    else
    {
        tSqlSelect.AppendString(" order by");
        for (s32 i = 0; i < cReq.GetOrderField().size(); ++i)
        {
            if (i > 0)
            {
                tSqlSelect.AppendString(",");
            }
            if (cReq.GetOrderField()[i].columnId != "devName")
            {
                tSqlSelect.AppendString(" a.%s %s", 
                    cReq.GetOrderField()[i].columnId.c_str(), cReq.GetOrderField()[i].asc?"asc":"desc");
            }
            else
            {
                tSqlSelect.AppendString(" convert(a.%s using gbk) %s", 
                    cReq.GetOrderField()[i].columnId.c_str(), cReq.GetOrderField()[i].asc?"asc":"desc");
            }
        }
    }
    AddRowNumLimitModifer(tSqlSelect, cReq.GetStartEntryIndex(), cReq.GetExpectEntryNum());
    return CMS_SUCCESS;
}

ENTasErrno CDbOpr::FetchDeviceQryRecord( CDeviceInfo &device )
{
    CDeviceModelInfo modelInfo = g_cDbCache.GetDeviceModelInfo(GetString(F_DEVICE_DEVMODELID), NULL);
    if (modelInfo.GetDeviceModelId().empty())
    {
        return ERR_TAS_RECORD_NOT_EXISTS;
    }

    device.SetDeviceUUID(GetString(F_DEVICE_DEVID));
    device.SetDomainUUID(GetString(F_DEVICE_DOMAINID));
    device.SetDeviceName(GetString(F_DEVICE_DEVNAME));
    device.SetDeviceAlias(GetString(F_DEVICE_DEVALIAS));
    device.SetDeviceType(GetInt(F_DEVICE_DEVTYPE));
    device.SetDeviceModel(GetString(F_DEVICE_DEVMODELID));
    TDevicePropertyFlags cProperFlags;
    cProperFlags.ConvertFromInt(GetInt(F_DEVICE_DEVPROPERTYFLAGS));
    device.SetPropertyFlags(cProperFlags);
    device.SetManufacturer(modelInfo.GetManufacturer());
    device.SetModelName(modelInfo.GetDeviceModelName());
    device.GetDeviceModelCapDetail() = modelInfo.GetDeviceCapInfo();
    device.SetUsedFlag(GetBool(F_DEVICE_USEFLAG));
    device.SetNetType(GetInt(F_DEVICE_NETTYPE));
    device.SetNetAddress(GetString(F_DEVICE_NETADDR));
    device.SetDescription(GetString(F_DEVICE_DEVDESC));
    device.SetExpireDate(GetCmsDate(F_DEVICE_EXPIREDATE));
    device.SetCreateDate(GetCmsDate(F_DEVICE_CREATEDATE));
    device.SetCreater(GetInt(F_DEVICE_CREATER));
	device.SetOwner(GetString(F_DEVICE_OWNER));

	string strDevData = GetString(F_DEVICE_DEVDATA);
	if (!strDevData.empty())
	{
		TDevExtData &tDevExtData = device.GetDevData();
		tDevExtData.FromXml(strDevData);
	}

    return CMS_SUCCESS;
}

void CDbOpr::DeviceModelAdd( const CTaskMsgTransfer<CDeviceModelAddReq> *pcMsg )
{
	DeviceModelAdd(pcMsg,false);	
}

string CDbOpr::DeviceModelAdd( const CTaskMsgTransfer<CDeviceModelAddReq> *pcMsg,bool bUseInner )
{
	const CDeviceModelAddReq &cReq = pcMsg->GetMsgObj();
	CTaskMsgTransfer<CDeviceModelAddRsp> cTrans;
	CDeviceModelAddRsp &cRsp = cTrans.GetMsgObj();
	CTaskMsgTransfer<CDeviceModelAddNtfReq> cTransNtf;
	CDeviceModelAddNtfReq &cNtf = cTransNtf.GetMsgObj();
	bool bHasNtf = false;

	do 
	{
		const CDeviceModelInfo& cModelInfo = cReq.GetDeviceModelInfo();
		string strModelId;
		if (CREATE_BY_PLAT == cModelInfo.GetCreater())			// 平台上创建设备型号时，要求不能存在同名的设备型号
		{
			cRsp.SetErrorCode(GetExistDeviceModelId(CurrentDomainId(), cModelInfo.GetDeviceModelName(), 
				cModelInfo.GetCreater(), strModelId));
		}
		else
		{
			cRsp.SetErrorCode(GetExistDeviceModelId(CurrentDomainId(), cModelInfo.GetDeviceTypeId(), 
				cModelInfo.GetDeviceModelName(),false, cModelInfo.GetManufacturer(), cModelInfo.GetCreater(), 
				cModelInfo.GetDeviceCapInfo(), strModelId));
		}

		if (cRsp.GetErrorCode() != CMS_SUCCESS)			// 检测已存在的设备型号过程中出错
		{
			break;
		}

		if (!strModelId.empty())			// 已经存在相同信息的设备型号
		{
			cRsp.SetErrorCode(ERR_TAS_SAME_DEVICEMODEL_INFO);
			cRsp.SetDeviceModelId(strModelId);
		}
		else
		{
			CUUID cModelId;
			CDeviceModelInfo model = cModelInfo;
			model.SetDeviceModelId(cModelId);
			model.SetDomainId(CurrentDomainId());

			cRsp.SetErrorCode(DeviceModelItemAdd(model, true));
			if (cRsp.GetErrorCode() != CMS_SUCCESS)
			{
				break;
			}
			cRsp.SetDeviceModelId(cModelId);

			bHasNtf = (CMS_SUCCESS == DeviceModelItemQry(cModelId, cNtf.GetDeviceModelInfo()));

			SyncAnalyse(enSyncDataDeviceModel, enSyncOperateAdd, CurrentDomainId(), &cModelId, model.GetDeviceTypeId());
		}
	} while (false);

	if (!bUseInner)
	{
		PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
	}
	
	if (bHasNtf) PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cNtf, cTransNtf);

	if (cRsp.GetErrorCode()!= CMS_SUCCESS)
	{
		return string("");
	} 

	return cRsp.GetDeviceModelId();
}

ENTasErrno CDbOpr::GetExistDeviceModelId(const string &strDomainId, const string &strModelName, TCreater tCreater, string &strModelId)
{
	TSqlString tSql;
	tSql.Format("select * from %s where %s='%s' and %s='%s' and %s=%d limit 0,1", 
		T_DEVICEMODEL, 
		F_DEVICEMODEL_DOMAINID, DbStr(strDomainId), 
		F_DEVICEMODEL_MODELNAME, DbStr(strModelName), 
		F_DEVICEMODEL_CREATER, tCreater);
	if (ExecSql(tSql, enExecMayRet))
	{
		if (IsNotEof())
		{
			strModelId = GetString(F_DEVICEMODEL_MODELID);
		}

		return CMS_SUCCESS;
	}
	else
	{
		return ERR_TAS_DBERROR;
	}
}

ENTasErrno CDbOpr::GetExistDeviceModelId(const string &strDomainId, s32 nType, const string &strModelName,bool bIgnoreModelName, const string &strManufacturer, 
	TCreater tCreater, const vector<TDeviceCapInfo> &vecCapInfo, string &strModelId)
{
	TSqlString tSql;
	if (bIgnoreModelName)
	{
		tSql.Format("select * from %s where %s='%s' and %s=%d and %s='%s' and %s=%d order by %s", 
			T_DEVICEMODEL, F_DEVICEMODEL_DOMAINID, DbStr(strDomainId), 
			F_DEVICEMODEL_DEVICETYPEID, nType, 
			F_DEVICEMODEL_MANUFACTURER, DbStr(strManufacturer), 
			F_DEVICEMODEL_CREATER, tCreater, 
			F_DEVICEMODEL_MODELID);
	} 
	else
	{
		tSql.Format("select * from %s where %s='%s' and %s=%d and %s='%s' and %s='%s' and %s=%d order by %s", 
			T_DEVICEMODEL, F_DEVICEMODEL_DOMAINID, DbStr(strDomainId), 
			F_DEVICEMODEL_DEVICETYPEID, nType, 
			F_DEVICEMODEL_MODELNAME, DbStr(strModelName), 
			F_DEVICEMODEL_MANUFACTURER, DbStr(strManufacturer), 
			F_DEVICEMODEL_CREATER, tCreater, 
			F_DEVICEMODEL_MODELID);
	}

	if (ExecSql(tSql, enExecMayRet))
	{
		typedef vector<TDeviceCapInfo> TDeviceCapInfoList;
		map<string, TDeviceCapInfoList> mapModelId2CapInfoList;

		CDeviceModelInfo cCurInfo, cPreInfo;
		TDeviceCapInfoList tCapInfoList;
		while (IsNotEof())
		{
			FetchDeviceModelQryRecord(cCurInfo);		// 获取一条设备型号记录信息

			if (cPreInfo.GetDeviceModelId() != cCurInfo.GetDeviceModelId() && !tCapInfoList.empty())
			{
				mapModelId2CapInfoList.insert(pair<string, TDeviceCapInfoList>(cPreInfo.GetDeviceModelId(), tCapInfoList));
				tCapInfoList.clear();
			}
			
			//合并当前记录中的设备能力
			//由于FetchDeviceModelQryRecord每次都只会获取到一个设备能力，也可简单的写为：tCapInfoList.push_back(cCurInfo.GetDeviceCapInfo()[0]);
			const TDeviceCapInfoList &tTmpCapInfoList = cCurInfo.GetDeviceCapInfo();
			for (TDeviceCapInfoList::const_iterator it = tTmpCapInfoList.begin(); 
				tTmpCapInfoList.end() != it; ++it)
			{
				tCapInfoList.push_back(*it);
			}

			// 当前记录处理完毕，将当前记录设置为上一条记录
			cPreInfo = cCurInfo;

			if (!MoveNext())
			{
				return ERR_TAS_DBERROR;
			}
		}

		// 最后一个设备型号，至此所有的设备型号的设备能力和Id都设置到mapCapInfoList2ModelId中
		mapModelId2CapInfoList.insert(pair<string, TDeviceCapInfoList>(cPreInfo.GetDeviceModelId(), tCapInfoList));
		tCapInfoList.clear();

		//找出符合条件的设备型号id
		for (map<string, TDeviceCapInfoList>::const_iterator it = mapModelId2CapInfoList.begin(); 
			mapModelId2CapInfoList.end() != it; ++it)
		{
			if (CompareCapInfo(vecCapInfo, it->second))
			{
				strModelId = it->first;
				break;
			}
		}

		return CMS_SUCCESS;
	}
	else
	{
		return ERR_TAS_DBERROR;
	}
}

ENTasErrno CDbOpr::IsDeviceModelUsing( const string &modelId )
{
    TSqlString tSql;
    tSql.Format("select 1 from %s where %s='%s' limit 1", T_DEVICE, F_DEVICE_DEVMODELID, DbStr(modelId));
    if (!ExecSql(tSql, enExecMayRet))
    {
        return ERR_TAS_DBERROR;
    }

    return !m_cDbEngine.IsEof()? ERR_TAS_DEVICE_MODEL_USING:CMS_SUCCESS;
}

bool CDbOpr::CompareCapInfo(const vector<TDeviceCapInfo> &vecCapInfo1, 
	const vector<TDeviceCapInfo> &vecCapInfo2)
{
	// 检查vecCapInfo1中的每一项能力是否都能在vecCapInfo2中找到匹配的
	for (u32 i = 0; i < vecCapInfo1.size(); ++i)
	{
		// 本地存储不填写和值为false是同一个意思，故当值为false时直接跳过比较
		// 在对设备型号进行了整理后，决定将并口告警能力不作为匹配设备型号的依据
		if ((DEVICE_CAP_LOCAL_STORAGE == vecCapInfo1[i].type && 0 == vecCapInfo1[i].num))
		{
			continue;
		}

		bool bExist = false;
		for (u32 j = 0; j < vecCapInfo2.size(); ++j)
		{
			if (vecCapInfo1[i] == vecCapInfo2[j])
			{
				bExist =true;
				break;
			}
		}

		if (!bExist)
		{
			return false;
		}
	}

	// 检查vecCapInfo2中的每一项能力是否都能在vecCapInfo1中找到匹配的
	for (u32 i = 0; i < vecCapInfo2.size(); ++i)
	{
		// 本地存储不填写和值为false是同一个意思，故当值为false时直接跳过比较
		// 在对设备型号进行了整理后，决定将并口告警能力不作为匹配设备型号的依据
		if ((DEVICE_CAP_LOCAL_STORAGE == vecCapInfo2[i].type && 0 == vecCapInfo2[i].num))
		{
			continue;
		}

		bool bExist = false;
		for (u32 j = 0; j < vecCapInfo1.size(); ++j)
		{
			if (vecCapInfo2[i] == vecCapInfo1[j])
			{
				bExist =true;
				break;
			}
		}

		if (!bExist)
		{
			return false;
		}
	}

	return true;
}

void CDbOpr::DeviceModelDel( const CTaskMsgTransfer<CDeviceModelDelReq> *pcMsg )
{
    CTaskMsgTransfer<CDeviceModelDelRsp> cTrans;
    CTaskMsgTransfer<CDeviceModelDelNtfReq> cTransNtf;
    CDeviceModelDelRsp &cRsp = cTrans.GetMsgObj();
    CDeviceModelDelNtfReq &cNtf = cTransNtf.GetMsgObj();
    bool bHasNtf = false;

    const CDeviceModelDelReq &cReq = pcMsg->GetMsgObj();

    do 
    {
        cRsp.SetErrorCode(IsDeviceModelUsing(cReq.GetDeviceModelId()));
        if (cRsp.GetErrorCode() != CMS_SUCCESS)break;

        CDeviceModelInfo cDevModInfo;
        ENTasErrno enQryTasErr = DeviceModelItemQry(cReq.GetDeviceModelId(), cDevModInfo);
        if (ERR_TAS_DBERROR == enQryTasErr)
        {
            cRsp.SetErrorCode(ERR_TAS_DBERROR);
            break;
        }

        cRsp.SetErrorCode(DeviceModelItemDel(cReq.GetDeviceModelId()));
        if (cRsp.GetErrorCode() != CMS_SUCCESS)
        {
            break;
        }

        bHasNtf = true;
        cNtf.SetDeviceModelId(cReq.GetDeviceModelId());

        if (enQryTasErr == CMS_SUCCESS)
            SyncAnalyse(enSyncDataDeviceModel, enSyncOperateDel, CurrentDomainId(),
                        &cReq.GetDeviceModelId(), cDevModInfo.GetDeviceTypeId());
    } while (false);
    
    PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
    if (bHasNtf) PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cNtf, cTransNtf);
}

void CDbOpr::DeviceModelQry( const CTaskMsgTransfer<CDeviceModelQryReq> *pcMsg )
{
    DatabaseQueryTemplate<CDeviceModelQryReq, CDeviceModelQryRsp, CDeviceModelQryNtfReq, CDeviceModelInfo, TDeviceCapInfo>(
    pcMsg, &CDbOpr::PrepareDeviceModelQrySql, &CDbOpr::FetchDeviceModelQryRecord, &CDeviceModelQryNtfReq::GetDeviceModelInfo, 
    &CDeviceModelInfo::GetDeviceCapInfo, &CDeviceModelInfo::GetDeviceModelId);
}

ENTasErrno CDbOpr::PrepareDeviceModelQrySql( const CDeviceModelQryReq &cReq, TSqlString &tSqlSelect, TSqlString &tSqlCount )
{
    const CDeviceModelQueryCondition &cCond = cReq.GetDeviceModelQryCondition();

    /* 生成count语句 */
    TSqlString tSqlCond;
    tSqlCount.Format("select count(distinct a.%s) from %s a", F_DEVICEMODEL_MODELID, T_DEVICEMODEL);

    AppendQryCondValuePair(cCond.GetDevTypeId, m_nFieldValue, tSqlCond, F_DEVICEMODEL_DEVICETYPEID, a);
    AppendQryCondValuePair(cCond.GetModelId, m_strFieldValue, tSqlCond, F_DEVICEMODEL_MODELID, a);
    AppendQryCondValuePair(cCond.GetDomainId, m_strFieldValue, tSqlCond, F_DEVICEMODEL_DOMAINID, a);
    AppendQryCondValuePair(cCond.GetCreater, m_nFieldValue, tSqlCond, F_DEVICEMODEL_CREATER, a);
	AppendQryCondValuePair(cCond.GetOwner, m_strFieldValue, tSqlCond, F_DEVICEMODEL_OWNER, a);

    tSqlCond.EraseLastRedundantAnd();

    if (!tSqlCond.Empty())
    {
        tSqlCond.AppendString(" and ");
    }
    tSqlCond.AppendString(" a.%s != %d", F_DEVICEMODEL_DEVICETYPEID, DEVICE_TYPE_TV_WALL);

    tSqlCount.AppendString(" where ");
    tSqlCount.AppendString("%s", tSqlCond.GetBuf());


    /* 生成select语句 */
    tSqlSelect.Format("select a.* from %s a", T_DEVICEMODEL);

    string strDomainId;
    s32 nDevType = -1;
    s32 nCreater = -1;
    cReq.GetDeviceModelQryCondition().GetCreater(nCreater);
    string strModelId;
    cCond.GetDomainId(strDomainId);
    cCond.GetDevTypeId(nDevType);
    cCond.GetModelId(strModelId);
	string strOwner;
	cCond.GetOwner(strOwner);

    tSqlSelect.Format("call %s('%s', %d, %d, '%s', '%s', %d, %d)", P_DEVMODELQRY, DbStr(strDomainId),
        nDevType, nCreater, DbStr(strOwner), DbStr(strModelId), cReq.GetStartEntryIndex(), GetExpectEntryNum(cReq.GetExpectEntryNum()));

    return CMS_SUCCESS;
}

void CDbOpr::FetchDeviceModelQryRecord( CDeviceModelInfo &cInfo )
{
    cInfo.SetDeviceModelId(GetString(F_DEVICEMODEL_MODELID));
    cInfo.SetDomainId(GetString(F_DEVICEMODEL_DOMAINID));
    cInfo.SetDeviceTypeId(GetInt(F_DEVICEMODEL_DEVICETYPEID));
    cInfo.SetDeviceModelName(GetString(F_DEVICEMODEL_MODELNAME));
    cInfo.SetManufacturer(GetString(F_DEVICEMODEL_MANUFACTURER));
    cInfo.SetCreater(GetInt(F_DEVICEMODEL_CREATER));
	cInfo.SetOwner(GetString(F_DEVICEMODEL_OWNER));

    TDeviceCapInfo cCapInfo;
    cCapInfo.type = GetInt(F_DEVICEMODEL_DEVCAPID);
    cCapInfo.num = GetInt(F_DEVICEMODEL_CAPNUM);
    cCapInfo.license = GetInt(F_DEVICEMODEL_LICENSENUM);

    cInfo.GetDeviceCapInfo().clear();
    cInfo.GetDeviceCapInfo().push_back(cCapInfo);
}

void CDbOpr::DeviceCapIndexNameGet( const CTaskMsgTransfer<CDeviceCapIndexNameGetReq> *pcMsg )
{
    CTaskMsgTransfer<CDeviceCapIndexNameGetRsp> cTrans;
    CDeviceCapIndexNameGetRsp &cRsp = cTrans.GetMsgObj();

    const CDeviceCapIndexNameGetReq &cReq = pcMsg->GetMsgObj();


    ENTasErrno enRet = GetDeviceCapIndexName(cReq.GetDeviceId(), cReq.GetCapId(), cReq.GetCapIndex(), cRsp.GetDeviceCapName());
    if (ERR_TAS_DBERROR == enRet)
    {
        cRsp.SetErrorCode(ERR_TAS_DBERROR);
    }
    
    PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
}

ENTasErrno CDbOpr::DeviceCapIndexNameDel( const UUIDString &devId )
{
    TSqlString tSql;
    tSql.Format("delete from %s where %s='%s'", T_DEVICECAPINDEXDATA, 
        F_DEVICECAPINDEXDATA_DEVICEID, DbStr(devId));

    if (ExecSql(tSql, enExecNoRet))
        return CMS_SUCCESS;
    else
        return ERR_TAS_DBERROR;
}

ENTasErrno CDbOpr::DeviceCapIndexNameItemSet( const TDeviceCapName &cInfo, bool bAllowAppendCapNameWhenEmpty/*false*/ )
{
    TSqlString tSql;

    for (vector<TDeviceCapIndexName>::const_iterator it= cInfo.deviceCapIndexNames.begin();
        it != cInfo.deviceCapIndexNames.end(); ++it)
    {
		// 先查询出原有的扩展信息
		tSql.Format("select %s from %s where %s='%s' and %s=%d and %s = %d", 
			F_DEVICECAPINDEXDATA_DEVICECAPINDEXEXTDATA, T_DEVICECAPINDEXDATA, 
			F_DEVICECAPINDEXDATA_DEVICEID, DbStr(cInfo.deviceId),
			F_DEVICECAPINDEXDATA_DEVICECAPID, cInfo.capId, 
			F_DEVICECAPINDEXDATA_DEVICECAPINDEX, it->index);
		if (!ExecSql(tSql, enExecMayRet))
		{
			return ERR_TAS_DBERROR;
		}

		string strExtData;
		TDeviceCapExtData tOldExtData;
		if (IsNotEof())
		{
			strExtData = GetString(F_DEVICECAPINDEXDATA_DEVICECAPINDEXEXTDATA);
			if (!strExtData.empty())
			{
				tOldExtData.FromXml(strExtData);
			}
		}

		if (!it->extData.GetExtData().empty())
		{
			// 将修改后的扩展信息合并到已有数据中
			for (map<TDeviceCapExtDataType, string>::const_iterator it4Data = it->extData.GetExtData().begin(); 
				it->extData.GetExtData().end() != it4Data; ++it4Data)
			{
				double lfTmp = 0.00;
				if (DEVICE_CAP_EXT_DATA_TYPE_LON == it4Data->first)				// 经度
				{
					if (it->extData.GetLon(lfTmp) && fabs(lfTmp - INVALID_LONGITUDE) < 0.001)		// 和无效值的差值在0.001之间视为无效
					{
						// 无效值则不更新
						// tOldExtData.GetExtData().erase(it4Data->first);
					}
					else
					{
						tOldExtData.SetLon(lfTmp);
					}
				}
				else if (DEVICE_CAP_EXT_DATA_TYPE_LAT == it4Data->first)		// 纬度
				{
					if (it->extData.GetLat(lfTmp) && fabs(lfTmp - INVALID_LATITUDE) < 0.001)		// 和无效值的差值在0.001之间视为无效
					{
						// 无效值则不更新
						// tOldExtData.GetExtData().erase(it4Data->first);
					}
					else
					{
						tOldExtData.SetLat(lfTmp);
					}
				}
				else
				{
					tOldExtData.GetExtData()[it4Data->first] = it4Data->second;
				}
			}

			string strTmp = tOldExtData.ToXml();
			if (DEVICE_CAP_INDEX_EXTDATA_LENGTH <= strTmp.length())
			{
				return ERR_TAS_EXTDATA_TOO_LONG;
			}
			else
			{
				strExtData = strTmp;
			}
		}
		else
		{
			if (bAllowAppendCapNameWhenEmpty == true)
			{
				*const_cast<TDeviceCapExtData *>(&it->extData) = tOldExtData;  //假如为空，则将原有数据添加进去
			}
		}

		// 这里是先删除原有的数据，然后添加新的数据，这样比较别扭的实现了新增和更新两个功能
        tSql.Format("delete from %s where %s='%s' and %s=%d and %s = %d",
            T_DEVICECAPINDEXDATA, F_DEVICECAPINDEXDATA_DEVICEID, DbStr(cInfo.deviceId),
            F_DEVICECAPINDEXDATA_DEVICECAPID, cInfo.capId, F_DEVICECAPINDEXDATA_DEVICECAPINDEX, it->index);
        if (!ExecSql(tSql, enExecNoRet))
        {
            return ERR_TAS_DBERROR;
        }

        tSql.Format("insert into %s value('%s', %d, %d, '%s', '%s')", 
            T_DEVICECAPINDEXDATA, DbStr(cInfo.deviceId), 
            cInfo.capId, it->index, DbStr(it->name), DbStr(strExtData));
        if (!ExecSql(tSql, enExecNoRet))
        {
            return ERR_TAS_DBERROR;
        }


        // 更新视频源拼音搜索表
        if (cInfo.capId==DEVICE_CAP_VIDEOSOURCE)
        {
            vector<int> vidIds;
            vector<string> vidNames;
            for (vector<TDeviceCapIndexName>::const_iterator it=cInfo.deviceCapIndexNames.begin();
                it!=cInfo.deviceCapIndexNames.end();++it)
            {
                vidIds.push_back(it->index);
                vidNames.push_back(it->name);
            }

            ENTasErrno enRet=DevicePinyinSearchVideoNameSet(cInfo.deviceId, vidIds, vidNames);
            if(enRet!=CMS_SUCCESS) return enRet;
        }
    }

    return CMS_SUCCESS;
}

void CDbOpr::DeviceCapIndexNameSet( const CTaskMsgTransfer<CDeviceCapIndexNameSetReq> *pcMsg )
{
    CTaskMsgTransfer<CDeviceCapIndexNameSetRsp> cTrans;
    CDeviceCapIndexNameSetRsp &cRsp = cTrans.GetMsgObj();
    CTaskMsgTransfer<CDeviceCapIndexNameModNtfReq> cTransNtf;
    CDeviceCapIndexNameModNtfReq &cNtf = cTransNtf.GetMsgObj();
    bool bHasNtf=false;

    const CDeviceCapIndexNameSetReq &cReq = pcMsg->GetMsgObj();

    do 
    {
        const TDeviceCapName& tCapName = cReq.GetDeviceCapName();

        CDeviceInfo cDevInfo;
        ENTasErrno enDevQryErr = DeviceItemQry(tCapName.deviceId, cDevInfo);
        if (CMS_SUCCESS != enDevQryErr)
        {
            cRsp.SetErrorCode(enDevQryErr);
            break;
        }

		/* 需求要求上级能修改下级的经纬度
        if (cDevInfo.GetDomainUUID() != CurrentDomainId())
        {
            cRsp.SetErrorCode(ERR_TAS_INVALID_OPERATION);
            break;
        }
		*/

        cRsp.SetErrorCode(DeviceCapIndexNameItemSet(tCapName,true));
        if (cRsp.GetErrorCode() != CMS_SUCCESS)
        {
            break;
        }

        bHasNtf=true;
		cNtf.SetDomainId(cDevInfo.GetDomainUUID());

		TDeviceCapName tNtfCapName=tCapName;
		tNtfCapName.devDomainId = cDevInfo.GetDomainUUID();
        cNtf.SetDeviceCapName(tNtfCapName);

        for (u32 i = 0; i < tCapName.deviceCapIndexNames.size(); ++i)
        {
            SyncAnalyse(enSyncDataDeviceCapIndexName, enSyncOperateMod, CurrentDomainId(), 
                &tCapName.deviceId, tCapName.capId, tCapName.deviceCapIndexNames[i].index,
                cDevInfo.GetDeviceType());
        }
		if (tCapName.capId == DEVICE_CAP_VIDEOSOURCE
			&&tCapName.deviceCapIndexNames.size()==1
			&&cDevInfo.GetDomainUUID() == GetDomainUUID()
			&&cDevInfo.GetCreater() != CREATE_BY_GBS_FOREIGN_DOMAIN)
		{
			//查询数据库判断该设备的视频源个数是几，若是1，则同步修改设备名
			TSqlString tSql;
			tSql.Format("select %s from %s where %s=%d and %s=(select %s from %s where %s='%s')",
				F_DEVICEMODEL_CAPNUM, T_DEVICEMODEL,F_DEVICEMODEL_DEVCAPID,DEVICE_CAP_VIDEOSOURCE,
				F_DEVICEMODEL_MODELID, F_DEVICE_DEVMODELID,T_DEVICE,F_DEVICE_DEVID,DbStr(tCapName.deviceId));
			if (!ExecSql(tSql, enExecMustRet))
			{
				cRsp.SetErrorCode(ERR_TAS_DBERROR);
				break;
			}
			if (GetIntResult()==1)//单视频源设备同步修改其设备名
			{
				tSql.Format("update %s set %s='%s' where %s='%s'",
					T_DEVICE, F_DEVICE_DEVNAME,DbStr(tCapName.deviceCapIndexNames[0].name),
					F_DEVICE_DEVID, DbStr(tCapName.deviceId));
				if (!ExecSql(tSql, enExecNoRet))
				{
					cRsp.SetErrorCode(ERR_TAS_DBERROR);
					break;
				}
				CTaskMsgTransfer<CDeviceModNtfReq> cDevModTransNtf;
				CDeviceModNtfReq &cDevModNtf = cDevModTransNtf.GetMsgObj();
				cRsp.SetErrorCode(DeviceItemQry(tCapName.deviceId, cDevModNtf.GetDeviceInfo()));
				if (CMS_SUCCESS != cRsp.GetErrorCode())
				{
					break;
				}
				PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cDevModNtf, cDevModTransNtf);
			
				set<TChangedField> setChangedFields;
				setChangedFields.insert(TCHANGED_FIELD_DEVNAME);
				RedisDeviceMod(cDevModNtf.GetDeviceInfo(), setChangedFields);

				SyncAnalyse(enSyncDataDeviceInfo, enSyncOperateMod, cDevInfo.GetDomainUUID(), &(cDevInfo.GetDeviceUUID()), cDevInfo.GetDeviceType(), 
					cDevInfo.GetDevicePropertyFlags().HasPropertyFlag(TDevicePropertyFlags::em_PropertyFlags_PrivateDev),
					cDevInfo.GetDevicePropertyFlags().HasPropertyFlag(TDevicePropertyFlags::em_PropertyFlags_PrivateDev));

			}
		}

        UpdateDevTreeVer(tCapName.deviceId);
    } while (false);

    PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
    if (bHasNtf) 
    {
		RedisVidSrcAliasOrGpsSet(cReq.GetDeviceCapName());
        PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cNtf, cTransNtf);
    }
}

void CDbOpr::DeviceCapIndexNameQry( const CTaskMsgTransfer<CDeviceCapIndexNameQryReq> *pcMsg )
{
    DatabaseQueryTemplate<CDeviceCapIndexNameQryReq, CDeviceCapIndexNameQryRsp, CDeviceCapIndexNameQryNtfReq, TDeviceCapName>(
        pcMsg, &CDbOpr::PrepareDeviceCapIndexNameQrySql, &CDbOpr::FetchDeviceCapIndexNameQryRecord, 
        &CDeviceCapIndexNameQryNtfReq::GetInfos, &TDeviceCapName::GetDeviceId);
}

ENTasErrno CDbOpr::PrepareDeviceCapIndexNameQrySql( const CDeviceCapIndexNameQryReq &cReq, TSqlString &tSqlSelect, TSqlString &tSqlCount )
{
    tSqlSelect.Format("select a.*,b.%s from %s a,%s b where a.%s = b.%s order by a.%s", F_DEVICE_DOMAINID,T_DEVICECAPINDEXDATA,T_DEVICE,
		F_DEVICECAPINDEXDATA_DEVICEID,F_DEVICE_DEVID,F_DEVICECAPINDEXDATA_DEVICEID);
    AddRowNumLimitModifer(tSqlSelect, cReq.GetStartEntryIndex(), cReq.GetExpectEntryNum());

    tSqlCount.Format("select count(*) from %s", T_DEVICECAPINDEXDATA);

    return CMS_SUCCESS;
}

void CDbOpr::FetchDeviceCapIndexNameQryRecord( TDeviceCapName &devCapName )
{
    devCapName.deviceId = GetString(F_DEVICECAPINDEXDATA_DEVICEID);
    devCapName.capId = GetInt(F_DEVICECAPINDEXDATA_DEVICECAPID);
    devCapName.devDomainId=GetString(F_DEVICE_DOMAINID);

    TDeviceCapIndexName tCapIndexName;
    tCapIndexName.index = GetInt(F_DEVICECAPINDEXDATA_DEVICECAPINDEX);
    tCapIndexName.name = GetString(F_DEVICECAPINDEXDATA_DEVICECAPINDEXNAME);
	string strExtData = GetString(F_DEVICECAPINDEXDATA_DEVICECAPINDEXEXTDATA);
	if (!strExtData.empty())
	{
		tCapIndexName.extData.FromXml(strExtData);
	}

    devCapName.deviceCapIndexNames.clear();
    devCapName.deviceCapIndexNames.push_back(tCapIndexName);
}


bool CDbOpr::GetCuserInfo( const UUIDString &strId, CUserInfo &cInfo )
{
    TSqlString tSql;
    tSql.Format("select * from %s where %s='%s'", T_CUSER, F_CUSER_CUSERID, DbStr(strId));

    if (ExecSql(tSql, enExecMustRet))
    {
        FetchCuserQryRecord(cInfo);
        return true;
    }
    else
    {
        return false;
    }
}

ENTasErrno CDbOpr::GetCuserInfoByName( const string &strName, CUserInfo &cInfo )
{
    TSqlString tSql;
    tSql.Format("select * from %s where %s='%s'", T_CUSER, F_CUSER_USERNAME, DbStr(strName));

    if (ExecSql(tSql, enExecMayRet))
    {
        if (IsNotEof())
        {
            FetchCuserQryRecord(cInfo);
            return CMS_SUCCESS;
        }
        else
        {
            return ERR_TAS_RECORD_NOT_EXISTS;
        }
    }
    else
    {
        return ERR_TAS_DBERROR;
    }
}

bool CDbOpr::IsExistCuserInCertainUsergrp( const UUIDString &cusergrpid, bool &bExistFlag )
{
    TSqlString tSql;
    tSql.Format("select count(*) from %s where %s='%s'", T_CUSER, F_CUSER_USERGROUPID, DbStr(cusergrpid));

    if (ExecSql(tSql, enExecMustRet))
    {
        bExistFlag = (GetIntResult() > 0);
        return true;
    }
    else
    {
        return false;
    }
}

ENTasErrno CDbOpr::DeviceModelItemQry( const string &strId, CDeviceModelInfo &cInfo )
{
    TSqlString tSql;
    tSql.Format("select * from %s where %s = '%s'", T_DEVICEMODEL, F_DEVICEMODEL_MODELID, DbStr(strId));
    if (!ExecSql(tSql, enExecMayRet))
    {
        return ERR_TAS_DBERROR;
    }
    if (IsEof())
    {
        return ERR_TAS_RECORD_NOT_EXISTS;
    }

    cInfo.SetDeviceModelId(strId);
    cInfo.SetDeviceTypeId(GetInt(F_DEVICEMODEL_DEVICETYPEID));
    cInfo.SetDomainId(GetString(F_DEVICEMODEL_DOMAINID));
    cInfo.SetDeviceModelName(GetString(F_DEVICEMODEL_MODELNAME));
    cInfo.GetManufacturer() = GetString(F_DEVICEMODEL_MANUFACTURER);
    cInfo.SetCreater(GetInt(F_DEVICEMODEL_CREATER));
    TDeviceCapInfo tCapInfo;
    while (IsNotEof())
    {
        tCapInfo.type = GetInt(F_DEVICEMODEL_DEVCAPID);
        tCapInfo.num = GetInt(F_DEVICEMODEL_CAPNUM);
        tCapInfo.license = GetInt(F_DEVICEMODEL_LICENSENUM);

        cInfo.GetDeviceCapInfo().push_back(tCapInfo);

        if (!MoveNext())
        {
            return ERR_TAS_DBERROR;
        }
    }

    return CMS_SUCCESS;
}

ENTasErrno CDbOpr::DeviceModelItemAdd( const CDeviceModelInfo &model, bool bValidateCapId )
{
    TInnerDeviceTypeInfo tDevTypeInfo;
    if (!GetDeviceTypeInfo(model.GetDeviceTypeId(), tDevTypeInfo))
    {
        return ERR_TAS_INVALID_DEVICETYPEID;
    }

    for (vector<TDeviceCapInfo>::const_iterator it = model.GetDeviceCapInfo().begin(); it != model.GetDeviceCapInfo().end(); ++it)
    {
        if (it->num > 10000 || (bValidateCapId && tDevTypeInfo.adwDevCapIds.end() == 
            find(tDevTypeInfo.adwDevCapIds.begin(), tDevTypeInfo.adwDevCapIds.end(), it->type)))
        {
			CLASSLOG(enLogSrcDbTaskPool, ERROR_LEV, 
				"check param failed, ModelId[%s], CapId[%d], license[%d], num[%d]\n", 
				model.GetDeviceModelId().c_str(), it->type, it->license, it->num);

            return ERR_TAS_INVALID_PARM;
        }
    }

    TSqlString tSql;
    CUUID cModelId = model.GetDeviceModelId();

    const TDeviceCapInfo* ptCapInfo;
    u32 i;
    for (i = 0; i < model.GetDeviceCapInfo().size(); ++i)
    {
        ptCapInfo = &model.GetDeviceCapInfo()[i];
        tSql.Format("insert into %s values('%s', '%s', '%s', %d, '%s', %d, %d, %d, %d, '%s')", T_DEVICEMODEL, 
            DbStr(cModelId), DbStr(model.GetDomainId()), DbStr(model.GetDeviceModelName()), model.GetDeviceTypeId(),
            DbStr(model.GetManufacturer()), ptCapInfo->type, ptCapInfo->num, ptCapInfo->license, 
			(s32)model.GetCreater(), DbStr(model.GetOwner()));

        if (!ExecSql(tSql, enExecNoRet)) break;
    }

    if (i == model.GetDeviceCapInfo().size())
    {
        return CMS_SUCCESS;
    }
    else
    {
        tSql.Format("delete from %s where %s='%s'", T_DEVICEMODEL, F_DEVICEMODEL_MODELID, DbStr(cModelId));
        ExecSql(tSql, enExecNoRet);

        return ERR_TAS_DBERROR;
    }
}

ENTasErrno CDbOpr::DeviceModelItemDel( const string &strModelId )
{
    TSqlString tSql;

    tSql.Format("delete from %s where %s='%s'", T_DEVICEMODEL, F_DEVICEMODEL_MODELID, DbStr(strModelId));
    if (ExecSql(tSql, enExecNoRet))
    {
        g_cDbCache.DeleteDeviceModelInfoByModelId(strModelId);
        return CMS_SUCCESS;
    }
    else
    {
        return ERR_TAS_DBERROR;
    }
}


ENTasErrno CDbOpr::DeviceModelItemQryByDevId( const string &strDevId, CDeviceModelInfo &cInfo )
{
    ENTasErrno enErr;
    
    do 
    {
        CDeviceInfo cDevInfo;
        enErr = DeviceItemQry(strDevId, cDevInfo);
        if (CMS_SUCCESS != enErr)
        {
            break;
        }

        cInfo = g_cDbCache.GetDeviceModelInfo(cDevInfo.GetDeviceModel(), this);
        if (cInfo.GetDeviceModelId().empty())
        {
            enErr = ERR_TAS_RECORD_NOT_EXISTS;
            break;
        }

        enErr = CMS_SUCCESS;
    } while (false);
    
    return enErr;
}



bool CDbOpr::GetCuserGrpId( const UUIDString &strUserId, UUIDString &strGrpId )
{
    TSqlString tSql;
    tSql.Format("select %s from %s where %s='%s'", F_CUSER_USERGROUPID, T_CUSER, F_CUSER_CUSERID, DbStr(strUserId));
    if (!ExecSql(tSql, enExecMustRet))
    {
        return false;
    }

    strGrpId = GetString(F_CUSER_USERGROUPID);
    return true;
}

ENTasErrno CDbOpr::CuserQryByUserGrp( const UUIDString &strUserGrpId,vector<CUserInfo> &cUserInfos )
{
	TSqlString tSql;
	tSql.Format("select * from %s where  %s = '%s'",T_CUSER,F_CUSER_USERGROUPID,DbStr(strUserGrpId));
	if ( !ExecSql(tSql, enExecMayRet))
	{
		return ERR_TAS_DBERROR;
	}

	CUserInfo cTmpUser;
	while (IsNotEof())
	{
		cTmpUser.Clear();
		FetchCuserQryRecord(cTmpUser);
		cUserInfos.push_back(cTmpUser);
		if (!MoveNext())
		{
			return ERR_TAS_DBERROR;
		}
	}

	return CMS_SUCCESS;
}


void CDbOpr::UpdateCuserDevTreeVer( const UUIDString &strUserId )
{
	++s_dwDevTreeVerExt;

    string strId = !strUserId.empty()? strUserId:CurrentDomainId();
    CCMSTime cTm;
    TSqlString tSql;
    tSql.Format("update %s set %s='%s_%u' where %s='%s'", 
        T_CUSER,F_CUSER_DEVTREEVER, DbStr(cTm.ToString()), s_dwDevTreeVerExt, F_CUSER_CUSERID, DbStr(strId));
    if (!ExecSql(tSql, enExecNoRet))
    {
        CLASSLOG(enLogSrcDbTaskPool, ERROR_LEV, 
            "update cuser's devTreeVer failed, cuserId=%s\n", strUserId.c_str());
    }
}

bool CDbOpr::GetDeviceTypeInfo( u32 dwType, TInnerDeviceTypeInfo &cInfo )
{
    TSqlString tSql;
    tSql.Format("select * from %s where %s = %d", T_DEVICETYPE, F_DEVICETYPE_DEVTYPEID, (s32)dwType);
    if (!ExecSql(tSql, enExecMustRet))
    {
        return false;
    }

    cInfo.dwTypeId = dwType;
    while (IsNotEof())
    {
        cInfo.adwDevCapIds.push_back(GetInt(F_DEVICETYPE_DEVCAPID));

        if (!MoveNext())
        {
            return false;
        }
    }
	if (dwType == DEVICE_TYPE_DECODER)
	{
		cInfo.adwDevCapIds.push_back(DEVICE_CAP_LABEL_GBDEVICE);
	}
    return true;
}

void CDbOpr::DeviceManufacturerAdd( const CTaskMsgTransfer<CDeviceManufacturerAddReq> *pcMsg )
{
    CTaskMsgTransfer<CDeviceManufacturerAddRsp> cTrans;
    CDeviceManufacturerAddRsp &cRsp = cTrans.GetMsgObj();

    const CDeviceManufacturerAddReq &cReq = pcMsg->GetMsgObj();

    do 
    {
        TSqlString tSql;
        tSql.Format("select count(*) from %s where %s='%s'", T_DEVICEMANUFACTURER, 
            F_DEVICEMANUFACTURER_MANUFACTURERNAME, DbStr(cReq.GetManufacturer()));
        if (!ExecSql(tSql, enExecMustRet))
        {
            cRsp.SetErrorCode(ERR_TAS_DBERROR);
            break;
        }
        if (GetIntResult() > 0)
        {
            cRsp.SetErrorCode(ERR_TAS_RECORD_EXISTS);
            break;
        }
        
        tSql.Format("insert into %s values('%s', %d)", T_DEVICEMANUFACTURER, 
            DbStr(cReq.GetManufacturer()), 0);

        if (!ExecSql(tSql, enExecNoRet))
        {
            cRsp.SetErrorCode(ERR_TAS_DBERROR);
            break;
        }
    } while (false);

    PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
}

void CDbOpr::DeviceManufacturerDel( const CTaskMsgTransfer<CDeviceManufacturerDelReq> *pcMsg )
{
    CTaskMsgTransfer<CDeviceManufacturerDelRsp> cTrans;
    CDeviceManufacturerDelRsp &cRsp = cTrans.GetMsgObj();

    const CDeviceManufacturerDelReq &cReq = pcMsg->GetMsgObj();
    do 
    {
        TSqlString tSql;
        tSql.Format("select count(*) from %s where %s='%s' and %s=1", T_DEVICEMANUFACTURER,
            F_DEVICEMANUFACTURER_MANUFACTURERNAME, DbStr(cReq.GetManufacturer()), F_DEVICEMANUFACTURER_BUILDIN);
        if (ExecSql(tSql, enExecMustRet))
        {
            if (GetIntResult() > 0)
            {
                cRsp.SetErrorCode(ERR_TAS_INVALID_OPERATION);
                break;
            }
        }
        else
        {
            cRsp.SetErrorCode(ERR_TAS_DBERROR);
            break;
        }

        tSql.Format("delete from %s where %s = '%s' and %s=0", T_DEVICEMANUFACTURER, 
            F_DEVICEMANUFACTURER_MANUFACTURERNAME, DbStr(cReq.GetManufacturer()), F_DEVICEMANUFACTURER_BUILDIN);
        if (!ExecSql(tSql, enExecNoRet))
        {
            cRsp.SetErrorCode(ERR_TAS_DBERROR);
        }
    } while (false);

    PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
}

void CDbOpr::DeviceManufacturerQry( const CTaskMsgTransfer<CDeviceManufacturerQryReq> *pcMsg )
{
    DatabaseQueryTemplate<CDeviceManufacturerQryReq, CDeviceManufacturerQryRsp, CDeviceManufacturerQryNtfReq, CDeviceManufacturerInfo>(
        pcMsg, &CDbOpr::PrepareDeviceManufacturerQrySql, &CDbOpr::FetchDeviceManufacturerQryRecord, 
        &CDeviceManufacturerQryNtfReq::GetDeviceManufacturerInfo, &CDeviceManufacturerInfo::GetManufacturer);
}

ENTasErrno CDbOpr::PrepareDeviceManufacturerQrySql( const CDeviceManufacturerQryReq &cReq, TSqlString &tSqlSelect, TSqlString &tSqlCount )
{
    tSqlSelect.Format("select a.* from %s a order by convert(a.%s using gbk) ", T_DEVICEMANUFACTURER, F_DEVICEMANUFACTURER_MANUFACTURERNAME);
    tSqlCount.Format("select count(*) from %s a", T_DEVICEMANUFACTURER);

    AddRowNumLimitModifer(tSqlSelect, cReq.GetStartEntryIndex(), cReq.GetExpectEntryNum());
    return CMS_SUCCESS;
}

void CDbOpr::FetchDeviceManufacturerQryRecord( CDeviceManufacturerInfo &manufacturer )
{
    manufacturer.SetManufacturer(GetString(F_DEVICEMANUFACTURER_MANUFACTURERNAME));
    manufacturer.SetBuildIn(GetBool(F_DEVICEMANUFACTURER_BUILDIN));
}

void CDbOpr::CuserDevicefullinfoAndPrivilegeQry( const CTaskMsgTransfer<CCuserDevicefullinfoAndPrivilegeQryReq> *pcMsg )
{
    // 此处由于存在多个子容器和局部性能优化，不使用DatabaseQueryTemplate模板方式实现

	CTaskMsgTransfer<CCuserDevicefullinfoAndPrivilegeQryRsp> cTrans;
	CCuserDevicefullinfoAndPrivilegeQryRsp &cRsp = cTrans.GetMsgObj();
	CTaskMsgTransfer<CCuserDevicefullinfoAndPrivilegeQryNtfReq> cTransNtf;
	CCuserDevicefullinfoAndPrivilegeQryNtfReq &cNtf = cTransNtf.GetMsgObj();

	const CCuserDevicefullinfoAndPrivilegeQryReq &cReq = pcMsg->GetMsgObj();
	TSqlString tSql, tSqlCount;

	do 
	{
		bool bNeedQryHint;
		ENTasErrno enErr = PrepareCuserDevicefullinfoAndPrivilegeQrySql(cReq, tSql, tSqlCount, bNeedQryHint);
		if (CMS_SUCCESS != enErr)
		{
			cRsp.SetErrorCode(enErr);
			break;
		}

		if (cReq.GetStartEntryIndex() == 0 && cReq.GetQueryHint().empty())
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
		TDevicefullinfoAndPrivilege cInfo;
        u32 RecordNumInNtf = 0;
        u32 dwNtfContentSize = 0;
		// 算法： 循环里面一条一条的获取DB记录，同时组装业务记录 --> 组装N条业务记录为一个NTF消息; 循环之外处理最后一条消息
		while (IsNotEof())
		{
			// 获取DB记录和组装业务记录
			bool bBusinessRecordCombineFinished;
            cRsp.SetErrorCode(FetchCuserDevicefullinfoAndPrivilegeQryRecord(cInfo, cInfo.deviceInfo.GetDeviceUUID().empty(), bBusinessRecordCombineFinished));
            if (cRsp.GetErrorCode() != CMS_SUCCESS)
            {
                break;
            }

            if (!bBusinessRecordCombineFinished && (dwNtfContentSize + cInfo.ToXml().size() >= SIP_MSG_LEN_MAX))
            {
                bBusinessRecordCombineFinished = true; // 消息过大，强制完成
            }

			// 组装NTF消息
			if (bBusinessRecordCombineFinished)
			{
                u32 dwRecordContentSize = cInfo.ToXml().size();
                if (dwNtfContentSize + dwRecordContentSize < SIP_MSG_LEN_MAX)
                {
                    dwNtfContentSize += dwRecordContentSize;
                    ++RecordNumInNtf;
                    cNtf.GetDevicefullinfoAndPrivileges().push_back(cInfo);
                }
                else
                {
                    cNtf.SetEntryNum(RecordNumInNtf);
                    cNtf.SetLastNtf(false);
                    PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cNtf, cTransNtf);

                    dwRecordcount += RecordNumInNtf;

                    cNtf.GetDevicefullinfoAndPrivileges().clear();
                    cNtf.GetDevicefullinfoAndPrivileges().push_back(cInfo);
                    dwNtfContentSize = dwRecordContentSize;
                    RecordNumInNtf = 1;
                }

				// 记录下一条的第一个子记录，因为前面组装完成时未记录
				cRsp.SetErrorCode(FetchCuserDevicefullinfoAndPrivilegeQryRecord(cInfo, true, bBusinessRecordCombineFinished));
                if (cRsp.GetErrorCode() != CMS_SUCCESS)
                {
                    break;
                }
			}

			if (!MoveNext())
			{
				cRsp.SetErrorCode(ERR_TAS_DBERROR);
				break;
			}
		}

		// 组装最后一个NTF消息, 不出错且存在记录时才执行(只要存在一条记录cInfo就一定不为空)
		if (cRsp.GetErrorCode() == CMS_SUCCESS && !cInfo.deviceInfo.GetDeviceUUID().empty())
        {
            if (dwNtfContentSize + cInfo.ToXml().size() < SIP_MSG_LEN_MAX)
            {
                ++RecordNumInNtf;

                cNtf.GetDevicefullinfoAndPrivileges().push_back(cInfo);
                cNtf.SetEntryNum(RecordNumInNtf);
                cNtf.SetLastNtf(true);
				if(bNeedQryHint)
				{
					cNtf.SetQueryHint(cInfo.deviceInfo.GetDeviceUUID());
				}
                PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cNtf, cTransNtf);

                dwRecordcount += RecordNumInNtf;
            }
            else
            {
                cNtf.SetEntryNum(RecordNumInNtf);
                cNtf.SetLastNtf(false);
                PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cNtf, cTransNtf);
                dwRecordcount += RecordNumInNtf;

                cNtf.GetDevicefullinfoAndPrivileges().clear();
                cNtf.GetDevicefullinfoAndPrivileges().push_back(cInfo);
                cNtf.SetEntryNum(1);
                cNtf.SetLastNtf(true);
				if(bNeedQryHint)
				{
					cNtf.SetQueryHint(cInfo.deviceInfo.GetDeviceUUID());
				}
                PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cNtf, cTransNtf);
                dwRecordcount += 1;
            }
		}

		cRsp.SetEntryNum(dwRecordcount);
	} while (false);

	PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
}

ENTasErrno CDbOpr::PrepareCuserDevicefullinfoAndPrivilegeQrySql( const CCuserDevicefullinfoAndPrivilegeQryReq &cReq, TSqlString &tSqlSelect, TSqlString &tSqlCount,
	bool &bNeedQryHint)
{
	bNeedQryHint = false;
    string strDomainId, strDevicegroupId,strDevTreeId;

    const CCuserDevicefullinfoAndPrivilegeQueryCondition &cCond = cReq.GetCondition();
    cCond.GetDomainId(strDomainId);
    if (cCond.GetDevicegroupId(strDevicegroupId))
    {
        if (strDevicegroupId.empty()) return ERR_TAS_INVALID_PARM; // 所有的设备都属于组，无效条件
    }

	cCond.GetDevTreeId(strDevTreeId);

    /* 确定count语句 */
    UUIDString cusergrpId;
    if (!GetCuserGrpId(cReq.GetCuserId(), cusergrpId))
    {
        return ERR_TAS_USER_NOT_EXIST;
    }

    if (cusergrpId.empty())
    {
        if (strDomainId.empty())
        {
			if (!strDevicegroupId.empty())
			{
				tSqlCount.Format("select count(*) from\
								 ((select b.*, a.cuserId as cuserId, a.devPrivilege as devPrivilege\
								 FROM tblCuser2Dev a, tblDevice b, %s c\
								 where a.devId = b.devId and a.devId = c.devId and a.cuserId='%s' and \
								 c.devgroupId='%s')\
								 union\
								 (select b.*, '' as cuserId, 0 as devPrivilege\
								 FROM tblDevice b, %s c, tblDeviceModel d\
								 where b.devId = c.devId and c.devgroupId='%s' \
								 and b.devModelId=d.modelId and d.deviceTypeId=2 and d.devCapId=5)) res", 
								 DbStr(GetTblDevGrp2DevName(strDevTreeId)),DbStr(cReq.GetCuserId()), DbStr(strDevicegroupId), 
								 DbStr(GetTblDevGrp2DevName(strDevTreeId)), DbStr(strDevicegroupId));
			}
			else
			{
				tSqlCount.Format("select count(*) from\
								 ((select b.*, a.cuserId as cuserId, a.devPrivilege as devPrivilege\
								 FROM tblCuser2Dev a, tblDevice b, %s c\
								 where a.devId = b.devId and a.devId = c.devId and a.cuserId='%s') \
								 union\
								 (select b.*, '' as cuserId, 0 as devPrivilege\
								 FROM tblDevice b, %s c, tblDeviceModel d\
								 where b.devId = c.devId \
								 and b.devModelId=d.modelId and d.deviceTypeId=2 and d.devCapId=5)) res", 
								 DbStr(GetTblDevGrp2DevName(strDevTreeId)),DbStr(cReq.GetCuserId()), 
								 DbStr(GetTblDevGrp2DevName(strDevTreeId)));
			}
            
        }
        else
        {
			if (!strDevicegroupId.empty())
			{
				tSqlCount.Format("select count(*) from\
								 ((select b.*, a.cuserId as cuserId, a.devPrivilege as devPrivilege\
								 FROM tblCuser2Dev a, tblDevice b, %s c\
								 where a.devId = b.devId and a.devId = c.devId and a.cuserId='%s' and \
								 c.devgroupId='%s' and b.domainId='%s')\
								 union\
								 (select b.*, '' as cuserId, 0 as devPrivilege\
								 FROM tblDevice b, %s c, tblDeviceModel d\
								 where b.devId = c.devId and c.devgroupId='%s' \
								 and b.devModelId=d.modelId and d.deviceTypeId=2 and d.devCapId=5 and b.domainId='%s')) res", 
								 DbStr(GetTblDevGrp2DevName(strDevTreeId)),
								 DbStr(cReq.GetCuserId()), DbStr(strDevicegroupId), DbStr(strDomainId), 
								 DbStr(GetTblDevGrp2DevName(strDevTreeId)), DbStr(strDevicegroupId), DbStr(strDomainId));
			}
			else
			{
				tSqlCount.Format("select count(*) from\
								 ((select b.*, a.cuserId as cuserId, a.devPrivilege as devPrivilege\
								 FROM tblCuser2Dev a, tblDevice b, %s c\
								 where a.devId = b.devId and a.devId = c.devId and a.cuserId='%s' and b.domainId='%s')\
								 union\
								 (select b.*, '' as cuserId, 0 as devPrivilege\
								 FROM tblDevice b, %s c, tblDeviceModel d\
								 where b.devId = c.devId  \
								 and b.devModelId=d.modelId and d.deviceTypeId=2 and d.devCapId=5 and b.domainId='%s')) res", 
								 DbStr(GetTblDevGrp2DevName(strDevTreeId)),
								 DbStr(cReq.GetCuserId()), DbStr(strDomainId), 
								 DbStr(GetTblDevGrp2DevName(strDevTreeId)), DbStr(strDomainId));
			}
            
        }
    }
    else
    {
        if (strDomainId.empty())
        {
			if (!strDevicegroupId.empty())
			{
				tSqlCount.Format("select count(*) from\
								 ((select b.*, a.cusergroupId as cusergroupId, a.devPrivilege as devPrivilege\
								 FROM tblCusergrp2Dev a, tblDevice b, %s c\
								 where a.devId = b.devId and a.devId = c.devId and a.cusergroupId='%s' and \
								 c.devgroupId='%s')\
								 union\
								 (select b.*, '' as cuserId, 0 as devPrivilege\
								 FROM tblDevice b, %s c, tblDeviceModel d\
								 where b.devId = c.devId and c.devgroupId='%s' \
								 and b.devModelId=d.modelId and d.deviceTypeId=2 and d.devCapId=5)) res", 
								 DbStr(GetTblDevGrp2DevName(strDevTreeId)), DbStr(cusergrpId), DbStr(strDevicegroupId), 
								 DbStr(GetTblDevGrp2DevName(strDevTreeId)), DbStr(strDevicegroupId));
			}
			else
			{
				tSqlCount.Format("select count(*) from\
								 ((select b.*, a.cusergroupId as cusergroupId, a.devPrivilege as devPrivilege\
								 FROM tblCusergrp2Dev a, tblDevice b, %s c\
								 where a.devId = b.devId and a.devId = c.devId and a.cusergroupId='%s' )\
								 union\
								 (select b.*, '' as cuserId, 0 as devPrivilege\
								 FROM tblDevice b, %s c, tblDeviceModel d\
								 where b.devId = c.devId  \
								 and b.devModelId=d.modelId and d.deviceTypeId=2 and d.devCapId=5)) res", 
								 DbStr(GetTblDevGrp2DevName(strDevTreeId)), DbStr(cusergrpId), 
								 DbStr(GetTblDevGrp2DevName(strDevTreeId)));
			}
           
        }
        else
        {
			if (!strDevicegroupId.empty())
			{
				tSqlCount.Format("select count(*) from\
								 ((select b.*, a.cusergroupId as cusergroupId, a.devPrivilege as devPrivilege\
								 FROM tblCusergrp2Dev a, tblDevice b, %s c\
								 where a.devId = b.devId and a.devId = c.devId and a.cusergroupId='%s' and \
								 c.devgroupId='%s' and b.domainId='%s')\
								 union\
								 (select b.*, '' as cuserId, 0 as devPrivilege\
								 FROM tblDevice b, %s c, tblDeviceModel d\
								 where b.devId = c.devId and c.devgroupId='%s' \
								 and b.devModelId=d.modelId and d.deviceTypeId=2 and d.devCapId=5 and b.domainId='%s')) res", 
								 DbStr(GetTblDevGrp2DevName(strDevTreeId)), DbStr(cusergrpId), DbStr(strDevicegroupId), DbStr(strDomainId), 
								 DbStr(GetTblDevGrp2DevName(strDevTreeId)), DbStr(strDevicegroupId), DbStr(strDomainId));
			}
			else
			{
				tSqlCount.Format("select count(*) from\
								 ((select b.*, a.cusergroupId as cusergroupId, a.devPrivilege as devPrivilege\
								 FROM tblCusergrp2Dev a, tblDevice b, %s c\
								 where a.devId = b.devId and a.devId = c.devId and a.cusergroupId='%s' and  b.domainId='%s')\
								 union\
								 (select b.*, '' as cuserId, 0 as devPrivilege\
								 FROM tblDevice b, %s c, tblDeviceModel d\
								 where b.devId = c.devId  \
								 and b.devModelId=d.modelId and d.deviceTypeId=2 and d.devCapId=5 and b.domainId='%s')) res", 
								 DbStr(GetTblDevGrp2DevName(strDevTreeId)), DbStr(cusergrpId), DbStr(strDomainId), 
								 DbStr(GetTblDevGrp2DevName(strDevTreeId)), DbStr(strDomainId));
			}
            
        }
    }

    /*select count(*) from tblDevice a
    where a.devType=2 and a.domainId=''
        and EXISTS(SELECT * FROM tblDevGrp2Dev b WHERE b.devgroupId='769e758e20ee4ae4ac02aa5898832a82' AND b.devId=a.devId)*/
    /*tSqlCount2.Format("select count(*) from %s a where a.%s=%d", T_DEVICE, F_DEVICE_DEVTYPE, DEVICE_TYPE_DECODER);
    if (!strDomainId.empty())
    {
        tSqlCount2.AppendString(" and a.%s='%s'", F_DEVICE_DOMAINID, DbStr(strDomainId));
    }
    tSqlCount2.AppendString(" and EXISTS(SELECT * FROM %s b WHERE b.%s='%s' AND b.%s=a.%s)",
        T_DEVGRP2DEV, F_DEVGRP2DEV_DEVGROUPID, DbStr(strDevicegroupId),
        F_DEVGRP2DEV_DEVID, F_DEVICE_DEVID);*/

    /* 确定查询内容语句 */
	int nDevGrpDevNum = 0;
	tSqlSelect.Format("select count(*) from %s where %s='%s'", 
		DbStr(GetTblDevGrp2DevName(strDevTreeId)), 
		F_DEVGRP2DEV_DEVGROUPID,DbStr(strDevicegroupId));
	if (!ExecSql(tSqlSelect, enExecMustRet))
	{
		return ERR_TAS_DBERROR;
	}
	nDevGrpDevNum = GetIntResult();
    if (cusergrpId.empty())
    {
		if (nDevGrpDevNum>5000)
		{
			bNeedQryHint = true;
			tSqlSelect.Format("call %s('%s', '%s', '%s','%s', '%s', %d)", P_DEVICEFULLINFOANDPRIVILEGE3, 
				DbStr(cReq.GetCuserId()), DbStr(strDomainId), DbStr(strDevicegroupId),DbStr(strDevTreeId),
				DbStr(cReq.GetQueryHint()), 
				GetExpectEntryNum(cReq.GetExpectEntryNum()));
		}
		else
		{
        tSqlSelect.Format("call %s('%s', '%s', '%s','%s', %d, %d)", P_DEVICEFULLINFOANDPRIVILEGE, 
            DbStr(cReq.GetCuserId()), DbStr(strDomainId), DbStr(strDevicegroupId),DbStr(strDevTreeId),
            cReq.GetStartEntryIndex(), 
				GetExpectEntryNum(cReq.GetExpectEntryNum()));
		}
    }
    else
    {
		if (nDevGrpDevNum>5000)
		{
			bNeedQryHint = true;
			tSqlSelect.Format("call %s('%s', '%s', '%s','%s', '%s', %d)", P_DEVICEFULLINFOANDPRIVILEGE4, 
				DbStr(cusergrpId), DbStr(strDomainId), DbStr(strDevicegroupId),DbStr(strDevTreeId),
				DbStr(cReq.GetQueryHint()), 
            GetExpectEntryNum(cReq.GetExpectEntryNum()));
    }
    else
    {
        tSqlSelect.Format("call %s('%s', '%s', '%s', '%s',%d, %d)", P_DEVICEFULLINFOANDPRIVILEGE2, 
            DbStr(cusergrpId), DbStr(strDomainId), DbStr(strDevicegroupId),DbStr(strDevTreeId),
            cReq.GetStartEntryIndex(), 
            GetExpectEntryNum(cReq.GetExpectEntryNum()));
		}
    }

    return CMS_SUCCESS;
}

ENTasErrno CDbOpr::FetchCuserDevicefullinfoAndPrivilegeQryRecord( TDevicefullinfoAndPrivilege &record, bool bNew, bool &bCombineFinished )
{
    UUIDString devId = GetString(F_V_DEVICEFULLINFOANDPRIVILEGE_DEVID);
    if (bNew)
    {
        // 新的设备才需要获取设备信息
        CDeviceInfo &dev = record.deviceInfo;
        dev.SetDeviceModel(GetString(F_V_DEVICEFULLINFOANDPRIVILEGE_DEVMODELID));
        CDeviceModelInfo modelInfo = g_cDbCache.GetDeviceModelInfo(dev.GetDeviceModel(), NULL);
        if (modelInfo.GetDeviceModelId().empty())
        {
            return ERR_TAS_RECORD_NOT_EXISTS;
        }

        dev.SetDeviceUUID(devId);
        dev.SetDomainUUID(GetString(F_V_DEVICEFULLINFOANDPRIVILEGE_DOMAINID));
        dev.SetDeviceName(GetString(F_V_DEVICEFULLINFOANDPRIVILEGE_DEVNAME));
        dev.SetDeviceAlias(GetString(F_V_DEVICEFULLINFOANDPRIVILEGE_DEVALIAS));
        dev.SetDeviceType(GetInt(F_V_DEVICEFULLINFOANDPRIVILEGE_DEVTYPE));
        dev.SetManufacturer(modelInfo.GetManufacturer());
        dev.SetModelName(modelInfo.GetDeviceModelName());
        dev.GetDeviceModelCapDetail() = modelInfo.GetDeviceCapInfo();
        dev.SetUsedFlag(GetBool(F_V_DEVICEFULLINFOANDPRIVILEGE_USEFLAG));
        dev.SetNetType(GetInt(F_V_DEVICEFULLINFOANDPRIVILEGE_NETTYPE));
        dev.SetNetAddress(GetString(F_V_DEVICEFULLINFOANDPRIVILEGE_NETADDR));
        dev.SetDescription(GetString(F_V_DEVICEFULLINFOANDPRIVILEGE_DEVDESC));
        dev.SetExpireDate(GetCmsDate(F_V_DEVICEFULLINFOANDPRIVILEGE_EXPIREDATE));
        dev.SetCreateDate(GetCmsDate(F_V_DEVICEFULLINFOANDPRIVILEGE_CREATEDATE));
        dev.SetCreater(GetInt(F_V_DEVICEFULLINFOANDPRIVILEGE_CREATER));
		dev.SetOwner(GetString(F_V_DEVICEFULLINFOANDPRIVILEGE_OWNER));

		string strDevExtData = GetString(F_V_DEVICEFULLINFOANDPRIVILEGE_DEVDATA);
		if (!strDevExtData.empty())
		{
			TDevExtData &tDevExtData = dev.GetDevData();
			tDevExtData.FromXml(strDevExtData);
		}

        CEncoderPrivilege cDevPrivi;
        cDevPrivi.ConvertFromInt(GetInt(F_V_DEVICEFULLINFOANDPRIVILEGE_DEVPRIVILEGE));
        record.devicePrivilege.SetEncoderPrivilege(cDevPrivi);

        record.videosourceinfos.clear();
        record.alarminputinfos.clear();
		record.vidSrcIndex.clear();
    }
    else
    {
        // 上一个设备的信息获取完成
        if (record.deviceInfo.GetDeviceUUID() != devId) 
        {
            bCombineFinished = true;
            return CMS_SUCCESS;
        }
    }

    s32 nCapId = GetInt(F_V_DEVICEFULLINFOANDPRIVILEGE_DEVCAPID);
    s32 nCapIndex = GetInt(F_V_DEVICEFULLINFOANDPRIVILEGE_DEVCAPINDEX);
    
    if (DEVICE_CAP_VIDEOSOURCE == nCapId)
    {
        TUserVideoSourceCapInfo tCapInfo;
        tCapInfo.name = GetString(F_V_DEVICEFULLINFOANDPRIVILEGE_DEVICECAPINDEXNAME);
        tCapInfo.privilege.ConvertFromInt(GetInt(F_V_DEVICEFULLINFOANDPRIVILEGE_DEVCAPINDEXPRIVILEGE));
		tCapInfo.extData = GetString(F_V_DEVICEFULLINFOANDPRIVILEGE_DEVICECAPINDEXDATA);
        record.videosourceinfos.insert(map<int, TUserVideoSourceCapInfo>::value_type(nCapIndex, tCapInfo));
		record.vidSrcIndex.push_back(nCapIndex);
    }
    else if (DEVICE_CAP_PARALLEL_PORT_ALARM_INPUT == nCapId)
    {
        TUserAlarmInputCapInfo tCapInfo;
        tCapInfo.name = GetString(F_V_DEVICEFULLINFOANDPRIVILEGE_DEVICECAPINDEXNAME);
        tCapInfo.privilege.ConvertFromInt(GetInt(F_V_DEVICEFULLINFOANDPRIVILEGE_DEVCAPINDEXPRIVILEGE));
        record.alarminputinfos.insert(map<int, TUserAlarmInputCapInfo>::value_type(nCapIndex, tCapInfo));
    }

    bCombineFinished = false;
    return CMS_SUCCESS;
}

void CDbOpr::CuserAdd( const CTaskMsgTransfer<CUserAddReq> *pcMsg )
{
    CTaskMsgTransfer<CUserAddRsp> cTrans;
    CTaskMsgTransfer<CUserAddNtfReq> cTransNtf;
    CUserAddRsp &cRsp = cTrans.GetMsgObj();
    CUserAddNtfReq &cNtf = cTransNtf.GetMsgObj();
    bool bHasNtf = false;

    const CUserAddReq &cReq = pcMsg->GetMsgObj();

    do 
    {
        const CUserInfo &cInfo = cReq.GetUserInfo();

        if (cInfo.GetUserType() == CUSER_TYPE_SUPERADMIN)
        {
            cRsp.SetErrorCode(ERR_TAS_INVALID_OPERATION);
            break;
        }

        TSqlString tSql;
        tSql.Format("select count(*) from %s where %s='%s'", T_CUSER, 
            F_CUSER_USERNAME, DbStr(cInfo.GetUserName()));
        if (!ExecSql(tSql, enExecMustRet))
        {
            cRsp.SetErrorCode(ERR_TAS_DBERROR);
            break;
        }
        if (GetIntResult() > 0)
        {
            cRsp.SetErrorCode(ERR_TAS_RECORD_EXISTS);
            break;
        }
        CUserInfo cCreatorInfo;
        if (!GetCuserInfo(cInfo.GetCreatorID(), cCreatorInfo))
        {
            cRsp.SetErrorCode(ERR_TAS_USER_NOT_EXIST);
            break;
        }

        CUserPrivilege cUserPrivi = cInfo.GetUserPrivilege();
		int nSwitchNum = cInfo.GetSwitchNum();
        if (!cInfo.GetUsergrpID().empty())
        {
            CUserGroupInfo cUserGrpInfo;
            if (GetCuserGrpInfo(cInfo.GetUsergrpID(), cUserGrpInfo))
            {
                if (cInfo.GetUserType() != cUserGrpInfo.GetUserType())
                {
                    cRsp.SetErrorCode(ERR_TAS_INVALID_OPERATION);
                    break;
                }
            }
            else
            {
                cRsp.SetErrorCode(ERR_TAS_RECORD_FETCH_FAILED);
                break;
            }

            cUserPrivi = cUserGrpInfo.GetUserPrivilege();
			nSwitchNum = cUserGrpInfo.GetSwitchNum();
        }

		string strDevTreeVer = cInfo.GetDeviceTreeVer();
		if (strDevTreeVer.empty())
		{
			strDevTreeVer = CCMSTime().ToString();
		}

		CUUID cuserId;

		// 35114要使用国标ID.
		cuserId = this->GetNewUserGBID();




		CCMSTime cPwdExpireDate;
		if (cInfo.GetPwdExpireEnabledFlag())
		{
			cPwdExpireDate = CCMSTime() + cInfo.GetPwdExpireTime() * 24 * 60 * 60;
			tSql.Format("insert into %s values('%s', '%s', '%s', %d, '%s', %d, %d, %d, '%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, %d, '%s', %d, %d, %d, %d, %d, '%s', '%s', '%s','%s')", 
				T_CUSER, DbStr(cuserId), DbStr(cInfo.GetUserName()),
				DbStr(cInfo.GetUserPwd()), cInfo.GetUserType(), DbStr(cInfo.GetUsergrpID()), cUserPrivi.Convert2Int(),
				cInfo.GetMLoginFlag(), cInfo.GetEnabledFlag(), DbStr(cInfo.GetDescription()), DbDateTime(cInfo.GetNameExpireDate()),
				DbDateTime(cPwdExpireDate.ToString()), cInfo.GetPwdExpireTime(), cInfo.GetPwdExpireEnabledFlag(), DbStr(cInfo.GetLoginIpList()), 
				cInfo.GetIpListEnabledFlag(), cInfo.GetIpBindFlagForMobileClient(),  nSwitchNum, cInfo.GetWaterMarkEnabled(), 
				DbStr(cInfo.GetWaterMarkFont()), cInfo.GetWaterMarkFontSize(), cInfo.GetWaterMarkRotationAngle(), cInfo.GetWaterMarkColor(), 
				cInfo.GetWaterMarkTransparent(), cInfo.GetWaterMarkTiled(), DbNowDateTime(), DbStr(cInfo.GetCreatorID()), DbStr(strDevTreeVer),"");
		}
		else
		{
			tSql.Format("insert into %s values('%s', '%s', '%s', %d, '%s', %d, %d, %d, '%s', '%s', '%s', %d, %d, '%s', %d, %d, %d,  %d, '%s', %d, %d, %d, %d, %d,'%s', '%s', '%s','%s')", 
				T_CUSER, DbStr(cuserId), DbStr(cInfo.GetUserName()),
				DbStr(cInfo.GetUserPwd()), cInfo.GetUserType(), DbStr(cInfo.GetUsergrpID()), cUserPrivi.Convert2Int(),
				cInfo.GetMLoginFlag(), cInfo.GetEnabledFlag(), DbStr(cInfo.GetDescription()), DbDateTime(cInfo.GetNameExpireDate()),
				DbDateTime(cInfo.GetPwdExpireDate()), 90, cInfo.GetPwdExpireEnabledFlag(), DbStr(cInfo.GetLoginIpList()), cInfo.GetIpListEnabledFlag(), 
				cInfo.GetIpBindFlagForMobileClient(), nSwitchNum, cInfo.GetWaterMarkEnabled(),DbStr(cInfo.GetWaterMarkFont()), 
				cInfo.GetWaterMarkFontSize(), cInfo.GetWaterMarkRotationAngle(), cInfo.GetWaterMarkColor(),cInfo.GetWaterMarkTransparent(),
				cInfo.GetWaterMarkTiled(),DbNowDateTime(), DbStr(cInfo.GetCreatorID()), DbStr(strDevTreeVer),"");
		}
        if (ExecSql(tSql, enExecNoRet))
        {
            cRsp.SetUserID(cuserId);
            bHasNtf = GetCuserInfo(cuserId, cNtf.GetUserInfo());
            cNtf.GetUserInfo().SetUserPwd("");
        }
        else
        {
            cRsp.SetErrorCode(ERR_TAS_DBERROR);
            break;
        }

        if (!cInfo.GetUsergrpID().empty())
        {
            RedisSetUserUsergrpId(cuserId, cInfo.GetUsergrpID());
        }

        CCMSTime cNowTime;
        tSql.Format("insert into %s values('%s', '%s', '%s', %d, %d)", 
            T_DEVTREE, DbStr(cuserId), "favorites",
            DbStr(cNowTime.ToString()), 0, (s32)CDevTreeInfo::UserFavorites);
        if (!ExecSql(tSql, enExecNoRet))
        {
            cRsp.SetErrorCode(ERR_TAS_DBERROR);
            break;
        }

        tSql.Format(s_szDevGroupTableTmpl, DbStr(cuserId));
        if (!ExecSql(tSql, enExecNoRet))
        {
            cRsp.SetErrorCode(ERR_TAS_DBERROR);
            break;
        }
        tSql.Format(s_szDevGrp2DevTableTmpl, DbStr(cuserId));
        if (!ExecSql(tSql, enExecNoRet))
        {
            cRsp.SetErrorCode(ERR_TAS_DBERROR);
            break;
        }

        CDeviceGroupInfo cRootGrp;
        DevGrpItemQry(CurrentDomainId(), cRootGrp);		

        CDeviceGroupInfo cGrpInfo;
        cGrpInfo.SetDeviceGroupID(cuserId);
        cGrpInfo.SetDomainUUID(CurrentDomainId());
        cGrpInfo.SetDeviceGroupName(cRootGrp.GetDeviceGroupName());
        cGrpInfo.SetCreateDate(cNowTime.ToString());
        cRsp.SetErrorCode(DevGrpItemAdd(cGrpInfo, cuserId));
        if (CMS_SUCCESS != cRsp.GetErrorCode())
        {
            break;
        }
    } while (false);

    PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
    if (bHasNtf) PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cNtf, cTransNtf);
}

void CDbOpr::CuserDel( const CTaskMsgTransfer<CUserDelReq> *pcMsg )
{
    CTaskMsgTransfer<CUserDelRsp> cTrans;
    CTaskMsgTransfer<CUserDelNtfReq> cTransNtf;
    CUserDelRsp &cRsp = cTrans.GetMsgObj();
    CUserDelNtfReq &cNtf = cTransNtf.GetMsgObj();

    const CUserDelReq &cReq = pcMsg->GetMsgObj();
    CUUID cOprId;
    
    // 不允许删除超级管理员
    if (cReq.GetUserID() == CurrentDomainId())
    {
        cRsp.SetErrorCode(ERR_TAS_INVALID_OPERATION);
        PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
        return;
    }

    CUserInfo cUser;
    if (!GetCuserInfo(cReq.GetUserID(), cUser))
    {
        cRsp.SetErrorCode(ERR_TAS_RECORD_FETCH_FAILED);
        PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
        return;
    }

    g_cDbTaskPool.AddTransProgress(cOprId, 100, 5);
    cRsp.SetOperUuid(cOprId);
    PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);

    do 
    {
        cRsp.SetErrorCode(CuserDevDelByUserId(cUser.GetUserID()));
        if (cRsp.GetErrorCode() != CMS_SUCCESS) break;

        //这里需考虑同步删除redis中该用户创建的用户组信息。
        vector<UUIDString> vecCuserGrpIds;
        if (CMS_SUCCESS == GetCuserGrpIdsByCreatorId(cUser.GetUserID(), vecCuserGrpIds))
        {
            for (u32 dwindex = 0; dwindex < vecCuserGrpIds.size(); ++dwindex)
            {
                RedisDelUsergrp(vecCuserGrpIds[dwindex]);
            }
        }

        g_cDbTaskPool.AddTransProgress(cOprId, 60);

        TSqlString tSql;
        // 删除设备组表及设备组设备关系表
        tSql.Format("select 1 from %s where %s='%s' and %s=%d", T_DEVTREE, F_DEVTREE_ID, 
            DbStr(cReq.GetUserID()), F_DEVTREE_TYPE, (s32)CDevTreeInfo::UserFavorites);
        if (!ExecSql(tSql, enExecMayRet))
        {
            cRsp.SetErrorCode(ERR_TAS_DBERROR);
            break;
        }
        else if (IsNotEof())
        {
            tSql.Format("drop table %s",  DbStr(GetTblDevGrp2DevName(cReq.GetUserID())));
            if (!ExecSql(tSql, enExecNoRet))
            {
                cRsp.SetErrorCode(ERR_TAS_DBERROR);
                break;
            }
            tSql.Format("drop table %s",  DbStr(GetTblDevGrpName(cReq.GetUserID())));
            if (!ExecSql(tSql, enExecNoRet))
            {
                cRsp.SetErrorCode(ERR_TAS_DBERROR);
                break;
            }
        }

        tSql.Format("delete from %s where %s = '%s'", T_DEVTREE, F_DEVTREE_ID, DbStr(cReq.GetUserID()));
        if (!ExecSql(tSql, enExecNoRet))
        {
            cRsp.SetErrorCode(ERR_TAS_DBERROR);
            break;
        }

        tSql.Format("call %s('%s')", P_CUSERGRPDELBYCREATORID, DbStr(cUser.GetUserID()));
        if (!ExecSql(tSql, enExecNoRet))
        {
            cRsp.SetErrorCode(ERR_TAS_DBERROR);
            break;
        }

        g_cDbTaskPool.AddTransProgress(cOprId, 80);

        tSql.Format("update %s set %s = '%s' where %s = '%s'", T_CUSER, F_CUSER_USERCREATORID,
            DbStr(cUser.GetCreatorID()), F_CUSER_USERCREATORID, DbStr(cUser.GetUserID()));
        if (!ExecSql(tSql, enExecNoRet))
        {
            cRsp.SetErrorCode(ERR_TAS_DBERROR);
            break;
        }
        tSql.Format("delete from %s where %s = '%s'", T_CUSER, F_CUSER_CUSERID, DbStr(cReq.GetUserID()));
        if (ExecSql(tSql, enExecNoRet))
        {
            cNtf.SetUserID(cReq.GetUserID());

            // 删除用户后删除用户对应的自动更新组信息
            ClearUserAutoAssignDeviceGroup(cUser.GetUserID());

            // 删除用户，删除用户PTZ锁 [4/14/2016 pangubing]
            ClearUserPtzLockInfo(cUser.GetUserID());

            RedisDelUser(cUser.GetUserID());

            g_cDbTaskPool.AddTransProgress(cOprId, 100);
            PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cNtf, cTransNtf);
        }
        else
        {
            cRsp.SetErrorCode(ERR_TAS_DBERROR);
        }
    } while (false);

    if (cRsp.GetErrorCode() != CMS_SUCCESS)
    {
        g_cDbTaskPool.AddTransProgress(cOprId, -1, (ECMS_ERRORCODE)cRsp.GetErrorCode());
    }
}

void CDbOpr::CuserMod( const CTaskMsgTransfer<CUserModReq> *pcMsg )
{
    CTaskMsgTransfer<CUserModRsp> cTrans;
    CTaskMsgTransfer<CUserModNtfReq> cTransNtf;
    CUserModRsp &cRsp = cTrans.GetMsgObj();
    CUserModNtfReq &cNtf = cTransNtf.GetMsgObj();
    bool bHasNtf = false;
    
    const CUserModReq &cReq = pcMsg->GetMsgObj();

    do 
    {
        TSqlString tSql;
        UUIDString cUserId;
        CUserInfo cUserInfo;
        cReq.GetUserID(cUserId);

        if (!GetCuserInfo(cUserId, cUserInfo))
        {
            cRsp.SetErrorCode(ERR_TAS_USER_NOT_EXIST);
            break;
        }

        if (cReq.GetUserPwd(m_strFieldValue))
        {
            if (cReq.GetUserOldPwd(m_strFieldValue))
            {
                if (cUserInfo.GetUserPwd() != m_strFieldValue)
                {
                    cRsp.SetErrorCode(ERR_OLD_PASSWORD_INCORRECT);
                    break;
                }
            }
			cReq.GetUserPwd(m_strFieldValue);
			if (cUserInfo.GetUserPwd() == m_strFieldValue)//新旧密码不能相同
			{
				cRsp.SetErrorCode(ERR_OLD_PASSWORD_EQUAL_NEW_PASSWORD);
				break;
			}
        }
		
        // 超级管理员的一些属性禁止修改
		/* if (cUserId == CurrentDomainId())
		{
		string tmpstr;
		int tmpint;
		bool tmpbool;
		if (cReq.GetUsergrpID(tmpstr) || cReq.GetUserName(tmpstr) ||
		cReq.GetEnabledFlag(tmpbool) || cReq.GetUserType(tmpint))
		{
		cRsp.SetErrorCode(ERR_TAS_INVALID_OPERATION);
		break;
		}
		}*/
		string strNewCuserGrpId;
		tSql.Format("update %s set ", T_CUSER);
		//只能修改超级管理员的密码有效期、ip地址绑定功能，此处做限制
		if (cUserId != CurrentDomainId())
		{
			s32 nUserType;
			if (cReq.GetUserType(nUserType))
			{
				/* if (CUSER_TYPE_SUPERADMIN == nUserType)
				{
				cRsp.SetErrorCode(ERR_TAS_INVALID_OPERATION);
				break;
				}*/

				if (CUSER_TYPE_OPERATOR == nUserType)
				{
					tSql.Format("select count(*) from %s where %s = '%s'", T_CUSER,
						F_CUSER_USERCREATORID, DbStr(cUserId));
					if (ExecSql(tSql, enExecMustRet))
					{
						if (GetIntResult() > 0)
						{
							cRsp.SetErrorCode(ERR_TAS_USER_TYPE_DISABLE_CHANGE);
							break;
						}
					}
					else
					{
						cRsp.SetErrorCode(ERR_TAS_DBERROR);
						break;
					}
				}
			}

			CUserPrivilege cPrivi;
			if (cReq.GetUserPrivilege(cPrivi))
			{
				// 不属于任何组时才能修改用户权限, 之所以不判断和返回失败是为了帮助CUI(它与CU间的协议无法区分这个字段是否需要设置)
			}
			else
			{
				// 未设置时使用原来的权限
				cPrivi = cUserInfo.GetUserPrivilege();
			}

			int nSwitchNum = -1;
			if (!cReq.GetSwitchNum(nSwitchNum))
			{
				nSwitchNum = cUserInfo.GetSwitchNum();
			}
			bool bWaterMarkEnabled = false;                                  
			string strWaterMarkFont;                                    
			int nWaterMarkFontSize = 72;                                   
			int nWaterMarkRotationAngle = 30;                            
			int nWaterMarkColor;                                    
			int nWaterMarkTransparent = 30;                                
			bool bWaterMarkTiled = true;

			if (!cReq.GetWaterMarkEnabled(bWaterMarkEnabled))
			{
				bWaterMarkEnabled = cUserInfo.GetWaterMarkEnabled();
			}
			if (!cReq.GetWaterMarkFont(strWaterMarkFont))
			{
				strWaterMarkFont = cUserInfo.GetWaterMarkFont();
			}
			if (!cReq.GetWaterMarkFontSize(nWaterMarkFontSize))
			{
				nWaterMarkFontSize = cUserInfo.GetWaterMarkFontSize();
			}
			if (!cReq.GetWaterMarkRotationAngle(nWaterMarkRotationAngle))
			{
				nWaterMarkRotationAngle = cUserInfo.GetWaterMarkRotationAngle();
			}
			if (!cReq.GetWaterMarkColor(nWaterMarkColor))
			{
				nWaterMarkColor = cUserInfo.GetWaterMarkColor();
			}
			if (!cReq.GetWaterMarkTransparent(nWaterMarkTransparent))
			{
				nWaterMarkTransparent = cUserInfo.GetWaterMarkTransparent();
			}
			if (!cReq.GetWaterMarkTiled(bWaterMarkTiled))
			{
				bWaterMarkTiled = cUserInfo.GetWaterMarkTiled();
			}

			if (cReq.GetUsergrpID(strNewCuserGrpId) && !strNewCuserGrpId.empty()) // 修改组
			{
				CUserGroupInfo cUserGrpInfo;
				if (GetCuserGrpInfo(strNewCuserGrpId, cUserGrpInfo))
				{
					if (!cReq.GetUserType(nUserType))
					{
						nUserType = cUserInfo.GetUserType();
					}

					if (nUserType != cUserGrpInfo.GetUserType())
					{
						cRsp.SetErrorCode(ERR_TAS_INVALID_OPERATION);
						break;
					}
				}
				else
				{
					cRsp.SetErrorCode(ERR_TAS_RECORD_FETCH_FAILED);
					break;
				}

				cPrivi = cUserGrpInfo.GetUserPrivilege(); // 用户权限与组权限一致
				nSwitchNum = cUserGrpInfo.GetSwitchNum();
				bWaterMarkEnabled = cUserGrpInfo.GetWaterMarkEnabled();                                  
				strWaterMarkFont = cUserGrpInfo.GetWaterMarkFont();                                    
				nWaterMarkFontSize = cUserGrpInfo.GetWaterMarkFontSize();
				nWaterMarkRotationAngle = cUserGrpInfo.GetWaterMarkRotationAngle();                            
				nWaterMarkColor = cUserGrpInfo.GetWaterMarkColor();                                    
				nWaterMarkTransparent = cUserGrpInfo.GetWaterMarkTransparent();                               
				bWaterMarkTiled = cUserGrpInfo.GetWaterMarkTiled();
			}

			if (!cUserInfo.GetUsergrpID().empty() && strNewCuserGrpId.empty())
			{
				// 退组时保留退组前的权限
				tSql.Format("call %s('%s', '%s')", P_COPYCUSERGRPPRIVILEGE, DbStr(cUserInfo.GetUserID()),
					DbStr(cUserInfo.GetUsergrpID()));
				if (!ExecSql(tSql, enExecNoRet))
				{
					cRsp.SetErrorCode(ERR_TAS_DBERROR);
					break;
				}
			}

			if (cUserInfo.GetUsergrpID().empty() && !strNewCuserGrpId.empty())
			{
				// 加入新组时，需要清除用户原有的自动更新组信息
				ClearUserAutoAssignDeviceGroup(cUserId);
			}

			tSql.Format("update %s set ", T_CUSER);
			AppendUpdateValuePair(cReq.GetUsergrpID, m_strFieldValue, tSql, F_CUSER_USERGROUPID);
			AppendUpdateValuePair(cReq.GetUserName, m_strFieldValue, tSql, F_CUSER_USERNAME);
			AppendUpdateValuePair(cReq.GetEnabledFlag, m_bFieldValue, tSql, F_CUSER_USEFLAG);		
			AppendUpdateValuePair(cReq.GetUserType, m_nFieldValue, tSql, F_CUSER_CUSERTYPE);			
			AppendUpdateValuePair(cReq.GetEnabledFlag, m_bFieldValue, tSql, F_CUSER_USEFLAG);
			AppendUpdateValuePair(cReq.GetDescription, m_strFieldValue, tSql, F_CUSER_USERDESC);
			AppendUpdateDateValuePair(cReq.GetNameExpireDate, m_strFieldValue, tSql, F_CUSER_USEREXPIREDATE);
			tSql.AppendString("%s=%d,", F_CUSER_CUSERPRIVILEGE, (s32)cPrivi.Convert2Int());
			tSql.AppendString("%s=%d,", F_CUSER_SWITCHNUM, nSwitchNum);
			tSql.AppendString("%s=%d,", F_CUSER_WATERMARK_ENABLED, bWaterMarkEnabled);
			tSql.AppendString("%s='%s',", F_CUSER_WATERMARK_FONT, DbStr(strWaterMarkFont));
			tSql.AppendString("%s='%d',", F_CUSER_WATERMARK_FONT_SIZE, nWaterMarkFontSize);
			tSql.AppendString("%s='%d',", F_CUSER_WATERMARK_ROTATION_ANGLE, nWaterMarkRotationAngle);
			tSql.AppendString("%s='%d',", F_CUSER_WATERMARK_COLOR, nWaterMarkColor);
			tSql.AppendString("%s='%d',", F_CUSER_WATERMARK_TRANSPARENT, nWaterMarkTransparent);
			tSql.AppendString("%s='%d',", F_CUSER_WATERMARK_TILED, bWaterMarkTiled);
			
		}
        AppendUpdateValuePair(cReq.GetMLoginFlag, m_bFieldValue, tSql, F_CUSER_MULTILOGIN);
		AppendUpdateValuePair(cReq.GetUserPwd, m_strFieldValue, tSql, F_CUSER_USERPASSWORD);
		//普通用户只有修改密码的权限，且在修改的消息中不带用户名字段，超级管理员修改其他用户带有用户名
		if (!cReq.GetUserName(m_strFieldValue))
		{
			if (cUserInfo.GetPwdExpireEnabledFlag())
			{
				AppendFieldDatetimeValuePairImp(tSql, F_CUSER_PASSWORDEXPIREDATE, (CCMSTime() + cUserInfo.GetPwdExpireTime() * 60 * 60 * 24).ToString()); 
				tSql.AppendString(",");
			}
		}
		else
		{
			//判断密码有效期是否有做修改
			if (cReq.GetPwdExpireEnabledFlag(m_bFieldValue))
			{
				if (m_bFieldValue)
				{
					//密码如果有修改，且有效期启用了
					if (cReq.GetUserPwd(m_strFieldValue))
					{
						if (cReq.GetPwdExpireTime(m_nFieldValue))
						{
							AppendFieldDatetimeValuePairImp(tSql, F_CUSER_PASSWORDEXPIREDATE, (CCMSTime() + m_nFieldValue * 60 * 60 * 24).ToString()); 
							tSql.AppendString(",");
						}
						else
						{
							AppendFieldDatetimeValuePairImp(tSql, F_CUSER_PASSWORDEXPIREDATE, (CCMSTime() + cUserInfo.GetPwdExpireTime() * 60 * 60 * 24).ToString()); 
							tSql.AppendString(",");
						}
					}
					else
					{
						if (cReq.GetPwdExpireTime(m_nFieldValue))
						{
							if (cUserInfo.GetPwdExpireEnabledFlag())
							{
								m_nFieldValue = m_nFieldValue - cUserInfo.GetPwdExpireTime();
								AppendFieldDatetimeValuePairImp(tSql, F_CUSER_PASSWORDEXPIREDATE, (CCMSTime(cUserInfo.GetPwdExpireDate()) +  m_nFieldValue * 60 * 60 * 24).ToString());
								tSql.AppendString(",");
							}
							else
							{
								AppendFieldDatetimeValuePairImp(tSql, F_CUSER_PASSWORDEXPIREDATE, (CCMSTime() + m_nFieldValue * 60 * 60 * 24).ToString());
								tSql.AppendString(",");
							}
						}
					}

					AppendUpdateValuePair(cReq.GetPwdExpireTime, m_nFieldValue, tSql, F_CUSER_PASSWORDEXPIRETIME);
				}
				else
				{
					if (cReq.GetPwdExpireDate(m_strFieldValue))
					{
						AppendFieldDatetimeValuePairImp(tSql, F_CUSER_PASSWORDEXPIREDATE, m_strFieldValue); 
						tSql.AppendString(",");
					}
				}

			}
			else
			{
				//没有携带有效期是否启用字段，但密码修改了，且之前有效期启用了，则在原来基础上顺延
				do 
				{
					if (cReq.GetUserPwd(m_strFieldValue))
					{
						if (cUserInfo.GetPwdExpireEnabledFlag())
						{
							AppendFieldDatetimeValuePairImp(tSql, F_CUSER_PASSWORDEXPIREDATE, (CCMSTime() + cUserInfo.GetPwdExpireTime() * 60 * 60 * 24).ToString()); 
							tSql.AppendString(",");
							break;
						}			
					}
					if (cReq.GetPwdExpireDate(m_strFieldValue))
					{
						AppendFieldDatetimeValuePairImp(tSql, F_CUSER_PASSWORDEXPIREDATE, m_strFieldValue); 
						tSql.AppendString(",");
					}
				} while (0);

			}
			AppendUpdateValuePair(cReq.GetPwdExpireEnabledFlag, m_bFieldValue, tSql, F_CUSER_PASSWORDEXPIREENABLEDFLAG);
			AppendUpdateValuePair(cReq.GetLoginIpList, m_strFieldValue, tSql, F_CUSER_LOGINIPLIST);
			AppendUpdateValuePair(cReq.GetIpListEnabledFlag, m_bFieldValue, tSql, F_CUSER_IPLISTENABLEDFLAG);
			AppendUpdateValuePair(cReq.GetIpBindFlagForMobileClient, m_bFieldValue, tSql, F_CUSER_IPBINDFLAGFORMOBILECLIENT);
		}
      
        
        tSql.EraseLastRedundantComma();

        tSql.AppendString(" where %s = '%s'", F_CUSER_CUSERID, DbStr(cUserId));
        if (ExecSql(tSql, enExecNoRet))
        {
            bHasNtf = GetCuserInfo(cUserId, cNtf.GetUserInfo());
            cNtf.GetUserInfo().SetUserPwd("");
        }
        else
        {
            cRsp.SetErrorCode(ERR_TAS_DBERROR);
        }
		//超级管理员属性作限制
		if (cUserId != CurrentDomainId())
		{
			if (!strNewCuserGrpId.empty())
			{
				// 处理PTZ锁变更 [4/15/2016 pangubing]
				vector<CPtzLockInfo> cPtzLockInfos;
				QryUserPtzLockInfo(cUserId,cPtzLockInfos);
				for (size_t i=0;i<cPtzLockInfos.size();i++)
				{
					CVideosourcePrivilege cNewDevCapIndexPrivilege;
					// 新组没有设备权限或者设备没有PTZ锁权限 [4/15/2016 pangubing]
					if (!GetUserDevCapInfo(cNtf.GetUserInfo(),cPtzLockInfos[i].GetDeviceId(),cPtzLockInfos[i].GetVideoSrcId(),cNewDevCapIndexPrivilege)
						||!cNewDevCapIndexPrivilege.HasPrivilege(CVideosourcePrivilege::en_PTZ_Lock))
					{
						DeletePtzLockInner(pcMsg,cPtzLockInfos[i].GetDeviceId(),cPtzLockInfos[i].GetVideoSrcId());
					}
				}
			}

			if (cReq.GetUsergrpID(strNewCuserGrpId))
			{
				//get成功说明使用者要修改该用户的用户组属性
				RedisSetUserUsergrpId(cUserId, strNewCuserGrpId);
			}
			
		}
		
    } while (false);

    PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
    if (bHasNtf) PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cNtf, cTransNtf);
}

void CDbOpr::CuserQry( const CTaskMsgTransfer<CUserQryReq> *pcMsg )
{
    DatabaseQueryTemplate<CUserQryReq, CUserQryRsp, CUserQryNtfReq, CUserInfo>(
        pcMsg, &CDbOpr::PrepareCuserQrySql, &CDbOpr::FetchCuserQryRecord, 
        &CUserQryNtfReq::GetUserInfo, &CUserInfo::GetUserID);
}

string CDbOpr::GetNewUserGBID()
{
	string domainGBId;
	bool result = this->GetLocalDomainGbId(domainGBId);

	// 用户ID. 11-13 300-399表示类型为中心用户 400 - 499表示类型为终端用户
	if (domainGBId.size() == 20)
	{
		domainGBId[10] = '4';
		domainGBId[11] = '0';
		domainGBId[12] = '0';
	}
	else
	{
		return CUUID();
	}

	// 14－20	设备、用户序号
	string userSeq = "0000001";
	string sql = "SELECT cuserId, length(cuserId) FROM KDMAAA.tblCuser where length(cuserId) = 20 order by cuserId DESC limit 1;";
	if (ExecSql( sql.c_str(), enExecMayRet))
	{
		int curSeq = 0;
		if (IsNotEof())
		{
			string maxUserId = GetString("cuserId");
			string strCurSeq = maxUserId.substr(13, 19);
			curSeq = atoi(strCurSeq.c_str());
			curSeq++;
			char achTmp[32] = { 0 };
			sprintf(achTmp, "%07d", curSeq);
			userSeq = achTmp;
		}
	}
	else
	{
	}
	string userGbId = domainGBId.substr(0, 13) + userSeq;
	return userGbId;
}

ENTasErrno CDbOpr::PrepareCuserQrySql(const CUserQryReq &cReq, TSqlString &tSqlSelect, TSqlString &tSqlCount)
{
    const CUserInfoQueryCondition &cCond = cReq.GetUserInfoQueryCondition();

    UUIDString cuserId;
    if (cCond.GetCuserID(cuserId))
    {
        // 查询指定CU用户所能看到的用户列表 树型数据提取显示
        ENDBType enDbType = GetEngineDBType();
        if (enDBTypeMysql == enDbType)
        {
            tSqlSelect.Format("call %s('%s', %d, %d)", P_CUSERQRY, DbStr(cuserId), cReq.GetStartEntryIndex(), cReq.GetExpectEntryNum());
            tSqlCount.Format("call %s('%s')", P_CUSERQRY4COUNT, DbStr(cuserId));
        }
        else
        {
            CMS_ASSERT(false);
        }
    }
    else
    {
        TSqlString tSqlCond;
        TSqlString tSqlOrderby;
        tSqlSelect.Format("select a.* from %s a", T_CUSER);
        tSqlCount.Format("select count(*) from %s a", T_CUSER);

        AppendQryCondValuePair(cCond.GetUserID, m_strFieldValue, tSqlCond, F_CUSER_CUSERID, a);
        AppendQryCondValuePair(cCond.GetUserName, m_strFieldValue, tSqlCond, F_CUSER_USERNAME, a);
        AppendQryCondValuePair(cCond.GetUserPwd, m_strFieldValue, tSqlCond, F_CUSER_USERPASSWORD, a);
        AppendQryCondValuePair(cCond.GetUserType, m_nFieldValue, tSqlCond, F_CUSER_CUSERTYPE, a);
        AppendQryCondValuePair(cCond.GetMLoginFlag, m_bFieldValue, tSqlCond, F_CUSER_MULTILOGIN, a);
        AppendQryCondValuePair(cCond.GetEnabledFlag, m_bFieldValue, tSqlCond, F_CUSER_USEFLAG, a);
        AppendQryCondValuePair(cCond.GetDescription, m_strFieldValue, tSqlCond, F_CUSER_USERDESC, a);
        AppendQryCondValuePair(cCond.GetCreatorNO, m_strFieldValue, tSqlCond, F_CUSER_USERCREATORID, a);
		AppendQryCondValuePair(cCond.GetPwdExpireTime, m_nFieldValue, tSqlCond, F_CUSER_PASSWORDEXPIRETIME, a);
		AppendQryCondValuePair(cCond.GetPwdExpireEnableFlag, m_bFieldValue, tSqlCond, F_CUSER_PASSWORDEXPIREENABLEDFLAG, a);
		AppendQryCondValuePair(cCond.GetLoginIpList, m_strFieldValue, tSqlCond, F_CUSER_LOGINIPLIST, a);
        AppendQryCondDatePeriodPair(cCond.GetNameExpireDate_From, m_strFieldValue, cCond.GetNameExpireDate_To, m_strFieldValue2,
            tSqlCond, F_CUSER_USEREXPIREDATE, a);
        AppendQryCondDatePeriodPair(cCond.GetPwdExpireDate_From, m_strFieldValue, cCond.GetPwdExpireDate_To, m_strFieldValue2,
            tSqlCond, F_CUSER_PASSWORDEXPIREDATE, a);
        AppendQryCondDatePeriodPair(cCond.GetCreateDate_From, m_strFieldValue, cCond.GetCreateDate_To, m_strFieldValue2,
            tSqlCond, F_CUSER_USERCREATEDATE, a);
        tSqlCond.EraseLastRedundantAnd();

        if (!tSqlCond.Empty())
        {
            tSqlSelect.AppendString(" where ");
            tSqlSelect.AppendString("%s", tSqlCond.GetBuf());
            tSqlCount.AppendString(" where ");
            tSqlCount.AppendString("%s", tSqlCond.GetBuf());
        }

        tSqlSelect.AppendString(" order by a.%s ", F_CUSER_CUSERID);
        AddRowNumLimitModifer(tSqlSelect, cReq.GetStartEntryIndex(), cReq.GetExpectEntryNum());
    }

    return CMS_SUCCESS;
}



void CDbOpr::FetchCuserQryRecord( CUserInfo &cInfo )
{
    cInfo.SetUserID(GetString(F_CUSER_CUSERID));
    cInfo.SetUsergrpID(GetString(F_CUSER_USERGROUPID));
    cInfo.SetUserName(GetString(F_CUSER_USERNAME));
    cInfo.SetUserPwd(GetString(F_CUSER_USERPASSWORD));
    cInfo.SetUserType(GetInt(F_CUSER_CUSERTYPE));
    cInfo.SetMLoginFlag(GetBool(F_CUSER_MULTILOGIN));
    cInfo.SetEnabledFlag(GetBool(F_CUSER_USEFLAG));
    cInfo.SetDescription(GetString(F_CUSER_USERDESC));
    cInfo.SetNameExpireDate(GetCmsDate(F_CUSER_USEREXPIREDATE));
    cInfo.SetPwdExpireDate(GetCmsDate(F_CUSER_PASSWORDEXPIREDATE));
	cInfo.SetPwdExpireTime(GetInt(F_CUSER_PASSWORDEXPIRETIME));
	cInfo.SetPwdExpireEnabledFlag(GetBool(F_CUSER_PASSWORDEXPIREENABLEDFLAG));
	cInfo.SetLoginIpList(GetString(F_CUSER_LOGINIPLIST));
	cInfo.SetIpListEnabledFlag(GetBool(F_CUSER_IPLISTENABLEDFLAG));
	cInfo.SetIpBindFlagForMobileClient(GetBool(F_CUSER_IPBINDFLAGFORMOBILECLIENT));
    cInfo.SetCreateDate(GetCmsDate(F_CUSER_USERCREATEDATE));
    cInfo.SetCreatorID(GetString(F_CUSER_USERCREATORID));
    cInfo.GetUserPrivilege().ConvertFromInt(GetInt(F_CUSER_CUSERPRIVILEGE));
    cInfo.SetDeviceTreeVer(GetString(F_CUSER_DEVTREEVER));
	cInfo.SetSwitchNum(GetInt(F_CUSER_SWITCHNUM));
	cInfo.SetWaterMarkEnabled(GetBool(F_CUSER_WATERMARK_ENABLED));
	cInfo.SetWaterMarkFont(GetString(F_CUSER_WATERMARK_FONT));
	cInfo.SetWaterMarkFontSize(GetInt(F_CUSER_WATERMARK_FONT_SIZE));
	cInfo.SetWaterMarkRotationAngle(GetInt(F_CUSER_WATERMARK_ROTATION_ANGLE));
	cInfo.SetWaterMarkColor(GetInt(F_CUSER_WATERMARK_COLOR));
	cInfo.SetWaterMarkTransparent(GetInt(F_CUSER_WATERMARK_TRANSPARENT));
	cInfo.SetWaterMarkTiled(GetBool(F_CUSER_WATERMARK_TILED));

}

ENTasErrno CDbOpr::DeviceItemQry( const UUIDString &strId, CDeviceInfo &cInfo )
{
    TSqlString tSql;
    tSql.Format("select * from %s where %s='%s'", T_DEVICE, F_DEVICE_DEVID, DbStr(strId));
    
    if (ExecSql(tSql, enExecMayRet))
    {
        if (IsNotEof())
        {
            return FetchDeviceQryRecord(cInfo);
        }
        else
        {
            return ERR_TAS_RECORD_NOT_EXISTS;
        }
    }
    else
    {
        return ERR_TAS_DBERROR;
    }
}

ENTasErrno CDbOpr::DeviceItemAdd( const CDeviceInfo &cDev )
{
	string strDevData;
	const TDevExtData &tDevExtData = cDev.GetDevData();
	if (!tDevExtData.GetExtData().empty())
	{
		strDevData = tDevExtData.ToXml();
		if (DEVICE_DEVDATA_LENGTH <= strDevData.length())
		{
			return ERR_TAS_EXTDATA_TOO_LONG;
		}
	}


    TSqlString tSql;
    tSql.AppendString("insert into %s values(", T_DEVICE);
    tSql.AppendString("'%s', '%s', '%s', '%s',", DbStr(cDev.GetDeviceUUID()), DbStr(cDev.GetDomainUUID()), DbStr(cDev.GetDeviceName()), DbStr(cDev.GetDeviceAlias()));
    tSql.AppendString("%d, '%s', %d,", cDev.GetDeviceType(), DbStr(cDev.GetDeviceModel()), (s32)cDev.GetDevicePropertyFlags().Convert2Int());
    tSql.AppendString("%d, %d, '%s',", (s32)cDev.GetUsedFlag(), cDev.GetNetType(), DbStr(cDev.GetNetAddress()));
    tSql.AppendString("'%s', '%s', '%s', %d, ", DbStr(cDev.GetDescription()), DbDateTime(cDev.GetCreateDate()), DbDateTime(cDev.GetExpireDate()), (s32)cDev.GetCreater());
    tSql.AppendString("'%s', '%s')", DbStr(strDevData), DbStr(cDev.GetOwner()));

    if (ExecSql(tSql, enExecNoRet))
    {
        CDeviceModelInfo cModel=g_cDbCache.GetDeviceModelInfo(cDev.GetDeviceModel(), this);
        int videoSourceNum=0;
        for (vector<TDeviceCapInfo>::const_iterator it = cModel.GetDeviceCapInfo().begin(); it != cModel.GetDeviceCapInfo().end(); ++it)
        {
            if (it->type == DEVICE_CAP_VIDEOSOURCE)
            {
                videoSourceNum=it->num;
                break;
            }
        }
        if(videoSourceNum>0){
            ENTasErrno enRet= DevicePinyinSearchVideoNameInit(cDev.GetDeviceUUID(), videoSourceNum, 0, cDev.GetDeviceName().c_str());
            if(enRet != CMS_SUCCESS) {
                tSql.Format("delete from %s where %s = '%s'", T_DEVICE, F_DEVICE_DEVID, DbStr(cDev.GetDeviceUUID()));
                ExecSql(tSql, enExecNoRet);
                return enRet;
            }
        }

        return CMS_SUCCESS;
    }
    else
    { 
        CDeviceInfo cDevInfo;
        if (CMS_SUCCESS == DeviceItemQry(cDev.GetDeviceUUID(), cDevInfo))
        {
            return ERR_TAS_RECORD_EXISTS;
        }
        else
        {
            return ERR_TAS_DBERROR;
        }
    }
}

ENTasErrno CDbOpr::DeviceItemDel( const UUIDString &strDevId )
{
    ENTasErrno enRet=DevicePinyinSearchVideoNameDel(strDevId);
    if(enRet != CMS_SUCCESS) return enRet;

    TSqlString tSql;
    tSql.Format("delete from %s where %s = '%s'", T_DEVICE, F_DEVICE_DEVID, DbStr(strDevId));
    if (ExecSql(tSql, enExecNoRet))
    {
        return CMS_SUCCESS;
    }
    else
    {
        return ERR_TAS_DBERROR;
    }
}

ENTasErrno CDbOpr::DeviceItemMod( const CDeviceInfo &cInfo )
{


	string strDevData;
	const TDevExtData &tDevExtData = cInfo.GetDevData();
	if (!tDevExtData.GetExtData().empty())
	{
		strDevData = tDevExtData.ToXml();
		if (DEVICE_DEVDATA_LENGTH <= strDevData.length())
		{
			return ERR_TAS_EXTDATA_TOO_LONG;
		}
	}

    TSqlString tSql;
    tSql.Format("update %s set %s='%s', %s='%s', %s='%s', %s=%d, \
        %s='%s', %s=%d, %s=%d, %s=%d, %s='%s', %s='%s', %s='%s', %s='%s', %s='%s' \
        where %s='%s'", T_DEVICE, 
        F_DEVICE_DOMAINID, DbStr(cInfo.GetDomainUUID()), F_DEVICE_DEVNAME, DbStr(cInfo.GetDeviceName()),
        F_DEVICE_DEVALIAS, DbStr(cInfo.GetDeviceAlias()), F_DEVICE_DEVTYPE, cInfo.GetDeviceType(),
        F_DEVICE_DEVMODELID, DbStr(cInfo.GetDeviceModel()), F_DEVICE_DEVPROPERTYFLAGS, cInfo.GetDevicePropertyFlags().Convert2Int(),
        F_DEVICE_USEFLAG, cInfo.GetUsedFlag(), F_DEVICE_NETTYPE, cInfo.GetNetType(), 
        F_DEVICE_NETADDR, DbStr(cInfo.GetNetAddress()), F_DEVICE_DEVDESC, DbStr(cInfo.GetDescription()),
        F_DEVICE_CREATEDATE, DbDateTime(cInfo.GetCreateDate()), F_DEVICE_EXPIREDATE, DbDateTime(cInfo.GetExpireDate()),
		F_DEVICE_DEVDATA, DbStr(strDevData), 
        F_DEVICE_DEVID, DbStr(cInfo.GetDeviceUUID()));

    if (ExecSql(tSql, enExecNoRet))
    {
        return CMS_SUCCESS;
    }
    else
    {
        return ERR_TAS_DBERROR;
    }
}


void CDbOpr::TvwallAdd( const CTaskMsgTransfer<CTvwallAddReq> *pcMsg )
{
    CTaskMsgTransfer<CTvwallAddRsp> cTrans;
    CTaskMsgTransfer<CTvwallAddNtfReq> cTransNtf;
    CTvwallAddRsp &cRsp = cTrans.GetMsgObj();
    CTvwallAddNtfReq &cNtf = cTransNtf.GetMsgObj();
    bool bHasNtf = false;

    const CTvwallAddReq &cReq = pcMsg->GetMsgObj();
    CTvwallInfo cDev = cReq.GetTvwallInfo();

    TSqlString tSql;
    CUUID cDevId;

    do 
    {
        cRsp.SetErrorCode(ValidateDecoderTvBinder(cReq.GetTvwallInfo().GetTvDecoderBinders(), NULL));
        if (cRsp.GetErrorCode() != CMS_SUCCESS)
        {
            break;
        }

        if (IsExistSameNameTvwall(cReq.GetTvwallInfo().GetDeviceName()))
        {
            cRsp.SetErrorCode(ERR_TAS_EXIST_SAMENAME_RECORD);
            break;
        }

        cDev.SetDeviceUUID(cDevId);
        cDev.SetDomainUUID(CurrentDomainId());
        cDev.SetDeviceModel(CurrentDomainId()); // 电视墙型号编号
        cDev.SetDeviceType(DEVICE_TYPE_TV_WALL);
        CCMSTime cDateNow;
        cDev.SetCreateDate(cDateNow.ToString());
        if (cDev.GetExpireDate().empty())
        {
            cRsp.SetErrorCode(ERR_TAS_INVALID_EXPIRE_DATE);
            break;
        }
        
        cRsp.SetErrorCode(TvwallItemAdd(cDev));
        if (CMS_SUCCESS != cRsp.GetErrorCode())
        {
            break;
        }

        UpdateCuserDevTreeVer("");

        cRsp.SetErrorCode(TvwallItemQry(cDevId, cNtf.GetTvwallInfo()));
        if (CMS_SUCCESS != cRsp.GetErrorCode())
        {
            break;
        }

        cRsp.SetTvwallId(cDevId);

        bHasNtf = true;
    } while (false);

    if (cRsp.GetErrorCode() != CMS_SUCCESS)
    {
        TvwallDelImp(cDevId);
        bHasNtf = false; // 失败时取消动态通知消息
    }

    PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
    if (bHasNtf)
    {
        PostTaskMsgTransfer2CCtrl4LargeNtf(CTvwallAddNtfReq, GetTvwallInfo, GetTvDecoderBinders)
    }
}

void CDbOpr::TvwallDel( const CTaskMsgTransfer<CTvwallDelReq> *pcMsg )
{
    CTaskMsgTransfer<CTvwallDelRsp> cTrans;
    CTaskMsgTransfer<CTvwallDelNtfReq> cTransNtf;
    CTvwallDelRsp &cRsp = cTrans.GetMsgObj();
    CTvwallDelNtfReq &cNtf = cTransNtf.GetMsgObj();
    bool bHasNtf = false;

    const CTvwallDelReq &cReq = pcMsg->GetMsgObj();

    do 
    {
        CTvwallInfo cInfo;

        ENTasErrno enErrno = TvwallItemQry(cReq.GetTvwallId(), cInfo);
        if (CMS_SUCCESS != enErrno)
        {
            cRsp.SetErrorCode(enErrno);
            break;
        }

        if (cInfo.GetDeviceType() != DEVICE_TYPE_TV_WALL)
        {
            cRsp.SetErrorCode(ERR_TAS_INVALID_PARM);
            break;
        }

        UpdateDevTreeVer(cReq.GetTvwallId());

        enErrno = TvwallDelImp(cReq.GetTvwallId());
        if (CMS_SUCCESS != enErrno)
        {
            cRsp.SetErrorCode(enErrno);
            break;
        }

        bHasNtf = true;
		cNtf.SetDomainId(cInfo.GetDomainUUID());
        cNtf.SetTvwallId(cReq.GetTvwallId());
    } while (false);

    PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
    if (bHasNtf) PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cNtf, cTransNtf);
}


ENTasErrno CDbOpr::TvwallDelImp( const UUIDString &tvwallId )
{
    ENTasErrno enErrno;

    do 
    {
        enErrno = DevGrpDevDelByDevId(tvwallId);
        if (CMS_SUCCESS != enErrno) break;

        enErrno = CuserGrpDevDel(tvwallId);
        if (CMS_SUCCESS != enErrno) break;

        enErrno = CuserDevDelByDevId(tvwallId);
        if (CMS_SUCCESS != enErrno) break;

        enErrno = TvwallItemDel(tvwallId);
        if (CMS_SUCCESS != enErrno) break;
    } while (false);

    return enErrno;
}


void CDbOpr::TvwallMod( const CTaskMsgTransfer<CTvwallModReq> *pcMsg ,bool bUseInner/*=false*/)
{
    CTaskMsgTransfer<CTvwallModRsp> cTrans;
    CTaskMsgTransfer<CTvwallModNtfReq> cTransNtf;
    CTvwallModRsp &cRsp = cTrans.GetMsgObj();
    CTvwallModNtfReq &cNtf = cTransNtf.GetMsgObj();
    bool bHasNtf = false;

    const CTvwallModReq &cReq = pcMsg->GetMsgObj();

    do 
    {
        UUIDString cTvwallId;
        CTvwallInfo cInfo;

        if (!cReq.GetId(cTvwallId))
        {
            cRsp.SetErrorCode(ERR_TAS_INVALID_PARM);
            break;
        }

        cRsp.SetErrorCode(TvwallItemQry(cTvwallId, cInfo));
        if (CMS_SUCCESS != cRsp.GetErrorCode())
        {
            break;
        }

        string name;
        int tvNum;    
        int style;       
        string customStyleData;
        string desc;
        vector<TvDecoderBinder> tvDecoderBinders;

        if (cReq.GetName(name))
        {
            if (IsExistSameNameTvwall(name, &cTvwallId))
            {
                cRsp.SetErrorCode(ERR_TAS_EXIST_SAMENAME_RECORD);
                break;
            }

            cInfo.SetDeviceName(name);
        }
        if (cReq.GetDesc(desc))
        {
            cInfo.SetDescription(desc);
        }
        if (cReq.GetTvNum(tvNum))
        {
            cInfo.SetTvNum(tvNum);
        }
        if (cReq.GetStyle(style))
        {
            cInfo.SetTvwallStyle(style);
        }
        if (cReq.GetCustomStyleData(customStyleData))
        {
            cInfo.SetTvwallCustomStyleData(customStyleData);
        }
        if (cReq.GetTvDecoderBinders(tvDecoderBinders))
        {
            cInfo.SetTvDecoderBinders(tvDecoderBinders);

            cRsp.SetErrorCode(ValidateDecoderTvBinder(tvDecoderBinders, &cInfo.GetDeviceUUID()));
            if (cRsp.GetErrorCode() != CMS_SUCCESS)
            {
                break;
            }
        }

        UpdateDevTreeVer(cTvwallId);

        cRsp.SetErrorCode(TvwallItemMod(cInfo));
        if (cRsp.GetErrorCode() != CMS_SUCCESS)
        {
            break;
        }


        bHasNtf = true;
        cNtf.SetTvwallInfo(cInfo);
    } while (false);

	if (!bUseInner)
	{
		PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
	}
    
    if (bHasNtf)
    {
        PostTaskMsgTransfer2CCtrl4LargeNtf(CTvwallModNtfReq, GetTvwallInfo, GetTvDecoderBinders)
    }
}

void CDbOpr::TvwallQry( const CTaskMsgTransfer<CTvwallQryReq> *pcMsg )
{
     DatabaseQueryTemplate<CTvwallQryReq, CTvwallQryRsp, CTvwallQryNtfReq, CTvwallInfo, TvDecoderBinder>(
        pcMsg, &CDbOpr::PrepareTvwallQrySql, &CDbOpr::FetchTvwallQryRecord, &CTvwallQryNtfReq::GetInfos, 
        &CTvwallInfo::GetTvDecoderBinders, &CTvwallInfo::GetDeviceUUID);
}


ENTasErrno CDbOpr::PrepareTvwallQrySql( const CTvwallQryReq &cReq, TSqlString &tSqlSelect, TSqlString &tSqlCount )
{
    const CTvwallInfoQueryCondition &cCond = cReq.GetCondition();

    if (cCond.GetCuserId(m_strFieldValue) && cCond.GetCuserGrpId(m_strFieldValue2))
    {
        return ERR_TAS_INVALID_PARM; // 不能同时指定两个
    }


    TSqlString tSqlCond;  

    if (cCond.GetTvwallId(m_strFieldValue))
    {
        string strDevId = m_strFieldValue;
        tSqlCond.AppendString("a.%s='%s'", F_V_TVWALLBASEINFO_DEVID, DbStr(strDevId));
        
        if (cCond.GetCuserId(m_strFieldValue))
        {
            string strUserId = m_strFieldValue;
            string strUserGrpId;
            if (!GetCuserGrpId(strUserId, strUserGrpId))
            {
                return ERR_TAS_USER_NOT_EXIST;
            }

            if (!strUserGrpId.empty())
            {
                tSqlCond.AppendString(" and exists(select * from %s c where c.%s='%s' and c.%s=a.%s)",
                    T_CUSERGRP2DEV, F_CUSERGRP2DEV_CUSERGROUPID, DbStr(strUserGrpId), 
                    F_CUSERGRP2DEV_DEVID, F_V_TVWALLBASEINFO_DEVID);
            }
            else
            {
                tSqlCond.AppendString(" and exists(select * from %s c where c.%s='%s' and c.%s=a.%s)",
                    T_CUSER2DEV, F_CUSER2DEV_CUSERID, DbStr(strUserId), 
                    F_CUSER2DEV_DEVID, F_V_TVWALLBASEINFO_DEVID);
            }
        }
        else if (cCond.GetCuserGrpId(m_strFieldValue))
        {
            tSqlCond.AppendString(" and exists(select * from %s c where c.%s='%s' and c.%s=a.%s)",
                T_CUSERGRP2DEV, F_CUSERGRP2DEV_CUSERGROUPID, DbStr(m_strFieldValue), 
                F_CUSERGRP2DEV_DEVID, F_V_TVWALLBASEINFO_DEVID);
        }
    }
    else
    {
        if (cCond.GetCuserId(m_strFieldValue))
        {
            string strUserId = m_strFieldValue;
            string strUserGrpId;
            if (!GetCuserGrpId(strUserId, strUserGrpId))
            {
                return ERR_TAS_USER_NOT_EXIST;
            }

            if (!strUserGrpId.empty())
            {
                tSqlCond.AppendString("exists(select * from %s c where c.%s='%s' and c.%s=a.%s)",
                    T_CUSERGRP2DEV, F_CUSERGRP2DEV_CUSERGROUPID, DbStr(strUserGrpId), 
                    F_CUSERGRP2DEV_DEVID, F_V_TVWALLBASEINFO_DEVID);
            }
            else
            {
                tSqlCond.AppendString("exists(select * from %s c where c.%s='%s' and c.%s=a.%s)",
                    T_CUSER2DEV, F_CUSER2DEV_CUSERID, DbStr(strUserId), 
                    F_CUSER2DEV_DEVID, F_V_TVWALLBASEINFO_DEVID);
            }
        }
        else if (cCond.GetCuserGrpId(m_strFieldValue))
        {
            tSqlCond.AppendString("exists(select * from %s c where c.%s='%s' and c.%s=a.%s)",
                T_CUSERGRP2DEV, F_CUSERGRP2DEV_CUSERGROUPID, DbStr(m_strFieldValue), 
                F_CUSERGRP2DEV_DEVID, F_V_TVWALLBASEINFO_DEVID);
        }
    }

    tSqlCount.Format("SELECT count(distinct a.%s) FROM %s a left join %s b on a.%s=b.%s",
        F_V_TVWALLBASEINFO_DEVID, V_TVWALLBASEINFO, T_TV2DECODER, F_V_TVWALLBASEINFO_DEVID, F_TV2DECODER_TVWALLID);

    if (!tSqlCond.Empty())
    {
        tSqlCount.AppendString(" where ");
        tSqlCount.AppendString("%s", tSqlCond.GetBuf());
    }

    string strUserId, strUserGrpId;
    string strTvwallId;
    cCond.GetCuserId(strUserId);
    cCond.GetCuserGrpId(strUserGrpId);
    cCond.GetTvwallId(strTvwallId);
    tSqlSelect.Format("call %s('%s', '%s', '%s', %d, %d)", P_TVWALLQRY, DbStr(strUserId), DbStr(strUserGrpId),
        DbStr(strTvwallId), cReq.GetStartEntryIndex(), GetExpectEntryNum(cReq.GetExpectEntryNum()));
    return CMS_SUCCESS;
}


void CDbOpr::FetchTvwallQryRecord( CTvwallInfo &cTvwall )
{
    FetchDeviceQryRecord(cTvwall);

    cTvwall.SetTvNum(GetInt(F_TVWALL_TVNUM));
    cTvwall.SetTvwallStyle(GetInt(F_TVWALL_TVWALLSTYLE));
    cTvwall.SetTvwallCustomStyleData(GetString(F_TVWALL_TVWALLCUSTOMSTYLEDATA));

    TvDecoderBinder tBinder;
    tBinder.tvId = GetInt(F_TV2DECODER_TVID);
    tBinder.decoderId = GetString(F_TV2DECODER_DECODERID);
    tBinder.decoderOutputId = GetInt(F_TV2DECODER_DECODEROUTPUTID);
    cTvwall.GetTvDecoderBinders().clear();
    cTvwall.GetTvDecoderBinders().push_back(tBinder);
}



ENTasErrno CDbOpr::TvwallItemAdd( const CTvwallInfo &cTvwall )
{
    ENTasErrno enErr = CMS_SUCCESS;

    TSqlString tSql;

    do 
    {
        enErr = DeviceItemAdd(cTvwall);
        if (CMS_SUCCESS != enErr)
        {
            break;
        }

        tSql.Format("insert into %s values('%s', %d, %d, '%s')",
            T_TVWALL, DbStr(cTvwall.GetDeviceUUID()), (s32)cTvwall.GetTvNum(), (s32)cTvwall.GetTvwallStyle(), 
            DbStr(cTvwall.GetTvwallCustomStyleData()));
        if (!ExecSql(tSql, enExecNoRet))
        {
            enErr = ERR_TAS_DBERROR;
            break;
        }

        vector<TvDecoderBinder> cDecBinders = cTvwall.GetTvDecoderBinders();
        for (u32 i = 0; i < cDecBinders.size(); ++i)
        {
            tSql.Format("insert into %s values('%s', %d, '%s', %d)", 
                T_TV2DECODER, DbStr(cTvwall.GetDeviceUUID()), 
                cDecBinders[i].tvId, DbStr(cDecBinders[i].decoderId), cDecBinders[i].decoderOutputId);

            if (!ExecSql(tSql, enExecNoRet))
            {
                enErr = ERR_TAS_DBERROR;
                break;
            }
        }

        enErr = CuserDevAddDefault(cTvwall.GetDeviceUUID(), cTvwall.GetDeviceModel());
        if (enErr != CMS_SUCCESS) break;

        enErr = DevGrpDevAddDefault(cTvwall.GetDeviceUUID(), cTvwall.GetDeviceModel());
        if (enErr != CMS_SUCCESS) break;
    } while (false);

    if (CMS_SUCCESS != enErr)
    {
        TvwallItemDel(cTvwall.GetDeviceUUID());
    }

    return enErr;
}

ENTasErrno CDbOpr::TvwallItemDel( const UUIDString &cTvwallId )
{
    ENTasErrno enErr = CMS_SUCCESS;

    TSqlString tSql;

    do 
    {
        enErr = DeviceItemDel(cTvwallId);
        if (CMS_SUCCESS != enErr)
        {
            break;
        }

        tSql.Format("delete from %s where %s='%s'", T_TVWALL, F_TVWALL_TVWALLID, DbStr(cTvwallId));
        if (!ExecSql(tSql, enExecNoRet))
        {
            enErr = ERR_TAS_DBERROR;
            break;
        }

        tSql.Format("delete from %s where %s='%s'", 
            T_TV2DECODER, F_TV2DECODER_TVWALLID, DbStr(cTvwallId));
        if (!ExecSql(tSql, enExecNoRet))
        {
            enErr = ERR_TAS_DBERROR;
            break;
        }

        enErr = DevGrpDevDelByDevId(cTvwallId);
        if (enErr != CMS_SUCCESS) break;

        enErr = CuserGrpDevDel(cTvwallId);
        if (CMS_SUCCESS != enErr) break;

        enErr = CuserDevDelByDevId(cTvwallId);
        if (CMS_SUCCESS != enErr) break;
    } while (false);

    return enErr;
}

ENTasErrno CDbOpr::TvwallItemMod( const CTvwallInfo &cTvwall )
{
    ENTasErrno enErr = CMS_SUCCESS;

    TSqlString tSql;

    do 
    {
        DeviceItemMod(cTvwall);

        tSql.Format("update %s set %s=%d, %s=%d, %s='%s' where %s='%s'",
            T_TVWALL, F_TVWALL_TVNUM, cTvwall.GetTvNum(), F_TVWALL_TVWALLSTYLE, cTvwall.GetTvwallStyle(), 
            F_TVWALL_TVWALLCUSTOMSTYLEDATA, DbStr(cTvwall.GetTvwallCustomStyleData()),
            F_TVWALL_TVWALLID, DbStr(cTvwall.GetDeviceUUID()));
        if (!ExecSql(tSql, enExecNoRet))
        {
            enErr = ERR_TAS_DBERROR;
            break;
        }

        tSql.Format("delete from %s where %s='%s'", T_TV2DECODER, F_TVWALL_TVWALLID, DbStr(cTvwall.GetDeviceUUID()));
        if (!ExecSql(tSql, enExecNoRet))
        {
            enErr = ERR_TAS_DBERROR;
            break;
        }
        vector<TvDecoderBinder> cDecBinders = cTvwall.GetTvDecoderBinders();
        for (u32 i = 0; i < cDecBinders.size(); ++i)
        {
            tSql.Format("insert into %s values('%s', %d, '%s', %d)", 
                T_TV2DECODER, DbStr(cTvwall.GetDeviceUUID()), 
                cDecBinders[i].tvId, DbStr(cDecBinders[i].decoderId), cDecBinders[i].decoderOutputId);

            if (!ExecSql(tSql, enExecNoRet))
            {
                enErr = ERR_TAS_DBERROR;
                break;
            }
        }
    } while (false);

    return enErr;
}


ENTasErrno CDbOpr::TvwallItemQry( const UUIDString &cTvwallId, CTvwallInfo &cTvwall )
{
    ENTasErrno enErr = CMS_SUCCESS;

    TSqlString tSql;

    do 
    {
        /*select * from tblTvwall a inner join tblDevice b
        on a.tvwallId=b.devId where a.tvwallId=''*/
        tSql.Format("select * from %s a inner join %s b on a.%s=b.%s where a.%s='%s'",
            T_TVWALL, T_DEVICE, F_TVWALL_TVWALLID, F_DEVICE_DEVID, 
            F_TVWALL_TVWALLID, DbStr(cTvwallId));
        if (!ExecSql(tSql, enExecMayRet))
        {
            enErr = ERR_TAS_DBERROR;
            break;
        }

        if (IsNotEof())
        {
            FetchDeviceQryRecord(cTvwall); 

            cTvwall.SetTvNum(GetInt(F_TVWALL_TVNUM));
            cTvwall.SetTvwallStyle(GetInt(F_TVWALL_TVWALLSTYLE));
            cTvwall.SetTvwallCustomStyleData(GetString(F_TVWALL_TVWALLCUSTOMSTYLEDATA));
        }
        else
        {
            enErr = ERR_TAS_RECORD_NOT_EXISTS;
            break;
        }

        cTvwall.GetTvDecoderBinders().clear();
        tSql.Format("select * from %s where %s='%s'", T_TV2DECODER, F_TV2DECODER_TVWALLID, DbStr(cTvwallId));
        if (!ExecSql(tSql, enExecMayRet))
        {
            enErr = ERR_TAS_DBERROR;
            break;
        }

        TvDecoderBinder tBinder;
        while(IsNotEof())
        {
            tBinder.tvId = GetInt(F_TV2DECODER_TVID);
            tBinder.decoderId = GetString(F_TV2DECODER_DECODERID);
            tBinder.decoderOutputId = GetInt(F_TV2DECODER_DECODEROUTPUTID);

            cTvwall.GetTvDecoderBinders().push_back(tBinder);
            if (!MoveNext())
            {
                enErr = ERR_TAS_DBERROR;
                break;
            }
        }
    } while (false);

    return enErr;
}


bool CDbOpr::IsExistSameNameTvwall( const string &strName, const UUIDString *ptExcludeTvwallId /*= NULL*/ )
{
    TSqlString tSql;
    tSql.Format("select count(*) from %s a where a.%s='%s' and a.%s=%d", T_DEVICE, 
        F_DEVICE_DEVNAME, DbStr(strName), F_DEVICE_DEVTYPE, DEVICE_TYPE_TV_WALL);
    if (NULL != ptExcludeTvwallId)
    {
        tSql.AppendString(" and a.%s!='%s'", F_DEVICE_DEVID, DbStr(*ptExcludeTvwallId));
    }

    if (ExecSql(tSql, enExecMustRet))
    {
        return GetIntResult() > 0;
    }
    else
    {
        // 失败时返回存在，好暴露问题
        return true;
    }
}


void CDbOpr::CuserDevAdd( const CTaskMsgTransfer<CUserDeviceAddReq> *pcMsg )
{
    CTaskMsgTransfer<CUserDeviceAddRsp> cTrans;
    CTaskMsgTransfer<CUserDeviceAddNtfReq> cTransNtf;
    CUserDeviceAddRsp &cRsp = cTrans.GetMsgObj();
    CUserDeviceAddNtfReq &cNtf = cTransNtf.GetMsgObj();
    bool bHasNtf = false;

    const CUserDeviceAddReq &cReq = pcMsg->GetMsgObj();

    do 
    {
        CUserDeviceInfo cInfo = cReq.GetUserDeviceInfo();

        if (cInfo.GetDeviceCapPrivilege().size() == 0)
        {
            cRsp.SetErrorCode(ERR_TAS_INVALID_PARM);
            break;
        }

		CDeviceInfo cDevInfo;
		ENTasErrno enDevQryErr = DeviceItemQry(cInfo.GetDeviceID(), cDevInfo);
		if (CMS_SUCCESS != enDevQryErr)
		{
			cRsp.SetErrorCode(enDevQryErr);
			break;
		}

        /* 编码设备时自动划归编码通道权限 */
        CDeviceModelInfo cDevModelInfo;
        cRsp.SetErrorCode(DeviceModelItemQryByDevId(cInfo.GetDeviceID(), cDevModelInfo));
        if (cRsp.GetErrorCode() != CMS_SUCCESS) break;
        if (cDevModelInfo.GetDeviceTypeId() == DEVICE_TYPE_ENCODER)
        {
            GenerateEncodeCapPrivilegeInfo(cDevModelInfo, 
                cReq.GetUserDeviceInfo().GetDeviceCapPrivilege(),
                cInfo.GetDeviceCapPrivilege());
        }

        CUserInfo cUser;
        if (!GetCuserInfo(cInfo.GetUserID(), cUser))
        {
            cRsp.SetErrorCode(ERR_TAS_USER_NOT_EXIST);
            break;
        }
        if (!cUser.GetUsergrpID().empty())
        {
            cRsp.SetErrorCode(ERR_TAS_INVALID_OPERATION);
            break;
        }

        cRsp.SetErrorCode(ValidateCurrentUserDevPrivilege(cReq.GetSession(), cInfo));
        if (cRsp.GetErrorCode() != CMS_SUCCESS) break;

        cRsp.SetErrorCode(CuserDevItemAdd(cInfo));
        if (cRsp.GetErrorCode() != CMS_SUCCESS)
        {
            break;
        }
		else
		{
			if (CurrentDomainId() != cInfo.GetUserID())
			{
				RedisAddUserDevPrivilege(cInfo);
			}
		}

        bHasNtf = true;
		cInfo.SetDevDomainID(cDevInfo.GetDomainUUID());
        cNtf.SetUserDeviceInfo(cInfo);

        UpdateCuserDevTreeVer(cInfo.GetUserID());
    } while (false);

    PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
    if (bHasNtf)
    {
        PostTaskMsgTransfer2CCtrl4LargeNtf(CUserDeviceAddNtfReq, GetUserDeviceInfo, GetDeviceCapPrivilege)
    }
}

ENTasErrno CDbOpr::CuserDevAddDefault( const UUIDString &devId, const UUIDString &devModelId )
{
    TSqlString tSql;

    CDeviceModelInfo cModel = g_cDbCache.GetDeviceModelInfo(devModelId, this);
    if (cModel.GetDeviceModelId().empty())
    {
        return ERR_TAS_INVALID_DEVICEMODELID;
    }

    if (cModel.GetDeviceTypeId() == DEVICE_TYPE_ENCODER)
    {
        vector<TDeviceCapInfo> &atCapInfos = cModel.GetDeviceCapInfo();

        bool bDevAssigned = false;
        for (vector<TDeviceCapInfo>::const_iterator it = atCapInfos.begin(); it != atCapInfos.end(); ++it)
        {
            if (!(it->type == DEVICE_CAP_VIDEOSOURCE || it->type == DEVICE_CAP_ENCODE_CHANNEL))
            {
                continue;
            }

            if (!bDevAssigned)
            {
                tSql.Format("insert into %s values('%s', '%s', %d)", T_CUSER2DEV, 
                    DbStr(CurrentDomainId()), DbStr(devId), -1); // 所有的权限
                if (!ExecSql(tSql, enExecNoRet))
                {
                    return ERR_TAS_DBERROR;
                }

                bDevAssigned = true;
            }

            for (u32 i = 0; i < static_cast<u32>(it->num); ++i)
            {
                tSql.Format("insert into %s values('%s', '%s', %d, %d, %d)", 
                    T_CUSER2DEVICECAP, DbStr(CurrentDomainId()), DbStr(devId), it->type, i, -1); // 所有的权限
                
                if (!ExecSql(tSql, enExecNoRet))
                {
                    return ERR_TAS_DBERROR;
                }
            }
        }
    }
    else if (cModel.GetDeviceTypeId() == DEVICE_TYPE_TV_WALL)
    {
        tSql.Format("insert into %s values('%s', '%s', %d)", T_CUSER2DEV, 
            DbStr(CurrentDomainId()), DbStr(devId), -1); // 所有的权限
        if (!ExecSql(tSql, enExecNoRet))
        {
            return ERR_TAS_DBERROR;
        }

        tSql.Format("insert into %s values('%s', '%s', %d, %d, %d)", 
            T_CUSER2DEVICECAP, DbStr(CurrentDomainId()), DbStr(devId), DEVICE_CAP_DUMMY, 0, -1); // 所有的权限
        if (!ExecSql(tSql, enExecNoRet))
        {
            return ERR_TAS_DBERROR;
        }
    }

    return CMS_SUCCESS;
}

void CDbOpr::CuserDevDel( const CTaskMsgTransfer<CUserDeviceDelReq> *pcMsg )
{
    CTaskMsgTransfer<CUserDeviceDelRsp> cTrans;
    CTaskMsgTransfer<CUserDeviceDelNtfReq> cTransNtf;
    CUserDeviceDelRsp &cRsp = cTrans.GetMsgObj();
    CUserDeviceDelNtfReq &cNtf = cTransNtf.GetMsgObj();
    bool bHasNtf = false;

    const CUserDeviceDelReq &cReq = pcMsg->GetMsgObj();
    CUserDeviceInfoKey cKey = cReq.GetUserDeviceInfoKey();

    do 
    {
		CDeviceInfo cDevInfo;
		ENTasErrno enDevQryErr = DeviceItemQry(cKey.GetDeviceID(), cDevInfo);
		if (CMS_SUCCESS != enDevQryErr)
		{
			cRsp.SetErrorCode(enDevQryErr);
			break;
		}
		
        /* 编码设备时自动删除编码通道权限 */
        CDeviceModelInfo cDevModelInfo;
        cRsp.SetErrorCode(DeviceModelItemQryByDevId(cKey.GetDeviceID(), cDevModelInfo));
        if (cRsp.GetErrorCode() != CMS_SUCCESS) break;
        if (cDevModelInfo.GetDeviceTypeId() == DEVICE_TYPE_ENCODER)
        {
            GenerateEncodeCapIndexInfo(cDevModelInfo, 
                cReq.GetUserDeviceInfoKey().GetDeviceCapIndexs(),
                cKey.GetDeviceCapIndexs());
        }

		// 检查该设备所在组是否是自动更新组，是则不允许取消划归
		if (IsAutoAssignDeviceGroup(cKey.GetDeviceID(), cKey.GetDeviceCapIndexs(), cKey.GetUserID()))
		{
			cRsp.SetErrorCode(ERR_TAS_DEV_GROUP_IS_AUTO_UPDATE);
			break;
		}

		// 删除用户设备，删除用户对应的PTZ锁 [4/14/2016 pangubing]
		DeleteUserPtzLockInner(pcMsg,cKey.GetUserID(),cKey.GetDeviceID(),cKey.GetDeviceCapIndexs());

        // 删除收藏夹中的数据
        for (vector<TDeviceCapIndexs>::const_iterator it=cKey.GetDeviceCapIndexs().begin();
            it!=cKey.GetDeviceCapIndexs().end();++it)
        {
            if (it->id!=DEVICE_CAP_VIDEOSOURCE) continue;

            for (vector<int>::const_iterator it2=it->indexs.begin();it2!=it->indexs.end();++it2)
            {
                DevGrpDevDel4Favorite(cKey.GetUserID(), cKey.GetDeviceID(), *it2);
            }
        }

        cRsp.SetErrorCode(CuserDevItemDel(cKey));

        if (cRsp.GetErrorCode() == CMS_SUCCESS)
        {
            bHasNtf = true;
			cNtf.SetDevDomainId(cDevInfo.GetDomainUUID());
            cNtf.SetUserDeviceInfoKey(cKey);

			
			if (CurrentDomainId() != cKey.GetUserID())
			{
				RedisDelUserDevPrivilege(cKey);
			}
        }

        UpdateCuserDevTreeVer(cKey.GetUserID());
    } while (false);

    PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
    if (bHasNtf) PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cNtf, cTransNtf);
}

ENTasErrno CDbOpr::CuserDevDelByDevId( const UUIDString &devId )
{
    TSqlString tSql;

    tSql.Format("delete from %s where %s='%s'", T_CUSER2DEVICECAP, F_CUSER2DEVICECAP_DEVID, DbStr(devId));
    if (!ExecSql(tSql, enExecNoRet)) return ERR_TAS_DBERROR;
    
    tSql.Format("delete from %s where %s='%s'", T_CUSER2DEV, F_CUSER2DEV_DEVID, DbStr(devId));
    if (!ExecSql(tSql, enExecNoRet)) return ERR_TAS_DBERROR;

    return CMS_SUCCESS;
}

ENTasErrno CDbOpr::CuserDevDelByUserId( const UUIDString &userId )
{
    TSqlString tSql;

    tSql.Format("delete from %s where %s='%s'", T_CUSER2DEVICECAP, F_CUSER2DEVICECAP_CUSERID, DbStr(userId));
    if (!ExecSql(tSql, enExecNoRet)) return ERR_TAS_DBERROR;

    tSql.Format("delete from %s where %s='%s'", T_CUSER2DEV, F_CUSER2DEV_CUSERID, DbStr(userId));
    if (!ExecSql(tSql, enExecNoRet)) return ERR_TAS_DBERROR;

    return CMS_SUCCESS;
}

void CDbOpr::CuserDevMod( const CTaskMsgTransfer<CUserDeviceModReq> *pcMsg )
{
    CTaskMsgTransfer<CUserDeviceModRsp> cTrans;
    CTaskMsgTransfer<CUserDeviceModNtfReq> cTransNtf;
    CUserDeviceModRsp &cRsp = cTrans.GetMsgObj();
    CUserDeviceModNtfReq &cNtf = cTransNtf.GetMsgObj();
    bool bHasNtf = false;

    const CUserDeviceModReq &cReq = pcMsg->GetMsgObj();
    bool bModDevCapPriv;
    do 
    {
        CDevicePrivilege cDevPrivi;
        vector<TDeviceCapPrivilege> cDevCapPrivis;
        cReq.GetDevicePrivilege(cDevPrivi);
        bModDevCapPriv = cReq.GetDeviceCapPrivilege(cDevCapPrivis);
		
		// 增加校验视频源权限 [12/11/2015 pangubing]
		if (bModDevCapPriv)
		{
			cRsp.SetErrorCode(IsDevCapPrivilegeVailed(cDevCapPrivis));
			if (cRsp.GetErrorCode() != CMS_SUCCESS)
			{
				break;
			}
		}

        // 自动调整编码通道权限
        CDeviceModelInfo cDevModelInfo;
        cRsp.SetErrorCode(DeviceModelItemQryByDevId(cReq.GetDeviceID(), cDevModelInfo));
        if (cRsp.GetErrorCode() != CMS_SUCCESS)
        {
            break;
        }
        if (cDevModelInfo.GetDeviceTypeId() == DEVICE_TYPE_ENCODER)
        {
            vector<TDeviceCapPrivilege> cDevCapPrivisTmp(cDevCapPrivis);
            GenerateEncodeCapPrivilegeInfo(cDevModelInfo, cDevCapPrivisTmp, cDevCapPrivis);
        }

        CUserDeviceInfo cUserDevInfo4Validate;
        cUserDevInfo4Validate.SetUserID(cReq.GetUserID());
        cUserDevInfo4Validate.SetDeviceID(cReq.GetDeviceID());
        cUserDevInfo4Validate.SetDevicePrivilege(cDevPrivi);
        cUserDevInfo4Validate.SetDeviceCapPrivilege(cDevCapPrivis);
        cRsp.SetErrorCode(ValidateCurrentUserDevPrivilege(cReq.GetSession(), cUserDevInfo4Validate));
        if (cRsp.GetErrorCode() != CMS_SUCCESS)
        {
            break;
        }

        TSqlString tSql;
        if (cReq.GetDevicePrivilege(cDevPrivi))
        {
            tSql.Format("update %s set %s=%d where %s='%s' and %s='%s'", T_CUSER2DEV,
                F_CUSER2DEV_DEVPRIVILEGE, (s32)cDevPrivi.Convert2Int(), F_CUSER2DEV_CUSERID, DbStr(cReq.GetUserID()),
                F_CUSER2DEV_DEVID, DbStr(cReq.GetDeviceID()));
            if (!ExecSql(tSql, enExecNoRet))
            {
                cRsp.SetErrorCode(ERR_TAS_DBERROR);
                break;
            }
        }

        if (bModDevCapPriv)
        {
			CVideosourcePrivilege tmpPrivilege;
            u32 i;
            for (i = 0; i < cDevCapPrivis.size(); ++i)
            {
                map<int, CBitsContainer>::iterator it;
                for (it = cDevCapPrivis[i].privileges.begin(); it != cDevCapPrivis[i].privileges.end(); ++it)
                {
                    tSql.Format("update %s set %s=%d where %s='%s' and %s='%s' and %s=%d and %s=%d",
                        T_CUSER2DEVICECAP, F_CUSER2DEVICECAP_DEVCAPINDEXPRIVILEGE, (s32)it->second.Convert2Int(),
                        F_CUSER2DEVICECAP_CUSERID, DbStr(cReq.GetUserID()), F_CUSER2DEVICECAP_DEVID, DbStr(cReq.GetDeviceID()),
                        F_CUSER2DEVICECAP_DEVCAPID, cDevCapPrivis[i].id, F_CUSER2DEVICECAP_DEVCAPINDEX, it->first);
                    if (!ExecSql(tSql, enExecNoRet))
                    {
                        cRsp.SetErrorCode(ERR_TAS_DBERROR);
                        break;
                    }

					// 取消视频源PTZ锁权限，删除PTZ锁 [4/14/2016 pangubing]
					if (DEVICE_CAP_VIDEOSOURCE == cDevCapPrivis[i].id)
					{
						tmpPrivilege.ConvertFromBitsContainer(it->second);
						if (!tmpPrivilege.HasPrivilege(CVideosourcePrivilege::en_PTZ_Lock))
						{
							CPtzLockInfo cInfo;
							if (GetPtzLockInfo(cReq.GetDeviceID(),it->first,cInfo)&&
								cInfo.GetUserId()==cReq.GetUserID())
							{
								DeletePtzLockInner(pcMsg,cReq.GetDeviceID(),it->first);
							}

						}
					}
                }



                if (it != cDevCapPrivis[i].privileges.end())
                {
                    break;
                }
            }

            if (i != cDevCapPrivis.size())
            {
                break;
            }
        }

        UpdateCuserDevTreeVer(cReq.GetUserID());

		
		if (CurrentDomainId() != cReq.GetUserID())
		{
			RedisModUserDevPrivilege(cReq);
		}
		

        CDeviceInfo cDeviceInfo;
        bHasNtf = (CMS_SUCCESS == DeviceItemQry(cReq.GetDeviceID(), cDeviceInfo));
        CUserDeviceInfo &cUserDevInfo4Ntf = cNtf.GetUserDeviceInfo();
        cUserDevInfo4Ntf.SetUserID(cReq.GetUserID());
        cUserDevInfo4Ntf.SetDeviceID(cReq.GetDeviceID());
        cUserDevInfo4Ntf.SetDevDomainID(cDeviceInfo.GetDomainUUID());
        cUserDevInfo4Ntf.SetDevicePrivilege(cDevPrivi);
        cUserDevInfo4Ntf.SetDeviceCapPrivilege(cDevCapPrivis);
    } while (false);

    PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
    if (bHasNtf)
    {
        PostTaskMsgTransfer2CCtrl4LargeNtf(CUserDeviceModNtfReq, GetUserDeviceInfo, GetDeviceCapPrivilege)
    }
}

void CDbOpr::CuserDevQry( const CTaskMsgTransfer<CUserDeviceQryReq> *pcMsg )
{
    DatabaseLongQueryTemplate<CUserDeviceQryReq, CUserDeviceQryRsp, CUserDeviceQryNtfReq, CUserDeviceInfo, TDeviceCapPrivilege>(
        pcMsg, &CDbOpr::PrepareCuserDevQrySql, &CDbOpr::FetchCuserDevQryRecord, &CUserDeviceQryNtfReq::GetCuserDevInfo,
        &CUserDeviceInfo::GetDeviceCapPrivilege, &CUserDeviceInfo::GetUserID, &CUserDeviceInfo::GetDeviceID);
}

ENTasErrno CDbOpr::PrepareCuserDevQrySql( const CUserDeviceQryReq &cReq, TSqlString &tSqlSelect, TSqlString &tSqlCount )
{
    const CUserDeviceInfoQueryCondition &cCond = cReq.GetUserDeviceInfoQueryCondition();

    string strCuserId;
    CUserInfo cUser;
    if (!cCond.GetUserID(strCuserId) || !GetCuserInfo(strCuserId, cUser))
    {
        return ERR_TAS_USER_NOT_EXIST;
    }
    
    string devGrpId;
    string devId;
    s32 devCapId = -1;
    s32 devCapIndex = -1;
    if (cCond.GetDevGrpID(devGrpId) && devGrpId.empty()) return ERR_TAS_INVALID_PARM; // 任何设备都属于设备组为空表示非法
    if (cCond.GetDeviceID(devId) && devId.empty()) return ERR_TAS_INVALID_PARM;
    cCond.GetCapID(devCapId);
    cCond.GetCapIndex(devCapIndex);

    tSqlCount.Format("call %s('%s','%s','%s',%d, %d)", P_CUSERDEVQRYCOUNT, DbStr(strCuserId), 
        DbStr(devGrpId), DbStr(devId), devCapId, devCapIndex);

    tSqlSelect.Format("call %s('%s', '%s', '%s', %d, %d, %d, %d)", P_CUSERDEVQRY,
        DbStr(strCuserId), DbStr(devGrpId), DbStr(devId), devCapId, devCapIndex, 
        cReq.GetStartEntryIndex(), GetExpectEntryNum(cReq.GetExpectEntryNum()));

    return CMS_SUCCESS;
}

void CDbOpr::FetchCuserDevQryRecord( CUserDeviceInfo &cInfo )
{
    CDevicePrivilege cDevPrivi;
    cInfo.SetUserID(GetString(F_CUSER2DEVICECAP_CUSERID));
    cInfo.SetDeviceID(GetString(F_CUSER2DEVICECAP_DEVID));
    cInfo.SetDevDomainID(GetString(F_DEVICE_DOMAINID));
    cDevPrivi.ConvertFromInt(GetInt(F_CUSER2DEV_DEVPRIVILEGE));
    cInfo.SetDevicePrivilege(cDevPrivi);

    TDeviceCapPrivilege cCapPrivi;
    cCapPrivi.id = GetInt(F_CUSER2DEVICECAP_DEVCAPID);
    CBitsContainer cBitsContainer;
    cBitsContainer.ConvertFromInt(GetInt(F_CUSER2DEVICECAP_DEVCAPINDEXPRIVILEGE));
    cCapPrivi.privileges.insert(map<int, CBitsContainer>::value_type(GetInt(F_CUSER2DEVICECAP_DEVCAPINDEX), cBitsContainer));
    
    cInfo.GetDeviceCapPrivilege().clear();
    cInfo.GetDeviceCapPrivilege().push_back(cCapPrivi);
}

ENTasErrno CDbOpr::GetCUserDeviceInfo( const UUIDString &cUserId, const UUIDString &cDevId, CUserDeviceInfo &cDevInfo )
{
    CUserInfo cUser;
    if (!GetCuserInfo(cUserId, cUser))
        return ERR_TAS_USER_NOT_EXIST;

    ENTasErrno enErrno = CMS_SUCCESS;

    do 
    {
        if (cUser.GetUsergrpID().empty())
        {
            TSqlString tSql;

            string devGrpId;
            s32 devCapId = -1;
            s32 devCapIndex = -1;
            tSql.Format("call %s('%s', '%s', '%s', %d, %d, %d, %d)", P_CUSERDEVQRY,
                DbStr(cUserId), DbStr(devGrpId), DbStr(cDevId), devCapId, devCapIndex, 0, ENTRY_MAX_NUM_ONE_PAGE);
            if (!ExecSql(tSql, enExecMayRet))
            {
                enErrno = ERR_TAS_DBERROR;
                break;
            }
            
            cDevInfo.GetUserID().clear();
            CUserDeviceInfo cInfoTmp;
            while (IsNotEof())
            {
                FetchCuserDevQryRecord(cInfoTmp);
                if (!cDevInfo.GetUserID().empty())
                {
                    cDevInfo.GetDeviceCapPrivilege().push_back(cInfoTmp.GetDeviceCapPrivilege().front());
                }
                else
                {
                    cDevInfo = cInfoTmp;
                }

                if (!MoveNext())
                {
                    enErrno = ERR_TAS_DBERROR;
                    break;
                }
            }
        }
        else
        {
            CUserGroupDeviceInfo cUserGrpDevInfo;
            enErrno = GetCUserGrpDeviceInfo(cUser.GetUsergrpID(), cDevId, cUserGrpDevInfo);
            if (CMS_SUCCESS != enErrno) break;

            cDevInfo.SetUserID(cUserId);
            cDevInfo.SetDeviceID(cUserGrpDevInfo.GetDeviceID());
            cDevInfo.SetDevDomainID(cUserGrpDevInfo.GetDevDomainID());
            cDevInfo.SetDevicePrivilege(cUserGrpDevInfo.GetDevicePrivilege());
            cDevInfo.SetDeviceCapPrivilege(cUserGrpDevInfo.GetDeviceCapPrivilege());
        }
    } while (false);
    
    return enErrno;
}

ENTasErrno CDbOpr::ValidateCurrentUserDevPrivilege( const string &strSessionId, const CUserDeviceInfo &cDevInfo )
{
    ENTasErrno enErrno;

    // 校验当前操作用户的设备权限
    do 
    {
        string strOprUser;
        if (GetCurrentCuserId(strSessionId, strOprUser))
        {
            CUserDeviceInfo cOprUserDevInfo;

            enErrno = GetCUserDeviceInfo(strOprUser, cDevInfo.GetDeviceID(), cOprUserDevInfo);
            if (CMS_SUCCESS != enErrno) break;

            // 比较设备的权限
            if (!cOprUserDevInfo.GetDevicePrivilege().IsSuperSet(cDevInfo.GetDevicePrivilege()))
            {
                enErrno = ERR_TAS_ULTRA_VIRES;
                break;
            }

            CBitsContainer cPriviBits;
            // 比较设备的具体能力权限
            for (size_t i = 0; i < cDevInfo.GetDeviceCapPrivilege().size(); ++i)
            {
                map<int, CBitsContainer>::const_iterator it;
                for (it = cDevInfo.GetDeviceCapPrivilege()[i].privileges.begin(); 
                    it != cDevInfo.GetDeviceCapPrivilege()[i].privileges.end(); ++it)
                {
                    size_t j;
                    for (j = 0; j < cOprUserDevInfo.GetDeviceCapPrivilege().size(); ++j)
                    {
                        map<int, CBitsContainer>::const_iterator it2;
                        for (it2 = cOprUserDevInfo.GetDeviceCapPrivilege()[j].privileges.begin(); 
                            it2 != cOprUserDevInfo.GetDeviceCapPrivilege()[j].privileges.end(); ++it2)
                        {
                            if (cDevInfo.GetDeviceCapPrivilege()[i].id == cOprUserDevInfo.GetDeviceCapPrivilege()[j].id
                                && it->first == it2->first)
                            {
                                cPriviBits = it2->second;
                                break;
                            }
                        }

                        if (it2 != cOprUserDevInfo.GetDeviceCapPrivilege()[j].privileges.end())
                        {
                            break; // 找到了, 提前结束循环
                        }
                    }

                    if (j == cOprUserDevInfo.GetDeviceCapPrivilege().size())
                    {
                        // 未找到
                        enErrno = ERR_TAS_ULTRA_VIRES;
                        break;
                    }

                    if (!cPriviBits.IsSuperSet(it->second))
                    {
                        enErrno = ERR_TAS_ULTRA_VIRES;
                        break;
                    }
                }

                if (it != cDevInfo.GetDeviceCapPrivilege()[i].privileges.end())
                {
                    // 提前发现存在未校验通过的问题
                    break;
                }
            }
        }
        else
        {
            enErrno = ERR_TAS_CURRENT_USER_NOT_EXIST;
            break;
        }
    } while (false);

    return enErrno;
}

ENTasErrno CDbOpr::CuserDevItemAdd( const CUserDeviceInfo &cInfo )
{
	// 增加校验视频源权限 [12/11/2015 pangubing]
	const vector<TDeviceCapPrivilege> &aCapPrivi = cInfo.GetDeviceCapPrivilege();
	if (IsDevCapPrivilegeVailed(aCapPrivi)!=CMS_SUCCESS)
	{
		return ERR_TAS_INVALID_PARM;
	}

    TSqlString tSql;
    tSql.Format("call %s('%s', '%s', %d)", P_CUSERDEVADD, 
		DbStr(cInfo.GetUserID()), DbStr(cInfo.GetDeviceID()), (s32)cInfo.GetDevicePrivilege().Convert2Int());
	if (!ExecSql(tSql, enExecNoRet))
    {
        return ERR_TAS_DBERROR;
    }

	u32 i;
    for (i = 0; i < aCapPrivi.size(); ++i)
    {
        const TDeviceCapPrivilege &tCap = aCapPrivi[i];

        map<int, CBitsContainer>::const_iterator it;
        for (it = tCap.privileges.begin(); it != tCap.privileges.end(); ++it)
        {
            tSql.Format("call %s('%s', '%s', %d, %d, %d)", 
                P_CUSERDEVCAPADD, DbStr(cInfo.GetUserID()), DbStr(cInfo.GetDeviceID()), tCap.id, 
                it->first, (s32)it->second.Convert2Int());

            if (!ExecSql(tSql, enExecNoRet)) break;
        }

        if (it != tCap.privileges.end())
            break;
    }
    if (i != aCapPrivi.size()) 
    {
        return ERR_TAS_DBERROR;
    }

    return CMS_SUCCESS;
}

ENTasErrno CDbOpr::CuserDevItemDel( const CUserDeviceInfoKey &cKey )
{
    TSqlString tSql;
    bool bNeedDelDev = false;
    const vector<TDeviceCapIndexs> tCapIndexs = cKey.GetDeviceCapIndexs();
    if (tCapIndexs.size() > 0)
    {
        for (u32 i = 0; i < tCapIndexs.size(); ++i)
        {
            for (u32 j = 0; j < tCapIndexs[i].indexs.size(); ++j)
            {
                tSql.Format("delete from %s where %s='%s' and %s='%s' and %s=%d and %s=%d", T_CUSER2DEVICECAP, 
                    F_CUSER2DEVICECAP_CUSERID, DbStr(cKey.GetUserID()), F_CUSER2DEVICECAP_DEVID, DbStr(cKey.GetDeviceID()),
                    F_CUSER2DEVICECAP_DEVCAPID, tCapIndexs[i].id, F_CUSER2DEVICECAP_DEVCAPINDEX, tCapIndexs[i].indexs[j]);
                if (!ExecSql(tSql, enExecNoRet))
                {
                    return ERR_TAS_DBERROR;
                }
            }
        }

        // 判断是否所有的设备能力权限都删除了，是的话后面自动删除设备权限
        tSql.Format("select count(*) from %s where %s='%s' and %s='%s'", T_CUSER2DEVICECAP,
            F_CUSER2DEVICECAP_CUSERID, DbStr(cKey.GetUserID()), F_CUSER2DEVICECAP_DEVID, DbStr(cKey.GetDeviceID()));
        if (ExecSql(tSql, enExecMustRet))
        {
            if (GetIntResult() == 0) bNeedDelDev = true;
        }
        else
        {
            return ERR_TAS_DBERROR;
        }
    }

    if (bNeedDelDev)
    {
        tSql.Format("delete from %s where %s='%s' and %s='%s'", T_CUSER2DEV, 
            F_CUSER2DEV_CUSERID, DbStr(cKey.GetUserID()), F_CUSER2DEV_DEVID, DbStr(cKey.GetDeviceID()));
        if (!ExecSql(tSql, enExecNoRet))
        {
            return ERR_TAS_DBERROR;
        }
    }

    return CMS_SUCCESS;
}

ENTasErrno CDbOpr::CuserDevItemMod( const CUserDeviceInfo &cInfo )
{
    TSqlString tSql;

    tSql.Format("update %s set %s=%d where %s='%s' and %s='%s'", T_CUSER2DEV,
        F_CUSER2DEV_DEVPRIVILEGE, (s32)cInfo.GetDevicePrivilege().Convert2Int(), F_CUSER2DEV_CUSERID, 
        DbStr(cInfo.GetUserID()), F_CUSER2DEV_DEVID, DbStr(cInfo.GetDeviceID()));
    if (!ExecSql(tSql, enExecNoRet))
    {
        return ERR_TAS_DBERROR;
    }

    const vector<TDeviceCapPrivilege> &cDevCapPrivis = cInfo.GetDeviceCapPrivilege();
    for (u32 i = 0; i < cDevCapPrivis.size(); ++i)
    {
        for (map<int, CBitsContainer>::const_iterator it = cDevCapPrivis[i].privileges.begin(); 
             it != cDevCapPrivis[i].privileges.end(); ++it)
        {
            tSql.Format("update %s set %s=%d where %s='%s' and %s='%s' and %s=%d and %s=%d",
                T_CUSER2DEVICECAP, F_CUSER2DEVICECAP_DEVCAPINDEXPRIVILEGE, (s32)it->second.Convert2Int(),
                F_CUSER2DEVICECAP_CUSERID, DbStr(cInfo.GetUserID()), F_CUSER2DEVICECAP_DEVID, DbStr(cInfo.GetDeviceID()),
                F_CUSER2DEVICECAP_DEVCAPID, cDevCapPrivis[i].id, F_CUSER2DEVICECAP_DEVCAPINDEX, it->first);
            if (!ExecSql(tSql, enExecNoRet))
            {
                return ERR_TAS_DBERROR;
            }
        }
    }

    return CMS_SUCCESS;
}

void CDbOpr::DevGrpAdd( const CTaskMsgTransfer<CDeviceGroupAddReq> *pcMsg )
{
    CTaskMsgTransfer<CDeviceGroupAddRsp> cTrans;
    CTaskMsgTransfer<CDeviceGroupAddNtfReq> cTransNtf;
    CDeviceGroupAddRsp &cRsp = cTrans.GetMsgObj();
    CDeviceGroupAddNtfReq &cNtf = cTransNtf.GetMsgObj();
    bool bHasNtf = false;

    const CDeviceGroupAddReq &cReq = pcMsg->GetMsgObj();

    do 
    {
        CDeviceGroupInfo cInfo = cReq.GetDeviceGroupInfo();
		const string &strDevTreeId=cInfo.GetDevtreeId();
		bool bSystemDevTree=strDevTreeId.empty();

        if (cInfo.GetDomainUUID() != CurrentDomainId())
        {
            cRsp.SetErrorCode(ERR_TAS_INVALID_OPERATION);
            break;
        }

        string strUserId;
        if (GetCurrentCuserId(cReq.GetSession(), strUserId) && cInfo.GetDeviceGroupName().size() >= 11 
            && cInfo.GetDeviceGroupName()[cInfo.GetDeviceGroupName().size()-11] == '-')
        {
            u32 i=cInfo.GetDeviceGroupName().size()-1;
            for (; i >= cInfo.GetDeviceGroupName().size()-10; --i)
            {
                if (!isdigit(cInfo.GetDeviceGroupName()[i]))
                {
                    break;
                }
            }
            if (i < cInfo.GetDeviceGroupName().size()-10)
            {
                cRsp.SetErrorCode(ERR_TAS_INVALID_OPERATION); // 不允许CU创建GBS使用的组名特征
                break;
            }
        }

        if (cInfo.GetDeviceParentGroupID().empty() || cInfo.IsAuthorizationDevGrp())
        {
            // 不允许添加根组和跨域授权组
            cRsp.SetErrorCode(ERR_TAS_INVALID_OPERATION);
            break;
        }


        CDeviceGroupInfo cParentDevGrp;
        cRsp.SetErrorCode(DevGrpItemQry(cInfo.GetDeviceParentGroupID(), cParentDevGrp,strDevTreeId));
        if (CMS_SUCCESS != cRsp.GetErrorCode())
        {
            break;
        }
        if (cParentDevGrp.IsAuthorizationDevGrp())
        {
            // 跨域授权组下不允许添加组
            cRsp.SetErrorCode(ERR_TAS_INVALID_OPERATION);
            break;
        }
        if (cParentDevGrp.GetDomainUUID() != cInfo.GetDomainUUID())
        {
            // 不允许在非本域的组下创建组
            cRsp.SetErrorCode(ERR_TAS_INVALID_OPERATION);
            break;
        }

        if (IsExistSameNameDevGrp(cInfo.GetDeviceGroupName(), cInfo.GetDeviceParentGroupID(),NULL,strDevTreeId))
        {
            cRsp.SetErrorCode(ERR_TAS_EXIST_SAMENAME_RECORD);
            break;
        }



        TSqlString tSql;

		//国标推选树，不允许分组层级超过4层(父分组层级不允许超过三层)
		if (IsDevTreeCanBeUse4Gb(cReq.GetDeviceGroupInfo().GetDevtreeId()))
		{
			tSql.Format("select %s,%s from %s where %s='%s'",
				F_DEVGROUP_FULLPATH, F_DEVGROUP_NEWGBID, DbStr(GetTblDevGrpName(cReq.GetDeviceGroupInfo().GetDevtreeId())),
				F_DEVGROUP_DEVGROUPID, DbStr(cReq.GetDeviceGroupInfo().GetDeviceParentGroupID()));
			if (!ExecSql(tSql,enExecMayRet))
			{
				cRsp.SetErrorCode(ERR_TAS_DBERROR);
				break;
			}
			if (IsNotEof())
			{
				string strFullPath = GetString(F_DEVGROUP_FULLPATH);
				if (TDEV_GROUP_TYPE_CIVILCODE == cReq.GetDeviceGroupInfo().GetGrpType()
					&& strFullPath.length()>32*(GetMaxCivilcodeLev()-1))
				{
					cRsp.SetErrorCode(ERR_TAS_SRCTREE_HIERARCHY_TOO_DEEP);
					break;
				}

				//行政区划分组父分组只能是行政区划分组
				string strGrpGbid= GetString(F_DEVGROUP_NEWGBID);
				if (!IsDevGrpCivilCode(strGrpGbid) 
					&& TDEV_GROUP_TYPE_CIVILCODE == cReq.GetDeviceGroupInfo().GetGrpType())
				{
					cRsp.SetErrorCode(ERR_TAS_INVALID_OPERATION);
					break;
				}
			}
			else
			{
				cRsp.SetErrorCode(ERR_TAS_RECORD_EXISTS); //父分组不存在
				break;
			}

		}

		// 增加指定分组ID入网 [11/3/2015 pangubing]
		CUUID grpId;
		if (!cInfo.GetDeviceGroupID().empty())		
		{
			grpId = cInfo.GetDeviceGroupID();

			if (UUID_LENGTH != grpId.size())// 检查DeviceId长度是否合法
			{
				cRsp.SetErrorCode(ERR_TAS_INVALID_DEVICEID);
				break;
			}

			CDeviceGroupInfo  cTmpInfo;
			ENTasErrno nRes = DevGrpItemQry(grpId, cTmpInfo,strDevTreeId);
			if (CMS_SUCCESS == nRes)
			{
				cRsp.SetErrorCode(ERR_TAS_RECORD_EXISTS);
				break;
			}
			else if (ERR_TAS_DBERROR==nRes)
			{
				cRsp.SetErrorCode(ERR_TAS_DBERROR);
				break;
			}
		}
		// 增加指定分组ID入网 end----------------------------------- [11/3/2015 pangubing]

		// 计算fullpath[3/9/2016 pangubing]
		string fullpath=cParentDevGrp.GetFullpath()+cParentDevGrp.GetDeviceGroupID()+"/";

        CCMSTime cNowTime;
        cInfo.SetDeviceGroupID(grpId);
        cInfo.SetCreateDate(cNowTime.ToString());
		cInfo.SetFullpath(fullpath);

		// 如果设备组没有赋值初始序号，则默认添加到同级设备组最后 [5/6/2016 pangubing]
		if (0==cInfo.GetGrpseq())
		{
			cInfo.SetGrpseq(GetNextDevGrpSeq(cInfo.GetDeviceParentGroupID()));
		}

		//设置新建分组的国标ID
		CDevTreeInfo cDevTree;
		if (!strDevTreeId.empty() && GetDevTreeInfo(strDevTreeId,cDevTree))
		{
			if (cDevTree.GetCanBeUsed4Gb())
			{
				string strAvailableCiviCode;
				cRsp.SetErrorCode(GetNextDevGrpGbid(cReq.GetDeviceGroupInfo().GetDevtreeId(), cReq.GetDeviceGroupInfo().GetDeviceParentGroupID(), cReq.GetDeviceGroupInfo().GetGrpType(), strAvailableCiviCode));
				if (CMS_SUCCESS != cRsp.GetErrorCode())
				{
					break;
				}
				cInfo.SetOldgbid(strAvailableCiviCode);
				cInfo.SetNewgbid(strAvailableCiviCode);
			}
		}
		

        cRsp.SetErrorCode(DevGrpItemAdd(cInfo,strDevTreeId));
        if (CMS_SUCCESS != cRsp.GetErrorCode())
        {
            break;
        }

		cRsp.GetDeviceGroupInfoKey().SetDeviceGroupID(grpId);
		cRsp.GetDeviceGroupInfoKey().SetDevtreeId(strDevTreeId);
		bHasNtf = (CMS_SUCCESS == DevGrpItemQry(grpId, cNtf.GetDeviceGroupInfo(),strDevTreeId));
		cNtf.GetDeviceGroupInfo().SetDevtreeId(cReq.GetDeviceGroupInfo().GetDevtreeId());

		if (bSystemDevTree)
		{
			// 上级组如果是自动更新组，则新增组也为自动更新组
			map<string, u8> cOwners;
			GetAutoAssignDeviceGroup(cInfo.GetDeviceParentGroupID(), cOwners);
			if (!cOwners.empty())
			{
				RegistAutoAssignDeviceGroup(cInfo.GetDeviceGroupID(), cOwners);
			}

			SyncAnalyse(enSyncDataDeviceGroup, enSyncOperateAdd, CurrentDomainId(), &grpId);

			UpdateDevTreeVer("");
		}
		else
		{	
			bHasNtf = false;
			UpdateTreesVerByDevId("");
		}

    } while (false);

    PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
    if (bHasNtf) PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cNtf, cTransNtf);
}

void CDbOpr::DevGrpDel( const CTaskMsgTransfer<CDeviceGroupDelReq> *pcMsg )
{
    CTaskMsgTransfer<CDeviceGroupDelRsp> cTrans;
    CTaskMsgTransfer<CDeviceGroupDelNtfReq> cTransNtf;
    CDeviceGroupDelRsp &cRsp = cTrans.GetMsgObj();
    CDeviceGroupDelNtfReq &cNtf = cTransNtf.GetMsgObj();
    bool bHasNtf = false;

    const CDeviceGroupDelReq &cReq = pcMsg->GetMsgObj();
	const string &strDevTreeId=cReq.GetDeviceGroupInfoKey().GetDevtreeId();
	bool bSystemDevTree=strDevTreeId.empty();
    do 
    {
        CDeviceGroupInfo cDevGrp;
        cRsp.SetErrorCode(DevGrpItemQry(cReq.GetDeviceGroupInfoKey().GetDeviceGroupID(), cDevGrp,strDevTreeId));
        if (CMS_SUCCESS != cRsp.GetErrorCode())
        {
            break;
        }

		if (bSystemDevTree)
		{
			if (cDevGrp.GetDomainUUID() != CurrentDomainId() || cDevGrp.GetDeviceGroupID() == CurrentDomainId() 
				|| cDevGrp.IsAuthorizationDevGrp())
			{
				cRsp.SetErrorCode(ERR_TAS_INVALID_OPERATION); // 非本域的不允许删除，默认设备组和跨域授权组不允许删除
				break;
			}

			string strUserId;
			if (GetCurrentCuserId(cReq.GetSession(), strUserId) && cDevGrp.GetCreater() == CREATE_BY_GBS_FOREIGN_DOMAIN)
			{
				cRsp.SetErrorCode(ERR_TAS_INVALID_OPERATION); // 不允许CU操作GBS创建的域设备组
				break;
			}
		}


		vector<UUIDString> vctGroups;
		map<string, u8> cOwners;
		const string &strGroupId = cReq.GetDeviceGroupInfoKey().GetDeviceGroupID();

		GetGroupsByGroupId(strGroupId, vctGroups,strDevTreeId);

		if (bSystemDevTree)
		{
			//如果删除组是自定更新组，则该组及其子组需要取消自动更新
			GetAutoAssignDeviceGroup(strGroupId, cOwners);
			if (!cOwners.empty())
			{
				for (vector<UUIDString>::const_iterator it = vctGroups.begin(); vctGroups.end() != it; ++it)
				{
					cRsp.SetErrorCode(UnRegistAutoAssignDeviceGroup(*it));
					if (CMS_SUCCESS != cRsp.GetErrorCode())
					{
						break;
					}
				}
			}
			if (CMS_SUCCESS != cRsp.GetErrorCode())
			{
				break;
			}
		}

		// 所有设备重新回到根组下面 [3/9/2016 pangubing]
		string strRootGrpId=strDevTreeId.empty()?CurrentDomainId():strDevTreeId;

		for (vector<UUIDString>::const_iterator it = vctGroups.begin(); vctGroups.end() != it; ++it)
		{
			cRsp.SetErrorCode(DevGrpDel(*it, strRootGrpId,strDevTreeId));
			if (CMS_SUCCESS != cRsp.GetErrorCode())
			{
				break;
			}
		}
		if (CMS_SUCCESS != cRsp.GetErrorCode())
		{
			break;
		}
		
		
		cNtf.SetDomainId(cDevGrp.GetDomainUUID());
		cNtf.GetDeviceGroupInfoKey().SetDeviceGroupID(cReq.GetDeviceGroupInfoKey().GetDeviceGroupID());
		cNtf.GetDeviceGroupInfoKey().SetDevtreeId(strDevTreeId);

		if (bSystemDevTree)
		{
			bHasNtf = true;
			 SyncAnalyse(enSyncDataDeviceGroup, enSyncOperateDel, CurrentDomainId(), &cReq.GetDeviceGroupInfoKey().GetDeviceGroupID());
			 UpdateDevTreeVer("");
		}
		else
		{
			UpdateTreesVerByDevId("");
		}

    } while (false);

    PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
    if (bHasNtf) PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cNtf, cTransNtf);
}

void CDbOpr::DevGrpMod( const CTaskMsgTransfer<CDeviceGroupModReq> *pcMsg )
{
    CTaskMsgTransfer<CDeviceGroupModRsp> cTrans;
    CTaskMsgTransfer<CDeviceGroupModNtfReq> cTransNtf;
    CDeviceGroupModRsp &cRsp = cTrans.GetMsgObj();
    CDeviceGroupModNtfReq &cNtf = cTransNtf.GetMsgObj();
    bool bHasNtf = false;

    CDeviceGroupModReq cReq = pcMsg->GetMsgObj();
	string strDevTreeId;
	cReq.GetDevTreeId(strDevTreeId);
	bool bSystemDevTree = strDevTreeId.empty();

	// 由于CU不管修改什么信息都是把所有信息都发给平台，因此，这里增加信息校验 [3/17/2016 pangubing]
    do 
    {
        UUIDString cGrpId;
        CDeviceGroupInfo cInfo;

        cReq.GetDeviceGroupID(cGrpId);
        cRsp.SetErrorCode(DevGrpItemQry(cGrpId, cInfo,strDevTreeId));
        if (CMS_SUCCESS != cRsp.GetErrorCode())
        {
            break;
        }

		// 上级组可以修改下级根组组的序号及GBS创建的组，并且只允许修改序号 [3/24/2016 pangubing]
		TSqlString tSql;
		string strUserId;
		if (cReq.GetGrpSeq(m_fFieldValue)&&((cInfo.GetDomainUUID() != CurrentDomainId()&&cInfo.GetDeviceParentGroupID()==CurrentDomainId())
			|| (GetCurrentCuserId(cReq.GetSession(), strUserId)&&cInfo.GetCreater() == CREATE_BY_GBS_FOREIGN_DOMAIN)))
		{
            /* 只允许修改序号 */
            string strTmp;
            if (cReq.GetDeviceParentGroupID(strTmp) && strTmp != cInfo.GetDeviceParentGroupID())
            {
                cRsp.SetErrorCode(ERR_TAS_INVALID_OPERATION); 
                break;
            }
            if (cReq.GetDeviceGroupName(strTmp) && strTmp != cInfo.GetDeviceGroupName())
            {
                if (strTmp == CutDevGrpGbId(cInfo.GetDeviceGroupName()))
                {
                    cReq.SetDeviceGroupName(cInfo.GetDeviceGroupName());
                }
                else
                {
                    cRsp.SetErrorCode(ERR_TAS_INVALID_OPERATION); 
				    break;
                }
            }
            if (cReq.GetDeviceGroupDesc(strTmp) && strTmp != cInfo.GetDeviceGroupDesc())
            {
                cRsp.SetErrorCode(ERR_TAS_INVALID_OPERATION); 
                break;
            }
			
			tSql.Format("update %s set ", DbStr(GetTblDevGrpName(strDevTreeId)));
			AppendUpdateValuePair(cReq.GetGrpSeq, m_fFieldValue, tSql, F_DEVGROUP_GRPSEQ);

		}
		else
		{
			if (cInfo.GetDomainUUID() != CurrentDomainId() 	|| cInfo.IsAuthorizationDevGrp())
			{
				cRsp.SetErrorCode(ERR_TAS_INVALID_OPERATION); // 非本域组不能修改，跨域授权组不允许修改
				break;
			}


			if (GetCurrentCuserId(cReq.GetSession(), strUserId) && cInfo.GetCreater() == CREATE_BY_GBS_FOREIGN_DOMAIN)
			{
				cRsp.SetErrorCode(ERR_TAS_INVALID_OPERATION); // 不允许CU操作GBS创建的域设备组
				break;
			}

			UUIDString strDevParentGroupId;
			CDeviceGroupInfo cNewDevParentGrp;
			if (cReq.GetDeviceParentGroupID(strDevParentGroupId)&&strDevParentGroupId!=cInfo.GetDeviceParentGroupID())
			{
				// 默认组（根组）没有父组 [7/8/2015 pangubing]
				if ((bSystemDevTree&&cInfo.GetDeviceGroupID() != CurrentDomainId())||(!bSystemDevTree&&cInfo.GetDeviceGroupID() != strDevTreeId))
				{
					cRsp.SetErrorCode(DevGrpItemQry(strDevParentGroupId, cNewDevParentGrp, strDevTreeId));
					if (CMS_SUCCESS != cRsp.GetErrorCode())
					{
						break;

					}

					if (cNewDevParentGrp.GetDomainUUID() != cInfo.GetDomainUUID())
					{
						cRsp.SetErrorCode(ERR_TAS_INVALID_OPERATION); // 不能跨域设置上级组
						break;
					}


					// 先更新子组的fullpath [3/9/2016 pangubing]
					string oldFullpath=cInfo.GetFullpath()+cInfo.GetDeviceGroupID()+"/";
					string newFullPath=cNewDevParentGrp.GetFullpath()+cNewDevParentGrp.GetDeviceGroupID()+"/"+cInfo.GetDeviceGroupID()+"/";

					TSqlString tSqlTmp;
					tSqlTmp.Format("update %s set %s=REPLACE(%s,'%s','%s') where %s LIKE '%s%%'", DbStr(GetTblDevGrpName(strDevTreeId)),F_DEVGROUP_FULLPATH,F_DEVGROUP_FULLPATH,
						DbStr(oldFullpath),DbStr(newFullPath),F_DEVGROUP_FULLPATH,DbStr(oldFullpath));

					if (!ExecSql(tSqlTmp, enExecNoRet))
					{
						cRsp.SetErrorCode(ERR_TAS_DBERROR);
						CLASSLOG(enLogSrcDbTaskPool, ERROR_LEV, "update [%s] child groups fullpath failed\n", cGrpId.c_str());
						break;
					}
				}
			}
			else
			{
				strDevParentGroupId = cInfo.GetDeviceParentGroupID();
			}

			string strNewDevGrpName;
			if (cReq.GetDeviceGroupName(strNewDevGrpName) && strNewDevGrpName!=cInfo.GetDeviceGroupName()&&
				IsExistSameNameDevGrp(strNewDevGrpName, strDevParentGroupId, &cGrpId,strDevTreeId))
			{
				cRsp.SetErrorCode(ERR_TAS_EXIST_SAMENAME_RECORD);
				break;
			}

			tSql.Format("update %s set ", DbStr(GetTblDevGrpName(strDevTreeId)));
			AppendUpdateValuePair(cReq.GetDeviceParentGroupID, m_strFieldValue, tSql, F_DEVGROUP_PARENTDEVGROUPID);
			AppendUpdateValuePair(cReq.GetDeviceGroupName, m_strFieldValue, tSql, F_DEVGROUP_GROUPNAME);
			AppendUpdateValuePair(cReq.GetDeviceGroupDesc, m_strFieldValue, tSql, F_DEVGROUP_GROUPDESC);
			AppendUpdateValuePair(cReq.GetGrpSeq, m_fFieldValue, tSql, F_DEVGROUP_GRPSEQ);

			if (cReq.GetDeviceParentGroupID(strDevParentGroupId)&&strDevParentGroupId!=cInfo.GetDeviceParentGroupID())
			{
				string fullpath=cNewDevParentGrp.GetFullpath()+cNewDevParentGrp.GetDeviceGroupID()+"/";
				tSql.AppendString("%s = '%s'", F_DEVGROUP_FULLPATH, DbStr(fullpath));
				// 如果没有设置序号，则重置序号为0 [3/9/2016 pangubing]
				if (!cReq.GetGrpSeq(m_fFieldValue))
				{
					tSql.AppendString(",%s = %.14lf", F_DEVGROUP_GRPSEQ,0.0);
				}
			}


		}
		
        tSql.EraseLastRedundantComma();

        tSql.AppendString(" where %s = '%s'", F_DEVGROUP_DEVGROUPID, DbStr(cGrpId));
        if (ExecSql(tSql, enExecNoRet))
        {
            bHasNtf = (CMS_SUCCESS == DevGrpItemQry(cGrpId, cNtf.GetDeviceGroupInfo(),strDevTreeId));
			cNtf.GetDeviceGroupInfo().SetDevtreeId(strDevTreeId);
        }
        else
        {
            cRsp.SetErrorCode(ERR_TAS_DBERROR);
        }

		if (bSystemDevTree)
		{
			SyncAnalyse(enSyncDataDeviceGroup, enSyncOperateMod, CurrentDomainId(), &cGrpId);
			UpdateDevTreeVer("");
		}
		else
		{
			bHasNtf = false;
			UpdateTreesVerByDevId("");
		}

    } while (false);
    
    PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
    if (bHasNtf) PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cNtf, cTransNtf);
}


ENTasErrno CDbOpr::DomainAuthorizationDevGrpId( string &strId )
{
    TSqlString tSql;
    tSql.Format("select %s from %s where %s='%s'", 
        F_DEVGROUP_DEVGROUPID, T_DEVGROUP, 
        F_DEVGROUP_GROUPNAME, DOMAIN_AUTHORIZATION_DEVGRPNAME);

    if (ExecSql(tSql, enExecMustRet))
    {
        strId = GetString(F_DEVGROUP_DEVGROUPID);
        return CMS_SUCCESS;
    }
    else
    {
        return ERR_TAS_DBERROR;
    }
}


ENTasErrno CDbOpr::MoveupDevGrps( const UUIDString &grpId, const UUIDString &parentGrpId )
{
    TSqlString tSql;

    tSql.Format("update %s set %s='%s' where %s='%s'", T_DEVGROUP, 
        F_DEVGROUP_PARENTDEVGROUPID, DbStr(parentGrpId),
        F_DEVGROUP_PARENTDEVGROUPID, DbStr(grpId));
    if (!ExecSql(tSql, enExecNoRet))
    {
        return ERR_TAS_DBERROR;
    }

    return CMS_SUCCESS;
}

void CDbOpr::DevGrpQry( const CTaskMsgTransfer<CDeviceGroupQryReq> *pcMsg )
{
    // 拥有两个子容器，不使用DatabaseQueryTemplate模板方式实现

    CTaskMsgTransfer<CDeviceGroupQryRsp> cTrans;
    CDeviceGroupQryRsp &cRsp = cTrans.GetMsgObj();
    CTaskMsgTransfer<CDeviceGroupQryNtfReq> cTransNtf;
    CDeviceGroupQryNtfReq &cNtf = cTransNtf.GetMsgObj();

    const CDeviceGroupQryReq &cReq = pcMsg->GetMsgObj();
    TSqlString tSql, tSqlCount;

    do 
    {
        ENTasErrno enErr = PrepareDevGrpQrySql(cReq, tSql, tSqlCount);
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
		
		string strTreeId;
		bool bCanBeUse4Gb = false;
		if (cReq.GetDeviceGroupInfoQueryCondition().GetDevTreeId(strTreeId))
		{
			bCanBeUse4Gb = IsDevTreeCanBeUse4Gb(strTreeId);
		}

        if (!ExecSql(tSql, enExecMayRet))
        {
            cRsp.SetErrorCode(ERR_TAS_DBERROR);
            break;
        }

        u32 dwRecordcount = 0;
        u32 RecordNumInNtf = 0;
        u32 dwNtfContentSize = 0;
        CDeviceGroupInfo cInfo;
		cReq.GetDeviceGroupInfoQueryCondition().GetDevTreeId(cInfo.GetDevtreeId());
        bool bExistDev;
        // 算法： 循环里面一条一条的获取DB记录 --> 1条DB记录为一条完整的业务记录 --> 组装N条业务记录为一个NTF消息; 循环之外处理最后一条消息
        while (IsNotEof())
        {
            // 获取一条DB记录
            FetchDevGrpQryRecord(cInfo, bExistDev, bCanBeUse4Gb);
            if (!MoveNext())
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
                cNtf.GetDeviceGroupInfo().push_back(cInfo);
                cNtf.GetDeviceGroupHasDevice().push_back(bExistDev);
            }
            else
            {
                cNtf.SetEntryNum(RecordNumInNtf);
                cNtf.SetLastNtf(false);
                PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cNtf, cTransNtf);

                dwRecordcount += RecordNumInNtf;

                cNtf.GetDeviceGroupInfo().clear();
                cNtf.GetDeviceGroupHasDevice().clear();
                cNtf.GetDeviceGroupInfo().push_back(cInfo);
                cNtf.GetDeviceGroupHasDevice().push_back(bExistDev);
                dwNtfContentSize = dwRecordContentSize;
                RecordNumInNtf = 1;
            }
        }

        // 发送组装好尚未发送的最后一个NTF消息
        if (cRsp.GetErrorCode() == CMS_SUCCESS && !cInfo.GetDeviceGroupID().empty())
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

ENTasErrno CDbOpr::PrepareDevGrpQrySql( const CDeviceGroupQryReq &cReq, TSqlString &tSqlSelect, TSqlString &tSqlCount )
{
    const CDeviceGroupInfoQueryCondition &cCond = cReq.GetDeviceGroupInfoQueryCondition();
    TSqlString tSqlCond;

	string strDevTreeId;
	cCond.GetDevTreeId(strDevTreeId);

    UUIDString cuserId;
    if (cCond.GetCuserID(cuserId))
    {
        CUserInfo cUserInfo;
        if (!GetCuserInfo(cuserId, cUserInfo))
        {
            return ERR_TAS_USER_NOT_EXIST;
        }

        if (cUserInfo.GetUsergrpID().empty())
        {
            tSqlSelect.Format("select a.*, \
                exists(select * from %s b where b.%s=a.%s \
                and (exists(select * from %s d where d.%s=b.%s and d.%s=2) or \
                exists(select * from %s c where c.%s='%s' and c.%s=b.%s))) existflag, coalesce(e.%s, '') %s \
                from %s a left join %s e on a.%s = e.%s ", 
                DbStr(GetTblDevGrp2DevName(strDevTreeId)), F_DEVGRP2DEV_DEVGROUPID, F_DEVGROUP_DEVGROUPID,
                T_DEVICE, F_DEVICE_DEVID, F_DEVGRP2DEV_DEVID, F_DEVICE_DEVTYPE,
                T_CUSER2DEV, F_CUSER2DEV_CUSERID, DbStr(cuserId), F_CUSER2DEV_DEVID, F_DEVGRP2DEV_DEVID, 
				F_DEVGROUP_DOMAINID, F_DEVGROUP_PARENTDOMAINID, DbStr(GetTblDevGrpName(strDevTreeId)), DbStr(GetTblDevGrpName(strDevTreeId)), 
				F_DEVGROUP_PARENTDEVGROUPID, F_DEVGROUP_DEVGROUPID);
			tSqlCount.Format("select count(*) from %s a ", DbStr(GetTblDevGrpName(strDevTreeId)));
        }
        else
        {
            tSqlSelect.Format("select a.*, \
                              exists(select * from %s b where b.%s=a.%s \
                              and (exists(select * from %s d where d.%s=b.%s and d.%s=2) or \
                              exists(select * from %s c where c.%s='%s' and c.%s=b.%s))) existflag, coalesce(e.%s, '') %s \
                              from %s a left join %s e on a.%s = e.%s ", 
                              DbStr(GetTblDevGrp2DevName(strDevTreeId)), F_DEVGRP2DEV_DEVGROUPID, F_DEVGROUP_DEVGROUPID,
                              T_DEVICE, F_DEVICE_DEVID, F_DEVGRP2DEV_DEVID, F_DEVICE_DEVTYPE,
                              T_CUSERGRP2DEV, F_CUSERGRP2DEV_CUSERGROUPID, DbStr(cUserInfo.GetUsergrpID()), 
                              F_CUSERGRP2DEV_DEVID, F_DEVGRP2DEV_DEVID, F_DEVGROUP_DOMAINID, 
							  F_DEVGROUP_PARENTDOMAINID, DbStr(GetTblDevGrpName(strDevTreeId)), DbStr(GetTblDevGrpName(strDevTreeId)), 
							  F_DEVGROUP_PARENTDEVGROUPID, F_DEVGROUP_DEVGROUPID);
			tSqlCount.Format("select count(*) from %s a ", DbStr(GetTblDevGrpName(strDevTreeId)));
        }
    }
    else
    {
        
        tSqlSelect.Format("select a.*, exists(select * from %s b where b.%s=a.%s) existflag, coalesce(c.%s, '') %s \
                          from %s a left join %s c on a.%s = c.%s ", 
						  DbStr(GetTblDevGrp2DevName(strDevTreeId)), F_DEVGRP2DEV_DEVGROUPID, F_DEVGROUP_DEVGROUPID, F_DEVGROUP_DOMAINID, 
						  F_DEVGROUP_PARENTDOMAINID, DbStr(GetTblDevGrpName(strDevTreeId)), DbStr(GetTblDevGrpName(strDevTreeId)), F_DEVGROUP_PARENTDEVGROUPID, F_DEVGROUP_DEVGROUPID);
        tSqlCount.Format("select count(*) from %s a ", DbStr(GetTblDevGrpName(strDevTreeId)));

    }
    AppendQryCondValuePair(cCond.GetDeviceGroupID, m_strFieldValue, tSqlCond, F_DEVGROUP_DEVGROUPID, a);
    AppendQryCondValuePair(cCond.GetDeviceParentGroupID, m_strFieldValue, tSqlCond, F_DEVGROUP_PARENTDEVGROUPID, a);
    AppendQryCondValuePair(cCond.GetDomainUUID, m_strFieldValue, tSqlCond, F_DEVGROUP_DOMAINID, a);
    AppendQryCondValuePair(cCond.GetDeviceGroupName, m_strFieldValue, tSqlCond, F_DEVGROUP_GROUPNAME, a);
    AppendQryCondValuePair(cCond.GetDeviceGroupDesc, m_strFieldValue, tSqlCond, F_DEVGROUP_GROUPDESC, a);
    AppendQryCondDatePeriodPair(cCond.GetCreateDate_From, m_strFieldValue, cCond.GetCreateDate_To, m_strFieldValue2,
        tSqlCond, F_DEVGROUP_CREATEDATE, a);
	AppendQryCondValuePair(cCond.GetOwner, m_strFieldValue, tSqlCond, F_DEVGROUP_OWNER, a);
    
	set<TCreater> creaters;
	if (cCond.GetCreaters(creaters) && 0 < creaters.size())		// 指定了创建者集合
	{
		tSqlCond.AppendString("(");
		for (set<TCreater>::iterator it = creaters.begin(); it != creaters.end(); ++it)
		{
			if (it != creaters.begin()) 
				tSqlCond.AppendString(" or ");
			tSqlCond.AppendString("a.%s=%d", F_DEVGROUP_CREATER, *it);
		}
		tSqlCond.AppendString(") and ");
	}

    tSqlCond.EraseLastRedundantAnd();

    if (!tSqlCond.Empty())
    {
        tSqlSelect.AppendString(" where ");
        tSqlSelect.AppendString("%s", tSqlCond.GetBuf());
        tSqlCount.AppendString(" where ");
        tSqlCount.AppendString("%s", tSqlCond.GetBuf());
    }

    tSqlSelect.AppendString(" order by a.%s, a.%s , convert(a.%s using gbk)", F_DEVGROUP_FULLPATH, F_DEVGROUP_GRPSEQ,F_DEVGROUP_GROUPNAME);
    AddRowNumLimitModifer(tSqlSelect, cReq.GetStartEntryIndex(), cReq.GetExpectEntryNum());
    return CMS_SUCCESS;
}

void CDbOpr::FetchDevGrpQryRecord( CDeviceGroupInfo &cInfo, bool &bExistDev, bool CanBeUse4Gb )
{
	cInfo.SetDeviceGroupID(GetString(F_DEVGROUP_DEVGROUPID));
	cInfo.SetDomainUUID(GetString(F_DEVGROUP_DOMAINID));
	cInfo.SetDeviceParentGroupID(GetString(F_DEVGROUP_PARENTDEVGROUPID));
	cInfo.SetParentDomainUUID(GetString(F_DEVGROUP_PARENTDOMAINID));				// 父域Id
	cInfo.SetDeviceGroupName(GetString(F_DEVGROUP_GROUPNAME));
	cInfo.SetDeviceGroupDesc(GetString(F_DEVGROUP_GROUPDESC));
	cInfo.SetCreateDate(GetCmsDate(F_DEVGROUP_CREATEDATE));
	cInfo.SetCreater(GetInt(F_DEVGROUP_CREATER));
	cInfo.SetOwner(GetString(F_DEVGROUP_OWNER));
	cInfo.SetFullpath(GetString(F_DEVGROUP_FULLPATH));
	cInfo.SetGrpseq(GetDouble(F_DEVGROUP_GRPSEQ));
	if (CanBeUse4Gb)
	{
		cInfo.SetOldgbid(GetString(F_DEVGROUP_OLDGBID));
		cInfo.SetNewgbid(GetString(F_DEVGROUP_NEWGBID));
		cInfo.SetGrpType(IsDevGrpCivilCode(cInfo.GetNewgbid())?TDEV_GROUP_TYPE_CIVILCODE:TDEV_GROUP_TYPE_VIRTUALGRP);
	}
    bExistDev = GetBool("existflag");
}


bool CDbOpr::IsDevGrpCivilCode(const string& strGrpGbid)
{
	return strGrpGbid.size()<=2*GetMaxCivilcodeLev();
}


bool CDbOpr::IsDevGrpBussinessGrp(const string& strGrpGbid)
{
	return strGrpGbid.size()==20 && "215" == strGrpGbid.substr(10,3);
}


void CDbOpr::FetchDevGrpQryRecord( CDeviceGroupInfo &cInfo, bool CanBeUse4Gb )
{
	cInfo.SetDeviceGroupID(GetString(F_DEVGROUP_DEVGROUPID));
	cInfo.SetDomainUUID(GetString(F_DEVGROUP_DOMAINID));
	cInfo.SetDeviceParentGroupID(GetString(F_DEVGROUP_PARENTDEVGROUPID));
	cInfo.SetParentDomainUUID(GetString(F_DEVGROUP_PARENTDOMAINID));				// 父域Id
	cInfo.SetDeviceGroupName(GetString(F_DEVGROUP_GROUPNAME));
	cInfo.SetDeviceGroupDesc(GetString(F_DEVGROUP_GROUPDESC));
	cInfo.SetCreateDate(GetCmsDate(F_DEVGROUP_CREATEDATE));
	cInfo.SetCreater(GetInt(F_DEVGROUP_CREATER));
	cInfo.SetOwner(GetString(F_DEVGROUP_OWNER));
	cInfo.SetFullpath(GetString(F_DEVGROUP_FULLPATH));
	cInfo.SetGrpseq(GetDouble(F_DEVGROUP_GRPSEQ));
	if (CanBeUse4Gb)
	{
		cInfo.SetOldgbid(GetString(F_DEVGROUP_OLDGBID));
		cInfo.SetNewgbid(GetString(F_DEVGROUP_NEWGBID));
	}
}


ENTasErrno CDbOpr::DevGrpItemQry( const UUIDString &strId, CDeviceGroupInfo &cInfo ,const string& devTreeId)
{
	bool bCanBeUse4Gb = IsDevTreeCanBeUse4Gb(devTreeId);
    TSqlString tSql;
    tSql.Format("select a.*, 0 existflag, coalesce(b.%s, '') %s from %s a left join %s b on a.%s = b.%s where a.%s='%s'", 
		F_DEVGROUP_DOMAINID, F_DEVGROUP_PARENTDOMAINID, DbStr(GetTblDevGrpName(devTreeId)), DbStr(GetTblDevGrpName(devTreeId)), 
		F_DEVGROUP_PARENTDEVGROUPID, F_DEVGROUP_DEVGROUPID, F_DEVGROUP_DEVGROUPID, DbStr(strId));

    if (ExecSql(tSql, enExecMayRet))
    {
        if (IsEof())
        {
            return ERR_TAS_RECORD_NOT_EXISTS;
        }

        bool bTemp;
        FetchDevGrpQryRecord(cInfo, bTemp, bCanBeUse4Gb);
        return CMS_SUCCESS;
    }
    else
    {
        return ERR_TAS_DBERROR;
    }
}

ENTasErrno CDbOpr::DevGrpItemQryOnly( const UUIDString &strId, CDeviceGroupInfo &cInfo,const string& devTreeId/*=""*/ )
{
	bool bCanBeUse4Gb = IsDevTreeCanBeUse4Gb(devTreeId);
	TSqlString tSql;
	//tSql.Format("select * from %s where %s='%s'", DbStr(GetTblDevGrpName(devTreeId)), F_DEVGROUP_DEVGROUPID, DbStr(strId));
	tSql.Format("select a.*, coalesce(b.%s, '') %s from %s a left join %s b on a.%s = b.%s where a.%s='%s'", 
		F_DEVGROUP_DOMAINID, F_DEVGROUP_PARENTDOMAINID, DbStr(GetTblDevGrpName(devTreeId)), DbStr(GetTblDevGrpName(devTreeId)), 
		F_DEVGROUP_PARENTDEVGROUPID, F_DEVGROUP_DEVGROUPID, F_DEVGROUP_DEVGROUPID, DbStr(strId));

	if (ExecSql(tSql, enExecMayRet))
	{
		if (IsNotEof())
		{
			FetchDevGrpQryRecord(cInfo, bCanBeUse4Gb);
			return CMS_SUCCESS;
		}
		else
		{
			return ERR_TAS_RECORD_NOT_EXISTS;
		}
	}
	else
	{
		return ERR_TAS_DBERROR;
	}

}


ENTasErrno CDbOpr::DevGrpItemAdd( const CDeviceGroupInfo &cInfo,const string& devTreeId)
{
    TSqlString tSql;

	if (IsDevTreeCanBeUse4Gb(devTreeId))
	{
		tSql.Format("insert into %s values('%s', '%s', '%s', '%s', '%s', '%s', %d, '%s','%s',%.14lf,'%s','%s')", 
			DbStr(GetTblDevGrpName(devTreeId)), DbStr(cInfo.GetDeviceGroupID()), DbStr(cInfo.GetDomainUUID()), DbStr(cInfo.GetDeviceParentGroupID()), 
			DbStr(cInfo.GetDeviceGroupName()), DbStr(cInfo.GetDeviceGroupDesc()), DbDateTime(cInfo.GetCreateDate()), 
			(s32)cInfo.GetCreater(), DbStr(cInfo.GetOwner()),DbStr(cInfo.GetFullpath()),cInfo.GetGrpseq(),DbStr(cInfo.GetOldgbid()),DbStr(cInfo.GetNewgbid()));
	}
	else
	{
		tSql.Format("insert into %s values('%s', '%s', '%s', '%s', '%s', '%s', %d, '%s','%s',%.14lf)", 
			DbStr(GetTblDevGrpName(devTreeId)), DbStr(cInfo.GetDeviceGroupID()), DbStr(cInfo.GetDomainUUID()), DbStr(cInfo.GetDeviceParentGroupID()), 
			DbStr(cInfo.GetDeviceGroupName()), DbStr(cInfo.GetDeviceGroupDesc()), DbDateTime(cInfo.GetCreateDate()), 
			(s32)cInfo.GetCreater(), DbStr(cInfo.GetOwner()),DbStr(cInfo.GetFullpath()),cInfo.GetGrpseq());
	}
    

    if (ExecSql(tSql, enExecNoRet))
    {
        return CMS_SUCCESS;
    }
    else
    {
        return ERR_TAS_DBERROR;
    }
}

ENTasErrno CDbOpr::DevGrpItemDel( const UUIDString &strDevGrpId,const string &devTreeId )
{
    TSqlString tSql;
    tSql.Format("delete from %s where %s = '%s'", DbStr(GetTblDevGrpName(devTreeId)), F_DEVGROUP_DEVGROUPID, DbStr(strDevGrpId));
    if (ExecSql(tSql, enExecNoRet))
    {
        return CMS_SUCCESS;
    }
    else
    {
        return ERR_TAS_DBERROR;
    }
}

ENTasErrno CDbOpr::DevGrpItemMod( const CDeviceGroupInfo &cInfo )
{
    TSqlString tSql;
    tSql.Format("update %s set %s='%s', %s='%s', %s='%s', %s='%s', %s='%s' , %s='%s' , %s=%.14lf \
                where %s='%s'", T_DEVGROUP, F_DEVGROUP_DOMAINID, DbStr(cInfo.GetDomainUUID()),
                F_DEVGROUP_PARENTDEVGROUPID, DbStr(cInfo.GetDeviceParentGroupID()), 
                F_DEVGROUP_GROUPNAME, DbStr(cInfo.GetDeviceGroupName()),
                F_DEVGROUP_GROUPDESC, DbStr(cInfo.GetDeviceGroupDesc()), 
                F_DEVGROUP_CREATEDATE, DbDateTime(cInfo.GetCreateDate()),
				F_DEVGROUP_FULLPATH, DbStr(cInfo.GetFullpath()),
				F_DEVGROUP_GRPSEQ,cInfo.GetGrpseq(),
                F_DEVGROUP_DEVGROUPID, DbStr(cInfo.GetDeviceGroupID())
				);

    if (ExecSql(tSql, enExecNoRet))
    {
        return CMS_SUCCESS;
    }
    else
    {
        return ERR_TAS_DBERROR;
    }
}

bool CDbOpr::IsExistSameNameDevGrp( const string &strName, const string &strParentDevGrpId,const string *pstrExcludeDevGrpId ,const string& devTreeId )
{
    TSqlString tSql;
    tSql.Format("select count(*) from %s a where a.%s='%s' and a.%s='%s'", DbStr(GetTblDevGrpName(devTreeId)), 
        F_DEVGROUP_PARENTDEVGROUPID, DbStr(strParentDevGrpId), F_DEVGROUP_GROUPNAME, DbStr(strName));

    if (ExecSql(tSql, enExecMustRet))
    {
        return GetIntResult() > 0;
    }
    else
    {
        // 失败时返回存在，好暴露问题
        return true;
    }
}

void CDbOpr::DevGrpDevAdd( const CTaskMsgTransfer<CDeviceGroupDeviceAddReq> *pcMsg )
{
	DevGrpDevAdd(pcMsg,false);
}

ENTasErrno CDbOpr::DevGrpDevAdd(const CTaskMsgTransfer<CDeviceGroupDeviceAddReq> *pcMsg,bool bAddDevWithGrp)
{
	CTaskMsgTransfer<CDeviceGroupDeviceAddRsp> cTrans;
	CTaskMsgTransfer<CDeviceGroupDeviceAddNtfReq> cTransNtf;
	CDeviceGroupDeviceAddRsp &cRsp = cTrans.GetMsgObj();
	CDeviceGroupDeviceAddNtfReq &cNtf = cTransNtf.GetMsgObj();
	bool bHasNtf = false;

	const CDeviceGroupDeviceAddReq &cReq = pcMsg->GetMsgObj();
	const string& strDevTreeId=cReq.GetDeviceGroupDeviceInfo().GetDevDevTreeId();
	const string& strGrpTreeId=cReq.GetDeviceGroupDeviceInfo().GetGrpDevTreeId();
	/*
	 1、同一设备树内操作（剪切）：从源组删除，目的组添加
	 2、不同设备树之间操作（复制）：直接在目的组添加
	 3、目前自定义设备树只支持2（不同设备树之间的操作）
	*/
	bool bSystemDevTree=(strDevTreeId.empty()&&strGrpTreeId.empty());
	do 
	{
		const CDeviceGroupDeviceInfo &cInfo = cReq.GetDeviceGroupDeviceInfo();

		if (!strDevTreeId.empty()&&strGrpTreeId.empty())
		{
			cRsp.SetErrorCode(ERR_TAS_INVALID_OPERATION);
			break;
		}

		CDeviceGroupInfo cDevGrpInfo;
		cRsp.SetErrorCode(DevGrpItemQry(cInfo.GetDeviceGroupID(), cDevGrpInfo, strGrpTreeId));
		if (CMS_SUCCESS != cRsp.GetErrorCode())
		{
			break;
		}

		CDeviceInfo cDevInfo;
		cRsp.SetErrorCode(DeviceItemQry(cInfo.GetDeviceID(), cDevInfo));
		if (CMS_SUCCESS != cRsp.GetErrorCode())
		{
			break;
		}

		//判断设备是否已在该目的设备组里（2个不同的cu都使用admin登录，cu上禁止把同一视频源拖拽到同一分组的限定就会被攻破）
		for (int i =0; i<cInfo.GetDeviceCapIndexs().size(); ++i)
		{
			if (cInfo.GetDeviceCapIndexs()[i].id == DEVICE_CAP_VIDEOSOURCE)
			{
				for (int j = 0; j<cInfo.GetDeviceCapIndexs()[i].indexs.size(); ++j)
				{
					if (IsVidSrcInDevGrp(cInfo.GetGrpDevTreeId(), cInfo.GetDeviceGroupID(), cInfo.GetDeviceID(), cInfo.GetDeviceCapIndexs()[i].indexs[j]))
					{
						cRsp.SetErrorCode(ERR_TAS_RECORD_EXISTS);
						break;
					}
				}
			}
		}
		if (CMS_SUCCESS != cRsp.GetErrorCode())
		{
			break;
		}



		if (bSystemDevTree)
		{
			if (cDevGrpInfo.GetDomainUUID() != cDevInfo.GetDomainUUID() || cDevGrpInfo.GetDomainUUID() != CurrentDomainId())
			{
				cRsp.SetErrorCode(ERR_TAS_INVALID_OPERATION);
				break;
			}

			if (cDevGrpInfo.IsAuthorizationDevGrp())
			{
				// 跨域授权组下不允许手工添加设备
				cRsp.SetErrorCode(ERR_TAS_INVALID_OPERATION);
				break;
			}
		}



		// 设备带设备组入网，不需要取消划归，提高效率 [11/23/2015 pangubing]
		if (!bAddDevWithGrp)
		{
			if (bSystemDevTree)
			{
				// 将设备从源组中移除之前先更新原拥有者的设备树版本信息（否则过后设备属于新的拥有者了，就没有办法更新原拥有者的版本信息了）
				UpdateDevTreeVer(cInfo.GetDeviceID());

				// 先从源组对应的用户划归信息中清除出该设备信息
				AutoRevokeDevice(pcMsg, cReq, cInfo.GetDeviceID(), cInfo.GetDeviceCapIndexs(), string());
			}
		}
		

		cRsp.SetErrorCode(DevGrpDevItemAdd(cInfo,bAddDevWithGrp,strGrpTreeId,strDevTreeId));
		if (cRsp.GetErrorCode() != CMS_SUCCESS)
		{
			break;
		}


		if (bSystemDevTree)
		{
			// 再根据目标组添加新的用户划归信息
			AutoAssignDevice(pcMsg, cReq, cInfo.GetDeviceID(), cInfo.GetDeviceCapIndexs(), cInfo.GetDeviceGroupID());
		    double dDevSeq;
			string strDevTreeId;
			for (u32 i = 0; i < cInfo.GetDeviceCapIndexs().size(); ++i)
			{
				for (u32 j = 0; j < cInfo.GetDeviceCapIndexs()[i].indexs.size(); ++j)
				{
					SyncAnalyse(enSyncDataDeviceGroup2Dev, enSyncOperateAdd, CurrentDomainId(), 
						&cInfo.GetDeviceGroupID(),
						&cInfo.GetDeviceID(),
						cInfo.GetDeviceCapIndexs()[i].id, 
						cInfo.GetDeviceCapIndexs()[i].indexs[j], cDevInfo.GetDeviceType());
				}
			}

			UpdateDevTreeVer(cInfo.GetDeviceID());

			bHasNtf = true;
			cNtf.SetDeviceGroupDeviceInfo(cInfo);
			cNtf.GetDeviceGroupDeviceInfo().SetDomainId(cDevInfo.GetDomainUUID());
		}
		else
		{
			UpdateTreesVerByDevId("");
		}

	} while (false);

	if (!bAddDevWithGrp)
	{
		PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
	}
	
	if (bHasNtf)
	{
		PostTaskMsgTransfer2CCtrl4LargeNtf(CDeviceGroupDeviceAddNtfReq, GetDeviceGroupDeviceInfo, GetDeviceCapIndexs)
	}

	return (ENTasErrno)cRsp.GetErrorCode();
}

ENTasErrno CDbOpr::DevGrpDevAddDefault( const UUIDString &devId, const UUIDString &devModelId, const string *pstrCertainDomainId )
{
    string strCertainDomainId = (pstrCertainDomainId != NULL)? *pstrCertainDomainId : CurrentDomainId();

    CDeviceModelInfo cModel = g_cDbCache.GetDeviceModelInfo(devModelId, this);
    if (cModel.GetDeviceModelId().empty())
    {
        return ERR_TAS_INVALID_DEVICEMODELID;
    }
	string strDevTreeId;
    TSqlString tSql;
    if (cModel.GetDeviceTypeId() == DEVICE_TYPE_DECODER || cModel.GetDeviceTypeId() == DEVICE_TYPE_TV_WALL)
    {// 解码器，电视墙划归伪能力

		double devSeq;
		if (!GetDevGrpNextDevSeq(strDevTreeId, GetDomainUUID(), devId, -1, devSeq))
		{
			return ERR_TAS_DBERROR;
		}

        tSql.Format("INSERT INTO %s VALUES('%s', '%s', %d, %d,%.14lf)", T_DEVGRP2DEV,
            DbStr(strCertainDomainId), DbStr(devId), DEVICE_CAP_DUMMY, 0, devSeq);
        if (!ExecSql(tSql, enExecNoRet))
        {
            return ERR_TAS_DBERROR;
        }

        return CMS_SUCCESS;
    }

    vector<TDeviceCapInfo> &atCapInfos = cModel.GetDeviceCapInfo();
    for (vector<TDeviceCapInfo>::const_iterator it = atCapInfos.begin(); it != atCapInfos.end(); ++it)
    {// 编码器划归视频源
        if (it->type != DEVICE_CAP_VIDEOSOURCE)
        {
            continue;
        }

		double devSeq;
        for (u32 i = 0; i < static_cast<u32>(it->num); ++i)
        {
			if (!GetDevGrpNextDevSeq(strDevTreeId, GetDomainUUID(), devId, i, devSeq))
			{
				return ERR_TAS_DBERROR;
			}

            tSql.Format("INSERT INTO %s VALUES('%s', '%s', %d, %d, %.14lf)", T_DEVGRP2DEV,
                DbStr(strCertainDomainId), DbStr(devId), it->type, i, devSeq);
            if (!ExecSql(tSql, enExecNoRet))
            {
                return ERR_TAS_DBERROR;
            }
        }
    }

    return CMS_SUCCESS;
}


ENTasErrno CDbOpr::DevGrpDevAddDomainAuthorization( const UUIDString &devId, const UUIDString &devModelId )
{
    string strDevGrpId;
    if (CMS_SUCCESS != DomainAuthorizationDevGrpId(strDevGrpId))
    {
        return ERR_TAS_DBERROR;
    }

    CDeviceModelInfo cModel = g_cDbCache.GetDeviceModelInfo(devModelId, this);
    if (cModel.GetDeviceModelId().empty())
    {
        return ERR_TAS_INVALID_DEVICEMODELID;
    }
	UUIDString devTreeId;
	double dDevSeq;
    TSqlString tSql;
    vector<TDeviceCapInfo> &atCapInfos = cModel.GetDeviceCapInfo();
    for (vector<TDeviceCapInfo>::const_iterator it = atCapInfos.begin(); it != atCapInfos.end(); ++it)
    {
        if (it->type != DEVICE_CAP_VIDEOSOURCE)
        {
            continue;
        }

        for (u32 i = 0; i < static_cast<u32>(it->num); ++i)
        {
			if (!GetDevGrpNextDevSeq(devTreeId, strDevGrpId, devId, i, dDevSeq))
			{
				return ERR_TAS_DBERROR;
			}
            tSql.Format("INSERT INTO %s VALUES('%s', '%s', %d, %d, %.14lf)", T_DEVGRP2DEV,
                DbStr(strDevGrpId), DbStr(devId), it->type, i);
            if (!ExecSql(tSql, enExecNoRet))
            {
                return ERR_TAS_DBERROR;
            }
        }
    }

    return CMS_SUCCESS;
}

void CDbOpr::DevGrpDevDel( const CTaskMsgTransfer<CDeviceGroupDeviceDelReq> *pcMsg )
{
    CTaskMsgTransfer<CDeviceGroupDeviceDelRsp> cTrans;
    CTaskMsgTransfer<CDeviceGroupDeviceDelNtfReq> cTransNtf;
    CDeviceGroupDeviceDelRsp &cRsp = cTrans.GetMsgObj();
    CDeviceGroupDeviceDelNtfReq &cNtf = cTransNtf.GetMsgObj();
    bool bHasNtf = false;

    const CDeviceGroupDeviceDelReq &cReq = pcMsg->GetMsgObj();
    const CDeviceGroupDeviceInfoKey &cKey = cReq.GetDeviceGroupDeviceInfoKey();
	const string& strDevTreeId=cReq.GetDeviceGroupDeviceInfoKey().GetDevtreeId();
	bool bSystemDevTree=strDevTreeId.empty();
    do 
    {
        CDeviceGroupInfo cDevGrpInfo;
        cRsp.SetErrorCode(DevGrpItemQry(cKey.GetDeviceGroupID(), cDevGrpInfo,strDevTreeId));
        if (CMS_SUCCESS != cRsp.GetErrorCode())
        {
            break;
        }
        CDeviceInfo cDevInfo;
        cRsp.SetErrorCode(DeviceItemQry(cKey.GetDeviceID(), cDevInfo));
        if (CMS_SUCCESS != cRsp.GetErrorCode())
        {
            break;
        }

		if (bSystemDevTree)
		{
			if (cDevGrpInfo.GetDomainUUID() != cDevInfo.GetDomainUUID() || cDevGrpInfo.GetDomainUUID() != CurrentDomainId())
			{
				cRsp.SetErrorCode(ERR_TAS_INVALID_OPERATION);
				break;
			}

			if (cDevGrpInfo.IsAuthorizationDevGrp())
			{
				// 跨域授权组下不允许手工删除设备
				cRsp.SetErrorCode(ERR_TAS_INVALID_OPERATION);
				break;
			}

			// 将设备从源组中移除之前先更新原拥有者的设备树版本信息（否则过后设备属于新的拥有者了，就没有办法更新原拥有者的版本信息了）
			UpdateDevTreeVer(cKey.GetDeviceID());

			// 先从源组对应的用户划归信息中清除出该设备信息
			AutoRevokeDevice(pcMsg, cReq, cKey.GetDeviceID(), cKey.GetDeviceCapIndexs(), cKey.GetDeviceGroupID());
			UUIDString devTreeId;
			double dDevSeq;
			TSqlString tSql;
			const vector<TDeviceCapIndexs> tCapIndexs = cKey.GetDeviceCapIndexs();
			if (tCapIndexs.size() > 0)
			{
				for (u32 i = 0; i < tCapIndexs.size(); ++i)
				{
					for (u32 j = 0; j < tCapIndexs[i].indexs.size(); ++j)
					{
						tSql.Format("delete from %s where %s='%s' and %s='%s' and %s=%d and %s=%d", T_DEVGRP2DEV, 
							F_DEVGRP2DEV_DEVGROUPID, DbStr(cKey.GetDeviceGroupID()), F_DEVGRP2DEV_DEVID, DbStr(cKey.GetDeviceID()),
							F_DEVGRP2DEV_DEVCAPID, tCapIndexs[i].id, F_DEVGRP2DEV_DEVCAPINDEX, tCapIndexs[i].indexs[j]);
						if (!ExecSql(tSql, enExecNoRet))
						{
							cRsp.SetErrorCode(ERR_TAS_DBERROR);
							break;
						}
						if (tCapIndexs[i].id != DEVICE_CAP_VIDEOSOURCE)
						{
							dDevSeq = 0;
						}
						else 	
						{
							if (!GetDevGrpNextDevSeq(devTreeId, CurrentDomainId(), cKey.GetDeviceID(), tCapIndexs[i].indexs[j], dDevSeq))
							{
								cRsp.SetErrorCode(ERR_TAS_DBERROR);
								break;
							}
						}
						tSql.Format("insert into %s values('%s', '%s', %d, %d, %.14lf)", T_DEVGRP2DEV,
							DbStr(CurrentDomainId()), DbStr(cKey.GetDeviceID()), tCapIndexs[i].id, tCapIndexs[i].indexs[j],dDevSeq);
						if (!ExecSql(tSql, enExecNoRet))
						{
							cRsp.SetErrorCode(ERR_TAS_DBERROR);
							break;
						}

						SyncAnalyse(enSyncDataDeviceGroup2Dev, enSyncOperateDel, CurrentDomainId(), 
							&cKey.GetDeviceGroupID(),
							&cKey.GetDeviceID(),
							cKey.GetDeviceCapIndexs()[i].id, 
							cKey.GetDeviceCapIndexs()[i].indexs[j], cDevInfo.GetDeviceType());
					}

					if (cRsp.GetErrorCode() != CMS_SUCCESS)
					{
						break;
					}
				}
			}

			// 再根据目标组添加新的用户划归信息
			AutoAssignDevice(pcMsg, cReq, cKey.GetDeviceID(), cKey.GetDeviceCapIndexs(), string());

			if (cRsp.GetErrorCode() == CMS_SUCCESS)
			{
				bHasNtf = true;
				cNtf.SetDomainId(cDevInfo.GetDomainUUID());
				cNtf.SetDeviceGroupDeviceInfoKey(cKey);
			}

			UpdateDevTreeVer(cKey.GetDeviceID());
		} 
		else
		{
			TSqlString tSql;
			const vector<TDeviceCapIndexs> tCapIndexs = cKey.GetDeviceCapIndexs();
			if (tCapIndexs.size() > 0)
			{
				for (u32 i = 0; i < tCapIndexs.size(); ++i)
				{
					for (u32 j = 0; j < tCapIndexs[i].indexs.size(); ++j)
					{
						tSql.Format("delete from %s where %s='%s' and %s='%s' and %s=%d and %s=%d", DbStr(GetTblDevGrp2DevName(strDevTreeId)), 
							F_DEVGRP2DEV_DEVGROUPID, DbStr(cKey.GetDeviceGroupID()), F_DEVGRP2DEV_DEVID, DbStr(cKey.GetDeviceID()),
							F_DEVGRP2DEV_DEVCAPID, tCapIndexs[i].id, F_DEVGRP2DEV_DEVCAPINDEX, tCapIndexs[i].indexs[j]);
						if (!ExecSql(tSql, enExecNoRet))
						{
							cRsp.SetErrorCode(ERR_TAS_DBERROR);
							break;
						}
					}

					if (cRsp.GetErrorCode() != CMS_SUCCESS)
					{
						break;
					}
				}
			}

			UpdateTreesVerByDevId("");
		}
	
    } while (false);

    PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);

    if (bHasNtf&&bSystemDevTree) PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cNtf, cTransNtf);
}

ENTasErrno CDbOpr::DevGrpDevDelByDevId( const UUIDString &devId,bool bNeedDelTreeInfo/*=true*/ )
{
    TSqlString tSql;
    tSql.Format("delete from %s where %s='%s'", T_DEVGRP2DEV, F_DEVGRP2DEV_DEVID, DbStr(devId));
    if (!ExecSql(tSql, enExecNoRet))
    {
        return ERR_TAS_DBERROR;
    }

	if (bNeedDelTreeInfo)
	{
		//删除自定义设备树设备信息[3/17/2016 pangubing]
		vector<CDevTreeInfo> cDevTreeList;
		DevTreeListQry(cDevTreeList);
		for (size_t i=0;i<cDevTreeList.size();i++)
		{
			tSql.Format("delete from %s where %s='%s'", DbStr(GetTblDevGrp2DevName(cDevTreeList[i].GetDevtreeId())), 
				F_DEVGRP2DEV_DEVID, DbStr(devId));

			if (!ExecSql(tSql, enExecNoRet))
			{
				return ERR_TAS_DBERROR;
			}

			UpdateDevTreeVer("",cDevTreeList[i].GetDevtreeId());
		}
	}


    return CMS_SUCCESS;
}

ENTasErrno CDbOpr::DevGrpDevExistsTest( const UUIDString &grpId, bool &bExists )
{
    TSqlString tSql;
    tSql.Format("select count(*) from %s where %s='%s'", T_DEVGRP2DEV, 
        F_DEVGRP2DEV_DEVGROUPID, DbStr(grpId));
    if (!ExecSql(tSql, enExecMustRet))
    {
        return ERR_TAS_DBERROR;
    }
    bExists = (GetIntResult() > 0);
    return CMS_SUCCESS;
}


ENTasErrno CDbOpr::MoveupDevGrpDevs( const UUIDString &grpId, const UUIDString &parentGrpId,const string &devTreeId )
{
    TSqlString tSql;
	if (devTreeId.empty())
	{
		tSql.Format("update %s set %s='%s' where %s='%s'", DbStr(GetTblDevGrp2DevName(devTreeId)), 
			F_DEVGRP2DEV_DEVGROUPID, DbStr(parentGrpId), 
			F_DEVGRP2DEV_DEVGROUPID, DbStr(grpId));
	} 
	else
	{
		tSql.Format("delete from %s where %s='%s'", DbStr(GetTblDevGrp2DevName(devTreeId)), 
			F_DEVGRP2DEV_DEVGROUPID, DbStr(grpId));
	}

    if (!ExecSql(tSql, enExecNoRet))
    {
        return ERR_TAS_DBERROR;
    }

    return CMS_SUCCESS;
}

void CDbOpr::DevGbInfoQry(const CTaskMsgTransfer<CDeviceGbinfoQryReq> *pcMsg)
{
	CTaskMsgTransfer<CDeviceGbinfoQryRsp> cTrans;
	CDeviceGbinfoQryRsp &cRsp = cTrans.GetMsgObj();
	CTaskMsgTransfer<CDeviceGbinfoQryNtfReq> cTransNtf;
	CDeviceGbinfoQryNtfReq &cNtf = cTransNtf.GetMsgObj();

	const CDeviceGbinfoQryReq &cReq = pcMsg->GetMsgObj();
	string strTreeId = cReq.GetDevTreeId();
	string strGbid = cReq.GetGbid();
	string strDevGrpId;
	string strFullPath;
	vector<string> vecCurrentGrpId;
	int dwSumNum=0;
	vector<string> vecGrpId;
	TSqlString tSql, tSqlCount;
	CDeviceFullGbInfo cInfo;
	do 
	{
		if (!cReq.GetName().empty())
		{
			//应根据name字段找到相应的视频源或设备信息,之后去相应的Devgrp2dev表中找出视频源信息
				tSql.Format("call %s('%s','%s','%s',%d,%d)", P_QRYDEVGBINFOBYNAME,
					DbStr(cReq.GetDevTreeId()), DbStr(cReq.GetGbid()), DbStr(cReq.GetName()), cReq.GetStartEntryIndex(), cReq.GetExpectEntryNum());
		}
		else if(!cReq.GetGbid().empty())
		{
			tSql.Format("select a.%s,a.%s,a.%s,a.%s,a.%s,a.%s,a.%s,b.%s,b.%s,c.%s from %s a left join tblDevice b on a.devId=b.devId left join tblDeviceCapIndexName c \
				on a.devId=c.deviceId and a.devCapIndex=c.deviceCapIndex where a.%s='%s' or a.%s='%s' or a.%s='%s' or a.%s='%s' limit %d,%d ",
				F_DEVGRP2DEV_DEVGROUPID,F_DEVGRP2DEV_DEVID,F_DEVGRP2DEV_DEVCAPINDEX,
				F_DEVGRP2DEV_PRIMARYOLDGBID,F_DEVGRP2DEV_PRIMARYNEWGBID,F_DEVGRP2DEV_SECONDARYOLDGBID,F_DEVGRP2DEV_SECONDARYNEWGBID,
				F_DEVICE_DEVNAME,F_DEVICE_DOMAINID,F_DEVICECAPINDEXDATA_DEVICECAPINDEXNAME,
				DbStr(GetTblDevGrp2DevName(strTreeId)),
				F_DEVGRP2DEV_PRIMARYOLDGBID,DbStr(strGbid),
				F_DEVGRP2DEV_PRIMARYNEWGBID,DbStr(strGbid),
				F_DEVGRP2DEV_SECONDARYOLDGBID,DbStr(strGbid),
				F_DEVGRP2DEV_SECONDARYNEWGBID,DbStr(strGbid),
				cReq.GetStartEntryIndex(), cReq.GetExpectEntryNum());
		}
		else
		{
			cRsp.SetErrorCode(ERR_TAS_INVALID_PARM);
			break;
		}
		

		if (!ExecSql(tSql, enExecMayRet))
		{
			cRsp.SetErrorCode(ERR_TAS_DBERROR);
			break;
		}
		vector<CDeviceFullGbInfo> devGbinfos;
		while (IsNotEof())
		{
			//获取设备的基本信息
			FetchDeviceGbInfoQry(cInfo);
			devGbinfos.push_back(cInfo);
			vecCurrentGrpId.push_back(GetString(F_DEVGRP2DEV_DEVGROUPID));
			dwSumNum++;
			MoveNext();
		}

		for (int i =0 ; i< vecCurrentGrpId.size(); i++)
		{
			tSql.Format("select %s from %s where devgroupId='%s'",F_DEVGROUP_FULLPATH,DbStr(GetTblDevGrpName(strTreeId)),DbStr(vecCurrentGrpId[i]));
			if (!ExecSql(tSql, enExecMayRet))
			{
				cRsp.SetErrorCode(ERR_TAS_DBERROR);
				break;
			}
			strFullPath = GetString(F_DEVGROUP_FULLPATH);

			strtok(strFullPath.c_str(), "/", vecGrpId);
			vecGrpId.push_back(vecCurrentGrpId[i]);//加入本级分组
			for (vector<string>::iterator it = vecGrpId.begin();it != vecGrpId.end();it++)
			{
				tSql.Format("select * from %s where devgroupId='%s'",DbStr(GetTblDevGrpName(strTreeId)),DbStr(*it));
				if (!ExecSql(tSql, enExecMayRet))
				{
					cRsp.SetErrorCode(ERR_TAS_DBERROR);
					break;
				}
				while(IsNotEof())
				{
					//获取设备所在所有分组的完整信息
					//这里带上父分组的domainid作为子分组的parentgrpdomainid
					FetchDeviceGbFullGrpInfoQry(devGbinfos[i],strTreeId);
					MoveNext();
				}
			}

		}
		cRsp.SetTotalEntryNum(dwSumNum);
		cRsp.SetEntryNum(dwSumNum);
		cNtf.SetEntryNum(dwSumNum);
		int nNum = 0;
		for (int i = 0; i < devGbinfos.size(); ++i)
		{
			if (nNum<5)
			{
				cNtf.GetDevGbinfos().push_back(devGbinfos[i]);
				++nNum;
			}
			else
			{
				if (devGbinfos.size()%5==0 && i+1>=devGbinfos.size())
				{
					cNtf.SetLastNtf(true);	
				}
				PostTaskMsgTransfer2CCtrl(*pcMsg,cReq,cNtf,cTransNtf);
				cNtf.GetDevGbinfos().clear();
				cNtf.GetDevGbinfos().push_back(devGbinfos[i]);
				nNum=1;
			}	
		}
		if (nNum<=5&&!cNtf.GetDevGbinfos().empty())
		{
			cNtf.SetLastNtf(true);
			PostTaskMsgTransfer2CCtrl(*pcMsg,cReq,cNtf,cTransNtf);
		}

		PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);

	} while (false);

}

void CDbOpr::FetchDeviceGbInfoQry(CDeviceFullGbInfo &cInfo)
{
	cInfo.SetDevId(GetString(F_DEVGRP2DEV_DEVID));
	cInfo.SetDevName(GetString(F_DEVICE_DEVNAME));
	cInfo.SetDevDomainId(GetString(F_DEVICE_DOMAINID));
	cInfo.SetVidIdx(GetInt(F_DEVGRP2DEV_DEVCAPINDEX));
	cInfo.SetVidSrcName(GetString(F_DEVICECAPINDEXDATA_DEVICECAPINDEXNAME));
	cInfo.SetOldPriGbid(GetString(F_DEVGRP2DEV_PRIMARYOLDGBID));
	cInfo.SetOldSecGbid(GetString(F_DEVGRP2DEV_SECONDARYOLDGBID));
	cInfo.SetNewPriGbid(GetString(F_DEVGRP2DEV_PRIMARYNEWGBID));
	cInfo.SetNewSecGbid(GetString(F_DEVGRP2DEV_SECONDARYNEWGBID));
}
void CDbOpr::FetchDeviceGbFullGrpInfoQry(CDeviceFullGbInfo &cInfo,const string& strTreeId)
{
	string strParentGrpDomainId = "";
	//这里parentdomainID就不去查了，直接用父分组的domainId
	if (!cInfo.GetDevGrpInfos().empty())
	{
		strParentGrpDomainId = cInfo.GetDevGrpInfos().back().GetDomainUUID();
	}
	
	CDeviceGroupInfo cGrpInfo;
	cGrpInfo.SetDevtreeId(strTreeId);
	cGrpInfo.SetDeviceGroupID(GetString(F_DEVGROUP_DEVGROUPID));
	cGrpInfo.SetDomainUUID(GetString(F_DEVGROUP_DOMAINID));
	cGrpInfo.SetDeviceParentGroupID(GetString(F_DEVGROUP_PARENTDEVGROUPID));
	cGrpInfo.SetParentDomainUUID(strParentGrpDomainId);
	cGrpInfo.SetDeviceGroupName(GetString(F_DEVGROUP_GROUPNAME));
	cGrpInfo.SetDeviceGroupDesc(GetString(F_DEVGROUP_GROUPDESC));
	cGrpInfo.SetCreateDate(GetString(F_DEVGROUP_CREATEDATE));
	cGrpInfo.SetCreater(GetInt(F_DEVGROUP_CREATER));
	cGrpInfo.SetOwner(GetString(F_DEVGROUP_OWNER));
	cGrpInfo.SetFullpath(GetString(F_DEVGROUP_FULLPATH));
	cGrpInfo.SetGrpseq(GetInt(F_DEVGROUP_GRPSEQ));
	cGrpInfo.SetOldgbid(GetString(F_DEVGROUP_OLDGBID));
	cGrpInfo.SetNewgbid(GetString(F_DEVGROUP_NEWGBID));

	cInfo.GetDevGrpInfos().push_back(cGrpInfo);
}

void CDbOpr::DevGrpGbInfoQry(const CTaskMsgTransfer<CDevgrpGbinfoQryReq> *pcMsg)
{
	CTaskMsgTransfer<CDevgrpGbinfoQryRsp> cTrans;
	CDevgrpGbinfoQryRsp &cRsp = cTrans.GetMsgObj();
	CTaskMsgTransfer<CDevgrpGbinfoQryNtfReq> cTransNtf;
	CDevgrpGbinfoQryNtfReq &cNtf = cTransNtf.GetMsgObj();

	const CDevgrpGbinfoQryReq &cReq = pcMsg->GetMsgObj();
	string strTreeId = cReq.GetDevTreeId();
	string strGbid = cReq.GetCivilCode();
	string strName = cReq.GetName();
	string strGrpId;
	string strFullPath;
	vector<string> vecGrpId;
	vector<string> vecCurrentGrpId;
	vector<string> vecCurrentFullPath;
	int dwSumNum=0;
	TSqlString tSql, tSqlCount;
	TDevGrpFullGbInfos tInfo;
	do 
	{
		if (!strGbid.empty() && !strName.empty() && strGbid==strName) //cu区分不了时，分组名和civilcode都查
		{
			tSql.Format("select %s,%s from %s where oldGbid='%s' or newGbid='%s' or %s like '%%%s%%'",
				F_DEVGROUP_DEVGROUPID,F_DEVGROUP_FULLPATH,DbStr(GetTblDevGrpName(strTreeId)),DbStr(strGbid),DbStr(strGbid),
				F_DEVGROUP_GROUPNAME,DbStr(strName));
		}
		else if (!strGbid.empty())
		{
			tSql.Format("select %s,%s from %s where oldGbid='%s' or newGbid='%s'",
				F_DEVGROUP_DEVGROUPID,F_DEVGROUP_FULLPATH,DbStr(GetTblDevGrpName(strTreeId)),DbStr(strGbid),DbStr(strGbid));
		}
		else if (!strName.empty())
		{
			tSql.Format("select %s,%s from %s where %s like '%%%s%%' ",
				F_DEVGROUP_DEVGROUPID,F_DEVGROUP_FULLPATH,
				DbStr(GetTblDevGrpName(strTreeId)),
				F_DEVGROUP_GROUPNAME,DbStr(strName));
		}
		else
		{
			cRsp.SetErrorCode(ERR_TAS_INVALID_PARM);
			break;
		}
		
		if (!ExecSql(tSql, enExecMayRet))
		{
			cRsp.SetErrorCode(ERR_TAS_DBERROR);
			break;
		}
		while(IsNotEof())
		{
			vecCurrentFullPath.push_back(GetString(F_DEVGROUP_FULLPATH));
			vecCurrentGrpId.push_back(GetString(F_DEVGROUP_DEVGROUPID));
			dwSumNum++;
			MoveNext();
		}
		cRsp.SetEntryNum(dwSumNum);
		cRsp.SetTotalEntryNum(dwSumNum);
		cNtf.SetEntryNum(1);
		for (int i=0;i<vecCurrentGrpId.size();i++)
		{
			strtok(vecCurrentFullPath[i].c_str(),"/",vecGrpId);
			vecGrpId.push_back(vecCurrentGrpId[i]);
			for (vector<string>::iterator it = vecGrpId.begin();it != vecGrpId.end();it++)
			{
				tSql.Format("select * from %s where devgroupId='%s'",DbStr(GetTblDevGrpName(strTreeId)),DbStr(*it));
				if (!ExecSql(tSql, enExecMayRet))
				{
					cRsp.SetErrorCode(ERR_TAS_DBERROR);
					break;
				}
				while(IsNotEof())
				{
					FetchDeviceGrpGbInfoQry(tInfo,strTreeId);
					MoveNext();
				}
			}
			if (i+1>=vecCurrentGrpId.size())
			{
				cNtf.SetLastNtf(true);
			}
			cNtf.GetDevGbinfos().push_back(tInfo);
			PostTaskMsgTransfer2CCtrl(*pcMsg,cReq,cNtf,cTransNtf);
			cNtf.GetDevGbinfos().clear();
			tInfo.clear();
		}
	} while (false);

	PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
}

void CDbOpr::FetchDeviceGrpGbInfoQry(TDevGrpFullGbInfos &cInfo,const string& strTreeId)
{
	string strParentGrpDomainId = "";
	//这里parentdomainID就不去查了，直接用父分组的domainId
	if (!cInfo.empty())
	{
		strParentGrpDomainId = cInfo.back().GetDomainUUID();
	}
	CDeviceGroupInfo cGrpInfo;
	cGrpInfo.SetDevtreeId(strTreeId);
	cGrpInfo.SetDeviceGroupID(GetString(F_DEVGROUP_DEVGROUPID));
	cGrpInfo.SetDomainUUID(GetString(F_DEVGROUP_DOMAINID));
	cGrpInfo.SetDeviceParentGroupID(GetString(F_DEVGROUP_PARENTDEVGROUPID));
	cGrpInfo.SetParentDomainUUID(strParentGrpDomainId);
	cGrpInfo.SetDeviceGroupName(GetString(F_DEVGROUP_GROUPNAME));
	cGrpInfo.SetDeviceGroupDesc(GetString(F_DEVGROUP_GROUPDESC));
	cGrpInfo.SetCreateDate(GetString(F_DEVGROUP_CREATEDATE));
	cGrpInfo.SetCreater(GetInt(F_DEVGROUP_CREATER));
	cGrpInfo.SetOwner(GetString(F_DEVGROUP_OWNER));
	cGrpInfo.SetFullpath(GetString(F_DEVGROUP_FULLPATH));
	cGrpInfo.SetGrpseq(GetInt(F_DEVGROUP_GRPSEQ));
	cGrpInfo.SetOldgbid(GetString(F_DEVGROUP_OLDGBID));
	cGrpInfo.SetNewgbid(GetString(F_DEVGROUP_NEWGBID));

	cInfo.push_back(cGrpInfo);
}


void CDbOpr::DevGrpMatch(const CTaskMsgTransfer<CDeviceMatchGbidReq> *pcMsg)
{
	CTaskMsgTransfer<CDeviceMatchGbidRsp> cTrans;
	CDeviceMatchGbidRsp &cRsp = cTrans.GetMsgObj();
	TSqlString tSql;
	string strGbid;
	CUUID uuid;
	cRsp.SetOperId(uuid);
	bool bHasRsped = false;
	g_cDbTaskPool.AddTransProgress(uuid, 100,5);
	const CDeviceMatchGbidReq &cReq = pcMsg->GetMsgObj();
	do 
	{
		tSql.Format("select %s,%s from %s where devgroupId='%s'",F_DEVGROUP_NEWGBID,F_DEVGROUP_PARENTDEVGROUPID, DbStr(GetTblDevGrpName(cReq.GetDevTreeId())),
			DbStr(cReq.GetDevGrpId()));
		if (!ExecSql(tSql,enExecMayRet))
		{
			cRsp.SetErrorCode(ERR_TAS_DBERROR);
			break;
		}
		while(IsNotEof())
		{
			strGbid = GetString(F_DEVGROUP_NEWGBID);
			break;
		}

		//strGbid可能是215、216，这里需要找到对应的父civilcode，根据父civilcode来修改strGbid下的设备
		string strDevGrpId;
		while (!IsDevGrpCivilCode(strGbid))
		{
			strDevGrpId = GetString(F_DEVGROUP_PARENTDEVGROUPID);
			tSql.Format("select %s,%s from %s where devgroupId='%s'",F_DEVGROUP_NEWGBID,F_DEVGROUP_PARENTDEVGROUPID, DbStr(GetTblDevGrpName(cReq.GetDevTreeId())),
				DbStr(strDevGrpId));
			if (!ExecSql(tSql,enExecMayRet))
			{
				cRsp.SetErrorCode(ERR_TAS_DBERROR);
				break;
			}
			if(IsEof())
			{
				cRsp.SetErrorCode(ERR_TAS_DBERROR);
				break;
			}
			strGbid = GetString(F_DEVGROUP_NEWGBID);
		}
		if (CMS_SUCCESS != cRsp.GetErrorCode())
		{
			break;
		}
		g_cDbTaskPool.AddTransProgress(uuid,15);
		PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
		bHasRsped = true;
		if (cReq.GetDevId().empty())//匹配分组和设备
		{
			tSql.Format("call %s('%s','%s','%s')", P_MATCHDEVGRPCIVILCODE, DbStr(cReq.GetDevTreeId()), DbStr(cReq.GetDevGrpId()),
				DbStr(strGbid));
		}
		else //匹配设备
		{
			if (cReq.GetDevMatchType() == 0)//主流
			{
				tSql.Format("update %s set %s=concat('%s',right(%s,20-length('%s'))) where devId='%s' and devCapIndex=%d",
					DbStr(GetTblDevGrp2DevName(cReq.GetDevTreeId())),F_DEVGRP2DEV_PRIMARYNEWGBID,DbStr(strGbid),F_DEVGRP2DEV_PRIMARYNEWGBID,
					DbStr(strGbid),DbStr(cReq.GetDevId()),cReq.GetDevVidIndex());
			}
			else if (cReq.GetDevMatchType() == 1)
			{
				tSql.Format("update %s set %s=concat('%s',right(%s,20-length('%s'))) where devId='%s' and devCapIndex=%d",
					DbStr(GetTblDevGrp2DevName(cReq.GetDevTreeId())),F_DEVGRP2DEV_SECONDARYNEWGBID,DbStr(strGbid),F_DEVGRP2DEV_SECONDARYNEWGBID,
					DbStr(strGbid),DbStr(cReq.GetDevId()),cReq.GetDevVidIndex());
			}
			else
			{
				cRsp.SetErrorCode(ERR_TAS_INVALID_PARM);
				g_cDbTaskPool.AddTransProgress(uuid,-1, ERR_TAS_INVALID_PARM);
				break;
			}
			
		}
		if (!ExecSql(tSql,enExecNoRet))
		{
			cRsp.SetErrorCode(ERR_TAS_DBERROR);
			g_cDbTaskPool.AddTransProgress(uuid,-1, ERR_TAS_DBERROR);
			break;
		}
		g_cDbTaskPool.AddTransProgress(uuid, 100,25);
		if (cReq.GetDevId().empty() && cReq.GetMatchLastBits()) //匹配设备尾数
		{
			tSql.Format("call %s('%s','%s')", P_MATCHGBIDLASTBITS, DbStr(cReq.GetDevTreeId()), DbStr(cReq.GetDevGrpId()));
			if (!ExecSql(tSql, enExecNoRet))
			{
				cRsp.SetErrorCode(ERR_TAS_DBERROR);
				g_cDbTaskPool.AddTransProgress(uuid,-1, ERR_TAS_DBERROR);
				break;
			}
		}

	} while (false);
	g_cDbTaskPool.AddTransProgress(uuid, 100,35);
	if (cRsp.GetErrorCode() == CMS_SUCCESS && !cReq.GetDevId().empty())
	{
		if (cReq.GetDevMatchType() == 0)
		{
			tSql.Format("select %s from %s where devId='%s' and devCapIndex=%d",
				F_DEVGRP2DEV_PRIMARYNEWGBID,DbStr(GetTblDevGrp2DevName(cReq.GetDevTreeId())),DbStr(cReq.GetDevId()),cReq.GetDevVidIndex());
			if (!ExecSql(tSql,enExecMayRet))
			{
				cRsp.SetErrorCode(ERR_TAS_DBERROR);
				g_cDbTaskPool.AddTransProgress(uuid,-1, ERR_TAS_DBERROR);
			}
			while(IsNotEof())
			{
				cRsp.SetGbid(GetString(F_DEVGRP2DEV_PRIMARYNEWGBID));
				MoveNext();
			}
		}
		else if (cReq.GetDevMatchType() == 1)
		{
			tSql.Format("select %s from %s where devId='%s' and devCapIndex=%d",
				F_DEVGRP2DEV_SECONDARYNEWGBID,DbStr(GetTblDevGrp2DevName(cReq.GetDevTreeId())),DbStr(cReq.GetDevId()),cReq.GetDevVidIndex());
			if (!ExecSql(tSql,enExecMayRet))
			{
				cRsp.SetErrorCode(ERR_TAS_DBERROR);
				g_cDbTaskPool.AddTransProgress(uuid,-1, ERR_TAS_DBERROR);
			}
			while(IsNotEof())
			{
				cRsp.SetGbid(GetString(F_DEVGRP2DEV_SECONDARYNEWGBID));
				MoveNext();
			}
		}
		
	}
	
	if (cRsp.GetErrorCode() == CMS_SUCCESS)
	{
		g_cDbTaskPool.AddTransProgress(uuid, 100,100);
		UpdateTreesVerByDevId("");
		//检查，将重复的gbid放入tbl中
	}
	if (!bHasRsped)
	{
		PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
	}
}

void CDbOpr::DevGbidNextErrQry(const CTaskMsgTransfer<CDeviceGbidErrGetReq> *pcMsg)
{
	CTaskMsgTransfer<CDeviceGbidErrGetRsp> cTrans;
	CDeviceGbidErrGetRsp &cRsp = cTrans.GetMsgObj();

	const CDeviceGbidErrGetReq& cReq = pcMsg->GetMsgObj();
	TSqlString tSql;
	bool bExist=false;
	TDevGbidErrItem tDevGbidErrItem;
	int Grp2DevIndex=0;
	string strFullPath="";
	vector<string> vecGrps;

	
	do 
	{
		if (cReq.GetDevTreeId().empty() 
			|| !IsDevTreeCanBeUse4Gb(cReq.GetDevTreeId()))
		{
			cRsp.SetErrorCode(ERR_TAS_INVALID_PARM);
			break;
		}
		
		tSql.Format("call %s('%s','%s',%d)", P_QRYNEXTDEVGBIDERRINFO,
			DbStr(cReq.GetDevTreeId()), DbStr(cReq.GetGroupId()), cReq.GetErrIndex());
		if (!ExecSql(tSql, enExecMayRet))
		{
			cRsp.SetErrorCode(ERR_TAS_DBERROR);
			break;
		}
		else
		{
			if (IsNotEof())
			{
				tDevGbidErrItem.devGrpId=string(GetString(F_DEVERRGBINFO_DEVGRPID));
				tDevGbidErrItem.devId=string(GetString(F_DEVERRGBINFO_DEVID));
				tDevGbidErrItem.vidSrcId = GetInt(F_DEVERRGBINFO_VIDSRCIDX);
				tDevGbidErrItem.gbidErrType =(TDevGbidErrType)GetInt(F_DEVERRGBINFO_ERRTYPE);
				tDevGbidErrItem.errIndex = GetInt(F_DEVERRGBINFO_ERRIDX);
				if (tDevGbidErrItem.gbidErrType == GBID_ERRTYPE_NOERROR)
				{
					cRsp.SetExistErr(false);
					break;
				}
				else
				{
					cRsp.SetExistErr(true);
				}
			}
			else
			{
				cRsp.SetExistErr(false);
				break;
			}
		}

		//找到该错误设备所在分组的所有设备，以便确认该视频源在该分组中的序号（便于cu展示）
		tSql.Format("select %s,%s from %s where %s='%s'",
			F_DEVGRP2DEV_DEVID,F_DEVGRP2DEV_DEVCAPINDEX,
			DbStr(GetTblDevGrp2DevName(cReq.GetDevTreeId())),
			F_DEVGRP2DEV_DEVGROUPID,DbStr(tDevGbidErrItem.devGrpId));
		if (!ExecSql(tSql,enExecMayRet))
		{
			cRsp.SetErrorCode(ERR_TAS_DBERROR);
			break;
		}
		while(IsNotEof())
		{
			Grp2DevIndex++;
			if (GetString(F_DEVGRP2DEV_DEVID) == tDevGbidErrItem.devId && GetInt(F_DEVGRP2DEV_DEVCAPINDEX) == tDevGbidErrItem.vidSrcId)
			{
				tDevGbidErrItem.vidSrcGrpIdx = Grp2DevIndex;
				break;
			}
			MoveNext();
		}

		//找到该视频源的详细信息
		tSql.Format("select a.%s, a.%s, a.%s, a.%s, b.%s, b.%s, c.%s from %s a inner join %s b on a.%s=b.%s \
			left join %s c on a.%s=c.%s and a.%s=c.%s where a.%s='%s' and a.%s=%d and a.%s='%s'",
			F_DEVGRP2DEV_PRIMARYOLDGBID,F_DEVGRP2DEV_PRIMARYNEWGBID,F_DEVGRP2DEV_SECONDARYOLDGBID,F_DEVGRP2DEV_SECONDARYNEWGBID,
			F_DEVICE_DOMAINID,F_DEVICE_DEVNAME,F_DEVICECAPINDEXDATA_DEVICECAPINDEXNAME,
			DbStr(GetTblDevGrp2DevName(cReq.GetDevTreeId())),T_DEVICE,
			F_DEVGRP2DEV_DEVID,F_DEVICE_DEVID,
			T_DEVICECAPINDEXDATA,
			F_DEVGRP2DEV_DEVID,F_DEVICECAPINDEXDATA_DEVICEID,
			F_DEVGRP2DEV_DEVCAPINDEX,F_DEVICECAPINDEXDATA_DEVICECAPINDEX,
			F_DEVGRP2DEV_DEVID,DbStr(tDevGbidErrItem.devId),
			F_DEVGRP2DEV_DEVCAPINDEX,tDevGbidErrItem.vidSrcId,
			F_DEVGRP2DEV_DEVGROUPID,DbStr(tDevGbidErrItem.devGrpId));

		if (!ExecSql(tSql,enExecMayRet))
		{
			cRsp.SetErrorCode(ERR_TAS_DBERROR);
			break;
		}
		while(IsNotEof())
		{
			FetchDeviceErrQry(tDevGbidErrItem);
			MoveNext();
		}

		//找到该视频源到该树根分组的路径详细信息（便于cu展示）
		tSql.Format("select %s from %s where %s='%s'",
			F_DEVGROUP_FULLPATH,DbStr(GetTblDevGrpName(cReq.GetDevTreeId())),F_DEVGROUP_DEVGROUPID,DbStr(tDevGbidErrItem.devGrpId));
		if (!ExecSql(tSql,enExecMayRet))
		{
			cRsp.SetErrorCode(ERR_TAS_DBERROR);
			break;
		}
		strFullPath = GetString(F_DEVGROUP_FULLPATH);

		strtok(strFullPath.c_str(),"/",vecGrps);
		vecGrps.push_back(tDevGbidErrItem.devGrpId);
		for (vector<string>::iterator it = vecGrps.begin();it != vecGrps.end();it++)
		{
			tSql.Format("select * from %s where %s='%s'",DbStr(GetTblDevGrpName(cReq.GetDevTreeId())),F_DEVGROUP_DEVGROUPID,DbStr(*it));
			if (!ExecSql(tSql,enExecMayRet))
			{
				cRsp.SetErrorCode(ERR_TAS_DBERROR);
				break;
			}
			while(IsNotEof())
			{
				//这里复用下
				FetchDeviceGrpGbInfoQry(tDevGbidErrItem.devGrpFullGbinfos,cReq.GetDevTreeId());
				MoveNext();
			}
		}

		cRsp.SetErrItem(tDevGbidErrItem);

	} while (false);
	
	PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);


}

void CDbOpr::FetchDeviceErrQry(TDevGbidErrItem& tDevErr)
{	
	tDevErr.primaryOldGbid = GetString(F_DEVGRP2DEV_PRIMARYOLDGBID);
	tDevErr.primaryNewGbid = GetString(F_DEVGRP2DEV_PRIMARYNEWGBID);
	tDevErr.secondaryOldGbid = GetString(F_DEVGRP2DEV_SECONDARYOLDGBID);
	tDevErr.secondaryNewGbid = GetString(F_DEVGRP2DEV_SECONDARYNEWGBID);
	tDevErr.domainId = GetString(F_DEVICE_DOMAINID);
	tDevErr.devName = GetString(F_DEVICE_DEVNAME);
	tDevErr.vidSrcName = GetString(F_DEVICECAPINDEXDATA_DEVICECAPINDEXNAME);
}


void CDbOpr::DevGrpGbidOperate(const CDbMsgTransfer<CDevGrpGbidOperateReq> *pcMsg)
{
	CDbMsgTransfer<CDevGrpGbidOperateRsp> cTrans(*pcMsg, msgDevGrpGbidOperateRsp);
	CDevGrpGbidOperateRsp &cRsp = cTrans.GetMsgObj();

	const CDevGrpGbidOperateReq &cReq = pcMsg->GetMsgObj();
	TSqlString tSql;
	bool bExist=false;

	do 
	{
		if (cReq.devTreeId.empty() || cReq.devGrpId.empty())
		{
			cRsp.SetErrno(ERR_TAS_INVALID_OPERATION);
			break;
		}
		//先查看有无重复的
		tSql.Format("select count(*) from %s where %s='%s'",DbStr(GetTblDevGrpName(cReq.devTreeId)),
			F_DEVGROUP_NEWGBID,DbStr(cReq.devGrpGbid));
		if (ExecSql(tSql,enExecMustRet))
		{
			bExist = (GetIntResult()==0)? false:true;
		}
		if (bExist)//已经存在
		{
			cRsp.SetErrno(ERR_TAS_RECORD_EXISTS);
			break;
		}

		//禁止分组gbid类型被修改（行政区划被修改成虚拟组织，或反之）
		tSql.Format("select %s from %s where %s='%s'", 
			F_DEVGROUP_NEWGBID, DbStr(GetTblDevGrpName(cReq.devTreeId)),
			F_DEVGRP2DEV_DEVGROUPID,DbStr(cReq.devGrpId));
		if (!ExecSql(tSql,enExecMayRet))
		{
			cRsp.SetErrno(ERR_TAS_DBERROR);
			break;
		}
		string strOldGbid = GetString(F_DEVGROUP_NEWGBID);
		if (strOldGbid.size()>cReq.devGrpGbid.size() 
			&& strOldGbid.size() - cReq.devGrpGbid.size()>=10             //虚拟组织被改成行政区划
			||strOldGbid.size() < cReq.devGrpGbid.size() 
			&& cReq.devGrpGbid.size() - strOldGbid.size() >=10)        //行政区划被改成虚拟组织
		{
			cRsp.SetErrno(ERR_TAS_INVALID_OPERATION);
			break;
		}

		tSql.Format("update %s set %s='%s' where %s='%s'",DbStr(GetTblDevGrpName(cReq.devTreeId)),
			F_DEVGROUP_NEWGBID,DbStr(cReq.devGrpGbid),F_DEVGRP2DEV_DEVGROUPID,DbStr(cReq.devGrpId));
	
		if (!ExecSql(tSql,enExecNoRet))
		{
			cRsp.SetErrno(ERR_TAS_DBERROR);
			break;
		}

	} while (false);

	PostDbMsgTransferFromDbTaskPool(cTrans);
}

void CDbOpr::DevGrp2DevGbidOperate(const CDbMsgTransfer<CDevGrp2DevGbidOperateReq> *pcMsg)
{
	CDbMsgTransfer<CDevGrp2DevGbidOperateRsp> cTrans(*pcMsg, msgDevGrp2DevGbidOperateRsp);
	CDevGrp2DevGbidOperateRsp &cRsp = cTrans.GetMsgObj();

	const CDevGrp2DevGbidOperateReq &cReq = pcMsg->GetMsgObj();
	TSqlString tSql;
	bool bNeedMatch= false;
	string strGrpGbid;
	string strMatchedGbid;
	vector<string> vecBatchDelGrps;
	TOperateGbDevInfo tGbDevInfo;

	do 
	{
		if (cReq.devTreeId.empty())
		{
			cRsp.SetErrno(ERR_TAS_INVALID_OPERATION); //系统设备树不允许如此操作
			break;
		}
		if (cReq.devs.size() == 1 && (cReq.devs.front().operType == OPERATE_TYPE_BATCH_DEL_PRIMARY || cReq.devs.front().operType == OPERATE_TYPE_BATCH_DEL_SECOND)
			&& !cReq.devs.front().devGrpId.empty() )
		{
			tSql.Format("select %s from %s where %s='%s' or %s like '%%%s%%'",
				F_DEVGROUP_DEVGROUPID,DbStr(GetTblDevGrpName(cReq.devTreeId)),
				F_DEVGROUP_DEVGROUPID,DbStr(cReq.devs.front().devGrpId),
				F_DEVGROUP_FULLPATH,DbStr(cReq.devs.front().devGrpId));
			if (!ExecSql(tSql,enExecMayRet))
			{
				cRsp.SetErrno(ERR_TAS_DBERROR);
				break;
			}
			while(IsNotEof())
			{
				vecBatchDelGrps.push_back(GetString(F_DEVGROUP_DEVGROUPID));
				MoveNext();
			}

			for (vector<string>::iterator it = vecBatchDelGrps.begin();it != vecBatchDelGrps.end();it++)
			{
				if (cReq.devs.front().operType == OPERATE_TYPE_BATCH_DEL_PRIMARY)
				{
					tSql.Format("update %s set %s='',%s='' where %s='%s'",DbStr(GetTblDevGrp2DevName(cReq.devTreeId)),
						F_DEVGRP2DEV_PRIMARYOLDGBID,F_DEVGRP2DEV_PRIMARYNEWGBID,
						F_DEVGROUP_DEVGROUPID,DbStr(*it));
				}
				else if (cReq.devs.front().operType == OPERATE_TYPE_BATCH_DEL_SECOND)
				{
					tSql.Format("update %s set %s='',%s='' where %s='%s'",DbStr(GetTblDevGrp2DevName(cReq.devTreeId)),
						F_DEVGRP2DEV_SECONDARYOLDGBID,F_DEVGRP2DEV_SECONDARYNEWGBID,
						F_DEVGROUP_DEVGROUPID,DbStr(*it));
				}
				ExecSql(tSql,enExecNoRet);
			}

			break;
		}
		for (int i =0;i < cReq.devs.size();i++)
		{
			tGbDevInfo = cReq.devs[i];
			strMatchedGbid = tGbDevInfo.gbid;
			switch (tGbDevInfo.operType)
			{
			case OPERATE_TYPE_ADD_PRIMARY://增加主流
			case OPERATE_TYPE_DEL_PRIMARY://删除主流
				{
					if (tGbDevInfo.operType == OPERATE_TYPE_ADD_PRIMARY)
					{
						tSql.Format("select count(*) from %s where %s='%s' or %s='%s'",DbStr(GetTblDevGrp2DevName(cReq.devTreeId)),
							F_DEVGRP2DEV_PRIMARYNEWGBID,DbStr(tGbDevInfo.gbid),
							F_DEVGRP2DEV_SECONDARYNEWGBID,DbStr(tGbDevInfo.gbid));
						if (ExecSql(tSql,enExecMustRet))
							bNeedMatch = (GetIntResult()==0)? false:true;
						else 
							break;
						if (!tGbDevInfo.gbid.empty() && bNeedMatch)//需要匹配
						{
							tSql.Format("select %s from %s where %s='%s'",
								F_DEVGROUP_NEWGBID,DbStr(GetTblDevGrpName(cReq.devTreeId)),
								F_DEVGROUP_DEVGROUPID,DbStr(tGbDevInfo.devGrpId));
							if (ExecSql(tSql,enExecMayRet))
							{
								strGrpGbid = GetString(F_DEVGROUP_NEWGBID);
							}
							else 
								break;
							strMatchedGbid.replace(0,strGrpGbid.size(),strGrpGbid.c_str());
						}
					}

					tSql.Format("update %s set %s='%s',%s='%s' where %s='%s' and %s='%s' and %s=%d and %s=%d",
						DbStr(GetTblDevGrp2DevName(cReq.devTreeId)),
						F_DEVGRP2DEV_PRIMARYOLDGBID,DbStr(tGbDevInfo.gbid),
						F_DEVGRP2DEV_PRIMARYNEWGBID,DbStr(strMatchedGbid),
						F_DEVGRP2DEV_DEVGROUPID,DbStr(tGbDevInfo.devGrpId),
						F_DEVGRP2DEV_DEVID,DbStr(tGbDevInfo.devId),
						F_DEVGRP2DEV_DEVCAPID,1,
						F_DEVGRP2DEV_DEVCAPINDEX,tGbDevInfo.vidSrcIdx);
				}
				break;
			case OPERATE_TYPE_ADD_SECOND://增加辅流
			case OPERATE_TYPE_DEL_SECOND://删除辅流
				{
					if (tGbDevInfo.operType == OPERATE_TYPE_ADD_SECOND)
					{
						tSql.Format("select count(*) from %s where %s='%s' or %s='%s'",DbStr(GetTblDevGrp2DevName(cReq.devTreeId)),
							F_DEVGRP2DEV_PRIMARYNEWGBID,DbStr(tGbDevInfo.gbid),
							F_DEVGRP2DEV_SECONDARYNEWGBID,DbStr(tGbDevInfo.gbid));
						if (ExecSql(tSql,enExecMustRet))
							bNeedMatch = (GetIntResult()==0)? false:true;
						else 
							break;
						if (!tGbDevInfo.gbid.empty() && bNeedMatch)//需要匹配
						{
							tSql.Format("select %s from %s where %s='%s'",
								F_DEVGROUP_NEWGBID,DbStr(GetTblDevGrpName(cReq.devTreeId)),
								F_DEVGROUP_DEVGROUPID,DbStr(tGbDevInfo.devGrpId));
							if (ExecSql(tSql,enExecMayRet))
							{
								strGrpGbid = GetString(F_DEVGROUP_NEWGBID);
							}
							else 
								break;
							strMatchedGbid.replace(0,strGrpGbid.size(),strGrpGbid.c_str());
						}
					}
					tSql.Format("update %s set %s='%s',%s='%s' where %s='%s' and %s='%s' and %s=%d and %s=%d",
						DbStr(GetTblDevGrp2DevName(cReq.devTreeId)),
						F_DEVGRP2DEV_SECONDARYOLDGBID,DbStr(tGbDevInfo.gbid),
						F_DEVGRP2DEV_SECONDARYNEWGBID,DbStr(strMatchedGbid),
						F_DEVGRP2DEV_DEVGROUPID,DbStr(tGbDevInfo.devGrpId),
						F_DEVGRP2DEV_DEVID,DbStr(tGbDevInfo.devId),
						F_DEVGRP2DEV_DEVCAPID,1,
						F_DEVGRP2DEV_DEVCAPINDEX,tGbDevInfo.vidSrcIdx);
				}
				break;
			case OPERATE_TYPE_MOD_PRIMARY:
			case OPERATE_TYPE_MOD_SECOND:
				{
					if (tGbDevInfo.gbid.size() != 20)
					{
						cRsp.SetErrno(ERR_TAS_INVALID_OPERATION);
						break;
					}
					tSql.Format("select count(*) from %s where %s='%s' or %s='%s'",
						DbStr(GetTblDevGrp2DevName(cReq.devTreeId)),
						F_DEVGRP2DEV_PRIMARYNEWGBID,DbStr(tGbDevInfo.gbid),
						F_DEVGRP2DEV_SECONDARYNEWGBID,DbStr(tGbDevInfo.gbid));
					if (ExecSql(tSql,enExecMustRet))
					{
						if (GetIntResult() != 0)//有冲突不给修改
						{
							cRsp.SetErrno(ERR_TAS_RECORD_EXISTS);
							break;
						}
						else
						{
							if (tGbDevInfo.operType == OPERATE_TYPE_MOD_PRIMARY)
							{
								tSql.Format("update %s set %s='%s' where %s='%s' and %s='%s' and %s=%d and %s=%d",
									DbStr(GetTblDevGrp2DevName(cReq.devTreeId)),
									F_DEVGRP2DEV_PRIMARYNEWGBID,DbStr(tGbDevInfo.gbid),
									F_DEVGRP2DEV_DEVGROUPID,DbStr(tGbDevInfo.devGrpId),
									F_DEVGRP2DEV_DEVID,DbStr(tGbDevInfo.devId),
									F_DEVGRP2DEV_DEVCAPID,1,
									F_DEVGRP2DEV_DEVCAPINDEX,tGbDevInfo.vidSrcIdx);
							}
							else if (tGbDevInfo.operType == OPERATE_TYPE_MOD_SECOND)
							{
								tSql.Format("update %s set %s='%s' where %s='%s' and %s='%s' and %s=%d and %s=%d",
									DbStr(GetTblDevGrp2DevName(cReq.devTreeId)),
									F_DEVGRP2DEV_SECONDARYNEWGBID,DbStr(tGbDevInfo.gbid),
									F_DEVGRP2DEV_DEVGROUPID,DbStr(tGbDevInfo.devGrpId),
									F_DEVGRP2DEV_DEVID,DbStr(tGbDevInfo.devId),
									F_DEVGRP2DEV_DEVCAPID,1,
									F_DEVGRP2DEV_DEVCAPINDEX,tGbDevInfo.vidSrcIdx);
							}
							if (!ExecSql(tSql,enExecNoRet))
							{
								cRsp.SetErrno(ERR_TAS_DBERROR);
								break;
							}
						}
					}
				}
				break;
			default:break;
			}
			cRsp.gbids.push_back(strMatchedGbid);
			if (!ExecSql(tSql,enExecNoRet))
			{
				cRsp.SetErrno(ERR_TAS_DBERROR);
				break;
			}
		}


	} while (false);
	if (cRsp.GetErrno() == CMS_SUCCESS)
	{
		UpdateTreesVerByDevId("");
	}
	PostDbMsgTransferFromDbTaskPool(cTrans);
}

void CDbOpr::DevGrpAllChildDevQry(const CDbMsgTransfer<CDevGrpAllChildDevQryReq> *pcMsg)
{
	CDbMsgTransfer<CDevGrpAllChildDevQryRsp> cTrans(*pcMsg, msgDevGrpAllChildDevQryRsp);
	CDevGrpAllChildDevQryRsp &cRsp = cTrans.GetMsgObj();

	const CDevGrpAllChildDevQryReq &cReq = pcMsg->GetMsgObj();
	TSqlString tSql;
	string strTreeId = cReq.devTreeId;
	string strGrpId = cReq.devGrpId;
	vector<string> vecGrps;//需要找的所有分组
	vecGrps.push_back(strGrpId);//本级分组肯定要的

	CDeviceGroupDeviceInfo4Gb cDevInfo4Gb,cNextDevInfo4Gb;
	vector<CDeviceGroupDeviceInfo4Gb> vecDevInfo4GbTmp;

	do 
	{
		//先找到所有的子分组
		tSql.Format("select %s from %s where %s like '%%%s%%'",
			F_DEVGROUP_DEVGROUPID,DbStr(GetTblDevGrpName(strTreeId)),
			F_DEVGROUP_FULLPATH,DbStr(strGrpId));
		if (!ExecSql(tSql,enExecMayRet))
		{
			cRsp.SetErrno(ERR_TAS_DBERROR);
			break;
		}
		while (IsNotEof())
		{
			vecGrps.push_back(GetString(F_DEVGROUP_DEVGROUPID));
			MoveNext();
		}

		for (vector<string>::iterator it = vecGrps.begin();it != vecGrps.end();it++)
		{
			//select a.primaryOldGbid,a.primaryNewGbid,a.secondaryOldGbid,a.secondaryNewGbid,a.devgroupId,a.devId,a.devCapIndex,b.domainId,b.devName,c.deviceCapIndexName 
				//from tblDevGrp2Dev_d5ef939a1c9b48ad928ca47255e0d8bb a inner join tblDevice b on a.devId=b.devId left join tblDeviceCapIndexName c on a.devId=c.deviceId and a.devCapIndex=c.deviceCapIndex where ;
			tSql.Format("select a.%s,a.%s,a.%s,a.%s,a.%s,a.%s,a.%s,b.%s,b.%s,c.%s from %s a \
				inner join %s b on a.%s=b.%s left join %s c on a.%s=c.%s and a.%s=c.%s where a.%s='%s'",
				F_DEVGRP2DEV_PRIMARYOLDGBID,F_DEVGRP2DEV_PRIMARYNEWGBID,F_DEVGRP2DEV_SECONDARYOLDGBID,F_DEVGRP2DEV_SECONDARYNEWGBID,
				F_DEVGRP2DEV_DEVGROUPID,F_DEVGRP2DEV_DEVID,F_DEVGRP2DEV_DEVCAPINDEX,F_DEVICE_DOMAINID,F_DEVICE_DEVNAME,F_DEVICECAPINDEXDATA_DEVICECAPINDEXNAME,
				DbStr(GetTblDevGrp2DevName(strTreeId)),T_DEVICE,
				F_DEVGRP2DEV_DEVID,F_DEVICE_DEVID,
				T_DEVICECAPINDEXDATA,
				F_DEVGRP2DEV_DEVID,F_DEVICECAPINDEXDATA_DEVICEID,
				F_DEVGRP2DEV_DEVCAPINDEX,F_DEVICECAPINDEXDATA_DEVICECAPINDEX,
				F_DEVGRP2DEV_DEVGROUPID,DbStr(*it));
			if (!ExecSql(tSql,enExecMayRet))
				continue;
			while (IsNotEof())
			{
				FetchDevGrpAllDevQry(cNextDevInfo4Gb);
				//合并包
				if (cDevInfo4Gb.GetDevGrpId() == cNextDevInfo4Gb.GetDevGrpId() && cDevInfo4Gb.GetDeviceId() == cNextDevInfo4Gb.GetDeviceId())
				{
					vecDevInfo4GbTmp.back().GetDevinfo().push_back(cNextDevInfo4Gb.GetDevinfo().back());
				}
				else
					vecDevInfo4GbTmp.push_back(cNextDevInfo4Gb);
				cDevInfo4Gb = cNextDevInfo4Gb;
				cNextDevInfo4Gb.Clear();
				MoveNext();
			}
		}
		cRsp.devInfos = vecDevInfo4GbTmp;

	} while (false);
	PostDbMsgTransferFromDbTaskPool(cTrans);

}

void CDbOpr::FetchDevGrpAllDevQry(CDeviceGroupDeviceInfo4Gb& cInfo)
{
	TDeviceInfo4Gb tDev;
	tDev.SetVidIndex(GetInt(F_DEVGRP2DEV_DEVCAPINDEX));
	tDev.SetVidSrcName(GetString(F_DEVICECAPINDEXDATA_DEVICECAPINDEXNAME));
	tDev.GetGbid().SetOldprimarygbid(GetString(F_DEVGRP2DEV_PRIMARYOLDGBID));
	tDev.GetGbid().SetNewprimarygbid(GetString(F_DEVGRP2DEV_PRIMARYNEWGBID));
	tDev.GetGbid().SetOldsecondarygbid(GetString(F_DEVGRP2DEV_SECONDARYOLDGBID));
	tDev.GetGbid().SetNewsecondarygbid(GetString(F_DEVGRP2DEV_SECONDARYNEWGBID));

	cInfo.SetDevDevTreeId("");
	cInfo.SetDevGrpId(GetString(F_DEVGRP2DEV_DEVGROUPID));
	cInfo.SetDeviceId(GetString(F_DEVGRP2DEV_DEVID));
	cInfo.SetDevDomainId(GetString(F_DEVICE_DOMAINID));
	cInfo.SetDevName(GetString(F_DEVICE_DEVNAME));

	cInfo.GetDevinfo().push_back(tDev);
}

void CDbOpr::TableDataToPushXmlFile(const CDbMsgTransfer<string> *pcMsg)
{
	CDbMsgTransfer<string> cTrans(*pcMsg, msgTableDataToPushXmlRsp);
	string &cRsp = cTrans.GetMsgObj();
	const string &cReq = pcMsg->GetMsgObj();

	string strTreeId = cReq;
	TPushXmlGrpInfo tXmlGrpInfo;
	TPushXmlGrpDevInfo tXmlGrpDevInfo;
	TPushXmlTreeInfo tPushXmlTreeInfo;
	
	map<string,string> map_devGrp2devGbid;

	TSqlString tSql;
	do 
	{
		tSql.Format("select %s,%s from %s where %s='%s'",
			F_DEVTREE_ID,F_DEVTREE_VERSION,T_DEVTREE,F_DEVTREE_ID,DbStr(strTreeId));
		if (!ExecSql(tSql,enExecMayRet))
			return;
		tPushXmlTreeInfo.treeid = GetString(F_DEVTREE_ID);
		tPushXmlTreeInfo.treever =GetString(F_DEVTREE_VERSION);

		//找到所有分组
		tSql.Format("select %s,%s,%s,%s from %s order by length(%s) , length(%s), %s  ",
			F_DEVGROUP_DEVGROUPID,F_DEVGROUP_NEWGBID,F_DEVGROUP_GROUPNAME,F_DEVGROUP_PARENTDEVGROUPID,
			DbStr(GetTblDevGrpName(strTreeId)),F_DEVGROUP_FULLPATH, F_DEVGROUP_NEWGBID, F_DEVGROUP_NEWGBID);
		if (!ExecSql(tSql,enExecMayRet))
			return;
		while (IsNotEof())
		{
			tXmlGrpInfo.groupId = GetString(F_DEVGROUP_DEVGROUPID);
			tXmlGrpInfo.groupGbid = GetString(F_DEVGROUP_NEWGBID);
			tXmlGrpInfo.groupName = GetString(F_DEVGROUP_GROUPNAME);
			tXmlGrpInfo.parentId = GetString(F_DEVGROUP_PARENTDEVGROUPID);
			map_devGrp2devGbid.insert(pair<string,string>(tXmlGrpInfo.groupId,tXmlGrpInfo.groupGbid));
			tPushXmlTreeInfo.grpinfo.push_back(tXmlGrpInfo);
			MoveNext();
		}

		//找到所有216--->civilcode的信息
		string strVirGrpKdId,strCivilCode;
		map<string, string> mapVirGrpKdid2Civilcode; //215\216Kdid--->civilcode
		tSql.Format("call %s('%s')", P_QRYVIRGRPCIVILCODE, DbStr(strTreeId));
		if (!ExecSql(tSql,enExecMayRet))
		{
			return;
		}
		while(IsNotEof())
		{
			strVirGrpKdId=string(GetString("devGrpKdId"));
			strCivilCode=string(GetString("devCivilCode"));
			mapVirGrpKdid2Civilcode.insert(std::pair<string, string>(strVirGrpKdId, strCivilCode));

			MoveNext();
		}

		map<string, string> ::iterator it2;

		//主辅流都有的情况
		tSql.Format("select a.%s,a.%s,a.%s,a.%s,a.%s,a.%s,a.%s,b.%s from %s a inner join %s b on a.%s=b.%s where a.%s<>'' and a.%s<>''",
			F_DEVGRP2DEV_DEVID,F_DEVGRP2DEV_DEVGROUPID,F_DEVGRP2DEV_DEVCAPINDEX,
			F_DEVGRP2DEV_PRIMARYOLDGBID,F_DEVGRP2DEV_PRIMARYNEWGBID,F_DEVGRP2DEV_SECONDARYOLDGBID,F_DEVGRP2DEV_SECONDARYNEWGBID,F_DEVICE_DEVNAME,
			DbStr(GetTblDevGrp2DevName(strTreeId)),T_DEVICE,F_DEVGRP2DEV_DEVID,F_DEVICE_DEVID,F_DEVGRP2DEV_PRIMARYNEWGBID,F_DEVGRP2DEV_SECONDARYNEWGBID);
		if (ExecSql(tSql,enExecMayRet))
		{
			while (IsNotEof())
			{
				//主流
				tXmlGrpDevInfo.devId = GetString(F_DEVGRP2DEV_DEVID);
				tXmlGrpDevInfo.devName = GetString(F_DEVICE_DEVNAME);
				tXmlGrpDevInfo.devOldGbid = GetString(F_DEVGRP2DEV_PRIMARYOLDGBID);
				tXmlGrpDevInfo.devNewGbid = GetString(F_DEVGRP2DEV_PRIMARYNEWGBID);
				tXmlGrpDevInfo.chnid = GetInt(F_DEVGRP2DEV_DEVCAPINDEX);
				tXmlGrpDevInfo.primary = 1;
				tXmlGrpDevInfo.parentId = GetString(F_DEVGRP2DEV_DEVGROUPID);
				
				map<string,string>::iterator it = map_devGrp2devGbid.find(tXmlGrpDevInfo.parentId);
				if (map_devGrp2devGbid.end() != it)
				{
					if (IsDevGrpCivilCode(it->second))
					{
						tXmlGrpDevInfo.civilcode = it->second;
					}
					else
					{
						it2 = mapVirGrpKdid2Civilcode.find(tXmlGrpDevInfo.parentId);
						if(mapVirGrpKdid2Civilcode.end() != it2)
						{
							tXmlGrpDevInfo.civilcode = it2->second;
						}
					}
				}
					

				tPushXmlTreeInfo.grpdevinfo.push_back(tXmlGrpDevInfo);
				
				//辅流
				tXmlGrpDevInfo.primary =0;
				tXmlGrpDevInfo.devOldGbid = GetString(F_DEVGRP2DEV_SECONDARYOLDGBID);
				tXmlGrpDevInfo.devNewGbid = GetString(F_DEVGRP2DEV_SECONDARYNEWGBID);
				tPushXmlTreeInfo.grpdevinfo.push_back(tXmlGrpDevInfo);

				MoveNext();
			}
		}

		//只有主流的情况
		tSql.Format("select a.%s,a.%s,a.%s,a.%s,a.%s,b.%s from %s a inner join %s b on a.%s=b.%s where a.%s<>'' and a.%s=''",
			F_DEVGRP2DEV_DEVID,F_DEVGRP2DEV_DEVGROUPID,F_DEVGRP2DEV_DEVCAPINDEX,
			F_DEVGRP2DEV_PRIMARYOLDGBID,F_DEVGRP2DEV_PRIMARYNEWGBID,F_DEVICE_DEVNAME,
			DbStr(GetTblDevGrp2DevName(strTreeId)),T_DEVICE,F_DEVGRP2DEV_DEVID,F_DEVICE_DEVID,F_DEVGRP2DEV_PRIMARYNEWGBID,F_DEVGRP2DEV_SECONDARYNEWGBID);
		if (ExecSql(tSql,enExecMayRet))
		{
			while (IsNotEof())
			{
				//主流
				tXmlGrpDevInfo.devId = GetString(F_DEVGRP2DEV_DEVID);
				tXmlGrpDevInfo.devName = GetString(F_DEVICE_DEVNAME);
				tXmlGrpDevInfo.devOldGbid = GetString(F_DEVGRP2DEV_PRIMARYOLDGBID);
				tXmlGrpDevInfo.devNewGbid = GetString(F_DEVGRP2DEV_PRIMARYNEWGBID);
				tXmlGrpDevInfo.chnid = GetInt(F_DEVGRP2DEV_DEVCAPINDEX);
				tXmlGrpDevInfo.primary = 1;
				tXmlGrpDevInfo.parentId = GetString(F_DEVGRP2DEV_DEVGROUPID);
				map<string,string>::iterator it = map_devGrp2devGbid.find(tXmlGrpDevInfo.parentId);
				if (map_devGrp2devGbid.end() != it)
				{
					if (IsDevGrpCivilCode(it->second))
					{
						tXmlGrpDevInfo.civilcode = it->second;
					}
					else
					{
						it2 = mapVirGrpKdid2Civilcode.find(tXmlGrpDevInfo.parentId);
						if(mapVirGrpKdid2Civilcode.end() != it2)
						{
							tXmlGrpDevInfo.civilcode = it2->second;
						}
					}
				}

				tPushXmlTreeInfo.grpdevinfo.push_back(tXmlGrpDevInfo);
				MoveNext();
			}
		}

		//只有辅流的情况
		tSql.Format("select a.%s,a.%s,a.%s,a.%s,a.%s,b.%s from %s a inner join %s b on a.%s=b.%s where a.%s='' and a.%s<>''",
			F_DEVGRP2DEV_DEVID,F_DEVGRP2DEV_DEVGROUPID,F_DEVGRP2DEV_DEVCAPINDEX,
			F_DEVGRP2DEV_SECONDARYOLDGBID,F_DEVGRP2DEV_SECONDARYNEWGBID,F_DEVICE_DEVNAME,
			DbStr(GetTblDevGrp2DevName(strTreeId)),T_DEVICE,F_DEVGRP2DEV_DEVID,F_DEVICE_DEVID,F_DEVGRP2DEV_PRIMARYNEWGBID,F_DEVGRP2DEV_SECONDARYNEWGBID);
		if (ExecSql(tSql,enExecMayRet))
		{
			while (IsNotEof())
			{
				//辅流
				tXmlGrpDevInfo.devId = GetString(F_DEVGRP2DEV_DEVID);
				tXmlGrpDevInfo.devName = GetString(F_DEVICE_DEVNAME);
				tXmlGrpDevInfo.devOldGbid = GetString(F_DEVGRP2DEV_SECONDARYOLDGBID);
				tXmlGrpDevInfo.devNewGbid = GetString(F_DEVGRP2DEV_SECONDARYNEWGBID);
				tXmlGrpDevInfo.chnid = GetInt(F_DEVGRP2DEV_DEVCAPINDEX);
				tXmlGrpDevInfo.primary = 0;
				tXmlGrpDevInfo.parentId = GetString(F_DEVGRP2DEV_DEVGROUPID);
				map<string,string>::iterator it = map_devGrp2devGbid.find(tXmlGrpDevInfo.parentId);
				if (map_devGrp2devGbid.end() != it)
				{
					if (IsDevGrpCivilCode(it->second))
					{
						tXmlGrpDevInfo.civilcode = it->second;
					}
					else
					{
						it2 = mapVirGrpKdid2Civilcode.find(tXmlGrpDevInfo.parentId);
						if(mapVirGrpKdid2Civilcode.end() != it2)
						{
							tXmlGrpDevInfo.civilcode = it2->second;
						}
					}
				}

				tPushXmlTreeInfo.grpdevinfo.push_back(tXmlGrpDevInfo);
				MoveNext();
			}
		}

	} while (false);

	WriteTableDataToPushXmlFile(strTreeId,tPushXmlTreeInfo);
	PostDbMsgTransferFromDbTaskPool(cTrans);

}

void CDbOpr::DevGrpDevSumNumQry(const CTaskMsgTransfer<CDeviceGrpDevSumNumReq> *pcMsg)
{
	CTaskMsgTransfer<CDeviceGrpDevSumNumRsp> cTrans;
	CDeviceGrpDevSumNumRsp &cRsp = cTrans.GetMsgObj();

	const CDeviceGrpDevSumNumReq &cReq = pcMsg->GetMsgObj();

	int nSumNum=0;
	ENTasErrno enRet = GetDevGrpDevSumNum(cReq.GetDevTreeId(), cReq.GetDevGrpId(),nSumNum);
	if (ERR_TAS_DBERROR == enRet)
	{
		cRsp.SetErrorCode(ERR_TAS_DBERROR);
	}
	cRsp.SetTotalNum(nSumNum);

	PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
}

ENTasErrno CDbOpr::GetDevGrpDevSumNum( const string &strTreeId, const string& strGrpId,int& dwSumNum)
{
	dwSumNum = 0;
	ENTasErrno enRet = CMS_SUCCESS;
	if (strTreeId.empty() || strGrpId.empty())
	{
		enRet = ERR_TAS_INVALID_PARM;
		return enRet;
	}

	TSqlString tSql;
	
	if (strTreeId==strGrpId) //根分组的查询使用后面的逻辑，性能太差，特殊处理
	{
		tSql.Format("select count(*) from %s where %s<>''",
			DbStr(GetTblDevGrp2DevName(strTreeId)), F_DEVGRP2DEV_PRIMARYNEWGBID);
		if (!ExecSql(tSql, enExecMustRet))
		{
			return ERR_TAS_DBERROR;
		}
		dwSumNum+=GetIntResult();

		tSql.Format("select count(*) from %s where %s<>''",
			DbStr(GetTblDevGrp2DevName(strTreeId)), F_DEVGRP2DEV_SECONDARYNEWGBID);
		if (!ExecSql(tSql, enExecMustRet))
		{
			return ERR_TAS_DBERROR;
		}
		dwSumNum+=GetIntResult();

		return CMS_SUCCESS;
	}

	//先相关分组查出来
	vector<string> vecGrpIds;
	tSql.Format("select %s from %s where %s='%s' or fullpath like '%%%s%%'",
		F_DEVGROUP_DEVGROUPID, DbStr(GetTblDevGrpName(strTreeId)),
		F_DEVGROUP_DEVGROUPID, DbStr(strGrpId),DbStr(strGrpId));
	if (!ExecSql(tSql, enExecMayRet))
	{
		return ERR_TAS_DBERROR;
	}
	while(IsNotEof())
	{
		vecGrpIds.push_back(string(GetString(F_DEVGROUP_DEVGROUPID)));
		MoveNext();
	}
	for (int i = 0 ; i<vecGrpIds.size(); ++i)
	{
		//查询该分组下，主流的个数
		tSql.Format("select count(*) from %s where %s='%s' and %s<>''",
			DbStr(GetTblDevGrp2DevName(strTreeId)), 
			F_DEVGRP2DEV_DEVGROUPID,DbStr(vecGrpIds[i]),
			F_DEVGRP2DEV_PRIMARYNEWGBID);
		if (!ExecSql(tSql, enExecMustRet))
		{
			return ERR_TAS_DBERROR;
		}
		dwSumNum +=GetIntResult();

		//查询该分组下，辅流的个数
		tSql.Format("select count(*) from %s where %s='%s' and %s<>''",
			DbStr(GetTblDevGrp2DevName(strTreeId)), 
			F_DEVGRP2DEV_DEVGROUPID,DbStr(vecGrpIds[i]),
			F_DEVGRP2DEV_SECONDARYNEWGBID);
		if (!ExecSql(tSql, enExecMustRet))
		{
			return ERR_TAS_DBERROR;
		}
		dwSumNum +=GetIntResult();
	}

	return enRet;
}

void CDbOpr::DevGrpDevQry4Gb(const CTaskMsgTransfer<CDeviceGroupDeviceQry4GbReq> *pcMsg)
{
	DatabaseQueryTemplate<CDeviceGroupDeviceQry4GbReq, CDeviceGroupDeviceQry4GbRsp, CDeviceGroupDeviceQry4GbNtfReq, CDeviceGroupDeviceInfo4Gb, TDeviceInfo4Gb>(
		pcMsg, &CDbOpr::PrepareDevGrpDevQry4GbSql, &CDbOpr::FetchDevGrpDevQryRecord4Gb, &CDeviceGroupDeviceQry4GbNtfReq::GetInfos, 
		&CDeviceGroupDeviceInfo4Gb::GetDevinfo, &CDeviceGroupDeviceInfo4Gb::GetDeviceId, &CDeviceGroupDeviceInfo4Gb::GetDevGrpId);
}

ENTasErrno CDbOpr::PrepareDevGrpDevQry4GbSql( const CDeviceGroupDeviceQry4GbReq &cReq, TSqlString &tSqlSelect, TSqlString &tSqlCount )
{
	const CDeviceGroupDeviceInfoQuery4GbCondition cCond = cReq.GetCondition();

	TSqlString tSqlCond;
	string strTreeId;
	string strGrpId;
	cCond.GetDevTreeId(strTreeId);
	
	if (cCond.GetDevicegroupId(m_strFieldValue) && m_strFieldValue.empty())
	{
		return ERR_TAS_INVALID_PARM; // 所有的设备都属于设备组
	}

	tSqlCount.Format("select count(distinct a.%s, a.%s) from %s a inner join %s b on a.%s=b.%s", 
		F_DEVGRP2DEV_DEVGROUPID, F_DEVGRP2DEV_DEVID, 
		 DbStr(GetTblDevGrp2DevName(strTreeId)), DbStr(GetTblDevGrpName(strTreeId)),
		 F_DEVGRP2DEV_DEVGROUPID, F_DEVGROUP_DEVGROUPID);
	AppendQryCondValuePair(cCond.GetDevicegroupId,m_strFieldValue,tSqlCond,F_DEVGRP2DEV_DEVGROUPID,a);

	tSqlCond.EraseLastRedundantAnd();

	if (!tSqlCond.Empty())
	{
		tSqlCount.AppendString(" where ");
		tSqlCount.AppendString("%s", tSqlCond.GetBuf());
	}

	/* 生成select语句 */
	{
		string strDevGrpId;
		cCond.GetDevicegroupId(strDevGrpId);
		tSqlSelect.Format("call %s('%s', '%s', %d, %d)",
			"procDevGrpDevQry4Gb", DbStr(strTreeId), DbStr(strDevGrpId),
			cReq.GetStartEntryIndex(), GetExpectEntryNum(cReq.GetExpectEntryNum()));
	}
	return CMS_SUCCESS;
}

void CDbOpr::FetchDevGrpDevQryRecord4Gb( CDeviceGroupDeviceInfo4Gb &cInfo )
{
	cInfo.SetDevDevTreeId("");
	cInfo.SetDevGrpId(GetString(F_DEVGRP2DEV_DEVGROUPID));
	cInfo.SetDeviceId(GetString(F_DEVGRP2DEV_DEVID));
	cInfo.SetDevName(GetString(F_DEVICE_DEVNAME));
	cInfo.SetDevDomainId(GetString(F_DEVICE_DOMAINID));

	TDeviceInfo4Gb tDev;
	tDev.SetVidSrcName(GetString(F_DEVICECAPINDEXDATA_DEVICECAPINDEXNAME));
	tDev.vidIndex = GetInt(F_DEVGRP2DEV_DEVCAPINDEX);
	
	tDev.gbid.oldprimarygbid = GetString(F_DEVGRP2DEV_PRIMARYOLDGBID);
	tDev.gbid.oldsecondarygbid = GetString(F_DEVGRP2DEV_SECONDARYOLDGBID);
	tDev.gbid.newprimarygbid = GetString(F_DEVGRP2DEV_PRIMARYNEWGBID);
	tDev.gbid.newsecondarygbid = GetString(F_DEVGRP2DEV_SECONDARYNEWGBID);

	cInfo.GetDevinfo().push_back(tDev);
}

void CDbOpr::DevGrpDevQry( const CTaskMsgTransfer<CDeviceGroupDeviceQryReq> *pcMsg )
{
    DatabaseQueryTemplate<CDeviceGroupDeviceQryReq, CDeviceGroupDeviceQryRsp, CDeviceGroupDeviceQryNtfReq, CDeviceGroupDeviceInfo, TDeviceCapIndexs>(
        pcMsg, &CDbOpr::PrepareDevGrpDevQrySql, &CDbOpr::FetchDevGrpDevQryRecord, &CDeviceGroupDeviceQryNtfReq::GetDeviceGroupDeviceInfo, 
        &CDeviceGroupDeviceInfo::GetDeviceCapIndexs, &CDeviceGroupDeviceInfo::GetDeviceID, &CDeviceGroupDeviceInfo::GetDeviceGroupID);
}

ENTasErrno CDbOpr::PrepareDevGrpDevQrySql( const CDeviceGroupDeviceQryReq &cReq, TSqlString &tSqlSelect, TSqlString &tSqlCount )
{
    const CDeviceGroupDeviceInfoQueryCondition &cCond = cReq.GetDeviceGroupDeviceInfoQueryCondition();

    TSqlString tSqlCond;
    if (cCond.GetDeviceGroupID(m_strFieldValue) && m_strFieldValue.empty())
    {
        return ERR_TAS_INVALID_PARM; // 所有的设备都属于设备组
    }

    tSqlCount.Format("select count(distinct a.%s, a.%s, a.%s) from %s a inner join %s b on a.%s=b.%s", 
        F_DEVGRP2DEV_DEVGROUPID, F_DEVGRP2DEV_DEVID, F_DEVGRP2DEV_DEVCAPINDEX, T_DEVGRP2DEV, T_DEVGROUP, F_DEVGRP2DEV_DEVGROUPID, F_DEVGROUP_DEVGROUPID);
    AppendQryCondValuePair(cCond.GetDeviceGroupID, m_strFieldValue, tSqlCond, F_DEVGRP2DEV_DEVGROUPID, a);
    AppendQryCondValuePair(cCond.GetDeviceID, m_strFieldValue, tSqlCond, F_DEVGRP2DEV_DEVID, a);
    AppendQryCondValuePair(cCond.GetDeviceCapID, m_nFieldValue, tSqlCond, F_DEVGRP2DEV_DEVCAPID, a);
    AppendQryCondValuePair(cCond.GetDeviceCapIndex, m_nFieldValue, tSqlCond, F_DEVGRP2DEV_DEVCAPINDEX, a);

    UUIDString cUserId;
    if (cCond.GetCuserID(cUserId))
    {
        CUserInfo cUserInfo;
        if (!GetCuserInfo(cUserId, cUserInfo)) 
            return ERR_TAS_USER_NOT_EXIST;

        if (cUserInfo.GetUsergrpID().empty())
        {
            tSqlCond.AppendString("EXISTS(SELECT * FROM %s c WHERE c.%s='%s' AND c.%s=a.%s)",
                T_CUSER2DEV, F_CUSER2DEV_CUSERID, DbStr(cUserId), F_CUSER2DEV_DEVID, F_DEVGRP2DEV_DEVID);
        }
        else
        {
            tSqlCond.AppendString("EXISTS(SELECT * FROM %s c WHERE c.%s='%s' AND c.%s=a.%s)",
                T_CUSERGRP2DEV, F_CUSERGRP2DEV_CUSERGROUPID, DbStr(cUserInfo.GetUsergrpID()), 
                F_CUSERGRP2DEV_DEVID, F_DEVGRP2DEV_DEVID);
        }
    }

    tSqlCond.EraseLastRedundantAnd();

    if (!tSqlCond.Empty())
    {
        tSqlCount.AppendString(" where ");
        tSqlCount.AppendString("%s", tSqlCond.GetBuf());
    }
    
    /* 生成select语句 */
    {
        string strUserId;
        string strDevGrpId;
        string strDevId;
        s32 nDevCapId = -1;
        s32 nDevCapIndex = -1;
        cCond.GetCuserID(strUserId);
        cCond.GetDeviceGroupID(strDevGrpId);
        cCond.GetDeviceID(strDevId);
        cCond.GetDeviceCapID(nDevCapId);
        cCond.GetDeviceCapIndex(nDevCapIndex);
        tSqlSelect.Format("call %s('%s', '%s', '%s', %d, %d, %d, %d)",
            P_DEVGRPDEVQRY, DbStr(strUserId), DbStr(strDevGrpId), DbStr(strDevId), nDevCapId,
            nDevCapIndex, cReq.GetStartEntryIndex(), GetExpectEntryNum(cReq.GetExpectEntryNum(), TRUE));
    }

    return CMS_SUCCESS;
}

void CDbOpr::FetchDevGrpDevQryRecord( CDeviceGroupDeviceInfo &cInfo )
{
    cInfo.SetDeviceGroupID(GetString(F_DEVGRP2DEV_DEVGROUPID));
    cInfo.SetDeviceID(GetString(F_DEVGRP2DEV_DEVID));
    cInfo.SetDomainId(GetString(F_DEVGROUP_DOMAINID));

    TDeviceCapIndexs tCapIndex;
    tCapIndex.id = GetInt(F_DEVGRP2DEV_DEVCAPID);
    tCapIndex.indexs.clear();
    tCapIndex.indexs.push_back(GetInt(F_DEVGRP2DEV_DEVCAPINDEX));

    cInfo.GetDeviceCapIndexs().clear();
    cInfo.GetDeviceCapIndexs().push_back(tCapIndex);
}

ENTasErrno CDbOpr::DevGrpDevItemAdd( const CDeviceGroupDeviceInfo &cInfo,bool bAddDevWithGrp ,const string &grpTreeId,const string &devTreeId)
{
    TSqlString tSql;

	bool bDstTreeCanBeUse4Gb = false;
	//先判断目的树是否可用于选择推送
	if (!grpTreeId.empty())
	{
		CDevTreeInfo devTree;
		if(!GetDevTreeInfo(grpTreeId, devTree))
		{
			return ERR_TAS_RECORD_NOT_EXISTS;
		}
		bDstTreeCanBeUse4Gb = devTree.GetCanBeUsed4Gb();
	}

    const vector<TDeviceCapIndexs> &tDevCapIndexs = cInfo.GetDeviceCapIndexs();
	double devSeq=0;
    for (u32 i = 0; i < tDevCapIndexs.size(); ++i)
    {
        for (u32 j = 0; j < tDevCapIndexs[i].indexs.size(); ++j)
        {
			// 如果是带设备组入网设备的话，不需要删除,减少SQL执行 [12/4/2015 pangubing]
			if (!bAddDevWithGrp&&grpTreeId.empty())
			{
				tSql.Format("DELETE FROM %s WHERE %s='%s' AND %s=%d AND %s=%d", 
					DbStr(GetTblDevGrp2DevName(grpTreeId)), F_DEVGRP2DEV_DEVID, DbStr(cInfo.GetDeviceID()),
					F_DEVGRP2DEV_DEVCAPID, tDevCapIndexs[i].id, F_DEVGRP2DEV_DEVCAPINDEX, tDevCapIndexs[i].indexs[j]);
				if (!ExecSql(tSql, enExecNoRet))
				{
					return ERR_TAS_DBERROR;
				}
			}
			if (cInfo.GetDomainId() != CurrentDomainId() && !cInfo.GetDomainId().empty())
			{
				devSeq = 0;
			}
			else if(!GetDevGrpNextDevSeq(grpTreeId, cInfo.GetDeviceGroupID(), cInfo.GetDeviceID(),tDevCapIndexs[i].indexs[j],devSeq))
			{
				return ERR_TAS_DBERROR;
			}
			if (!bDstTreeCanBeUse4Gb)
			{
				tSql.Format("INSERT ignore INTO %s VALUES('%s', '%s', %d, %d, %.14lf)", DbStr(GetTblDevGrp2DevName(grpTreeId)),
					DbStr(cInfo.GetDeviceGroupID()), DbStr(cInfo.GetDeviceID()), tDevCapIndexs[i].id, tDevCapIndexs[i].indexs[j],
					devSeq);
			}
			else
			{
				//本域215分组下不允许划归设备
				tSql.Format("select %s from %s where %s='%s'",F_DEVGROUP_NEWGBID, DbStr(GetTblDevGrpName(grpTreeId)),
					F_DEVGROUP_DEVGROUPID, DbStr(cInfo.GetDeviceGroupID()));
				if (!ExecSql(tSql, enExecMayRet) || IsEof())
				{
					return ERR_TAS_DBERROR;
				}
				string strGrpGbId = GetString(F_DEVGROUP_NEWGBID);
				if (IsDevGrpBussinessGrp(strGrpGbId))
				{
					return ERR_TAS_INVALID_OPERATION;
				}

				tSql.Format("INSERT ignore INTO %s VALUES('%s', '%s', %d, %d,'','','','', %.14lf)", DbStr(GetTblDevGrp2DevName(grpTreeId)),
					DbStr(cInfo.GetDeviceGroupID()), DbStr(cInfo.GetDeviceID()), tDevCapIndexs[i].id, tDevCapIndexs[i].indexs[j],
					devSeq);
			}
			
            if (!ExecSql(tSql, enExecNoRet))
            {
                return ERR_TAS_DBERROR;
            }
        }
    }
    
    return CMS_SUCCESS;
}

ENTasErrno CDbOpr::DevGrpDevItemDel( const CDeviceGroupDeviceInfo &cInfo )
{
    CDeviceGroupDeviceInfo cNewInfo = cInfo;
    CDeviceGroupInfo cDevGrpInfo;

    ENTasErrno enErr = DevGrpItemQry(cNewInfo.GetDeviceGroupID(), cDevGrpInfo);
    if (CMS_SUCCESS != enErr)
    {
        return enErr;
    }

    cNewInfo.SetDomainId(cDevGrpInfo.GetDomainUUID());
    cNewInfo.SetDeviceGroupID(cNewInfo.GetDomainId());

    return DevGrpDevItemAdd(cNewInfo);
}


ENTasErrno CDbOpr::GetDeviceGroupDeviceInfoKeys( const string &strDevId, vector<CDeviceGroupDeviceInfoKey> &tKeys )
{
    ENTasErrno enRet = CMS_SUCCESS;

    TSqlString tSql;
    tSql.Format("select * from %s where %s='%s'", 
        T_DEVGRP2DEV, F_DEVGRP2DEV_DEVID, DbStr(strDevId));

    tKeys.clear();
    if (ExecSql(tSql, enExecMayRet))
    {
        if (IsNotEof())
        {
            CDeviceGroupDeviceInfoKey tKey;
            TDeviceCapIndexs tCapIndex;
            while (IsNotEof())
            {
                tKey.SetDeviceGroupID(GetString(F_DEVGRP2DEV_DEVGROUPID));
                tKey.SetDeviceID(GetString(F_DEVGRP2DEV_DEVID));

                tCapIndex.id = GetInt(F_DEVGRP2DEV_DEVCAPID);
                tCapIndex.indexs.clear();
                tCapIndex.indexs.push_back(GetInt(F_DEVGRP2DEV_DEVCAPINDEX));
                tKey.GetDeviceCapIndexs().clear();
                tKey.GetDeviceCapIndexs().push_back(tCapIndex);

                tKeys.push_back(tKey);

                if (!MoveNext())
                {
                    enRet = ERR_TAS_DBERROR;
                    break;
                }
            }
        }
        else
        {
            enRet = ERR_TAS_RECORD_NOT_EXISTS;
        }
    }
    else
    {
        enRet = ERR_TAS_DBERROR;
    }

    return enRet;
}


void CDbOpr::CuserGrpAdd( const CTaskMsgTransfer<CUserGroupAddReq> *pcMsg )
{
    CTaskMsgTransfer<CUserGroupAddRsp> cTrans;
    CTaskMsgTransfer<CUserGroupAddNtfReq> cTransNtf;
    CUserGroupAddRsp &cRsp = cTrans.GetMsgObj();
    CUserGroupAddNtfReq &cNtf = cTransNtf.GetMsgObj();
    bool bHasNtf = false;

    const CUserGroupAddReq &cReq = pcMsg->GetMsgObj();

    do 
    {
        const CUserGroupInfo &cInfo = cReq.GetUserGroupInfo();
        if (IsExistSameNameCuserGrp(cInfo.GetGroupName()))
        {
            cRsp.SetErrorCode(ERR_TAS_EXIST_SAMENAME_RECORD);
            break;
        }

        CUserInfo cUserInfo;
        if (!GetCuserInfo(cInfo.GetCreateUser(), cUserInfo))
        {
            cRsp.SetErrorCode(ERR_TAS_INVALID_PARM);
            break;
        }

        CUUID grpId;
        TSqlString tSql;
        tSql.Format("insert into %s values('%s', '%s', '%s', '%s', %d, %d, '%s', '', %d, %d, '%s', %d, %d, %d, %d, %d)", 
            T_CUSERGROUP, DbStr(grpId), DbStr(cInfo.GetGroupName()), DbStr(cInfo.GetDescription()), 
            DbNowDateTime(), cInfo.GetUserType(), (s32)cInfo.GetUserPrivilege().Convert2Int(), 
            DbStr(cInfo.GetCreateUser()), cInfo.GetSwitchNum(), cInfo.GetWaterMarkEnabled(), DbStr(cInfo.GetWaterMarkFont()),
			cInfo.GetWaterMarkFontSize(), cInfo.GetWaterMarkRotationAngle(), cInfo.GetWaterMarkColor(), cInfo.GetWaterMarkTransparent(),
			cInfo.GetWaterMarkTiled());

        if (ExecSql(tSql, enExecNoRet))
        {
            cRsp.GetUserGroupInfoKey().SetGroupNO(grpId);
            bHasNtf = GetCuserGrpInfo(grpId, cNtf.GetUserGroupInfo());
        }
        else
        {
            cRsp.SetErrorCode(ERR_TAS_DBERROR);
            break;
        }
    } while (false);

    PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
    if (bHasNtf) PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cNtf, cTransNtf);
}

void CDbOpr::CuserGrpDel( const CTaskMsgTransfer<CUserGroupDelReq> *pcMsg )
{
    CTaskMsgTransfer<CUserGroupDelRsp> cTrans;
    CTaskMsgTransfer<CUserGroupDelNtfReq> cTransNtf;
    CUserGroupDelRsp &cRsp = cTrans.GetMsgObj();
    CUserGroupDelNtfReq &cNtf = cTransNtf.GetMsgObj();

    const CUserGroupDelReq &cReq = pcMsg->GetMsgObj();
    CUUID cOprId;
    g_cDbTaskPool.AddTransProgress(cOprId, 100, 5);
    cRsp.SetOperUuid(cOprId);
    PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);

    TSqlString tSql;
    tSql.Format("call %s('%s')", P_CUSERGRPDEL, DbStr(cReq.GetUserGroupInfoKey().GetGroupNO()));
    if (ExecSql(tSql, enExecNoRet))
    {
        g_cDbTaskPool.AddTransProgress(cOprId, 90);
        cNtf.GetUserGroupInfoKey().SetGroupNO(cReq.GetUserGroupInfoKey().GetGroupNO());

        // 删除用户组后删除用户组对应的自动更新组信息
        ClearUserAutoAssignDeviceGroup(cReq.GetUserGroupInfoKey().GetGroupNO(), 1);

        RedisDelUsergrp(cReq.GetUserGroupInfoKey().GetGroupNO());

        g_cDbTaskPool.AddTransProgress(cOprId, 100);
        PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cNtf, cTransNtf);
    }
    else
    {
        g_cDbTaskPool.AddTransProgress(cOprId, -1, ERR_TAS_DBERROR);
    }
}

void CDbOpr::CuserGrpMod( const CTaskMsgTransfer<CUserGroupModReq> *pcMsg )
{
    CTaskMsgTransfer<CUserGroupModRsp> cTrans;
    CTaskMsgTransfer<CUserGroupModNtfReq> cTransNtf;
    CUserGroupModRsp &cRsp = cTrans.GetMsgObj();
    CUserGroupModNtfReq &cNtf = cTransNtf.GetMsgObj();
    bool bHasNtf = false;

    const CUserGroupModReq &cReq = pcMsg->GetMsgObj();
    do 
    {
        if (cReq.GetGroupName(m_strFieldValue))
        {
            cReq.GetGroupNO(m_strFieldValue2);
            if (IsExistSameNameCuserGrp(m_strFieldValue, &m_strFieldValue2))
            {
                cRsp.SetErrorCode(ERR_TAS_EXIST_SAMENAME_RECORD);
                break;
            }
        }

        TSqlString tSql;
        CUserPrivilege cPrivi;
		

		tSql.Format("update %s set ", T_CUSER);
		if (cReq.GetUserPrivilege(cPrivi))
		{
			tSql.AppendString("%s=%d,", F_CUSER_CUSERPRIVILEGE, (s32)cPrivi.Convert2Int());
		}
		AppendUpdateValuePair(cReq.GetSwitchNum, m_nFieldValue, tSql, F_CUSER_SWITCHNUM);
		AppendUpdateValuePair(cReq.GetWaterMarkEnabled, m_bFieldValue, tSql, F_CUSER_WATERMARK_ENABLED);
		AppendUpdateValuePair(cReq.GetWaterMarkFont, m_strFieldValue, tSql, F_CUSER_WATERMARK_FONT);
		AppendUpdateValuePair(cReq.GetWaterMarkFontSize, m_nFieldValue, tSql, F_CUSER_WATERMARK_FONT_SIZE);
		AppendUpdateValuePair(cReq.GetWaterMarkRotationAngle, m_nFieldValue, tSql, F_CUSER_WATERMARK_ROTATION_ANGLE);
		AppendUpdateValuePair(cReq.GetWaterMarkColor, m_nFieldValue, tSql, F_CUSER_WATERMARK_COLOR);
		AppendUpdateValuePair(cReq.GetWaterMarkTransparent, m_nFieldValue, tSql, F_CUSER_WATERMARK_TRANSPARENT);
		AppendUpdateValuePair(cReq.GetWaterMarkTiled, m_bFieldValue, tSql, F_CUSER_WATERMARK_TILED);

		tSql.EraseLastRedundantComma();
		cReq.GetGroupNO(m_strFieldValue);
		tSql.AppendString(" where %s = '%s'", F_CUSER_USERGROUPID, DbStr(m_strFieldValue));
		
		if (!ExecSql(tSql, enExecNoRet))
		{
			cRsp.SetErrorCode(ERR_TAS_DBERROR);
			break;
		}


        tSql.Format("update %s set ", T_CUSERGROUP);
        AppendUpdateValuePair(cReq.GetGroupName, m_strFieldValue, tSql, F_CUSERGROUP_GROUPNAME);
        AppendUpdateValuePair(cReq.GetDescription, m_strFieldValue, tSql, F_CUSERGROUP_GROUPDESC);
		AppendUpdateValuePair(cReq.GetSwitchNum, m_nFieldValue, tSql, F_CUSERGROUP_SWITCHNUM);
		AppendUpdateValuePair(cReq.GetWaterMarkEnabled, m_bFieldValue, tSql, F_CUSER_WATERMARK_ENABLED);
		AppendUpdateValuePair(cReq.GetWaterMarkFont, m_strFieldValue, tSql, F_CUSER_WATERMARK_FONT);
		AppendUpdateValuePair(cReq.GetWaterMarkFontSize, m_nFieldValue, tSql, F_CUSER_WATERMARK_FONT_SIZE);
		AppendUpdateValuePair(cReq.GetWaterMarkRotationAngle, m_nFieldValue, tSql, F_CUSER_WATERMARK_ROTATION_ANGLE);
		AppendUpdateValuePair(cReq.GetWaterMarkColor, m_nFieldValue, tSql, F_CUSER_WATERMARK_COLOR);
		AppendUpdateValuePair(cReq.GetWaterMarkTransparent, m_nFieldValue, tSql, F_CUSER_WATERMARK_TRANSPARENT);
		AppendUpdateValuePair(cReq.GetWaterMarkTiled, m_bFieldValue, tSql, F_CUSER_WATERMARK_TILED);
        if (cReq.GetUserPrivilege(cPrivi))
        {
            tSql.AppendString("%s=%d,", F_CUSERGROUP_CUSERPRIVILEGE, (s32)cPrivi.Convert2Int());
        }
        tSql.EraseLastRedundantComma();

        cReq.GetGroupNO(m_strFieldValue);
        tSql.AppendString(" where %s = '%s'", F_CUSERGROUP_CUSERGROUPID, DbStr(m_strFieldValue));
        if (ExecSql(tSql, enExecNoRet))
        {
            bHasNtf = GetCuserGrpInfo(m_strFieldValue, cNtf.GetUserGroupInfo());
        }
        else
        {
            cRsp.SetErrorCode(ERR_TAS_DBERROR);
        }
    } while (false);

    PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
    if (bHasNtf) PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cNtf, cTransNtf);
}

void CDbOpr::CuserGrpQry( const CTaskMsgTransfer<CUserGroupQryReq> *pcMsg )
{
    DatabaseQueryTemplate<CUserGroupQryReq, CUserGroupQryRsp, CUserGroupQryNtfReq, CUserGroupInfo>(
        pcMsg, &CDbOpr::PrepareCuserGrpQrySql, &CDbOpr::FetchCuserGrpQryRecord, 
        &CUserGroupQryNtfReq::GetUserGroupInfo, &CUserGroupInfo::GetGroupNO);
}


void CDbOpr::UpdateCuserGrpDevTreeVer( const UUIDString &strUserGrpId )
{
	++s_dwDevTreeVerExt;

    CCMSTime cTm;
    TSqlString tSql;
    tSql.Format("update %s set %s='%s_%u' where %s='%s'", 
        T_CUSERGROUP, F_CUSERGROUP_DEVTREEVER, DbStr(cTm.ToString()), s_dwDevTreeVerExt, 
        F_CUSERGROUP_CUSERGROUPID, DbStr(strUserGrpId));
    if (!ExecSql(tSql, enExecNoRet))
    {
        CLASSLOG(enLogSrcDbTaskPool, ERROR_LEV, 
            "update cuserGroup's devTreeVer failed, cuserGroupId=%s\n", strUserGrpId.c_str());
    }
}


ENTasErrno CDbOpr::PrepareCuserGrpQrySql( const CUserGroupQryReq &cReq, TSqlString &tSqlSelect, TSqlString &tSqlCount )
{
    const CUserGroupInfoQueryCondition &cCond = cReq.GetUserGroupInfoQueryCondition();

    tSqlSelect.Format("select a.* from %s a", T_CUSERGROUP);
    tSqlCount.Format("select count(*) from %s a", T_CUSERGROUP);

    TSqlString tSqlCond;
    AppendQryCondValuePair(cCond.GetGroupNO, m_strFieldValue, tSqlCond, F_CUSERGROUP_CUSERGROUPID, a);
    AppendQryCondValuePair(cCond.GetCuserID, m_strFieldValue, tSqlCond, F_CUSERGROUP_USERCREATORID, a);
    AppendQryCondValuePair(cCond.GetGroupName, m_strFieldValue, tSqlCond, F_CUSERGROUP_GROUPNAME, a);
    AppendQryCondValuePair(cCond.GetDescription, m_strFieldValue, tSqlCond, F_CUSERGROUP_GROUPDESC, a);
    AppendQryCondDatePeriodPair(cCond.GetCreateDate_From, m_strFieldValue, cCond.GetCreateDate_To, m_strFieldValue2,
        tSqlCond, F_CUSERGROUP_CREATEDATE, a);
    tSqlCond.EraseLastRedundantAnd();

    if (!tSqlCond.Empty())
    {
        tSqlSelect.AppendString(" where ");
        tSqlSelect.AppendString("%s", tSqlCond.GetBuf());
        tSqlCount.AppendString(" where ");
        tSqlCount.AppendString("%s", tSqlCond.GetBuf());
    }

    tSqlSelect.AppendString(" order by a.%s", F_CUSERGROUP_CUSERGROUPID);
    AddRowNumLimitModifer(tSqlSelect, cReq.GetStartEntryIndex(), cReq.GetExpectEntryNum());
    return CMS_SUCCESS;
}

void CDbOpr::FetchCuserGrpQryRecord( CUserGroupInfo &cInfo )
{
    cInfo.SetGroupNO(GetString(F_CUSERGROUP_CUSERGROUPID));
    cInfo.SetGroupName(GetString(F_CUSERGROUP_GROUPNAME));
    cInfo.SetDescription(GetString(F_CUSERGROUP_GROUPDESC));
    cInfo.SetUserType(GetInt(F_CUSERGROUP_CUSERTYPE));
    CUserPrivilege cPrivi;
    cPrivi.ConvertFromInt(GetInt(F_CUSERGROUP_CUSERPRIVILEGE));
    cInfo.SetUserPrivilege(cPrivi);
    cInfo.SetCreateUser(GetString(F_CUSERGROUP_USERCREATORID));
    cInfo.SetCreateDate(GetCmsDate(F_CUSERGROUP_CREATEDATE));
    cInfo.SetDeviceTreeVer(GetString(F_CUSERGROUP_DEVTREEVER));
	cInfo.SetSwitchNum(GetInt(F_CUSERGROUP_SWITCHNUM));
	cInfo.SetWaterMarkEnabled(GetBool(F_CUSERGROUP_WATERMARK_ENABLED));
	cInfo.SetWaterMarkFont(GetString(F_CUSERGROUP_WATERMARK_FONT));
	cInfo.SetWaterMarkFontSize(GetInt(F_CUSERGROUP_WATERMARK_FONT_SIZE));
	cInfo.SetWaterMarkRotationAngle(GetInt(F_CUSERGROUP_WATERMARK_ROTATION_ANGLE));
	cInfo.SetWaterMarkColor(GetInt(F_CUSERGROUP_WATERMARK_COLOR));
	cInfo.SetWaterMarkTransparent(GetInt(F_CUSERGROUP_WATERMARK_TRANSPARENT));
	cInfo.SetWaterMarkTiled(GetBool(F_CUSERGROUP_WATERMARK_TILED));
}

bool CDbOpr::GetCuserGrpInfo( const UUIDString &strId, CUserGroupInfo &cInfo )
{
    TSqlString tSql;
    tSql.Format("select * from %s where %s='%s'", T_CUSERGROUP, F_CUSERGROUP_CUSERGROUPID, DbStr(strId));

    if (ExecSql(tSql, enExecMayRet))
    {
        FetchCuserGrpQryRecord(cInfo);
        return true;
    }
    else
    {
        return false;
    }
}

void CDbOpr::CuserGrpDevAdd( const CTaskMsgTransfer<CUserGroupDeviceAddReq> *pcMsg )
{
    CTaskMsgTransfer<CUserGroupDeviceAddRsp> cTrans;
    CTaskMsgTransfer<CUserGroupDeviceAddNtfReq> cTransNtf;
    CUserGroupDeviceAddRsp &cRsp = cTrans.GetMsgObj();
    CUserGroupDeviceAddNtfReq &cNtf = cTransNtf.GetMsgObj();
    bool bHasNtf = false;

    const CUserGroupDeviceAddReq &cReq = pcMsg->GetMsgObj();

    do 
    {
        CUserGroupDeviceInfo cInfo = cReq.GetUserGroupDeviceInfo();
        if (cInfo.GetDeviceCapPrivilege().size() == 0)
        {
            cRsp.SetErrorCode(ERR_TAS_INVALID_PARM);
            break;
        }

		CDeviceInfo cDevInfo;
		ENTasErrno enDevQryErr = DeviceItemQry(cInfo.GetDeviceID(), cDevInfo);
		if (CMS_SUCCESS != enDevQryErr)
		{
			cRsp.SetErrorCode(enDevQryErr);
			break;
		}

        /* 编码设备时自动划归编码通道权限 */
        CDeviceModelInfo cDevModelInfo;
        cRsp.SetErrorCode(DeviceModelItemQryByDevId(cInfo.GetDeviceID(), cDevModelInfo));
        if (cRsp.GetErrorCode() != CMS_SUCCESS) break;
        if (cDevModelInfo.GetDeviceTypeId() == DEVICE_TYPE_ENCODER)
        {
            GenerateEncodeCapPrivilegeInfo(cDevModelInfo, 
                cReq.GetUserGroupDeviceInfo().GetDeviceCapPrivilege(),
                cInfo.GetDeviceCapPrivilege());
        }

        CUserDeviceInfo cUserDevInfo;
        cUserDevInfo.SetDeviceID(cInfo.GetDeviceID());
        cUserDevInfo.SetDevicePrivilege(cInfo.GetDevicePrivilege());
        cUserDevInfo.SetDeviceCapPrivilege(cInfo.GetDeviceCapPrivilege());
        cRsp.SetErrorCode(ValidateCurrentUserDevPrivilege(cReq.GetSession(), cUserDevInfo));
        if (cRsp.GetErrorCode() != CMS_SUCCESS)
        {
            break;
        }

		cRsp.SetErrorCode(CuserGrpDevItemAdd(cInfo));
        if (cRsp.GetErrorCode() != CMS_SUCCESS)
        {
            break;
        }

		RedisAddUsergrpDevPrivilege(cInfo);

        UpdateCuserGrpDevTreeVer(cInfo.GetGroupID());

        bHasNtf = true;
		cInfo.SetDevDomainID(cDevInfo.GetDomainUUID());
        cNtf.SetUserGroupDeviceInfo(cInfo);
    } while (false);

    PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
    if (bHasNtf)
    {
        PostTaskMsgTransfer2CCtrl4LargeNtf(CUserGroupDeviceAddNtfReq, GetUserGroupDeviceInfo, GetDeviceCapPrivilege)
    }
}

void CDbOpr::CuserGrpDevDel( const CTaskMsgTransfer<CUserGroupDeviceDelReq> *pcMsg )
{
    CTaskMsgTransfer<CUserGroupDeviceDelRsp> cTrans;
    CTaskMsgTransfer<CUserGroupDeviceDelNtfReq> cTransNtf;
    CUserGroupDeviceDelRsp &cRsp = cTrans.GetMsgObj();
    CUserGroupDeviceDelNtfReq &cNtf = cTransNtf.GetMsgObj();
    bool bHasNtf = false;

    const CUserGroupDeviceDelReq &cReq = pcMsg->GetMsgObj();
    CUserGroupDeviceInfoKey cKey = cReq.GetUserGroupDeviceInfoKey();


    do 
    {
		CDeviceInfo cDevInfo;
		ENTasErrno enDevQryErr = DeviceItemQry(cKey.GetDeviceID(), cDevInfo);
		if (CMS_SUCCESS != enDevQryErr)
		{
			cRsp.SetErrorCode(enDevQryErr);
			break;
		}

		// 检查该设备所在组是否是自动更新组，是则不允许取消划归
		if (IsAutoAssignDeviceGroup(cKey.GetDeviceID(), cKey.GetDeviceCapIndexs(), cKey.GetGroupID(), 1))
		{
			cRsp.SetErrorCode(ERR_TAS_DEV_GROUP_IS_AUTO_UPDATE);
			break;
		}

        /* 编码设备时自动删除编码通道权限 */
        CDeviceModelInfo cDevModelInfo;
        cRsp.SetErrorCode(DeviceModelItemQryByDevId(cKey.GetDeviceID(), cDevModelInfo));
        if (cRsp.GetErrorCode() != CMS_SUCCESS) break;
        if (cDevModelInfo.GetDeviceTypeId() == DEVICE_TYPE_ENCODER)
        {
            GenerateEncodeCapIndexInfo(cDevModelInfo, 
                cReq.GetUserGroupDeviceInfoKey().GetDeviceCapIndexs(),
                cKey.GetDeviceCapIndexs());
        }

        // 删除收藏夹中的数据
        vector<CUserInfo> vecCuserInfo;
        cRsp.SetErrorCode(CuserQryByUserGrp(cKey.GetGroupID(), vecCuserInfo));
        if (cRsp.GetErrorCode() != CMS_SUCCESS) break;
        for (vector<CUserInfo>::const_iterator it=vecCuserInfo.begin();it!=vecCuserInfo.end();++it)
        {
            for (vector<TDeviceCapIndexs>::const_iterator it2=cKey.GetDeviceCapIndexs().begin();
                it2!=cKey.GetDeviceCapIndexs().end();++it2)
            {
                if (it2->id!=DEVICE_CAP_VIDEOSOURCE) continue;

                for (vector<int>::const_iterator it3=it2->indexs.begin();it3!=it2->indexs.end();++it3)
                {
                    DevGrpDevDel4Favorite(it->GetUserID(), cKey.GetDeviceID(), *it3);
                }
            }
        }
        

		cRsp.SetErrorCode(CuserGrpDevItemDel(cKey));

		// 删除用户组设备，删除用户对应的PTZ锁 [4/14/2016 pangubing]
		vector<CUserInfo> vctUserInfos;
		CuserQryByUserGrp(cKey.GetGroupID(),vctUserInfos);
		for (u32 k = 0; k< vctUserInfos.size(); ++k)
		{
			DeleteUserPtzLockInner(pcMsg,vctUserInfos[k].GetUserID(),cKey.GetDeviceID(),cKey.GetDeviceCapIndexs());
		}

		if (cRsp.GetErrorCode() != CMS_SUCCESS)
		{
			break;
		}

        UpdateCuserGrpDevTreeVer(cKey.GetGroupID());

        if (cRsp.GetErrorCode() == CMS_SUCCESS)
        {
            bHasNtf = true;

			cNtf.SetDevDomainId(cDevInfo.GetDomainUUID());
            cNtf.SetUserGroupDeviceInfoKey(cKey);

			RedisDelUsergrpDevPrivilege(cKey);
        }
    } while (false);

    PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
    if (bHasNtf) PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cNtf, cTransNtf);
}

ENTasErrno CDbOpr::CuserGrpDevDel( const UUIDString &devId )
{
    TSqlString tSql;
    tSql.Format("delete from %s where %s='%s'", T_CUSERGRP2DEVICECAP, F_CUSERGRP2DEVICECAP_DEVID, DbStr(devId));
    if (!ExecSql(tSql, enExecNoRet)) return ERR_TAS_DBERROR;

    tSql.Format("delete from %s where %s='%s'", T_CUSERGRP2DEV, F_CUSERGRP2DEV_DEVID, DbStr(devId));
    if (!ExecSql(tSql, enExecNoRet)) return ERR_TAS_DBERROR;

    return CMS_SUCCESS;
}

void CDbOpr::CuserGrpDevMod( const CTaskMsgTransfer<CUserGroupDeviceModReq> *pcMsg )
{
    CTaskMsgTransfer<CUserGroupDeviceModRsp> cTrans;
    CTaskMsgTransfer<CUserGroupDeviceModNtfReq> cTransNtf;
    CUserGroupDeviceModRsp &cRsp = cTrans.GetMsgObj();
    CUserGroupDeviceModNtfReq &cNtf = cTransNtf.GetMsgObj();
    bool bHasNtf = false;

    const CUserGroupDeviceModReq &cReq = pcMsg->GetMsgObj();
    bool bModDevCapPriv;

    do 
    {
        TSqlString tSql;
        CDevicePrivilege cDevPrivi;
        vector<TDeviceCapPrivilege> cDevCapPrivis;
        UUIDString cGrpId, cDevId;
        cReq.GetDevicePrivilege(cDevPrivi);
        bModDevCapPriv = cReq.GetDeviceCapPrivilege(cDevCapPrivis);
		// 增加校验视频源权限 [12/11/2015 pangubing]
		if (bModDevCapPriv)
		{
			cRsp.SetErrorCode(IsDevCapPrivilegeVailed(cDevCapPrivis));
			if (cRsp.GetErrorCode() != CMS_SUCCESS)
			{
				break;
			}
		}

        cReq.GetGroupID(cGrpId);
        cReq.GetDeviceID(cDevId);

        // 自动调整编码通道权限
        CDeviceModelInfo cDevModelInfo;
        cRsp.SetErrorCode(DeviceModelItemQryByDevId(cDevId, cDevModelInfo));
        if (cRsp.GetErrorCode() != CMS_SUCCESS)
        {
            break;
        }
        if (cDevModelInfo.GetDeviceTypeId() == DEVICE_TYPE_ENCODER)
        {
            vector<TDeviceCapPrivilege> cDevCapPrivisTmp(cDevCapPrivis);
            GenerateEncodeCapPrivilegeInfo(cDevModelInfo, cDevCapPrivisTmp, cDevCapPrivis);
        }

        CUserDeviceInfo cUserDevInfo4Validate;
        cUserDevInfo4Validate.SetDeviceID(cDevId);
        cUserDevInfo4Validate.SetDevicePrivilege(cDevPrivi);
        cUserDevInfo4Validate.SetDeviceCapPrivilege(cDevCapPrivis);
        cRsp.SetErrorCode(ValidateCurrentUserDevPrivilege(cReq.GetSession(), cUserDevInfo4Validate));
        if (cRsp.GetErrorCode() != CMS_SUCCESS)
        {
            break;
        }

        if (cReq.GetDevicePrivilege(cDevPrivi))
        {
            tSql.Format("update %s set %s=%d where %s='%s' and %s='%s'", T_CUSERGRP2DEV,
                F_CUSERGRP2DEV_DEVPRIVILEGE, (s32)cDevPrivi.Convert2Int(), F_CUSERGRP2DEV_CUSERGROUPID, DbStr(cGrpId),
                F_CUSERGRP2DEV_DEVID, DbStr(cDevId));
            if (!ExecSql(tSql, enExecNoRet))
            {
                cRsp.SetErrorCode(ERR_TAS_DBERROR);
                break;
            }
        }

        if (bModDevCapPriv && cRsp.GetErrorCode() == CMS_SUCCESS)
        {
			TSqlString tSql;
			vector<CUserInfo> vctUserInfos;
			CuserQryByUserGrp(cGrpId,vctUserInfos);

			CVideosourcePrivilege tmpPrivilege;
            u32 i;
            for (i = 0; i < cDevCapPrivis.size(); ++i)
            {
                map<int, CBitsContainer>::iterator it;
                for (it = cDevCapPrivis[i].privileges.begin(); it != cDevCapPrivis[i].privileges.end(); ++it)
                {
                    tSql.Format("update %s set %s=%d where %s='%s' and %s='%s' and %s=%d and %s=%d",
                        T_CUSERGRP2DEVICECAP, F_CUSERGRP2DEVICECAP_DEVCAPINDEXPRIVILEGE, (s32)it->second.Convert2Int(),
                        F_CUSERGRP2DEVICECAP_CUSERGROUPID, DbStr(cGrpId), F_CUSERGRP2DEVICECAP_DEVID, DbStr(cDevId),
                        F_CUSERGRP2DEVICECAP_DEVCAPID, cDevCapPrivis[i].id, F_CUSERGRP2DEVICECAP_DEVCAPINDEX, it->first);
                    if (!ExecSql(tSql, enExecNoRet))
                    {
                        cRsp.SetErrorCode(ERR_TAS_DBERROR);
                        break;
                    }

					// 取消视频源PTZ锁权限，删除PTZ锁 [4/14/2016 pangubing]
					if (DEVICE_CAP_VIDEOSOURCE == cDevCapPrivis[i].id)
					{
						tmpPrivilege.ConvertFromBitsContainer(it->second);
						if (!tmpPrivilege.HasPrivilege(CVideosourcePrivilege::en_PTZ_Lock))
						{
							for (u32 k = 0; k< vctUserInfos.size(); ++k)
							{
								CPtzLockInfo cInfo;
								if (GetPtzLockInfo(cDevId,it->first,cInfo)&&cInfo.GetUserId()==vctUserInfos[k].GetUserID())
								{
									DeletePtzLockInner(pcMsg,cDevId,it->first);
								}
							}
						}
					}

                }

                if (it != cDevCapPrivis[i].privileges.end())
                {
                    break;
                }
            }

            if (i != cDevCapPrivis.size())
            {
                break;
            }
        }

        UpdateCuserGrpDevTreeVer(cGrpId);

		RedisModUsergrpDevPrivilege(cReq);

        CDeviceInfo cDeviceInfo;
        bHasNtf = (CMS_SUCCESS == DeviceItemQry(cDevId, cDeviceInfo)); // 获取域信息，设备已经不存在时不通知
        CUserGroupDeviceInfo &cUserDevInfo4Ntf = cNtf.GetUserGroupDeviceInfo();
        cUserDevInfo4Ntf.SetGroupID(cGrpId);
        cUserDevInfo4Ntf.SetDeviceID(cDevId);
        cUserDevInfo4Ntf.SetDevDomainID(cDeviceInfo.GetDomainUUID());
        cUserDevInfo4Ntf.SetDevicePrivilege(cDevPrivi);
        cUserDevInfo4Ntf.SetDeviceCapPrivilege(cDevCapPrivis);
    } while (false);

    PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
    if (bHasNtf)
    {
        PostTaskMsgTransfer2CCtrl4LargeNtf(CUserGroupDeviceModNtfReq, GetUserGroupDeviceInfo, GetDeviceCapPrivilege)
    }
}

void CDbOpr::CuserGrpDevQry( const CTaskMsgTransfer<CUserGroupDeviceQryReq> *pcMsg )
{
    DatabaseQueryTemplate<CUserGroupDeviceQryReq, CUserGroupDeviceQryRsp, CUserGroupDeviceQryNtfReq, CUserGroupDeviceInfo, TDeviceCapPrivilege>(
        pcMsg, &CDbOpr::PrepareCuserGrpDevQrySql, &CDbOpr::FetchCuserGrpDevQryRecord, &CUserGroupDeviceQryNtfReq::GetUserGroupDeviceInfo,
        &CUserGroupDeviceInfo::GetDeviceCapPrivilege, &CUserGroupDeviceInfo::GetGroupID, &CUserGroupDeviceInfo::GetDeviceID);
}

ENTasErrno CDbOpr::PrepareCuserGrpDevQrySql( const CUserGroupDeviceQryReq &cReq, TSqlString &tSqlSelect, TSqlString &tSqlCount )
{
    const CUserGroupDeviceInfoQueryCondition &cCond = cReq.GetUserGroupDeviceInfoQueryCondition();

    string strUsergrpId;
    if (cCond.GetCuserGrpID(strUsergrpId) && strUsergrpId.empty()) return ERR_TAS_INVALID_PARM;
    string strDevGrpId;
    if (cCond.GetDevGrpID(strDevGrpId) && strDevGrpId.empty()) return ERR_TAS_INVALID_PARM;
    string strDevId;
    if (cCond.GetDeviceID(strDevId) && strDevId.empty()) return ERR_TAS_INVALID_PARM;
    s32 nDevCapId = -1;
    cCond.GetDeviceCapID(nDevCapId);
    s32 nDevCapIndex = -1;
    cCond.GetDeviceCapIndex(nDevCapIndex);

    tSqlCount.Format("call %s('%s', '%s', '%s', %d, %d)", P_CUSERGRPDEVQRYCOUNT,
        DbStr(strUsergrpId), DbStr(strDevGrpId), DbStr(strDevId), nDevCapId, nDevCapIndex);
    tSqlSelect.Format("call %s('%s', '%s', '%s', %d, %d, %d, %d)", P_CUSERGRPDEVQRY,
        DbStr(strUsergrpId), DbStr(strDevGrpId), DbStr(strDevId), nDevCapId, nDevCapIndex, 
        cReq.GetStartEntryIndex(), GetExpectEntryNum(cReq.GetExpectEntryNum(), TRUE));

    return CMS_SUCCESS;
}

void CDbOpr::FetchCuserGrpDevQryRecord( CUserGroupDeviceInfo &cInfo )
{
    CDevicePrivilege cDevPrivi;
    cInfo.SetGroupID(GetString(F_CUSERGRP2DEVICECAP_CUSERGROUPID));
    cInfo.SetDeviceID(GetString(F_CUSERGRP2DEVICECAP_DEVID));
    cInfo.SetDevDomainID(GetString(F_DEVICE_DOMAINID));
    cDevPrivi.ConvertFromInt(GetInt(F_CUSERGRP2DEV_DEVPRIVILEGE));
    cInfo.SetDevicePrivilege(cDevPrivi);

    TDeviceCapPrivilege cCapPrivi;
    cCapPrivi.id = GetInt(F_CUSER2DEVICECAP_DEVCAPID);
    CBitsContainer cBitsContainer;
    cBitsContainer.ConvertFromInt(GetInt(F_CUSERGRP2DEVICECAP_DEVCAPINDEXPRIVILEGE));
    cCapPrivi.privileges.insert(map<int, CBitsContainer>::value_type(GetInt(F_CUSERGRP2DEVICECAP_DEVCAPINDEX),cBitsContainer));
    cInfo.GetDeviceCapPrivilege().clear();
    cInfo.GetDeviceCapPrivilege().push_back(cCapPrivi);
}

ENTasErrno CDbOpr::GetCUserGrpDeviceInfo( const UUIDString &cUserGrpId, const UUIDString &cDevId, CUserGroupDeviceInfo &cDevInfo )
{
    ENTasErrno enErrno = CMS_SUCCESS;

    do 
    {
        TSqlString tSql;

        string devGrpId;
        s32 devCapId = -1;
        s32 devCapIndex = -1;
        tSql.Format("call %s('%s', '%s', '%s', %d, %d, %d, %d)", P_CUSERGRPDEVQRY,
            DbStr(cUserGrpId), DbStr(devGrpId), DbStr(cDevId), devCapId, devCapIndex, 0, ENTRY_MAX_NUM_ONE_PAGE);
        if (!ExecSql(tSql, enExecMayRet))
        {
            enErrno = ERR_TAS_DBERROR;
            break;
        }

        cDevInfo.GetGroupID().clear();
        CUserGroupDeviceInfo cInfoTmp;
        while (IsNotEof())
        {
            FetchCuserGrpDevQryRecord(cInfoTmp);
            if (!cDevInfo.GetGroupID().empty())
            {
                cDevInfo.GetDeviceCapPrivilege().push_back(cInfoTmp.GetDeviceCapPrivilege().front());
            }
            else
            {
                cDevInfo = cInfoTmp;
            }

            if (!MoveNext())
            {
                enErrno = ERR_TAS_DBERROR;
                break;
            }
        }
    } while (false);

    return enErrno;
}

void CDbOpr::CuserDevGrpVidSrcQry( const CTaskMsgTransfer<CUserDevGrpVidsrcQryReq> *pcMsg )
{
	CTaskMsgTransfer<CUserDevGrpVidsrcQryRsp> cTrans;
	CUserDevGrpVidsrcQryRsp &cRsp = cTrans.GetMsgObj();
	CTaskMsgTransfer<CUserDevGrpVidsrcQryNtfReq> cTransNtf;
	CUserDevGrpVidsrcQryNtfReq &cNtf = cTransNtf.GetMsgObj();
	const CUserDevGrpVidsrcQryReq &cReq = pcMsg->GetMsgObj();
	cRsp.SetQueryTag(cReq.GetQueryTag());
	cNtf.SetQueryTag(cReq.GetQueryTag());
	TSqlString tSql, tSqlCount;
	bool bHasRsped = false;
	do 
	{
		ENTasErrno enErr = PrepareCuserDevGrpVidSrcQrySql(cReq, tSql);
		if (CMS_SUCCESS != enErr)
		{
			cRsp.SetErrorCode(enErr);
			break;
		}

		if (!ExecSql(tSql, enExecMayRet))
		{
			cRsp.SetErrorCode(ERR_TAS_DBERROR);
			break;
		}
		else
		{
			cRsp.SetHasRecords(IsNotEof());
		}

		PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
		if (!cRsp.GetHasRecords())
		{
			return;
		}
		bHasRsped = true;

		u32 dwNtfContentSize = 0;
		CUserDevGrpVidSrcInfo cInfo, cInfoTmp;
		bool bNeedMerage = false;
		
		// 算法： 循环里面一条一条的获取DB记录 --> 组装N条DB记录为一条完整的业务记录(超过消息大小限制时拆分业务记录) --> 组装N条业务记录为一个NTF消息; 循环之外处理最后一条消息
		while (IsNotEof())
		{
			// 获取一条DB记录
			cInfoTmp.GetVidSrcIndex().clear();
			FetchCuserDevGrpVidSrcQryRecord(cInfoTmp);
			if (!MoveNext())
			{
				cRsp.SetErrorCode(ERR_TAS_DBERROR);
				break;
			}
			u32 dwInfoSize = cInfo.ToXml().size();
			u32 dwInfoTmpSize = cInfoTmp.ToXml().size();
			if (cInfo.GetDevId() == cInfoTmp.GetDevId())
			{
				if (dwNtfContentSize + dwInfoSize + dwInfoTmpSize >= 10240)	// 该记录处需要拆包，先将前面的数据发送出去
				{
					cNtf.GetInfos().push_back(cInfo);
					PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cNtf, cTransNtf);
					cNtf.GetInfos().clear();
					dwNtfContentSize = 0;
				}

				cInfo.GetDevCapPri().privileges.insert(cInfoTmp.GetDevCapPri().privileges.begin(), cInfoTmp.GetDevCapPri().privileges.end());
				cInfo.GetVidSrcIndex().insert(cInfo.GetVidSrcIndex().end(), cInfoTmp.GetVidSrcIndex().begin(), cInfoTmp.GetVidSrcIndex().end());
			}
			else				// 发生业务记录切换
			{
				if (!cInfo.GetDevCapPri().privileges.empty())
				{
					dwNtfContentSize += dwInfoSize;
					cNtf.GetInfos().push_back(cInfo);
				}

				cInfo = cInfoTmp;

				if (dwNtfContentSize + dwInfoTmpSize >= SIP_MSG_LEN_MAX)
				{
					PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cNtf, cTransNtf);
					cNtf.GetInfos().clear();
					dwNtfContentSize = 0;
				}
			}            
		}

		if (cRsp.GetErrorCode() == CMS_SUCCESS && (!cInfo.GetDevCapPri().privileges.empty() || !cNtf.GetInfos().empty()))
		{
			cNtf.GetInfos().push_back(cInfo);
			cNtf.SetLastNtf(true);
			PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cNtf, cTransNtf);
			cNtf.GetInfos().clear();
			dwNtfContentSize = 0;
		}

	} while (false);

	if (!bHasRsped)
	{
		PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
	}
}

ENTasErrno CDbOpr::PrepareCuserDevGrpVidSrcQrySql( const CUserDevGrpVidsrcQryReq &cReq, TSqlString &tSqlSelect)
{
	if (cReq.GetDevGrpId().empty() || cReq.GetCuserId().empty())
	{
		return ERR_TAS_INVALID_PARM;
	}

	tSqlSelect.Format("call %s('%s', '%s', '%s','%s',%d)", 
		P_CUSERDEVGRPVIDSRCQRY,
		DbStr(cReq.GetCuserId()), 
		DbStr(cReq.GetDevGrpId()), 
		DbStr(GetTblDevGrp2DevName(cReq.GetDevtreeId())),
		DbStr(GetTblDevGrpName(cReq.GetDevtreeId())),
		cReq.GetRecursive()?1:0);

	return CMS_SUCCESS;
}

void CDbOpr::FetchCuserDevGrpVidSrcQryRecord( CUserDevGrpVidSrcInfo &cInfo)
{
	cInfo.SetDevId(string(GetString(F_CUSER2DEVICECAP_DEVID)));
	cInfo.SetDevDomainID(string(GetString(F_DEVICE_DOMAINID)));

	CDevicePrivilege cDevPri;
	cDevPri.ConvertFromInt(GetInt(F_CUSER2DEV_DEVPRIVILEGE));
	cInfo.SetDevPrivilege(cDevPri);

	TDeviceCapPrivilege tDevCapPri;
	tDevCapPri.id = DEVICE_CAP_VIDEOSOURCE;  //此业务中，能力类型固定为视频源
	CBitsContainer bitCont;
	bitCont.ConvertFromInt(GetInt(F_CUSER2DEVICECAP_DEVCAPINDEXPRIVILEGE));
	s32 nDevCapIndex = GetInt(F_CUSER2DEVICECAP_DEVCAPINDEX);
	cInfo.GetVidSrcIndex().push_back(nDevCapIndex);
	tDevCapPri.privileges.insert(pair<int, CBitsContainer>(nDevCapIndex, bitCont));
	cInfo.SetDevCapPri(tDevCapPri);
}

void CDbOpr::CDevGrpVidsrcNumGet( const CTaskMsgTransfer<CDevGrpVidsrcTotalnumGetReq> *pcMsg )
{
	CTaskMsgTransfer<CDevGrpVidsrcTotalnumGetRsp> cTrans;
	CDevGrpVidsrcTotalnumGetRsp &cRsp = cTrans.GetMsgObj();
	const CDevGrpVidsrcTotalnumGetReq &cReq = pcMsg->GetMsgObj();
	
	TSqlString tSql;
	CDevGrpVidSrcNum cNum;
	u32 dwIndex = 0;
	CUserInfo cUserInfo;
	do 
	{
		if(!GetCuserInfo(cReq.GetUserId(),cUserInfo))
		{
			cRsp.SetErrorCode(ERR_TAS_INVALID_PARM);
			break;
		}

		while(dwIndex < cReq.GetDevGrpIds().size())
		{
			if (cUserInfo.GetUsergrpID().empty())
			{
				tSql.Format("select count(a.devId)  from  tblCuser2DeviceCap a ,tblDevGrp2Dev b \
							where a.devId = b.DevId and a.devCapId = b.devCapId and a.devCapIndex = b.devCapIndex \
							and a.devCapId=1 and a.cuserId='%s' and b.devgroupId='%s'",
							DbStr(cReq.GetUserId()), 
							DbStr(cReq.GetDevGrpIds()[dwIndex]));
			}
			else
			{
				tSql.Format("select count(a.devId)  from  tblCusergrp2DeviceCap a ,tblDevGrp2Dev b  \
				where  a.devId = b.DevId and a.devCapId = b.devCapId and a.devCapIndex = b.devCapIndex \
				and a.devCapId=1 and a.cusergroupId='%s' and b.devgroupId='%s' ",
				DbStr(cUserInfo.GetUsergrpID()),
				DbStr(cReq.GetDevGrpIds()[dwIndex]));
			}

			cNum.SetDevGrpId(cReq.GetDevGrpIds()[dwIndex]);

			if (!ExecSql(tSql, enExecMustRet))
			{
				cRsp.SetErrorCode(ERR_TAS_DBERROR);
				break;
			}
			else
			{
				cNum.SetVidSrcNum(GetIntResult());
			}
			cRsp.GetDevGrpVidsrcNum().push_back(cNum);
			++dwIndex;
		}
	} while (false);

	PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
}

void CDbOpr::DomainRelationQry( const CTaskMsgTransfer<CDomainRelationQryReq> *pcMsg )
{
    DatabaseQueryTemplate<CDomainRelationQryReq, CDomainRelationQryRsp, CDomainRelationQryNtfReq, CDomainRelationInfo>(
        pcMsg, &CDbOpr::PrepareDomainRelationQrySql, &CDbOpr::FetchDomainRelationQryRecord, 
        &CDomainRelationQryNtfReq::GetDomainRelationInfo, &CDomainRelationInfo::GetDomainID);
}

ENTasErrno CDbOpr::PrepareDomainRelationQrySql( const CDomainRelationQryReq &cReq, TSqlString &tSqlSelect, TSqlString &tSqlCount )
{
    TSqlString tSqlCond;
    const CDomainRelationQueryCondition &cCond = cReq.GetCondition();

    AppendQryCondValuePair(cCond.GetDomainID, m_strFieldValue, tSqlCond, F_DOMAINRELATION_DOMAINID, a);
    AppendQryCondValuePair(cCond.GetParentDomainID, m_strFieldValue, tSqlCond, F_DOMAINRELATION_PARENTDOMAINID, a);
    AppendQryCondValuePair(cCond.GetDomainName, m_strFieldValue, tSqlCond, F_DOMAINRELATION_DOMAINNAME, a);
    tSqlCond.EraseLastRedundantAnd();

    tSqlSelect.Format("select a.* from %s a", T_DOMAINRELATION);
    tSqlCount.Format("select count(*) from %s a", T_DOMAINRELATION);

    if (!tSqlCond.Empty())
    {
        tSqlSelect.AppendString(" where ");
        tSqlSelect.AppendString("%s", tSqlCond.GetBuf());

        tSqlCount.AppendString(" where ");
        tSqlCount.AppendString("%s", tSqlCond.GetBuf());
    }

    tSqlSelect.AppendString(" order by a.%s", F_DOMAINRELATION_DOMAINID);
    AddRowNumLimitModifer(tSqlSelect, cReq.GetStartEntryIndex(), cReq.GetExpectEntryNum());
    return CMS_SUCCESS;
}

void CDbOpr::FetchDomainRelationQryRecord( CDomainRelationInfo &cInfo )
{
    cInfo.SetDomainID(GetString(F_DOMAINRELATION_DOMAINID));
    cInfo.SetParentDomainID(GetString(F_DOMAINRELATION_PARENTDOMAINID));
    cInfo.SetDomainName(GetString(F_DOMAINRELATION_DOMAINNAME));
    //mod by liangli 2014.3.21 将domainAlias改为map
    string strDomainAlias = GetString(F_DOMAINRELATION_DOMAINALIAS);
    if ( ! strDomainAlias.empty() )
    {
        cInfo.GetDomainAlias().FromXml(strDomainAlias);
    }
}

void CDbOpr::GetDomainName( const CTaskMsgTransfer<CDomainNameGetReq> *pcMsg )
{
    CTaskMsgTransfer<CDomainNameGetRsp> cTrans;
    CDomainNameGetRsp &cRsp = cTrans.GetMsgObj();

    const CDomainNameGetReq &cReq = pcMsg->GetMsgObj();

    do 
    {
        if (cReq.GetDomainId().empty())
        {
            cRsp.SetDomainId(CurrentDomainId());
            cRsp.SetDomainName(CurrentDomainName());
            break;
        }

        CDomainRelationInfo tInfo;
        ENTasErrno enErr = DomainRelationItemQry(cReq.GetDomainId(), tInfo);
        if (CMS_SUCCESS == enErr)
        {
            cRsp.SetDomainId(tInfo.GetDomainID());
            cRsp.SetDomainName(tInfo.GetDomainName());
        }
        else
        {
            cRsp.SetErrorCode(enErr);
        }
    } while (false);
    
    PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
}

ENTasErrno CDbOpr::DomainRelationItemQry( const string &strDomainId, CDomainRelationInfo &tInfo )
{
    TSqlString tSql;
    tSql.Format("select a.* from %s a where a.%s='%s'", T_DOMAINRELATION,  F_DOMAINRELATION_DOMAINID, DbStr(strDomainId));
    if (!ExecSql(tSql, enExecMayRet))
    {
        return ERR_TAS_DBERROR;
    }
    if (IsEof())
    {
        return ERR_TAS_RECORD_NOT_EXISTS;
    }

    FetchDomainRelationQryRecord(tInfo);

    return CMS_SUCCESS;
}

ENTasErrno CDbOpr::DomainRelationItemDel( const string &strDomainId )
{
    TSqlString tSql;
    tSql.Format("delete from %s where %s='%s'", T_DOMAINRELATION,  F_DOMAINRELATION_DOMAINID, DbStr(strDomainId));
    if (!ExecSql(tSql, enExecNoRet))
    {
        return ERR_TAS_DBERROR;
    }

    return CMS_SUCCESS;
}

ENTasErrno CDbOpr::DomainRelationItemAdd( const CDomainRelationInfo &tInfo )
{
    string strDomainAlias;
    if ( ! tInfo.GetDomainAlias().IsEmpty() )
    {
        strDomainAlias = tInfo.GetDomainAlias().ToXml();
        if ( DOMAIN_ALIAS_DATA_LEN <= strDomainAlias.length() )
        {
            return ERR_TAS_EXTDATA_TOO_LONG;
        }
    }

    TSqlString tSql;
    tSql.Format("insert into %s values('%s', '%s', '%s', '%s')", T_DOMAINRELATION, DbStr(tInfo.GetDomainID()),
        DbStr(tInfo.GetDomainName()), DbStr(tInfo.GetParentDomainID()), DbStr(strDomainAlias));
    if (!ExecSql(tSql, enExecNoRet))
    {
        return ERR_TAS_DBERROR;
    }

    return CMS_SUCCESS;
}

ENTasErrno CDbOpr::DomainRelationItemMod( const CDomainRelationInfo &tInfo )
{
	// 需要先查询出原有扩展信息，然后进行增量的修改
	TSqlString tSelSql;
	tSelSql.Format("select %s from %s where %s='%s' ",
		F_DOMAINRELATION_DOMAINALIAS, T_DOMAINRELATION, 
		F_DOMAINRELATION_DOMAINID, DbStr(tInfo.GetDomainID()));

	if ( !ExecSql(tSelSql, enExecMayRet) )
	{
		return ERR_TAS_DBERROR;
	}

	string strDomainAlias;
	TDomainExtData tExtData;
	if ( IsNotEof() )
	{ 
		string strDomainExtData = GetString(F_DOMAINRELATION_DOMAINALIAS);

		if (!strDomainExtData.empty())
		{
			tExtData.FromXml(strDomainExtData);

			//再将本次修改字段更新到MAP中
			for ( map<TDomainExtDataType, string>::const_iterator pItem = tInfo.GetDomainAlias().GetExtData().begin();
				pItem != tInfo.GetDomainAlias().GetExtData().end(); ++pItem )
			{
				tExtData.GetExtData()[pItem->first] = pItem->second;
			}

			strDomainAlias = tExtData.ToXml();
		}
		else
		{
			strDomainAlias = tInfo.GetDomainAlias().ToXml();
		}
	}
	else
	{
		return ERR_TAS_RECORD_FETCH_FAILED;			// 发生异常，原记录不存在
	}

	if ( DOMAIN_ALIAS_DATA_LEN <= strDomainAlias.length() )
	{
		return ERR_TAS_EXTDATA_TOO_LONG;
	}

    TSqlString tSql;
    tSql.Format("update %s set %s='%s', %s='%s', %s='%s' where %s='%s'", T_DOMAINRELATION, 
        F_DOMAINRELATION_DOMAINNAME, DbStr(tInfo.GetDomainName()), 
        F_DOMAINRELATION_PARENTDOMAINID, DbStr(tInfo.GetParentDomainID()),
		F_DOMAINRELATION_DOMAINALIAS, DbStr(strDomainAlias), 
        F_DOMAINRELATION_DOMAINID, DbStr(tInfo.GetDomainID()));
    if (!ExecSql(tSql, enExecNoRet))
    {
        return ERR_TAS_DBERROR;
    }

    return CMS_SUCCESS;
}

ENTasErrno CDbOpr::PrivateDeviceDel( const UUIDString &devId )
{
    TSqlString tSql;
    tSql.Format("delete from %s where %s='%s'", T_PRIVATEDEVICE, F_PRIVATEDEVICE_PRIVATEDEVICEID, DbStr(devId));
    if (!ExecSql(tSql, enExecNoRet))
    {
        return ERR_TAS_DBERROR;
    }

    return CMS_SUCCESS;
}


ENTasErrno CDbOpr::GetDomainAuthorizationInfo( const string &strDevId, const string &strUser, const string &strUserDomainId, TSyncDataUserDeviceInfo &tInfo )
{
    /*
    SELECT a.*,b.deviceCapId,b.deviceCapIndex,b.deviceCapPrivilege 
    FROM tblDomainAuthorization a inner join tblForeignCuser2DevCapPrivilege b
    on a.innerId=b.innerId
    where a.deviceId='' and a.cuserName='' and a.cuserDomainId='';
    */
    TSqlString tSql;
    tSql.Format("select a.*, b.%s, b.%s, b.%s from %s a inner join %s b \
                on a.innerId=b.innerId where a.%s='%s' and a.%s='%s' and a.%s='%s'",
                F_FOREIGNCUSER2DEVCAPPRIVILEGE_DEVICECAPID, F_FOREIGNCUSER2DEVCAPPRIVILEGE_DEVICECAPINDEX, F_FOREIGNCUSER2DEVCAPPRIVILEGE_DEVICECAPPRIVILEGE,
                T_DOMAINAUTHORIZATION, T_FOREIGNCUSER2DEVCAPPRIVILEGE,
                F_DOMAINAUTHORIZATION_DEVICEID, DbStr(strDevId),
                F_DOMAINAUTHORIZATION_CUSERNAME, DbStr(strUser),
                F_DOMAINAUTHORIZATION_CUSERDOMAINID, DbStr(strUserDomainId));


    if (!ExecSql(tSql, enExecMayRet))
    {
        return ERR_TAS_DBERROR;
    }

    if (IsEof())
    {
        return ERR_TAS_RECORD_NOT_EXISTS;
    }

    tInfo.userName = GetString(F_DOMAINAUTHORIZATION_CUSERNAME);
    tInfo.deviceID = GetString(F_DOMAINAUTHORIZATION_DEVICEID);
    tInfo.devicePrivilege.ConvertFromInt(GetInt(F_DOMAINAUTHORIZATION_DEVPRIVILEGE));

    TDeviceCapPrivilege tCapPrivi;
    tInfo.deviceCapPrivilege.clear();
    u32 dwCapIndex, dwCapIndexPrivi;
    CBitsContainer cBits;
    while (IsNotEof())
    {
        tCapPrivi.id = GetInt(F_FOREIGNCUSER2DEVCAPPRIVILEGE_DEVICECAPID);
        dwCapIndex = GetInt(F_FOREIGNCUSER2DEVCAPPRIVILEGE_DEVICECAPINDEX);
        dwCapIndexPrivi = GetInt(F_FOREIGNCUSER2DEVCAPPRIVILEGE_DEVICECAPPRIVILEGE);
        cBits.ConvertFromInt(dwCapIndexPrivi);
        tCapPrivi.privileges.clear();
        tCapPrivi.privileges.insert(map<int, CBitsContainer>::value_type(dwCapIndex, cBits));

        tInfo.deviceCapPrivilege.push_back(tCapPrivi);

        if (!MoveNext())
        {
            return ERR_TAS_DBERROR;
        }
    }

    return CMS_SUCCESS;
}

ENTasErrno CDbOpr::DomainAuthorizationDel( const UUIDString &devId )
{
    /*delete from tblForeignCuser2DevCapPrivilege
    where innerId in (select innerId from tblDomainAuthorization where deviceId='')*/

    TSqlString tSql;
    tSql.Format("delete from %s where %s in (select %s from %s where %s='%s')", 
        T_FOREIGNCUSER2DEVCAPPRIVILEGE, F_FOREIGNCUSER2DEVCAPPRIVILEGE_INNERID, 
        F_DOMAINAUTHORIZATION_INNERID, T_DOMAINAUTHORIZATION, 
        F_DOMAINAUTHORIZATION_DEVICEID, DbStr(devId));
    if (!ExecSql(tSql, enExecNoRet))
    {
        return ERR_TAS_DBERROR;
    }

    tSql.Format("delete from %s where %s='%s'", T_DOMAINAUTHORIZATION, F_DOMAINAUTHORIZATION_DEVICEID, DbStr(devId));
    if (!ExecSql(tSql, enExecNoRet))
    {
        return ERR_TAS_DBERROR;
    }

    return CMS_SUCCESS;
}

void CDbOpr::Dump()
{
    if (IsConnectedDb() && enDBTypeMysql == GetEngineDBType())
    {
        if (!ExecSql("show variables like '%character_set_%'", enExecMustRet))
        {
            TelePrint("%s", "mysql execute \"show variables like '%character_set_%'\" failed");
            return;
        }

        TelePrint(" 数据库连接的字符集信息=============================================================");
        s8 szBuf[1024];
        while (IsNotEof())
        {
            for (u32 i = 1; i <= m_cDbEngine.GetFieldCount(); ++i)
            {
                m_cDbEngine.GetFieldValue(static_cast<u8>(i), szBuf, sizeof(szBuf));
                szBuf[sizeof(szBuf)-1] = '\0';
                OspPrintf(TRUE, FALSE, " %s", szBuf);
            }
            OspPrintf(TRUE, FALSE, "\n");

            MoveNext();
        }
    }
}

void CDbOpr::PrivateDeviceIdListQry( const CDbMsgTransfer<void> *pcMsg )
{
    CDbMsgTransfer<TPrivateDevIdListGetRsp> cTransfer(*pcMsg, msgPrivateDevIdListGetRsp);
    TPrivateDevIdListGetRsp &cRsp = cTransfer.GetMsgObj();

    TSqlString tSql;
    tSql.Format("select %s from %s", F_PRIVATEDEVICE_PRIVATEDEVICEID, T_PRIVATEDEVICE);
    
    do 
    {
        if (!ExecSql(tSql, enExecMayRet))
        {
            cRsp.SetErrno(ERR_TAS_DBERROR);
            break;
        }

        while (IsNotEof())
        {
            cRsp.devIds.insert(string(GetString(F_PRIVATEDEVICE_PRIVATEDEVICEID)));

            if (!MoveNext())
            {
                cRsp.SetErrno(ERR_TAS_DBERROR);
                break;
            }
        }
    } while (false);

    PostDbMsgTransferFromDbTaskPool(cTransfer);
}

void CDbOpr::DomainRelationListQry( const CDbMsgTransfer<void> *pcMsg )
{
    CDbMsgTransfer<TDomainRelationListGetRsp> cTransfer(*pcMsg, msgDomainRelationListGetRsp);
    TDomainRelationListGetRsp &cRsp = cTransfer.GetMsgObj();

    cRsp.SetErrno(DomainRelationListQry(cRsp.domainRelationList));

    PostDbMsgTransferFromDbTaskPool(cTransfer);
}

ENTasErrno CDbOpr::DomainRelationListQry( vector<CDomainRelationInfo> &tDomains )
{
    TSqlString tSql;
    tSql.Format("select * from %s", T_DOMAINRELATION);

    if (!ExecSql(tSql, enExecMustRet))
    {
        return ERR_TAS_DBERROR;
    }

    tDomains.clear();
    CDomainRelationInfo tInfo;
    while (IsNotEof())
    {
		FetchDomainRelationQryRecord(tInfo);

        tDomains.push_back(tInfo);

        if (!MoveNext())
        {
            return ERR_TAS_DBERROR;
        }
    }

    return CMS_SUCCESS;
}


void CDbOpr::SyncDstDomainListQry( const CDbMsgTransfer<void> *pcMsg )
{
    CDbMsgTransfer<TSyncDstDomainListGetRsp> cTransfer(*pcMsg, msgSyncDstDomainListGetRsp);
    TSyncDstDomainListGetRsp &cRsp = cTransfer.GetMsgObj();

    cRsp.SetErrno(SyncDstDomainItemQry(cRsp.dstDomainList));

    PostDbMsgTransferFromDbTaskPool(cTransfer);
}


ENTasErrno CDbOpr::SyncDstDomainItemQry( vector<TSyncDstDomainInfo> &tSyncDstDomains )
{
    TSqlString tSql;
    tSql.Format("select * from %s", T_SYNCDESTDOMAINS);


    if (!ExecSql(tSql, enExecMayRet))
    {
        return ERR_TAS_DBERROR;
    }

    tSyncDstDomains.clear();
    TSyncDstDomainInfo tSyncDstDomainInfo;
    while(IsNotEof())
    {
        tSyncDstDomainInfo.domainId = GetString(F_SYNCDESTDOMAINS_DOMAINID);
        tSyncDstDomainInfo.domainName = GetString(F_SYNCDESTDOMAINS_DOAMAINNAME);
        tSyncDstDomainInfo.syncType = static_cast<ENSyncType>(GetInt(F_SYNCDESTDOMAINS_SYNCTYPE));

        tSyncDstDomains.push_back(tSyncDstDomainInfo);
        if (!MoveNext())
        {
            return ERR_TAS_DBERROR;
        }
    }

    return CMS_SUCCESS;
}


void CDbOpr::DomainAuthorizationDeviceIdListQry( const CDbMsgTransfer<string> *pcMsg )
{
    CDbMsgTransfer<TDomainAuthorizationDevIdListGetRsp> cTransfer(*pcMsg, msgDomainAuthorizationDevIdListGetRsp);
    TDomainAuthorizationDevIdListGetRsp &cRsp = cTransfer.GetMsgObj();

    TSqlString tSql;
    tSql.Format("select %s from %s where %s='%s'", F_DOMAINAUTHORIZATION_DEVICEID,
        T_DOMAINAUTHORIZATION, F_DOMAINAUTHORIZATION_CUSERDOMAINID, DbStr(pcMsg->GetMsgObj()));

    do 
    {
        if (!ExecSql(tSql, enExecMayRet))
        {
            cRsp.SetErrno(ERR_TAS_DBERROR);
            break;
        }

        cRsp.devIdList.push_back(GetString(F_DOMAINAUTHORIZATION_DEVICEID));
        if (!MoveNext())
        {
            cRsp.SetErrno(ERR_TAS_DBERROR);
            break;
        }
    } while (false);

    PostDbMsgTransferFromDbTaskPool(cTransfer);
}

void CDbOpr::SyncDataRecordAdd( const CDbMsgTransfer<TSyncDataRecordStoreReq> *pcMsg )
{
    CDbMsgTransfer<TSyncDataRecordStoreRsp> cTransfer(*pcMsg, msgSyncDataRecordStoreRsp);
    TSyncDataRecordStoreRsp &cRsp = cTransfer.GetMsgObj();

    const TSyncDataRecordStoreReq &cReq = pcMsg->GetMsgObj();
    TSqlString tSql;
    tSql.Format("insert into %s%s_%d values(NULL, %d, '%s', %d, '%s')", T_SYNCDATARECORDS_PREFIX, 
        DbStr(cReq.dstDomainId), (s32)cReq.enSyncType, 
        cReq.info.dataType, DbStr(cReq.info.dataId),
        cReq.info.operateType, DbStr(cReq.info.dataDomainId));

    if (!ExecSql(tSql, enExecNoRet))
    {
        cRsp.SetErrno(ERR_TAS_DBERROR);
    }

    PostDbMsgTransferFromDbTaskPool(cTransfer);
}

void CDbOpr::SyncDataRecordDel( const CDbMsgTransfer<TSyncDataRecordDelReq> *pcMsg )
{
    CDbMsgTransfer<TSyncDataRecordDelRsp> cTransfer(*pcMsg, msgSyncDataRecordDelRsp);
    TSyncDataRecordDelRsp &cRsp = cTransfer.GetMsgObj();

    const TSyncDataRecordDelReq &cReq = pcMsg->GetMsgObj();
    TSqlString tSql;

    for (u32 i = 0; i < cReq.sns.size(); ++i)
    {
        tSql.Format("delete from %s%s_%d where %s=%d", T_SYNCDATARECORDS_PREFIX, 
            cReq.dstDomainId.c_str(), (s32)cReq.enSyncType, F_SYNCDATARECORDS_SN, cReq.sns[i]);

        if (!ExecSql(tSql, enExecNoRet))
        {
            cRsp.SetErrno(ERR_TAS_DBERROR);
            break;
        }
    }
    

    PostDbMsgTransferFromDbTaskPool(cTransfer);
}

void CDbOpr::SyncDataConstruct( const CDbMsgTransfer<TSyncDataConstructReq> *pcMsg )
{
    CDbMsgTransfer<TSyncDataConstructRsp> cTransfer(*pcMsg, msgSyncDataConstructRsp);
    TSyncDataConstructRsp &cRsp = cTransfer.GetMsgObj();

    const TSyncDataConstructReq &cReq = pcMsg->GetMsgObj();

    do 
    {
        vector<TSyncDataRecordInfo> tSyncRecords;
        bool bRet = SyncDataRecordQry(cReq.dstDomainId, cReq.enSyncType, cReq.dwExpectedDataNum, tSyncRecords);
        if (!bRet)
        {
            cRsp.SetErrno(ERR_TAS_DBERROR);
            break;
        }
        if (tSyncRecords.size() == 0)
        {
            // 没有记录直接返回
            break;
        }

        bRet = SyncDataConstructImp(cReq.dstDomainId, cReq.enSyncType, tSyncRecords, cRsp.results);
        if (!bRet)
        {
            cRsp.SetErrno(ERR_TAS_DBERROR);
            break;
        }
    } while (false);

    PostDbMsgTransferFromDbTaskPool(cTransfer);
}

void CDbOpr::SyncDataStore( const CDbMsgTransfer<TSyncDataStoreReq> *pcMsg )
{
    CDbMsgTransfer<TSyncDataStoreRsp> cTransfer(*pcMsg, msgSyncDataStoreRsp);
    TSyncDataStoreRsp &cRsp = cTransfer.GetMsgObj();

    const TSyncDataStoreReq &cReq = pcMsg->GetMsgObj();

    ENTasErrno enErr;
    TSyncDataResult tResult;
    for (u32 i = 0; i < cReq.data.size(); ++i)
    {
        enErr = SyncDataStoreImp(cReq.enSyncType, cReq.data[i]);

        if (cReq.data[i].dataType == enSyncDataDomainInfo && enErr != CMS_SUCCESS)
        {
            // 域关系数据必须完全成功
            cRsp.SetErrno(enErr);
            break;
        }
        else if (!(CMS_SUCCESS == enErr || ERR_TAS_INVALID_SYNCDATA == enErr))
        {
            cRsp.SetErrno(enErr);
            break;
        }

        tResult.sn = cReq.data[i].sn;
        tResult.result = enErr;
        cRsp.results.push_back(tResult);
    }

    PostDbMsgTransferFromDbTaskPool(cTransfer);
}

void CDbOpr::SyncDomainAuthorizationDependenceQry( const CDbMsgTransfer<string> *pcMsg )
{
    CDbMsgTransfer<TSyncDomainAuthorizationDependenceGetRsp> cTransfer(*pcMsg, msgSyncDomainAuthorizationDependenceGetRsp);
    TSyncDomainAuthorizationDependenceGetRsp &cRsp = cTransfer.GetMsgObj();

    do 
    {
        string strDevId = pcMsg->GetMsgObj();

        ENTasErrno enRet;

        CDeviceInfo cDevInfo;
        enRet = DeviceItemQry(strDevId, cDevInfo);
        if (enRet == ERR_TAS_DBERROR)
        {
            cRsp.SetErrno(ERR_TAS_DBERROR);
            break;
        }
        if (enRet == ERR_TAS_RECORD_NOT_EXISTS)
        {
            cRsp.SetErrno(ERR_TAS_INVALID_SYNCDATA);
            break;
        }

        vector<TDeviceCapIndexs> tDevCapIndexs;
        enRet = GetDeviceCapIndexs(strDevId, tDevCapIndexs);
        if (enRet == ERR_TAS_DBERROR)
        {
            cRsp.SetErrno(ERR_TAS_DBERROR);
            break;
        }

        cRsp.devModelId = cDevInfo.GetDeviceModel();
        cRsp.devId = cDevInfo.GetDeviceUUID();
        cRsp.devCapIndexs = tDevCapIndexs;
    } while (false);
}

void CDbOpr::SyncDevInfoRelateDataQry( const CDbMsgTransfer<string> *pcMsg )
{
    CDbMsgTransfer<TSyncDevInfoRelateDataGetRsp> cTransfer(*pcMsg, msgSyncDevInfoRelateDataGetRsp);
    TSyncDevInfoRelateDataGetRsp &cRsp = cTransfer.GetMsgObj();

    do 
    {
        string strDevId = pcMsg->GetMsgObj();

        ENTasErrno enRet;

        cRsp.devId = strDevId;

        enRet = GetDeviceCapIndexs(strDevId, cRsp.devCapIndexs);
        if (enRet == ERR_TAS_DBERROR)
        {
            cRsp.SetErrno(ERR_TAS_DBERROR);
            break;
        }

        enRet = GetDeviceGroupDeviceInfoKeys(strDevId, cRsp.devGrpDevKeys);
        if (enRet == ERR_TAS_DBERROR)
        {
            cRsp.SetErrno(ERR_TAS_DBERROR);
            break;
        }
        /*下级同步过来的设备，此时可能不处于任何组 (enRet == ERR_TAS_RECORD_NOT_EXISTS)
        {
            cRsp.SetErrno(ERR_TAS_INVALID_SYNCDATA);
            break;
        }*/
    } while (false);

    PostDbMsgTransferFromDbTaskPool(cTransfer);
}

ENTasErrno CDbOpr::GetDeviceCapIndexName( const string &strDevId, s32 nCapId, s32 nCapIndex, TDeviceCapName &tCapIndexName )
{
    ENTasErrno enRet = CMS_SUCCESS;

	tCapIndexName.deviceId = strDevId;
	tCapIndexName.capId = nCapId;

    TSqlString tSql;
    tSql.Format("select * from %s where %s='%s' and %s=%d", 
        T_DEVICECAPINDEXDATA, F_DEVICECAPINDEXDATA_DEVICEID, DbStr(strDevId), 
        F_DEVICECAPINDEXDATA_DEVICECAPID, nCapId);
    if (nCapIndex != -1)
    {
        tSql.AppendString(" and %s=%d", F_DEVICECAPINDEXDATA_DEVICECAPINDEX, nCapIndex);
    }

    if (ExecSql(tSql, enExecMayRet))
    {
        if (IsNotEof())
        {
            TDeviceCapIndexName tIndexName;
            while (IsNotEof())
            {
                tIndexName.index = GetInt(F_DEVICECAPINDEXDATA_DEVICECAPINDEX);
                tIndexName.name = GetString(F_DEVICECAPINDEXDATA_DEVICECAPINDEXNAME);
				string strExtData = GetString(F_DEVICECAPINDEXDATA_DEVICECAPINDEXEXTDATA);
				if (!strExtData.empty())
				{
					tIndexName.extData.FromXml(strExtData);
				}
                tCapIndexName.deviceCapIndexNames.push_back(tIndexName);

                if (!MoveNext())
                {
                    enRet = ERR_TAS_DBERROR;
                    break;
                }
            }
        }
        else
        {
            enRet = ERR_TAS_RECORD_NOT_EXISTS;
        }
    }
    else
    {
        enRet = ERR_TAS_DBERROR;
    }

    return enRet;
}

ENTasErrno CDbOpr::GetDeviceCapIndexs( const string &strDevId, vector<TDeviceCapIndexs> &tCapIndexs )
{
    ENTasErrno enRet = CMS_SUCCESS;

    TSqlString tSql;
    tSql.Format("select %s,%s from %s where %s='%s' order by %s, %s", 
        F_DEVICECAPINDEXDATA_DEVICECAPID, F_DEVICECAPINDEXDATA_DEVICECAPINDEX,
        T_DEVICECAPINDEXDATA, F_DEVICECAPINDEXDATA_DEVICEID, DbStr(strDevId),
        F_DEVICECAPINDEXDATA_DEVICEID, F_DEVICECAPINDEXDATA_DEVICECAPID);

    if (ExecSql(tSql, enExecMayRet))
    {
        if (IsNotEof())
        {
            TDeviceCapIndexs tIndex;
            while (IsNotEof())
            {
                tIndex.id = GetInt(F_DEVICECAPINDEXDATA_DEVICECAPID);
                tIndex.indexs.clear();
                tIndex.indexs.push_back(GetInt(F_DEVICECAPINDEXDATA_DEVICECAPINDEX));
                tCapIndexs.push_back(tIndex);

                if (!MoveNext())
                {
                    enRet = ERR_TAS_DBERROR;
                    break;
                }
            }
        }
        else
        {
            enRet = ERR_TAS_RECORD_NOT_EXISTS;
        }
    }
    else
    {
        enRet = ERR_TAS_DBERROR;
    }

    return enRet;
}

bool CDbOpr::SyncDataRecordQry( const string &strDomainId, ENSyncType enSyncType, u32 dwExpectedNum, vector<TSyncDataRecordInfo> &tRecords )
{
    TSqlString tSql;
    tSql.Format("select * from %s%s_%d order by %s asc", T_SYNCDATARECORDS_PREFIX, 
        DbStr(strDomainId), (s32)enSyncType, F_SYNCDATARECORDS_SN);
    AddRowNumLimitModifer(tSql, 0, dwExpectedNum);

    if (!ExecSql(tSql, enExecMayRet))
    {
        return false;
    }

    tRecords.clear();
    TSyncDataRecordInfo tRecord;
    while(IsNotEof())
    {
        tRecord.dataDomainId = GetString(F_SYNCDATARECORDS_DATADOMAINID);
        tRecord.dataId = GetString(F_SYNCDATARECORDS_DATAID);
        tRecord.dataType = static_cast<ENSyncDataType>(GetInt(F_SYNCDATARECORDS_DATATYPE));
        tRecord.operateType = static_cast<ENSyncOperateType>(GetInt(F_SYNCDATARECORDS_OPERATETYPE));
        tRecord.sn = GetInt(F_SYNCDATARECORDS_SN);

        tRecords.push_back(tRecord);

        if (!MoveNext())
        {
            return false;
        }
    }

    return true;
}

bool CDbOpr::SyncDataConstructImp( const string &strDstDomainId, ENSyncType enSyncType, const vector<TSyncDataRecordInfo> &tRecords, vector<TSyncDataInfo> &tResults )
{
    tResults.clear();

    TSyncDataInfo tDataInfo;
    for (u32 i = 0; i < tRecords.size(); ++i)
    {
        tDataInfo.dataType = tRecords[i].dataType;
        tDataInfo.operateType = tRecords[i].operateType;
        tDataInfo.sn = tRecords[i].sn;
        tDataInfo.dataDomainId = tRecords[i].dataDomainId;

        if (!SyncDataContent(strDstDomainId, enSyncType, tDataInfo.dataType, tDataInfo.operateType, tRecords[i].dataId, tDataInfo.dataContent))
        {
            return false;
        }

        tResults.push_back(tDataInfo);
    }

    return true;
}

bool CDbOpr::SyncDataContent( const string &strDstDomainId, ENSyncType enSyncType, ENSyncDataType dataType, ENSyncOperateType operateType, const string &dataId, string &dataContent )
{
    /*
    enSyncDataDomainInfo,         // 域信息, 数据内容为CDomainRelationInfo序列化内容
    enSyncDataDeviceModel,        // 设备型号信息, 数据内容为CDeviceModelInfo序列化内容
    enSyncDataDeviceInfo,         // 设备信息, 数据内容为CDeviceInfo序列化内容
    enSyncDataDeviceCapIndexName, // 设备能力索引名称, 数据内容为TDeviceCapName序列化内容
    enSyncDataDeviceGroup,        // 设备分组信息, 数据内容为CDeviceGroupInfo序列化内容
    enSyncDataDeviceGroup2Dev,    // 设备分组划归, 数据内容为CDeviceGroupDeviceInfo序列化内容
    enSyncDataUserDevice          // 设备跨域授权, 数据内容为TSyncDataUserDeviceInfo序列化内容
    */
    s8 szBuf[1024];
    s8 szBuf2[1024];
    s8 szBuf3[1024];

    ENTasErrno enErr;
    if (enSyncOperateAdd == operateType || enSyncOperateMod == operateType)
    {
        enErr = CMS_SUCCESS;

        if (enSyncDataDomainInfo == dataType)
        {
            CDomainRelationInfo tData;
            enErr = DomainRelationItemQry(dataId, tData);
            dataContent = tData.ToXml();
        }
        else if (enSyncDataDeviceModel == dataType)
        {
            CDeviceModelInfo tData;
            enErr = DeviceModelItemQry(dataId, tData);

			map<string, TParentSyncRspLoginExternInfo>::const_iterator it = g_cParentSyncExtInfoList.find(strDstDomainId);
			if (g_cParentSyncExtInfoList.end() != it && it->second.platVersion <= PLAT_VERSION_V2R2B1)	// 上级的版本早于或等于v2r2b1
			{
				vector<TDeviceCapInfo> &cCapInfo = tData.GetDeviceCapInfo();
				for (vector<TDeviceCapInfo>::iterator it4Cap = cCapInfo.begin(); cCapInfo.end() != it4Cap; ++it4Cap)
				{
					if (it4Cap->type == DEVICE_CAP_LABEL_CONFDEVICE)		// 视频会议设备能力不能同步上去，否则上级将不识别该能力而报错
					{
						cCapInfo.erase(it4Cap);
						break;
					}
				}

                for (vector<TDeviceCapInfo>::iterator it4Cap = cCapInfo.begin(); cCapInfo.end() != it4Cap; ++it4Cap)
                {
                    if (it4Cap->type == DEVICE_CAP_LABEL_GBRAWSTREAM)		// 国标原始码流不能同步上去，否则上级将不识别该能力而报错
                    {
                        cCapInfo.erase(it4Cap);
                        break;
                    }
                }
			}

            dataContent = tData.ToXml();
        }
        else if (enSyncDataDeviceInfo == dataType)
        {
            CDeviceInfo tData;
            sscanf(dataId.c_str(), "%s", szBuf);
            enErr = DeviceItemQry(szBuf, tData);
            dataContent = tData.ToXml();
        }
        else if (enSyncDataDeviceCapIndexName == dataType)
        {
            TDeviceCapName tData;
            s32 nCapId, nCapIndex;
            sscanf(dataId.c_str(), "%s%d%d", szBuf, &nCapId, &nCapIndex);
            enErr = GetDeviceCapIndexName(szBuf, nCapId, nCapIndex, tData);
            dataContent = tData.ToXml();
        }
        else if (enSyncDataDeviceGroup == dataType)
        {
            CDeviceGroupInfo tData;
            enErr = DevGrpItemQry(dataId, tData);
            dataContent = tData.ToXml();
        }
        else if (enSyncDataDeviceGroup2Dev == dataType)
        {
            CDeviceGroupDeviceInfo tData;
            s32 nCapId, nCapIndex;
            sscanf(dataId.c_str(), "%s%s%d%d", szBuf, szBuf2, &nCapId, &nCapIndex);
            tData.SetDeviceGroupID(szBuf);
            tData.SetDeviceID(szBuf2);
            TDeviceCapIndexs tDevCapIndex;
            tDevCapIndex.id = nCapId;
            tDevCapIndex.indexs.push_back(nCapIndex);
            tData.GetDeviceCapIndexs().push_back(tDevCapIndex);

			CDeviceInfo tDeviceData;
			enErr = DeviceItemQry(tData.GetDeviceID(), tDeviceData);
			tData.SetDomainId(tDeviceData.GetDomainUUID());
            dataContent = tData.ToXml();
        }
		else if (enSyncDataDevGrpDevSeq == dataType)
		{
			CDeviceGroupDevSeqInfo tData;
			s32 nCapIndex;
			string strDevSeq;
			sscanf(dataId.c_str(), "%s%s%d%s", szBuf, szBuf2,&nCapIndex,szBuf3);
			tData.SetDevGrpId(szBuf);
			tData.SetDevId(szBuf2);
			tData.SetVidSrcIdx(nCapIndex);
			tData.SetDevSeq(atof(szBuf3));

			dataContent = tData.ToXml();
		}
        else // enSyncDataUserDevice
        {
            TSyncDataUserDeviceInfo tData;
            sscanf(dataId.c_str(), "%s%s", szBuf, szBuf2);
            enErr = GetDomainAuthorizationInfo(szBuf, szBuf2, strDstDomainId, tData);
            
            dataContent = tData.ToXml();
        }

        // 修正结果
        if (ERR_TAS_RECORD_NOT_EXISTS == enErr)
        {
            dataContent = ""; // 数据置为无效
        }
        else if (CMS_SUCCESS != enErr)
        {
            return false;
        }
    }
    else // enSyncOperateDel
    {
        if (enSyncDataDomainInfo == dataType)
        {
            CDomainRelationInfo tData;
            tData.SetDomainID(dataId);

            dataContent = tData.ToXml();
        }
        else if (enSyncDataDeviceModel == dataType)
        {
            CDeviceModelInfo tData;
            tData.SetDeviceModelId(dataId);
            dataContent = tData.ToXml();
        }
        else if (enSyncDataDeviceInfo == dataType)
        {
            CDeviceInfo tData;
            sscanf(dataId.c_str(), "%s", szBuf);
            tData.SetDeviceUUID(szBuf);
            dataContent = tData.ToXml();
        }
        else if (enSyncDataDeviceCapIndexName == dataType)
        {
            // 不存在删除操作
            CMS_ASSERT(false);
            return false;
        }
        else if (enSyncDataDeviceGroup == dataType)
        {
            CDeviceGroupInfo tData;
            tData.SetDeviceGroupID(dataId);
            dataContent = tData.ToXml();
        }
        else if (enSyncDataDeviceGroup2Dev == dataType)
        {
            CDeviceGroupDeviceInfo tData;
            s32 nCapId, nCapIndex;
            sscanf(dataId.c_str(), "%s%s%d%d", szBuf, szBuf2, &nCapId, &nCapIndex);
            tData.SetDeviceGroupID(szBuf);
            tData.SetDeviceID(szBuf2);
            TDeviceCapIndexs tDevCapIndex;
            tDevCapIndex.id = nCapId;
            tDevCapIndex.indexs.push_back(nCapIndex);
            tData.GetDeviceCapIndexs().push_back(tDevCapIndex);
            dataContent = tData.ToXml();
        }
        else // enSyncDataUserDevice
        {
            TSyncDataUserDeviceInfo tData;
            sscanf(dataId.c_str(), "%s%s%s", szBuf, szBuf2, szBuf3);
            enErr = GetDomainAuthorizationInfo(szBuf, szBuf2, szBuf3, tData);
            dataContent = tData.ToXml();
        }
    }
    
    return true;
}

ENTasErrno CDbOpr::SyncDataStoreImp( ENSyncType enSyncType, const TSyncDataInfo &tSyncData )
{
    if (tSyncData.dataContent.empty())
    {
        return CMS_SUCCESS; // 无效数据内容时，直接返回成功
    }

    ENTasErrno enErrno;
    ENSyncDataType dataType = tSyncData.dataType;

    if (enSyncDataDomainInfo == dataType)
    {
        CDomainRelationInfo tData;
        tData.FromXml(tSyncData.dataContent);

        UpdateDevTreeVer("");

        enErrno = SyncDataStoreDomainInfo(tSyncData.dataDomainId, tData, tSyncData.operateType, enSyncType);
        if (CMS_SUCCESS == enErrno && enSyncTypeParent == enSyncType)
        {
            CSyncAnalyzerAppData &cAppData = g_cSyncAnalyzerApp.GetAppData();
            if (tSyncData.operateType == enSyncOperateAdd)
            {
                cAppData.SubDomainIdAdd(tData);
                SyncAnalyse(dataType, tSyncData.operateType, tSyncData.dataDomainId);
            }
            else if (tSyncData.operateType == enSyncOperateDel)
            {
                SyncAnalyse(dataType, tSyncData.operateType, tSyncData.dataDomainId);
                OspPost(MAKEIID(AID_SyncAnalyze, CInstance::DAEMON), msgSyncMngSubdomainDelNtf, 
                    tSyncData.dataDomainId.c_str(), strlen(tSyncData.dataDomainId.c_str())+1);
            }
            else
            {
                SyncAnalyse(dataType, tSyncData.operateType, tSyncData.dataDomainId);
            }
        }
    }
    else if (enSyncDataDeviceModel == dataType)
    {
        CDeviceModelInfo tData;
        tData.FromXml(tSyncData.dataContent);

        enErrno = SyncDataStoreDeviceModel(tSyncData.dataDomainId, tData, tSyncData.operateType, enSyncType);
        if (CMS_SUCCESS == enErrno && enSyncTypeParent == enSyncType)
        {
            SyncAnalyse(dataType, tSyncData.operateType, tSyncData.dataDomainId, &tData.GetDeviceModelId());
        }
    }
    else if (enSyncDataDeviceInfo == dataType)
    {
        CDeviceInfo tData;
        tData.FromXml(tSyncData.dataContent);

		if (tSyncData.operateType == enSyncOperateDel)
		{
			UpdateDevTreeVer(tData.GetDeviceUUID());
		}
		else
		{
			tData.GetDevData().GetExtData().erase(DEV_EXT_GBID);
		}
		
        enErrno = SyncDataStoreDeviceInfo(tSyncData.dataDomainId, tData, tSyncData.operateType, enSyncType);
        if (CMS_SUCCESS == enErrno && enSyncTypeParent == enSyncType)
        {
			// 新增设备在有了自动更新组之后，初始可能是多个用户的
			if (tSyncData.operateType != enSyncOperateDel)
			{
				UpdateDevTreeVer(tData.GetDeviceUUID());
			}

            // 儿子平台过来的设备数据，当前与之前的私有属性只在设备所在域特殊处理，此处统一设置成非私有
            SyncAnalyse(dataType, tSyncData.operateType, tSyncData.dataDomainId,
                &tData.GetDeviceUUID(), tData.GetDeviceType(), false, false);
        }
    }
    else if (enSyncDataDeviceCapIndexName == dataType)
    {
        TDeviceCapName tData;
        tData.FromXml(tSyncData.dataContent);

        UpdateDevTreeVer(tData.deviceId);

        enErrno = SyncDataStoreDeviceCapIndexName(tSyncData.dataDomainId, tData, tSyncData.operateType, enSyncType);
        if (CMS_SUCCESS == enErrno && enSyncTypeParent == enSyncType)
        {
            for (u32 i = 0; i < tData.deviceCapIndexNames.size(); ++i)
            {
                SyncAnalyse(dataType, tSyncData.operateType, tSyncData.dataDomainId,
                    &tData.deviceId, tData.capId, tData.deviceCapIndexNames[i].index);
            }
        }
    }
    else if (enSyncDataDeviceGroup == dataType)
    {
        CDeviceGroupInfo tData;
        tData.FromXml(tSyncData.dataContent);

        UpdateDevTreeVer("");

        enErrno = SyncDataStoreDeviceGroup(tSyncData.dataDomainId, tData, tSyncData.operateType, enSyncType);
        if (CMS_SUCCESS == enErrno && enSyncTypeParent == enSyncType)
        {
            SyncAnalyse(dataType, tSyncData.operateType, tSyncData.dataDomainId, &tData.GetDeviceGroupID());
        }
    }
    else if (enSyncDataDeviceGroup2Dev == dataType)
    {
        CDeviceGroupDeviceInfo tData;
        tData.FromXml(tSyncData.dataContent);

        UpdateDevTreeVer(tData.GetDeviceID());

        enErrno = SyncDataStoreDeviceGroup2Dev(tSyncData.dataDomainId, tData, tSyncData.operateType, enSyncType);
        if (CMS_SUCCESS == enErrno && enSyncTypeParent == enSyncType)
        {
            for (u32 i = 0; i < tData.GetDeviceCapIndexs().size(); ++i)
            {
                for (u32 j = 0; j < tData.GetDeviceCapIndexs()[i].indexs.size(); ++j)
                {
                    SyncAnalyse(dataType, tSyncData.operateType, tSyncData.dataDomainId,
                        &tData.GetDeviceGroupID(), &tData.GetDeviceID(), 
                        tData.GetDeviceCapIndexs()[i].id, tData.GetDeviceCapIndexs()[i].indexs[j]);
                }
            }
        }
    }
	else if (enSyncDataDevGrpDevSeq == dataType)
	{
		CDeviceGroupDevSeqInfo tData;
		tData.FromXml(tSyncData.dataContent);
		tData.GetDevSeq();
		tData.ToXml();
		enErrno = SyncDataStoreDevGrp2DevSeq(tSyncData.dataDomainId, tData, tSyncData.operateType, enSyncType);
		if (CMS_SUCCESS == enErrno && enSyncTypeParent == enSyncType)
		{
			SyncAnalyse(dataType, tSyncData.operateType, tSyncData.dataDomainId, &tData.GetDevGrpId(),&tData.GetDevId(),
				tData.GetVidSrcIdx(), tData.GetDevSeq());
		}
	}
    else // enSyncDataUserDevice
    {
        TSyncDataUserDeviceInfo tData;
        tData.FromXml(tSyncData.dataContent);

        CUserInfo cUserInfo;
        if (CMS_SUCCESS == GetCuserInfoByName(tData.userName, cUserInfo))
            UpdateCuserDevTreeVer(cUserInfo.GetUserID());

        enErrno = SyncDataStoreUserDevice(tSyncData.dataDomainId, tData, tSyncData.operateType, enSyncType);
    }

    return enErrno;
}


ENTasErrno CDbOpr::SyncDataStoreDomainInfo( const string &strSyncDataDomainId, const CDomainRelationInfo &tSyncData, ENSyncOperateType operateType, ENSyncType enSyncType )
{
    ENTasErrno enErr;

    vector<CDomainRelationInfo> tAllDomains;
    enErr = DomainRelationListQry(tAllDomains);
    if (CMS_SUCCESS != enErr)
    {
        return enErr;
    }

    if (enSyncOperateAdd == operateType)
    {
        CDomainRelationInfo tDbInfo;
        if (CMS_SUCCESS == DomainRelationItemQry(tSyncData.GetDomainID(), tDbInfo))
        {
            if (tDbInfo.GetParentDomainID() != tSyncData.GetParentDomainID())
            {
                return ERR_TAS_INVALID_SYNCDATA;
            }

            if (!ValidateDomainRelationInfo4Mod(tAllDomains, tSyncData))
            {
                return ERR_TAS_INVALID_SYNCDATA; 
            }
            enErr = DomainRelationItemMod(tSyncData);
            if (CMS_SUCCESS != enErr)
            {
                return enErr;
            }

            CDomainRelationModNtfReq cNtfReq;
            cNtfReq.SetDomainRelation(tSyncData);
			RedisDomainRelationMod(tSyncData);
            PostSyncDataChangeNtf2CCtrl(cNtfReq);
            return CMS_SUCCESS;
        }
        else
        {
            if (!ValidateDomainRelationInfo4Add(tAllDomains, tSyncData))
            {
                return ERR_TAS_INVALID_SYNCDATA; 
            }
            enErr = DomainRelationItemAdd(tSyncData);
            if (CMS_SUCCESS != enErr)
            {
                return enErr;
            }

			RedisDomainRelationAdd(tSyncData);
            CDomainRelationAddNtfReq cNtfReq;
            cNtfReq.SetDomainRelation(tSyncData);
            PostSyncDataChangeNtf2CCtrl(cNtfReq);
            
            return CMS_SUCCESS;
        }

        
    }
    else if (enSyncOperateDel == operateType)
    {
        if (!IsDomainIdExists(tAllDomains, tSyncData.GetDomainID()))
        {
            // 不存在直接返回成功
            return CMS_SUCCESS;
        }

        vector<CDomainRelationInfo> tSubDomains;
        FindSubdomainList(tAllDomains, tSyncData.GetDomainID(), tSubDomains);

		CDomainRelationDelNtfReq cNtfReq;

        // 删除时从叶子开始删除
        for (s32 i = (s32)(tSubDomains.size())-1; i >= 0; --i)
        {
            enErr = DeleteAllDataFromCertainDomain(tSubDomains[i].GetDomainID());
            if (CMS_SUCCESS == enErr)
            {
				cNtfReq.SetDomainId(tSubDomains[i].GetDomainID());
				RedisDomainRelationDel(tSubDomains[i]);
				PostSyncDataChangeNtf2CCtrl(cNtfReq);
            }
        }

        enErr = DeleteAllDataFromCertainDomain(tSyncData.GetDomainID());
        if (CMS_SUCCESS == enErr)
        {
			cNtfReq.SetDomainId(tSyncData.GetDomainID());
			PostSyncDataChangeNtf2CCtrl(cNtfReq);

            for (vector<CDomainRelationInfo>::const_iterator it = tAllDomains.begin();
                it != tAllDomains.end(); ++it)
            {
                if (it->GetDomainID() == tSyncData.GetDomainID())
                {
                    RedisDomainRelationDel(*it);
                    break;
                }
            }
        }

        return enErr;
    }
    else // enSyncOperateMod
    {
        if (!ValidateDomainRelationInfo4Mod(tAllDomains, tSyncData))
        {
            return ERR_TAS_INVALID_SYNCDATA; 
        }

        // 数据同步时域拓扑结构不能修改，只允许添加与删除；即使跨域授权也一样
        CDomainRelationInfo tDbInfo;

        if (CMS_SUCCESS == DomainRelationItemQry(tSyncData.GetDomainID(), tDbInfo))
        {
            if (tDbInfo.GetParentDomainID() != tSyncData.GetParentDomainID())
            {
                return ERR_TAS_INVALID_SYNCDATA;
            }

            enErr = DomainRelationItemMod(tSyncData);
            if (CMS_SUCCESS != enErr)
            {
                return enErr;
            }

            CDomainRelationModNtfReq cNtfReq;
            cNtfReq.SetDomainRelation(tSyncData);
			RedisDomainRelationMod(tSyncData);
            PostSyncDataChangeNtf2CCtrl(cNtfReq);
            return CMS_SUCCESS;
        }
        else
        {
            return ERR_TAS_INVALID_SYNCDATA;
        }
    }
}

ENTasErrno CDbOpr::SyncDataStoreDeviceModel( const string &strSyncDataDomainId, const CDeviceModelInfo &tSyncData, ENSyncOperateType operateType, ENSyncType enSyncType )
{
    ENTasErrno enErr;
    if (enSyncOperateAdd == operateType)
    {
        CDeviceModelInfo tDbInfo;
        if (CMS_SUCCESS == DeviceModelItemQry(tSyncData.GetDeviceModelId(), tDbInfo))
        {
            return CMS_SUCCESS;
        }

        enErr = DeviceModelItemAdd(tSyncData, false);
        if (CMS_SUCCESS != enErr)
        {
            return enErr;
        }

        CDeviceModelAddNtfReq cNtfReq;
        cNtfReq.SetDeviceModelInfo(tSyncData);
        PostSyncDataChangeNtf2CCtrl(cNtfReq);
        return CMS_SUCCESS;
    }
    else if (enSyncOperateDel == operateType)
    {
        enErr=IsDeviceModelUsing(tSyncData.GetDeviceModelId());
        if (CMS_SUCCESS != enErr)
        {
            CLASSLOG(ERROR_LEV, "is deleting the used devmodel, modelid=%s enErr=%d.\n", 
                tSyncData.GetDeviceModelId().c_str(), enErr);
            // ERR_TAS_DEVICE_MODEL_USING时改为ERR_TAS_INVALID_SYNCDATA忽略
            return enErr==ERR_TAS_DEVICE_MODEL_USING? ERR_TAS_INVALID_SYNCDATA:enErr;
        }

        enErr = DeviceModelItemDel(tSyncData.GetDeviceModelId());
        if (CMS_SUCCESS != enErr)
        {
            return enErr;
        }

        CDeviceModelDelNtfReq cNtfReq;
        cNtfReq.SetDeviceModelId(tSyncData.GetDeviceModelId());
        PostSyncDataChangeNtf2CCtrl(cNtfReq);
        return CMS_SUCCESS;
    }
    else // enSyncOperateMod
    {
        CMS_ASSERT(false);
        return ERR_TAS_INVALID_PARM;
    }
}

ENTasErrno CDbOpr::SyncDataStoreDeviceCapIndexName( const string &strSyncDataDomainId, const TDeviceCapName &tSyncData, ENSyncOperateType operateType, ENSyncType enSyncType )
{
    if (enSyncOperateMod == operateType)
    {
        ENTasErrno enErr;

        // 设备不存在时直接返回
        CDeviceInfo cDevInfo;
        enErr = DeviceItemQry(tSyncData.deviceId, cDevInfo);
        if (CMS_SUCCESS == enErr)
        {
            // 正常，可以进行后续处理
        }
        else if (ERR_TAS_RECORD_NOT_EXISTS == enErr)
        {
            return ERR_TAS_INVALID_SYNCDATA; // 比如对方多发送过来的数据
        }
        else
        {
            return enErr;
        }

        enErr = DeviceCapIndexNameItemSet(tSyncData);
        if (CMS_SUCCESS != enErr)
        {
            return enErr;
        }

        CDeviceCapIndexNameModNtfReq cNtfReq;
        cNtfReq.SetDeviceCapName(tSyncData);

		//{{补全2个字段domainid+devDomainId, [6/6/2016 liuyongtao]
		if (cNtfReq.GetDeviceCapName().devDomainId.empty())
		{
			cNtfReq.GetDeviceCapName().devDomainId = cDevInfo.GetDomainUUID();
		}
		cNtfReq.SetDomainId(cNtfReq.GetDeviceCapName().devDomainId);
		//}}mod end

		RedisVidSrcAliasOrGpsSet(tSyncData);
        PostSyncDataChangeNtf2CCtrl(cNtfReq);
        return CMS_SUCCESS;
    }
    else 
    {
        CMS_ASSERT(false);
        return ERR_TAS_INVALID_PARM;
    }
}

bool g_bLicenseMngPu2Many = false;
ENTasErrno CDbOpr::SyncDataStoreDeviceInfo( const string &strSyncDataDomainId, const CDeviceInfo &tSyncData, ENSyncOperateType operateType, ENSyncType enSyncType )
{
    ENTasErrno enErr;
    CDeviceInfo tExistDeviceInfo;
    CDeviceModelInfo cModel;

    if (enSyncTypeParent == enSyncType)
    {
        if (enSyncOperateAdd == operateType)
        {
SyncDataStoreDeviceInfo_AddOperation:
            enErr = DeviceItemQry(tSyncData.GetDeviceUUID(), tExistDeviceInfo);
            if (!(CMS_SUCCESS == enErr || ERR_TAS_RECORD_NOT_EXISTS == enErr))
            {
                return enErr;
            }

            if (CMS_SUCCESS == enErr)
            {
                if (tExistDeviceInfo.GetDevicePropertyFlags().HasPropertyFlag(TDevicePropertyFlags::em_PropertyFlags_P2PDev_DISABLE_REASSIGN)
                    || tExistDeviceInfo.GetDevicePropertyFlags().HasPropertyFlag(TDevicePropertyFlags::em_PropertyFlags_P2PDev_ABLE_REASSIGN))
                {
                    enErr = DevGrpDevDelByDevId(tSyncData.GetDeviceUUID());
                    if (CMS_SUCCESS != enErr) return enErr;

                    enErr = CuserGrpDevDel(tSyncData.GetDeviceUUID());
                    if (CMS_SUCCESS != enErr) return enErr;

                    enErr = CuserDevDelByDevId(tSyncData.GetDeviceUUID());
                    if (CMS_SUCCESS != enErr) return enErr;

                    enErr = DeviceCapIndexNameDel(tSyncData.GetDeviceUUID());
                    if (CMS_SUCCESS != enErr) return enErr;

					enErr = PtzLockDelByDevId(tSyncData.GetDeviceUUID());
					if (CMS_SUCCESS != enErr) return enErr;

                    cModel = g_cDbCache.GetDeviceModelInfo(tExistDeviceInfo.GetDeviceModel(), this);
                    enErr = DeviceItemDel(tSyncData.GetDeviceUUID());
                    if (CMS_SUCCESS != enErr) return enErr;
                    DeallocManagePULicense(cModel);
                }
                else
                {
                    return CMS_SUCCESS; // 已经存在直接返回
                }
            }

            cModel = g_cDbCache.GetDeviceModelInfo(tSyncData.GetDeviceModel(), this);
            if (cModel.GetDeviceModelId().empty())
            {
                CLASSLOG(ERROR_LEV, "has no device model of the device(devname=%s, devid=%s), it will not be added.\n",
                    tSyncData.GetDeviceName().c_str(), tSyncData.GetDeviceUUID().c_str());
                return ERR_TAS_INVALID_SYNCDATA;
            }

            enErr = AllocManagePULicense(cModel);
            if (CMS_SUCCESS != enErr)
            {
				if (ERR_TAS_LICENSE_MANAGE_PU_TOO_MANY == enErr)
				{
					g_bLicenseMngPu2Many = true;
					KeyLiscenseTooManyAlarm(true);
				}
                return enErr;
            }
			else if (g_bLicenseMngPu2Many)
			{
				g_bLicenseMngPu2Many = false;
				KeyLiscenseTooManyAlarm(false);
			}

            enErr = DeviceItemAdd(tSyncData);
            if (CMS_SUCCESS != enErr)
            {
                DeallocManagePULicense(cModel);
                return enErr;
            }
            enErr = CuserDevAddDefault(tSyncData.GetDeviceUUID(), tSyncData.GetDeviceModel());
            if (CMS_SUCCESS != enErr)
            {
                return enErr;
            }

            CDeviceAddNtfReq cNtfReq;
            cNtfReq.SetDeviceInfo(tSyncData);
            PostSyncDataChangeNtf2CCtrl(cNtfReq);
            RedisDeviceAdd(tSyncData);
        }
        else if (enSyncOperateDel == operateType)
        {
            enErr = DeviceItemQry(tSyncData.GetDeviceUUID(), tExistDeviceInfo);
            if ((CMS_SUCCESS != enErr) || (ERR_TAS_RECORD_NOT_EXISTS == enErr))
            {
                return CMS_SUCCESS;
            }
            cModel = g_cDbCache.GetDeviceModelInfo(tExistDeviceInfo.GetDeviceModel(), this);

            enErr = DevGrpDevDelByDevId(tSyncData.GetDeviceUUID());
            if (CMS_SUCCESS != enErr) return enErr;

            enErr = CuserGrpDevDel(tSyncData.GetDeviceUUID());
            if (CMS_SUCCESS != enErr) return enErr;

            enErr = CuserDevDelByDevId(tSyncData.GetDeviceUUID());
            if (CMS_SUCCESS != enErr) return enErr;

            enErr = DeviceCapIndexNameDel(tSyncData.GetDeviceUUID());
            if (CMS_SUCCESS != enErr) return enErr;

			enErr = PtzLockDelByDevId(tSyncData.GetDeviceUUID());
			if (CMS_SUCCESS != enErr) return enErr;

            enErr = DeviceItemDel(tSyncData.GetDeviceUUID());
            if (CMS_SUCCESS != enErr) return enErr;
            DeallocManagePULicense(cModel);

            CDeviceDelNtfReq cNtfReq;
            cNtfReq.SetDeviceUUID(tSyncData.GetDeviceUUID());
            cNtfReq.SetDomainId(strSyncDataDomainId);
            PostSyncDataChangeNtf2CCtrl(cNtfReq);
            RedisDeviceDel(tExistDeviceInfo);
        }
        else // enSyncOperateMod
        {
            enErr = DeviceItemQry(tSyncData.GetDeviceUUID(), tExistDeviceInfo);
            if (ERR_TAS_RECORD_NOT_EXISTS == enErr) goto SyncDataStoreDeviceInfo_AddOperation;
            else if (CMS_SUCCESS != enErr) return enErr;

			set<TChangedField> setChangedFields;
            int OldVidNum, NewVidNum;
            NewVidNum = 0;
            OldVidNum = 0;
            if (tExistDeviceInfo.GetDeviceModel() != tSyncData.GetDeviceModel())
            {
				setChangedFields.insert(TCHANGED_FIELD_DEVMODEL);
                CDeviceModelInfo cModelOld = g_cDbCache.GetDeviceModelInfo(tExistDeviceInfo.GetDeviceModel(), this);
                cModel = g_cDbCache.GetDeviceModelInfo(tSyncData.GetDeviceModel(), this);
                if (cModel.GetDeviceModelId().empty())
                {
                    CLASSLOG(ERROR_LEV, "the device(devname=%s,devid=%s)'s new modelid is not existing, it will not be changed.\n",
                        tSyncData.GetDeviceName().c_str(), tSyncData.GetDeviceUUID().c_str());
                    return ERR_TAS_INVALID_SYNCDATA;
                }

                for (vector<TDeviceCapInfo>::const_iterator it = cModel.GetDeviceCapInfo().begin();
                    it != cModel.GetDeviceCapInfo().end(); ++it)
                {
                    if (it->type == DEVICE_CAP_VIDEOSOURCE)
                    {
                        NewVidNum = it->num;
                        break;
                    }
                }
                for (vector<TDeviceCapInfo>::const_iterator it = cModelOld.GetDeviceCapInfo().begin();
                    it != cModelOld.GetDeviceCapInfo().end(); ++it)
                {
                    if (it->type == DEVICE_CAP_VIDEOSOURCE)
                    {
                        OldVidNum = it->num;
                        break;
                    }
                }
                if (NewVidNum > OldVidNum)
                {
                    enErr = AllocManagePULicense(NewVidNum - OldVidNum);
                    if (CMS_SUCCESS != enErr)
					{
						if (ERR_TAS_LICENSE_MANAGE_PU_TOO_MANY == enErr)
						{
							g_bLicenseMngPu2Many = true;
							KeyLiscenseTooManyAlarm(true);
						}
						return enErr;
					}
					else if(g_bLicenseMngPu2Many)
					{
						g_bLicenseMngPu2Many = false;
						KeyLiscenseTooManyAlarm(false);
					}
                }
				
                enErr = DeviceModelPreMod4Parent(tSyncData.GetDeviceUUID(), tSyncData.GetDomainUUID(), tExistDeviceInfo.GetDeviceModel(), tSyncData.GetDeviceModel());
                if (CMS_SUCCESS != enErr) 
                {
                    if (NewVidNum > OldVidNum) DeallocManagePULicense(NewVidNum - OldVidNum);
                    return enErr;
                }
            }
			
			if (tSyncData.GetDeviceName() != tExistDeviceInfo.GetDeviceName())
			{
				setChangedFields.insert(TCHANGED_FIELD_DEVNAME);
			}
			if (tSyncData.GetDeviceAlias() != tExistDeviceInfo.GetDeviceAlias())
			{
				setChangedFields.insert(TCHANGED_FIELD_DEVALIAS);
			}
			if (tSyncData.GetNetType() != tExistDeviceInfo.GetNetType())
			{
				setChangedFields.insert(TCHANGED_FIELD_DEVNETTYPE);
			}
			if (tSyncData.GetNetAddress() != tExistDeviceInfo.GetNetAddress())
			{
				setChangedFields.insert(TCHANGED_FIELD_DEVNETADDRESS);
			}
			if (tSyncData.GetUsedFlag() != tExistDeviceInfo.GetUsedFlag())
			{
				setChangedFields.insert(TCHANGED_FIELD_DEVUSEFLAG);
			}

            enErr = DeviceItemMod(tSyncData);
            if (CMS_SUCCESS != enErr) 
            {
                if (NewVidNum > OldVidNum) DeallocManagePULicense(NewVidNum - OldVidNum);
                return enErr;
            }
            else
            {
                if (OldVidNum > NewVidNum)
                {
                    DeallocManagePULicense(OldVidNum - NewVidNum);
                    DevicePinyinSearchVideoNameDel(tSyncData.GetDeviceUUID(), NewVidNum);
                }
                else if (OldVidNum < NewVidNum)
                {
                    DevicePinyinSearchVideoNameInit(tSyncData.GetDeviceUUID(), NewVidNum, OldVidNum, tExistDeviceInfo.GetDeviceName().c_str());
                }
            }

            CDeviceModNtfReq cNtfReq;
            cNtfReq.SetDeviceInfo(tSyncData);
            PostSyncDataChangeNtf2CCtrl(cNtfReq);
            RedisDeviceMod(tSyncData, setChangedFields);
        }
    }
    else if (enSyncTypeP2PLimit == enSyncType)
    {
        if (enSyncOperateAdd == operateType)
        {
            enErr = DeviceItemQry(tSyncData.GetDeviceUUID(), tExistDeviceInfo);
            if (ERR_TAS_RECORD_NOT_EXISTS != enErr) return enErr;

            cModel = g_cDbCache.GetDeviceModelInfo(tSyncData.GetDeviceModel(), this);
            enErr = AllocManagePULicense(cModel);
            if (CMS_SUCCESS != enErr)
            {
				if (ERR_TAS_LICENSE_MANAGE_PU_TOO_MANY == enErr)
				{
					g_bLicenseMngPu2Many = true;
					KeyLiscenseTooManyAlarm(true);
				}
                return enErr;
            }
			else if(g_bLicenseMngPu2Many)
			{
				g_bLicenseMngPu2Many = false;
				KeyLiscenseTooManyAlarm(false);
			}
			
			
            enErr = DevGrpDevAddDomainAuthorization(tSyncData.GetDeviceUUID(), tSyncData.GetDeviceModel());
            if (CMS_SUCCESS != enErr) return enErr;

            enErr = DeviceItemAdd(tSyncData);
            if (CMS_SUCCESS != enErr) return enErr;

            CDeviceAddNtfReq cNtfReq;
            cNtfReq.SetDeviceInfo(tSyncData);
            PostSyncDataChangeNtf2CCtrl(cNtfReq);
            RedisDeviceAdd(tSyncData);
        }
        else
        {
            return ERR_TAS_INVALID_PARM;
        }
    }
    else // enSyncTypeP2PNoLimit
    {
        return ERR_TAS_INVALID_PARM; // hzytodo2 全联通暂时不实现
    }

    return CMS_SUCCESS;
}

ENTasErrno CDbOpr::SyncDataStoreDeviceGroup( const string &strSyncDataDomainId, const CDeviceGroupInfo &tSyncData, ENSyncOperateType operateType, ENSyncType enSyncType )
{
    ENTasErrno enErr;
    if (enSyncOperateAdd == operateType)
    {
        CDeviceGroupInfo tDevGrpInfo = tSyncData;
        if (tDevGrpInfo.IsAuthorizationDevGrp())
        {
            return CMS_SUCCESS; // 不需要同步此设备组，直接回复成功
        }

        CDeviceGroupInfo cExistDbInfo;
        enErr = DevGrpItemQry(tSyncData.GetDeviceGroupID(), cExistDbInfo);
        if (CMS_SUCCESS == enErr || ERR_TAS_DBERROR == enErr)
        {
            return enErr;
        }
		

		// 如果旧的下级没有fullpath，则在上级计算fullpath [5/10/2016 pangubing]
		string fullpath = "";
		if (!tDevGrpInfo.GetDeviceParentGroupID().empty()&&tDevGrpInfo.GetFullpath().empty())
		{
			string parentFullPath = GetDevGrpFullPath(tDevGrpInfo.GetDeviceParentGroupID());
			fullpath = parentFullPath + tDevGrpInfo.GetDeviceParentGroupID()  + "/";
		}
		else
		{
			// 源域的设备组到上级fullpath需要增加，上级根组 [3/9/2016 pangubing]
			fullpath = CurrentDomainId() + "/" + tDevGrpInfo.GetFullpath();
		}
		tDevGrpInfo.SetFullpath(fullpath);


        /* 源域的根组需要调整它的父组编号 */
        if (tDevGrpInfo.GetDeviceParentGroupID().empty())
        {
            tDevGrpInfo.SetDeviceParentGroupID(CurrentDomainId());
			// 规避多个下级都是序号0 ，导致无法排序 [5/5/2016 pangubing]
			tDevGrpInfo.SetGrpseq(GetNextDevGrpSeq(CurrentDomainId()));
        }


        enErr = DevGrpItemAdd(tDevGrpInfo);
        if (CMS_SUCCESS != enErr)
        {
            return enErr;
        }

		CDeviceGroupInfo cNewDbInfo;
		enErr = DevGrpItemQry(tDevGrpInfo.GetDeviceGroupID(), cNewDbInfo);
		if (CMS_SUCCESS != enErr)
		{
			return enErr;
		}


		// 调整父组的域Id
		tDevGrpInfo.SetParentDomainUUID(cNewDbInfo.GetParentDomainUUID());

		//  如果旧的下级没有序号，则在上级计算[5/6/2016 pangubing]
		if (0==tDevGrpInfo.GetGrpseq())
		{
			CalcDomainGrpSeq(tDevGrpInfo.GetDomainUUID());
		}

        CDeviceGroupAddNtfReq cNtfReq;
        cNtfReq.SetDeviceGroupInfo(tDevGrpInfo);
        PostSyncDataChangeNtf2CCtrl(cNtfReq);
        return CMS_SUCCESS;
    }
    else if (enSyncOperateDel == operateType)
    {
        CDeviceGroupInfo tDevGrpInfo;
        enErr = DevGrpItemQry(tSyncData.GetDeviceGroupID(), tDevGrpInfo);
        if (ERR_TAS_RECORD_NOT_EXISTS == enErr)
        {
            return CMS_SUCCESS;
        }
        if (ERR_TAS_DBERROR == enErr)
        {
            return enErr;
        }
	
		// 递归删除组及所有子组 [3/9/2016 pangubing]	
		vector<UUIDString> vctGroups;
		GetGroupsByGroupId(tDevGrpInfo.GetDeviceGroupID(), vctGroups);

		for (vector<UUIDString>::const_iterator it = vctGroups.begin(); vctGroups.end() != it; ++it)
		{
			enErr = DevGrpDel(*it, tDevGrpInfo.GetDomainUUID());
			if (CMS_SUCCESS != enErr)
			{
				return enErr;
			}
		}

        CDeviceGroupDelNtfReq cNtfReq;
        cNtfReq.GetDeviceGroupInfoKey().SetDeviceGroupID(tSyncData.GetDeviceGroupID());
		cNtfReq.SetDomainId(tDevGrpInfo.GetDomainUUID());
        PostSyncDataChangeNtf2CCtrl(cNtfReq);
        return CMS_SUCCESS;
    }
    else // enSyncOperateMod
    {
        CDeviceGroupInfo tDevGrpInfo = tSyncData;

		// 如果旧的下级没有fullpath，则在上级计算fullpath [5/10/2016 pangubing]
		string fullpath = "";
		if (!tDevGrpInfo.GetDeviceParentGroupID().empty()&&tDevGrpInfo.GetFullpath().empty())
		{
			string parentFullPath = GetDevGrpFullPath(tDevGrpInfo.GetDeviceParentGroupID());
			fullpath = parentFullPath + tDevGrpInfo.GetDeviceParentGroupID()  + "/";
		}
		else
		{
			// 源域的设备组到上级fullpath需要增加，上级根组 [3/9/2016 pangubing]
			fullpath = CurrentDomainId() + "/" + tDevGrpInfo.GetFullpath();
		}
		tDevGrpInfo.SetFullpath(fullpath);
		

        /* 源域的根组需要调整它的父组编号 */
        if (tDevGrpInfo.GetDeviceParentGroupID().empty())
        {
            tDevGrpInfo.SetDeviceParentGroupID(CurrentDomainId());
        }

		CDeviceGroupInfo cExistDbInfo;
		enErr = DevGrpItemQry(tSyncData.GetDeviceGroupID(), cExistDbInfo);
		if (CMS_SUCCESS != enErr)
		{
			return enErr;
		}

		// 源域根组的序号，不同步下级的 [4/1/2016 pangubing]
		if (tDevGrpInfo.GetDeviceParentGroupID()==CurrentDomainId())
		{
			tDevGrpInfo.SetGrpseq(cExistDbInfo.GetGrpseq());
		}


        enErr = DevGrpItemMod(tDevGrpInfo);
        if (CMS_SUCCESS != enErr)
        {
            return enErr;
        }

		//  如果旧的下级没有序号，则在上级计算[5/6/2016 pangubing]
		if (0==tDevGrpInfo.GetGrpseq())
		{
			CalcDomainGrpSeq(tDevGrpInfo.GetDomainUUID());
		}


		// 调整父组的域Id
		tDevGrpInfo.SetParentDomainUUID(cExistDbInfo.GetParentDomainUUID());

        CDeviceGroupModNtfReq cNtfReq;
        cNtfReq.SetDeviceGroupInfo(tDevGrpInfo);
        PostSyncDataChangeNtf2CCtrl(cNtfReq);
        return CMS_SUCCESS;
    }
}

ENTasErrno CDbOpr::SyncDataStoreDeviceGroup2Dev( const string &strSyncDataDomainId, const CDeviceGroupDeviceInfo &tSyncData, ENSyncOperateType operateType, ENSyncType enSyncType )
{
    // 设备组和设备必须同时存在
    ENTasErrno enErr;

    CDeviceGroupInfo cDevGrpInfo;
    enErr = DevGrpItemQry(tSyncData.GetDeviceGroupID(), cDevGrpInfo);
    if (CMS_SUCCESS == enErr)
    {
        // 正常，可以进行后续处理
    }
    else if (ERR_TAS_RECORD_NOT_EXISTS == enErr)
    {
        return ERR_TAS_INVALID_SYNCDATA; // 比如对方多发送过来的数据
    }
    else
    {
        return enErr;
    }

    CDeviceInfo cDevInfo;
    enErr = DeviceItemQry(tSyncData.GetDeviceID(), cDevInfo);
    if (CMS_SUCCESS == enErr)
    {
        // 正常，可以进行后续处理
    }
    else if (ERR_TAS_RECORD_NOT_EXISTS == enErr)
    {
        return ERR_TAS_INVALID_SYNCDATA; // 比如对方多发送过来的数据
    }
    else
    {
        return enErr;
    }

    if (enSyncOperateAdd == operateType)
    {
		// 将设备从源组中移除之前先更新原拥有者的设备树版本信息（否则过后设备属于新的拥有者了，就没有办法更新原拥有者的版本信息了）
		UpdateDevTreeVer(tSyncData.GetDeviceID());

		// 下级向上级同步设备组下的设备时，先将原来组对应的用户和用户组中删除该设备
		AutoRevokeDevice(NULL, CEventReq(), tSyncData.GetDeviceID(), tSyncData.GetDeviceCapIndexs(), string());

        enErr = DevGrpDevItemAdd(tSyncData);
        if (CMS_SUCCESS != enErr)
        {
            return enErr;
        }

		// 下级向上级同步设备组下的设备时，再将新组对应的用户和用户组中添加该设备
		AutoAssignDevice(NULL, CEventReq(), tSyncData.GetDeviceID(), tSyncData.GetDeviceCapIndexs(), tSyncData.GetDeviceGroupID());

		// 这里还要再更新一次新的拥有者的设备树版本信息
		UpdateDevTreeVer(tSyncData.GetDeviceID());

        CDeviceGroupDeviceAddNtfReq cNtfReq;
        cNtfReq.SetDeviceGroupDeviceInfo(tSyncData);
        PostSyncDataChangeNtf2CCtrl(cNtfReq);
        return CMS_SUCCESS;
    }
    else if (enSyncOperateDel == operateType)
    {
		// 将设备从源组中移除之前先更新原拥有者的设备树版本信息（否则过后设备属于新的拥有者了，就没有办法更新原拥有者的版本信息了）
		UpdateDevTreeVer(tSyncData.GetDeviceID());

		AutoRevokeDevice(NULL, CEventReq(), tSyncData.GetDeviceID(), tSyncData.GetDeviceCapIndexs(), tSyncData.GetDeviceGroupID());

        enErr = DevGrpDevItemDel(tSyncData);
        if (CMS_SUCCESS != enErr)
        {
            return enErr;
        }

		// 下级某个删除从组中删除时，将移动到下级的未分组
		AutoAssignDevice(NULL, CEventReq(), tSyncData.GetDeviceID(), tSyncData.GetDeviceCapIndexs(), string());

		// 这里还要再更新一次新的拥有者的设备树版本信息
		UpdateDevTreeVer(tSyncData.GetDeviceID());

        CDeviceGroupDeviceDelNtfReq cNtfReq;
        CDeviceGroupDeviceInfoKey cKey = cNtfReq.GetDeviceGroupDeviceInfoKey();
        cKey.SetDeviceGroupID(tSyncData.GetDeviceGroupID());
        cKey.SetDeviceID(tSyncData.GetDeviceID());
        cKey.SetDeviceCapIndexs(tSyncData.GetDeviceCapIndexs());

		cNtfReq.SetDomainId(tSyncData.GetDomainId());
        PostSyncDataChangeNtf2CCtrl(cNtfReq);
        return CMS_SUCCESS;
    }
    else // enSyncOperateMod
    {
        CMS_ASSERT(false);
        return ERR_TAS_INVALID_PARM;
    }
}

ENTasErrno CDbOpr::SyncDataStoreUserDevice( const string &strSyncDataDomainId, const TSyncDataUserDeviceInfo &tSyncData, ENSyncOperateType operateType, ENSyncType enSyncType )
{
    ENTasErrno enErr;
    CDeviceInfo cDevInfo;
    enErr = DeviceItemQry(tSyncData.deviceID, cDevInfo);
    if (CMS_SUCCESS == enErr)
    {
        if (!cDevInfo.GetDevicePropertyFlags().HasPropertyFlag(TDevicePropertyFlags::em_PropertyFlags_P2PDev_DISABLE_REASSIGN))
        {
            return ERR_TAS_INVALID_SYNCDATA;
        }
    }
    else if (ERR_TAS_RECORD_NOT_EXISTS == enErr)
    {
        return ERR_TAS_INVALID_SYNCDATA;
    }
    else // ERR_TAS_DBERROR
    {
        return enErr;
    }

    CUserInfo cUserInfo;
    enErr = GetCuserInfoByName(tSyncData.userName, cUserInfo);
    if (CMS_SUCCESS == enErr)
    {
        CUserDeviceInfo cUserDevInfo;
        cUserDevInfo.SetUserID(cUserInfo.GetUserID());
        cUserDevInfo.SetDeviceID(tSyncData.deviceID);
        cUserDevInfo.SetDevicePrivilege(tSyncData.devicePrivilege);
        cUserDevInfo.SetDeviceCapPrivilege(tSyncData.deviceCapPrivilege);

        enErr = CuserDevItemAdd(cUserDevInfo);
        if (CMS_SUCCESS != enErr)
        {
            return enErr;
        }

        CUserDeviceAddNtfReq cNtfReq;
        cNtfReq.SetUserDeviceInfo(cUserDevInfo);
        PostSyncDataChangeNtf2CCtrl(cNtfReq);
    }
    else if (ERR_TAS_RECORD_NOT_EXISTS == enErr)
    {
        return ERR_TAS_INVALID_SYNCDATA;
    }
    else
    {
        return enErr;
    }

    return CMS_SUCCESS;
}

ENTasErrno CDbOpr::ParentDomainSyncDataConfigAdd(const string &strParentDomainId, const string &strParentDomainName)
{
    // 添加上级时增加父域同步配置(包括同步目的域配置和初始化同步记录数据)
    ENTasErrno enErr = CMS_SUCCESS;

    do 
    {
        vector<CDomainRelationInfo> tAllDomains, tSubDomains;
        enErr = DomainRelationListQry(tAllDomains);
        if (enErr != CMS_SUCCESS)
        {
            break;
        }
        FindSubdomainList(tAllDomains, CurrentDomainId(), tSubDomains);
        
        string strSyncDomainIds;
        strSyncDomainIds.append(CurrentDomainId());
        for (u32 i = 0; i < tSubDomains.size(); ++i)
        {
            strSyncDomainIds.append(tSubDomains[i].GetDomainID());
        }

        TSqlString tSql;
        tSql.Format("call %s('%s', '%s', '%s')", P_PARENTDOMAINSYNCDATACONFIGADD,
            DbStr(strParentDomainId), DbStr(strParentDomainName), DbStr(strSyncDomainIds));

        if (!ExecSql(tSql, enExecNoRet))
        {
            enErr = ERR_TAS_DBERROR;
        }
    } while (false);

    return enErr;
}

ENTasErrno CDbOpr::ParentDomainSyncDataConfigDel( )
{
    TSqlString tSql;
    tSql.Format("call %s()", P_PARENTDOMAINSYNCDATACONFIGDEL);

    if (ExecSql(tSql, enExecNoRet))
    {
        return CMS_SUCCESS;
    }
    else
    {
        return ERR_TAS_DBERROR;
    }
}

ENTasErrno CDbOpr::DeleteAllDataFromCertainDomain( const string &strDomainId )
{
    int vidNum;
    TSqlString tSqlVidnum;
    tSqlVidnum.Format("select sum(a.capNum) vidNum \
                from tblDeviceModel a inner join tblDevice b \
                on a.modelId=b.devModelId \
                where a.deviceTypeId=1 and a.devCapId=1 and b.domainId='%s'", 
                strDomainId.c_str());
    if (!ExecSql(tSqlVidnum, enExecMustRet))
    {
        return ERR_TAS_DBERROR;
    }
    vidNum = GetInt("vidNum");
    TSqlString tSql;
    tSql.Format("call %s('%s')", P_DELALLDATAFROMCERTAINDOMAIN, DbStr(strDomainId));

    if (ExecSql(tSql, enExecNoRet))
    {
        DeallocManagePULicense(vidNum);
        return CMS_SUCCESS;
    }
    else
    {
        return ERR_TAS_DBERROR;
    }
}


void CDbOpr::DeleteAllDataFromSubDomain( const CDbMsgTransfer<string> *pcMsg )
{
    CDbMsgTransfer<CInnerRspBase> cTransfer(*pcMsg, msgDeleteAllDataFromSubDomainRsp);
    CInnerRspBase &cRsp = cTransfer.GetMsgObj();

    const string &strDomainId = pcMsg->GetMsgObj();

    do 
    {
        vector<CDomainRelationInfo> tAllDomains, tSubDomains;
        ENTasErrno enErr = DomainRelationListQry(tAllDomains);
        if (CMS_SUCCESS != enErr)
        {
            cRsp.SetErrno(enErr);
            break;
        }
        FindSubdomainList(tAllDomains, strDomainId, tSubDomains);

        // 删除时从叶子开始删除
        s32 i;
        for (i = (s32)(tSubDomains.size())-1; i >= 0; --i)
        {
            enErr = DeleteAllDataFromCertainDomain(tSubDomains[i].GetDomainID());
            if (CMS_SUCCESS != enErr)
            {
                cRsp.SetErrno(enErr);
                break;
            }
        }
        if (i >= 0)
        {
            break;
        }

        enErr = DeleteAllDataFromCertainDomain(strDomainId);
        if (CMS_SUCCESS != enErr)
        {
            cRsp.SetErrno(enErr);
            break;
        }
    } while (false);
    
    PostDbMsgTransferFromDbTaskPool(cTransfer);
}


void CDbOpr::TasConfigGet( const CTaskMsgTransfer<CConfigTasGetReq> *pcMsg )
{
    CTaskMsgTransfer<CConfigTasGetRsp> cTrans;
    CConfigTasGetRsp &cRsp = cTrans.GetMsgObj();

    const CConfigTasGetReq &cReq = pcMsg->GetMsgObj();

    do 
    {
        CDomainRelationInfo cDomainRelation;
        cRsp.SetErrorCode(DomainRelationItemQry(CurrentDomainId(), cDomainRelation));
        if (CMS_SUCCESS != cRsp.GetErrorCode())
        {
            break;
        }

        cRsp.SetSelfDomainId(cDomainRelation.GetDomainID());
        cRsp.SetDomainName(cDomainRelation.GetDomainName());
		cRsp.SetDomainAlias(cDomainRelation.GetDomainAlias());

        vector<TSyncDstDomainInfo> tSyncDstDomains;
        cRsp.SetErrorCode(SyncDstDomainItemQry(tSyncDstDomains));
        if (CMS_SUCCESS != cRsp.GetErrorCode())
        {
            break;
        }
        
        for (u32 i = 0; i < tSyncDstDomains.size(); ++i)
        {
            if (tSyncDstDomains[i].syncType == enSyncTypeParent)
            {
                cRsp.SetParentDomainId(tSyncDstDomains[i].domainId);
                cRsp.SetParentDomainName(tSyncDstDomains[i].domainName);
                break;
            }
        }
    } while (false);

    PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
}

void CDbOpr::TasConfigSet( const CTaskMsgTransfer<CConfigTasSetReq> *pcMsg )
{
    CTaskMsgTransfer<CConfigTasSetRsp> cTrans;
    CConfigTasSetRsp &cRsp = cTrans.GetMsgObj();
    CTaskMsgTransfer<CDomainRelationModNtfReq> cTransNtf;

    const CConfigTasSetReq &cReq = pcMsg->GetMsgObj();

    do 
    {
        vector<CDomainRelationInfo> tAllDomains;
        cRsp.SetErrorCode(DomainRelationListQry(tAllDomains));
        if (CMS_SUCCESS != cRsp.GetErrorCode())
        {
            break;
        }

        CDomainRelationInfo cDomainRelation;
        /* 修改本域域名 */
		string strOlddomainName;
        string strNewdomainName;
		TDomainExtData tNewDomainAlias;
        if (cReq.GetDomainName(strNewdomainName) || cReq.GetDomainAlias(tNewDomainAlias))
        {
            cRsp.SetErrorCode(DomainRelationItemQry(CurrentDomainId(), cDomainRelation));
            if (CMS_SUCCESS != cRsp.GetErrorCode())
            {
                break;
            }

			strOlddomainName=cDomainRelation.GetDomainName();

			if (!strNewdomainName.empty())
			{
				cDomainRelation.SetDomainName(strNewdomainName);
			}
            
			if (!tNewDomainAlias.IsEmpty())
			{
				cDomainRelation.SetDomainAlias(tNewDomainAlias);
			}

            if (!ValidateDomainRelationInfo4Mod(tAllDomains, cDomainRelation))
            {
                cRsp.SetErrorCode(ERR_TAS_DOMAIN_RELATION_VALIDATE_FAILED);
                break;
            }

            cRsp.SetErrorCode(DomainRelationItemMod(cDomainRelation));
            if (CMS_SUCCESS != cRsp.GetErrorCode())
            {
                break;
            }

			SyncAnalyse(enSyncDataDomainInfo, enSyncOperateMod, CurrentDomainId());
            RedisDomainRelationMod(cDomainRelation);

			// 如果修改了域名，要修改对应的默认分组的名字
			if (!strNewdomainName.empty())
			{
				CDeviceGroupInfo cDefaultDevGrp;
				cRsp.SetErrorCode(DevGrpItemQry(CurrentDomainId(), cDefaultDevGrp));
				if (CMS_SUCCESS != cRsp.GetErrorCode())
				{
					break;
				}

				// 如果用户已经手动设置过默认组名称，则修改域名不再修改组名 [7/6/2015 pangubing]
				if (strOlddomainName==cDefaultDevGrp.GetDeviceGroupName())
				{
					cDefaultDevGrp.SetDeviceGroupName(strNewdomainName);
					cRsp.SetErrorCode(DevGrpItemMod(cDefaultDevGrp));
					if (CMS_SUCCESS != cRsp.GetErrorCode())
					{
						break;
					}

					SyncAnalyse(enSyncDataDeviceGroup, enSyncOperateMod, CurrentDomainId(), &cDefaultDevGrp.GetDeviceGroupID());
				}
				else
				{
					CLASSLOG(enLogSrcDbTaskPool, EVENT_LEV, "Old domainName[%s] is not same as root group name[%s] \
						do not need change root group name.\n",strOlddomainName.c_str(),cDefaultDevGrp.GetDeviceGroupName().c_str());
				}
			}
        }

        /* 修改父域配置 */
        UUIDString strParentDomainId;
        string strParentDomainName;
        if (cReq.GetParentDomainId(strParentDomainId) && cReq.GetParentDomainName(strParentDomainName))
        {
            cRsp.SetErrorCode(DomainRelationItemQry(CurrentDomainId(), cDomainRelation));
            if (CMS_SUCCESS != cRsp.GetErrorCode())
            {
                break;
            }
            cDomainRelation.SetParentDomainID(strParentDomainId);
            if (!ValidateDomainRelationInfo4Mod(tAllDomains, cDomainRelation))
            {
                cRsp.SetErrorCode(ERR_TAS_DOMAIN_RELATION_VALIDATE_FAILED);
                break;
            }

            cRsp.SetErrorCode(DomainRelationItemMod(cDomainRelation));
            if (CMS_SUCCESS != cRsp.GetErrorCode())
            {
                break;
            }
            RedisDomainRelationMod(cDomainRelation);

            if (!strParentDomainId.empty())
            {
                if (strParentDomainName.empty())
                {
                    cRsp.SetErrorCode(ERR_TAS_INVALID_PARM);
                    break;
                }
                cRsp.SetErrorCode(ParentDomainSyncDataConfigAdd(strParentDomainId, strParentDomainName));
                if (CMS_SUCCESS != cRsp.GetErrorCode())
                {
                    break;
                }
            }
            else
            {
                cRsp.SetErrorCode(ParentDomainSyncDataConfigDel());
                if (CMS_SUCCESS != cRsp.GetErrorCode())
                {
                    break;
                }
            }
        }

        UpdateDevTreeVer("");

        // 域关系变更通知, 一定要先于rsp发出，否则rsp之后投递请求的任务将终止
        if (CMS_SUCCESS == DomainRelationItemQry(CurrentDomainId(), cDomainRelation))
        {
            cTransNtf.GetMsgObj().SetDomainRelation(cDomainRelation);
            PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cTransNtf.GetMsgObj(), cTransNtf);
        }
        else
        {
            CLASSLOG(enLogSrcDbTaskPool, ERROR_LEV, 
                "Query relation item failed, current domain's relation modify notify will not be sent\n");
        }
    } while (false);

    PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
}

void CDbOpr::ChildTasAdd( const CTaskMsgTransfer<CChildtasAddReq> *pcMsg )
{
    CTaskMsgTransfer<CChildtasAddRsp> cTrans;
    CChildtasAddRsp &cRsp = cTrans.GetMsgObj();

    const CChildtasAddReq &cReq = pcMsg->GetMsgObj();

    do 
    {
        if (cReq.GetChildDomainId().empty() || cReq.GetChildDomainName().empty())
        {
            cRsp.SetErrorCode(ERR_PMS_INVALID_PARAM);
            break;
        }

        vector<CDomainRelationInfo> tAllDomains;
        cRsp.SetErrorCode(DomainRelationListQry(tAllDomains));
        if (CMS_SUCCESS != cRsp.GetErrorCode())
        {
            break;
        }

        ENTasErrno enErr;
        CDomainRelationInfo cDomainRelation;
        enErr = DomainRelationItemQry(cReq.GetChildDomainId(), cDomainRelation);
        if (CMS_SUCCESS == enErr)
        {
            if (cDomainRelation.GetParentDomainID() != CurrentDomainId()
                || cDomainRelation.GetDomainID() != cReq.GetChildDomainId()
                || cDomainRelation.GetDomainName() != cReq.GetChildDomainName())
            {
                // 域关系校验冲突，需要先删除该域数据
                cRsp.SetErrorCode(ERR_TAS_INVALID_OPERATION);
                break;
            }

            // 校验数据一致，直接返回成功
            break;
        }
        else if (ERR_TAS_DBERROR == enErr)
        {
            cRsp.SetErrorCode(ERR_TAS_DBERROR);
            break;
        }

        cDomainRelation.SetDomainID(cReq.GetChildDomainId());
        cDomainRelation.SetDomainName(cReq.GetChildDomainName());
        cDomainRelation.SetParentDomainID(CurrentDomainId());

        if (!ValidateDomainRelationInfo4Add(tAllDomains, cDomainRelation))
        {
            cRsp.SetErrorCode(ERR_TAS_DOMAIN_RELATION_VALIDATE_FAILED);
            break;
        }

        enErr = DomainRelationItemAdd(cDomainRelation);
        if (CMS_SUCCESS != enErr)
        {
            cRsp.SetErrorCode(enErr);
            break;
        }

        SyncAnalyse(enSyncDataDomainInfo, enSyncOperateAdd, cReq.GetChildDomainId());

        CTaskMsgTransfer<CDomainRelationAddNtfReq> cTransNtf;
        CDomainRelationAddNtfReq &cNtfReq = cTransNtf.GetMsgObj();
        cNtfReq.GetDomainRelation().SetDomainID(cReq.GetChildDomainId());
        cNtfReq.GetDomainRelation().SetDomainName(cReq.GetChildDomainName());
        cNtfReq.GetDomainRelation().SetParentDomainID(CurrentDomainId());
		cNtfReq.GetDomainRelation().SetDomainAlias(cDomainRelation.GetDomainAlias());

		RedisDomainRelationAdd(cNtfReq.GetDomainRelation());
        PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cNtfReq, cTransNtf);
    } while (false);

    PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
}

void CDbOpr::ChildTasDel( const CTaskMsgTransfer<CChildtasDelReq> *pcMsg )
{
    CTaskMsgTransfer<CChildtasDelRsp> cTrans;
    CChildtasDelRsp &cRsp = cTrans.GetMsgObj();

    const CChildtasDelReq &cReq = pcMsg->GetMsgObj();
	bool bHasRsped = false;
    do 
    {
        ENTasErrno enErr;
        vector<CDomainRelationInfo> cAllDomains, cSubDomains;
        enErr = DomainRelationListQry(cAllDomains);
        if (CMS_SUCCESS != enErr)
        {
            cRsp.SetErrorCode(enErr);
            break;
        }
        if (!IsDomainIdExists(cAllDomains, cReq.GetChildDomainId()))
        {
            // 不存在直接返回成功
            break;
        }


        UpdateDevTreeVer("");

        FindSubdomainList(cAllDomains, cReq.GetChildDomainId(), cSubDomains);

        CTaskMsgTransfer<CDomainRelationDelNtfReq> cTransNtf;
        CDomainRelationDelNtfReq &cNtfReq = cTransNtf.GetMsgObj();
		g_cDbTaskPool.AddTransProgress(cReq.GetChildDomainId(), 100, 5);
		PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
		bHasRsped = true;
        // 删除时从叶子开始删除
        s32 i;
        for (i = (s32)cSubDomains.size()-1; i >= 0; --i)
        {
            enErr = DeleteAllDataFromCertainDomain(cSubDomains[i].GetDomainID());
            if (CMS_SUCCESS != enErr)
            {
                cRsp.SetErrorCode(enErr);
                break;
            }

            cNtfReq.SetDomainId(cSubDomains[i].GetDomainID());
            RedisDomainRelationDel(cSubDomains[i]);
			PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cNtfReq, cTransNtf);
            
        }
        if (i >= 0)
        {
            break;
        }
		g_cDbTaskPool.AddTransProgress(cReq.GetChildDomainId(), 15);
        enErr = DeleteAllDataFromCertainDomain(cReq.GetChildDomainId());
        if (CMS_SUCCESS != enErr)
        {
            cRsp.SetErrorCode(enErr);
            break;
        }
		g_cDbTaskPool.AddTransProgress(cReq.GetChildDomainId(), 35);
        SyncAnalyse(enSyncDataDomainInfo, enSyncOperateDel, cReq.GetChildDomainId());

        cNtfReq.SetDomainId(cReq.GetChildDomainId());
        PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cNtfReq, cTransNtf);
        for (vector<CDomainRelationInfo>::const_iterator it = cAllDomains.begin();
            it != cAllDomains.end(); ++it)
        {
            if (it->GetDomainID() == cReq.GetChildDomainId())
            {
                RedisDomainRelationDel(*it);
                break;
            }
        }
		g_cDbTaskPool.AddTransProgress(cReq.GetChildDomainId(), 100);
    } while (false);
	PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
}

bool CDbOpr::IsExistSameNameCuserGrp( const string &strName, const string *pstrExcludeCuserGrpId /*= NULL*/ )
{
    TSqlString tSql;
    tSql.Format("select count(*) from %s a where a.%s='%s'", T_CUSERGROUP, 
        F_CUSERGROUP_GROUPNAME, DbStr(strName));
    if (NULL != pstrExcludeCuserGrpId)
    {
        tSql.AppendString(" and a.%s!='%s'", F_CUSERGROUP_CUSERGROUPID, DbStr(*pstrExcludeCuserGrpId));
    }

    if (ExecSql(tSql, enExecMustRet))
    {
        return GetIntResult() > 0;
    }
    else
    {
        // 失败时返回存在，好暴露问题
        return true;
    }
}

ENTasErrno CDbOpr::ValidateDecoderTvBinder( const vector<TvDecoderBinder> &cBinder, const UUIDString *ptExcludeTvwallId )
{
    // 自身配置校验
    for (vector<TvDecoderBinder>::const_iterator it = cBinder.begin(); it != cBinder.end(); ++it)
    {
        if (it->decoderId.empty())
        {
            continue;
        }

        for (vector<TvDecoderBinder>::const_iterator it2 = it+1; it2 != cBinder.end(); ++it2)
        {
            if (it->decoderId == it2->decoderId && it->decoderOutputId == it2->decoderOutputId)
            {
                return ERR_TAS_INVALID_PARM;
            }

            if (it->tvId == it2->tvId)
            {
                return ERR_TAS_INVALID_PARM;
            }
        }
    }

    // 全局性解码器绑定重复性判断
    TSqlString tSql;
    for (vector<TvDecoderBinder>::const_iterator it = cBinder.begin(); it != cBinder.end(); ++it)
    {
        if (it->decoderId.empty())
            continue;

        tSql.Format("select count(*) from %s a where a.%s='%s' and a.%s=%d", T_TV2DECODER, 
            F_TV2DECODER_DECODERID, DbStr(it->decoderId), F_TV2DECODER_DECODEROUTPUTID, it->decoderOutputId);
        if (NULL != ptExcludeTvwallId)
        {
            tSql.AppendString(" and a.%s!='%s'", F_TV2DECODER_TVWALLID, DbStr(*ptExcludeTvwallId));
        }

        if (ExecSql(tSql, enExecMustRet))
        {
            if (GetIntResult() > 0)
                return ERR_TAS_DECODER_ALREADY_BINDED;
        }
        else
        {
            return ERR_TAS_DBERROR;
        }
    }

    return CMS_SUCCESS;
}

ENTasErrno CDbOpr::DecoderTvBinderDel( const UUIDString &cDecoderId )
{
    TSqlString tSql;
    tSql.Format("delete from %s where %s='%s'", T_TV2DECODER, 
        F_TV2DECODER_DECODERID, DbStr(cDecoderId));

    if (!ExecSql(tSql, enExecNoRet))
    {
        return ERR_TAS_DBERROR;
    }

    return CMS_SUCCESS;
}

void CDbOpr::QueryLicenseUsage( void )
{
    CInnerLicenseUsageGetRsp cRsp;

    do 
    {
        TLicenseUsageInfo &tUsageInfo = cRsp.m_tLicenseUsageInfo;

        TSqlString tSql;
        /*select a.modelId,a.licenseNum,a.manufacturer, count(b.devId) devNum
        from tblDeviceModel a inner join tblDevice b
        on a.modelId=b.devModelId
        where a.licenseNum>0 and a.domainId='57893db5d2ec487db53f00707b72e976' and a.deviceTypeId=1
        group by a.modelId,a.licenseNum,a.manufacturer*/
        tSql.Format(
            "select a.%s,a.%s,a.%s, count(b.%s) devNum "
            "from %s a inner join %s b "
            "on a.%s=b.%s "
            "where a.%s='%s' and a.%s=1 "
            "group by a.%s,a.%s,a.%s", 
            F_DEVICEMODEL_MODELID, F_DEVICEMODEL_LICENSENUM, F_DEVICEMODEL_MANUFACTURER, F_DEVICE_DEVID,
            T_DEVICEMODEL, T_DEVICE,
            F_DEVICEMODEL_MODELID, F_DEVICE_DEVMODELID,
            F_DEVICEMODEL_DOMAINID, CurrentDomainId().c_str(), F_DEVICEMODEL_DEVICETYPEID,
            F_DEVICEMODEL_MODELID, F_DEVICEMODEL_LICENSENUM, F_DEVICEMODEL_MANUFACTURER);

        if (!ExecSql(tSql, enExecMayRet))
        {
            cRsp.SetErrno(ERR_TAS_DBERROR);
            break;
        }
        
        string strManufacture;
        s32 nLicenseNum, nDevNum;
        while(IsNotEof())
        {
            nLicenseNum = GetInt(F_DEVICEMODEL_LICENSENUM);
            nDevNum = GetInt("devNum");
            strManufacture = GetString(F_DEVICEMODEL_MANUFACTURER);

            tUsageInfo.m_dwPUNum += nLicenseNum*nDevNum;

            if (strManufacture != MANUFACTURE_KEDACOM)
            {
                vector<TOtherPULicenceInfo>::iterator it = tUsageInfo.m_atOtherPULicenseInfos.begin();
                while(it != tUsageInfo.m_atOtherPULicenseInfos.end())
                {
                    if (it->m_strManufacture == strManufacture)
                    {
                        it->m_dwLicenseNum += nLicenseNum*nDevNum;
                        break;
                    }

                    ++it;
                }
                if (it == tUsageInfo.m_atOtherPULicenseInfos.end())
                {
                    tUsageInfo.m_atOtherPULicenseInfos.push_back(
                        TOtherPULicenceInfo(strManufacture, nLicenseNum*nDevNum));
                }
            }

            if (!MoveNext())
            {
                cRsp.SetErrno(ERR_TAS_DBERROR);
                break;
            }
        }
        tSql.Format("%s", "select sum(a.capNum) vidNum \
            from tblDeviceModel a inner join tblDevice b \
            on a.modelId=b.devModelId \
        where a.deviceTypeId=1 and a.devCapId=1");
        if (!ExecSql(tSql, enExecMayRet))
        {
            cRsp.SetErrno(ERR_TAS_DBERROR);
            break;
        }
        tUsageInfo.m_dwManagePUNum = GetInt("vidNum");
    } while (false);
    
    PostMsg2License(msgLicenseUsageGetRsp, cRsp);
}

void CDbOpr::CuserExportDevData( const CTaskMsgTransfer<CFileExportReq> *pcMsg )
{
    CTaskMsgTransfer<CFileExportRsp> cTrans;
    CFileExportRsp &cRsp = cTrans.GetMsgObj();

    const CFileExportReq &cReq = pcMsg->GetMsgObj();
    bool bLocked = false;
    do 
    {
        string strOprUser;
        if (!GetCurrentCuserId(cReq.GetSession(), strOprUser))
        {
            cRsp.SetErrorCode(ERR_TAS_CURRENT_USER_NOT_EXIST);
            break;
        }

        s_pcLock4CuserExportDevData->Lock();
        bLocked = true;

        s32 nCmdRet = 0;
        //nCmdRet = ExecCmd("%s", "[ $(ls /dev/shm/tas/*.csv 2>/dev/null|wc -l) -gt 0 ] && rm -rf /dev/shm/tas/*.csv");
		vector<string> vecFileNameList;
		GetDirFileList("/dev/shm/tas/",vecFileNameList);

		for(size_t i=0;i<vecFileNameList.size();i++)
		{
            bool bNeedDel = false;
            int FileNameLen = vecFileNameList[i].size();
            if ( FileNameLen > 4 && '.' == vecFileNameList[i].at(FileNameLen-4)
                && 'c' == vecFileNameList[i].at(FileNameLen-3)
                && 's' == vecFileNameList[i].at(FileNameLen-2)
                && 'v' == vecFileNameList[i].at(FileNameLen-1))
            {
                bNeedDel = true;
            }
			if (bNeedDel)
			{
                if (!CMSDeleteFile(string("/dev/shm/tas/") + vecFileNameList[i]))
                {
                    nCmdRet=-1;
                    break;
                }
			}
		}

        if (nCmdRet == -1)
        {
            cRsp.SetErrorCode(ERR_TAS_SHELL_CMD_FAILED);
            break;
        }

        TSqlString tSql;
        tSql.Format("call %s('%s')", P_EXPORTCUSERDEVICEDATA, DbStr(strOprUser));
        if (!ExecSql(tSql, enExecNoRet))
        {
            cRsp.SetErrorCode(ERR_TAS_DBERROR);
        }
		
		//nCmdRet = ExecCmd("rm -rf /dev/shm/tas/%s.zip; cd /dev/shm/tas && zip %s.zip *.csv",
		//	cReq.GetSession().c_str(), cReq.GetSession().c_str());

		vecFileNameList.clear();
		GetDirFileList("/dev/shm/tas/",vecFileNameList);
		if (vecFileNameList.empty())
		{
			cRsp.SetErrorCode(ERR_TAS_DBERROR);
			break;
		}
		string strZipFile = cReq.GetSession() + ".zip";
		for(size_t i=0;i<vecFileNameList.size();i++)
		{
			vecFileNameList[i] = string("/dev/shm/tas/") + vecFileNameList[i];
		}

        nCmdRet = ZipCompress(vecFileNameList, string("/dev/shm/tas/") + strZipFile, true);
		
		CLASSLOG(enLogSrcDbTaskPool, EVENT_LEV, "ZipCompress[%s][%d]\n",strZipFile.c_str(), nCmdRet);
        if (nCmdRet != 0)
        {
            cRsp.SetErrorCode(ERR_TAS_SHELL_CMD_FAILED);
            break;
        }
    } while (false);
    
    if (bLocked) s_pcLock4CuserExportDevData->UnLock();
    PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
}

void CDbOpr::VidsrcInfoQry( const CTaskMsgTransfer<CVidsrcInfoQryReq> *pcMsg )
{
	CTaskMsgTransfer<CVidsrcInfoQryRsp> cTrans;
	CVidsrcInfoQryRsp &cRsp = cTrans.GetMsgObj();
	CTaskMsgTransfer<CVidsrcInfoQryNtfReq> cTransNtf;
	CVidsrcInfoQryNtfReq& cNtf = cTransNtf.GetMsgObj();
	const CVidsrcInfoQryReq &cReq = pcMsg->GetMsgObj();
	
	if (cReq.GetDevVidsrcs().empty())
	{
		cRsp.SetErrorCode(ERR_TAS_INVALID_PARM);
		PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
		return;
	}

	vector<CVidsrcInfoQryNtfReq> vecNtf;
	TSqlString tSql, tSqlExtData;
	CDevGrpVidSrcNum cNum;
	u32 dwIndex = 0;
	CDevVidSrcInfo cInfo;
	string strTempAlias, strExtData;

	while(dwIndex < cReq.GetDevVidsrcs().size())
	{
		cInfo.Clear();

		cInfo.SetDevId(cReq.GetDevVidsrcs()[dwIndex].GetDevId());
		cInfo.SetVidsrcIdx(cReq.GetDevVidsrcs()[dwIndex].GetVidSrcIdx());

		if (CMS_SUCCESS != PrepareVidsrcInfoQrySql(cReq.GetDevVidsrcs()[dwIndex].GetDevId(), cReq.GetDevVidsrcs()[dwIndex].GetVidSrcIdx(),
			tSql, tSqlExtData))
		{
			cRsp.SetErrorCode(ERR_TAS_INVALID_PARM);
			break;
		}
		if (!ExecSql(tSql, CDbOpr::enExecMayRet))
		{
			cRsp.SetErrorCode(ERR_TAS_DBERROR);
			break;
		}
		if (IsEof())
		{
			cInfo.SetExist(false);
			cNtf.GetDevVidsrcInfo().push_back(cInfo);
			if (cNtf.ToXml().size()>=10240)
			{
				cNtf.SetEntryNum(cNtf.GetDevVidsrcInfo().size());
				cNtf.SetLastNtf(false);
				vecNtf.push_back(cNtf);
				cNtf.GetDevVidsrcInfo().clear();
			}
			
			++dwIndex;
			continue;
		}

		cInfo.SetDevGrpdomainId(string(GetString(F_DEVGROUP_DOMAINID)));
		cInfo.SetDevGrpId(string(GetString(F_DEVGRP2DEV_DEVGROUPID)));
		cInfo.SetDevGrpName(string(GetString(F_DEVGROUP_GROUPNAME)));
		cInfo.SetExist(true);
		cInfo.SetIpAddress(string(GetString(F_DEVICE_NETADDR)));
		cInfo.SetDevName(string(GetString(F_DEVICE_DEVNAME)));
		

		//别名、经纬度信息
		if (!ExecSql(tSqlExtData, CDbOpr::enExecMayRet))
		{
			cRsp.SetErrorCode(ERR_TAS_DBERROR);
			break;
		}
		if(IsEof())
		{
			cNtf.GetDevVidsrcInfo().push_back(cInfo);
			if (cNtf.ToXml().size()>=10240)
			{
				cNtf.SetEntryNum(cNtf.GetDevVidsrcInfo().size());
				cNtf.SetLastNtf(false);
				vecNtf.push_back(cNtf);
				cNtf.GetDevVidsrcInfo().clear();
			}
			++dwIndex;
			continue;
		}
		
		cInfo.SetVidAlias(string(GetString(F_DEVICECAPINDEXDATA_DEVICECAPINDEXNAME)));
		cInfo.GetExtData().FromXml(string(GetString(F_DEVICECAPINDEXDATA_DEVICECAPINDEXEXTDATA)));
		
		cNtf.GetDevVidsrcInfo().push_back(cInfo);
		
		if (cNtf.ToXml().size() >= 10240)
		{
			cNtf.SetEntryNum(cNtf.GetDevVidsrcInfo().size());
			cNtf.SetLastNtf(false);
			vecNtf.push_back(cNtf);
			cNtf.GetDevVidsrcInfo().clear();
		}

		++dwIndex;
	}

	if (!cNtf.GetDevVidsrcInfo().empty())
	{
		cNtf.SetEntryNum(cNtf.GetDevVidsrcInfo().size());
		vecNtf.push_back(cNtf);
	}

	cRsp.SetTotalEntryNum(vecNtf.empty()?0:cReq.GetDevVidsrcs().size());
	cRsp.SetEntryNum(cRsp.GetTotalEntryNum());
	cRsp.SetErrorCode(CMS_SUCCESS);

	
	for (int i = 0; i < vecNtf.size(); ++i)
	{
		if (i+1 == vecNtf.size())
		{
			vecNtf[i].SetLastNtf(true);
		}
		cNtf = vecNtf[i];
		PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cNtf, cTransNtf);

	}

	PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
}

ENTasErrno CDbOpr::PrepareVidsrcInfoQrySql( const UUIDString &strDevId, const int vidsrcIdx, TSqlString &tSqlSelect, TSqlString &tSqlSelect2 )
{
	if (vidsrcIdx<0)
	{
		return ERR_TAS_INVALID_PARM;
	}

	tSqlSelect.Format("select a.devgroupId,a.devId,a.devCapIndex,b.domainId,b.groupName,c.devName, c.netAddr from tblDevGrp2Dev a, tblDevGroup b,tblDevice c \
	where a.devCapId=1 and a.devCapIndex = %d and a.devId = '%s' and b.devgroupId=a.devgroupId and c.devId = a.devId ",
	vidsrcIdx,
	DbStr(strDevId)
	);


	tSqlSelect2.Format("select deviceCapIndexName,deviceCapIndexExtData from tblDeviceCapIndexData where deviceCapId=1 and deviceId='%s' and deviceCapIndex=%d ",
		DbStr(strDevId),
		vidsrcIdx);

	return CMS_SUCCESS;
}

void CDbOpr::CuserDeviceGroupAdd(const CTaskMsgTransfer<CUserDeviceGroupAddReq> *pcMsg)
{
	CTaskMsgTransfer<CUserDeviceGroupAddRsp> cTrans;
	CUserDeviceGroupAddRsp &cRsp = cTrans.GetMsgObj();

	const CUserDeviceGroupAddReq &cReq = pcMsg->GetMsgObj();
    if (cReq.GetAssignPriv())
    {
        // 校验operId为admin用户id
        string strAdminId;
        if (CMS_SUCCESS != GetAdminUserId(strAdminId))
        {
            cRsp.SetErrorCode(ERR_TAS_DBERROR);
            PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
            return;
        }
        if (strAdminId != cReq.GetOperId())
        {
            cRsp.SetErrorCode(ERR_TAS_OP_MUST_ADMIN);
            PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
            return;
        }
    }

	vector<UUIDString> vctGroups;
	ENTasErrno enRet = GetGroupsByGroupId(cReq.GetDevGrpId(), vctGroups);
	if (CMS_SUCCESS != enRet)			// 获取数据出错
	{
		cRsp.SetErrorCode(enRet);
		PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
	}
	else
	{
		if (vctGroups.empty())			// 没有有需要处理的设备组，直接返回应答
		{
			PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
		}
		else							// 有需要处理的设备组，生成TransTag，以便后续进度查询
		{
			string strTransTag = CUUID();
			g_cDbTaskPool.AddTransProgress(strTransTag, vctGroups.size(), 0);		// 登记初始处理进度

			// 先给客户端回应答，后续可以根据strTransTag来查询事务进度
			cRsp.SetOperUuid(strTransTag);
			PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);

			// 开始循环处理每一个设备组
            int LimitNumOneBatch=0;
			for (u32 i = 0; i < vctGroups.size(); ++i)
			{
                if(++LimitNumOneBatch%20==0) OspTaskDelay(100);

				// 将用户指定设备组内的所有设备划归给指定用户
                if (!cReq.GetAssignPriv())
                {
                    AssignDeviceByDeviceGroup(pcMsg, cReq, vctGroups[i], cReq.GetOperId(), cReq.GetUserId(), 0);
                }
                else
                {
                    AssignDeviceByDeviceGroup(pcMsg, cReq, vctGroups[i], cReq.GetOperId(), cReq.GetUserId(), 0,
                        true, &cReq.GetDevicePrivilege(), &cReq.GetDeviceCapPrivilege());
                }

				// 更新事务处理进度
				if(!g_cDbTaskPool.AddTransProgress(strTransTag, i + 1)) break;
			}

			// 所有的处理完成后更新划归用户对应的版本信息
			UpdateCuserDevTreeVer(cReq.GetUserId());
		}
	}
}

void CDbOpr::CuserDeviceGroupDel(const CTaskMsgTransfer<CUserDeviceGroupDelReq> *pcMsg)
{
	CTaskMsgTransfer<CUserDeviceGroupDelRsp> cTrans;
	CUserDeviceGroupDelRsp &cRsp = cTrans.GetMsgObj();

	const CUserDeviceGroupDelReq &cReq = pcMsg->GetMsgObj();

	// 检查该组是否是自动更新组，是则不允许取消划归
	if (IsAutoAssignDeviceGroup(cReq.GetDevGrpId(), cReq.GetUserId()))
	{
		cRsp.SetErrorCode(ERR_TAS_DEV_GROUP_IS_AUTO_UPDATE);
		PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
		return;
	}

	vector<UUIDString> vctGroups;
	ENTasErrno enRet = GetGroupsByGroupId(cReq.GetDevGrpId(), vctGroups);
	if (CMS_SUCCESS != enRet)			// 获取数据出错
	{
		cRsp.SetErrorCode(enRet);
		PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
	}
	else
	{
		if (vctGroups.empty())			// 没有有需要处理的设备组，直接返回应答
		{
			PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
		}
		else							// 有需要处理的设备组，生成TransTag，以便后续进度查询
		{
			string strTransTag = CUUID();
			g_cDbTaskPool.AddTransProgress(strTransTag, vctGroups.size(), 0);		// 登记初始处理进度

			// 先给客户端回应答，后续可以根据strTransTag来查询事务进度
			cRsp.SetOperUuid(strTransTag);
			PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);

			// 开始循环处理每一个设备组
			for (u32 i = 0; i < vctGroups.size(); ++i)
			{
				// 当改组不是自动更新组时，才取消划归
				if (!IsAutoAssignDeviceGroup(vctGroups[i], cReq.GetUserId()))
				{
					// 将用户指定设备组内的所有设备划归给指定用户
					RevokeDeviceByDeviceGroup(pcMsg, cReq, vctGroups[i], cReq.GetOperId(), cReq.GetUserId());
				}

				// 更新事务处理进度
				if(!g_cDbTaskPool.AddTransProgress(strTransTag, i + 1)) break;
			}

			// 所有的处理完成后更新划归用户对应的版本信息
			UpdateCuserDevTreeVer(cReq.GetUserId());
		}
	}
}

void CDbOpr::CuserGroupDeviceGroupAdd(const CTaskMsgTransfer<CUserGroupDeviceGroupAddReq> *pcMsg)
{
	CTaskMsgTransfer<CUserGroupDeviceGroupAddRsp> cTrans;
	CUserGroupDeviceGroupAddRsp &cRsp = cTrans.GetMsgObj();

	const CUserGroupDeviceGroupAddReq &cReq = pcMsg->GetMsgObj();

    if (cReq.GetAssignPriv())
    {
        // 校验operId为admin用户id
        string strAdminId;
        if (CMS_SUCCESS != GetAdminUserId(strAdminId))
        {
            cRsp.SetErrorCode(ERR_TAS_DBERROR);
            PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
            return;
        }
        if (strAdminId != cReq.GetOperId())
        {
            cRsp.SetErrorCode(ERR_TAS_OP_MUST_ADMIN);
            PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
            return;
        }
    }

	vector<UUIDString> vctGroups;
	ENTasErrno enRet = GetGroupsByGroupId(cReq.GetDevGrpId(), vctGroups);
	if (CMS_SUCCESS != enRet)			// 获取数据出错
	{
		cRsp.SetErrorCode(enRet);
		PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
	}
	else
	{
		if (vctGroups.empty())			// 没有有需要处理的设备组，直接返回应答
		{
			PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
		}
		else							// 有需要处理的设备组，生成TransTag，以便后续进度查询
		{
			string strTransTag = CUUID();
			g_cDbTaskPool.AddTransProgress(strTransTag, vctGroups.size(), 0);		// 登记初始处理进度

			// 先给客户端回应答，后续可以根据strTransTag来查询事务进度
			cRsp.SetOperUuid(strTransTag);
			PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);

			// 开始循环处理每一个设备组
            int LimitNumOneBatch=0;
			for (u32 i = 0; i < vctGroups.size(); ++i)
			{
                if(++LimitNumOneBatch%20==0) OspTaskDelay(100);

				// 将用户指定设备组内的所有设备划归给指定用户组
                if (!cReq.GetAssignPriv())
                {
                    AssignDeviceByDeviceGroup(pcMsg, cReq, vctGroups[i], cReq.GetOperId(), cReq.GetUserGroupId(), 1);
                }
                else
                {
                    AssignDeviceByDeviceGroup(pcMsg, cReq, vctGroups[i], cReq.GetOperId(), cReq.GetUserGroupId(), 1,
                        true, &cReq.GetDevicePrivilege(), &cReq.GetDeviceCapPrivilege());
                }

				// 更新事务处理进度
				if(!g_cDbTaskPool.AddTransProgress(strTransTag, i + 1)) break;
			}

			// 所有的处理完成后更新划归用户组对应的版本信息
			UpdateCuserGrpDevTreeVer(cReq.GetUserGroupId());
		}
	}
}

void CDbOpr::CuserGroupDeviceGroupDel(const CTaskMsgTransfer<CUserGroupDeviceGroupDelReq> *pcMsg)
{
	CTaskMsgTransfer<CUserGroupDeviceGroupDelRsp> cTrans;
	CUserGroupDeviceGroupDelRsp &cRsp = cTrans.GetMsgObj();

	const CUserGroupDeviceGroupDelReq &cReq = pcMsg->GetMsgObj();

	// 检查该组是否是自动更新组，是则不允许取消划归
	if (IsAutoAssignDeviceGroup(cReq.GetDevGrpId(), cReq.GetUserGroupId(), 1))
	{
		cRsp.SetErrorCode(ERR_TAS_DEV_GROUP_IS_AUTO_UPDATE);
		PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
		return;
	}

	vector<UUIDString> vctGroups;
	ENTasErrno enRet = GetGroupsByGroupId(cReq.GetDevGrpId(), vctGroups);
	if (CMS_SUCCESS != enRet)			// 获取数据出错
	{
		cRsp.SetErrorCode(enRet);
		PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
	}
	else
	{
		if (vctGroups.empty())			// 没有有需要处理的设备组，直接返回应答
		{
			PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
		}
		else							// 有需要处理的设备组，生成TransTag，以便后续进度查询
		{
			string strTransTag = CUUID();
			g_cDbTaskPool.AddTransProgress(strTransTag, vctGroups.size(), 0);		// 登记初始处理进度

			// 先给客户端回应答，后续可以根据strTransTag来查询事务进度
			cRsp.SetOperUuid(strTransTag);
			PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);

			// 开始循环处理每一个设备组
			for (u32 i = 0; i < vctGroups.size(); ++i)
			{
				// 当改组不是自动更新组时，才取消划归
				if (!IsAutoAssignDeviceGroup(vctGroups[i], cReq.GetUserGroupId(), 1))
				{
					// 将用户指定设备组内的所有设备划归给指定用户
					RevokeDeviceByDeviceGroup(pcMsg, cReq, vctGroups[i], cReq.GetOperId(), cReq.GetUserGroupId(), 1);
				}

				// 更新事务处理进度
				if(!g_cDbTaskPool.AddTransProgress(strTransTag, i + 1)) break;
			}

			// 所有的处理完成后更新划归用户组对应的版本信息
			UpdateCuserGrpDevTreeVer(cReq.GetUserGroupId());
		}
	}
}

void CDbOpr::CDeviceAutoAssignToUserSet(const CTaskMsgTransfer<CDeviceAutoAssignToUserSetReq> *pcMsg)
{
	CTaskMsgTransfer<CDeviceAutoAssignToUserSetRsp> cTrans;
	CDeviceAutoAssignToUserSetRsp &cRsp = cTrans.GetMsgObj();

	const CDeviceAutoAssignToUserSetReq &cReq = pcMsg->GetMsgObj();

	// 校验operId为admin用户id，否则返回错误
	string strAdminId;
	if (CMS_SUCCESS != GetAdminUserId(strAdminId))
	{
		cRsp.SetErrorCode(ERR_TAS_DBERROR);
		PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
		return;
	}

	if (strAdminId != cReq.GetOperId())
	{
		cRsp.SetErrorCode(ERR_TAS_OP_MUST_ADMIN);
		PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
		return;
	}

	if (!cReq.GetAutoAssign())
	{
		// 非自动同步组，禁止取消自动同步
		if (!IsAutoAssignDeviceGroup(cReq.GetDevGrpId(), cReq.GetUserId()))
		{
			cRsp.SetErrorCode(ERR_TAS_GROUP_IS_NOT_AUTO_UPDATE);
			PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
			return;
		}

		// 取消自动更新，要求改组为最顶层的自动更新组
		if (!IsTopAutoAssignDeviceGroup(cReq.GetDevGrpId(), cReq.GetUserId()))
		{
			cRsp.SetErrorCode(ERR_TAS_GROUP_MUST_TOP_AUTO_UPDATE);
			PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
			return;
		}
	}

	vector<UUIDString> vctGroups;
	ENTasErrno enRet = GetGroupsByGroupId(cReq.GetDevGrpId(), vctGroups);
	if (CMS_SUCCESS != enRet)			// 获取数据出错
	{
		cRsp.SetErrorCode(enRet);
		PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
	}
	else
	{
		if (vctGroups.empty())			// 没有有需要处理的设备组，直接返回应答
		{
			PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
		}
		else							// 有需要处理的设备组，生成TransTag，以便后续进度查询
		{
			string strTransTag = CUUID();
			g_cDbTaskPool.AddTransProgress(strTransTag, vctGroups.size(), 0);		// 登记初始处理进度

			// 先给客户端回应答，后续可以根据strTransTag来查询事务进度
			cRsp.SetOperUuid(strTransTag);
			PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);

			// 设置自动更新，这里要注意，可能某些下级组已经设置为自动更新了，这里不能跳过处理，因为可能权限不同了
            int LimitNumOneBatch=0;
			if (cReq.GetAutoAssign())
			{
				// 开始循环处理每一个设备组
				for (u32 i = 0; i < vctGroups.size(); ++i)
				{
                    if(++LimitNumOneBatch%20==0) OspTaskDelay(100);

					RegistAutoAssignDeviceGroup(vctGroups[i], cReq.GetUserId());

					// 将用户指定设备组内的所有设备划归给指定用户
					AssignDeviceByDeviceGroup(pcMsg, cReq, vctGroups[i], cReq.GetOperId(), cReq.GetUserId(), 0);

					// 更新事务处理进度
					g_cDbTaskPool.AddTransProgress(strTransTag, i + 1);
				}
			}
			else						// 取消自动更新
			{
				// 开始循环处理每一个设备组
				for (u32 i = 0; i < vctGroups.size(); ++i)
				{
                    if(++LimitNumOneBatch%20==0) OspTaskDelay(100);

					UnRegistAutoAssignDeviceGroup(vctGroups[i], cReq.GetUserId());

					// 和SE讨论后，觉得取消自动同步后，原先划归的设备还是保留
					// RevokeDeviceByDeviceGroup(*pcMsg, cReq, vctGroups[i], cReq.GetOperId(), cReq.GetUserId());

					// 更新事务处理进度
					g_cDbTaskPool.AddTransProgress(strTransTag, i + 1);
				}
			}

			// 所有的处理完成后更新划归用户对应的版本信息
			UpdateCuserDevTreeVer(cReq.GetUserId());
		}
	}
}

void CDbOpr::CDeviceAutoAssignToUserGroupSet(const CTaskMsgTransfer<CDeviceAutoAssignToUserGroupSetReq> *pcMsg)
{
	CTaskMsgTransfer<CDeviceAutoAssignToUserGroupSetRsp> cTrans;
	CDeviceAutoAssignToUserGroupSetRsp &cRsp = cTrans.GetMsgObj();

	const CDeviceAutoAssignToUserGroupSetReq &cReq = pcMsg->GetMsgObj();

	// 校验operId为admin用户id，否则返回错误
	string strAdminId;
	if (CMS_SUCCESS != GetAdminUserId(strAdminId))
	{
		cRsp.SetErrorCode(ERR_TAS_DBERROR);
		PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
		return;
	}

	if (strAdminId != cReq.GetOperId())
	{
		cRsp.SetErrorCode(ERR_TAS_OP_MUST_ADMIN);
		PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
		return;
	}

	if (!cReq.GetAutoAssign())
	{
		// 非自动同步组，禁止取消自动同步
		if (!IsAutoAssignDeviceGroup(cReq.GetDevGrpId(), cReq.GetUserGroupId(), 1))
		{
			cRsp.SetErrorCode(ERR_TAS_GROUP_IS_NOT_AUTO_UPDATE);
			PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
			return;
		}

		// 取消自动更新，要求改组为最顶层的自动更新组
		if (!IsTopAutoAssignDeviceGroup(cReq.GetDevGrpId(), cReq.GetUserGroupId(), 1))
		{
			cRsp.SetErrorCode(ERR_TAS_GROUP_MUST_TOP_AUTO_UPDATE);
			PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
			return;
		}
	}

	vector<UUIDString> vctGroups;
	ENTasErrno enRet = GetGroupsByGroupId(cReq.GetDevGrpId(), vctGroups);
	if (CMS_SUCCESS != enRet)			// 获取数据出错
	{
		cRsp.SetErrorCode(enRet);
		PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
	}
	else
	{
		if (vctGroups.empty())			// 没有有需要处理的设备组，直接返回应答
		{
			PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
		}
		else							// 有需要处理的设备组，生成TransTag，以便后续进度查询
		{
			string strTransTag = CUUID();
			g_cDbTaskPool.AddTransProgress(strTransTag, vctGroups.size(), 0);		// 登记初始处理进度

			// 先给客户端回应答，后续可以根据strTransTag来查询事务进度
			cRsp.SetOperUuid(strTransTag);
			PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);

			// 设置自动更新，这里要注意，可能某些下级组已经设置为自动更新了，这里不能跳过处理，因为可能权限不同了
            int LimitNumOneBatch=0;
			if (cReq.GetAutoAssign())
			{
				// 开始循环处理每一个设备组
				for (u32 i = 0; i < vctGroups.size(); ++i)
				{
                    if(++LimitNumOneBatch%20==0) OspTaskDelay(100);

					RegistAutoAssignDeviceGroup(vctGroups[i], cReq.GetUserGroupId(), 1);

					// 将用户指定设备组内的所有设备划归给指定用户
					AssignDeviceByDeviceGroup(pcMsg, cReq, vctGroups[i], cReq.GetOperId(), cReq.GetUserGroupId(), 1);

					// 更新事务处理进度
					g_cDbTaskPool.AddTransProgress(strTransTag, i + 1);
				}
			}
			else						// 取消自动更新
			{
				// 开始循环处理每一个设备组
				for (u32 i = 0; i < vctGroups.size(); ++i)
				{
                    if(++LimitNumOneBatch%20==0) OspTaskDelay(100);

					UnRegistAutoAssignDeviceGroup(vctGroups[i], cReq.GetUserGroupId(), 1);

					// 和SE讨论后，觉得取消自动同步后，原先划归的设备还是保留
					// RevokeDeviceByDeviceGroup(*pcMsg, cReq, vctGroups[i], cReq.GetOperId(), cReq.GetUserGroupId(), 1);

					// 更新事务处理进度
					g_cDbTaskPool.AddTransProgress(strTransTag, i + 1);
				}
			}

			// 所有的处理完成后更新划归用户组对应的版本信息
			UpdateCuserGrpDevTreeVer(cReq.GetUserGroupId());
		}
	}
}

void CDbOpr::CDeviceAutoAssignToUserQry(const CTaskMsgTransfer<CDeviceAutoAssignToUserQryReq> *pcMsg)
{
	DatabaseQueryTemplate<CDeviceAutoAssignToUserQryReq, CDeviceAutoAssignToUserQryRsp, CDeviceAutoAssignToUserQryNtfReq, CDeviceAutoAssignInfo>(
		pcMsg, &CDbOpr::PrepareDeviceAutoAssignToUserQrySql, &CDbOpr::FetchDeviceAutoAssignToOwnerQryRecord, 
		&CDeviceAutoAssignToUserQryNtfReq::GetDeviceAutoAssignInfo, &CDeviceAutoAssignInfo::GetDevGrpId);
}

void CDbOpr::CDeviceAutoAssignToUserGroupQry(const CTaskMsgTransfer<CDeviceAutoAssignToUserGroupQryReq> *pcMsg)
{
	DatabaseQueryTemplate<CDeviceAutoAssignToUserGroupQryReq, CDeviceAutoAssignToUserGroupQryRsp, CDeviceAutoAssignToUserGroupQryNtfReq, CDeviceAutoAssignInfo>(
		pcMsg, &CDbOpr::PrepareDeviceAutoAssignToUserGroupQrySql, &CDbOpr::FetchDeviceAutoAssignToOwnerQryRecord, 
		&CDeviceAutoAssignToUserGroupQryNtfReq::GetDeviceAutoAssignInfo, &CDeviceAutoAssignInfo::GetDevGrpId);
}

ENTasErrno CDbOpr::PrepareDeviceAutoAssignToUserQrySql(const CDeviceAutoAssignToUserQryReq &cReq, TSqlString &tSqlSelect, TSqlString &tSqlCount)
{
	CUserInfo cUserInfo;
	if (GetCuserInfo(cReq.GetUserId(), cUserInfo))
	{
		if (!cUserInfo.GetUsergrpID().empty())		// 用户组不为空，表现为用户组的特性
		{
			/* 
			 * 这两个语句可以优化，既然表现为用户组的特性，那就直接查用户组对应的信息就好了
			 *
			tSqlCount.Format("select count(*) from (select a.* from %s a inner join %s b on a.%s = b.%s "
				"where b.%s = '%s' and a.%s = 1) t", 
				T_DEVICEAUTOASSIGN, T_CUSER, 
				F_DEVICEAUTOASSIGN_OWNERID, F_CUSER_USERGROUPID, 
				F_CUSER_CUSERID, cReq.GetUserId().c_str(), F_DEVICEAUTOASSIGN_OWNERTYPE);

			tSqlSelect.Format("select t.*, c.%s from (select a.* from %s a inner join %s b on a.%s = b.%s "
				"where b.%s = '%s' and a.%s = 1) t "
				"inner join %s c on t.%s = c.%s", 
				F_DEVGROUP_DOMAINID, T_DEVICEAUTOASSIGN, T_CUSER, 
				F_DEVICEAUTOASSIGN_OWNERID, F_CUSER_USERGROUPID, 
				F_CUSER_CUSERID, cReq.GetUserId().c_str(), F_DEVICEAUTOASSIGN_OWNERTYPE, 
				T_DEVGROUP, F_DEVICEAUTOASSIGN_GROUPID, F_DEVGROUP_DEVGROUPID);
			 */

			tSqlCount.Format("select count(*) from %s where %s = '%s'", 
				T_DEVICEAUTOASSIGN, F_DEVICEAUTOASSIGN_OWNERID, cUserInfo.GetUsergrpID().c_str());
			tSqlSelect.Format("select a.*, b.%s from %s a inner join %s b on a.%s = b.%s where a.%s = '%s'", 
				F_DEVGROUP_DOMAINID, T_DEVICEAUTOASSIGN, T_DEVGROUP, 
				F_DEVICEAUTOASSIGN_GROUPID, F_DEVGROUP_DEVGROUPID, 
				F_DEVICEAUTOASSIGN_OWNERID, cUserInfo.GetUsergrpID().c_str());
		}
		else										// 用户组为空，表现为用户的特性
		{
			tSqlCount.Format("select count(*) from %s where %s = '%s'", 
				T_DEVICEAUTOASSIGN, F_DEVICEAUTOASSIGN_OWNERID, cReq.GetUserId().c_str());
			tSqlSelect.Format("select a.*, b.%s from %s a inner join %s b on a.%s = b.%s where a.%s = '%s'", 
				F_DEVGROUP_DOMAINID, T_DEVICEAUTOASSIGN, T_DEVGROUP, 
				F_DEVICEAUTOASSIGN_GROUPID, F_DEVGROUP_DEVGROUPID, 
				F_DEVICEAUTOASSIGN_OWNERID, cReq.GetUserId().c_str());
		}		

		AddRowNumLimitModifer(tSqlSelect, cReq.GetStartEntryIndex(), cReq.GetExpectEntryNum());
	}
	else
	{
		return ERR_TAS_DBERROR;
	}

	return CMS_SUCCESS;
}

ENTasErrno CDbOpr::PrepareDeviceAutoAssignToUserGroupQrySql(const CDeviceAutoAssignToUserGroupQryReq &cReq, TSqlString &tSqlSelect, TSqlString &tSqlCount)
{
	tSqlCount.Format("select count(*) from %s where %s = '%s'", 
		T_DEVICEAUTOASSIGN, F_DEVICEAUTOASSIGN_OWNERID, cReq.GetUserGroupId().c_str());
	tSqlSelect.Format("select a.*, b.%s from %s a inner join %s b on a.%s = b.%s where a.%s = '%s'", 
		F_DEVGROUP_DOMAINID, T_DEVICEAUTOASSIGN, T_DEVGROUP, 
		F_DEVICEAUTOASSIGN_GROUPID, F_DEVGROUP_DEVGROUPID, 
		F_DEVICEAUTOASSIGN_OWNERID, cReq.GetUserGroupId().c_str());

	AddRowNumLimitModifer(tSqlSelect, cReq.GetStartEntryIndex(), cReq.GetExpectEntryNum());

	return CMS_SUCCESS;
}

void CDbOpr::FetchDeviceAutoAssignToOwnerQryRecord(CDeviceAutoAssignInfo &cInfo)
{
	cInfo.SetDevGrpId(GetString(F_DEVICEAUTOASSIGN_GROUPID));
	cInfo.SetDomainId(GetString(F_DEVGROUP_DOMAINID));
	cInfo.SetOwnerId(GetString(F_DEVICEAUTOASSIGN_OWNERID));
	cInfo.SetOwnerType(GetInt(F_DEVICEAUTOASSIGN_OWNERTYPE));
	cInfo.SetOperTime(GetCmsDate(F_DEVICEAUTOASSIGN_OPERTIME));
}

ENTasErrno CDbOpr::AssignDeviceByDevice(const CTaskMsgTransferBase *pcSrcTransfer, const CEventReq &cReq, const CUserDeviceInfo &cInfo)
{
	// 执行划归操作
	CuserDevItemAdd(cInfo);

	//redis中用户权限数据同步更新
	RedisAddUserDevPrivilege(cInfo);

	if (NULL != pcSrcTransfer)
	{
		// 发送划归通知
		CTaskMsgTransfer<CUserDeviceAddNtfReq> cTransNtf;
		CUserDeviceAddNtfReq &cNtf = cTransNtf.GetMsgObj();
		cNtf.SetUserDeviceInfo(cInfo);
		const CTaskMsgTransferBase *pcMsg = pcSrcTransfer;
		PostTaskMsgTransfer2CCtrl4LargeNtf(CUserDeviceAddNtfReq, GetUserDeviceInfo, GetDeviceCapPrivilege)
	}
	else
	{
		CUserDeviceAddNtfReq cNtf;
		cNtf.SetUserDeviceInfo(cInfo);
		PostSyncDataChangeNtf2CCtrl(cNtf);
	}

	return CMS_SUCCESS;
}

ENTasErrno CDbOpr::AssignDeviceByDevice(const CTaskMsgTransferBase *pcSrcTransfer, const CEventReq &cReq, const CUserGroupDeviceInfo &cInfo)
{
	// 执行划归操作
	CuserGrpDevItemAdd(cInfo);

	//redis中用户权限数据同步更新
	RedisAddUsergrpDevPrivilege(cInfo);

	if (NULL != pcSrcTransfer)
	{
		// 发送划归通知
		CTaskMsgTransfer<CUserGroupDeviceAddNtfReq> cTransNtf;
		CUserGroupDeviceAddNtfReq &cNtf = cTransNtf.GetMsgObj();
		cNtf.SetUserGroupDeviceInfo(cInfo);
		const CTaskMsgTransferBase *pcMsg = pcSrcTransfer;
		PostTaskMsgTransfer2CCtrl4LargeNtf(CUserGroupDeviceAddNtfReq, GetUserGroupDeviceInfo, GetDeviceCapPrivilege)
	}
	else
	{
		CUserGroupDeviceAddNtfReq cNtf;
		cNtf.SetUserGroupDeviceInfo(cInfo);
		PostSyncDataChangeNtf2CCtrl(cNtf);
	}

	return CMS_SUCCESS;
}

/*
 * 该函数的功能是将指定操作者用户下的指定分组的所有设备划归给指定的用户或用户组
 * 获取指定操作者用户下的执行分组下的所有设备的sql为：
 * select a.devId, a.devPrivilege from tblCuser2Dev a inner join tblDevGrp2Dev b on a.devId = b.devId where a.cuserId = 'xxx' and b.devgroupId = 'yyy';
 *
 * 实现的流程为：
 * 1、查询操作用户指定组下可供划归的设备信息
 * 2、将查询出来的信息以设备为单位组装成特定的数据结构，以便后续将批量操作转换为单个设备的操作
 * 3、循环处理每一个设备的划归操作，并以设备为单位发送变更通知（由于cui没有缓存设备组和设备之间的信息，故暂时难以处理分组划归的通知）
 */
ENTasErrno CDbOpr::AssignDeviceByDeviceGroup(const CTaskMsgTransferBase *pcSrcTransfer, const CEventReq &cReq, 
	const string &strGroupId, const string &strOperId, const string &strOwnerId, u8 byOwnerType,
    bool bAssignPriv, const CEncoderPrivilege *pencPriv, const CVideosourcePrivilege *pvideoPriv)
{
    int LimitNumOneBatch=0;
	if (0 == byOwnerType)			// 划归给用户
	{
		map<UUIDString, CUserDeviceInfo> cInfos;
		GetCuserDevGroupAddInfo(strGroupId, strOperId, strOwnerId, cInfos);

		// 开始逐条的进行划归
		for (map<UUIDString, CUserDeviceInfo>::iterator it4Info = cInfos.begin(); cInfos.end() != it4Info; ++it4Info)
		{
            if(++LimitNumOneBatch%1000==0) OspTaskDelay(100);
            
			CLASSLOG(enLogSrcDbTaskPool, PROGRESS_LEV, "【设备划归】设备组[%s]，设备[%s]，用户[%s]，操作用户[%s]\n", 
				strGroupId.c_str(), it4Info->first.c_str(), strOwnerId.c_str(), strOperId.c_str());

			// 注意，GetCuserDevGroupAddInfo中查询出来的结果中没有编码通道的信息，需要额外进行计算
			CDeviceModelInfo cDevModelInfo;
			DeviceModelItemQryByDevId(it4Info->first, cDevModelInfo);
			if (cDevModelInfo.GetDeviceTypeId() == DEVICE_TYPE_ENCODER)
			{
				AddEncodePrivilegeInfo(cDevModelInfo, it4Info->second.GetDeviceCapPrivilege());
			}

            if (bAssignPriv)
            {
                it4Info->second.GetDevicePrivilege().ConvertFromBitsContainer(*pencPriv);

                for (vector<TDeviceCapPrivilege>::iterator itCap=it4Info->second.GetDeviceCapPrivilege().begin();
                    itCap != it4Info->second.GetDeviceCapPrivilege().end(); ++itCap)
                {
                    if (itCap->id == DEVICE_CAP_VIDEOSOURCE || itCap->id == DEVICE_CAP_ENCODE_CHANNEL)
                    {
                        for (map<int, CBitsContainer>::iterator itCapPriv=itCap->privileges.begin();
                            itCapPriv!=itCap->privileges.end(); ++itCapPriv)
                        {
                            itCapPriv->second = *pvideoPriv;
                        }
                    }
                }
            }
			AssignDeviceByDevice(pcSrcTransfer, cReq, it4Info->second);
		}
	}
	else							// 划归给用户组
	{
		map<UUIDString, CUserGroupDeviceInfo> cInfos;
		GetCuserGroupDevGroupAddInfo(strGroupId, strOperId, strOwnerId, cInfos);

		// 开始逐条的进行划归
		for (map<UUIDString, CUserGroupDeviceInfo>::iterator it4Info = cInfos.begin(); cInfos.end() != it4Info; ++it4Info)
		{
            if(++LimitNumOneBatch%1000==0) OspTaskDelay(100);

			CLASSLOG(enLogSrcDbTaskPool, PROGRESS_LEV, "【设备划归】设备组[%s]，设备[%s]，用户组[%s]，操作用户[%s]\n", 
				strGroupId.c_str(), it4Info->first.c_str(), strOwnerId.c_str(), strOperId.c_str());

			// 注意，GetCuserDevGroupAddInfo中查询出来的结果中没有编码通道的信息，需要额外进行计算
			CDeviceModelInfo cDevModelInfo;
			DeviceModelItemQryByDevId(it4Info->first, cDevModelInfo);
			if (cDevModelInfo.GetDeviceTypeId() == DEVICE_TYPE_ENCODER)
			{
				AddEncodePrivilegeInfo(cDevModelInfo, it4Info->second.GetDeviceCapPrivilege());
			}


            if (bAssignPriv)
            {
                it4Info->second.GetDevicePrivilege().ConvertFromBitsContainer(*pencPriv);

                for (vector<TDeviceCapPrivilege>::iterator itCap=it4Info->second.GetDeviceCapPrivilege().begin();
                    itCap != it4Info->second.GetDeviceCapPrivilege().end(); ++itCap)
                {
                    if (itCap->id == DEVICE_CAP_VIDEOSOURCE || itCap->id == DEVICE_CAP_ENCODE_CHANNEL)
                    {
                        for (map<int, CBitsContainer>::iterator itCapPriv=itCap->privileges.begin();
                            itCapPriv!=itCap->privileges.end(); ++itCapPriv)
                        {
                            itCapPriv->second = *pvideoPriv;
                        }
                    }
                }
            }
			AssignDeviceByDevice(pcSrcTransfer, cReq, it4Info->second);
		}
	}

	return CMS_SUCCESS;
}

ENTasErrno CDbOpr::AssignDeviceByDeviceGroup(const CTaskMsgTransferBase *pcSrcTransfer, const CEventReq &cReq, 
	const string &strGroupId, const string &strOperId, map<string, u8> cOwners)
{
	for (map<string, u8>::const_iterator it = cOwners.begin(); cOwners.end() != it; ++it)
	{
		AssignDeviceByDeviceGroup(pcSrcTransfer, cReq, strGroupId, strOperId, it->first, it->second);
	}

	return CMS_SUCCESS;
}

ENTasErrno CDbOpr::RevokeDeviceByDevice(const CTaskMsgTransferBase *pcSrcTransfer, const CEventReq &cReq, const CUserDeviceDelInfo &cDelInfo)
{
	// 删除用户设备，删除用户对应的PTZ锁 [4/14/2016 pangubing]
	DeleteUserPtzLockInner(pcSrcTransfer,cDelInfo.key.GetUserID(),cDelInfo.key.GetDeviceID(),cDelInfo.key.GetDeviceCapIndexs());

	// 执行取消划归操作
	CuserDevItemDel(cDelInfo.key);

    // redis中用户权限数据同步删除
	RedisDelUserDevPrivilege(cDelInfo.key);

	if (NULL != pcSrcTransfer)
	{
		// 发送划归通知
		CTaskMsgTransfer<CUserDeviceDelNtfReq> cTransNtf;
		CUserDeviceDelNtfReq &cNtf = cTransNtf.GetMsgObj();
		cNtf.SetDevDomainId(cDelInfo.domainId);
		cNtf.SetUserDeviceInfoKey(cDelInfo.key);
		const CTaskMsgTransferBase *pcMsg = pcSrcTransfer;

		// 原代码中划归的时候拆包了，取消划归又不拆包，理论上会有问题，这里暂时保持一致
		PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cNtf, cTransNtf);
	}
	else
	{
		CUserDeviceDelNtfReq cNtf;
		cNtf.SetDevDomainId(cDelInfo.domainId);
		cNtf.SetUserDeviceInfoKey(cDelInfo.key);
		PostSyncDataChangeNtf2CCtrl(cNtf);
	}

	return CMS_SUCCESS;
}

ENTasErrno CDbOpr::RevokeDeviceByDevice(const CTaskMsgTransferBase *pcSrcTransfer, const CEventReq &cReq, const CUserGroupDeviceDelInfo &cDelInfo)
{

	// 删除用户组设备，删除用户对应的PTZ锁 [4/14/2016 pangubing]
	vector<CUserInfo> vctUserInfos;
	CuserQryByUserGrp(cDelInfo.key.GetGroupID(),vctUserInfos);
	for (u32 k = 0; k< vctUserInfos.size(); ++k)
	{
		DeleteUserPtzLockInner(pcSrcTransfer,vctUserInfos[k].GetUserID(),cDelInfo.key.GetDeviceID(),cDelInfo.key.GetDeviceCapIndexs());
	}

	// 执行取消划归操作
	CuserGrpDevItemDel(cDelInfo.key);

	// redis中用户权限数据同步删除
	RedisDelUsergrpDevPrivilege(cDelInfo.key);

	if (NULL != pcSrcTransfer)
	{
		// 发送划归通知
		CTaskMsgTransfer<CUserGroupDeviceDelNtfReq> cTransNtf;
		CUserGroupDeviceDelNtfReq &cNtf = cTransNtf.GetMsgObj();
		cNtf.SetDevDomainId(cDelInfo.domainId);
		cNtf.SetUserGroupDeviceInfoKey(cDelInfo.key);
		const CTaskMsgTransferBase *pcMsg = pcSrcTransfer;

		// 原代码中划归的时候拆包了，取消划归又不拆包，理论上会有问题，这里暂时保持一致
		PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cNtf, cTransNtf);
	}
	else
	{
		CUserGroupDeviceDelNtfReq cNtf;
		cNtf.SetDevDomainId(cDelInfo.domainId);
		cNtf.SetUserGroupDeviceInfoKey(cDelInfo.key);
		PostSyncDataChangeNtf2CCtrl(cNtf);
	}

	return CMS_SUCCESS;
}

ENTasErrno CDbOpr::RevokeDeviceByDeviceGroup(const CTaskMsgTransferBase *pcSrcTransfer, const CEventReq &cReq, 
	const string &strGroupId, const string &strOperId, const string &strOwnerId, u8 byOwnerType)
{
	if (0 == byOwnerType)			// 从用户取消划归
	{
		map<UUIDString, CUserDeviceDelInfo> cInfos;
		GetCuserDevGroupDelInfo(strGroupId, strOperId, strOwnerId, cInfos);

		// 开始逐条的进行划归
		for (map<UUIDString, CUserDeviceDelInfo>::iterator it4Info = cInfos.begin(); cInfos.end() != it4Info; ++it4Info)
		{
			CLASSLOG(enLogSrcDbTaskPool, PROGRESS_LEV, "【设备取消划归】设备组[%s]，设备[%s]，用户[%s]，操作用户[%s]\n", 
				strGroupId.c_str(), it4Info->first.c_str(), strOwnerId.c_str(), strOperId.c_str());

            // 删除收藏夹中的数据
            DevGrpDevDel4Favorite(strOwnerId, it4Info->second.key);

			// 注意，GetCuserDevGroupDelInfo中查询出来的结果中没有编码通道的信息，需要额外进行计算
			CDeviceModelInfo cDevModelInfo;
			DeviceModelItemQryByDevId(it4Info->first, cDevModelInfo);
			if (cDevModelInfo.GetDeviceTypeId() == DEVICE_TYPE_ENCODER)
			{
				AddEncodeIndexInfo(cDevModelInfo, it4Info->second.key.GetDeviceCapIndexs());
			}

			RevokeDeviceByDevice(pcSrcTransfer, cReq, it4Info->second);
		}
	}
	else							// 从用户组取消划归
	{
		map<UUIDString, CUserGroupDeviceDelInfo> cInfos;
		GetCuserGroupDevGroupDelInfo(strGroupId, strOperId, strOwnerId, cInfos);

        vector<CUserInfo> vecCuserInfo;
        CuserQryByUserGrp(strOwnerId, vecCuserInfo);
        CUserDeviceInfoKey cUserDevKey;

		// 开始逐条的进行划归
		for (map<UUIDString, CUserGroupDeviceDelInfo>::iterator it4Info = cInfos.begin(); cInfos.end() != it4Info; ++it4Info)
		{
			CLASSLOG(enLogSrcDbTaskPool, PROGRESS_LEV, "【设备取消划归】设备组[%s]，设备[%s]，用户[%s]，操作用户[%s]\n", 
				strGroupId.c_str(), it4Info->first.c_str(), strOwnerId.c_str(), strOperId.c_str());

            // 删除收藏夹中的数据
            cUserDevKey.SetDeviceID(it4Info->second.key.GetDeviceID());
            cUserDevKey.SetDeviceCapIndexs(it4Info->second.key.GetDeviceCapIndexs());
            for (vector<CUserInfo>::const_iterator itUser=vecCuserInfo.begin();itUser!=vecCuserInfo.end();++itUser)
            {
                DevGrpDevDel4Favorite(itUser->GetUserID(), cUserDevKey);
            }

			// 注意，GetCuserDevGroupDelInfo中查询出来的结果中没有编码通道的信息，需要额外进行计算
			CDeviceModelInfo cDevModelInfo;
			DeviceModelItemQryByDevId(it4Info->first, cDevModelInfo);
			if (cDevModelInfo.GetDeviceTypeId() == DEVICE_TYPE_ENCODER)
			{
				AddEncodeIndexInfo(cDevModelInfo, it4Info->second.key.GetDeviceCapIndexs());
			}

			RevokeDeviceByDevice(pcSrcTransfer, cReq, it4Info->second);
		}
	}

	return CMS_SUCCESS;
}

ENTasErrno CDbOpr::RevokeDeviceByDeviceGroup(const CTaskMsgTransferBase *pcSrcTransfer, const CEventReq &cReq, 
	const string &strGroupId, const string &strOperId, map<string, u8> cOwners)
{
	for (map<string, u8>::const_iterator it = cOwners.begin(); cOwners.end() != it; ++it)
	{
		RevokeDeviceByDeviceGroup(pcSrcTransfer, cReq, strGroupId, strOperId, it->first, it->second);
	}

	return CMS_SUCCESS;
}

ENTasErrno CDbOpr::RegistAutoAssignDeviceGroup(const string &strGroupId, const string &strOwnerId, u8 byOwnerType)
{
	TSqlString tSql;
	tSql.Format("insert into %s(%s, %s, %s, %s) values('%s', '%s', %d, '%s') on duplicate key update %s = '%s'", 
		T_DEVICEAUTOASSIGN, F_DEVICEAUTOASSIGN_GROUPID, F_DEVICEAUTOASSIGN_OWNERID, 
		F_DEVICEAUTOASSIGN_OWNERTYPE, F_DEVICEAUTOASSIGN_OPERTIME, 
		strGroupId.c_str(), strOwnerId.c_str(), 
		byOwnerType, DbNowDateTime(), 
		F_DEVICEAUTOASSIGN_OPERTIME, DbNowDateTime());

	ExecSql(tSql, enExecNoRet);

	return CMS_SUCCESS;
}

ENTasErrno CDbOpr::RegistAutoAssignDeviceGroup(const string &strGroupId, map<string, u8> cOwners)
{
	for (map<string, u8>::const_iterator it = cOwners.begin(); cOwners.end() != it; ++it)
	{
		RegistAutoAssignDeviceGroup(strGroupId, it->first, it->second);
	}

	return CMS_SUCCESS;
}

ENTasErrno CDbOpr::UnRegistAutoAssignDeviceGroup(const string &strGroupId, const string &strOwnerId, u8 byOwnerType)
{
	TSqlString tSql;
	tSql.Format("delete from %s where %s='%s' and %s = '%s' and %s = %d", 
		T_DEVICEAUTOASSIGN, F_DEVICEAUTOASSIGN_GROUPID, strGroupId.c_str(), 
		F_DEVICEAUTOASSIGN_OWNERID, strOwnerId.c_str(), 
		F_DEVICEAUTOASSIGN_OWNERTYPE, byOwnerType);

	if (!ExecSql(tSql, enExecNoRet))
	{
		return ERR_TAS_DBERROR;
	}

	return CMS_SUCCESS;
}

ENTasErrno CDbOpr::UnRegistAutoAssignDeviceGroup(const string &strGroupId)
{
	TSqlString tSql;
	tSql.Format("delete from %s where %s='%s'", 
		T_DEVICEAUTOASSIGN, F_DEVICEAUTOASSIGN_GROUPID, strGroupId.c_str());

	ExecSql(tSql, enExecNoRet);

	return CMS_SUCCESS;
}

ENTasErrno CDbOpr::AutoAssignDevice(const CTaskMsgTransferBase *pcSrcTransfer, const CEventReq &cReq, 
	const string &strDevId, const vector<TDeviceCapIndexs> &vctCap, const string &strGroupId)
{
	CDeviceModelInfo cDevModelInfo;
	DeviceModelItemQryByDevId(strDevId, cDevModelInfo);
	if (cDevModelInfo.GetDeviceTypeId() == DEVICE_TYPE_ENCODER)
	{
		vector<TDeviceCapPrivilege> vctInCapPrivilege;

		// 经与需求沟通，表示可以简单处理，设置权限为-1，即所有权限
		CBitsContainer cBits;
		cBits.ConvertFromInt(-1);

		// 将vector<TDeviceCapIndexs>转换为vector<TDeviceCapPrivilege>
		for (vector<TDeviceCapIndexs>::const_iterator it4Cap = vctCap.begin(); vctCap.end() != it4Cap; ++it4Cap)
		{
			vector<TDeviceCapPrivilege>::iterator it4CapPri = vctInCapPrivilege.begin();
			for (; vctInCapPrivilege.end() != it4CapPri; ++it4CapPri)
			{
				if (it4CapPri->id == it4Cap->id)
				{
					break;
				}
			}

			if (vctInCapPrivilege.end() != it4CapPri)
			{
				for (vector<int>::const_iterator it4Index = it4Cap->indexs.begin(); it4Cap->indexs.end() != it4Index; ++it4Index)
				{
					it4CapPri->privileges.insert(pair<int, CBitsContainer>(*it4Index, cBits));
				}
			}
			else
			{
				TDeviceCapPrivilege tItemPri;
				tItemPri.id = it4Cap->id;
				for (vector<int>::const_iterator it4Index = it4Cap->indexs.begin(); it4Cap->indexs.end() != it4Index; ++it4Index)
				{
					tItemPri.privileges.insert(pair<int, CBitsContainer>(*it4Index, cBits));
				}

				vctInCapPrivilege.push_back(tItemPri);
			}
		}

		map<UUIDString, CUserDeviceInfo> cInfos4User;
		map<UUIDString, CUserGroupDeviceInfo> cInfos4UserGroup;

		TSqlString tSql;
		for (vector<TDeviceCapPrivilege>::const_iterator it4Cap = vctInCapPrivilege.begin(); vctInCapPrivilege.end() != it4Cap; ++it4Cap)
		{
			if (DEVICE_CAP_VIDEOSOURCE == it4Cap->id)		// 只有视频源和编码通道允许划归，编码通道后续自动生成，这里只处理视频源
			{
				const map<int, CBitsContainer> cPrivileges = it4Cap->privileges;
				for (map<int, CBitsContainer>::const_iterator it4Pri = cPrivileges.begin(); cPrivileges.end() != it4Pri; ++it4Pri)
				{
					if (strGroupId.empty())		// strGroupId为空，需要先找到所属组
					{
						tSql.Format("select a.*, b.%s, b.%s, c.%s from %s a inner join %s b "
							"on a.%s = b.%s inner join %s c on a.%s = c.%s where a.%s = '%s' and a.%s = %d "
							" and a.%s = %d", 
							F_DEVICEAUTOASSIGN_OWNERID, F_DEVICEAUTOASSIGN_OWNERTYPE, F_DEVICE_DOMAINID, 
							T_DEVGRP2DEV, T_DEVICEAUTOASSIGN, F_DEVGRP2DEV_DEVGROUPID, F_DEVICEAUTOASSIGN_GROUPID, 
							T_DEVICE, F_DEVGRP2DEV_DEVID, F_DEVICE_DEVID, F_DEVGRP2DEV_DEVID, strDevId.c_str(), 
							F_DEVGRP2DEV_DEVCAPID, it4Cap->id, F_DEVGRP2DEV_DEVCAPINDEX, it4Pri->first);
					}
					else
					{
						tSql.Format("select '%s' %s, %d %s, %d %s, b.%s, b.%s, c.%s from %s b "
							"inner join %s c where b.%s = '%s' and c.%s = '%s' ", 
							strDevId.c_str(), F_DEVGRP2DEV_DEVID, it4Cap->id, F_DEVGRP2DEV_DEVCAPID, it4Pri->first, F_DEVGRP2DEV_DEVCAPINDEX, 
							F_DEVICEAUTOASSIGN_OWNERID, F_DEVICEAUTOASSIGN_OWNERTYPE, 
							F_DEVICE_DOMAINID, T_DEVICEAUTOASSIGN, T_DEVICE, 
							F_DEVICEAUTOASSIGN_GROUPID, strGroupId.c_str(), 
							F_DEVICE_DEVID, strDevId.c_str());
					}

					if (!ExecSql(tSql, enExecMayRet))
					{
						return ERR_TAS_DBERROR;
					}

					while (IsNotEof())
					{
						string strOwnerId = GetString(F_DEVICEAUTOASSIGN_OWNERID);
						u8 byOwnerType = (u8)GetInt(F_DEVICEAUTOASSIGN_OWNERTYPE);

						if (0 == byOwnerType)
						{
							CUserDeviceInfo &cItem = cInfos4User[strOwnerId];
							if (cItem.GetDeviceID().empty())
							{
								cItem.SetDeviceID(strDevId);
								cItem.SetUserID(strOwnerId);
								cItem.SetDevDomainID(GetString(F_DEVICE_DOMAINID));
								CDevicePrivilege cDevicePrivilege;
								cDevicePrivilege.ConvertFromInt(-1);		// 设备的权限没有意义，设置为-1
								cItem.SetDevicePrivilege(cDevicePrivilege);
							}

							s32 nCapId    = GetInt(F_DEVGRP2DEV_DEVCAPID);
							s32 nCapIndex = GetInt(F_DEVGRP2DEV_DEVCAPINDEX);
							vector<TDeviceCapPrivilege> &cCaps = cItem.GetDeviceCapPrivilege();
							vector<TDeviceCapPrivilege>::iterator it = cCaps.begin();
							for (; cCaps.end() != it; ++it)
							{
								if (it->id == nCapId)
								{
									break;
								}
							}

							if (cCaps.end() != it)
							{
								it->privileges.insert(pair<int, CBitsContainer>(nCapIndex, it4Pri->second));
							}
							else
							{
								TDeviceCapPrivilege tCapPrivilege;
								tCapPrivilege.id = nCapId;
								tCapPrivilege.privileges.insert(pair<int, CBitsContainer>(nCapIndex, it4Pri->second));
								cCaps.push_back(tCapPrivilege);
							}
						}
						else
						{
							CUserGroupDeviceInfo &cItem = cInfos4UserGroup[strOwnerId];
							if (cItem.GetDeviceID().empty())
							{
								cItem.SetDeviceID(strDevId);
								cItem.SetGroupID(strOwnerId);
								cItem.SetDevDomainID(GetString(F_DEVICE_DOMAINID));
								CDevicePrivilege cDevicePrivilege;
								cDevicePrivilege.ConvertFromInt(-1);		// 设备的权限没有意义，设置为-1
								cItem.SetDevicePrivilege(cDevicePrivilege);
							}

							s32 nCapId    = GetInt(F_DEVGRP2DEV_DEVCAPID);
							s32 nCapIndex = GetInt(F_DEVGRP2DEV_DEVCAPINDEX);
							vector<TDeviceCapPrivilege> &cCaps = cItem.GetDeviceCapPrivilege();
							vector<TDeviceCapPrivilege>::iterator it = cCaps.begin();
							for (; cCaps.end() != it; ++it)
							{
								if (it->id == nCapId)
								{
									break;
								}
							}

							if (cCaps.end() != it)
							{
								it->privileges.insert(pair<int, CBitsContainer>(nCapIndex, it4Pri->second));
							}
							else
							{
								TDeviceCapPrivilege tCapPrivilege;
								tCapPrivilege.id = nCapId;
								tCapPrivilege.privileges.insert(pair<int, CBitsContainer>(nCapIndex, it4Pri->second));
								cCaps.push_back(tCapPrivilege);
							}
						}

						MoveNext();
					}
				}
			
				break;
			}
		}

		// 根据视频源添加编码通道的信息
		for (map<UUIDString, CUserDeviceInfo>::iterator it4Info = cInfos4User.begin(); cInfos4User.end() != it4Info; ++it4Info)
		{
			AddEncodePrivilegeInfo(cDevModelInfo, it4Info->second.GetDeviceCapPrivilege());
		}

		for (map<UUIDString, CUserGroupDeviceInfo>::iterator it4Info = cInfos4UserGroup.begin(); cInfos4UserGroup.end() != it4Info; ++it4Info)
		{
			AddEncodePrivilegeInfo(cDevModelInfo, it4Info->second.GetDeviceCapPrivilege());
		}

		for (map<UUIDString, CUserDeviceInfo>::const_iterator it4Info = cInfos4User.begin(); cInfos4User.end() != it4Info; ++it4Info)
		{
			CLASSLOG(enLogSrcDbTaskPool, PROGRESS_LEV, "【自动划归】设备[%s]，用户[%s]\n", 
				strDevId.c_str(), it4Info->first.c_str());

			AssignDeviceByDevice(pcSrcTransfer, cReq, it4Info->second);
		}

		for (map<UUIDString, CUserGroupDeviceInfo>::const_iterator it4Info = cInfos4UserGroup.begin(); cInfos4UserGroup.end() != it4Info; ++it4Info)
		{
			CLASSLOG(enLogSrcDbTaskPool, PROGRESS_LEV, "【自动划归】设备[%s]，用户[%s]\n", 
				strDevId.c_str(), it4Info->first.c_str());

			AssignDeviceByDevice(pcSrcTransfer, cReq, it4Info->second);
		}
	}

	return CMS_SUCCESS;
}

ENTasErrno CDbOpr::AutoAssignDevice(const CTaskMsgTransferBase *pcSrcTransfer, const CEventReq &cReq, 
	const string &strDevId, const string &strDevDomainId, const CDeviceModelInfo &cModel, const string &strGroupId)
{
	if (DEVICE_TYPE_ENCODER == cModel.GetDeviceTypeId())
	{
		map<string, u8> cOwners;
		GetAutoAssignDeviceGroup(strGroupId, cOwners);

		if (cOwners.empty())			// 如果改组没有设置自动更新的用户和用户组，则没必要往下走了
		{
			return CMS_SUCCESS;
		}

		// 获取设备的所有可划归信息
		vector<TDeviceCapPrivilege> vctCapPri;
		CBitsContainer cPri;
		cPri.ConvertFromInt(-1);			// 默认权限为-1，即所有权限
		const vector<TDeviceCapInfo> &vctCapInfo = cModel.GetDeviceCapInfo();
		for (vector<TDeviceCapInfo>::const_iterator it = vctCapInfo.begin(); vctCapInfo.end() != it; ++it)
		{
			if (it->type == DEVICE_CAP_VIDEOSOURCE)		// 只有视频源和编码通道可以划归
			{
				TDeviceCapPrivilege tItem;
				tItem.id = it->type;

				for (u32 i = 0; i < it->num; ++i)
				{
					tItem.privileges[i] = cPri;
				}

				vctCapPri.push_back(tItem);

				break;
			}
		}

		// 根据视频源生成对应的编码通道信息
		AddEncodePrivilegeInfo(cModel, vctCapPri);

		for (map<string, u8>::const_iterator it = cOwners.begin(); cOwners.end() != it; ++it)
		{
			if (0 == it->second)							// 划归给用户
			{
				CUserDeviceInfo cInfo;
				cInfo.SetDeviceID(strDevId);
				cInfo.SetDevDomainID(strDevDomainId);
				CDevicePrivilege cDevicePrivilege;
				cDevicePrivilege.ConvertFromInt(-1);		// 默认为-1
				cInfo.SetDevicePrivilege(cDevicePrivilege);
				cInfo.SetUserID(it->first);
				cInfo.SetDeviceCapPrivilege(vctCapPri);

				AssignDeviceByDevice(pcSrcTransfer, cReq, cInfo);
			}
			else											// 划归给用户组
			{
				CUserGroupDeviceInfo cInfo;
				cInfo.SetDeviceID(strDevId);
				cInfo.SetDevDomainID(strDevDomainId);
				CDevicePrivilege cDevicePrivilege;
				cDevicePrivilege.ConvertFromInt(-1);		// 默认为-1
				cInfo.SetDevicePrivilege(cDevicePrivilege);
				cInfo.SetGroupID(it->first);
				cInfo.SetDeviceCapPrivilege(vctCapPri);

				AssignDeviceByDevice(pcSrcTransfer, cReq, cInfo);
			}
		}
	}

	return CMS_SUCCESS;
}

/*
 * 由于表tblDevGrp2Dev中没有编码通道的划归信息，而tblCuser2Dev中又有编码通道的划归信息，造成了这段代码变得复杂起来，shit!
 */
ENTasErrno CDbOpr::AutoRevokeDevice(const CTaskMsgTransferBase *pcSrcTransfer, const CEventReq &cReq, 
	const string &strDevId, const vector<TDeviceCapIndexs> &vctCap, const string &strGroupId)
{
	CDeviceModelInfo cDevModelInfo;
	DeviceModelItemQryByDevId(strDevId, cDevModelInfo);
	if (DEVICE_TYPE_ENCODER == cDevModelInfo.GetDeviceTypeId())		// 只有编码器才能划归
	{
		map<UUIDString, CUserDeviceDelInfo> cInfos4User;
		map<UUIDString, CUserGroupDeviceDelInfo> cInfos4UserGroup;

		TSqlString tSql;
		for (vector<TDeviceCapIndexs>::const_iterator it4Cap = vctCap.begin(); vctCap.end() != it4Cap; ++it4Cap)
		{
			if (DEVICE_CAP_VIDEOSOURCE == it4Cap->id)				// 只有视频源和编码通道允许划归，编码通道后续自动生成，这里只处理视频源
			{
				const vector<int> &vctIndexs = it4Cap->indexs;
				for (vector<int>::const_iterator it4Index = vctIndexs.begin(); vctIndexs.end() != it4Index; ++it4Index)
				{
					if (strGroupId.empty())		// strGroupId为空，则取设备能力所在组（视频源能通过tblDevGrp2Dev查出所在组，但是编码通道不行）
					{
						tSql.Format("select a.*, b.%s, b.%s, c.%s from %s a inner join %s b "
							"on a.%s = b.%s inner join %s c on a.%s = c.%s where a.%s = '%s' and a.%s = %d "
							" and a.%s = %d", 
							F_DEVICEAUTOASSIGN_OWNERID, F_DEVICEAUTOASSIGN_OWNERTYPE, F_DEVICE_DOMAINID, 
							T_DEVGRP2DEV, T_DEVICEAUTOASSIGN, F_DEVGRP2DEV_DEVGROUPID, F_DEVICEAUTOASSIGN_GROUPID, 
							T_DEVICE, F_DEVGRP2DEV_DEVID, F_DEVICE_DEVID, F_DEVGRP2DEV_DEVID, strDevId.c_str(), 
							F_DEVGRP2DEV_DEVCAPID, it4Cap->id, F_DEVGRP2DEV_DEVCAPINDEX, *it4Index);
					}
					else
					{
						tSql.Format("select '%s' %s, %d %s, %d %s, b.%s, b.%s, c.%s from %s b "
							"inner join %s c where b.%s = '%s' and c.%s = '%s' ", 
							strDevId.c_str(), F_DEVGRP2DEV_DEVID, it4Cap->id, F_DEVGRP2DEV_DEVCAPID, *it4Index, F_DEVGRP2DEV_DEVCAPINDEX, 
							F_DEVICEAUTOASSIGN_OWNERID, F_DEVICEAUTOASSIGN_OWNERTYPE, 
							F_DEVICE_DOMAINID, T_DEVICEAUTOASSIGN, T_DEVICE, 
							F_DEVICEAUTOASSIGN_GROUPID, strGroupId.c_str(), 
							F_DEVICE_DEVID, strDevId.c_str());
					}

					if (!ExecSql(tSql, enExecMayRet))
					{
						return ERR_TAS_DBERROR;
					}

					while (IsNotEof())
					{
						string strOwnerId = GetString(F_DEVICEAUTOASSIGN_OWNERID);
						u8 byOwnerType = (u8)GetInt(F_DEVICEAUTOASSIGN_OWNERTYPE);

						if (0 == byOwnerType)
						{
							CUserDeviceDelInfo &cItem = cInfos4User[strOwnerId];
							if (cItem.key.GetDeviceID().empty())
							{
								cItem.key.SetDeviceID(strDevId);
								cItem.key.SetUserID(strOwnerId);
								cItem.domainId = GetString(F_DEVICE_DOMAINID);
							}

							s32 nCapId    = GetInt(F_DEVGRP2DEV_DEVCAPID);
							s32 nCapIndex = GetInt(F_DEVGRP2DEV_DEVCAPINDEX);
							vector<TDeviceCapIndexs> &cCaps = cItem.key.GetDeviceCapIndexs();
							vector<TDeviceCapIndexs>::iterator it = cCaps.begin();
							for (; cCaps.end() != it; ++it)
							{
								if (it->id == nCapId)
								{
									break;
								}
							}

							if (cCaps.end() != it)
							{
								it->indexs.push_back(nCapIndex);
							}
							else
							{
								TDeviceCapIndexs tCapIndex;
								tCapIndex.id = nCapId;
								tCapIndex.indexs.push_back(nCapIndex);
								cCaps.push_back(tCapIndex);
							}
						}
						else
						{
							CUserGroupDeviceDelInfo &cItem = cInfos4UserGroup[strOwnerId];
							if (cItem.key.GetDeviceID().empty())
							{
								cItem.key.SetDeviceID(strDevId);
								cItem.key.SetGroupID(strOwnerId);
								cItem.domainId = GetString(F_DEVICE_DOMAINID);
							}

							s32 nCapId    = GetInt(F_DEVGRP2DEV_DEVCAPID);
							s32 nCapIndex = GetInt(F_DEVGRP2DEV_DEVCAPINDEX);
							vector<TDeviceCapIndexs> &cCaps = cItem.key.GetDeviceCapIndexs();
							vector<TDeviceCapIndexs>::iterator it = cCaps.begin();
							for (; cCaps.end() != it; ++it)
							{
								if (it->id == nCapId)
								{
									break;
								}
							}

							if (cCaps.end() != it)
							{
								it->indexs.push_back(nCapIndex);
							}
							else
							{
								TDeviceCapIndexs tCapIndex;
								tCapIndex.id = nCapId;
								tCapIndex.indexs.push_back(nCapIndex);
								cCaps.push_back(tCapIndex);
							}
						}

						MoveNext();
					}

					break;
				}
			}
		}

		// 根据视频源添加编码通道的信息
		for (map<UUIDString, CUserDeviceDelInfo>::iterator it4Info = cInfos4User.begin(); cInfos4User.end() != it4Info; ++it4Info)
		{
			AddEncodeIndexInfo(cDevModelInfo, it4Info->second.key.GetDeviceCapIndexs());
		}

		for (map<UUIDString, CUserGroupDeviceDelInfo>::iterator it4Info = cInfos4UserGroup.begin(); cInfos4UserGroup.end() != it4Info; ++it4Info)
		{
			AddEncodeIndexInfo(cDevModelInfo, it4Info->second.key.GetDeviceCapIndexs());
		}

		for (map<UUIDString, CUserDeviceDelInfo>::const_iterator it4Info = cInfos4User.begin(); cInfos4User.end() != it4Info; ++it4Info)
		{
			CLASSLOG(enLogSrcDbTaskPool, PROGRESS_LEV, "【自动取消划归】设备[%s]，用户[%s]\n", 
				strDevId.c_str(), it4Info->first.c_str());

			RevokeDeviceByDevice(pcSrcTransfer, cReq, it4Info->second);
		}

		for (map<UUIDString, CUserGroupDeviceDelInfo>::const_iterator it4Info = cInfos4UserGroup.begin(); cInfos4UserGroup.end() != it4Info; ++it4Info)
		{
			CLASSLOG(enLogSrcDbTaskPool, PROGRESS_LEV, "【自动取消划归】设备[%s]，用户[%s]\n", 
				strDevId.c_str(), it4Info->first.c_str());

			RevokeDeviceByDevice(pcSrcTransfer, cReq, it4Info->second);
		}
	}

	return CMS_SUCCESS;
}

bool CDbOpr::IsAutoAssignDeviceGroup(const string &strGroupId, const string &strOwnerId, u8 byOwnerType)
{
	TSqlString tSql;
	tSql.Format("select count(*) from %s where %s = '%s' and %s = '%s' and %s = %d", 
		T_DEVICEAUTOASSIGN, F_DEVICEAUTOASSIGN_GROUPID, strGroupId.c_str(), 
		F_DEVICEAUTOASSIGN_OWNERID, strOwnerId.c_str(), 
		F_DEVICEAUTOASSIGN_OWNERTYPE, byOwnerType);

	ExecSql(tSql, enExecMustRet);
	if (0 < GetIntResult())
	{
		return true;
	}

	return false;
}

/*
 * 判断指定的设备能力集中是否有处于自动更新组下的，这里要取消判断编码通道能力，因为tblDevGrp2Dev中没有编码通道的信息
 */
bool CDbOpr::IsAutoAssignDeviceGroup(const string &strDevId, const vector<TDeviceCapIndexs> &vctDevCap, const string &strOwnerId, u8 byOwnerType)
{
	TSqlString tSql;

	for (vector<TDeviceCapIndexs>::const_iterator it4Cap = vctDevCap.begin(); vctDevCap.end() != it4Cap; ++it4Cap)
	{
		if (it4Cap->id == DEVICE_CAP_ENCODE_CHANNEL)		// 取消判断编码通道能力，因为tblDevGrp2Dev中没有编码通道的信息
		{
			continue;
		}

		for (vector<int>::const_iterator it4Index = it4Cap->indexs.begin(); it4Cap->indexs.end() != it4Index; ++it4Index)
		{
			tSql.Format("select count(*) from %s a inner join %s b on a.%s = b.%s "
				"where a.%s = '%s' and a.%s = %d and a.%s = %d and b.%s = '%s' and b.%s = %d", 
				T_DEVGRP2DEV, T_DEVICEAUTOASSIGN, F_DEVGRP2DEV_DEVGROUPID, F_DEVICEAUTOASSIGN_GROUPID, 
				F_DEVGRP2DEV_DEVID, strDevId.c_str(), F_DEVGRP2DEV_DEVCAPID, it4Cap->id, F_DEVGRP2DEV_DEVCAPINDEX, *it4Index, 
				F_DEVICEAUTOASSIGN_OWNERID, strOwnerId.c_str(), F_DEVICEAUTOASSIGN_OWNERTYPE, byOwnerType);

			ExecSql(tSql, enExecMustRet);
			if (0 < GetIntResult())
			{
				return true;
			}
		}
	}

	return false;
}

bool CDbOpr::IsTopAutoAssignDeviceGroup(const string &strGroupId, const string &strOwnerId, u8 byOwnerType)
{
	TSqlString tSql;
	tSql.Format("select count(*) from %s c inner join(select a.%s from %s a where a.%s = '%s' "
		"and not exists(select * from %s b where b.%s = a.%s and b.%s = '%s' and b.%s = %d)) t "
		"on c.%s = t.%s where c.%s = '%s' and c.%s = '%s' and c.%s = %d", 
		T_DEVICEAUTOASSIGN, F_DEVGROUP_DEVGROUPID, T_DEVGROUP, 
		F_DEVGROUP_DEVGROUPID, strGroupId.c_str(), 
		T_DEVICEAUTOASSIGN, F_DEVICEAUTOASSIGN_GROUPID, F_DEVGROUP_PARENTDEVGROUPID, 
		F_DEVICEAUTOASSIGN_OWNERID, strOwnerId.c_str(), F_DEVICEAUTOASSIGN_OWNERTYPE, byOwnerType, 
		F_DEVICEAUTOASSIGN_GROUPID, F_DEVGROUP_DEVGROUPID, F_DEVICEAUTOASSIGN_GROUPID, strGroupId.c_str(), 
		F_DEVICEAUTOASSIGN_OWNERID, strOwnerId.c_str(), F_DEVICEAUTOASSIGN_OWNERTYPE, byOwnerType);

	ExecSql(tSql, enExecMustRet);
	if (0 < GetIntResult())
	{
		return true;
	}

	return false;
}

void CDbOpr::GetAutoAssignDeviceGroup(const string &strGroupId, map<string, u8> &cOwners)
{
	TSqlString tSql;
	tSql.Format("select * from %s where %s = '%s'", 
		T_DEVICEAUTOASSIGN, F_DEVICEAUTOASSIGN_GROUPID, strGroupId.c_str());

	ExecSql(tSql, enExecMayRet);
	
	while (IsNotEof())
	{
		cOwners.insert(pair<string, u8>(GetString(F_DEVICEAUTOASSIGN_OWNERID), (u8)GetInt(F_DEVICEAUTOASSIGN_OWNERTYPE)));

		MoveNext();
	}
}

void CDbOpr::GetTopAutoAssignDeviceGroup(const string &strGroupId, map<string, u8> &cOwners)
{
	TSqlString tSql;
	tSql.Format("select c.* from %s c inner join(select a.%s from %s a where a.%s = '%s' "
		"and not exists(select * from %s b where b.%s = a.%s)) t on c.%s = t.%s where c.%s = '%s'", 
		T_DEVICEAUTOASSIGN, F_DEVGROUP_DEVGROUPID, T_DEVGROUP, 
		F_DEVGROUP_DEVGROUPID, strGroupId.c_str(), 
		T_DEVICEAUTOASSIGN, F_DEVICEAUTOASSIGN_GROUPID, F_DEVGROUP_PARENTDEVGROUPID, 
		F_DEVICEAUTOASSIGN_GROUPID, F_DEVGROUP_DEVGROUPID, F_DEVICEAUTOASSIGN_GROUPID, strGroupId.c_str());


	ExecSql(tSql, enExecMayRet);

	while (IsNotEof())
	{
		cOwners.insert(pair<string, u8>(GetString(F_DEVICEAUTOASSIGN_OWNERID), (u8)GetInt(F_DEVICEAUTOASSIGN_OWNERTYPE)));

		MoveNext();
	}
}

void CDbOpr::ClearUserAutoAssignDeviceGroup(const string &strOwnerId, u8 byOwnerType)
{
	TSqlString tSql;
	tSql.Format("delete from %s where %s = '%s' and %s = %d", 
		T_DEVICEAUTOASSIGN, F_DEVICEAUTOASSIGN_OWNERID, strOwnerId.c_str(), 
		F_DEVICEAUTOASSIGN_OWNERTYPE, byOwnerType);

	ExecSql(tSql, enExecNoRet);
}

ENTasErrno CDbOpr::GetAdminUserId(string &strUserId)
{
	TSqlString tSql;
	tSql.Format("select %s from %s where %s = %d", 
		F_CUSER_CUSERID, T_CUSER, F_CUSER_CUSERTYPE, CUSER_TYPE_SUPERADMIN);
	if (ExecSql(tSql, enExecMayRet) && IsNotEof())
	{
		strUserId = GetString(F_CUSER_CUSERID);
		return CMS_SUCCESS;
	}
	else
	{
		return ERR_TAS_DBERROR;
	}
}

/*
 * 查询指定组的所有下级组（包括指定组）
 */
ENTasErrno CDbOpr::GetGroupsByGroupId(const UUIDString &strGroupId, vector<UUIDString> &vctGroups,const string& devTreeId/*=""*/)
{
	TSqlString tSql;
	tSql.Format("call %s('%s','%s', %d, %d)", P_DEVGRPQRY,DbStr(strGroupId),DbStr(devTreeId), 0, ENTRY_MAX_NUM_ONE_PAGE);


	if (!ExecSql(tSql, enExecMayRet))
	{
		return ERR_TAS_DBERROR;
	}

	while(IsNotEof())
	{
		// 保存好所有需要处理的设备组，这里先保存，而不是一边获取数据，一边处理，是为了在处理前统计出需要处理的数量
		vctGroups.push_back(GetString(F_DEVGROUP_DEVGROUPID));

		MoveNext();
	}

	return CMS_SUCCESS;
}

ENTasErrno CDbOpr::GetCuserDevGroupAddInfo(const string &strGroupId, const string &strOperId, const string &strOwnerId, 
	map<UUIDString, CUserDeviceInfo> &cInfos)
{
	// P_CUSERDEVCAPWITHOUTASSIGNQRY存储过程是带过滤功能的，但是这里不能进行过滤，
	// 因为已经划归过的设备再次进行划归时，可能权限不同，需要再次处理，不能跳过
	TSqlString tSql;
	tSql.Format("call %s('%s', '%s', '%s', %d)", P_CUSERDEVCAPWITHOUTASSIGNQRY, strOperId.c_str(), strGroupId.c_str(), string().c_str(), 0);
	if (!ExecSql(tSql, enExecMayRet))
	{
		return ERR_TAS_DBERROR;
	}

	// 将查询出来的结果转换为固定的接口
	while(IsNotEof())
	{
		string strDevId = GetString(F_CUSER2DEVICECAP_DEVID);
		CUserDeviceInfo &cItem = cInfos[strDevId];
		if (cItem.GetDeviceID().empty())
		{
			cItem.SetDeviceID(strDevId);
			cItem.SetDevDomainID(GetString(F_DEVICE_DOMAINID));
			cItem.SetUserID(strOwnerId);
			cItem.GetDevicePrivilege().ConvertFromInt(-1);		// 设备权限没有意义，默认为所有权限
		}

		// 具体的权限信息
		s32 nCapId    = GetInt(F_CUSER2DEVICECAP_DEVCAPID);
		s32 nCapIndex = GetInt(F_CUSER2DEVICECAP_DEVCAPINDEX);
		CBitsContainer cDeviceCapPrivilege;
		cDeviceCapPrivilege.ConvertFromInt(GetInt(F_CUSER2DEVICECAP_DEVCAPINDEXPRIVILEGE));

		vector<TDeviceCapPrivilege> &vctDeviceCap = cItem.GetDeviceCapPrivilege();
		vector<TDeviceCapPrivilege>::iterator it = vctDeviceCap.begin();
		for (; vctDeviceCap.end() != it; ++it)
		{
			if (it->id == nCapId)
			{
				break;
			}
		}

		if (vctDeviceCap.end() != it)							// 找到对应设备能力的划归信息
		{
			it->privileges.insert(pair<int, CBitsContainer>(nCapIndex, cDeviceCapPrivilege));
		}
		else
		{
			TDeviceCapPrivilege tDeviceCapItem;
			tDeviceCapItem.id = nCapId;
			tDeviceCapItem.privileges.insert(pair<int, CBitsContainer>(nCapIndex, cDeviceCapPrivilege));

			vctDeviceCap.push_back(tDeviceCapItem);
		}

		if (!MoveNext())
		{
			return ERR_TAS_DBERROR;
		}
	}

	return CMS_SUCCESS;
}

ENTasErrno CDbOpr::GetCuserDevGroupDelInfo(const string &strGroupId, const string &strOperId, const string &strOwnerId, 
	map<UUIDString, CUserDeviceDelInfo> &cInfos)
{
	// P_CUSERDEVCAPALREADYASSIGNQRY带过滤功能，这里过滤出所有已经划归的设备
	TSqlString tSql;
	tSql.Format("call %s('%s', '%s', '%s', %d)", P_CUSERDEVCAPALREADYASSIGNQRY, strOperId.c_str(), strGroupId.c_str(), strOwnerId.c_str(), 0);
	if (!ExecSql(tSql, enExecMayRet))
	{
		return ERR_TAS_DBERROR;
	}

	// 将查询出来的结果转换为固定的接口
	while(IsNotEof())
	{
		string strDevId = GetString(F_CUSER2DEVICECAP_DEVID);
		CUserDeviceDelInfo &cItem = cInfos[strDevId];
		if (cItem.key.GetDeviceID().empty())
		{
			cItem.key.SetDeviceID(strDevId);
			cItem.key.SetUserID(strOwnerId);
			cItem.domainId = GetString(F_DEVICE_DOMAINID);
		}

		// 具体的权限信息
		s32 nCapId    = GetInt(F_CUSER2DEVICECAP_DEVCAPID);
		s32 nCapIndex = GetInt(F_CUSER2DEVICECAP_DEVCAPINDEX);

		vector<TDeviceCapIndexs> &vctDeviceCap = cItem.key.GetDeviceCapIndexs();
		vector<TDeviceCapIndexs>::iterator it = vctDeviceCap.begin();
		for (; vctDeviceCap.end() != it; ++it)
		{
			if (it->id == nCapId)
			{
				break;
			}
		}

		if (vctDeviceCap.end() != it)							// 找到对应设备能力的划归信息
		{
			it->indexs.push_back(nCapIndex);
		}
		else
		{
			TDeviceCapIndexs tDeviceCapItem;
			tDeviceCapItem.id = nCapId;
			tDeviceCapItem.indexs.push_back(nCapIndex);

			vctDeviceCap.push_back(tDeviceCapItem);
		}

		if (!MoveNext())
		{
			return ERR_TAS_DBERROR;
		}
	}

	return CMS_SUCCESS;
}

ENTasErrno CDbOpr::GetCuserGroupDevGroupAddInfo(const string &strGroupId, const string &strOperId, const string &strOwnerId, 
	map<UUIDString, CUserGroupDeviceInfo> &cInfos)
{
	// P_CUSERDEVCAPWITHOUTASSIGNQRY存储过程是带过滤功能的，但是这里不能进行过滤，
	// 因为已经划归过的设备再次进行划归时，可能权限不同，需要再次处理，不能跳过
	TSqlString tSql;
	tSql.Format("call %s('%s', '%s', '%s', %d)", P_CUSERDEVCAPWITHOUTASSIGNQRY, strOperId.c_str(), strGroupId.c_str(), string().c_str(), 1);
	if (!ExecSql(tSql, enExecMayRet))
	{
		return ERR_TAS_DBERROR;
	}

	// 将查询出来的结果转换为固定的接口
	while(IsNotEof())
	{
		string strDevId = GetString(F_CUSER2DEVICECAP_DEVID);
		CUserGroupDeviceInfo &cItem = cInfos[strDevId];
		if (cItem.GetDeviceID().empty())
		{
			cItem.SetDeviceID(strDevId);
			cItem.SetDevDomainID(GetString(F_DEVICE_DOMAINID));
			cItem.SetGroupID(strOwnerId);
			cItem.GetDevicePrivilege().ConvertFromInt(-1);		// 设备权限没有意义，默认为所有权限
		}

		// 具体的权限信息
		s32 nCapId    = GetInt(F_CUSER2DEVICECAP_DEVCAPID);
		s32 nCapIndex = GetInt(F_CUSER2DEVICECAP_DEVCAPINDEX);
		CBitsContainer cDeviceCapPrivilege;
		cDeviceCapPrivilege.ConvertFromInt(GetInt(F_CUSER2DEVICECAP_DEVCAPINDEXPRIVILEGE));

		vector<TDeviceCapPrivilege> &vctDeviceCap = cItem.GetDeviceCapPrivilege();
		vector<TDeviceCapPrivilege>::iterator it = vctDeviceCap.begin();
		for (; vctDeviceCap.end() != it; ++it)
		{
			if (it->id == nCapId)
			{
				break;
			}
		}

		if (vctDeviceCap.end() != it)							// 找到对应设备能力的划归信息
		{
			it->privileges.insert(pair<int, CBitsContainer>(nCapIndex, cDeviceCapPrivilege));
		}
		else
		{
			TDeviceCapPrivilege tDeviceCapItem;
			tDeviceCapItem.id = nCapId;
			tDeviceCapItem.privileges.insert(pair<int, CBitsContainer>(nCapIndex, cDeviceCapPrivilege));

			vctDeviceCap.push_back(tDeviceCapItem);
		}

		if (!MoveNext())
		{
			return ERR_TAS_DBERROR;
		}
	}

	return CMS_SUCCESS;
}

ENTasErrno CDbOpr::GetCuserGroupDevGroupDelInfo(const string &strGroupId, const string &strOperId, const string &strOwnerId, 
	map<UUIDString, CUserGroupDeviceDelInfo> &cInfos)
{
	// P_CUSERDEVCAPALREADYASSIGNQRY带过滤功能，这里过滤出所有已经划归的设备
	TSqlString tSql;
	tSql.Format("call %s('%s', '%s', '%s', %d)", P_CUSERDEVCAPALREADYASSIGNQRY, strOperId.c_str(), strGroupId.c_str(), strOwnerId.c_str(), 1);
	if (!ExecSql(tSql, enExecMayRet))
	{
		return ERR_TAS_DBERROR;
	}

	// 将查询出来的结果转换为固定的接口
	while(IsNotEof())
	{
		string strDevId = GetString(F_CUSER2DEVICECAP_DEVID);
		CUserGroupDeviceDelInfo &cItem = cInfos[strDevId];
		if (cItem.key.GetDeviceID().empty())
		{
			cItem.key.SetDeviceID(strDevId);
			cItem.key.SetGroupID(strOwnerId);
			cItem.domainId = GetString(F_DEVICE_DOMAINID);
		}

		// 具体的权限信息
		s32 nCapId    = GetInt(F_CUSER2DEVICECAP_DEVCAPID);
		s32 nCapIndex = GetInt(F_CUSER2DEVICECAP_DEVCAPINDEX);

		vector<TDeviceCapIndexs> &vctDeviceCap = cItem.key.GetDeviceCapIndexs();
		vector<TDeviceCapIndexs>::iterator it = vctDeviceCap.begin();
		for (; vctDeviceCap.end() != it; ++it)
		{
			if (it->id == nCapId)
			{
				break;
			}
		}

		if (vctDeviceCap.end() != it)							// 找到对应设备能力的划归信息
		{
			it->indexs.push_back(nCapIndex);
		}
		else
		{
			TDeviceCapIndexs tDeviceCapItem;
			tDeviceCapItem.id = nCapId;
			tDeviceCapItem.indexs.push_back(nCapIndex);

			vctDeviceCap.push_back(tDeviceCapItem);
		}

		if (!MoveNext())
		{
			return ERR_TAS_DBERROR;
		}
	}

	return CMS_SUCCESS;
}

bool CDbOpr::Init()
{
    s_pcLock4CuserExportDevData = new CXSemLock();
    if (s_pcLock4CuserExportDevData == NULL)
    {
        return false;
    }

    return true;
}

void CDbOpr::Quit()
{
    delete s_pcLock4CuserExportDevData;
    s_pcLock4CuserExportDevData = NULL;
}

void CDbOpr::CuDataVersionGet( const CTaskMsgTransfer<CCuserDataVersionGetReq> *pcMsg )
{
    CTaskMsgTransfer<CCuserDataVersionGetRsp> cTrans;
    CCuserDataVersionGetRsp &cRsp = cTrans.GetMsgObj();

    const CCuserDataVersionGetReq &cReq = pcMsg->GetMsgObj();

    do 
    {
        string strCuserId;
        if (!GetCurrentCuserId(pcMsg->GetMsgObj().GetSession(), strCuserId))
        {
            cRsp.SetErrorCode(ERR_TAS_CURRENT_USER_NOT_EXIST);
            break;
        }

        CUserInfo cUser;
        if (!GetCuserInfo(strCuserId, cUser))
        {
            cRsp.SetErrorCode(ERR_TAS_USER_NOT_EXIST);
            break;
        }


		cRsp.SetDataVersion(cUser.GetDeviceTreeVer());
		
        if (!cUser.GetUsergrpID().empty())
        {
            CUserGroupInfo cGrpInfo;
            if (!GetCuserGrpInfo(cUser.GetUsergrpID(), cGrpInfo))
            {
                cRsp.SetErrorCode(ERR_TAS_RECORD_FETCH_FAILED);
                break;
            }
			if (!cGrpInfo.GetDeviceTreeVer().empty())
			{
				  cRsp.SetDataVersion(cGrpInfo.GetDeviceTreeVer());
			}
        }


    } while (false);

    PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
}

ENTasErrno CDbOpr::IsDevCapPrivilegeVailed( const vector<TDeviceCapPrivilege>& cDevCapPrivis )
{
	CVideosourcePrivilege tmpPrivilege;
	u32 i;
	for (i = 0; i < cDevCapPrivis.size(); ++i)
	{
		const TDeviceCapPrivilege &tCap = cDevCapPrivis[i];
		if (DEVICE_CAP_VIDEOSOURCE==tCap.id)
		{
			map<int, CBitsContainer>::const_iterator it;
			for (it = tCap.privileges.begin(); it != tCap.privileges.end(); ++it)
			{
				tmpPrivilege.ConvertFromBitsContainer(it->second);
				if (0==tmpPrivilege.GetPTZCtrl()&&tmpPrivilege.HasPrivilege(CVideosourcePrivilege::en_PrePos_Set))
				{
					CLASSLOG(enLogSrcDbTaskPool, ERROR_LEV, "IsDevCapPrivilegeVailed not have ptzctrl but have preposset prvilege!\n");
					return ERR_TAS_INVALID_PARM;
				}

			}
		}
	}

	return CMS_SUCCESS;
}

ENTasErrno CDbOpr::DevGrpDel( const UUIDString &grpId, const UUIDString &parentGrpId,const string &devTreeId )
{
	ENTasErrno enErr=CMS_SUCCESS;

	enErr =  MoveupDevGrpDevs(grpId,parentGrpId,devTreeId);
	if (CMS_SUCCESS != enErr)
	{
		return enErr;
	}
	
	enErr =  DevGrpItemDel(grpId,devTreeId);
	if (CMS_SUCCESS != enErr)
	{
		return enErr;
	}
	


	return enErr;
}

void CDbOpr::DevTreeAdd( const CTaskMsgTransfer<CDevTreeAddReq> *pcMsg )
{
	CTaskMsgTransfer<CDevTreeAddRsp> cTrans;
	CDevTreeAddRsp &cRsp = cTrans.GetMsgObj();


	const CDevTreeAddReq &cReq = pcMsg->GetMsgObj();

	bool bHasRsped = false;
	do 
	{
		const string &devtreeName = cReq.GetDevtreeName();

        if (devtreeName.empty() || cReq.GetType()==CDevTreeInfo::UserFavorites)
		{
			cRsp.SetErrorCode(ERR_TAS_INVALID_PARM);
			break;
		}

		TSqlString tSql;
		tSql.Format("select count(*) from %s where %s=%d", T_DEVTREE, F_DEVTREE_TYPE, (s32)CDevTreeInfo::SystemCustomTree);
		if (!ExecSql(tSql, enExecMustRet))
		{
			cRsp.SetErrorCode(ERR_TAS_DBERROR);
			break;
		}
		if (GetIntResult() >= 20) // 业务上限制最多只能创建20课系统树 [4/21/2016 pangubing] 8.28由10改为20
		{
			cRsp.SetErrorCode(ERR_TAS_DEVTREE_TOTALNUM_TOO_MUCH);
			break;
		}


		tSql.Format("select count(*) from %s where %s='%s'", T_DEVTREE, 
			F_DEVTREE_NAME, DbStr(devtreeName));
		if (!ExecSql(tSql, enExecMustRet))
		{
			cRsp.SetErrorCode(ERR_TAS_DBERROR);
			break;
		}
		if (GetIntResult() > 0)
		{
			cRsp.SetErrorCode(ERR_TAS_RECORD_EXISTS);
			break;
		}

		//拷贝表校验源设备树层级树，如果目的为可用于选择推送，则源设备树层级不允许超过4层
		if (cReq.GetCanBeUsed4Gb() && cReq.GetCopyFromOtherTree())
		{
			tSql.Format("select max(length(%s)) from %s ", F_DEVGROUP_FULLPATH, DbStr(GetTblDevGrpName(cReq.GetSrcTreeId())));
			if (ExecSql(tSql, enExecMustRet))
			{
				if (GetIntResult() >= (GetMaxCivilcodeLev()*32)) //即分组层级多于4层
				{
					cRsp.SetErrorCode(ERR_TAS_SRCTREE_HIERARCHY_TOO_DEEP);
					break;
				}
			}
			else
			{
				cRsp.SetErrorCode(ERR_TAS_DBERROR);
				break;
			}

			//原设备树任意分组子分组个数不能超过99个（单层行政区划有效个数为99）
			tSql.Format("select max(a.f1) from  (select count(fullpath) as  f1 from %s group by fullpath) a",
				DbStr(GetTblDevGrpName(cReq.GetSrcTreeId())));
			if (!ExecSql(tSql, enExecMustRet))
			{
				cRsp.SetErrorCode(ERR_TAS_DBERROR);
				break;
			}
			else
			{
				if (GetIntResult() > 99)
				{
					cRsp.SetErrorCode(ERR_TAS_SRCTREE_HIERARCHY_TOO_DEEP);
					break;
				}
			}
		}


		CUUID devtreeId,operId;
		cRsp.SetOperUuid(operId);
		CCMSTime tTimeVer;
        tSql.Format("insert into %s values('%s', '%s', '%s', %d, %d)", 
			T_DEVTREE, DbStr(devtreeId), DbStr(devtreeName),
			DbStr(tTimeVer.ToString()), cReq.GetCanBeUsed4Gb(), (s32)cReq.GetType());
        if (ExecSql(tSql, enExecNoRet))
		{
			cRsp.SetDevtreeId(devtreeId);
		}
		else
		{
			cRsp.SetErrorCode(ERR_TAS_DBERROR);
			break;
		}
		g_cDbTaskPool.AddTransProgress(operId, 100, 5);
		// 创建设备组表及设备组设备关系表 [3/10/2016 pangubing]
		tSql.Format(cReq.GetCanBeUsed4Gb()?s_szDevGroupTableTmplGb:s_szDevGroupTableTmpl, DbStr(devtreeId));
		if (!ExecSql(tSql, enExecNoRet))
		{
			cRsp.SetErrorCode(ERR_TAS_DBERROR);
			break;
		}

		tSql.Format(cReq.GetCanBeUsed4Gb()?s_szDevGrp2DevTableTmplGb:s_szDevGrp2DevTableTmpl, DbStr(devtreeId));
		if (!ExecSql(tSql, enExecNoRet))
		{
			cRsp.SetErrorCode(ERR_TAS_DBERROR);
			break;
		}
		g_cDbTaskPool.AddTransProgress(operId, 10);
		if (cReq.GetCopyFromOtherTree())
		{
			bHasRsped = true;
			PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
			tSql.Format("call %s('%s', '%s')", P_COPYDEVTREE, DbStr(cReq.GetSrcTreeId()), DbStr(devtreeId));
			if (!ExecSql(tSql, enExecNoRet))
			{
				g_cDbTaskPool.AddTransProgress(operId,15, ERR_TAS_DBERROR);//业务线程CDevtreeAddTask依赖此数据判断拷贝是否完成
				cRsp.SetErrorCode(ERR_TAS_DBERROR);
				break;
			}
			g_cDbTaskPool.AddTransProgress(operId,15);//业务线程CDevtreeAddTask依赖此数据判断拷贝是否完成
		}
		else
		{
			// 插入自定义设备树根组，组ID和树ID一致 [3/10/2016 pangubing]
			CDeviceGroupInfo cRootGrp;
			DevGrpItemQry(CurrentDomainId(), cRootGrp);		

			CDeviceGroupInfo cInfo;
			CCMSTime cNowTime;
			cInfo.SetDeviceGroupID(devtreeId);
			cInfo.SetDomainUUID(CurrentDomainId());
			cInfo.SetDeviceGroupName(cRootGrp.GetDeviceGroupName());
			cInfo.SetCreateDate(cNowTime.ToString());
			cRsp.SetErrorCode(DevGrpItemAdd(cInfo,devtreeId));
		}
		
		if (CMS_SUCCESS != cRsp.GetErrorCode())
		{
			g_cDbTaskPool.AddTransProgress(operId,-1);
			break;
		}
		else
		{
			UpdateTreesVerByDevId("");
		}


	} while (false);
	if (!bHasRsped)
	{
		PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
	}
}

void CDbOpr::DevTreeDel( const CTaskMsgTransfer<CDevTreeDelReq> *pcMsg )
{
	CTaskMsgTransfer<CDevTreeDelRsp> cTrans;
	CDevTreeDelRsp &cRsp = cTrans.GetMsgObj();


	const CDevTreeDelReq &cReq = pcMsg->GetMsgObj();

	do 
	{
		TSqlString tSql;
		UUIDString cdevtreeId = cReq.GetDevtreeId();
		CDevTreeInfo cDevTreeInfo;

        // 禁止删除系统设备树
        if (cdevtreeId.empty())
        {
            cRsp.SetErrorCode(ERR_TAS_INVALID_OPERATION);
            break;
        }

		if (!GetDevTreeInfo(cdevtreeId, cDevTreeInfo))
		{
			cRsp.SetErrorCode(ERR_TAS_RECORD_NOT_EXISTS);
			break;
		}

		// 删除设备组表及设备组设备关系表 [3/10/2016 pangubing]
		tSql.Format("drop table %s",  DbStr(GetTblDevGrp2DevName(cdevtreeId)));
		if (!ExecSql(tSql, enExecNoRet))
		{
			cRsp.SetErrorCode(ERR_TAS_DBERROR);
			break;
		}

		tSql.Format("drop table %s",  DbStr(GetTblDevGrpName(cdevtreeId)));
		if (!ExecSql(tSql, enExecNoRet))
		{
			cRsp.SetErrorCode(ERR_TAS_DBERROR);
			break;
		}

		// 更新用户表 [3/10/2016 pangubing]
		tSql.Format("update %s set %s = '' where %s = '%s'", T_CUSER, F_CUSER_DEVTREEID,F_CUSER_DEVTREEID, DbStr(cdevtreeId));
		if (!ExecSql(tSql, enExecNoRet))
		{
			cRsp.SetErrorCode(ERR_TAS_DBERROR);
			break;
		}

		tSql.Format("delete from %s where %s = '%s'", T_DEVTREE, F_DEVTREE_ID,  DbStr(cdevtreeId));
		if (!ExecSql(tSql, enExecNoRet))
		{
			cRsp.SetErrorCode(ERR_TAS_DBERROR);
		}
		else
		{
			UpdateTreesVerByDevId("");
		}

	} while (false);

	PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);

}

void CDbOpr::DevTreeMod( const CTaskMsgTransfer<CDevTreeModReq> *pcMsg )
{
	CTaskMsgTransfer<CDevTreeModRsp> cTrans;
	CDevTreeModRsp &cRsp = cTrans.GetMsgObj();

	const CDevTreeModReq &cReq = pcMsg->GetMsgObj();

	do 
	{
		TSqlString tSql;
		UUIDString cdevtreeId = cReq.GetDevtreeId();
		CDevTreeInfo cDevTreeInfo;

		if (!GetDevTreeInfo(cdevtreeId, cDevTreeInfo))
		{
			cRsp.SetErrorCode(ERR_TAS_RECORD_NOT_EXISTS);
			break;
		}

		const string& newName = cReq.GetDevtreeName();
		if (newName.empty()||newName==cDevTreeInfo.GetDevtreeName())
		{
			cRsp.SetErrorCode(ERR_TAS_INVALID_PARM);
			break;
		}

		//是否重名
		tSql.Format("select count(*) from %s where %s='%s'", T_DEVTREE, F_DEVTREE_NAME, DbStr(newName));
		if (!ExecSql(tSql, enExecMustRet))
		{
			cRsp.SetErrorCode(ERR_TAS_DBERROR);
			break;
		}
		if (GetIntResult() > 0)
		{
			cRsp.SetErrorCode(ERR_TAS_RECORD_EXISTS);
			break;
		}

		tSql.Format("update %s set %s='%s' ", T_DEVTREE, F_DEVTREE_NAME, DbStr(newName));
		tSql.AppendString(" where %s = '%s'", F_DEVTREE_ID, DbStr(cdevtreeId));
		if (ExecSql(tSql, enExecNoRet))
		{
			UpdateTreesVerByDevId("");
		}
		else
		{
			cRsp.SetErrorCode(ERR_TAS_DBERROR);
		}

	} while (false);

	PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);

}

void CDbOpr::DevTreeQry( const CTaskMsgTransfer<CDevTreeQryReq> *pcMsg )
{
	DatabaseQueryTemplate<CDevTreeQryReq, CDevTreeQryRsp, CDevTreeQryNtfReq, CDevTreeInfo>(
		pcMsg, &CDbOpr::PrepareDevTreeQrySql, &CDbOpr::FetchDevTreeQryRecord, 
		&CDevTreeQryNtfReq::GetInfos, &CDevTreeInfo::GetDevtreeId);
}

void CDbOpr::DevGrpGbidSet(const CDbMsgTransfer<TSetDevGrpGbidReq> *pcMsg )
{
	CDbMsgTransfer<TSetDevGrpGbidRsp> cTrans(*pcMsg, msgSetDevGrpGbidRsp);
	TSetDevGrpGbidRsp &cRsp = cTrans.GetMsgObj();

	const TSetDevGrpGbidReq &cReq = pcMsg->GetMsgObj();

	do 
	{
		if (cReq.devTreeId.empty())
		{
			cRsp.SetErrno(ERR_TAS_INVALID_OPERATION); //系统设备树不允许如此操作
			break;
		}
		TSqlString tSql;

		tSql.Format("select count(*) from %s where %s='%s'",
			DbStr(GetTblDevGrpName(cReq.devTreeId)),
			F_DEVGROUP_DEVGROUPID, DbStr(cReq.devGrpId));
		if (!ExecSql(tSql, enExecMustRet))
		{
			cRsp.SetErrno(ERR_TAS_DBERROR);
			break;
		}
		if (0 == GetIntResult())
		{
			cRsp.SetErrno(ERR_TAS_RECORD_NOT_EXISTS);
			break;
		}
		tSql.Format("select %s,%s from %s where %s='%s'", 
			F_DEVGROUP_OLDGBID, F_DEVGROUP_PARENTDEVGROUPID, DbStr(GetTblDevGrpName(cReq.devTreeId)),
			F_DEVGROUP_DEVGROUPID, DbStr(cReq.devGrpId));
		if (!ExecSql(tSql,enExecMayRet))
		{
			cRsp.SetErrno(ERR_TAS_DBERROR);
			break;
		}
		string strOldGbid=string(GetString(F_DEVGROUP_OLDGBID));
		string strParentGrpId = string(GetString(F_DEVGROUP_PARENTDEVGROUPID));

		//校验civilcode是否合法
		string strGrpGbid = cReq.devGrpGbid;
		if (IsDevGrpCivilCode(strGrpGbid) && !CheckGbidFit(cReq.devTreeId, cReq.devGrpId, strGrpGbid, strParentGrpId))
		{
			cRsp.SetErrno(GetNextDevGrpCivilCode(cReq.devTreeId, strParentGrpId, strGrpGbid));
			if (CMS_SUCCESS != cRsp.GetErrno())
			{
				break;
			}
		}
		if (strGrpGbid.empty())
		{
			cRsp.SetErrno(ERR_TAS_INVALID_OPERATION);
			break;
		}
		if (strOldGbid.empty())
		{
			tSql.Format("update %s set %s='%s', %s='%s' where %s='%s'",
				DbStr(GetTblDevGrpName(cReq.devTreeId)), 
				F_DEVGROUP_OLDGBID,DbStr(strGrpGbid),
				F_DEVGROUP_NEWGBID, DbStr(strGrpGbid),
				F_DEVGROUP_DEVGROUPID, DbStr(cReq.devGrpId));
		}
		else
		{
			tSql.Format("update %s set %s='%s' where %s='%s'",
				DbStr(GetTblDevGrpName(cReq.devTreeId)), 
				F_DEVGROUP_NEWGBID, DbStr(strGrpGbid),
				F_DEVGROUP_DEVGROUPID, DbStr(cReq.devGrpId));
		}
		if (!ExecSql(tSql, enExecNoRet))
		{
			cRsp.SetErrno(ERR_TAS_DBERROR);
			break;
		}

		UpdateDevTreeVer("", cReq.devTreeId);
	} while (false);

	PostDbMsgTransferFromDbTaskPool(cTrans);
	
}

void CDbOpr::DevGbidSet( const CDbMsgTransfer<TSetDevGbidReq> *pcMsg )
{
	CDbMsgTransfer<TSetDevGbidRsp> cTrans(*pcMsg, msgSetDevGbidRsp);
	TSetDevGbidRsp &cRsp = cTrans.GetMsgObj();

	const TSetDevGbidReq &cReq = pcMsg->GetMsgObj();

	do 
	{
		if (cReq.devTreeId.empty() || cReq.devGrpId.empty())
		{
			cRsp.SetErrno(ERR_TAS_INVALID_OPERATION); //系统设备树不允许如此操作
			break;
		}
		CDevTreeInfo cDevTreeInfo;
		if(!GetDevTreeInfo(cReq.devTreeId, cDevTreeInfo))
		{
			cRsp.SetErrno(ERR_TAS_RECORD_NOT_EXISTS);
			break;
		}
		if (!cDevTreeInfo.GetCanBeUsed4Gb())
		{
			cRsp.SetErrno(ERR_TAS_INVALID_OPERATION);
			break;
		}

		string strPri,strSec;
		TSqlString tSql;
		if (!cReq.DevGbids.empty())
		{
			for (vector<TDevGbId> ::const_iterator it = cReq.DevGbids.begin(); it != cReq.DevGbids.end(); ++it)
			{
				if (0 == it->operType) //仅修改主流
				{
					tSql.Format("select %s from %s where %s='%s' and %s='%s' and %s=%d and %s=%d",
						F_DEVGRP2DEV_PRIMARYOLDGBID,DbStr(GetTblDevGrp2DevName(cReq.devTreeId)),
						F_DEVGRP2DEV_DEVGROUPID,DbStr(cReq.devGrpId),
						F_DEVGRP2DEV_DEVID,DbStr(it->devId),
						F_DEVGRP2DEV_DEVCAPID,1,
						F_DEVGRP2DEV_DEVCAPINDEX,it->vidSrcIdx);
					if (ExecSql(tSql,enExecMayRet))
					{
						if (IsNotEof())
						{
							strPri = GetString(F_DEVGRP2DEV_PRIMARYOLDGBID);
						}
						else
						{
							continue;
						}
					}

					if (strPri.empty())
					{
						tSql.Format("update %s set %s='%s',%s='%s' where %s='%s' and  %s='%s' and %s=%d and %s=%d",
						DbStr(GetTblDevGrp2DevName(cReq.devTreeId)),
						F_DEVGRP2DEV_PRIMARYOLDGBID,DbStr(it->priGbid),
						F_DEVGRP2DEV_PRIMARYNEWGBID,DbStr(it->priGbid),
						F_DEVGRP2DEV_DEVGROUPID,DbStr(cReq.devGrpId),
						F_DEVGRP2DEV_DEVID,DbStr(it->devId),
						F_DEVGRP2DEV_DEVCAPID,1,
						F_DEVGRP2DEV_DEVCAPINDEX,it->vidSrcIdx);
					}
					else 
					{
						tSql.Format("update %s set %s='%s' where %s='%s' and %s='%s' and %s=%d and %s=%d",
						DbStr(GetTblDevGrp2DevName(cReq.devTreeId)),
						F_DEVGRP2DEV_PRIMARYNEWGBID,DbStr(it->priGbid),
						F_DEVGRP2DEV_DEVGROUPID,DbStr(cReq.devGrpId),
						F_DEVGRP2DEV_DEVID,DbStr(it->devId),
						F_DEVGRP2DEV_DEVCAPID,1,
						F_DEVGRP2DEV_DEVCAPINDEX,it->vidSrcIdx);
					}
				}
				else if (1 == it->operType) //仅修改辅流
				{
					tSql.Format("select %s from %s where %s='%s' and %s='%s' and %s=%d and %s=%d",
						F_DEVGRP2DEV_SECONDARYOLDGBID,DbStr(GetTblDevGrp2DevName(cReq.devTreeId)),
						F_DEVGRP2DEV_DEVGROUPID,DbStr(cReq.devGrpId),
						F_DEVGRP2DEV_DEVID,DbStr(it->devId),
						F_DEVGRP2DEV_DEVCAPID,1,
						F_DEVGRP2DEV_DEVCAPINDEX,it->vidSrcIdx);
					if (ExecSql(tSql,enExecMayRet))
					{
						if (IsNotEof())
						{
							strSec = GetString(F_DEVGRP2DEV_SECONDARYOLDGBID);
						}
						else
						{
							continue;
						}
						
					}
					if (strSec.empty())
					{
						tSql.Format("update %s set %s='%s',%s='%s' where %s='%s' and %s='%s' and %s=%d and %s=%d",
						DbStr(GetTblDevGrp2DevName(cReq.devTreeId)),
						F_DEVGRP2DEV_SECONDARYOLDGBID,DbStr(it->secGbid),
						F_DEVGRP2DEV_SECONDARYNEWGBID,DbStr(it->secGbid),
						F_DEVGRP2DEV_DEVGROUPID,DbStr(cReq.devGrpId),
						F_DEVGRP2DEV_DEVID,DbStr(it->devId),
						F_DEVGRP2DEV_DEVCAPID,1,
						F_DEVGRP2DEV_DEVCAPINDEX,it->vidSrcIdx);
					}
					else 
					{
						tSql.Format("update %s set %s='%s' where %s='%s' and %s='%s' and %s=%d and %s=%d",
						DbStr(GetTblDevGrp2DevName(cReq.devTreeId)),
						F_DEVGRP2DEV_SECONDARYNEWGBID,DbStr(it->secGbid),
						F_DEVGRP2DEV_DEVGROUPID,DbStr(cReq.devGrpId),
						F_DEVGRP2DEV_DEVID,DbStr(it->devId),
						F_DEVGRP2DEV_DEVCAPID,1,
						F_DEVGRP2DEV_DEVCAPINDEX,it->vidSrcIdx);
					}
				}
				else if (2 == it->operType) //主辅流都修改
				{
					tSql.Format("select %s, %s from %s where %s='%s' and %s='%s' and %s=%d and %s=%d",
						F_DEVGRP2DEV_PRIMARYOLDGBID,F_DEVGRP2DEV_SECONDARYOLDGBID,
						DbStr(GetTblDevGrp2DevName(cReq.devTreeId)),
						F_DEVGRP2DEV_DEVGROUPID,DbStr(cReq.devGrpId),
						F_DEVGRP2DEV_DEVID,DbStr(it->devId),
						F_DEVGRP2DEV_DEVCAPID,1,
						F_DEVGRP2DEV_DEVCAPINDEX,it->vidSrcIdx);
					if (ExecSql(tSql,enExecMayRet))
					{
						if (IsNotEof())
						{
							strPri = GetString(F_DEVGRP2DEV_PRIMARYOLDGBID);
							strSec = GetString(F_DEVGRP2DEV_SECONDARYOLDGBID);
						}
						else
						{
							continue;
						}
						
					}

					if (strPri.empty() && strSec.empty())
					{
						tSql.Format("update %s set %s='%s',%s='%s',%s='%s',%s='%s' where %s='%s' and %s='%s' and %s=%d and %s=%d",
						DbStr(GetTblDevGrp2DevName(cReq.devTreeId)),
						F_DEVGRP2DEV_PRIMARYOLDGBID,DbStr(it->priGbid),
						F_DEVGRP2DEV_PRIMARYNEWGBID,DbStr(it->priGbid),
						F_DEVGRP2DEV_SECONDARYOLDGBID,DbStr(it->secGbid),
						F_DEVGRP2DEV_SECONDARYNEWGBID,DbStr(it->secGbid),
						F_DEVGRP2DEV_DEVGROUPID,DbStr(cReq.devGrpId),
						F_DEVGRP2DEV_DEVID,DbStr(it->devId),
						F_DEVGRP2DEV_DEVCAPID,1,
						F_DEVGRP2DEV_DEVCAPINDEX,it->vidSrcIdx);
					}
					else if (strPri.empty() && !strSec.empty())
					{
						tSql.Format("update %s set %s='%s',%s='%s',%s='%s' where %s='%s' and %s='%s' and %s=%d and %s=%d",
							DbStr(GetTblDevGrp2DevName(cReq.devTreeId)),
							F_DEVGRP2DEV_PRIMARYOLDGBID,DbStr(it->priGbid),
							F_DEVGRP2DEV_PRIMARYNEWGBID,DbStr(it->priGbid),
							F_DEVGRP2DEV_SECONDARYNEWGBID,DbStr(it->secGbid),
							F_DEVGRP2DEV_DEVGROUPID,DbStr(cReq.devGrpId),
							F_DEVGRP2DEV_DEVID,DbStr(it->devId),
							F_DEVGRP2DEV_DEVCAPID,1,
							F_DEVGRP2DEV_DEVCAPINDEX,it->vidSrcIdx);
					}
					else if (!strPri.empty() && strSec.empty())
					{
						tSql.Format("update %s set %s='%s',%s='%s',%s='%s' where %s='%s' and %s='%s' and %s=%d and %s=%d",
							DbStr(GetTblDevGrp2DevName(cReq.devTreeId)),
							F_DEVGRP2DEV_PRIMARYNEWGBID,DbStr(it->priGbid),
							F_DEVGRP2DEV_SECONDARYOLDGBID,DbStr(it->secGbid),
							F_DEVGRP2DEV_SECONDARYNEWGBID,DbStr(it->secGbid),
							F_DEVGRP2DEV_DEVGROUPID,DbStr(cReq.devGrpId),
							F_DEVGRP2DEV_DEVID,DbStr(it->devId),
							F_DEVGRP2DEV_DEVCAPID,1,
							F_DEVGRP2DEV_DEVCAPINDEX,it->vidSrcIdx);
					}
					else if (!strPri.empty() && !strSec.empty())
					{
						tSql.Format("update %s set %s='%s',%s='%s' where %s='%s' and %s='%s' and %s=%d and %s=%d",
							DbStr(GetTblDevGrp2DevName(cReq.devTreeId)),
							F_DEVGRP2DEV_PRIMARYNEWGBID,DbStr(it->priGbid),
							F_DEVGRP2DEV_SECONDARYNEWGBID,DbStr(it->secGbid),
							F_DEVGRP2DEV_DEVGROUPID,DbStr(cReq.devGrpId),
							F_DEVGRP2DEV_DEVID,DbStr(it->devId),
							F_DEVGRP2DEV_DEVCAPID,1,
							F_DEVGRP2DEV_DEVCAPINDEX,it->vidSrcIdx);
					}
				}
				if (!ExecSql(tSql, enExecNoRet))
				{
					cRsp.SetErrno(ERR_TAS_DBERROR);
					break;
				}

				strPri.clear();
				strSec.clear();
			}
		}
		else
		{
			cRsp.SetErrno(ERR_TAS_FIELD_CONTENT_EMPTY);
			break;
		}

		UpdateDevTreeVer("",cReq.devTreeId);

	} while (false);

	PostDbMsgTransferFromDbTaskPool(cTrans);
}


bool CDbOpr::GetDevTreeInfo( const UUIDString &devtreeId, CDevTreeInfo &cInfo )
{
	TSqlString tSql;
	tSql.Format("select * from %s where %s='%s'", T_DEVTREE, F_DEVTREE_ID, DbStr(devtreeId));

	if (ExecSql(tSql, enExecMayRet))
	{
		FetchDevTreeQryRecord(cInfo);
		return true;
	}
	else
	{
		return false;
	}
}

void CDbOpr::FetchDevTreeQryRecord( CDevTreeInfo &cInfo )
{
	cInfo.SetDevtreeId(GetString(F_DEVTREE_ID));
	cInfo.SetDevtreeName(GetString(F_DEVTREE_NAME));
	cInfo.SetDevtreeVer(GetString(F_DEVTREE_VERSION));
	cInfo.SetCanBeUsed4Gb(GetInt(F_DEVTREE_CANBEUSE4GB));
    cInfo.SetType((CDevTreeInfo::TreeType)GetInt(F_DEVTREE_TYPE));
}

ENTasErrno CDbOpr::PrepareDevTreeQrySql( const CDevTreeQryReq &cReq, TSqlString &tSqlSelect, TSqlString &tSqlCount )
{
	TSqlString tSqlCond;
	TSqlString tSqlOrderby;
	tSqlSelect.Format("select a.* from %s a", T_DEVTREE);
	tSqlCount.Format("select count(*) from %s a", T_DEVTREE);

    if (cReq.GetType()!=CDevTreeInfo::AllTree)
    {
        tSqlCond.AppendString("a.%s=%d", F_DEVTREE_TYPE, (s32)cReq.GetType());

        if (cReq.GetType() != CDevTreeInfo::UserFavorites && cReq.GetIncludeFavorites())
        {
            string strUserId;
            GetCurrentCuserId(cReq.GetSession(), strUserId);
            tSqlCond.AppendString(" or (a.%s='%s' and a.%s=%d)", F_DEVTREE_ID, DbStr(strUserId),
                F_DEVTREE_TYPE, (s32)CDevTreeInfo::UserFavorites);
        }
    }

	if (!tSqlCond.Empty())
	{
		tSqlSelect.AppendString(" where ");
		tSqlSelect.AppendString("%s", tSqlCond.GetBuf());
		tSqlCount.AppendString(" where ");
		tSqlCount.AppendString("%s", tSqlCond.GetBuf());
	}

	tSqlSelect.AppendString(" order by convert(a.%s using gbk) ", F_DEVTREE_NAME);
	AddRowNumLimitModifer(tSqlSelect, cReq.GetStartEntryIndex(), cReq.GetExpectEntryNum());
	
	return CMS_SUCCESS;
}

ENTasErrno CDbOpr::DevTreeListQry( vector<CDevTreeInfo> &cInfoList )
{
	TSqlString tSql;
	tSql.Format("select * from %s", T_DEVTREE);
	if (!ExecSql(tSql, enExecMayRet))
	{
		return ERR_TAS_DBERROR;
	}
	if (IsEof())
	{
		return ERR_TAS_RECORD_NOT_EXISTS;
	}

	CDevTreeInfo cInfo;
	while (IsNotEof())
	{
		FetchDevTreeQryRecord(cInfo);
		cInfoList.push_back(cInfo);

		if (!MoveNext())
		{
			return ERR_TAS_DBERROR;
		}
	}

	return CMS_SUCCESS;
}


string  CDbOpr::GetTblDevGrpName( const string &devTreeId )
{
	return devTreeId.empty()?T_DEVGROUP:string(T_DEVGRP_PREFIX)+devTreeId;
}

string CDbOpr::GetTblDevGrp2DevName( const string &devTreeId )
{
	return devTreeId.empty()?T_DEVGRP2DEV:string(T_DEVGRP2DEV_PREFIX)+devTreeId;
}

void CDbOpr::DevTreeDevGrpAssign( const CTaskMsgTransfer<CDevTreeDevGrpAssignReq> *pcMsg )
{
	CTaskMsgTransfer<CDevTreeDevGrpAssignRsp> cTrans;
	CDevTreeDevGrpAssignRsp &cRsp = cTrans.GetMsgObj();

	const CDevTreeDevGrpAssignReq &cReq = pcMsg->GetMsgObj();
	bool bHasSendRsp = false;
	bool bNeedUpdateAllTreeVer = !cReq.GetSrcDevTreeId().empty() || !cReq.GetDstDevTreeId().empty();
	do 
	{
		TSqlString tSql;
		const UUIDString &srcGroupId = cReq.GetSrcGroupId();     //来源组ID             
		const UUIDString &srcDevTreeId = cReq.GetSrcDevTreeId();   //来源设备树ID 
		const UUIDString &dstParentGrpId = cReq.GetDstParentGrpId(); //移动目的父组ID
		const UUIDString &dstDevTreeId = cReq.GetDstDevTreeId();   //移动目的设备树ID

		if (srcGroupId.empty()||dstDevTreeId.empty())
		{
			cRsp.SetErrorCode(ERR_TAS_INVALID_PARM);
			break;
		}

		CDevTreeInfo cSrcDevTreeInfo;
		if (!srcDevTreeId.empty())
		{
			if (!GetDevTreeInfo(srcDevTreeId, cSrcDevTreeInfo))
			{
				cRsp.SetErrorCode(ERR_TAS_RECORD_NOT_EXISTS);
				break;
			}
		}

		CDeviceGroupInfo cSrcDevGrp;
		ENTasErrno enRet = DevGrpItemQryOnly(srcGroupId,cSrcDevGrp,srcDevTreeId);
		if (CMS_SUCCESS!=enRet)
		{
			cRsp.SetErrorCode(enRet);
			break;
		}


		CDevTreeInfo cDstDevTreeInfo;
		if (!GetDevTreeInfo(dstDevTreeId, cDstDevTreeInfo))
		{
			cRsp.SetErrorCode(ERR_TAS_RECORD_NOT_EXISTS);
			break;
		}


		CDeviceGroupInfo cDstParentGrp;
		enRet = DevGrpItemQryOnly(dstParentGrpId,cDstParentGrp,dstDevTreeId);
		if (CMS_SUCCESS!=enRet)
		{
			cRsp.SetErrorCode(enRet);
			break;
		}

		//目的树如果是国标选推树，则分组层级不能超过四层，且行政区划最多不能超过8位
		if (IsDevTreeCanBeUse4Gb(cReq.GetDstDevTreeId()) && cReq.GetGrpType() != TDEV_GROUP_TYPE_VIRTUALGRP)
		{
			int dwDstParentFullPathLen,dwSrcFullPathLen, dwSrcMaxFullPathLen;
			tSql.Format("select length(%s),%s from %s where %s='%s'",
				F_DEVGROUP_FULLPATH, F_DEVGROUP_NEWGBID, DbStr(GetTblDevGrpName(cReq.GetDstDevTreeId())),
				F_DEVGROUP_DEVGROUPID, DbStr(cReq.GetDstParentGrpId()));
			if (!ExecSql(tSql,enExecMustRet))
			{
				cRsp.SetErrorCode(ERR_TAS_DBERROR);
				break;
			}
			dwDstParentFullPathLen = GetIntResult();
			string strDstParentGrpGbId = GetString(F_DEVGROUP_NEWGBID);
			if (!IsDevGrpCivilCode(strDstParentGrpGbId))
			{
				cRsp.SetErrorCode(ERR_TAS_INVALID_OPERATION);
				break;
			}

			tSql.Format("select length(%s) from %s where %s='%s'",
				F_DEVGROUP_FULLPATH, DbStr(GetTblDevGrpName(cReq.GetSrcDevTreeId())),
				F_DEVGROUP_DEVGROUPID, DbStr(cReq.GetSrcGroupId()));
			if (!ExecSql(tSql,enExecMustRet))
			{
				cRsp.SetErrorCode(ERR_TAS_DBERROR);
				break;
			}
			dwSrcFullPathLen = GetIntResult();

			tSql.Format("select max(length(%s)) from %s where %s like concat('%%','%s','%%')",
				F_DEVGROUP_FULLPATH, DbStr(GetTblDevGrpName(cReq.GetSrcDevTreeId())),
				F_DEVGROUP_FULLPATH, DbStr(cReq.GetSrcGroupId()));
			if (!ExecSql(tSql,enExecMustRet))
			{
				cRsp.SetErrorCode(ERR_TAS_DBERROR);
				break;
			}
			dwSrcMaxFullPathLen = GetIntResult();
			if (dwSrcMaxFullPathLen<32)//分组拖拽到目的树上，至少会增加一层分组
			{
				dwSrcMaxFullPathLen = 32;
			}

			if (dwDstParentFullPathLen + dwSrcMaxFullPathLen-dwSrcFullPathLen>32*(GetMaxCivilcodeLev()-1))
			{
				cRsp.SetErrorCode(ERR_TAS_SRCTREE_HIERARCHY_TOO_DEEP);
				break;
			}

			//目的树的父分组civilcode长度不能超出六位。
			tSql.Format("select length(%s) from %s where %s='%s' ", 
				F_DEVGROUP_NEWGBID, DbStr(GetTblDevGrpName(cReq.GetDstDevTreeId())),
				F_DEVGROUP_DEVGROUPID, DbStr(cReq.GetDstParentGrpId()));
			if (!ExecSql(tSql,enExecMustRet))
			{
				cRsp.SetErrorCode(ERR_TAS_DBERROR);
				break;
			}
			else
			{
				int dwCivilCodeLen = GetIntResult();
				if (dwCivilCodeLen + ((dwSrcMaxFullPathLen-dwSrcFullPathLen)/32+1)*2 > 2*GetMaxCivilcodeLev())//目的树上父分组行政区划长度+源分组深度*2（每层2位）>8，认为超出限度
				{
					cRsp.SetErrorCode(ERR_TAS_SRCTREE_HIERARCHY_TOO_DEEP);
					break;
				}
			}

			//原设备树该分支任意分组的子分组个数不能超过99个（单层行政区划有效个数为99）
			tSql.Format("select %s,%s,count(%s) as TCount from %s group by %s having TCount>99 and fullpath like '%%%s%%'",
				F_DEVGROUP_PARENTDEVGROUPID, F_DEVGROUP_FULLPATH,F_DEVGROUP_PARENTDEVGROUPID,
				DbStr(GetTblDevGrpName(cReq.GetSrcDevTreeId())),F_DEVGROUP_PARENTDEVGROUPID,
				DbStr(cReq.GetSrcGroupId()));
			if (!ExecSql(tSql, enExecMayRet))
			{
				cRsp.SetErrorCode(ERR_TAS_DBERROR);
				break;
			}
			if(IsNotEof()) //以cReq.GetSrcGroupId()为根的分支下，有个分组下有超过99个子分组，目的树为国标选择推送树，无法成功处理
			{
					cRsp.SetErrorCode(ERR_TAS_SRCTREE_HIERARCHY_TOO_DEEP);
					break;
			}
			//目的分组当前子分组个数不能超过98个
			tSql.Format("select count(*) from %s where %s ='%s'",
				DbStr(GetTblDevGrpName(cReq.GetDstDevTreeId())),
				F_DEVGROUP_PARENTDEVGROUPID, DbStr(cReq.GetDstParentGrpId()));
			if (!ExecSql(tSql, enExecMustRet))
			{
				cRsp.SetErrorCode(ERR_TAS_DBERROR);
				break;
			}
			else
			{
				if (GetIntResult() >= 99)//已有99个子分组就不能再增加了
				{
					cRsp.SetErrorCode(ERR_TAS_SRCTREE_HIERARCHY_TOO_DEEP);
					break;
				}
			}

		}

		// 如果父组中有同名的，先删除同名组所有信息，再复制组过去 [3/25/2016 pangubing]
		tSql.Format("select * from %s where %s='%s' and %s='%s' ",
			DbStr(GetTblDevGrpName(dstDevTreeId)), 
			F_DEVGROUP_PARENTDEVGROUPID, DbStr(dstParentGrpId),
			F_DEVGROUP_GROUPNAME, DbStr(cSrcDevGrp.GetDeviceGroupName()));
		if ( !ExecSql(tSql, enExecMayRet))
		{
			cRsp.SetErrorCode(ERR_TAS_DBERROR);
			break;
		}

		vector<UUIDString> vctGroupsDel;
		if (IsNotEof()) // 设备组先前被添加过到该父组下面 [3/23/2016 pangubing]
		{
			CDeviceGroupInfo cDevGrp;
			//FetchDevGrpQryRecord(cDevGrp);
			cDevGrp.SetDeviceGroupID(GetString(F_DEVGROUP_DEVGROUPID));
			cDevGrp.SetDomainUUID(GetString(F_DEVGROUP_DOMAINID));
			cDevGrp.SetDeviceParentGroupID(GetString(F_DEVGROUP_PARENTDEVGROUPID));
			cDevGrp.SetDeviceGroupName(GetString(F_DEVGROUP_GROUPNAME));
			cDevGrp.SetDeviceGroupDesc(GetString(F_DEVGROUP_GROUPDESC));
			cDevGrp.SetCreateDate(GetCmsDate(F_DEVGROUP_CREATEDATE));
			cDevGrp.SetCreater(GetInt(F_DEVGROUP_CREATER));
			cDevGrp.SetOwner(GetString(F_DEVGROUP_OWNER));
			cDevGrp.SetFullpath(GetString(F_DEVGROUP_FULLPATH));
			cDevGrp.SetGrpseq(GetDouble(F_DEVGROUP_GRPSEQ));
			enRet=GetGroupsByGroupId(cDevGrp.GetDeviceGroupID(),vctGroupsDel,dstDevTreeId);
			if (CMS_SUCCESS!=enRet)
			{
				cRsp.SetErrorCode(enRet);
				break;
			}
		}
	
		//找出源设备树中所有包含源分组的子孙分组
		vector<CDeviceGroupInfo> vctGrpInfos;
		tSql.Format("select * from %s where  %s = '%s' or %s like             \
			        CONCAT((select CONCAT(%s,%s,'/') from %s where %s = '%s'),'%%') ORDER BY LENGTH(%s)",
					DbStr(GetTblDevGrpName(srcDevTreeId)),F_DEVGROUP_DEVGROUPID,DbStr(srcGroupId),F_DEVGROUP_FULLPATH,
					F_DEVGROUP_FULLPATH,F_DEVGROUP_DEVGROUPID,DbStr(GetTblDevGrpName(srcDevTreeId)),
					F_DEVGROUP_DEVGROUPID,DbStr(srcGroupId),F_DEVGROUP_FULLPATH);

		CDeviceGroupInfo cTmpGrp;
		if ( !ExecSql(tSql, enExecMayRet))
		{
			cRsp.SetErrorCode(ERR_TAS_DBERROR);
			break;
		}
		while (IsNotEof())
		{
			// 获取一条DB记录
			//FetchDevGrpQryRecord(cTmpGrp);
			cTmpGrp.SetDeviceGroupID(GetString(F_DEVGROUP_DEVGROUPID));
			cTmpGrp.SetDomainUUID(GetString(F_DEVGROUP_DOMAINID));
			cTmpGrp.SetDeviceParentGroupID(GetString(F_DEVGROUP_PARENTDEVGROUPID));
			cTmpGrp.SetDeviceGroupName(GetString(F_DEVGROUP_GROUPNAME));
			cTmpGrp.SetDeviceGroupDesc(GetString(F_DEVGROUP_GROUPDESC));
			cTmpGrp.SetCreateDate(GetCmsDate(F_DEVGROUP_CREATEDATE));
			cTmpGrp.SetCreater(GetInt(F_DEVGROUP_CREATER));
			cTmpGrp.SetOwner(GetString(F_DEVGROUP_OWNER));
			cTmpGrp.SetFullpath(GetString(F_DEVGROUP_FULLPATH));
			cTmpGrp.SetGrpseq(GetDouble(F_DEVGROUP_GRPSEQ));
			vctGrpInfos.push_back(cTmpGrp);
			if (!MoveNext())
			{
				enRet = ERR_TAS_DBERROR;
				break;
			}
		}
		if (CMS_SUCCESS!=enRet)
		{
			cRsp.SetErrorCode(enRet);
			break;
		}


		string strTransTag = CUUID();
		g_cDbTaskPool.AddTransProgress(strTransTag, vctGroupsDel.size()+vctGrpInfos.size(), 0);		// 登记初始处理进度
		// 先给客户端回应答，后续可以根据strTransTag来查询事务进度
		cRsp.SetOperUuid(strTransTag);
		PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
		bHasSendRsp = true; 

		// 删除原有的组及设备 [3/28/2016 pangubing]
		string strRootGrpId=dstDevTreeId.empty()?CurrentDomainId():dstDevTreeId;
		for (size_t i=0; i<vctGroupsDel.size(); i++)
		{
			cRsp.SetErrorCode(DevGrpDel(vctGroupsDel.at(i), strRootGrpId,dstDevTreeId));
			if (CMS_SUCCESS != cRsp.GetErrorCode())
			{
				break;
			}
			g_cDbTaskPool.AddTransProgress(strTransTag, i + 1);
		}

		 // 重新添加组到父组下面 [3/23/2016 pangubing]
		string newParentId;
		string newFullPath;
		map<string,string> mapOldGrpId2NewGrpId;
		map<string,string> mapNewGrpId2FullPath;
		map<string,string> ::iterator itMap;
		for (size_t i = 0; i < vctGrpInfos.size(); ++i)
		{
			const CDeviceGroupInfo& cInfo=vctGrpInfos.at(i);

			CUUID grpUuid;

			//先插入设备组到自定义设备树
			if (srcGroupId==cInfo.GetDeviceGroupID())
			{
				newParentId=dstParentGrpId;
				newFullPath = cDstParentGrp.GetFullpath() + cDstParentGrp.GetDeviceGroupID() +  "/";
			}
			else
			{
				itMap = mapOldGrpId2NewGrpId.find(cInfo.GetDeviceParentGroupID());
				if (itMap == mapOldGrpId2NewGrpId.end())
				{
					break;
				}
				newParentId=itMap->second;
				itMap = mapNewGrpId2FullPath.find(newParentId);
				if (itMap == mapNewGrpId2FullPath.end())
				{
					break;
				}
				newFullPath = itMap->second + newParentId + "/";
			}
			mapOldGrpId2NewGrpId[cInfo.GetDeviceGroupID()] = grpUuid;
			mapNewGrpId2FullPath[grpUuid] = newFullPath;
			
			//拖拽的分组，到了新的设备树中认为是平台内部创建的，不再认为是国标子域创建,分组名也不保留国标后缀。
			string strNewGrpName = cInfo.GetDeviceGroupName();
			if (cInfo.GetCreater() == CREATE_BY_GBS_FOREIGN_DOMAIN)
			{
				strNewGrpName = CutDevGrpGbId(strNewGrpName);
			}
			if (!cDstDevTreeInfo.GetCanBeUsed4Gb())
			{
				tSql.Format("insert into %s values('%s', '%s', '%s', '%s', '%s', '%s', %d, '','%s',%.14lf)", 
					DbStr(GetTblDevGrpName(dstDevTreeId)), DbStr(grpUuid), DbStr(CurrentDomainId()), DbStr(newParentId), 
					DbStr(strNewGrpName), DbStr(cInfo.GetDeviceGroupDesc()), DbDateTime(cInfo.GetCreateDate()), 
					(s32)(CREATE_BY_PLAT),DbStr(newFullPath),cInfo.GetGrpseq());
			}
			else
			{
				string strGbid;
				enRet = GetNextDevGrpGbid(cDstDevTreeInfo.GetDevtreeId(), newParentId, cReq.GetGrpType(), strGbid,true);
				if (CMS_SUCCESS != enRet)
				{
					break;
				}
				tSql.Format("insert into %s values('%s', '%s', '%s', '%s', '%s', '%s', %d, '','%s',%.14lf,'%s','%s')", 
					DbStr(GetTblDevGrpName(dstDevTreeId)), DbStr(grpUuid), DbStr(CurrentDomainId()), DbStr(newParentId), 
					DbStr(strNewGrpName), DbStr(cInfo.GetDeviceGroupDesc()), DbDateTime(cInfo.GetCreateDate()), 
					(s32)(CREATE_BY_PLAT),DbStr(newFullPath),cInfo.GetGrpseq(),
					DbStr(strGbid),DbStr(strGbid));
			}
			
			if (!ExecSql(tSql, enExecNoRet))
			{
				enRet = ERR_TAS_DBERROR;
				break;
			}


			//再插入设备组设备划归到自定义设备树（需补全这些设备的gbid）
			bool bDstTreeUse4Gb = cDstDevTreeInfo.GetCanBeUsed4Gb();
			vector<TVidSrcGbInfoExt> fitDevs;
			GetFitDev4Gb(dstDevTreeId, srcDevTreeId, cInfo.GetDeviceGroupID(), 
                cReq.GetOnlyOnline(), cReq.GetOnlyUnAssigned(), cReq.GetOnlyEnable(), fitDevs);
			double dDevSeq;
			if (bDstTreeUse4Gb)
			{
				for (int i =0; i<fitDevs.size(); ++i)
				{
					if(!GetDevGrpNextDevSeq(dstDevTreeId, grpUuid, fitDevs[i].devId,fitDevs[i].vidIndex,dDevSeq))
					{
						enRet = ERR_TAS_DBERROR;
						break;
					}
					tSql.Format("insert IGNORE into %s values('%s', '%s', %d, %d, '%s', '%s', '%s', '%s',%.14lf)",
							DbStr(GetTblDevGrp2DevName(dstDevTreeId)), 
							DbStr(grpUuid), DbStr(fitDevs[i].devId), DEVICE_CAP_VIDEOSOURCE, fitDevs[i].vidIndex,
							DbStr(fitDevs[i].oldPrimaryGbid),DbStr(fitDevs[i].newPrimaryGbid),
							DbStr(fitDevs[i].oldSecondaryGbid),DbStr(fitDevs[i].newSecondaryGbid),
							dDevSeq);
					if (!ExecSql(tSql, enExecNoRet))
					{
						enRet =  ERR_TAS_DBERROR;
						break;
					}
				}
			}
			else
			{
				for (int i =0; i<fitDevs.size(); ++i)
				{
					if(!GetDevGrpNextDevSeq(dstDevTreeId, grpUuid, fitDevs[i].devId,fitDevs[i].vidIndex,dDevSeq))
					{
						enRet = ERR_TAS_DBERROR;
						break;
					}
					tSql.Format("insert IGNORE into %s values('%s', '%s', %d, %d, %.14lf)",
						DbStr(GetTblDevGrp2DevName(dstDevTreeId)), 
						DbStr(grpUuid), DbStr(fitDevs[i].devId), DEVICE_CAP_VIDEOSOURCE, fitDevs[i].vidIndex,
						dDevSeq);
					if (!ExecSql(tSql, enExecNoRet))
					{
						enRet =  ERR_TAS_DBERROR;
						break;
					}
				}
			}
			if (enRet != CMS_SUCCESS)
			{
				break;
			}

			// 更新事务处理进度
			g_cDbTaskPool.AddTransProgress(strTransTag,vctGroupsDel.size() + i + 1);
		}
		g_cDbTaskPool.AddTransProgress(strTransTag, vctGroupsDel.size()+vctGrpInfos.size());

		//更新所有用户、所有用户组设备树版本
		if (bNeedUpdateAllTreeVer)
		{
			UpdateTreesVerByDevId("");
		}

	} while (false);

	if (!bHasSendRsp)
	{
		PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
	}
	
}

void CDbOpr::DeviceGrpPathQry( const CTaskMsgTransfer<CDeviceGrpPathQryReq> *pcMsg )
{
	CTaskMsgTransfer<CDeviceGrpPathQryRsp> cTrans;
	CDeviceGrpPathQryRsp &cRsp = cTrans.GetMsgObj();
	CTaskMsgTransfer<CDeviceGrpPathQryNtfReq> cTransNtf;
	CDeviceGrpPathQryNtfReq &cNtf = cTransNtf.GetMsgObj();

	const CDeviceGrpPathQryReq &cReq = pcMsg->GetMsgObj();
	TSqlString tSql, tSqlCount;

	do 
	{
		ENTasErrno enErr = PrepareDeviceGrpPathQrySql(cReq, tSql, tSqlCount);
		if (CMS_SUCCESS != enErr)
		{
			cRsp.SetErrorCode(enErr);
			break;
		}

		//先判断视频源是否存在于当前树
		TSqlString tSqlQry;
		tSqlQry.Format("select count(*) from %s where %s='%s' and %s=%d and %s=%d",
			DbStr(GetTblDevGrp2DevName(cReq.GetDevTreeId())), 
			F_DEVGRP2DEV_DEVID, DbStr(cReq.GetDeviceId()), 
			F_DEVGRP2DEV_DEVCAPID, DEVICE_CAP_VIDEOSOURCE,
			F_DEVGRP2DEV_DEVCAPINDEX, cReq.GetVideoSrcId());
		if (!ExecSql(tSqlQry, enExecMustRet))
		{
			cRsp.SetErrorCode(ERR_TAS_DBERROR);
			break;
		}
		else
		{
			if (GetIntResult()==0)
			{
				cRsp.SetTotalEntryNum(0);
				cRsp.SetErrorCode(ERR_TAS_RECORD_NOT_EXISTS);
				break;
			}
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
		bool bCanBeUse4Gb = IsDevTreeCanBeUse4Gb(cReq.GetDevTreeId());
		if (!this->ExecSql(tSql.GetBuf(), CDbOpr::enExecMayRet))
		{
			cRsp.SetErrorCode(ERR_TAS_DBERROR);
			break;
		}

		u32 dwRecordcount = 0;
		u32 RecordNumInNtf = 0;
		u32 dwNtfContentSize = 0;
		CDeviceGroupInfo cInfo;
		// 算法： 循环里面一条一条的获取DB记录 --> 1条DB记录为一条完整的业务记录 --> 组装N条业务记录为一个NTF消息; 循环之外处理最后一条消息
		while (this->IsNotEof())
		{
			cInfo.Clear();

			// 获取一条DB记录
			FetchDevGrpQryRecord(cInfo, bCanBeUse4Gb);
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
				cNtf.GetInfos().push_back(cInfo);
			}
			else
			{
				cNtf.SetEntryNum(RecordNumInNtf);
				cNtf.SetLastNtf(false);
				PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cNtf, cTransNtf);

				dwRecordcount += RecordNumInNtf;

				cNtf.GetInfos().clear();
				cNtf.GetInfos().push_back(cInfo);
				dwNtfContentSize = dwRecordContentSize;
				RecordNumInNtf = 1;
			}
		}

		// 发送组装好尚未发送的最后一个NTF消息
		if (cRsp.GetErrorCode() == CMS_SUCCESS && !(cNtf.GetInfos().empty()))
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

ENTasErrno CDbOpr::PrepareDeviceGrpPathQrySql( const CDeviceGrpPathQryReq &cReq, TSqlString &tSqlSelect, TSqlString &tSqlCount )
{
	// 查询指定设备视频源对应的设备树
	tSqlSelect.Format("call %s('%s', '%s', %d)", P_DEVGRPPATHQRY, DbStr(cReq.GetDevTreeId()), DbStr(cReq.GetDeviceId()), cReq.GetVideoSrcId());
	tSqlCount.Format("call %s('%s', '%s', %d)", P_DEVGRPPATHQRY4COUNT, DbStr(cReq.GetDevTreeId()), DbStr(cReq.GetDeviceId()), cReq.GetVideoSrcId());

	 return CMS_SUCCESS;
}

void CDbOpr::DevCapInfoMod( const CTaskMsgTransfer<CDevCapInfoModReq> *pcMsg )
{
	CTaskMsgTransfer<CDevCapInfoModRsp> cTrans;
	CTaskMsgTransfer<CDeviceModNtfReq> cTransNtf;
	CDevCapInfoModRsp &cRsp = cTrans.GetMsgObj();
	CDeviceModNtfReq &cNtf = cTransNtf.GetMsgObj();
	bool bHasNtf = false;

	const CDevCapInfoModReq &cReq = pcMsg->GetMsgObj();

	CDeviceModelInfo cOldModel, cNewModel;
    s32 nOldVidNum = 0;
    s32 nNewVidNum = 0;
    bool bNeedRollback = false;
	do 
	{
		CDeviceInfo cInfo;

		ENTasErrno enErrno = DeviceItemQry(cReq.GetDeviceUUID(), cInfo);
		if (CMS_SUCCESS != enErrno)
		{
			cRsp.SetErrorCode(enErrno);
			break;
		}

		TDevicePropertyFlags cPropertyFlags = cInfo.GetDevicePropertyFlags();;
		
		if (cInfo.GetDomainUUID() != CurrentDomainId())
		{
			cRsp.SetErrorCode(ERR_TAS_INVALID_OPERATION);
			break;
		}

		DeviceModelItemQry(cInfo.GetDeviceModel(), cOldModel);
        for (vector<TDeviceCapInfo>::const_iterator it = cOldModel.GetDeviceCapInfo().begin();
            it != cOldModel.GetDeviceCapInfo().end(); ++it)
        {
            if (it->type == DEVICE_CAP_VIDEOSOURCE)
            {
                nOldVidNum = it->num;
                break;
            }
        }
        for (vector<TDeviceCapInfo>::const_iterator it = cReq.GetDeviceModelCapDetail().begin();
            it != cReq.GetDeviceModelCapDetail().end(); ++it)
        {
            if (it->type == DEVICE_CAP_VIDEOSOURCE)
            {
                nNewVidNum = it->num;
                break;
            }
        }
        if (nNewVidNum > nOldVidNum)
        {
            cRsp.SetErrorCode(AllocManagePULicense(nNewVidNum - nOldVidNum));
            if (cRsp.GetErrorCode() != CMS_SUCCESS)
            {
                break;
            }
            bNeedRollback = true;
        }

		string strModelId;
		// 检测已存在的设备型号
		enErrno = GetExistDeviceModelId(CurrentDomainId(), cOldModel.GetDeviceTypeId(), cOldModel.GetDeviceModelName(),true,
			cOldModel.GetManufacturer(), cOldModel.GetCreater(),cReq.GetDeviceModelCapDetail(), strModelId);
		if (CMS_SUCCESS != enErrno)
		{
			cRsp.SetErrorCode(enErrno);
			break;
		}

		string strTempName = cOldModel.GetDeviceModelName();
		StrUpper((s8*)strTempName.c_str());
		if (strModelId.empty() || string::npos != strTempName.find("SVR"))
		{
			// 直接调用原有设备型号添加接口 [11/24/2015 pangubing]
			CDeviceModelAddReq cDevModelAddReq;
			cNewModel=cOldModel;
			cNewModel.SetDeviceCapInfo(cReq.GetDeviceModelCapDetail());
			enErrno = GenerateModelName(cNewModel);
			if (CMS_SUCCESS != enErrno)
			{
				cRsp.SetErrorCode(enErrno);
				break;
			}

			cDevModelAddReq.SetDeviceModelInfo(cNewModel);
			CTaskMsgTransfer<CDeviceModelAddReq> cTransDevModelAddReq;
			cTransDevModelAddReq.SetMsgObj(cDevModelAddReq);
			cTransDevModelAddReq.SetTransfer(*pcMsg,cDevModelAddReq.GetEventId());
			strModelId = DeviceModelAdd(&cTransDevModelAddReq,true);
			if (strModelId.empty())
			{
				cRsp.SetErrorCode(ERR_TAS_DBERROR);
				break;
			}
		} 

		cNewModel.Clear();
		DeviceModelItemQry(strModelId, cNewModel);

		enErrno = DeviceModelPreMod(cInfo.GetDeviceUUID(), cOldModel, cNewModel);
		if (CMS_SUCCESS != enErrno)
		{
			cRsp.SetErrorCode(enErrno);
			break;
		}

		if (DEVICE_TYPE_DECODER == cNewModel.GetDeviceTypeId())
		{
			Tv2DecoderUpdate(pcMsg,cInfo.GetDeviceUUID(), cOldModel, cNewModel);
		}


		TSqlString tSql;
		tSql.Format("update %s set %s='%s' where %s='%s'", T_DEVICE,
			         F_DEVICE_DEVMODELID,DbStr(strModelId), F_DEVICE_DEVID,DbStr(cReq.GetDeviceUUID()));

		if (!ExecSql(tSql, enExecNoRet))
		{
			cRsp.SetErrorCode(ERR_TAS_DBERROR);
			break;
		}
        bNeedRollback = false;
        if (nOldVidNum > nNewVidNum)
        {
            DeallocManagePULicense(nOldVidNum - nNewVidNum);
        }

		if (cOldModel.GetDeviceTypeId() != DEVICE_TYPE_DECODER)
		{
			UpdateDevTreeVer(cReq.GetDeviceUUID());
		}
		else
		{
			UpdateDevTreeVer("");
		}


		bHasNtf = (CMS_SUCCESS == DeviceItemQry(cReq.GetDeviceUUID(), cNtf.GetDeviceInfo()));

		SyncAnalyse(enSyncDataDeviceInfo, enSyncOperateMod, cInfo.GetDomainUUID(), &cInfo.GetDeviceUUID(), cInfo.GetDeviceType(), 
			cPropertyFlags.HasPropertyFlag(TDevicePropertyFlags::em_PropertyFlags_PrivateDev),
			cInfo.GetDevicePropertyFlags().HasPropertyFlag(TDevicePropertyFlags::em_PropertyFlags_PrivateDev));
	} while (false);


	PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
	if (bHasNtf) PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cNtf, cTransNtf);
    if (bNeedRollback)
    {
        DeallocManagePULicense(nNewVidNum - nOldVidNum);
    }
}

ENTasErrno CDbOpr::GenerateModelName( CDeviceModelInfo& cModelInfo )
{
	string strModelName;
	string strTempName = cModelInfo.GetDeviceModelName();
	StrUpper((s8*)strTempName.c_str());
	if (string::npos != strTempName.find("SVR"))
	{
		strModelName += cModelInfo.GetDeviceModelName()+"_";
	}
    else if (DEVICE_TYPE_ENCODER == cModelInfo.GetDeviceTypeId())               // 编码器
    {
        strModelName += "enc_";
    }
    else if (DEVICE_TYPE_DECODER == cModelInfo.GetDeviceTypeId())          // 解码器
    {
        strModelName += "dec_";
    }
    else                                                // 其他
    {
        strModelName += "oth_";
    }

	// 从两方面保证名字不重复 [5/4/2016 pangubing]
	const vector<TDeviceCapInfo>& vecCapInfo=cModelInfo.GetDeviceCapInfo();
	s8 achTmp[32] = {0};
	for (u32 i = 0; i < vecCapInfo.size(); ++i)
	{
		if (DEVICE_CAP_VIDEOSOURCE == vecCapInfo[i].type)
		{
			sprintf(achTmp, "%03d%03d", vecCapInfo[i].num,vecCapInfo[i].license);
		}
		else
		{
			sprintf(achTmp, "%03d", vecCapInfo[i].num);
		}

		strModelName += string(achTmp);
	}

	TSqlString tSqlString;
	tSqlString.Format("select count(DISTINCT(%s)) from %s", F_DEVICEMODEL_MODELID,T_DEVICEMODEL);
	if (!ExecSql(tSqlString, enExecMustRet))
	{
		return ERR_TAS_DBERROR;
	}

	s32 nModelNum = GetIntResult();

	sprintf(achTmp, "_%04d", nModelNum);

	strModelName += string(achTmp);

	cModelInfo.SetDeviceModelName(strModelName);


	return CMS_SUCCESS;
}

void CDbOpr::PtzLockAdd( const CTaskMsgTransfer<CPtzLockAddReq> *pcMsg )
{
	CTaskMsgTransfer<CPtzLockAddRsp> cTrans;
	CTaskMsgTransfer<CPtzLockAddNtfReq> cTransNtf;
	CPtzLockAddRsp &cRsp = cTrans.GetMsgObj();
	CPtzLockAddNtfReq &cNtf = cTransNtf.GetMsgObj();

	 bool bHasNtf = false;

	const CPtzLockAddReq &cReq = pcMsg->GetMsgObj();
	do 
	{
		const CPtzLockInfo &cPtzLockInfo = cReq.GetCPtzLockInfo();  

		if (cPtzLockInfo.GetDeviceId().empty())
		{
			cRsp.SetErrorCode(ERR_TAS_INVALID_PARM);
			break;
		}

		CUserInfo cNewUserInfo;
		GetCuserInfo(cPtzLockInfo.GetUserId(),cNewUserInfo);
		CVideosourcePrivilege cNewDevCapIndexPrivilege;
		if (GetUserDevCapInfo(cNewUserInfo,cPtzLockInfo.GetDeviceId(),cPtzLockInfo.GetVideoSrcId(),cNewDevCapIndexPrivilege))
		{
			if (!cNewDevCapIndexPrivilege.HasPrivilege(CVideosourcePrivilege::en_PTZ_Lock))
			{
				cRsp.SetErrorCode(ERR_TAS_INVALID_OPERATION);
				break;
			}
		}

		TSqlString tSql;
		CPtzLockInfo cOldPtzLockInfo;
		if (GetPtzLockInfo(cPtzLockInfo.GetDeviceId(),cPtzLockInfo.GetVideoSrcId(),cOldPtzLockInfo))
		{
			cRsp.SetBeginTime(cOldPtzLockInfo.GetBeginTime());
			cRsp.SetEndTime(cOldPtzLockInfo.GetEndTime());
			cRsp.SetUserId(cOldPtzLockInfo.GetUserId());
			CUserInfo cUserInfo;
			GetCuserInfo(cOldPtzLockInfo.GetUserId(),cUserInfo);
			cRsp.SetUsername(cUserInfo.GetUserName());
			// 查找PTZ权限 [4/12/2016 pangubing]
			CVideosourcePrivilege cDevCapIndexPrivilege;
			if (GetUserDevCapInfo(cUserInfo,cPtzLockInfo.GetDeviceId(),cPtzLockInfo.GetVideoSrcId(),cDevCapIndexPrivilege))
			{
				cRsp.SetPriPtzLevel(cDevCapIndexPrivilege.GetPTZCtrl());
			}
			cRsp.SetErrorCode(ERR_TAS_RECORD_EXISTS);
			break;
		}

		tSql.Format("insert into %s values('%s', %d ,'%s','%s','%s','%s')", 
			T_PTZLOCK, DbStr(cPtzLockInfo.GetDeviceId()), cPtzLockInfo.GetVideoSrcId(),DbDateTime(cPtzLockInfo.GetBeginTime()),
			DbDateTime(cPtzLockInfo.GetEndTime()),DbStr(cPtzLockInfo.GetUserId()),DbStr(cPtzLockInfo.GetPassword()));

		if (!ExecSql(tSql, enExecNoRet))
		{
			cRsp.SetErrorCode(ERR_TAS_DBERROR);
			break;
		}

		 bHasNtf = true;
		 cNtf.SetInfo(cPtzLockInfo);

	} while (false);

	PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
	if (bHasNtf) PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cNtf, cTransNtf);
}

void CDbOpr::PtzLockDel( const CTaskMsgTransfer<CPtzLockDelReq> *pcMsg,bool bIsDelInner/*=false*/ )
{
	CTaskMsgTransfer<CPtzLockDelRsp> cTrans;
	CTaskMsgTransfer<CPtzLockDelNtfReq> cTransNtf;
	CPtzLockDelRsp &cRsp = cTrans.GetMsgObj();
	CPtzLockDelNtfReq &cNtf = cTransNtf.GetMsgObj();

	bool bHasNtf = false;

	const CPtzLockDelReq &cReq = pcMsg->GetMsgObj();

	do 
	{
		TSqlString tSql;

		const string &deviceId = cReq.GetDeviceId();   
		int videoSrcId = cReq.GetVideoSrcId();         
		const string &userId = cReq.GetUserId();     
		const string &password = cReq.GetPassword();    
		const TPtzLockOprType &operateType=cReq.GetOperateType();    // 操作类型 解锁/删除/批量删除 unlock/delete/unlockall

		CUserInfo cUserInfo;
		if (!userId.empty()&&!GetCuserInfo(userId,cUserInfo))
		{
			cRsp.SetErrorCode(ERR_TAS_DBERROR);
			break;
		}

		if (PTZLOCK_UNLOCK == operateType)
		{
			CPtzLockInfo cInfo;
			CUserInfo cPtzLockUserInfo;
			CVideosourcePrivilege cDevCapIndexPrivilegePtzLock;
			if (!GetPtzLockInfo(deviceId,videoSrcId, cInfo))
			{
				cRsp.SetErrorCode(ERR_TAS_RECORD_NOT_EXISTS);
				break;
			}

			do 
			{
				//PTZ控制等级低或同级别不同用户解锁需要密码。
				//PTZ控制等级高的用户或者admin用户解锁时，不需要密码。
				if (cUserInfo.GetUserType() == CUSER_TYPE_SUPERADMIN)
				{
					if (!DeletePtzLock(deviceId,videoSrcId))
					{
						cRsp.SetErrorCode(ERR_TAS_DBERROR);
						break;
					}
				}
				else
				{
					if (!GetCuserInfo(cInfo.GetUserId(),cPtzLockUserInfo))
					{
						cRsp.SetErrorCode(ERR_TAS_DBERROR);
						break;
					}

					CVideosourcePrivilege cDevCapIndexPrivilege;
					if (!GetUserDevCapInfo(cUserInfo,deviceId,videoSrcId,cDevCapIndexPrivilege))
					{
						cRsp.SetErrorCode(ERR_TAS_DBERROR);
						break;
					}

					if (!GetUserDevCapInfo(cPtzLockUserInfo,deviceId,videoSrcId,cDevCapIndexPrivilegePtzLock))
					{
						cRsp.SetErrorCode(ERR_TAS_DBERROR);
						break;
					}

					if (cDevCapIndexPrivilege.GetPTZCtrl()>cDevCapIndexPrivilegePtzLock.GetPTZCtrl())
					{
						if (!DeletePtzLock(deviceId,videoSrcId))
						{
							cRsp.SetErrorCode(ERR_TAS_DBERROR);
							break;
						}
					} 
					else
					{
						if (password!=cInfo.GetPassword())
						{
							cRsp.SetErrorCode(ERR_TAS_INVALID_PASSWORD);
							break;
						}

						if (!DeletePtzLock(deviceId,videoSrcId))
						{
							cRsp.SetErrorCode(ERR_TAS_DBERROR);
							break;
						}
					}
				}
			} while (0);
			
			if (cRsp.GetErrorCode()!=CMS_SUCCESS)
			{
				cRsp.SetBeginTime(cInfo.GetBeginTime());
				cRsp.SetEndTime(cInfo.GetEndTime());
				cRsp.SetUserId(cInfo.GetUserId());
				cRsp.SetUsername(cPtzLockUserInfo.GetUserName());
				cRsp.SetPriPtzLevel(cDevCapIndexPrivilegePtzLock.GetPTZCtrl());
				break;
			}

		}
		else if (PTZLOCK_DELETE == operateType)
		{
			if (!DeletePtzLock(deviceId,videoSrcId))
			{
				cRsp.SetErrorCode(ERR_TAS_DBERROR);
				break;
			}
		}
		else if(PTZLOCK_UNLOCKALL == operateType)
		{
			if (cUserInfo.GetUserType() != CUSER_TYPE_SUPERADMIN)
			{
				cRsp.SetErrorCode(ERR_TAS_INVALID_OPERATION);
				break;
			}
			else
			{
				tSql.Format("delete from %s", T_PTZLOCK);
				if (!ExecSql(tSql, enExecNoRet))
				{
					cRsp.SetErrorCode(ERR_TAS_DBERROR);
					break;
				}
			}
		}
		else
		{
			cRsp.SetErrorCode(ERR_TAS_INVALID_PARM);
			break;
		}

		bHasNtf = true;
		cNtf.SetOperateType(operateType);
		cNtf.SetDeviceId(deviceId);
		cNtf.SetVideoSrcId(videoSrcId);

	} while (false);

	if (!bIsDelInner)
	{
		PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
	}
	
	if (bHasNtf) PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cNtf, cTransNtf);
}

ENTasErrno CDbOpr::PreparePtzLockQrySql( const CPtzLockQryReq &cReq, TSqlString &tSqlSelect, TSqlString &tSqlCount )
{
	TSqlString tSqlCond;
	const CPtzLockQueryCondition &cCond = cReq.GetCondition();

	AppendQryCondValuePair(cCond.GetDeviceID, m_strFieldValue, tSqlCond, F_PTZLOCK_DEVID, a);
	AppendQryCondValuePair(cCond.GetVideoSrcId, m_nFieldValue, tSqlCond, F_PTZLOCK_VIDSRCID, a);
	tSqlCond.EraseLastRedundantAnd();

	TSqlString tSqlOrderby;
	tSqlSelect.Format("select a.* from %s a", T_PTZLOCK);
	tSqlCount.Format("select count(*) from %s a", T_PTZLOCK);

	if (!tSqlCond.Empty())
	{
		tSqlSelect.AppendString(" where ");
		tSqlSelect.AppendString("%s", tSqlCond.GetBuf());
		tSqlCount.AppendString(" where ");
		tSqlCount.AppendString("%s", tSqlCond.GetBuf());
	}

	AddRowNumLimitModifer(tSqlSelect, cReq.GetStartEntryIndex(), cReq.GetExpectEntryNum());

	return CMS_SUCCESS;
}


void CDbOpr::PtzLockQry( const CTaskMsgTransfer<CPtzLockQryReq> *pcMsg )
{
	DatabaseQueryTemplate<CPtzLockQryReq, CPtzLockQryRsp, CPtzLockQryNtfReq, CPtzLockInfo>(
		pcMsg, &CDbOpr::PreparePtzLockQrySql, &CDbOpr::FetchPtzLockQryRecord, 
		&CPtzLockQryNtfReq::GetInfos, &CPtzLockInfo::GetDeviceId);
}

bool CDbOpr::GetPtzLockInfo( const UUIDString &devId,int nVidSrcId, CPtzLockInfo &cInfo )
{
	TSqlString tSql;
	tSql.Format("select * from %s where %s='%s' and %s=%d", T_PTZLOCK, F_PTZLOCK_DEVID, DbStr(devId),F_PTZLOCK_VIDSRCID,nVidSrcId);

	if (ExecSql(tSql, enExecMayRet))
	{
		if (IsNotEof())
		{
			FetchPtzLockQryRecord(cInfo);
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}


void CDbOpr::FetchPtzLockQryRecord( CPtzLockInfo &cInfo )
{
	cInfo.SetDeviceId(GetString(F_PTZLOCK_DEVID));
	cInfo.SetVideoSrcId(GetInt(F_PTZLOCK_VIDSRCID));
	cInfo.SetUserId(GetString(F_PTZLOCK_USERID));
	cInfo.SetBeginTime(GetCmsDate(F_PTZLOCK_BEGINTIME));
	cInfo.SetEndTime(GetCmsDate(F_PTZLOCK_ENDTIME));
	cInfo.SetPassword(GetString(F_PTZLOCK_PASSWORD));
}

bool CDbOpr::GetUserDevCapInfo( const CUserInfo &cUserInfo,const UUIDString &cDeviceId,int nVidSrcId,CVideosourcePrivilege &cDevCapIndexPrivilege )
{
	TSqlString tSql;
	if (cUserInfo.GetUsergrpID().empty())
	{
		tSql.Format("select * from %s where %s='%s' and %s='%s' and %s=%d and %s=%d", T_CUSER2DEVICECAP, F_CUSER2DEVICECAP_CUSERID, 
			DbStr(cUserInfo.GetUserID()),F_CUSER2DEVICECAP_DEVID, DbStr(cDeviceId),F_CUSERGRP2DEVICECAP_DEVCAPINDEX,
			nVidSrcId,F_CUSER2DEVICECAP_DEVCAPID,DEVICE_CAP_VIDEOSOURCE);
	} 
	else
	{
		tSql.Format("select * from %s where %s='%s' and %s='%s' and %s=%d and %s=%d", T_CUSERGRP2DEVICECAP, F_CUSERGRP2DEVICECAP_CUSERGROUPID, 
			DbStr(cUserInfo.GetUsergrpID()),F_CUSERGRP2DEVICECAP_DEVID, DbStr(cDeviceId),F_CUSERGRP2DEVICECAP_DEVCAPINDEX,
			nVidSrcId,F_CUSERGRP2DEVICECAP_DEVCAPID,DEVICE_CAP_VIDEOSOURCE);
	}

	if (ExecSql(tSql, enExecMayRet))
	{
		if (IsNotEof())
		{
			cDevCapIndexPrivilege.ConvertFromInt(GetInt(F_CUSER2DEVICECAP_DEVCAPINDEXPRIVILEGE));
			return true;
		}
		return false;
	}
	else
	{
		return false;
	}
}

bool CDbOpr::DeletePtzLock( const UUIDString &cDeviceId,int nVidSrcId ,const UUIDString &cUserId/*=""*/)
{
	TSqlString tSql;
	if (cUserId.empty())
	{
		tSql.Format("delete from %s where %s = '%s' and %s = %d", T_PTZLOCK, F_PTZLOCK_DEVID, DbStr(cDeviceId), F_PTZLOCK_VIDSRCID,nVidSrcId);
	} 
	else
	{
		tSql.Format("delete from %s where %s = '%s' and %s = '%s' and %s = %d", T_PTZLOCK, F_PTZLOCK_USERID,DbStr(cUserId),
			F_PTZLOCK_DEVID, DbStr(cDeviceId), F_PTZLOCK_VIDSRCID,nVidSrcId);
	}
	
	if (!ExecSql(tSql, enExecNoRet))
	{
		return false;
	}
	return true;
}

bool CDbOpr::DeleteUserPtzLockInner(const CTaskMsgTransferBase *pcMsg,const UUIDString &cUserId,const UUIDString &cDeviceId,const vector<TDeviceCapIndexs>& tCapIndexs)
{
	for (u32 i = 0; i < tCapIndexs.size(); ++i)
	{
		for (u32 j = 0; j < tCapIndexs[i].indexs.size(); ++j)
		{
			if (DEVICE_CAP_VIDEOSOURCE==tCapIndexs[i].id)
			{
				CPtzLockInfo cInfo;
				if (GetPtzLockInfo(cDeviceId,tCapIndexs[i].indexs[j],cInfo)&&
					cInfo.GetUserId()==cUserId)
				{
					DeletePtzLockInner(pcMsg,cDeviceId,tCapIndexs[i].indexs[j]);
				}
			}
		}
	}
	
	return true;
}

bool CDbOpr::DeletePtzLockInner( const CTaskMsgTransferBase *pcMsg,const UUIDString &cDeviceId,int nVidSrcId )
{
	CPtzLockDelReq cPtzLockDelReq;
	cPtzLockDelReq.SetDeviceId(cDeviceId);
	cPtzLockDelReq.SetVideoSrcId(nVidSrcId);
	cPtzLockDelReq.SetOperateType(PTZLOCK_DELETE);

	CTaskMsgTransfer<CPtzLockDelReq> cTransGrpAddDevReq;
	cTransGrpAddDevReq.SetMsgObj(cPtzLockDelReq);
	cTransGrpAddDevReq.SetTransfer(*pcMsg,cPtzLockDelReq.GetEventId());

	PtzLockDel(&cTransGrpAddDevReq,true);

	return true;
}


bool CDbOpr::ClearUserPtzLockInfo( const string &strUserId )
{
	TSqlString tSql;
	tSql.Format("delete from %s where %s = '%s'",T_PTZLOCK, F_PTZLOCK_USERID, DbStr(strUserId));

	return ExecSql(tSql, enExecNoRet);
}

ENTasErrno CDbOpr::QryUserPtzLockInfo( const string &strUserId,vector<CPtzLockInfo> &cPtzLockInfos )
{
	TSqlString tSql;
	tSql.Format("select * from %s where  %s = '%s'",T_PTZLOCK,F_PTZLOCK_USERID,DbStr(strUserId));
	if ( !ExecSql(tSql, enExecMayRet))
	{
		return ERR_TAS_DBERROR;
	}

	CPtzLockInfo cTmp;
	while (IsNotEof())
	{
		cTmp.Clear();
		FetchPtzLockQryRecord(cTmp);
		cPtzLockInfos.push_back(cTmp);
		if (!MoveNext())
		{
			return ERR_TAS_DBERROR;
		}
	}

	return CMS_SUCCESS;
}


ENTasErrno CDbOpr::PtzLockDelByDevId( const UUIDString &devId )
{
	TSqlString tSql;

	tSql.Format("delete from %s where %s='%s'", T_PTZLOCK, F_PTZLOCK_DEVID, DbStr(devId));
	if (!ExecSql(tSql, enExecNoRet)) return ERR_TAS_DBERROR;

	return CMS_SUCCESS;
}

ENTasErrno CDbOpr::PtzLockInfoUpdate( const UUIDString &tDevId, const CDeviceModelInfo &cOldModel, const CDeviceModelInfo &cNewModel )
{
	// 只有编码器的设备划归信息需要修改
	if (DEVICE_TYPE_ENCODER == cNewModel.GetDeviceTypeId())
	{
		const vector<TDeviceCapInfo>& vecOldDeviceCapInfo = cOldModel.GetDeviceCapInfo();
		const vector<TDeviceCapInfo>& vecNewDeviceCapInfo = cNewModel.GetDeviceCapInfo();

		u32 i = 0;      // 原设备型号视频源设备能力信息所在的下标
		u32 j = 0;      // 新设备型号视频源设备能力信息所在的下标
		for (; i < vecOldDeviceCapInfo.size(); ++i)
		{
			if (DEVICE_CAP_VIDEOSOURCE == vecOldDeviceCapInfo[i].type)
			{
				break;
			}
		}

		for (; j < vecNewDeviceCapInfo.size(); ++j)
		{
			if (DEVICE_CAP_VIDEOSOURCE == vecNewDeviceCapInfo[j].type)
			{
				break;
			}
		}

		TSqlString tSql;
		  // 旧设备型号有更多的视频源，删除多余视频源的Ptz锁信息
		if (vecOldDeviceCapInfo[i].num > vecNewDeviceCapInfo[j].num) 
		{
			tSql.Format("delete from %s where %s='%s'and %s>=%d", 
				T_PTZLOCK, F_PTZLOCK_DEVID, DbStr(tDevId), 
				F_PTZLOCK_VIDSRCID, vecNewDeviceCapInfo[j].num);
			if (!ExecSql(tSql, enExecNoRet))
			{
				return ERR_TAS_DBERROR;
			}
		}
	}

	return CMS_SUCCESS;
}

double CDbOpr::GetNextDevGrpSeq(const string& strParentGrpId)
{
	double fGrpseq=1024;

	TSqlString tSql;
	tSql.Format("select %s from %s where %s='%s' ORDER BY %s DESC limit 1", 
		F_DEVGROUP_GRPSEQ, T_DEVGROUP, F_DEVGROUP_PARENTDEVGROUPID,DbStr(strParentGrpId),F_DEVGROUP_GRPSEQ);

	if (ExecSql(tSql, enExecMayRet))
	{
		if (IsNotEof())
		{
			fGrpseq = GetDouble(F_DEVGROUP_GRPSEQ)+1024;
		}
	}

	return fGrpseq;
}


ENTasErrno CDbOpr::GetNextDevGrpGbid(const string& strTreeId, const string& strParentGrpId, TDevGroupType devGrpType, string& strAvaliableGrpGbId, bool bOnlyVirOrg)
{
	if (devGrpType==TDEV_GROUP_TYPE_CIVILCODE
		|| devGrpType == TDEV_GROUP_TYPE_UNKNOWN)
	{
		return GetNextDevGrpCivilCode(strTreeId, strParentGrpId, strAvaliableGrpGbId);
	}
	else if (devGrpType == TDEV_GROUP_TYPE_VIRTUALGRP)
	{
		return GetNextDevGrpVirGrpId(strTreeId, strParentGrpId, strAvaliableGrpGbId, bOnlyVirOrg);
	}
	else
	{
		return ERR_TAS_INVALID_PARM;
	}
}


ENTasErrno CDbOpr::GetNextDevGrpCivilCode(const string& strTreeId,  const string& strParentGrpId, string& strAvaliableGrpGbId )
{
	//思路：先根据treeId和parentgrpId去找同级已有分组，找到了就都保存起来，然后循环（找漏洞）找到当前没有的
	//就使用，如果都找完了而且连续，就递归。
	string strCivilCode, strParentCivilCode;
	set<string> setCivilCodes;
	TSqlString tSql;

	//先找到父分组的civiclode
	tSql.Format("select %s from %s where %s='%s'",
		F_DEVGROUP_NEWGBID, DbStr(GetTblDevGrpName(strTreeId)), F_DEVGROUP_DEVGROUPID, DbStr(strParentGrpId));
	if (!ExecSql(tSql, enExecMayRet))
	{
		return ERR_TAS_DBERROR;
	}

	if(IsEof())
	{
		return ERR_TAS_DBERROR;
	}
	strParentCivilCode = GetString(F_DEVGROUP_NEWGBID);
	if (strParentCivilCode.empty())
	{
		return ERR_TAS_DBERROR;
	}

	if (strParentCivilCode.size()>2*(GetMaxCivilcodeLev()-1))
	{
		return ERR_TAS_SRCTREE_HIERARCHY_TOO_DEEP;
	}

	tSql.Format("select %s from %s where %s='%s'", 
		F_DEVGROUP_NEWGBID,DbStr(GetTblDevGrpName(strTreeId)) , F_DEVGROUP_PARENTDEVGROUPID,DbStr(strParentGrpId));
	
	if (ExecSql(tSql, enExecMayRet))
	{
		if (IsNotEof())
		{
			do
			{
				//找到当前已有分组
				strCivilCode = GetString(F_DEVGROUP_NEWGBID);
				if (!strCivilCode.empty() && strCivilCode.size()==(strParentCivilCode.size()+2))
				{
					setCivilCodes.insert(strCivilCode);
				}

				MoveNext();
			
			}while(IsNotEof());

			if (setCivilCodes.size() >= 99) //civilcode每层分组最多只有99个
			{
				return ERR_TAS_SRCTREE_HIERARCHY_TOO_DEEP;
			}
			string astr1[10]={"0","1","2","3","4","5","6","7","8","9"};
			string astr2[10]={"0","1","2","3","4","5","6","7","8","9"};
			strCivilCode = strParentCivilCode;
			for(int i = 0; i<10; i++)
			{
				for(int j=0; j<10;j++)
				{
					if(i==0 && j==0) //+"00"不能成为新的分组civilcode
					{
						continue;
					}
					strCivilCode += astr1[i]+astr2[j];
					if (setCivilCodes.find(strCivilCode) == setCivilCodes.end())
					{
						strAvaliableGrpGbId = strCivilCode;
						return CMS_SUCCESS;
					}
					else
					{
						strCivilCode = strParentCivilCode;
					}
				}
			}
		}
		else
		{
			//认为是父分组的第一个子分组，根据父分组的civicode直接构造。
			strAvaliableGrpGbId = strParentCivilCode + "01";
		}
	}

	return CMS_SUCCESS;
}


ENTasErrno CDbOpr::GetNextDevGrpVirGrpId(const string& strTreeId, const string& strParentGrpId, string& strAvaliableGrpGbId, bool bOnlyVirOrg)
{
	string strLocalDomainGbid;
	if(!GetLocalDomainGbId(strLocalDomainGbid))
	{
		return ERR_TAS_DBERROR;
	}

	TSqlString tSql;
	//1、父分组为根分组，且根分组当前无20位子分组，则根据域GBID生成215分组。
	if (strTreeId == strParentGrpId && !bOnlyVirOrg)
	{
		tSql.Format("select %s from %s where substr(%s,11,3)='215' and %s='%s'",
			F_DEVGROUP_NEWGBID, DbStr(GetTblDevGrpName(strTreeId)), F_DEVGROUP_NEWGBID, F_DEVGROUP_PARENTDEVGROUPID, DbStr(strParentGrpId));
		if (!ExecSql(tSql, enExecMayRet))
		{
			return ERR_TAS_DBERROR;
		}
		if (IsEof())
		{
			char ach[20];
			sprintf(ach,"%s%s%s%s",strLocalDomainGbid.substr(0,10).c_str(), "215", strLocalDomainGbid.substr(13,1).c_str(), "000000");
			strAvaliableGrpGbId = string(ach);
			return CMS_SUCCESS;
		}
	}

	tSql.Format("select %s from %s where length(%s)=20 and left(%s,8)='%s' order by %s desc limit 1",
		F_DEVGROUP_NEWGBID, DbStr(GetTblDevGrpName(strTreeId)), 
		F_DEVGROUP_NEWGBID, F_DEVGROUP_NEWGBID,DbStr(strLocalDomainGbid.substr(0,8)),F_DEVGROUP_NEWGBID);
	if (!ExecSql(tSql, enExecMayRet))
	{
		return ERR_TAS_DBERROR;
	}
	if (IsEof())
	{
		//2、树当前无20位子分组，则根据域GBID生成216分组，尾数从“000000”开始，前10位和第14位与域GBID一致。
		char ach[20];
		sprintf(ach,"%s%s%s%s",strLocalDomainGbid.substr(0,10).c_str(), "216", strLocalDomainGbid.substr(13,1).c_str(), "000000");
		strAvaliableGrpGbId = string(ach);
		return CMS_SUCCESS;
	}
	else
	{
		//3、树当前有20位子分组，则根据域GBID生成216分组，前10位和第14位与域GBID一致。
		string strLastVirGrpGbid = GetString(F_DEVGROUP_NEWGBID);
		string strTemp = strLastVirGrpGbid.substr(14,6);
		char ach[7];
		if ( strTemp != "999999") //减少循环提升效率
		{
			u32 dwTemp = atoi(strTemp.c_str());
			sprintf(ach,"%06d\0",++dwTemp);
			strAvaliableGrpGbId = strLastVirGrpGbid.substr(0,10)+"216"+strLastVirGrpGbid.substr(13,1) + string(ach);
			return CMS_SUCCESS;
		}
		else
		{
			strLastVirGrpGbid[10]='2';
			strLastVirGrpGbid[11]='1';
			strLastVirGrpGbid[12]='6';
			for (u32 i = 0; i<999999;++i)
			{
				sprintf(ach,"%06d\0",i);
				strTemp = strLastVirGrpGbid.substr(0,14) + string(ach);
				tSql.Format("select %s from %s where %s='%s'",
					F_DEVGROUP_NEWGBID, DbStr(GetTblDevGrpName(strTreeId)), F_DEVGROUP_NEWGBID, DbStr(strTemp));
				if (!ExecSql(tSql, enExecMayRet))
				{
					return ERR_TAS_DBERROR;
				}
				if (IsEof())
				{
					strAvaliableGrpGbId = strTemp;
					return CMS_SUCCESS;
				}
			}
		}
	}
	return CMS_SUCCESS;
}


string CDbOpr::GetDevGrpFullPath(const string& strGrpId)
{
	string  strFullPath="";

	TSqlString tSql;
	tSql.Format("select %s from %s where %s='%s'", 
		F_DEVGROUP_FULLPATH, T_DEVGROUP, F_DEVGROUP_DEVGROUPID,DbStr(strGrpId));

	if (ExecSql(tSql, enExecMustRet))
	{
		strFullPath = GetString(F_DEVGROUP_FULLPATH);
	}

	return strFullPath;
}

ENTasErrno CDbOpr::CalcDomainGrpSeq( const string& strDomainId )
{
	TSqlString tSql;
	tSql.Format("call %s('%s')", P_GRPSEQCALC, DbStr(strDomainId));

	if (!ExecSql(tSql, enExecNoRet))
	{
		return ERR_TAS_DBERROR;
	}

	return CMS_SUCCESS;
}

ENTasErrno CDbOpr::GetCuserGrpIdsByCreatorId( const UUIDString& cCreatorId, vector<UUIDString>& vecCuserGrpIds )
{
	TSqlString tSql;
	tSql.Format("select %s from %s where %s='%s'", 
		F_CUSERGROUP_CUSERGROUPID, T_CUSERGROUP, F_CUSERGROUP_USERCREATORID,DbStr(cCreatorId));

	UUIDString strUserGrpId;
	if (ExecSql(tSql, enExecMayRet))
	{
		while(IsNotEof())
		{
			strUserGrpId = GetString(F_CUSERGROUP_CUSERGROUPID);
			vecCuserGrpIds.push_back(strUserGrpId);

			MoveNext();
		}
	}
	else
	{
		return ERR_TAS_DBERROR;
	}
	return CMS_SUCCESS;
}

ENTasErrno CDbOpr::DelGateWayData( const CTaskMsgTransfer<CGatewayDelReq> *pcMsg )
{
	//处理顺序：DeviceInfo----->DeviceModel----->DeviceGroup
	CTaskMsgTransfer<CGatewayDelRsp> cTrans;
	CGatewayDelRsp &cRsp = cTrans.GetMsgObj();
	const CGatewayDelReq &cReq = pcMsg->GetMsgObj();

	ENTasErrno tasErrNo = CMS_SUCCESS;

	vector<CDeviceInfo> vecDeviceDel;
	vector<UUIDString> vecModelDel;
	vector<UUIDString> vecDevGrpDel;
	g_cDbTaskPool.AddTransProgress(cReq.GetUri(), 100, 0);
	do
	{
		tasErrNo = DelGateWayDevice(pcMsg, vecDeviceDel);
		if (CMS_SUCCESS != tasErrNo)
		{
			break;
		}

		g_cDbTaskPool.AddTransProgress(cReq.GetUri(), 40);
		
		tasErrNo = DelGateWayDevModel(pcMsg, vecModelDel);
		if (CMS_SUCCESS != tasErrNo)
		{
			break;
		}

		g_cDbTaskPool.AddTransProgress(cReq.GetUri(), 50);

		tasErrNo = DelGateWayDevGrp(pcMsg, vecDevGrpDel);
		if (CMS_SUCCESS != tasErrNo)
		{
			break;
		}

		g_cDbTaskPool.AddTransProgress(cReq.GetUri(), 60);

	}while(false);

	//发送应答
	cRsp.SetErrorCode(tasErrNo);
	PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);

	if (CMS_SUCCESS == cRsp.GetErrorCode())
	{
		GateWayDataDelNtf(pcMsg, vecDeviceDel, vecModelDel, vecDevGrpDel);

		//Ntf发送完毕标记，用于高速g300_del_task所有Ntf已发送完毕
		cRsp.SetErrorCode(tasErrNo);
		PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
	}

	return tasErrNo;
}

ENTasErrno CDbOpr::DelGateWayDevice(const CTaskMsgTransfer<CGatewayDelReq> *pcMsg,vector<CDeviceInfo>& vecDeviceDel)
{
	//先查齐，之后循环封装Del_ntf，之后根据owner字段统一删除，然后发出Del_ntf，并且删除Redis中的数据发出Redis_ntf
	const CGatewayDelReq &cReq = pcMsg->GetMsgObj();
	TSipURI tUri(cReq.GetUri());

	ENTasErrno enErrno = CMS_SUCCESS;
	u32 dwRecordNum = 0;
	TSqlString tSql,tSqlCount;
	tSqlCount.Format("select count(*) from %s where %s='%s'", T_DEVICE, F_DEVICE_OWNER,DbStr(tUri.GetUser()));
	if (ExecSql(tSqlCount, enExecMustRet))
	{
		dwRecordNum = GetIntResult();
		if (dwRecordNum > 0)
		{
			vecDeviceDel.reserve(dwRecordNum);

			CDeviceInfo cDevInfo;
			tSql.Format("select * from %s where %s='%s'", T_DEVICE, F_DEVICE_OWNER,DbStr(tUri.GetUser()));
			if (ExecSql(tSql, enExecMayRet))
			{
				while(IsNotEof())
				{
					FetchDeviceQryRecord(cDevInfo);
					vecDeviceDel.push_back(cDevInfo);
					MoveNext();
				}
			}
			else
			{
				return ERR_TAS_DBERROR;
			}

			g_cDbTaskPool.AddTransProgress(cReq.GetUri(), 10);

			tSql.Format("delete from %s where %s in (select %s from %s where %s='%s')", T_PRIVATEDEVICE, F_PRIVATEDEVICE_PRIVATEDEVICEID, 
				F_DEVICE_DEVID, T_DEVICE, F_DEVICE_OWNER, DbStr(tUri.GetUser()));
			if (!ExecSql(tSql, enExecNoRet)) return ERR_TAS_DBERROR;


			tSql.Format("delete from %s where %s in (select %s from %s a inner join %s b ON a.%s=b.%s and b.%s='%s')", 
				T_FOREIGNCUSER2DEVCAPPRIVILEGE, F_FOREIGNCUSER2DEVCAPPRIVILEGE_INNERID, 
				F_DOMAINAUTHORIZATION_INNERID, T_DOMAINAUTHORIZATION, 
				T_DEVICE,F_DOMAINAUTHORIZATION_DEVICEID, F_DEVICE_DEVID, F_DEVICE_OWNER,DbStr(tUri.GetUser()));
			if (!ExecSql(tSql, enExecNoRet)) return ERR_TAS_DBERROR;


			tSql.Format("delete from %s where %s in (select %s from %s where %s='%s')", T_DOMAINAUTHORIZATION, F_DOMAINAUTHORIZATION_DEVICEID, 
				F_DEVICE_DEVID, T_DEVICE, F_DEVICE_OWNER, DbStr(tUri.GetUser()));
			if (!ExecSql(tSql, enExecNoRet)) return ERR_TAS_DBERROR;


			tSql.Format("delete from %s where %s in (select %s from %s where %s='%s')", T_TV2DECODER, F_TV2DECODER_DECODERID, 
				F_DEVICE_DEVID, T_DEVICE, F_DEVICE_OWNER, DbStr(tUri.GetUser()));
			if (!ExecSql(tSql, enExecNoRet)) return ERR_TAS_DBERROR;


			tSql.Format("delete from %s where %s in (select %s from %s where %s='%s')", T_DEVGRP2DEV, F_DEVGRP2DEV_DEVID, 
				F_DEVICE_DEVID, T_DEVICE, F_DEVICE_OWNER, DbStr(tUri.GetUser()));
			if (!ExecSql(tSql, enExecNoRet)) return ERR_TAS_DBERROR;


			vector<CDevTreeInfo> cDevTreeList;
			DevTreeListQry(cDevTreeList);
			for (size_t i=0;i<cDevTreeList.size();i++)
			{
				tSql.Format("delete from %s where %s in (select %s from %s where %s='%s')", DbStr(GetTblDevGrp2DevName(cDevTreeList[i].GetDevtreeId())),
					F_DEVGRP2DEV_DEVID, F_DEVICE_DEVID, T_DEVICE, F_DEVICE_OWNER, DbStr(tUri.GetUser()));
				if (!ExecSql(tSql, enExecNoRet)) return ERR_TAS_DBERROR;

				UpdateDevTreeVer("",cDevTreeList[i].GetDevtreeId());
			}


			tSql.Format("delete from %s where %s in (select %s from %s where %s='%s')", T_DEVICECAPINDEXDATA, F_DEVICECAPINDEXDATA_DEVICEID, 
				F_DEVICE_DEVID, T_DEVICE, F_DEVICE_OWNER, DbStr(tUri.GetUser()));
			if (!ExecSql(tSql, enExecNoRet)) return ERR_TAS_DBERROR;

            tSql.Format("delete from %s where %s in (select %s from %s where %s='%s')", T_VIDEONAMESEARCH, F_VIDEONAMESEARCH_DEVID, 
                F_DEVICE_DEVID, T_DEVICE, F_DEVICE_OWNER, DbStr(tUri.GetUser()));
            if (!ExecSql(tSql, enExecNoRet)) return ERR_TAS_DBERROR;


			tSql.Format("delete from %s where %s in (select %s from %s where %s='%s')", T_CUSERGRP2DEVICECAP, F_CUSERGRP2DEVICECAP_DEVID,
				F_DEVICE_DEVID, T_DEVICE, F_DEVICE_OWNER, DbStr(tUri.GetUser()));
			if (!ExecSql(tSql, enExecNoRet)) return ERR_TAS_DBERROR;


			tSql.Format("delete from %s where %s in (select %s from %s where %s='%s')", T_CUSERGRP2DEV, F_CUSERGRP2DEV_DEVID, 
				F_DEVICE_DEVID, T_DEVICE, F_DEVICE_OWNER, DbStr(tUri.GetUser()));
			if (!ExecSql(tSql, enExecNoRet)) return ERR_TAS_DBERROR;


			tSql.Format("delete from %s where %s in (select %s from %s where %s='%s')", T_CUSER2DEVICECAP, F_CUSER2DEVICECAP_DEVID,
				F_DEVICE_DEVID, T_DEVICE, F_DEVICE_OWNER,DbStr(tUri.GetUser()));
			if (!ExecSql(tSql, enExecNoRet)) return ERR_TAS_DBERROR;


			tSql.Format("delete from %s where %s in (select %s from %s where %s='%s')", T_CUSER2DEV, F_CUSER2DEV_DEVID, 
				F_DEVICE_DEVID, T_DEVICE, F_DEVICE_OWNER, DbStr(tUri.GetUser()));
			if (!ExecSql(tSql, enExecNoRet)) return ERR_TAS_DBERROR;


			tSql.Format("delete from %s where %s in (select %s from %s where %s='%s')", T_PTZLOCK, F_PTZLOCK_DEVID, 
				F_DEVICE_DEVID, T_DEVICE, F_DEVICE_OWNER, DbStr(tUri.GetUser()));
			if (!ExecSql(tSql, enExecNoRet)) return ERR_TAS_DBERROR;

			g_cDbTaskPool.AddTransProgress(cReq.GetUri(), 30);

			tSql.Format("delete from %s where %s='%s'", T_DEVICE, F_DEVICE_OWNER,DbStr(tUri.GetUser()));
			if (!ExecSql(tSql, enExecNoRet)) return ERR_TAS_DBERROR;

			UpdateDevTreeVer("");
		}
	}
	else
	{
		return ERR_TAS_DBERROR;
	}

	return CMS_SUCCESS;
}

ENTasErrno CDbOpr::DelGateWayDevModel( const CTaskMsgTransfer<CGatewayDelReq> *pcMsg, vector<UUIDString>& vecModelDel )
{
	//先查询，之后判断是否有设备正在使用，若否则打包del_ntf（Redis当前未存储）
	const CGatewayDelReq &cReq = pcMsg->GetMsgObj();
	TSipURI tUri(cReq.GetUri());

	TSqlString tSql,tSql2,tSqlCount;

	UUIDString devModelId;
	tSqlCount.Format("select count(*) from %s where %s='%s'", T_DEVICEMODEL, F_DEVICEMODEL_OWNER,DbStr(tUri.GetUser()));
	u32 dwRecordNum = 0;
	if (ExecSql(tSqlCount, enExecMustRet))
	{
		dwRecordNum = GetIntResult();
		if (dwRecordNum > 0)
		{
			vecModelDel.reserve(dwRecordNum);

			tSql.Format("select * from %s where %s='%s'", T_DEVICEMODEL, F_DEVICEMODEL_OWNER, DbStr(tUri.GetUser()));
			if (ExecSql(tSql, enExecMayRet))
			{
				while(IsNotEof())
				{
					devModelId = GetString(F_DEVICEMODEL_MODELID);

					//判断当前是否有设备正在使用此型号
					tSqlCount.Format("select count(*) from %s where %s='%s'", T_DEVICE, F_DEVICE_DEVMODELID,DbStr(devModelId));
					if (ExecSql(tSqlCount, enExecMustRet))
					{
						if (GetIntResult()==0)
						{
							vecModelDel.push_back(devModelId);
						}
						else
						{
							//该型号正在使用，设置其owner为空，避免被误删，引发其他问题
							tSql2.Format("update %s set %s='' where %s='%s'", T_DEVICEMODEL, F_DEVICEMODEL_OWNER, F_DEVICE_DEVMODELID, DbStr(devModelId));
							ExecSql(tSql2, enExecMayRet);
						}
					}
					else
					{
						return ERR_TAS_DBERROR;
					}

					MoveNext();
				}

				tSql.Format("delete from %s where %s='%s'", T_DEVICEMODEL, F_DEVICEMODEL_OWNER, DbStr(tUri.GetUser()));
				ExecSql(tSql, enExecMayRet);

				UpdateDevTreeVer("");
			}
			else
			{
				return ERR_TAS_DBERROR;
			}
		}
	}
	else
	{
		return ERR_TAS_DBERROR;
	}

	return CMS_SUCCESS;
}

ENTasErrno CDbOpr::DelGateWayDevGrp( const CTaskMsgTransfer<CGatewayDelReq> *pcMsg, vector<UUIDString>& vecDevGrpDel)
{
	//DeviceGroup:先查找，之后判断是否为空，若非空则设置owner字段为空，否则删除
	const CGatewayDelReq &cReq = pcMsg->GetMsgObj();
	TSipURI tUri(cReq.GetUri());
	
	TSqlString tSql,tSqlCount;

	tSqlCount.Format("select count(*)from %s where %s='%s'", T_DEVGROUP, F_DEVGROUP_OWNER,DbStr(tUri.GetUser()));
	if (ExecSql(tSqlCount,enExecMayRet))
	{
		u32 dwRecordNum = GetIntResult();
		if (dwRecordNum > 0)
		{
			vecDevGrpDel.reserve(dwRecordNum);
			UUIDString devGrpId;
			tSql.Format("select * from %s where %s='%s'", T_DEVGROUP, F_DEVGROUP_OWNER, DbStr(tUri.GetUser()));
			if (ExecSql(tSql, enExecMayRet))
			{
				while(IsNotEof())
				{
					devGrpId = GetString(F_DEVGROUP_DEVGROUPID);

					//判断此分组当前是否为空
					tSqlCount.Format("select count(*)from %s where %s='%s'", T_DEVGRP2DEV, F_DEVGRP2DEV_DEVGROUPID,DbStr(devGrpId));
					if (ExecSql(tSqlCount, enExecMustRet))
					{
						if (GetIntResult()==0)
						{
							vecDevGrpDel.push_back(devGrpId);
						}
					}
					else
					{
						return ERR_TAS_DBERROR;
					}

					MoveNext();
				}

				tSql.Format("delete from %s where %s='%s'", T_DEVGROUP, F_DEVGROUP_OWNER, DbStr(tUri.GetUser()));
				ExecSql(tSql, enExecMayRet);

				UpdateDevTreeVer("");
			}
			else
			{
				return ERR_TAS_DBERROR;
			}
		}
	}
	else
	{
		return ERR_TAS_DBERROR;
	}


	return CMS_SUCCESS;
}

void CDbOpr::GateWayDataDelNtf( const CTaskMsgTransfer<CGatewayDelReq> *pcMsg, vector<CDeviceInfo>& vecDeviceDel, vector<UUIDString>& vecModelDel, vector<UUIDString>& vecDevGrpDel )
{
	const CGatewayDelReq &cReq = pcMsg->GetMsgObj();
	u32 dwSeqNo = 0;

	//Devs-Ntf
	CTaskMsgTransfer<CDeviceDelNtfReq> cTransDevNtf;
	CDeviceDelNtfReq& cNtf = cTransDevNtf.GetMsgObj();
	cNtf.SetSession(cReq.GetSession());
	cNtf.SetDomainId(CurrentDomainId());
	u32 dwCurProgress = 60;
	for (vector<CDeviceInfo> ::iterator it = vecDeviceDel.begin(); it != vecDeviceDel.end(); ++it)
	{
		cNtf.SetSeqNum(++dwSeqNo);
		cNtf.SetDeviceUUID(it->GetDeviceUUID());
		PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cNtf, cTransDevNtf);

		RedisDeviceDel(*it); 
		if (0 == dwSeqNo%5000)
		{
			g_cDbTaskPool.AddTransProgress(cReq.GetUri(), ++dwCurProgress);
            OspTaskDelay(100);
		}
	}
	
	g_cDbTaskPool.AddTransProgress(cReq.GetUri(), 100);

	//DevModel-Ntf
	CTaskMsgTransfer<CDeviceModelDelNtfReq> cTransModelNtf;
	CDeviceModelDelNtfReq& cModelDelNtfReq = cTransModelNtf.GetMsgObj();
	cModelDelNtfReq.SetSession(cReq.GetSession());
	for (vector<UUIDString> ::iterator it = vecModelDel.begin(); it != vecModelDel.end(); ++it)
	{
		cModelDelNtfReq.SetSeqNum(++dwSeqNo);
		cModelDelNtfReq.SetDeviceModelId(*it);

		PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cModelDelNtfReq, cTransModelNtf);
	}

	//DevGrp-Ntf
	CTaskMsgTransfer<CDeviceGroupDelNtfReq> cTransGrpNtf;
	CDeviceGroupDelNtfReq& cDevGrpDelNtfReq = cTransGrpNtf.GetMsgObj();
	cDevGrpDelNtfReq.SetSession(cReq.GetSession());
	cDevGrpDelNtfReq.SetDomainId(CurrentDomainId());
	for (vector<UUIDString> ::iterator it = vecDevGrpDel.begin(); it != vecDevGrpDel.end(); ++it)
	{
		cDevGrpDelNtfReq.SetSeqNum(++dwSeqNo);
		cDevGrpDelNtfReq.GetDeviceGroupInfoKey().SetDeviceGroupID(*it);

		PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cDevGrpDelNtfReq, cTransGrpNtf);
	}
}

void CDbOpr::DevGrpChildrenQry( const CTaskMsgTransfer<CDeviceGroupChildrenQryReq> *pcMsg )
{
	CTaskMsgTransfer<CDeviceGroupChildrenQryRsp> cTrans;
	CDeviceGroupChildrenQryRsp &cRsp = cTrans.GetMsgObj();
	CTaskMsgTransfer<CDeviceGroupChildrenQryNtfReq> cTransNtf;
	CDeviceGroupChildrenQryNtfReq &cNtf = cTransNtf.GetMsgObj();

	const CDeviceGroupChildrenQryReq &cReq = pcMsg->GetMsgObj();
	TSqlString tSql, tSqlCount;
	bool bHasNtf = false;

	do 
	{
		if (cReq.GetDevGrpId().empty())
		{
			cRsp.SetErrorCode(ERR_TAS_INVALID_PARM);
			break;
		}

		cRsp.SetErrorCode(DevGrpChildrenQry(cReq.GetDevTreeId(), cReq.GetDevGrpId(), cNtf.GetDevGrpInfos(), cNtf.GetDevInfos()));
		if (cRsp.GetErrorCode() == CMS_SUCCESS)
		{
			cRsp.SetTotalEntryNum(cNtf.GetDevGrpInfos().size() + cNtf.GetDevInfos().size());
			if (cRsp.GetTotalEntryNum()>0)
			{
				bHasNtf = true;
			}
		}

	} while (false);
	PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
	if (bHasNtf)
	{
		cNtf.SetLastNtf(true);
		PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cNtf, cTransNtf);
	}

}

ENTasErrno CDbOpr::DevGrpChildrenQry( const UUIDString& devTreeId, const UUIDString& devGrpId, vector<CDeviceGroupInfo>& devGrpInfos, vector<CDeviceGroupDeviceInfo4Gb>& devInfos )
{
	bool bCanBeUse4Gb = IsDevTreeCanBeUse4Gb(devTreeId);

	CDeviceGroupInfo cDevGrpInfo;
	CDeviceGroupDeviceInfo4Gb cDevGbInfo;
	cDevGbInfo.SetDevDevTreeId(devTreeId);

	TDeviceInfo4Gb tDevGbInfo;
	TSqlString tSql,tSql2;
	string strTemp1,strTemp2;

	//首先查出本层分组的设备信息
	vector<CDeviceGroupDeviceInfo4Gb> devInfosTmp;
	tSql.Format("select a.*,b.%s,b.%s from %s a inner join tblDevice b ON b.%s = a.%s and a.%s = '%s'",
		F_DEVICE_DEVNAME, F_DEVICE_DOMAINID,DbStr(GetTblDevGrp2DevName(devTreeId)), F_DEVICE_DEVID, F_DEVGRP2DEV_DEVID, 
		F_DEVGRP2DEV_DEVGROUPID,DbStr(devGrpId));
	if (!ExecSql(tSql,enExecMayRet))
	{
		return ERR_TAS_DBERROR;
	}
	while(IsNotEof())
	{
		tDevGbInfo.SetVidIndex(GetInt(F_DEVGRP2DEV_DEVCAPINDEX));
		if (bCanBeUse4Gb)
		{
			tDevGbInfo.GetGbid().SetNewprimarygbid(GetString(F_DEVGRP2DEV_PRIMARYNEWGBID));
			tDevGbInfo.GetGbid().SetOldprimarygbid(GetString(F_DEVGRP2DEV_PRIMARYOLDGBID));
			tDevGbInfo.GetGbid().SetNewsecondarygbid(GetString(F_DEVGRP2DEV_SECONDARYNEWGBID));
			tDevGbInfo.GetGbid().SetOldprimarygbid(GetString(F_DEVGRP2DEV_SECONDARYOLDGBID));
		}
		cDevGbInfo.GetDevinfo().push_back(tDevGbInfo);
		cDevGbInfo.SetDevName(GetString(F_DEVICE_DEVNAME));
		cDevGbInfo.SetDevDomainId(GetString(F_DEVICE_DOMAINID));
		cDevGbInfo.SetDevGrpId(GetString(F_DEVGRP2DEV_DEVGROUPID));
		cDevGbInfo.SetDeviceId(GetString(F_DEVGRP2DEV_DEVID));

		
		devInfosTmp.push_back(cDevGbInfo);
		cDevGbInfo.Clear();

		MoveNext();
	}

	for (vector<CDeviceGroupDeviceInfo4Gb> ::iterator it = devInfosTmp.begin(); it != devInfosTmp.end(); ++it)
	{
		for (vector<TDeviceInfo4Gb> ::iterator it2 = it->GetDevinfo().begin(); it2 != it->GetDevinfo().end(); ++it2)
		{
			tSql2.Format("select %s from %s where %s = '%s' and  %s=%d and  %s=%d",
				F_DEVICECAPINDEXDATA_DEVICECAPINDEXNAME, T_DEVICECAPINDEXDATA,
				F_DEVICECAPINDEXDATA_DEVICEID, DbStr(it->GetDeviceId()), 
				F_DEVICECAPINDEXDATA_DEVICECAPID, DEVICE_CAP_VIDEOSOURCE,
				F_DEVICECAPINDEXDATA_DEVICECAPINDEX, it2->GetVidIndex());
			if (!ExecSql(tSql2, enExecMayRet))
			{
				return ERR_TAS_DBERROR;
			}
			else
			{
				if (IsNotEof())
				{
					it2->SetVidSrcName(GetString(F_DEVICECAPINDEXDATA_DEVICECAPINDEXNAME));
				}
				else
				{
					it2->SetVidSrcName("");
				}
			}
		}
	}
	if (!devInfosTmp.empty())
	{
		devInfos.insert(devInfos.end(), devInfosTmp.begin(), devInfosTmp.end());
	}
	
	//其次查询本层分组的子分组信息，之后递归处理
	vector<CDeviceGroupInfo> devGrpInfoTemp;
	tSql.Format("select * from %s where %s='%s'", 
		DbStr(GetTblDevGrpName(devTreeId)), 
		F_DEVGROUP_PARENTDEVGROUPID, DbStr(devGrpId));
	if (!ExecSql(tSql,enExecMayRet))
	{
		return ERR_TAS_DBERROR;
	}
	while(IsNotEof())
	{
		cDevGrpInfo.SetDeviceGroupID(GetString(F_DEVGROUP_DEVGROUPID));
		cDevGrpInfo.SetDomainUUID(GetString(F_DEVGROUP_DOMAINID));
		cDevGrpInfo.SetDeviceParentGroupID(GetString(F_DEVGROUP_PARENTDEVGROUPID));
		//cDevGrpInfo.SetParentDomainUUID(GetString(F_DEVGROUP_PARENTDOMAINID));				// 父域Id
		cDevGrpInfo.SetDeviceGroupName(GetString(F_DEVGROUP_GROUPNAME));
		cDevGrpInfo.SetDeviceGroupDesc(GetString(F_DEVGROUP_GROUPDESC));
		cDevGrpInfo.SetCreateDate(GetCmsDate(F_DEVGROUP_CREATEDATE));
		cDevGrpInfo.SetCreater(GetInt(F_DEVGROUP_CREATER));
		cDevGrpInfo.SetOwner(GetString(F_DEVGROUP_OWNER));
		cDevGrpInfo.SetFullpath(GetString(F_DEVGROUP_FULLPATH));
		cDevGrpInfo.SetGrpseq(GetDouble(F_DEVGROUP_GRPSEQ));
		if (bCanBeUse4Gb)
		{
			cDevGrpInfo.SetOldgbid(GetString(F_DEVGROUP_OLDGBID));
			cDevGrpInfo.SetNewgbid(GetString(F_DEVGROUP_NEWGBID));
		}

		devGrpInfoTemp.push_back(cDevGrpInfo);
		cDevGrpInfo.Clear();

		MoveNext();
	}
	if (!devGrpInfoTemp.empty())
	{
		devGrpInfos.insert(devGrpInfos.end(), devGrpInfoTemp.begin(), devGrpInfoTemp.end());
		
		ENTasErrno errCode = CMS_SUCCESS;
		for (vector<CDeviceGroupInfo> ::iterator it = devGrpInfoTemp.begin(); it != devGrpInfoTemp.end(); ++it)
		{
			errCode = DevGrpChildrenQry(devTreeId, it->GetDeviceGroupID(), devGrpInfos, devInfos);
			if (errCode != CMS_SUCCESS)
			{
				return errCode;
			}
		}
	}
	
	return CMS_SUCCESS;
}

ENTasErrno CDbOpr::GetDevGbidFromTblDevice( const UUIDString& strDevId, string& strGbId )
{
	TSqlString tSql;
	tSql.Format("select %s from %s where %s = '%s'", F_DEVICE_DEVDATA, T_DEVICE, F_DEVICE_DEVID, DbStr(strGbId));
	if (!ExecSql(tSql, enExecMayRet))
	{
		return ERR_TAS_DBERROR;
	}

	if (!IsNotEof())
	{
		return ERR_TAS_RECORD_NOT_EXISTS;
	}
	else
	{
		TDevExtData devData;
		devData.FromXml(string(GetString(F_DEVICE_DEVDATA)));
		map<TDevExtDataType, string> ::iterator it = devData.GetExtData().find(DEV_EXT_GBID);
		if (it != devData.GetExtData().end())
		{
			strGbId = it->second;
		}
		else
		{
			strGbId = "";
		}
	}
	return CMS_SUCCESS;
}

void CDbOpr::EmptyGbidDevsQry( const CDbMsgTransfer<TQryGbidEmptyDevsReq> *pcMsg )
{
	CDbMsgTransfer<TQryGbidEmptyDevsRsp> cTrans(*pcMsg, msgQryGbidEmptyDevsRsp);
	TQryGbidEmptyDevsRsp &cRsp = cTrans.GetMsgObj();
	vector<TVidSrcGbInfo> vecVidsrcGbInfos;
	bool bHasNtf = false;
	do 
	{
		CDevTreeInfo treeInfo;
		if(!GetDevTreeInfo(pcMsg->GetMsgObj().devTreeId, treeInfo))
		{
			cRsp.SetErrno(ERR_TAS_RECORD_NOT_EXISTS);
			break;
		}

		TSqlString tsql;
		tsql.Format("select %s,%s,%s from %s where %s=%d and length(%s)=%d ",
			F_DEVGRP2DEV_DEVGROUPID,F_DEVGRP2DEV_DEVID, F_DEVGRP2DEV_DEVCAPINDEX,
			DbStr(GetTblDevGrp2DevName(pcMsg->GetMsgObj().devTreeId)),
			F_DEVGRP2DEV_DEVCAPID, DEVICE_CAP_VIDEOSOURCE,
			F_DEVGRP2DEV_PRIMARYNEWGBID, 0);

		if (!ExecSql(tsql, enExecMayRet))
		{
			cRsp.SetErrno(ERR_TAS_DBERROR);
			break;
		}
		
		TVidSrcGbInfo tVidSrcGbInfo;
		while(IsNotEof())
		{
			tVidSrcGbInfo.devGrpId = GetString(F_DEVGRP2DEV_DEVGROUPID);
			tVidSrcGbInfo.devId = GetString(F_DEVGRP2DEV_DEVID);
			tVidSrcGbInfo.vidIndex = GetInt(F_DEVGRP2DEV_DEVCAPINDEX);

			vecVidsrcGbInfos.push_back(tVidSrcGbInfo);
			if (!MoveNext())
			{
				cRsp.SetErrno(ERR_TAS_DBERROR);
				break;
			}
		}
		if (cRsp.GetErrno() != CMS_SUCCESS)
		{
			break;
		}
		else 
		{
			if (vecVidsrcGbInfos.empty())
			{
				cRsp.totalNum = 0;
			}
			else
			{
				cRsp.totalNum = vecVidsrcGbInfos.size();
				bHasNtf = true;
			}
		}
	} while (false);

	PostDbMsgTransferFromDbTaskPool(cTrans);
	
	if (bHasNtf)
	{
		u32 dwIndex = 0;
		u32 dwTotal=vecVidsrcGbInfos.size();
		CDbMsgTransfer<TQryGbidEmptyDevsNtf> cTransNtf(*pcMsg, msgQryGbidEmptyDevsNtf);
		for (int i = 0; i < dwTotal; ++i)
		{
			++dwIndex;
			cTransNtf.GetMsgObj().vidSrcGbInfos.push_back(vecVidsrcGbInfos[i]);
			if (dwIndex==30)
			{
				if (i+1>=dwTotal)
				{
					cTransNtf.GetMsgObj().lastNtf = true;
				}
				PostDbMsgTransferFromDbTaskPool(cTransNtf);
				cTransNtf.GetMsgObj().vidSrcGbInfos.clear();
				dwIndex = 0;
			}
		}

		if (!cTransNtf.GetMsgObj().vidSrcGbInfos.empty())
		{
			cTransNtf.GetMsgObj().lastNtf = true;
			PostDbMsgTransferFromDbTaskPool(cTransNtf);
		}
	}
}

void CDbOpr::MatchChildrenGrpGbid( const CDbMsgTransfer<TMatchChildrenGrpGbidReq> *pcMsg )
{
	CDbMsgTransfer<TMatchChildrenGrpGbidRsp> cTrans(*pcMsg, msgMatchChildrenGrpGbidRsp);
	TMatchChildrenGrpGbidRsp &cRsp = cTrans.GetMsgObj();
	do 
	{
		CDevTreeInfo devTree;
		if (!GetDevTreeInfo(pcMsg->GetMsgObj().devTreeId, devTree))
		{
			cRsp.SetErrno(ERR_TAS_RECORD_NOT_EXISTS);
			break;
		}
		if (!devTree.GetCanBeUsed4Gb())
		{
			cRsp.SetErrno(ERR_TAS_INVALID_OPERATION);
			break;
		}

		cRsp.SetErrno(MatchChildrenGrpGbid(pcMsg->GetMsgObj().devTreeId, pcMsg->GetMsgObj().devParentGrpId, pcMsg->GetMsgObj().devParentGrpGbid));

	} while (false);
	
	PostDbMsgTransferFromDbTaskPool(cTrans);
}

ENTasErrno CDbOpr::MatchChildrenGrpGbid( const UUIDString& devTreeId, const UUIDString& devParentGrpId, const string& strParentGrpGbid )
{
	TSqlString tSql;
	//先设置oldgbid
	string strOldGbid;
	tSql.Format("select %s from %s where %s='%s'",
		F_DEVGROUP_OLDGBID, DbStr(GetTblDevGrpName(devTreeId)),
		F_DEVGROUP_DEVGROUPID, DbStr(devParentGrpId));
	if (!ExecSql(tSql, enExecMayRet))
	{
		return ERR_TAS_DBERROR;
	}
	if (strOldGbid.empty())
	{
		tSql.Format("update %s set %s=%s,%s='%s' where %s='%s'", 
			DbStr(GetTblDevGrpName(devTreeId)),
			F_DEVGROUP_OLDGBID,DbStr(strParentGrpGbid),
			F_DEVGROUP_NEWGBID,DbStr(strParentGrpGbid),
			F_DEVGROUP_DEVGROUPID, DbStr(devParentGrpId));
	}
	else
	{
		tSql.Format("update %s set %s='%s' where %s='%s'", 
			DbStr(GetTblDevGrpName(devTreeId)),
			F_DEVGROUP_NEWGBID,DbStr(strParentGrpGbid),
			F_DEVGROUP_DEVGROUPID, DbStr(devParentGrpId));
	}
	if (!ExecSql(tSql, enExecNoRet))
	{
		return ERR_TAS_DBERROR;
	}

	////再设置newgbid
	//tSql.Format("update %s set %s='%s' where %s='%s'",
	//	DbStr(GetTblDevGrpName(devTreeId)),F_DEVGROUP_NEWGBID,DbStr(strParentGrpGbid),
	//	F_DEVGROUP_DEVGROUPID, DbStr(devParentGrpId));
	//if (!ExecSql(tSql, enExecMayRet))
	//{
	//	return ERR_TAS_DBERROR;
	//}
	
	//最后设置子分组gbid
	vector<string> vecGrpIds;
	tSql.Format("select %s from %s where %s = '%s'", 
		F_DEVGROUP_DEVGROUPID, DbStr(GetTblDevGrpName(devTreeId)),
		F_DEVGROUP_PARENTDEVGROUPID,DbStr(devParentGrpId));
	if (ExecSql(tSql, enExecMayRet))
	{
		string strDevGrpId;
		while(IsNotEof())
		{
			strDevGrpId = GetString(F_DEVGROUP_DEVGROUPID);
			vecGrpIds.push_back(strDevGrpId);

			MoveNext();
		}
	}
	else
	{
		return ERR_TAS_DBERROR;
	}

	ENTasErrno errCode = CMS_SUCCESS;
	string strGrpGbid;
	for(vector<string> ::iterator it = vecGrpIds.begin(); it != vecGrpIds.end(); ++it)
	{
		errCode = GetNextDevGrpCivilCode(devTreeId, devParentGrpId, strGrpGbid);
		if (errCode != CMS_SUCCESS)
		{
			return errCode;
		}
		if (strGrpGbid.empty())
		{
			//无行政区划可用，报错
			return ERR_TAS_SRCTREE_HIERARCHY_TOO_DEEP;
		}
		errCode = MatchChildrenGrpGbid(devTreeId, *it, strGrpGbid);
		if (errCode != CMS_SUCCESS)
		{
			return errCode;
		}
	}
	return CMS_SUCCESS;
}

ENTasErrno CDbOpr::GetFitDev4Gb(const UUIDString& dstTreeId,const UUIDString& srcTreeId, const UUIDString& devGrpId, 
    bool bOnlyOnline, bool bUnAssigned, bool bOnlyEnable, vector<TVidSrcGbInfoExt>& fitDevs)
{
	//先判断源设备树是否是国标选择推送树，如果是则无需考虑设备gbid的问题
	bool bSrcTree4Gb = IsDevTreeCanBeUse4Gb(srcTreeId);

	TSqlString tSql;
	if (bSrcTree4Gb)
	{
		tSql.Format("select %s,%s,%s,%s from %s where %s='%s' and %s=%d", 
			F_DEVGRP2DEV_DEVID,F_DEVGRP2DEV_DEVCAPINDEX,F_DEVGRP2DEV_PRIMARYNEWGBID, F_DEVGRP2DEV_PRIMARYOLDGBID,
			DbStr(GetTblDevGrp2DevName(srcTreeId)),F_DEVGRP2DEV_DEVGROUPID, DbStr(devGrpId),
			F_DEVGRP2DEV_DEVCAPID, DEVICE_CAP_VIDEOSOURCE);
	}
	else
	{
		tSql.Format("select %s,%s from %s where %s='%s' and %s=%d",
			F_DEVGRP2DEV_DEVID,F_DEVGRP2DEV_DEVCAPINDEX, 
			DbStr(GetTblDevGrp2DevName(srcTreeId)),F_DEVGRP2DEV_DEVGROUPID, DbStr(devGrpId),
			F_DEVGRP2DEV_DEVCAPID, DEVICE_CAP_VIDEOSOURCE);
	}
	if (!ExecSql(tSql,enExecMayRet))
	{
		return ERR_TAS_DBERROR;
	}
	
	vector<TVidSrcGbInfoExt> vecTemp;
	TVidSrcGbInfoExt tExt;
	while(IsNotEof())
	{
		tExt.devId = GetString(F_DEVGRP2DEV_DEVID);
		tExt.vidIndex = GetInt(F_DEVGRP2DEV_DEVCAPINDEX);
		if(bOnlyOnline && !IsDevVidSrcOnline(tExt.devId, tExt.vidIndex))
		{
			tExt.clear();
			MoveNext();
			continue;
		}

        if (bOnlyEnable && !IsDevEnable(tExt.devId))
        {
            tExt.clear();
            MoveNext();
            continue;
        }

		if(bSrcTree4Gb)
		{
			tExt.newPrimaryGbid = GetString(F_DEVGRP2DEV_PRIMARYNEWGBID);
			tExt.oldPrimaryGbid = GetString(F_DEVGRP2DEV_PRIMARYOLDGBID);
			
			//分组拖拽时，只需要设置主流
			//tExt.newSecondaryGbid = GetString(F_DEVGRP2DEV_SECONDARYNEWGBID);
			//tExt.oldSecondaryGbid = GetString(F_DEVGRP2DEV_SECONDOLDGBID);
		}

		vecTemp.push_back(tExt);
		tExt.clear();
		MoveNext();
	}

	string strGbid;
	vector<TVidSrcGbInfoExt> vecTemp2;
	vecTemp2.reserve(vecTemp.size());
	for (vector<TVidSrcGbInfoExt> ::iterator it = vecTemp.begin(); it != vecTemp.end();++it)
	{
		//去掉已划归设备
		if (bUnAssigned && IsDevVidSrcAssigned(dstTreeId, it->devId, it->vidIndex))
		{
			continue;
		}

		//补全gbid
		if (GetOriginDevGbid(it->devId, strGbid))
		{
			if (strGbid.substr(10, 3) == "132" || strGbid.substr(10,3)=="131")
			{
				it->newPrimaryGbid = strGbid;
				it->oldPrimaryGbid = strGbid;
			}
		}
		vecTemp2.push_back(*it);
	}

	fitDevs.insert(fitDevs.end(), vecTemp2.begin(), vecTemp2.end());

	return CMS_SUCCESS;
}

bool CDbOpr::IsDevVidSrcAssigned( const UUIDString& devTreeId, const UUIDString& devId, const int vidSrcIdx )
{
	TSqlString tSql;
	tSql.Format("select count(*) from %s where %s='%s' and %s=%d and %s=%d",
		DbStr(GetTblDevGrp2DevName(devTreeId)), 
		F_DEVGRP2DEV_DEVID, DbStr(devId),
		F_DEVGRP2DEV_DEVCAPID, DEVICE_CAP_VIDEOSOURCE,
		F_DEVGRP2DEV_DEVCAPINDEX, vidSrcIdx);
	if (!ExecSql(tSql, enExecMustRet))
	{
		return false;
	}
	if (GetIntResult()>0)
	{
		return true;
	}
	return false;
}

bool CDbOpr::GetOriginDevGbid( const UUIDString& devId, string& strGbid )
{
	TSqlString tSql;
	tSql.Format("select %s from %s where %s='%s'",
		F_DEVICE_DEVDATA, T_DEVICE, F_DEVICE_DEVID, DbStr(devId));
	if (!ExecSql(tSql, enExecMayRet))
	{
		return false;
	}
	TDevExtData tData;
	if (IsNotEof())
	{
		tData.FromXml(GetString(F_DEVICE_DEVDATA));
		map<TDevExtDataType, string> ::iterator it = tData.GetExtData().find(DEV_EXT_GBID);
		if (it != tData.GetExtData().end())
		{
			strGbid = it->second;
			return true;
		}
	}
	return false;
}


bool CDbOpr::IsDevTreeCanBeUse4Gb( const UUIDString& devTreeId )
{
	if (devTreeId.empty())
	{
		return false;
	}

	CDevTreeInfo devTreeInfo;
	if (!GetDevTreeInfo(devTreeId, devTreeInfo))
	{
		return false;
	}
	return devTreeInfo.GetCanBeUsed4Gb();
}

ENTasErrno CDbOpr::DevicePinyinSearchVideoNameInit( const string &devId, int videoNum, int videoIndex/*=0*/, const char* szName )
{
    TSqlString tSql;
    string strDevName;
    if (szName!=NULL){
        strDevName=szName;
    }else{
        tSql.Format("select %s from %s where %s='%s'", F_DEVICE_DEVNAME, T_DEVICE, F_DEVICE_DEVID, DbStr(devId));
        if (!ExecSql(tSql,enExecMayRet)){
            return ERR_TAS_DBERROR;
        }
        if (IsNotEof()){
            strDevName=GetString(F_DEVICE_DEVNAME);
        }else{
            return ERR_TAS_RECORD_NOT_EXISTS;
        }
    }

    string vidName;
    string searchName;
    char szBuf[1000];
    szBuf[sizeof(szBuf)-1]='\0';
    for (int i=videoIndex; i<videoNum;++i)
    {
        if(videoNum==1){
            vidName=strDevName;
        }else{
            snprintf(szBuf, sizeof(szBuf)-1, "%s_%d", strDevName.c_str(), i+1);
            vidName=szBuf;
        }
        searchName=GetPinYinFromUtf8(vidName);
        
        tSql.Format("insert ignore into %s values('%s',%d,'%s','%s')", 
            T_VIDEONAMESEARCH, DbStr(devId), i, DbStr(searchName), DbStr(vidName));
        tSql.AppendString(";insert ignore into %s values('%s',1,%d,'%s','%s')", T_DEVICECAPINDEXDATA, 
            DbStr(devId), i, DbStr(vidName),
            "<TDeviceCapExtData><extData size=\"2\"><item index=\"0\"><key>Lat</key><value>65535</value></item><item index=\"1\"><key>Lon</key><value>65535</value></item></extData></TDeviceCapExtData>");

        if (!ExecSql(tSql,enExecNoRet)){
            return ERR_TAS_DBERROR;
        }
    }

    return CMS_SUCCESS;
}

ENTasErrno CDbOpr::DevicePinyinSearchVideoNameDel( const string &devId, int videoIndex/*=-1*/ )
{
    TSqlString tSql;
    if (videoIndex==-1){
        tSql.Format("delete from %s where %s='%s'", T_VIDEONAMESEARCH, F_VIDEONAMESEARCH_DEVID, DbStr(devId));
    }else{
        tSql.Format("delete from %s where %s='%s' and %s>=%d", T_VIDEONAMESEARCH, F_VIDEONAMESEARCH_DEVID, 
            DbStr(devId), F_VIDEONAMESEARCH_VIDEOID, videoIndex);
    }
    
    if (!ExecSql(tSql,enExecNoRet)){
        return ERR_TAS_DBERROR;
    }

    return CMS_SUCCESS;
}

ENTasErrno CDbOpr::DevicePinyinSearchVideoNameSet( const string &devId, vector<int> videoIndexs, vector<string> videoNames )
{
    if (videoIndexs.size()!=videoNames.size())
    {
        return ERR_TAS_INVALID_PARM;
    }

    TSqlString tSql;
    string searchName;
    for (int i=0; i<videoIndexs.size();++i)
    {
		if (videoNames[i].empty())
		{
			continue;
		}
        searchName=GetPinYinFromUtf8(videoNames[i]);
        tSql.Format("update %s set %s='%s',%s='%s' where %s='%s' and %s=%d", T_VIDEONAMESEARCH,  F_VIDEONAMESEARCH_SEARCHTEXT, DbStr(searchName), 
            F_VIDEONAMESEARCH_VIDEONAME, DbStr(videoNames[i]), F_VIDEONAMESEARCH_DEVID, DbStr(devId), F_VIDEONAMESEARCH_VIDEOID, videoIndexs[i]);
        if (!ExecSql(tSql,enExecNoRet)){
            return ERR_TAS_DBERROR;
        }
    }

    return CMS_SUCCESS;
}

void CDbOpr::DevGrpDevDel4Favorite( const string &userId, const string &devid, int videoid )
{
    TSqlString tSql;
    tSql.Format("delete from tblDevGrp2Dev_%s where %s='%s' and %s=%d and %s=%d", DbStr(userId), 
        F_DEVGRP2DEV_DEVID, DbStr(devid),
        F_DEVGRP2DEV_DEVCAPID, DEVICE_CAP_VIDEOSOURCE,
        F_DEVGRP2DEV_DEVCAPINDEX, videoid);

    ExecSql(tSql, enExecNoRet);
}

void CDbOpr::DevGrpDevDel4Favorite( const string &userId, const CUserDeviceInfoKey &key )
{
    TSqlString tSql;
    tSql.Format("delete from tblDevGrp2Dev_%s where %s='%s' and %s=%d and ( ", DbStr(userId), 
        F_DEVGRP2DEV_DEVID, DbStr(key.GetDeviceID()),
        F_DEVGRP2DEV_DEVCAPID, DEVICE_CAP_VIDEOSOURCE);

    bool bFirstVideo=true;
    for (vector<TDeviceCapIndexs>::const_iterator it=key.GetDeviceCapIndexs().begin();
        it!=key.GetDeviceCapIndexs().end();++it)
    {
        if (it->id!=DEVICE_CAP_VIDEOSOURCE){
            continue;
        }

        for (vector<int>::const_iterator it2=it->indexs.begin();
            it2!=it->indexs.end();++it2)
        {
            if(bFirstVideo) bFirstVideo=false;
            else{
                tSql.AppendString(" or ");
            }

            tSql.AppendString(" %s=%d ", F_DEVGRP2DEV_DEVCAPINDEX, *it2);
        }
    }
    if (bFirstVideo)
    {
        return;
    }

    tSql.AppendString(" )");
    ExecSql(tSql, enExecNoRet);
}


void CDbOpr::DeviceIdQry( const CTaskMsgTransfer<CDeviceQryReq> *pcMsg )
{
    CTaskMsgTransfer< pair< int, vector<string> > > *pRsp =new CTaskMsgTransfer< pair< int, vector<string> > >();
    pRsp->SetTransfer(*pcMsg, msgQryDevidRsp);

    do 
    {
        TSqlString tSqlSelect;
        /* 生成sql语句 begin */
        const CDeviceInfoQueryCondition &cCond = pcMsg->GetMsgObj().GetDeviceInfoQueryCondition();

        TSqlString tSqlCond;
        TSqlString tSqlOrderby;
        bool bNoUserLimit;
        if (cCond.GetCuserID(m_strFieldValue))
        {
            string strUserId = m_strFieldValue;
            string strUserGrpId;
            if (!GetCuserGrpId(strUserId, strUserGrpId))
            {
                pRsp->GetMsgObj().first=ERR_TAS_USER_NOT_EXIST;
                break;
            }

            if (strUserGrpId.empty())
            {
                tSqlSelect.Format("select a.%s from %s a inner join %s b on a.%s=b.%s where b.%s='%s'", 
                    F_DEVICE_DEVID, T_DEVICE, T_CUSER2DEV, F_DEVICE_DEVID, F_CUSER2DEV_DEVID, F_CUSER2DEV_CUSERID, 
                    DbStr(strUserId));
            }
            else
            {
                tSqlSelect.Format("select a.%s from %s a inner join %s b on a.%s=b.%s where b.%s='%s'", 
                    F_DEVICE_DEVID, T_DEVICE, T_CUSERGRP2DEV, F_DEVICE_DEVID, F_CUSERGRP2DEV_DEVID, 
                    F_CUSERGRP2DEV_CUSERGROUPID, DbStr(strUserGrpId));
            }

            bNoUserLimit = false;
        }
        else
        {
            tSqlSelect.Format("select a.%s from %s a", F_DEVICE_DEVID, T_DEVICE);

            bNoUserLimit = true;
        }

        AppendQryCondValuePair(cCond.GetDomainId, m_strFieldValue, tSqlCond, F_DEVICE_DOMAINID, a);
        AppendQryCondValuePair(cCond.GetDeviceUUID, m_strFieldValue, tSqlCond, F_DEVICE_DEVID, a);
        if (cCond.GetDeviceUUIDBegin(m_strFieldValue))
        {
            tSqlCond.AppendString("a.%s>'%s' and ", F_DEVICE_DEVID, DbStr(m_strFieldValue));
        }

        AppendQryCondLikeStringPair(cCond.GetDeviceName, m_strFieldValue, tSqlCond, F_DEVICE_DEVNAME, a);
        AppendQryCondLikeStringPair(cCond.GetDeviceAlias, m_strFieldValue, tSqlCond, F_DEVICE_DEVALIAS, a);
        AppendQryCondValuePair(cCond.GetOwner, m_strFieldValue, tSqlCond, F_DEVICE_OWNER, a);

        set<TCreater> creaters;
        if (cCond.GetCreaters(creaters) && 0 < creaters.size())		// 指定了创建者集合
        {
            tSqlCond.AppendString("(");
            for (set<TCreater>::iterator it = creaters.begin(); it != creaters.end(); ++it)
            {
                if (it != creaters.begin()) 
                    tSqlCond.AppendString(" or ");
                tSqlCond.AppendString("a.%s=%d", F_DEVICE_CREATER, *it);
            }
            tSqlCond.AppendString(") and ");
        }

        set<int> types;
        if (cCond.GetDeviceType(types))
        {
            if (types.size() == 0)
            {
                pRsp->GetMsgObj().first=ERR_TAS_INVALID_DEVICETYPEID;
                break;
            }

            tSqlCond.AppendString("(");
            for (set<int>::iterator it = types.begin(); it != types.end(); ++it)
            {
                if (it != types.begin()) tSqlCond.AppendString(" or ");
                tSqlCond.AppendString("a.%s=%d", F_DEVICE_DEVTYPE, *it);
            }
            tSqlCond.AppendString(") and ");
        }
        AppendQryCondValuePair(cCond.GetDeviceModelId, m_strFieldValue, tSqlCond, F_DEVICEMODEL_MODELID, a);
        AppendQryCondValuePair(cCond.GetUsedFlag, m_bFieldValue, tSqlCond, F_DEVICE_USEFLAG, a);
        AppendQryCondValuePair(cCond.GetNetType, m_nFieldValue, tSqlCond, F_DEVICE_NETTYPE, a);
        AppendQryCondValuePair(cCond.GetNetAddress, m_strFieldValue, tSqlCond, F_DEVICE_NETADDR, a);
        AppendQryCondValuePair(cCond.GetDescription, m_strFieldValue, tSqlCond, F_DEVICE_DEVDESC, a);
        AppendQryCondDatePeriodPair(cCond.GetExpireDate_From, m_strFieldValue, cCond.GetExpireDate_To, m_strFieldValue2,
            tSqlCond, F_DEVICE_EXPIREDATE, a);
        AppendQryCondDatePeriodPair(cCond.GetCreateDate_From, m_strFieldValue, cCond.GetCreateDate_To, m_strFieldValue2,
            tSqlCond, F_DEVICE_CREATEDATE, a);
        tSqlCond.EraseLastRedundantAnd();

        if (!tSqlCond.Empty())
        {
            if (bNoUserLimit)
            {
                tSqlSelect.AppendString(" where ");
            }
            else
            {
                tSqlSelect.AppendString(" ");
            }
            tSqlSelect.AppendString("%s", tSqlCond.GetBuf());
        }
        if (pcMsg->GetMsgObj().GetOrderField().empty())
        {
            tSqlSelect.AppendString(" order by convert(a.devName using gbk)");
        }
        else
        {
            tSqlSelect.AppendString(" order by");
            for (s32 i = 0; i < pcMsg->GetMsgObj().GetOrderField().size(); ++i)
            {
                if (i > 0)
                {
                    tSqlSelect.AppendString(",");
                }
                if (pcMsg->GetMsgObj().GetOrderField()[i].columnId != "devName")
                {
                    tSqlSelect.AppendString(" a.%s %s", 
                        pcMsg->GetMsgObj().GetOrderField()[i].columnId.c_str(), pcMsg->GetMsgObj().GetOrderField()[i].asc?"asc":"desc");
                }
                else
                {
                    tSqlSelect.AppendString(" convert(a.%s using gbk) %s", 
                        pcMsg->GetMsgObj().GetOrderField()[i].columnId.c_str(), pcMsg->GetMsgObj().GetOrderField()[i].asc?"asc":"desc");
                }
            }
        }
        AddRowNumLimitModifer(tSqlSelect, pcMsg->GetMsgObj().GetStartEntryIndex(), pcMsg->GetMsgObj().GetExpectEntryNum(), false);

        /* 生成sql语句 end */

        if(!ExecSql(tSqlSelect, enExecMayRet))
        {
            pRsp->GetMsgObj().first=ERR_TAS_DBERROR;
        }
        else
        {
            pRsp->GetMsgObj().second.reserve(100000);
            while (this->IsNotEof())
            {
                pRsp->GetMsgObj().second.push_back(GetString(F_DEVICE_DEVID));
                if (!this->MoveNext())
                {
                    pRsp->GetMsgObj().first=ERR_TAS_DBERROR;
                    break;
                }
            }
        }
    } while (false);

    ptrint ptr=reinterpret_cast<ptrint>(pRsp);
    GLOBELOG(enLogSrcDbTaskPool, ALL_LEV, "PostTaskMsgTransfer2CCtrl, msgobj pointer=%p\n", ptr);
    if (0 != OspPost(MAKEIID(AID_CCtrl, CInstance::DAEMON), msgTaskMsgTransfer4Db, &ptr, sizeof(ptr)))
    {
        delete pRsp;
    }
}


bool CDbOpr::CheckGbidFit( const UUIDString& devTreeId, const UUIDString& devGrpId, const string& devGrpGbid, string& strParentGrpId )
{
	if (strParentGrpId.empty()) //无父分组，则自身就是根分组，合法
	{
		return true;
	}
	TSqlString tSql;
	//找到父分组的newgbid
	tSql.Format("select %s from %s where %s='%s'",
		F_DEVGROUP_NEWGBID, DbStr(GetTblDevGrpName(devTreeId)), F_DEVGROUP_DEVGROUPID, DbStr(strParentGrpId));
	if (!ExecSql(tSql, enExecMayRet))
	{
		return false;
	}
	string strParentGrpGbid = string(GetString(F_DEVGROUP_NEWGBID));
	if (strParentGrpGbid.length() >= devGrpGbid.length())
	{
		return false;
	}
	if (devGrpGbid.find(strParentGrpGbid) == string::npos)
	{
		return false;
	}

	return true;
}

ENTasErrno CDbOpr::ModDeviceVidSrcName( const UUIDString& devId, int nIndex, const string& VidSrcName )
{
	TSqlString tSql;
	tSql.Format("update %s set %s='%s' where %s='%s' and %s=%d and %s=%d",
		T_DEVICECAPINDEXDATA,
		F_DEVICECAPINDEXDATA_DEVICECAPINDEXNAME, DbStr(VidSrcName),
		F_DEVICECAPINDEXDATA_DEVICEID,DbStr(devId),
		F_DEVICECAPINDEXDATA_DEVICECAPID, DEVICE_CAP_VIDEOSOURCE,
		F_DEVICECAPINDEXDATA_DEVICECAPINDEX, nIndex);
	if (!ExecSql(tSql, enExecNoRet))
	{
		return ERR_TAS_DBERROR;
	}
	return CMS_SUCCESS;
}



void CDbOpr::DevGrpDevOrder( const CTaskMsgTransfer<CDeviceGroupDeviceOrderReq> *pcMsg )
{
	CTaskMsgTransfer<CDeviceGroupDeviceOrderRsp> cTrans;
	CDeviceGroupDeviceOrderRsp &cRsp = cTrans.GetMsgObj();
	CTaskMsgTransfer<CDeviceGroupDeviceOrderNtfReq> cTransNtf;
	CDeviceGroupDeviceOrderNtfReq &cNtf = cTransNtf.GetMsgObj();

	const CDeviceGroupDeviceOrderReq &cReq = pcMsg->GetMsgObj();

	switch(cReq.GetVidsrcOrderInfo().GetOrderType())
	{
	case ENORDER_TYPE_INNER_DEVGRP_MOVE:
		{
			cRsp.SetErrorCode(DealInnerGrpMove(cReq));
		}
		break;
	case ENORDER_TYPE_INNER_DEVGRP_TOP:
		{
			cRsp.SetErrorCode(DealInnerGrpTop(cReq));
		}
		break;
	case ENORDER_TYPE_INNER_DEVGRP_BOTTOM:
		{
			cRsp.SetErrorCode(DealInnerGrpBottom(cReq));
		}
		break;
	case ENORDER_TYPE_INNER_DEVICE_MOVE:
		{
			cRsp.SetErrorCode(DealInnerDevMove(cReq));
		}
		break;
	case ENORDER_TYPE_INNER_DEVICE_TOP:
		{
			cRsp.SetErrorCode(DealInnerDevTop(cReq));
		}
		break;
	case ENORDER_TYPE_INNER_DEVICE_BOTTOM:
		{
			cRsp.SetErrorCode(DealInnerDevBottom(cReq));
		}
		break;
	default:
		{
			cRsp.SetErrorCode(ERR_TAS_INVALID_PARM);
		}
		break;
	}

	PostTaskMsgTransfer2CCtrl(*pcMsg, cReq, cRsp, cTrans);
}

bool CDbOpr::GetDevGrpNextDevSeq( const UUIDString &devTreeId, const UUIDString &devGrpId, const UUIDString& devId,int vidSrcIndex, double& devSeq )
{
	if (devGrpId.empty())
	{
		return false;
	}

	TSqlString tSql;
	if (!devTreeId.empty())
	{
		tSql.Format("select count(*) from %s where %s='%s'",T_DEVTREE, F_DEVTREE_ID, DbStr(devTreeId));
		if (!ExecSql(tSql, enExecMustRet))
		{
			return false;
		}
		if (GetIntResult()<=0)
		{
			return false;
		}
	}

	//该分组是否已有设备，无则认为此视频源是第一个设备，devSeq=0;
	tSql.Format("select count(*) from %s where %s='%s'", DbStr(GetTblDevGrp2DevName(devTreeId)), F_DEVGRP2DEV_DEVGROUPID,DbStr(devGrpId));
	if (!ExecSql(tSql, enExecMustRet))
	{
		return false;
	}
	if (GetIntResult()<=0)
	{
		devSeq=0; //认为该分组下当前尚无设备，故分组内第一个设备的序号设置为初始0
		return true;
	}
	else
	{
		//有，则先判断此分组下是否已有该设备的视频源，有则挑出最接近的index,查出对应的devSeq值。
		tSql.Format("select count(*) from %s where %s='%s' and %s='%s'",
			DbStr(GetTblDevGrp2DevName(devTreeId)), 
			F_DEVGRP2DEV_DEVGROUPID, DbStr(devGrpId),
			F_DEVGRP2DEV_DEVID,DbStr(devId));
		if (!ExecSql(tSql,enExecMustRet))
		{
			return false;
		}
		if (GetIntResult()<=0)
		{
			//该分组内有设备，但没有该设备的视频源，则直接拖到最后
			if (!QryRangeDevSeq(devTreeId, devGrpId, true, devSeq))
			{
				return false;
			}
			
			devSeq  += 512;
			return true;
		}
		else
		{
			return GetDevGrpNextDevSeq4SameDev(devTreeId, devGrpId, devId,vidSrcIndex, devSeq);
		}
	}
}

bool CDbOpr::GetDevGrpNextDevSeq4SameDev( const UUIDString &devTreeId, const UUIDString &devGrpId,const UUIDString& devId,int vidSrcIndex, double& devSeq )
{
	//该分组内已存在同设备的其他视频源
	bool bNoSmaller = false, bNoLarger=false;
	u32 dwSmallerIndex,dwMoreIndex; //在该分组内最接近vidSrcNum的视频源
	double dSmallerDevSeq, dMoreDevSeq;

	TSqlString tSql;
	tSql.Format("select %s,%s from %s where %s in (select max(%s) from %s where %s<%d and %s='%s' and %s='%s') and %s='%s'",
		F_DEVGRP2DEV_DEVCAPINDEX, F_DEVGRP2DEV_DEVSEQ,DbStr(GetTblDevGrp2DevName(devTreeId)),F_DEVGRP2DEV_DEVCAPINDEX,
		F_DEVGRP2DEV_DEVCAPINDEX, DbStr(GetTblDevGrp2DevName(devTreeId)),
		F_DEVGRP2DEV_DEVCAPINDEX, vidSrcIndex,
		F_DEVGRP2DEV_DEVGROUPID, DbStr(devGrpId),
		F_DEVGRP2DEV_DEVID, DbStr(devId),
		F_DEVGRP2DEV_DEVID, DbStr(devId));
	if (!ExecSql(tSql,enExecMayRet))
	{
		return false;
	}
	else
	{
		if (IsEof())
		{
			bNoSmaller=true;
		}
		else
		{
			dwSmallerIndex = GetInt(F_DEVGRP2DEV_DEVCAPINDEX);
			dSmallerDevSeq = GetDouble(F_DEVGRP2DEV_DEVSEQ);
		}
	}


	tSql.Format("select %s,%s from %s where %s in (select min(%s) from %s where %s>%d and %s='%s' and %s='%s') and %s='%s'",
		F_DEVGRP2DEV_DEVCAPINDEX, F_DEVGRP2DEV_DEVSEQ,DbStr(GetTblDevGrp2DevName(devTreeId)),F_DEVGRP2DEV_DEVCAPINDEX,
		F_DEVGRP2DEV_DEVCAPINDEX, DbStr(GetTblDevGrp2DevName(devTreeId)),
		F_DEVGRP2DEV_DEVCAPINDEX, vidSrcIndex,
		F_DEVGRP2DEV_DEVGROUPID, DbStr(devGrpId),
		F_DEVGRP2DEV_DEVID, DbStr(devId),
		F_DEVGRP2DEV_DEVID, DbStr(devId));
	if (!ExecSql(tSql,enExecMayRet))
	{
		return false;
	}
	else
	{
		if (IsEof())
		{
			if (bNoSmaller)
			{
				return false; //与前置条件不符合（前置条件：此分组内已有该设备的其他视频源）
			}
		    bNoLarger = true;
		}
		else
		{
			dwMoreIndex = GetInt(F_DEVGRP2DEV_DEVCAPINDEX);
			dMoreDevSeq = GetDouble(F_DEVGRP2DEV_DEVSEQ);
		}
	}

	if (bNoLarger)//分组内，没有该设备的序号更大的视频源
	{
		devSeq=dSmallerDevSeq;
		if (QryNeerDevSeq(devTreeId, devGrpId, dSmallerDevSeq, true, devSeq))
		{
			devSeq = dSmallerDevSeq+(devSeq-dSmallerDevSeq)/2;
			if (FLOAT_EQU(dSmallerDevSeq, devSeq))
			{
				DisperseDevSeq(devTreeId, devGrpId);
				return GetDevGrpNextDevSeq4SameDev(devTreeId, devGrpId, devId, vidSrcIndex, devSeq);
			}
		}
		else
		{
			//分组内没有比dSmallerDevSeq更大的值
			devSeq=dSmallerDevSeq+(vidSrcIndex-dwSmallerIndex)*512;
		}
	}
	else if(bNoSmaller)
	{
		if (QryNeerDevSeq(devTreeId, devGrpId, dMoreDevSeq, false, devSeq))
		{
			devSeq = dMoreDevSeq-(dMoreDevSeq-devSeq)/2;
			if (FLOAT_EQU(dMoreDevSeq, devSeq))
			{
				DisperseDevSeq(devTreeId, devGrpId);
				return GetDevGrpNextDevSeq4SameDev(devTreeId, devGrpId, devId, vidSrcIndex, devSeq);
			}
		}
		else
		{
			devSeq=dMoreDevSeq-(dwMoreIndex-vidSrcIndex)*512;
		}
	}
	else
	{
		if (FLOAT_EQU(dSmallerDevSeq, dMoreDevSeq))
		{
			DisperseDevSeq(devTreeId, devGrpId);
			return GetDevGrpNextDevSeq4SameDev(devTreeId, devGrpId, devId, vidSrcIndex, devSeq);
		}

		devSeq = dSmallerDevSeq+(dMoreDevSeq-dSmallerDevSeq)/2;
	}
	return true;
}

ENTasErrno CDbOpr::SetVidSrcDevSeq( const UUIDString& devTreeId, const UUIDString& devGrpId, const UUIDString& devId, const int vidSrc,const double dDevSeq )
{
	TSqlString tSql;
	tSql.Format("update %s set %s=%.14lf where %s='%s' and %s='%s' and %s=%d and %s=%d",
		DbStr(GetTblDevGrp2DevName(devTreeId)), 
		F_DEVGRP2DEV_DEVSEQ, dDevSeq,
		F_DEVGRP2DEV_DEVGROUPID, DbStr(devGrpId),
		F_DEVGRP2DEV_DEVID, DbStr(devId),
		F_DEVGRP2DEV_DEVCAPID, DEVICE_CAP_VIDEOSOURCE,
		F_DEVGRP2DEV_DEVCAPINDEX, vidSrc);
	if(!ExecSql(tSql, enExecNoRet))
	{
		return ERR_TAS_DBERROR;
	}

	CDeviceInfo devInfo;
	if (CMS_SUCCESS == DeviceItemQry(devId, devInfo))
	{
		if (devTreeId.empty())
		{
			SyncAnalyse(enSyncDataDevGrpDevSeq, enSyncOperateMod, devInfo.GetDomainUUID(), &devGrpId, &devId, vidSrc, dDevSeq, 
				DEVICE_TYPE_ENCODER);
		}
	}

	return CMS_SUCCESS;
}

bool CDbOpr::QryVidSrcDevSeq( const UUIDString& devTreeId, const UUIDString& devGrpId, const UUIDString& devId, const int vidSrc, double &dDevSeq )
{
	TSqlString tSql;
	tSql.Format("select %s from %s where %s='%s' and %s='%s' and %s=%d and %s=%d",
		F_DEVGRP2DEV_DEVSEQ,
		DbStr(GetTblDevGrp2DevName(devTreeId)), 
		F_DEVGRP2DEV_DEVGROUPID, DbStr(devGrpId),
		F_DEVGRP2DEV_DEVID, DbStr(devId),
		F_DEVGRP2DEV_DEVCAPID, DEVICE_CAP_VIDEOSOURCE,
		F_DEVGRP2DEV_DEVCAPINDEX, vidSrc);
	if(!ExecSql(tSql, enExecMayRet))
	{
		return false;
	}

	if (IsNotEof())
	{
		dDevSeq = GetDouble(F_DEVGRP2DEV_DEVSEQ);
		return true;
	}

	return false;
}

ENTasErrno CDbOpr::DealInnerGrpTop( const CDeviceGroupDeviceOrderReq& cReq )
{
	double devSeq;
	TSqlString tSql;
	//首先找到分组内排在第一个位置视频源的devSeq值
	if (!QryRangeDevSeq(cReq.GetDevTreeId(),cReq.GetVidsrcOrderInfo().GetDevGrpId(),
		false, devSeq))
	{
		return ERR_TAS_DBERROR;
	}
	ENTasErrno errCode=CMS_SUCCESS;

	//统计分组内需要置顶的视频源
	vector<int> vecVidsrcIndex;
	vector<double> vecVidsrcDevSeq;
	if (-1 == cReq.GetVidsrcOrderInfo().GetVidsrcIndexMoved()) //整个设备移动
	{
		tSql.Format("select %s from %s where %s='%s' and %s='%s' and %s=%d order by %s asc",
			F_DEVGRP2DEV_DEVCAPINDEX, DbStr(GetTblDevGrp2DevName(cReq.GetDevTreeId())),
			F_DEVGRP2DEV_DEVGROUPID, DbStr(cReq.GetVidsrcOrderInfo().GetDevGrpId()),
			F_DEVGRP2DEV_DEVID, DbStr(cReq.GetVidsrcOrderInfo().GetDevMovedId()),
			F_DEVGRP2DEV_DEVCAPID, DEVICE_CAP_VIDEOSOURCE,
			F_DEVGRP2DEV_DEVSEQ);
		if (!ExecSql(tSql, enExecMayRet))
		{
			return ERR_TAS_DBERROR;
		}
		while(IsNotEof())
		{
			vecVidsrcIndex.push_back(GetInt(F_DEVGRP2DEV_DEVCAPINDEX));
			MoveNext();
		}
	}
	else
	{
		vecVidsrcIndex.push_back(cReq.GetVidsrcOrderInfo().GetVidsrcIndexMoved());
	}

	//使用当前分组min(devSeq)-vidSrcDevSeq（正值，且相对大小不变）作为新的vidSrcDevSeq
	u32 dwVidSrcNum = vecVidsrcIndex.size();
	double dlargerDevSeq = (dwVidSrcNum+1)*(fabs(devSeq)+1);
	for (int i = 0;i<dwVidSrcNum;++i)
	{
		errCode  = SetVidSrcDevSeq(cReq.GetDevTreeId(), cReq.GetVidsrcOrderInfo().GetDevGrpId(), 
			cReq.GetVidsrcOrderInfo().GetDevMovedId(),vecVidsrcIndex[i],devSeq-dlargerDevSeq);
		if (errCode != CMS_SUCCESS)
		{
			return errCode;
		}
		dlargerDevSeq -= (fabs(devSeq)+1);
	}
	return errCode;
}

ENTasErrno CDbOpr::DealInnerGrpBottom( const CDeviceGroupDeviceOrderReq& cReq )
{
	TSqlString tSql;
	double devSeq;
	if (!QryRangeDevSeq(cReq.GetDevTreeId(),cReq.GetVidsrcOrderInfo().GetDevGrpId(),
		true, devSeq))
	{
		return ERR_TAS_DBERROR;
	}
	
	ENTasErrno errCode=CMS_SUCCESS;
	vector<int> vecVidSrcIndexs;
	vector<double> vecVidSrcDevSeq;
	if (-1 == cReq.GetVidsrcOrderInfo().GetVidsrcIndexMoved()) //整个设备移动
	{
		tSql.Format("select %s from %s where %s='%s' and %s='%s' and %s=%d order by %s asc",
			F_DEVGRP2DEV_DEVCAPINDEX,DbStr(GetTblDevGrp2DevName(cReq.GetDevTreeId())),
			F_DEVGRP2DEV_DEVGROUPID, DbStr(cReq.GetVidsrcOrderInfo().GetDevGrpId()),
			F_DEVGRP2DEV_DEVID, DbStr(cReq.GetVidsrcOrderInfo().GetDevMovedId()),
			F_DEVGRP2DEV_DEVCAPID, DEVICE_CAP_VIDEOSOURCE,
			F_DEVGRP2DEV_DEVSEQ);
		if (!ExecSql(tSql, enExecMayRet))
		{
			return ERR_TAS_DBERROR;
		}
		while(IsNotEof())
		{
			vecVidSrcIndexs.push_back(GetInt(F_DEVGRP2DEV_DEVCAPINDEX));
			MoveNext();
		}
	}
	else
	{
		vecVidSrcIndexs.push_back(cReq.GetVidsrcOrderInfo().GetVidsrcIndexMoved());
	}
	//对这些视频源的devSeq值进行正值离散（相对大小不变）
	u32 dwVidSrcNum = vecVidSrcIndexs.size();
	double dlargerDevSeq = fabs(devSeq)+1;
	for (int i = 0; i < dwVidSrcNum; ++i)
	{
		errCode  = SetVidSrcDevSeq(cReq.GetDevTreeId(), cReq.GetVidsrcOrderInfo().GetDevGrpId(), 
			cReq.GetVidsrcOrderInfo().GetDevMovedId(),vecVidSrcIndexs[i],devSeq+dlargerDevSeq);
		if (errCode != CMS_SUCCESS)
		{
			break;
		}
		dlargerDevSeq += (fabs(devSeq)+1);
	}
	return errCode;
}

ENTasErrno CDbOpr::DealInnerGrpMove( const CDeviceGroupDeviceOrderReq& cReq )
{
	double devSeqRefer,devSeq;
	TSqlString tSql;
	if (-1==cReq.GetVidsrcOrderInfo().GetVidsrcIndexRefer())
	{
		tSql.Format("select max(%s) as DevSeq from %s where %s='%s' and %s='%s' and %s=%d",
			F_DEVGRP2DEV_DEVSEQ, DbStr(GetTblDevGrp2DevName(cReq.GetDevTreeId())),
			F_DEVGRP2DEV_DEVGROUPID, DbStr(cReq.GetVidsrcOrderInfo().GetDevGrpId()),
			F_DEVGRP2DEV_DEVID, DbStr(cReq.GetVidsrcOrderInfo().GetDevReferId()),
			F_DEVGRP2DEV_DEVCAPID, DEVICE_CAP_VIDEOSOURCE);
	}
	else
	{
		tSql.Format("select %s as DevSeq from %s where %s='%s' and %s='%s' and %s=%d and %s=%d",
			F_DEVGRP2DEV_DEVSEQ, DbStr(GetTblDevGrp2DevName(cReq.GetDevTreeId())),
			F_DEVGRP2DEV_DEVGROUPID, DbStr(cReq.GetVidsrcOrderInfo().GetDevGrpId()),
			F_DEVGRP2DEV_DEVID, DbStr(cReq.GetVidsrcOrderInfo().GetDevReferId()),
			F_DEVGRP2DEV_DEVCAPID, DEVICE_CAP_VIDEOSOURCE,
			F_DEVGRP2DEV_DEVCAPINDEX, cReq.GetVidsrcOrderInfo().GetVidsrcIndexRefer());
	}
	
	if (!ExecSql(tSql, enExecMayRet))
	{
		return ERR_TAS_DBERROR;
	}
	if (IsEof())
	{
		return ERR_TAS_RECORD_NOT_EXISTS;
	}
	devSeqRefer=GetDouble("DevSeq");
	
	//此情景下分组内必然已经有视频源
	if (!QryNeerDevSeq(cReq.GetDevTreeId(), cReq.GetVidsrcOrderInfo().GetDevGrpId(),
		devSeqRefer, true, devSeq))
	{
		devSeq = devSeqRefer+512;
	}
	
	map<int,double> mapVidSrcDevSeq;
	vector<int> vecVidSrc;
	if (-1==cReq.GetVidsrcOrderInfo().GetVidsrcIndexMoved())
	{
		double devSeqTmp;
		TSqlString tSql;
		tSql.Format("select %s from %s where %s='%s' and %s='%s' order by %s asc",
			F_DEVGRP2DEV_DEVCAPINDEX, DbStr(GetTblDevGrp2DevName(cReq.GetDevTreeId())),
			F_DEVGRP2DEV_DEVGROUPID, DbStr(cReq.GetVidsrcOrderInfo().GetDevGrpId()),
			F_DEVGRP2DEV_DEVID, DbStr(cReq.GetVidsrcOrderInfo().GetDevMovedId()),
			F_DEVGRP2DEV_DEVSEQ);
		if (!ExecSql(tSql, enExecMayRet))
		{
			return ERR_TAS_DBERROR;
		}
		while(IsNotEof())
		{
			vecVidSrc.push_back(GetInt(F_DEVGRP2DEV_DEVCAPINDEX));
			MoveNext();
		}
		if (vecVidSrc.size()==0)
		{
			return ERR_TAS_RECORD_NOT_EXISTS;
		}
		if (FLOAT_EQU(devSeqRefer,devSeqRefer+(devSeq-devSeqRefer)/(vecVidSrc.size()+1)))
		{
			DisperseDevSeq(cReq.GetDevTreeId(), cReq.GetVidsrcOrderInfo().GetDevGrpId());
			return DealInnerGrpMove(cReq);
		}
		devSeqTmp = devSeqRefer;
		for (int i = 0; i<vecVidSrc.size(); ++i)
		{
			devSeqTmp += fabs((devSeq-devSeqRefer)/(vecVidSrc.size()+1));
			mapVidSrcDevSeq.insert(std::pair<int,double>(vecVidSrc[i], devSeqTmp));
		}
	}
	else
	{
		mapVidSrcDevSeq.insert(std::pair<int,double>(cReq.GetVidsrcOrderInfo().GetVidsrcIndexMoved(), devSeq));
	}
	ENTasErrno errCode = CMS_SUCCESS;
	for (map<int,double> ::iterator it = mapVidSrcDevSeq.begin(); it != mapVidSrcDevSeq.end();++it)
	{
		errCode = SetVidSrcDevSeq(cReq.GetDevTreeId(), cReq.GetVidsrcOrderInfo().GetDevGrpId(),
			cReq.GetVidsrcOrderInfo().GetDevMovedId(), it->first,it->second);
		if (CMS_SUCCESS != errCode)
		{
			return errCode;
		}
	}
	return errCode;
}

ENTasErrno CDbOpr::DealInnerDevTop( const CDeviceGroupDeviceOrderReq& cReq )
{
	if (-1 == cReq.GetVidsrcOrderInfo().GetVidsrcIndexMoved()) //整个设备移动
	{
		return ERR_TAS_INVALID_OPERATION;
	}

	double devSeqRefer=0, devSeq=0;
	if (!QryRangeDevSeq(cReq.GetDevTreeId(), cReq.GetVidsrcOrderInfo().GetDevGrpId(),false, devSeqRefer, 
		cReq.GetVidsrcOrderInfo().GetDevMovedId()))
	{
		return ERR_TAS_RECORD_NOT_EXISTS;
	}
	if (!QryNeerDevSeq(cReq.GetDevTreeId(), cReq.GetVidsrcOrderInfo().GetDevGrpId(), devSeqRefer, false, devSeq))
	{
		devSeq = devSeqRefer-512;
	}
	
	devSeq = devSeqRefer-fabs(devSeqRefer-devSeq)/2;
	if (FLOAT_EQU(devSeq, devSeqRefer))
	{
		DisperseDevSeq(cReq.GetDevTreeId(), cReq.GetVidsrcOrderInfo().GetDevGrpId());
		return DealInnerDevTop(cReq);
	}
	
	return SetVidSrcDevSeq(cReq.GetDevTreeId(), cReq.GetVidsrcOrderInfo().GetDevGrpId(), 
			cReq.GetVidsrcOrderInfo().GetDevMovedId(),cReq.GetVidsrcOrderInfo().GetVidsrcIndexMoved(),devSeq);
	
}

ENTasErrno CDbOpr::DealInnerDevBottom( const CDeviceGroupDeviceOrderReq& cReq )
{
	//设备内置底，不一定是分组内置底
	if (-1 == cReq.GetVidsrcOrderInfo().GetVidsrcIndexMoved()) //整个设备移动
	{
		return ERR_TAS_INVALID_OPERATION;
	}
	double devSeqRefer=0,devSeq=0;
	if (!QryRangeDevSeq(cReq.GetDevTreeId(), cReq.GetVidsrcOrderInfo().GetDevGrpId(),true,devSeqRefer, 
		cReq.GetVidsrcOrderInfo().GetDevMovedId()))
	{
		return ERR_TAS_RECORD_NOT_EXISTS;
	}
	if (!QryNeerDevSeq(cReq.GetDevTreeId(), cReq.GetVidsrcOrderInfo().GetDevGrpId(),
		devSeqRefer, true, devSeq))
	{
		devSeq = devSeqRefer+512;
	}
	devSeq = devSeqRefer+fabs(devSeq-devSeqRefer)/2;
	if (FLOAT_EQU(devSeqRefer, devSeq))
	{
		DisperseDevSeq(cReq.GetDevTreeId(),cReq.GetVidsrcOrderInfo().GetDevGrpId());
		return DealInnerDevBottom(cReq);
	}
	
	return SetVidSrcDevSeq(cReq.GetDevTreeId(), cReq.GetVidsrcOrderInfo().GetDevGrpId(), 
		cReq.GetVidsrcOrderInfo().GetDevMovedId(),cReq.GetVidsrcOrderInfo().GetVidsrcIndexMoved(),devSeq);
}

ENTasErrno CDbOpr::DealInnerDevMove( const CDeviceGroupDeviceOrderReq& cReq )
{
	double devSeqRefer=0, devSeq=0;
	TSqlString tSql;
	tSql.Format("select %s from %s where %s='%s' and %s='%s' and %s=%d and %s=%d",
		F_DEVGRP2DEV_DEVSEQ, DbStr(GetTblDevGrp2DevName(cReq.GetDevTreeId())),
		F_DEVGRP2DEV_DEVGROUPID, DbStr(cReq.GetVidsrcOrderInfo().GetDevGrpId()),
		F_DEVGRP2DEV_DEVID, DbStr(cReq.GetVidsrcOrderInfo().GetDevReferId()),
		F_DEVGRP2DEV_DEVCAPID, DEVICE_CAP_VIDEOSOURCE,
		F_DEVGRP2DEV_DEVCAPINDEX, cReq.GetVidsrcOrderInfo().GetVidsrcIndexRefer());
	if (!ExecSql(tSql, enExecMayRet))
	{
		return ERR_TAS_DBERROR;
	}
	devSeqRefer = GetDouble(F_DEVGRP2DEV_DEVSEQ);
	if (!QryNeerDevSeq(cReq.GetDevTreeId(), cReq.GetVidsrcOrderInfo().GetDevGrpId(),
		devSeqRefer, true, devSeq))
	{
		devSeq = devSeqRefer+512;
	}
	devSeq = devSeqRefer+fabs(devSeq-devSeqRefer)/2;
	if (FLOAT_EQU(devSeq, devSeqRefer))
	{
		DisperseDevSeq(cReq.GetDevTreeId(), cReq.GetVidsrcOrderInfo().GetDevGrpId());
		return DealInnerDevMove(cReq);
	}
	return SetVidSrcDevSeq(cReq.GetDevTreeId(), cReq.GetVidsrcOrderInfo().GetDevGrpId(), 
		cReq.GetVidsrcOrderInfo().GetDevMovedId(),cReq.GetVidsrcOrderInfo().GetVidsrcIndexMoved(),devSeq);
}

ENTasErrno CDbOpr::DisperseDevSeq( const UUIDString &devTreeId, const UUIDString &devGrpId)
{
	TSqlString tsql;

	tsql.Format("call %s('%s','%s')",P_DISPERSEDEVSEQ,DbStr(devTreeId), DbStr(devGrpId));
	if(!ExecSql(tsql, enExecNoRet))
	{
		return ERR_TAS_DBERROR;
	}

	if (devTreeId.empty())
	{
		//查处改分组下所有设备的devSeq，通知到需同步的对端（上级tas）
		string strDevId;
		int nCapIndex;
		double dDevSeq;
		
		tsql.Format("select %s,%s,%s from %s where %s='%s' and %s=%d",
			F_DEVGRP2DEV_DEVID, F_DEVGRP2DEV_DEVCAPINDEX, F_DEVGRP2DEV_DEVSEQ,
			DbStr(GetTblDevGrp2DevName(devTreeId)), 
			F_DEVGRP2DEV_DEVGROUPID, DbStr(devGrpId),
			F_DEVGRP2DEV_DEVCAPID,DEVICE_CAP_VIDEOSOURCE);
		if (ExecSql(tsql, enExecMayRet))
		{
			while(IsNotEof())
			{
				strDevId = GetString(F_DEVGRP2DEV_DEVID);
				nCapIndex = GetInt(F_DEVGRP2DEV_DEVCAPINDEX);
				dDevSeq = GetDouble(F_DEVGRP2DEV_DEVSEQ);

				SyncAnalyse(enSyncDataDevGrpDevSeq, enSyncOperateMod, CurrentDomainId(), &devGrpId, &strDevId, nCapIndex, dDevSeq, DEVICE_TYPE_ENCODER);
			
				MoveNext();
			}
		}
	}

	return CMS_SUCCESS;
}

bool CDbOpr::QryNeerDevSeq( const UUIDString& devTreeId, const UUIDString& devGrpId, const double referDevSeq, bool bLarger, /*true--选择大于referDevSeq的值，false--选择小于referDevSeq的值。 */ double& devSeq )
{
	TSqlString tSql;

	if (bLarger)
	{
		tSql.Format("select count(*) from %s where %s='%s' and %s>%.14lf",
			DbStr(GetTblDevGrp2DevName(devTreeId)),
			F_DEVGRP2DEV_DEVGROUPID, DbStr(devGrpId),
			F_DEVGRP2DEV_DEVSEQ, referDevSeq);
	}
	else
	{
		tSql.Format("select count(*) from %s where %s='%s' and %s<%.14lf",
			DbStr(GetTblDevGrp2DevName(devTreeId)),
			F_DEVGRP2DEV_DEVGROUPID, DbStr(devGrpId),
			F_DEVGRP2DEV_DEVSEQ, referDevSeq);
	}
	ExecSql(tSql, enExecMayRet);
	if (GetIntResult()<=0)
	{
		return false;
	}


	if (bLarger)
	{
		tSql.Format("select min(%s) as DevSeq from %s where %s='%s' and %s>%.14lf",
			F_DEVGRP2DEV_DEVSEQ, DbStr(GetTblDevGrp2DevName(devTreeId)),
			F_DEVGRP2DEV_DEVGROUPID, DbStr(devGrpId),
			F_DEVGRP2DEV_DEVSEQ, referDevSeq);
	}
	else
	{
		tSql.Format("select max(%s) as DevSeq from %s where %s='%s' and %s<%.14lf",
			F_DEVGRP2DEV_DEVSEQ, DbStr(GetTblDevGrp2DevName(devTreeId)),
			F_DEVGRP2DEV_DEVGROUPID, DbStr(devGrpId),
			F_DEVGRP2DEV_DEVSEQ, referDevSeq);
	}
	ExecSql(tSql, enExecMayRet);
	devSeq = GetDouble("DevSeq");
	return true;
}

bool CDbOpr::QryRangeDevSeq( const UUIDString& devTreeId, const UUIDString& devGrpId,bool bMax,double& devSeq,const UUIDString devId )
{
	TSqlString tSql;
	if (devId.empty())
	{
		tSql.Format("select count(*) from %s where %s='%s'",
			DbStr(GetTblDevGrp2DevName(devTreeId)),
			F_DEVGRP2DEV_DEVGROUPID, DbStr(devGrpId));
	}
	else
	{
		tSql.Format("select count(*) from %s where %s='%s' and %s='%s'",
			DbStr(GetTblDevGrp2DevName(devTreeId)),
			F_DEVGRP2DEV_DEVGROUPID, DbStr(devGrpId),
			F_DEVGRP2DEV_DEVID, DbStr(devId));
	}
	ExecSql(tSql, enExecMustRet);
	if (GetIntResult()<=0)
	{
		return false;
	}


	if (devId.empty())
	{
		if (bMax)
		{
			tSql.Format("select max(%s) as DevSeq from %s where %s='%s'",
				F_DEVGRP2DEV_DEVSEQ, DbStr(GetTblDevGrp2DevName(devTreeId)),
				F_DEVGRP2DEV_DEVGROUPID, DbStr(devGrpId));
		}
		else
		{
			tSql.Format("select min(%s) as DevSeq from %s where %s='%s'",
				F_DEVGRP2DEV_DEVSEQ, DbStr(GetTblDevGrp2DevName(devTreeId)),
				F_DEVGRP2DEV_DEVGROUPID, DbStr(devGrpId));
		}
	}
	else
	{
		if (bMax)
		{
			tSql.Format("select max(%s) as DevSeq from %s where %s='%s' and %s='%s'",
				F_DEVGRP2DEV_DEVSEQ, DbStr(GetTblDevGrp2DevName(devTreeId)),
				F_DEVGRP2DEV_DEVGROUPID, DbStr(devGrpId),
				F_DEVGRP2DEV_DEVID, DbStr(devId));
		}
		else
		{
			tSql.Format("select min(%s) as DevSeq from %s where %s='%s' and %s='%s'",
				F_DEVGRP2DEV_DEVSEQ, DbStr(GetTblDevGrp2DevName(devTreeId)),
				F_DEVGRP2DEV_DEVGROUPID, DbStr(devGrpId),
				F_DEVGRP2DEV_DEVID, DbStr(devId));
		}
	}

	if (!ExecSql(tSql, enExecMayRet))
	{
		return false;
	}
	devSeq=GetDouble("DevSeq");
	return true;
}

bool CDbOpr::IsVidSrcInDevGrp( const UUIDString& devTreeId, const UUIDString& devGrpId, const UUIDString& devId, const u32 dwVidSrcIndex )
{
	TSqlString tSql;
	tSql.Format("select count(*) from %s where %s='%s' and %s='%s' and %s=%d",
		DbStr(GetTblDevGrp2DevName(devTreeId)), 
		F_DEVGRP2DEV_DEVGROUPID, DbStr(devGrpId),
		F_DEVGRP2DEV_DEVID, DbStr(devId),
		F_DEVGRP2DEV_DEVCAPINDEX, dwVidSrcIndex);
	if (!ExecSql(tSql, enExecMustRet))
	{
		return false;
	}
	if (GetIntResult()>0)
	{
		return true;
	}
	return false;
}

ENTasErrno CDbOpr::SyncDataStoreDevGrp2DevSeq( const string &strSyncDataDomainId, const CDeviceGroupDevSeqInfo &tSyncData, ENSyncOperateType operateType, ENSyncType enSyncType )
{
	// 设备组和设备必须同时存在
	ENTasErrno enErr;

	CDeviceGroupInfo cDevGrpInfo;
	enErr = DevGrpItemQry(tSyncData.GetDevGrpId(), cDevGrpInfo);
	if (CMS_SUCCESS == enErr)
	{
		// 正常，可以进行后续处理
	}
	else if (ERR_TAS_RECORD_NOT_EXISTS == enErr)
	{
		return ERR_TAS_INVALID_SYNCDATA; // 比如对方多发送过来的数据
	}
	else
	{
		return enErr;
	}

	CDeviceInfo cDevInfo;
	enErr = DeviceItemQry(tSyncData.GetDevId(), cDevInfo);
	if (CMS_SUCCESS == enErr)
	{
		// 正常，可以进行后续处理
	}
	else if (ERR_TAS_RECORD_NOT_EXISTS == enErr)
	{
		return ERR_TAS_INVALID_SYNCDATA; // 比如对方多发送过来的数据
	}
	else
	{
		return enErr;
	}

	if (enSyncOperateMod == operateType)
	{
		UpdateDevTreeVer(tSyncData.GetDevId());
		string strTreeId="";
		ENTasErrno errCode = SetVidSrcDevSeq(strTreeId, tSyncData.GetDevGrpId(), tSyncData.GetDevId(), tSyncData.GetVidSrcIdx(), tSyncData.GetDevSeq());
		if (CMS_SUCCESS != errCode)
		{
			return errCode;
		}
		return CMS_SUCCESS;
	}
	else 
	{
		return ERR_TAS_INVALID_PARM;
	}
}

void FDBRedisStatuCallback(CFastDBInterface */*pInter*/, EFDB_StatusCode sc, void */*reserve*/, void */*userdata*/)
{
	if (sc != SC_DISCONNECTED)
	{
		return;
	} 

	GLOBELOG(ERROR_LEV, "FDBRedisStatuCallback detected disconnection ! \n");
}

bool CDbOpr::InitFastDb()
{
	FDB_ConnInfo fdbConn;
	fdbConn.pfStatusCallBack = FDBRedisStatuCallback;
	fdbConn.clientname = "tas_dbtask";
	fdbConn.username = "NA";
	fdbConn.password = GetRedisPassword();

	EFDB_ErrorCode enErr = m_cFastDb.Connect(GetRedisIP(), GetRedisPort(), fdbConn);
	if (enErr != EC_OK)
	{
		printf("redis connect failed, errno=%d.redis ip:%s, port:%d, password:%s.\n", 
			(s32)enErr, GetRedisIP().c_str(), (s32)GetRedisPort(), GetRedisPassword().c_str());
		return false;
	}

	return true;
}

bool CDbOpr::QuitFastDb()
{
	m_cFastDb.Disconnect();
}

bool CDbOpr::DumpFastDb()
{
	OspPrintf(TRUE, FALSE, "redis-connect[%d] \n", m_cFastDb.IsConnected());
}

bool CDbOpr::GetLocalDomainGbId(string& strLocalDomainGbId)
{
	return EC_OK == m_cFastDb.Get(FDB_KEY_GBS_LOCALDOMAIN_GBID, strLocalDomainGbId);
}

bool GetCurrentCuserId( const string &strSessionId, string &strUserId )
{
    return dynamic_cast<CCtrlAppData&>(g_cCtrlApp.GetAppData()).GetCuserId(strSessionId, strUserId);
}


string GetPinYinFromUtf8(const string& strUtf8)
{
	string searchName=Utf8ToPinyin(strUtf8);
	if (searchName.size() > MAX_SEARCH_NAME_LEN)
	{
		searchName = searchName.substr(0,MAX_SEARCH_NAME_LEN);
	}
	return searchName;
}