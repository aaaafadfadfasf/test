#include "dbengine.h"
#include "osp.h"
#include "inner_common.h"

static ENDBType s_enDbType = enDBTypeMysql;

/*=============================================================================
    函 数 名： DBGlobalCBProc
    功    能： DB全局函数回调
    算法实现： 
    全局变量： 
    参    数： u16 wErrNum
               LPCSTR lpSrvMsg
               void *pContext   由使用者指定，用以区分是哪个连接出错
    返 回 值： void 
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    20111108    1.0			胡志云                  创建
=============================================================================*/
void DBGlobalCBProc(u16 wErrNum, LPCSTR lpSrvMsg, void *pContext)
{
	if (NULL == lpSrvMsg)
	{
		return;
	}

	OspPrintf(TRUE, FALSE, "\n\nDBGlobalCBProc Start*******************************************\n");
	OspPrintf(TRUE, FALSE, "ErrNum:%d\n", wErrNum);
	OspPrintf(TRUE, FALSE, "SrvMsg:%s\n", lpSrvMsg);
	OspPrintf(TRUE, FALSE, "DBGlobalCBProc End*******************************************\n\n");
}

/*=============================================================================
    函 数 名： InitalTASDBEngine
    功    能： DB数据库初始化，设置全局回调以及缺省的连接的全局操作超时时间
    算法实现： 
    全局变量： 
    参    数： u8 byDBEngineType 数据库引擎类型
    返 回 值： bool 
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
=============================================================================*/
bool InitalDBEngine(ENDBType enDBType)
{
    do 
    {
        if (!DBInit(static_cast<u8>(enDBType)))
        {
            GLOBELOG(enLogSrcSystem, ERROR_LEV, "DBInit failed\n");
            break;
        }
        if (!SetDBLocale("", DB_CHARSET_UTF8)) // hzytodo2 第一个参数只有mysql是这样设置的
        {
            GLOBELOG(enLogSrcSystem, ERROR_LEV, "SetDbLocale failed\n");
            break;
        }
        //if (!SetupLibErrFuc(DBGlobalCBProc, NULL))
        //{
        //    GLOBELOG(enLogSrcSystem, ERROR_LEV, "SetupLibErrFunc failed\n");
        //    break;
        //}EDB_PARAM_WRITE_TIMEOUT
		u8 byTimeoutSec=600;
		if (!SetDBParam(EDB_PARAM_READ_TIMEOUT, &byTimeoutSec) || !SetDBParam(EDB_PARAM_WRITE_TIMEOUT, &byTimeoutSec))
		{
			GLOBELOG(enLogSrcSystem, ERROR_LEV, "SetDBParam failed\n");
			break;
		}
        if (!SetDBTimeOut(TAS_DBOPR_TIMEOUT_DEFAULTSPAN))
        {
            GLOBELOG(enLogSrcSystem, ERROR_LEV, "SetDbTimeout failed\n");
            break;
        }

        s_enDbType = enDBType;
        return true;
    } while (false);

    return false;
}

/*=============================================================================
    函 数 名： ExitDB
    功    能： 退出DB数据库
    算法实现： 
    全局变量： 
    参    数： void
    返 回 值： bool 
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
=============================================================================*/
bool ExitDBEngine(void)
{
	return (TRUE == DBExit());
}

ENDBType GetEngineDBType()
{
    return s_enDbType;
}

//构造
CDBEngine::CDBEngine()
{
	m_pcDBOprer      = NULL;
	m_pcDBResultSet  = NULL;
	m_bConnectDB     = false;
	m_hDataFile      = NULL;
}

//析构
CDBEngine::~CDBEngine()
{
	DisconnectDB();
	SAFE_DELETE(m_pcDBOprer)
	SAFE_DELETE(m_pcDBResultSet)
}

