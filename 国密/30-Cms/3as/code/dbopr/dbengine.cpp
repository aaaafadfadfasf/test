#include "dbengine.h"
#include "osp.h"
#include "inner_common.h"

static ENDBType s_enDbType = enDBTypeMysql;

/*=============================================================================
    �� �� ���� DBGlobalCBProc
    ��    �ܣ� DBȫ�ֺ����ص�
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� u16 wErrNum
               LPCSTR lpSrvMsg
               void *pContext   ��ʹ����ָ���������������ĸ����ӳ���
    �� �� ֵ�� void 
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    20111108    1.0			��־��                  ����
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
    �� �� ���� InitalTASDBEngine
    ��    �ܣ� DB���ݿ��ʼ��������ȫ�ֻص��Լ�ȱʡ�����ӵ�ȫ�ֲ�����ʱʱ��
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� u8 byDBEngineType ���ݿ���������
    �� �� ֵ�� bool 
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
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
        if (!SetDBLocale("", DB_CHARSET_UTF8)) // hzytodo2 ��һ������ֻ��mysql���������õ�
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
    �� �� ���� ExitDB
    ��    �ܣ� �˳�DB���ݿ�
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� void
    �� �� ֵ�� bool 
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
=============================================================================*/
bool ExitDBEngine(void)
{
	return (TRUE == DBExit());
}

ENDBType GetEngineDBType()
{
    return s_enDbType;
}

//����
CDBEngine::CDBEngine()
{
	m_pcDBOprer      = NULL;
	m_pcDBResultSet  = NULL;
	m_bConnectDB     = false;
	m_hDataFile      = NULL;
}

//����
CDBEngine::~CDBEngine()
{
	DisconnectDB();
	SAFE_DELETE(m_pcDBOprer)
	SAFE_DELETE(m_pcDBResultSet)
}

/*=============================================================================
    �� �� ���� ConnectDB
    ��    �ܣ� ������Ӧ����/������󣬲��������ݿ�
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� const TDBConnectInfo *ptDBConnectInfo
    �� �� ֵ�� u16 ������
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    20111108    1.0			��־��                  ����
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

	//����Ϊ������״̬
	m_bConnectDB = true;
	return SUCCESS_INTER_DB;
}

/*=============================================================================
    �� �� ���� DisconnectDB
    ��    �ܣ� �Ͽ����ݿ⣬�������Ӧ����/�������
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� void
    �� �� ֵ�� u16 ������  
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    20111108    1.0			��־��                  ����
=============================================================================*/
u16 CDBEngine::DisconnectDB(void)
{
	u16 wRet = SUCCESS_INTER_DB;

	if (IsConnectDB())
	{
		BOOL32 bRet = m_pcDBOprer->CloseConnection();
		wRet = (TRUE == bRet) ? SUCCESS_INTER_DB : ERR_INTER_DB_DISCONNECT_FAILED;
	}

	//����Ϊ�Ͽ�״̬
	m_bConnectDB     = false;

	return wRet;
}

/*=============================================================================
    �� �� ���� IsConnectDB
    ��    �ܣ� 
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� 
    �� �� ֵ�� bool 
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    20111108    1.0			��־��                  ����
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
    �� �� ���� ExecSql
    ��    �ܣ� ִ��Sql���
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� LPCSTR lpszSql
               bool bNeedRet = FALSE �Ƿ񷵻����ݼ�
    �� �� ֵ�� u16 ������  
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    20111108    1.0			��־��                  ����
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
    �� �� ���� GetFieldCount
    ��    �ܣ� ��ȡ����-��������ֶ�����
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� 
    �� �� ֵ�� u16 ��������ֶ�����
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    20111108    1.0			��־��                  ����
=============================================================================*/
u16 CDBEngine::GetFieldCount()
{
	return m_pcDBResultSet->GetFieldCount();
}

/*=============================================================================
    �� �� ���� GetCount
    ��    �ܣ� ��ȡ����-������м�¼����
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� 
    �� �� ֵ�� u32 ������м�¼����
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2006/3/23   1.0			��־��                  ����
=============================================================================*/
u32 CDBEngine::GetCount(void)
{
	u32 dwCount = m_pcDBResultSet->GetCount();
	return dwCount;
}

