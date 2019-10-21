/* 封装数据库操作帮助类（相对直接使用DBI的优点：更加易用和里面可以封装日志等功能）*/
#ifndef _DBENGINE_H_
#define _DBENGINE_H_

///////////////////////////////////////////////////////////

#include "dbi/dbinterface.h"
#include <string>
using std::string;

//数据库操作的一些错误码

#define   SUCCESS_INTER_DB                      0 //opr success
#define   ERR_INTER_DB_BGN                      (u16)1

#define   ERR_INTER_DB_INVALID_PARAM         ERR_INTER_DB_BGN     //invalid param
#define   ERR_INTER_DB_ALLOC_MEMORY          ERR_INTER_DB_BGN + 1 //fail to alloc memory
#define   ERR_INTER_DB_SET_CBDATA            ERR_INTER_DB_BGN + 2 //fail to set ErrFuc

#define   ERR_INTER_DB_CONNECT_FAILED        ERR_INTER_DB_BGN + 5 //fail to connect server
#define   ERR_INTER_DB_DISCONNECT_FAILED     ERR_INTER_DB_BGN + 6 //fail to disconnect server
#define   ERR_INTER_DB_EXECSQL_FAILED        ERR_INTER_DB_BGN + 7 //fail to execute sql
#define   ERR_INTER_DB_NOT_CONNECT           ERR_INTER_DB_BGN + 8 //database not connect

#define   ERR_INTER_DB_NO_RESULTSET          ERR_INTER_DB_BGN + 20//fail to find recordset
#define   ERR_INTER_DB_NO_ENOUGH_RESULTSET   ERR_INTER_DB_BGN + 21//fail to find enough recordset
#define   ERR_INTER_DB_NO_ASSIGN_NO_FAILED   ERR_INTER_DB_BGN + 22//fail to assign new NO

// 常量定义
#define TAS_DBOPR_TIMEOUT_DEFAULTSPAN         (u8)20//20秒

//回调函数的定义
#define TASDB_SERVERCBPROC DBCbFunc
#define TASDB_CLIENTCBPROC DBCbFunc

/* 初始化与反初始化DB引擎 */
enum ENDBType
{
    enDBTypeSybase = DBTYPE_SYBASE,
    enDBTypeMysql  = DBTYPE_MYSQL,
    enDBTypeOracle = DBTYPE_ORACLE
};
bool InitalDBEngine(ENDBType enDBType);
bool ExitDBEngine(void);
ENDBType GetEngineDBType();

class CResultSet
{
public:
	CResultSet();
	virtual ~CResultSet();
	
public:
	bool MoveNext(void);
	bool MoveNNext(u32 dwNStep, u32 &dwRealStep);
	u16  GetFieldCount();
	u32  GetCount(void);
	bool IsEof(void);
	u16  CancelCurRs(void);
	
public:
	CDBRs  m_cDBRs;
};

class CDBEngine
{
public:
    struct TDBConnectInfo
    {
        string  m_strDBIP;     // 数据库连接IP字串
        u16     m_wDBPort;     // 数据库连接PORT(主机序)
        string  m_strDBUserName; // 数据库连接用户名
        string  m_strDBUserPwd;  // 数据库连接密码
        string  m_strDBName;     // 数据库名称
    };
	CDBEngine();
	virtual ~CDBEngine();

public:
	u16    ConnectDB(const TDBConnectInfo *ptDBConnectInfo);
	u16    DisconnectDB();// 断开连接
	bool   IsConnectDB(); // 是否连接上数据库

	u16    ExecSql(LPCSTR lpszSql, bool bNeedRet = true);

	bool   MoveNext();
	bool   MoveNNext(u32 dwNStep, bool bGoOnProcRes = false);// 移动指定条数
	u16    GetFieldCount();       // 取列数
	u32    GetCount(void);        // 取行数-记录总数
	u16    GetFieldValue(LPCSTR lpszName, s32 &nValue);
	u16    GetFieldValue(LPCSTR lpszName, u32 &dwValue);
	u16    GetFieldValue(LPCSTR lpszName, s16 &sValue); 
	u16    GetFieldValue(LPCSTR lpszName, u16 &wValue); 
	u16    GetFieldValue(LPCSTR lpszName, s8  &chValue);
	u16    GetFieldValue(LPCSTR lpszName, u8  &byValue);
	u16    GetFieldValue(LPCSTR lpszName, double  &fValue);
	u16    GetFieldValue(LPCSTR lpszName, LPSTR pszstr, u16 wSize);
	u16    GetFieldValue(LPCSTR lpszName, void *ImagepBuf, u16 wImageBufLen, u16 &wImageOutLen);

	u16    GetFieldValue(u8 byIndex, s32 &nValue);
	u16    GetFieldValue(u8 byIndex, u32 &dwValue);
	u16    GetFieldValue(u8 byIndex, s16 &sValue);
	u16    GetFieldValue(u8 byIndex, u16 &wValue);
	u16    GetFieldValue(u8 byIndex, s8  &chValue);
	u16    GetFieldValue(u8 byIndex, u8  &byValue);
	u16    GetFieldValue(u8 byIndex, double  &fValue);
	u16    GetFieldValue(u8 byIndex, LPSTR pszstr, u16 wSize);
	u16    GetFieldValue(u8 byIndex, void *pImageBuf, u16 wImageBufLen, u16 &wImageOutLen);

	u16    GoOnProcRes();   // 继续处理下一个结果集(用于存储过程中)
	bool   IsEof();         // 是否到记录尾或者是否存在记录
	void   ClearResults();  // 清空结果集
    u32    GetAffectedRowCount();
private:
    static void DBClientCBProc(u16 wErrNum, LPCSTR lpSrvMsg, void *pContext);
    static void DBServerCBProc(u16 wErrNum, LPCSTR lpSrvMsg, void *pContext);
private:
	CDBHandler     *m_pcDBOprer;
	CResultSet     *m_pcDBResultSet;
	bool            m_bConnectDB;
	FILE           *m_hDataFile;
};

#endif /* _DBENGINE_H_ */