/*=============================================================================
    函 数 名： ConnectDB
    功    能： 创建相应连接/命令对象，并连接数据库
    算法实现： 
    全局变量： 
    参    数： const TDBConnectInfo *ptDBConnectInfo
    返 回 值： u16 错误码
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    20111108    1.0			胡志云                  创建
=============================================================================*/
u16 CDBEngine::ConnectDB(const TDBConnectInfo *ptDBConnectInfo)
{
	if (IsConnectDB())
	{
		return SUCCESS_INTER_DB;
	}

	if (NULL == ptDBConnectInfo)
	{
		return ERR_INTER_DB_INVALID_PARAM;
	}
	
	if (NULL == m_pcDBOprer)
	{
		m_pcDBOprer = new CDBHandler;
		if (NULL == m_pcDBOprer)
		{
			return ERR_INTER_DB_ALLOC_MEMORY;
		}
	}
	if (NULL == m_pcDBResultSet)
	{
		m_pcDBResultSet = new CResultSet;
		if (NULL == m_pcDBResultSet)
		{
			return ERR_INTER_DB_ALLOC_MEMORY;
		}
	}

	BOOL32 bRet = FALSE;

	bRet = m_pcDBOprer->SetupSrvErrFuc(&DBServerCBProc, this);
	if (FALSE == bRet)
	{
		return ERR_INTER_DB_SET_CBDATA;
	}
	bRet = m_pcDBOprer->SetupCtErrFuc(&DBClientCBProc, this);
	if (FALSE == bRet)
	{
		return ERR_INTER_DB_SET_CBDATA;
	}
	bRet = m_pcDBOprer->ConnecttoSrv(ptDBConnectInfo->m_strDBUserName.c_str(), ptDBConnectInfo->m_strDBUserPwd.c_str(), 
		                             ptDBConnectInfo->m_strDBName.c_str(), 
								     ptDBConnectInfo->m_strDBIP.c_str(), ptDBConnectInfo->m_wDBPort);
	if (FALSE == bRet)
	{
		return ERR_INTER_DB_CONNECT_FAILED;
	}

	//设置为已连接状态
	m_bConnectDB = true;
	return SUCCESS_INTER_DB;
}

/*=============================================================================
    函 数 名： DisconnectDB
    功    能： 断开数据库，并清除相应连接/命令对象
    算法实现： 
    全局变量： 
    参    数： void
    返 回 值： u16 错误码  
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    20111108    1.0			胡志云                  创建
=============================================================================*/
u16 CDBEngine::DisconnectDB(void)
{
	u16 wRet = SUCCESS_INTER_DB;

	if (IsConnectDB())
	{
		BOOL32 bRet = m_pcDBOprer->CloseConnection();
		wRet = (TRUE == bRet) ? SUCCESS_INTER_DB : ERR_INTER_DB_DISCONNECT_FAILED;
	}

	//设置为断开状态
	m_bConnectDB     = false;

	return wRet;
}

/*=============================================================================
    函 数 名： IsConnectDB
    功    能： 
    算法实现： 
    全局变量： 
    参    数： 
    返 回 值： bool 
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    20111108    1.0			胡志云                  创建
=============================================================================*/
bool CDBEngine::IsConnectDB()
{
	if (NULL == m_pcDBOprer || NULL == m_pcDBResultSet)
	{
		return FALSE;
	}

	return m_bConnectDB;
}

/*=============================================================================
    函 数 名： ExecSql
    功    能： 执行Sql语句
    算法实现： 
    全局变量： 
    参    数： LPCSTR lpszSql
               bool bNeedRet = FALSE 是否返回数据集
    返 回 值： u16 错误码  
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    20111108    1.0			胡志云                  创建
=============================================================================*/
u16 CDBEngine::ExecSql(LPCSTR lpszSql, bool bNeedRet /*= FALSE*/)
{
	if (NULL == lpszSql)
	{
		return ERR_INTER_DB_INVALID_PARAM;
	}
	if (FALSE == IsConnectDB())
	{
		return ERR_INTER_DB_NOT_CONNECT;
	}

	BOOL32 bRet = FALSE;
	if (bNeedRet)
	{
		bRet = m_pcDBOprer->ExecSql(lpszSql, &m_pcDBResultSet->m_cDBRs);
		if (bRet)
		{
			if (!m_pcDBResultSet->MoveNext())
                bRet = FALSE;
		}
	}
	else
	{
		bRet = m_pcDBOprer->ExecSql(lpszSql);
	}

	if (FALSE == bRet)
	{
		return ERR_INTER_DB_EXECSQL_FAILED;
	}
	
	return SUCCESS_INTER_DB;
}