/*=============================================================================
    �� �� ���� GetFieldValue
    ��    �ܣ� ���ֶ���ȡ�ֶ�ֵ
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� LPCSTR lpszName
               s32 &nValue
    �� �� ֵ�� u16 ������  
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    20111108    1.0			��־��                  ����
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
    �� �� ���� GetFieldValue
    ��    �ܣ� ���ֶ���ȡ�ֶ�ֵ
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� LPCSTR lpszName
               u32 &dwValue
    �� �� ֵ�� u16 ������  
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    20111108    1.0			��־��                  ����
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
    �� �� ���� GetFieldValue
    ��    �ܣ� ���ֶ���ȡ�ֶ�ֵ
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� LPCSTR lpszName
               s16 &sValue
    �� �� ֵ�� u16 ������  
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    20111108    1.0			��־��                  ����
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
    �� �� ���� GetFieldValue
    ��    �ܣ� ���ֶ���ȡ�ֶ�ֵ
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� LPCSTR lpszName
               u16 &wValue
    �� �� ֵ�� u16 ������  
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    20111108    1.0			��־��                  ����
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
    �� �� ���� GetFieldValue
    ��    �ܣ� ���ֶ���ȡ�ֶ�ֵ
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� LPCSTR lpszName
               s8 &chValue
    �� �� ֵ�� u16 ������  
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    20111108    1.0			��־��                  ����
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
    �� �� ���� GetFieldValue
    ��    �ܣ� ���ֶ���ȡ�ֶ�ֵ
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� LPCSTR lpszName
               u8 &byValue
    �� �� ֵ�� u16 ������  
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    20111108    1.0			��־��                  ����
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
    �� �� ���� GetFieldValue
    ��    �ܣ� ���ֶ���ȡ�ֶ�ֵ
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� LPCSTR lpszName
               LPSTR pszstr
               u16 wSize
    �� �� ֵ�� u16 ������  
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    20111108    1.0			��־��                  ����
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
    �� �� ���� GetFieldValue
    ��    �ܣ� ���ֶ���ȡ�ֶ�ֵ(Image�����ֶ�)
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� LPCSTR lpszName
               void *pImageBuf
               u16 wImageBufLen
               u16 &wImageOutLen
    �� �� ֵ�� u16 ������  
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    20111108    1.0			��־��                  ����
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
    �� �� ���� GetFieldValue
    ��    �ܣ� ���ֶ�����ȡ�ֶ�ֵ
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� u8 byIndex
               s32 &nValue
    �� �� ֵ�� u16 ������  
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    20111108    1.0			��־��                  ����
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
    �� �� ���� GetFieldValue
    ��    �ܣ� ���ֶ�����ȡ�ֶ�ֵ
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� u8 byIndex
               u32 &dwValue
    �� �� ֵ�� u16 ������  
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    20111108    1.0			��־��                  ����
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
    �� �� ���� GetFieldValue
    ��    �ܣ� ���ֶ�����ȡ�ֶ�ֵ
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� u8 byIndex
               s16 &sValue
    �� �� ֵ�� u16 ������  
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    20111108    1.0			��־��                  ����
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
    �� �� ���� GetFieldValue
    ��    �ܣ� ���ֶ�����ȡ�ֶ�ֵ
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� u8 byIndex
               u16 &wValue
    �� �� ֵ�� u16 ������  
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    20111108    1.0			��־��                  ����
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
    �� �� ���� GetFieldValue
    ��    �ܣ� ���ֶ�����ȡ�ֶ�ֵ
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� u8 byIndex
               s8 &chValue
    �� �� ֵ�� u16 ������  
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    20111108    1.0			��־��                  ����
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
    �� �� ���� GetFieldValue
    ��    �ܣ� ���ֶ�����ȡ�ֶ�ֵ
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� u8 byIndex
               u8 &byValue
    �� �� ֵ�� u16 ������  
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    20111108    1.0			��־��                  ����
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
    �� �� ���� GetFieldValue
    ��    �ܣ� ���ֶ�����ȡ�ֶ�ֵ
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� u8 byIndex
               LPSTR pszstr
               u16 wSize
    �� �� ֵ�� u16 ������  
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    20111108    1.0			��־��                  ����
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
    �� �� ���� GetFieldValue
    ��    �ܣ� ���ֶ���ȡ�ֶ�ֵ(Image�����ֶ�)
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� u8 byIndex
               void *ImagepBuf
               u16 wImageBufLen
               u16 &ImagewOutLen
    �� �� ֵ�� u16 ������  
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    20111108    1.0			��־��                  ����
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
    �� �� ���� MoveNext
    ��    �ܣ� 
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� 
    �� �� ֵ��  
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    20111108    1.0			��־��                  ����
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
    �� �� ���� MoveNNext
    ��    �ܣ� 
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� u32 dwNStep
	           bool bGoOnProcRes = FALSE
    �� �� ֵ�� bool      TRUE: ��β�� FALSE:û��β�� 
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    20111108    1.0			��־��                  ����
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
	//�������ݼ����ܰ������飬ǰһ�����ݼ���ת��ϣ�����������һ�����ݼ�
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
    �� �� ���� GoOnProcRes
    ��    �ܣ� ������������
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� 
    �� �� ֵ�� u16 
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2006/3/23   1.0			��־��                  ����
=============================================================================*/
u16 CDBEngine::GoOnProcRes()
{
	u16 wRet = SUCCESS_INTER_DB;
	EOpCode eRet = m_pcDBOprer->GoOnProcRes();
	if (EOpCode_FindRs != eRet)
	{
		return ERR_INTER_DB_NO_RESULTSET; // û�ҵ������
	}
	if (FALSE == m_pcDBResultSet->MoveNext())//Ų����һ��;
	{
		return ERR_INTER_DB_NO_RESULTSET; // û�ҵ������
	}

	return wRet;
}

