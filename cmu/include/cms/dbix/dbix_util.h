/*========================================================================================
ģ����    ��dbix.lib
�ļ���    ��dbix_util.h
����ļ�  ��
ʵ�ֹ���  �����ݿ��������еĹ�����, �������Ӳ������SQL��������
����      ��huzhiyun
��Ȩ      ��<Copyright(C) 2003-2008 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
�޸ļ�¼��
����               �汾              �޸���             �߶���              �޸ļ�¼
2012/04/27         V1.0              huzhiyun                              �½��ļ�
=========================================================================================*/
#ifndef _DBIX_UTIL_H_
#define _DBIX_UTIL_H_

#include "dbix_define.h"

DBIX_NS_BEGIN

struct TSqlString;
// ���Ӷ�����
class CDBILayer;
class CDBConnection
{
public:
    enum ENExecOpt
    {
        enExecNoRet,   // ���践��������
        enExecMustRet, // ���뷵�������У�������������Ҳ��Ϊ�Ǵ���
        enExecMayRet   // ���������У������Ǳ���ģ����ܲ�����������
    };
    enum ENMultiRsProcCode
    {
        enMultiRsProcFoundNewResultset, // �����µĽ����, ���Ի�ȡ�½������������
        enMultiRsProcNoNewResultset,    // û���µĽ����
        enMultiRsProcFailed,            // 
    };

    CDBConnection(const TDBConnectCfg &tCfg);
    ~CDBConnection();
    bool CheckConnection(bool bTryExecTestSql = false); // δ����ʱ��������, ����true��ʾ���ӽ���
    void Disconnect();
    const TDBConnectCfg& ConnectCfg() const;
private:
    CDBConnection(const CDBConnection&);
    CDBConnection& operator=(const CDBConnection&);

public:
    bool ExecSQL(const string &strSQL, ENExecOpt enExecOpt);
    bool ExecSQL(const TSqlString &strSQL, ENExecOpt enExecOpt);
    bool ExecSQL(const s8 *strSQL, ENExecOpt enExecOpt);
    u32 GetAffectedRowCount();
    bool IsNotEof();

    /* ���ڴ����ݿ�ȡ���� */
    const s8* GetString(const s8* field);  // ȡ�꼴��ʹ��
    CCMSTime GetCmsDate(const s8* field);
    s32 GetInt(const s8* field);
    bool GetBool(const s8* field);
    double GetDouble(const s8* field);

    bool MoveNext();   // ����false��ʾ�ƶ�ʱ����ע�⣬��ǰ�������һ��ʱ���ƶ�Ҳ�������
    ENMultiRsProcCode NextResultset(); // ��ȡ�¸����������

private:
    TDBConnectCfg m_tDBConnectCfg; // ��������
    CDBILayer *m_pcDBILayer;       // dbi�����
};

/* ���Ӷ�̬����, ע�⡪������ֻ������dbix����߳������Ļ�����ֱ��ʹ�� */
CDBConnection* AllocateDBConnection(const TDBConnectCfg* ptCfg = NULL); // ����ΪNULLʱʹ�ó�ʼ��dbixʱ�����ݿ���������
void DeallocateDBConnection(const CDBConnection* pcDBConn);




// SQL�ַ���������
struct TSqlString
{
    enum
    {
        MAXLEN_SQL_STRING = 65532
    };
public:
    TSqlString()
    {
        Clear();
    }
    operator const s8*()
    {
        return m_achSqlString;
    }
    bool Empty() const
    {
        return (0 == m_dwSqlLen);
    }
    void Clear(void)
    {
        m_achSqlString[0] = '\0';
        m_dwSqlLen = 0;
    }

    void Format(const s8* pszFmt, ...)
#ifdef __GNUC__
        __attribute__((format(printf, 2, 3)))
#endif
    {
        va_list tArgptr;
        va_start(tArgptr, pszFmt);
        m_dwSqlLen = vsnprintf(m_achSqlString, MAXLEN_SQL_STRING-1, pszFmt, tArgptr);
        va_end(tArgptr);
    }
    void AppendString(const s8* pszFmt, ...)
#ifdef __GNUC__
        __attribute__((format(printf, 2, 3)))
#endif
    {
        if (m_dwSqlLen > MAXLEN_SQL_STRING)
        {
            return;
        }

        va_list tArgptr;
        va_start(tArgptr, pszFmt);
        m_dwSqlLen += vsnprintf(m_achSqlString+m_dwSqlLen, MAXLEN_SQL_STRING-1-m_dwSqlLen, pszFmt, tArgptr);
        va_end(tArgptr);
    }
    void EraseLastRedundantComma()
    {
        if (m_dwSqlLen >= 1 && ',' == m_achSqlString[m_dwSqlLen-1])
        {
            m_achSqlString[--m_dwSqlLen] = '\0';
        }
    }
    void EraseLastRedundantAnd()
    {
        if (m_dwSqlLen >= 5 && 0 == strcmp(&m_achSqlString[m_dwSqlLen-5], " and "))
        {
            m_dwSqlLen -= 5;
            m_achSqlString[m_dwSqlLen] = '\0';
        }
    }
    const s8* GetBuf(void) const
    {
        return m_achSqlString;
    }
    u32 GetBufLen(void) const
    {
        return m_dwSqlLen;
    }

protected:
    s8  m_achSqlString[MAXLEN_SQL_STRING];
    u32 m_dwSqlLen; // ���Ȳ�����������
};

// ת��C++���������ݿ����ݣ�����,�ַ����������Զ���������ӵ����Ų����Զ�ת����Ҫת����ַ�
std::string DBData(bool val);
std::string DBData(s32 val);
std::string DBData(double val);
std::string DBData(const std::string &val);  // ע�⣬����like�ﾳʱ��ʹ��DBLikeString
std::string DBData(const s8* val);           // ע�⣬����like�ﾳʱ��ʹ��DBLikeString
std::string DBData(const CCMSTime &val);

std::string DBData(const Variant &val, ENDBDataType enDBDataType); // ת��Variant��ʽ��C++����ת�������ݿ�����

/* SQL92��׼���� */

const s8 SQLEscapeChar = '\\'; // ת���
const s8 SQLAnyBytes = '%';    // likeͨ���, ƥ����(����0��)�����ַ�
const s8 SQLSingleByte = '_';  // likeͨ���, ƥ��һ�������ַ�

// like�ﾳ�¶��������ַ���֧�֣���ͬ���ݿ���ܲ�ͬ���˺����Գ���%,_,\����������ַ��Զ�ת��
std::string DBLikeString(const std::string &val, ENDBType enDBType);
std::string DBLikeString(const std::string &val); // �������ĺ�������ʹ�ó�ʼ����ʱ���ӵ����ݿ�������ΪenDBType

// ���ɴ洢���̵������
void GenerateExecProcSql(string &strSql, const string &strProcName, const vector<TVariantWrapper> &tParms, ENDBType enDBType);

// ����ʵ��ṹ�Զ���ȡ�����, ���������pResultDataָ��ĵ�ַ
class CDBEntitySchema;
class CDBEntityValue;
ENDBResult AutoFetchResultset(CDBConnection *pcDBConnection, const CDBEntitySchema &cSchema, CDBEntityValue* &pResultData);

DBIX_NS_END
#endif // _DBIX_UTIL_H_