/*=============================================================================
    函 数 名： GetFieldCount
    功    能： 获取列数-结果集中字段总数
    算法实现： 
    全局变量： 
    参    数： 
    返 回 值： u16 结果集中字段总数
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    20111108    1.0			胡志云                  创建
=============================================================================*/
u16 CDBEngine::GetFieldCount()
{
	return m_pcDBResultSet->GetFieldCount();
}

/*=============================================================================
    函 数 名： GetCount
    功    能： 获取行数-结果集中记录总数
    算法实现： 
    全局变量： 
    参    数： 
    返 回 值： u32 结果集中记录总数
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2006/3/23   1.0			胡志云                  创建
=============================================================================*/
u32 CDBEngine::GetCount(void)
{
	u32 dwCount = m_pcDBResultSet->GetCount();
	return dwCount;
}

/*=============================================================================
    函 数 名： GetFieldValue
    功    能： 按字段名取字段值
    算法实现： 
    全局变量： 
    参    数： LPCSTR lpszName
               s32 &nValue
    返 回 值： u16 错误码  
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    20111108    1.0			胡志云                  创建
=============================================================================*/
u16 CDBEngine::GetFieldValue(LPCSTR lpszName, s32 &nValue)
{
	BOOL32 bRet = m_pcDBResultSet->m_cDBRs.GetFieldValue(lpszName, nValue);
	if (FALSE == bRet)
	{
		return ERR_INTER_DB_EXECSQL_FAILED;
	}
	
	return SUCCESS_INTER_DB;
}

/*=============================================================================
    函 数 名： GetFieldValue
    功    能： 按字段名取字段值
    算法实现： 
    全局变量： 
    参    数： LPCSTR lpszName
               u32 &dwValue
    返 回 值： u16 错误码  
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    20111108    1.0			胡志云                  创建
=============================================================================*/
u16 CDBEngine::GetFieldValue(LPCSTR lpszName, u32 &dwValue)
{
    BOOL32 bRet = m_pcDBResultSet->m_cDBRs.GetFieldValue(lpszName, dwValue);

	if (FALSE == bRet)
	{
		return ERR_INTER_DB_EXECSQL_FAILED;
	}
	
	return SUCCESS_INTER_DB;
}

/*=============================================================================
    函 数 名： GetFieldValue
    功    能： 按字段名取字段值
    算法实现： 
    全局变量： 
    参    数： LPCSTR lpszName
               s16 &sValue
    返 回 值： u16 错误码  
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    20111108    1.0			胡志云                  创建
=============================================================================*/
u16 CDBEngine::GetFieldValue(LPCSTR lpszName, s16 &sValue)
{
	BOOL32 bRet = m_pcDBResultSet->m_cDBRs.GetFieldValue(lpszName, sValue);
	if (FALSE == bRet)
	{
		return ERR_INTER_DB_EXECSQL_FAILED;
	}
	
	return SUCCESS_INTER_DB;
}

/*=============================================================================
    函 数 名： GetFieldValue
    功    能： 按字段名取字段值
    算法实现： 
    全局变量： 
    参    数： LPCSTR lpszName
               u16 &wValue
    返 回 值： u16 错误码  
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    20111108    1.0			胡志云                  创建
=============================================================================*/
u16 CDBEngine::GetFieldValue(LPCSTR lpszName, u16 &wValue)
{
	BOOL32 bRet = m_pcDBResultSet->m_cDBRs.GetFieldValue(lpszName, wValue);
	if (FALSE == bRet)
	{
		return ERR_INTER_DB_EXECSQL_FAILED;
	}
	
	return SUCCESS_INTER_DB;
}

/*=============================================================================
    函 数 名： GetFieldValue
    功    能： 按字段名取字段值
    算法实现： 
    全局变量： 
    参    数： LPCSTR lpszName
               s8 &chValue
    返 回 值： u16 错误码  
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    20111108    1.0			胡志云                  创建
=============================================================================*/
u16 CDBEngine::GetFieldValue(LPCSTR lpszName, s8 &chValue)
{
	BOOL32 bRet = m_pcDBResultSet->m_cDBRs.GetFieldValue(lpszName, chValue);
	if (FALSE == bRet)
	{
		return ERR_INTER_DB_EXECSQL_FAILED;
	}
	
	return SUCCESS_INTER_DB;
}