/*=============================================================================
    �� �� ���� IsEof
    ��    �ܣ� �Ƿ񵽼�¼β�����Ƿ���ڼ�¼
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� void
    �� �� ֵ�� bool 
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    20111108    1.0			��־��                  ����
=============================================================================*/
bool CDBEngine::IsEof(void)
{
	return m_pcDBResultSet->IsEof();
}

/*=============================================================================
    �� �� ���� ClearResults
    ��    �ܣ� ������м�¼
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� void
    �� �� ֵ�� void 
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    20111108    1.0			��־��                  ����
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
        //�������
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


//����
CResultSet::CResultSet()
{
}

//����
CResultSet::~CResultSet()
{
}

/*=============================================================================
    �� �� ���� MoveNext
    ��    �ܣ� 
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� 
    �� �� ֵ�� bool      true: ��β�� false:û��β��
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
=============================================================================*/
bool CResultSet::MoveNext()
{	
	return (TRUE == m_cDBRs.MoveNext());
}

/*=============================================================================
    �� �� ���� MoveNNext
    ��    �ܣ� 
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� u32 dwNStep     �ƶ��Ĳ���
	           u32 &dwRealStep ʵ���ƶ��Ĳ���
    �� �� ֵ�� bool      TRUE: ��β�� FALSE:û��β��
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    20111108    1.0			��־��                  ����
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
    �� �� ���� GetFieldCount
    ��    �ܣ� ��ȡ����
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� 
    �� �� ֵ�� u16 ����
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    20111108    1.0			��־��                  ����
=============================================================================*/
u16 CResultSet::GetFieldCount()
{
	return m_cDBRs.GetFieldCount();
}

/*=============================================================================
    �� �� ���� GetCount
    ��    �ܣ� ��ȡ����
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� 
    �� �� ֵ�� u32 ����
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2006/3/23   1.0			��־��                  ����
=============================================================================*/
u32 CResultSet::GetCount()
{
	u32  dwCount = 0;
	m_cDBRs.GetFieldValue(1, dwCount);
	return dwCount;
}

/*=============================================================================
    �� �� ���� IsEof
    ��    �ܣ� �Ƿ񵽼�¼β�����Ƿ���ڼ�¼
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� void
    �� �� ֵ�� bool 
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2006/3/23   1.0			��־��                  ����
=============================================================================*/
bool CResultSet::IsEof(void)
{
	return (TRUE == m_cDBRs.IsEof());
}

/*=============================================================================
    �� �� ���� CancelCurRs
    ��    �ܣ� ������м�¼
    �㷨ʵ�֣� 
    ȫ�ֱ����� 
    ��    ���� void
    �� �� ֵ�� u16 ������  
-----------------------------------------------------------------------------
    �޸ļ�¼��
    ��  ��		�汾		�޸���		�߶���    �޸�����
    2006/3/23   1.0			��־��                  ����
=============================================================================*/
u16 CResultSet::CancelCurRs(void)
{
	//m_cDBRs.CancelCurRs();
	return SUCCESS_INTER_DB;
}
