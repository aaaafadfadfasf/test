/* ��װ���ݿ���������ࣨ���ֱ��ʹ��DBI���ŵ㣺�������ú�������Է�װ��־�ȹ��ܣ�*/
#ifndef _DBENGINE_H_
#define _DBENGINE_H_

///////////////////////////////////////////////////////////

#include "dbi/dbinterface.h"
#include <string>
using std::string;

//���ݿ������һЩ������

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

// ��������
#define TAS_DBOPR_TIMEOUT_DEFAULTSPAN         (u8)20//20��

//�ص������Ķ���
#define TASDB_SERVERCBPROC DBCbFunc
#define TASDB_CLIENTCBPROC DBCbFunc

/* ��ʼ���뷴��ʼ��DB���� */
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
        string  m_strDBIP;     // ���ݿ�����IP�ִ�
        u16     m_wDBPort;     // ���ݿ�����PORT(������)
        string  m_strDBUserName; // ���ݿ������û���
        string  m_strDBUserPwd;  // ���ݿ���������
        string  m_strDBName;     // ���ݿ�����
    };
	CDBEngine();
	virtual ~CDBEngine();

public:
	u16    ConnectDB(const TDBConnectInfo *ptDBConnectInfo);
	u16    DisconnectDB();// �Ͽ�����
	bool   IsConnectDB(); // �Ƿ����������ݿ�

	u16    ExecSql(LPCSTR lpszSql, bool bNeedRet = true);

	bool   MoveNext();
	bool   MoveNNext(u32 dwNStep, bool bGoOnProcRes = false);// �ƶ�ָ������
	u16    GetFieldCount();       // ȡ����
	u32    GetCount(void);        // ȡ����-��¼����
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

	u16    GoOnProcRes();   // ����������һ�������(���ڴ洢������)
	bool   IsEof();         // �Ƿ񵽼�¼β�����Ƿ���ڼ�¼
	void   ClearResults();  // ��ս����
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