/*=============================================================================
    函 数 名： GetFieldValue
    功    能： 按字段名取字段值
    算法实现： 
    全局变量： 
    参    数： LPCSTR lpszName
               u8 &byValue
    返 回 值： u16 错误码  
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    20111108    1.0			胡志云                  创建
=============================================================================*/
u16 CDBEngine::GetFieldValue(LPCSTR lpszName, u8 &byValue)
{
	BOOL32 bRet = m_pcDBResultSet->m_cDBRs.GetFieldValue(lpszName, byValue);
	if (FALSE == bRet)
	{
		return ERR_INTER_DB_EXECSQL_FAILED;
	}
	
	return SUCCESS_INTER_DB;
}

u16 CDBEngine::GetFieldValue(LPCSTR lpszName, double  &fValue)
{
	BOOL32 bRet = m_pcDBResultSet->m_cDBRs.GetFieldValue(lpszName, fValue);
	if (FALSE == bRet)
	{
		return ERR_INTER_DB_EXECSQL_FAILED;
	}

	return SUCCESS_INTER_DB;
}

/*=============================================================================
    函 数 名： GetFieldValue
    功    能： 按字段名取字段值
    算法实现： 
    全局变量： 
    参    数： LPCSTR lpszName
               LPSTR pszstr
               u16 wSize
    返 回 值： u16 错误码  
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    20111108    1.0			胡志云                  创建
=============================================================================*/
u16 CDBEngine::GetFieldValue(LPCSTR lpszName, LPSTR pszstr, u16 wSize)
{
    CMS_ASSERT(wSize > 0);
    pszstr[0] = '\0';
	BOOL32 bRet = m_pcDBResultSet->m_cDBRs.GetFieldValue(lpszName, pszstr, wSize);
	if (FALSE == bRet)
	{
		return ERR_INTER_DB_EXECSQL_FAILED;
	}
	
	return SUCCESS_INTER_DB;
}

/*=============================================================================
    函 数 名： GetFieldValue
    功    能： 按字段名取字段值(Image类型字段)
    算法实现： 
    全局变量： 
    参    数： LPCSTR lpszName
               void *pImageBuf
               u16 wImageBufLen
               u16 &wImageOutLen
    返 回 值： u16 错误码  
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    20111108    1.0			胡志云                  创建
=============================================================================*/
u16 CDBEngine::GetFieldValue(LPCSTR lpszName, void *pImageBuf, u16 wImageBufLen, u16 &wImageOutLen)
{
	BOOL32 bRet = m_pcDBResultSet->m_cDBRs.GetFieldValue(lpszName, pImageBuf, wImageBufLen, wImageOutLen);
	if (FALSE == bRet)
	{
		return ERR_INTER_DB_EXECSQL_FAILED;
	}
	
	return SUCCESS_INTER_DB;
}

/*=============================================================================
    函 数 名： GetFieldValue
    功    能： 按字段索引取字段值
    算法实现： 
    全局变量： 
    参    数： u8 byIndex
               s32 &nValue
    返 回 值： u16 错误码  
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    20111108    1.0			胡志云                  创建
=============================================================================*/
u16 CDBEngine::GetFieldValue(u8 byIndex, s32 &nValue)
{
	BOOL32 bRet = m_pcDBResultSet->m_cDBRs.GetFieldValue(byIndex, nValue);
	if (FALSE == bRet)
	{
		return ERR_INTER_DB_EXECSQL_FAILED;
	}
	
	return SUCCESS_INTER_DB;
}

/*=============================================================================
    函 数 名： GetFieldValue
    功    能： 按字段索引取字段值
    算法实现： 
    全局变量： 
    参    数： u8 byIndex
               u32 &dwValue
    返 回 值： u16 错误码  
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    20111108    1.0			胡志云                  创建
=============================================================================*/
u16 CDBEngine::GetFieldValue(u8 byIndex, u32 &dwValue)
{
	BOOL32 bRet = m_pcDBResultSet->m_cDBRs.GetFieldValue(byIndex, dwValue);
	if (FALSE == bRet)
	{
		return ERR_INTER_DB_EXECSQL_FAILED;
	}
	
	return SUCCESS_INTER_DB;
}

