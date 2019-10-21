#ifndef _RS_DB_MANAGER_H_
#define _RS_DB_MANAGER_H_

#include "rstypes.h"
#include "dbinterface.h"

#define RS_MAXLEN_SQLSTR    3072

class CDbManager
{
public:
    CDbManager()
	{
		m_pDBHandler = NULL;
		m_pRecordSet = NULL;
		m_szNumToStr = NULL;
		m_szSql = NULL;
		m_byAppIndex = 0;
	}
	
    ~CDbManager()
	{
		m_pDBHandler = NULL;
		m_pRecordSet = NULL;
		m_szNumToStr = NULL;
		m_szSql = NULL;
		m_byAppIndex = 0;
	}
        
    BOOL32 IsEof()
	{
		return m_pRecordSet->IsEof();
	}

    BOOL32 MoveNext()
	{
		return m_pRecordSet->MoveNext();
	}
	
    void   SetDBHandler(CDBHandler *pDBHandler)
    {
        m_pDBHandler = pDBHandler;
    }

    void SetDBRecordSet(CDBRs *pDBRs)
    {
        m_pRecordSet = pDBRs;
    }

    void SetDbManagerParam( u8 byDBType, CDBHandler* pDbHandler, CDBRs *pDBRs, s8* sz1, s8* sz2, u8 byIndex)
    {
		m_byDBType = byDBType;
        m_pDBHandler = pDbHandler;
        m_pRecordSet = pDBRs;
        m_szNumToStr = sz1;
        m_szSql = sz2;
        m_byAppIndex = byIndex;
    }

    BOOL32 ExecProcedure(const s8* pszProcedureName,const s8* pszParams, BOOL32 bNeedResult=TRUE)
	{
		char szSQL[RS_MAXLEN_SQLSTR] = {0};    
		
		if (m_byDBType == DBTYPE_SYBASE)
		{
			RsSnprintf(szSQL, sizeof(szSQL), "exec %s %s", pszProcedureName, pszParams);
		}
		else if (m_byDBType == DBTYPE_MYSQL)
		{
			RsSnprintf(szSQL, sizeof(szSQL), "call %s(%s)", pszProcedureName, pszParams);
		}
		else
		{
			return FALSE;
		}   
		
		if (bNeedResult)
		{
			return m_pDBHandler->ExecSql(szSQL,m_pRecordSet);
		}
		else
		{
			return m_pDBHandler->ExecSql(szSQL);
		}
	}

	const s8* GetSQLSpliter() const
	{
		if (m_byDBType == DBTYPE_SYBASE)
		{
			return " \n ";
		}
		else if (m_byDBType == DBTYPE_MYSQL)
		{
			return " ; ";
		}
		else
		{
			return " ; ";
		}
	}
  
protected:
    CDBHandler *m_pDBHandler;
    CDBRs      *m_pRecordSet;
    s8         *m_szNumToStr;
    s8         *m_szSql;
    u8         m_byAppIndex;
	u8         m_byDBType;
};

#endif