/*=============================================================================
    函 数 名： GetFieldValue
    功    能： 按字段索引取字段值
    算法实现： 
    全局变量： 
    参    数： u8 byIndex
               s16 &sValue
    返 回 值： u16 错误码  
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    20111108    1.0			胡志云                  创建
=============================================================================*/
u16 CDBEngine::GetFieldValue(u8 byIndex, s16 &sValue)
{
	BOOL32 bRet = m_pcDBResultSet->m_cDBRs.GetFieldValue(byIndex, sValue);
	if (FALSE == bRet)
	{
		return ERR_INTER_DB_EXECSQL_FAILED;
	}
	
	return SUCCESS_INTER_DB;
}

/*=============================================================================
    函 数 名： GetFieldValue
    功    能： 按字段索引取字段值
    算法实现： 
    全局变量： 
    参    数： u8 byIndex
               u16 &wValue
    返 回 值： u16 错误码  
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    20111108    1.0			胡志云                  创建
=============================================================================*/
u16 CDBEngine::GetFieldValue(u8 byIndex, u16 &wValue)
{
	BOOL32 bRet = m_pcDBResultSet->m_cDBRs.GetFieldValue(byIndex, wValue);
	if (FALSE == bRet)
	{
		return ERR_INTER_DB_EXECSQL_FAILED;
	}
	
	return SUCCESS_INTER_DB;
}

/*=============================================================================
    函 数 名： GetFieldValue
    功    能： 按字段索引取字段值
    算法实现： 
    全局变量： 
    参    数： u8 byIndex
               s8 &chValue
    返 回 值： u16 错误码  
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    20111108    1.0			胡志云                  创建
=============================================================================*/
u16 CDBEngine::GetFieldValue(u8 byIndex, s8 &chValue)
{
	BOOL32 bRet = m_pcDBResultSet->m_cDBRs.GetFieldValue(byIndex, chValue);
	if (FALSE == bRet)
	{
		return ERR_INTER_DB_EXECSQL_FAILED;
	}
	
	return SUCCESS_INTER_DB;
}

/*=============================================================================
    函 数 名： GetFieldValue
    功    能： 按字段索引取字段值
    算法实现： 
    全局变量： 
    参    数： u8 byIndex
               u8 &byValue
    返 回 值： u16 错误码  
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    20111108    1.0			胡志云                  创建
=============================================================================*/
u16 CDBEngine::GetFieldValue(u8 byIndex, u8 &byValue)
{
	BOOL32 bRet = m_pcDBResultSet->m_cDBRs.GetFieldValue(byIndex, byValue);
	if (FALSE == bRet)
	{
		return ERR_INTER_DB_EXECSQL_FAILED;
	}
	
	return SUCCESS_INTER_DB;
}

u16 CDBEngine::GetFieldValue(u8 byIndex, double  &fValue)
{
	BOOL32 bRet = m_pcDBResultSet->m_cDBRs.GetFieldValue(byIndex, fValue);
	if (FALSE == bRet)
	{
		return ERR_INTER_DB_EXECSQL_FAILED;
	}

	return SUCCESS_INTER_DB;
}

/*=============================================================================
    函 数 名： GetFieldValue
    功    能： 按字段索引取字段值
    算法实现： 
    全局变量： 
    参    数： u8 byIndex
               LPSTR pszstr
               u16 wSize
    返 回 值： u16 错误码  
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    20111108    1.0			胡志云                  创建
=============================================================================*/
u16 CDBEngine::GetFieldValue(u8 byIndex, LPSTR pszstr, u16 wSize)
{
	BOOL32 bRet = m_pcDBResultSet->m_cDBRs.GetFieldValue(byIndex, pszstr, wSize);
	if (FALSE == bRet)
	{
		return ERR_INTER_DB_EXECSQL_FAILED;
	}
	
	return SUCCESS_INTER_DB;
}

/*=============================================================================
    函 数 名： GetFieldValue
    功    能： 按字段名取字段值(Image类型字段)
    算法实现： 
    全局变量： 
    参    数： u8 byIndex
               void *ImagepBuf
               u16 wImageBufLen
               u16 &ImagewOutLen
    返 回 值： u16 错误码  
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    20111108    1.0			胡志云                  创建
=============================================================================*/
u16 CDBEngine::GetFieldValue(u8 byIndex, void *pImageBuf, u16 wImageBufLen, u16 &wImageOutLen)
{
	BOOL32 bRet = m_pcDBResultSet->m_cDBRs.GetFieldValue(byIndex, pImageBuf, wImageBufLen, wImageOutLen);
	if (FALSE == bRet)
	{
		return ERR_INTER_DB_EXECSQL_FAILED;
	}
	
	return SUCCESS_INTER_DB;
}

/*=============================================================================
    函 数 名： MoveNext
    功    能： 
    算法实现： 
    全局变量： 
    参    数： 
    返 回 值：  
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    20111108    1.0			胡志云                  创建
=============================================================================*/
bool CDBEngine::MoveNext(void)
{
	if (FALSE == IsConnectDB())
	{
		return FALSE;
	}
	return m_pcDBResultSet->MoveNext();
}

/*=============================================================================
    函 数 名： MoveNNext
    功    能： 
    算法实现： 
    全局变量： 
    参    数： u32 dwNStep
	           bool bGoOnProcRes = FALSE
    返 回 值： bool      TRUE: 到尾部 FALSE:没到尾部 
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    20111108    1.0			胡志云                  创建
=============================================================================*/
bool CDBEngine::MoveNNext(u32 dwNStep, bool bGoOnProcRes /*= FALSE*/)
{
	if (FALSE == IsConnectDB())
	{
		return FALSE;
	}
	if (0 == dwNStep)
	{
		return TRUE;
	}

	bool bRet = FALSE;
	u32 dwRealStep  = 0;
	u32 dwNextNStep = 0;
	bRet = m_pcDBResultSet->MoveNNext(dwNStep, dwRealStep);
	//整个数据集可能包含两块，前一块数据集跳转完毕，则尝试跳置下一个数据集
	if (FALSE == bRet && TRUE == bGoOnProcRes)
	{
		if (TRUE == IsEof() && SUCCESS_INTER_DB != GoOnProcRes())
		{
			return bRet;
		}
		if (dwNStep >= dwRealStep)
		{
			dwNextNStep = dwNStep-dwRealStep;
			bRet = m_pcDBResultSet->MoveNNext(dwNextNStep, dwRealStep);
		}
	}

	return bRet;
}

/*=============================================================================
    函 数 名： GoOnProcRes
    功    能： 继续处理结果集
    算法实现： 
    全局变量： 
    参    数： 
    返 回 值： u16 
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2006/3/23   1.0			胡志云                  创建
=============================================================================*/
u16 CDBEngine::GoOnProcRes()
{
	u16 wRet = SUCCESS_INTER_DB;
	EOpCode eRet = m_pcDBOprer->GoOnProcRes();
	if (EOpCode_FindRs != eRet)
	{
		return ERR_INTER_DB_NO_RESULTSET; // 没找到结果集
	}
	if (FALSE == m_pcDBResultSet->MoveNext())//挪到第一条;
	{
		return ERR_INTER_DB_NO_RESULTSET; // 没找到结果集
	}

	return wRet;
}

/*=============================================================================
    函 数 名： IsEof
    功    能： 是否到记录尾或者是否存在记录
    算法实现： 
    全局变量： 
    参    数： void
    返 回 值： bool 
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    20111108    1.0			胡志云                  创建
=============================================================================*/
bool CDBEngine::IsEof(void)
{
	return m_pcDBResultSet->IsEof();
}

/*=============================================================================
    函 数 名： ClearResults
    功    能： 清除所有记录
    算法实现： 
    全局变量： 
    参    数： void
    返 回 值： void 
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    20111108    1.0			胡志云                  创建
=============================================================================*/
void CDBEngine::ClearResults(void)
{
	if (FALSE == IsConnectDB())
	{
		return;
	}
	m_pcDBResultSet->CancelCurRs();
}

void CDBEngine::DBClientCBProc( u16 wErrNum, LPCSTR lpSrvMsg, void *pContext )
{
    if (NULL != pContext && 0 == wErrNum)
    {
        //断链检测
        CDBEngine* pcDbEngine = (CDBEngine*)pContext;
        pcDbEngine->m_bConnectDB = false;
        OspPrintf(TRUE, FALSE, "DBDisconnect detected\n");
    }
}

void CDBEngine::DBServerCBProc( u16 wErrNum, LPCSTR lpSrvMsg, void *pContext )
{
    if (NULL == lpSrvMsg)
    {
        return;
    }

    OspPrintf(TRUE, FALSE, "\n\nDBServerCBProc Start*******************************************\n");
    OspPrintf(TRUE, FALSE, "ErrNum:%d\n", wErrNum);
    OspPrintf(TRUE, FALSE, "SrvMsg:%s\n", lpSrvMsg);
    OspPrintf(TRUE, FALSE, "DBServerCBProc End*******************************************\n\n");
}

u32 CDBEngine::GetAffectedRowCount()
{
    return m_pcDBOprer->GetAffectedRowCount();
}


//构造
CResultSet::CResultSet()
{
}

//析构
CResultSet::~CResultSet()
{
}

/*=============================================================================
    函 数 名： MoveNext
    功    能： 
    算法实现： 
    全局变量： 
    参    数： 
    返 回 值： bool      true: 到尾部 false:没到尾部
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
=============================================================================*/
bool CResultSet::MoveNext()
{	
	return (TRUE == m_cDBRs.MoveNext());
}

/*=============================================================================
    函 数 名： MoveNNext
    功    能： 
    算法实现： 
    全局变量： 
    参    数： u32 dwNStep     移动的步数
	           u32 &dwRealStep 实际移动的步数
    返 回 值： bool      TRUE: 到尾部 FALSE:没到尾部
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    20111108    1.0			胡志云                  创建
=============================================================================*/
bool CResultSet::MoveNNext(u32 dwNStep, u32 &dwRealStep)
{
	dwRealStep = 0;
	if (0 != dwNStep)
	{
		while(0 != dwNStep && (FALSE == m_cDBRs.IsEof()))
		{
			m_cDBRs.MoveNext();
			dwNStep--;
			dwRealStep++;
		}
	}
	
	return (FALSE == m_cDBRs.IsEof());
}

/*=============================================================================
    函 数 名： GetFieldCount
    功    能： 获取列数
    算法实现： 
    全局变量： 
    参    数： 
    返 回 值： u16 列数
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    20111108    1.0			胡志云                  创建
=============================================================================*/
u16 CResultSet::GetFieldCount()
{
	return m_cDBRs.GetFieldCount();
}

/*=============================================================================
    函 数 名： GetCount
    功    能： 获取行数
    算法实现： 
    全局变量： 
    参    数： 
    返 回 值： u32 行数
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2006/3/23   1.0			胡志云                  创建
=============================================================================*/
u32 CResultSet::GetCount()
{
	u32  dwCount = 0;
	m_cDBRs.GetFieldValue(1, dwCount);
	return dwCount;
}

/*=============================================================================
    函 数 名： IsEof
    功    能： 是否到记录尾或者是否存在记录
    算法实现： 
    全局变量： 
    参    数： void
    返 回 值： bool 
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2006/3/23   1.0			胡志云                  创建
=============================================================================*/
bool CResultSet::IsEof(void)
{
	return (TRUE == m_cDBRs.IsEof());
}

/*=============================================================================
    函 数 名： CancelCurRs
    功    能： 清除所有记录
    算法实现： 
    全局变量： 
    参    数： void
    返 回 值： u16 错误码  
-----------------------------------------------------------------------------
    修改记录：
    日  期		版本		修改人		走读人    修改内容
    2006/3/23   1.0			胡志云                  创建
=============================================================================*/
u16 CResultSet::CancelCurRs(void)
{
	//m_cDBRs.CancelCurRs();
	return SUCCESS_INTER_DB;
}
