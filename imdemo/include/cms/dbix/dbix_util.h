/*========================================================================================
模块名    ：dbix.lib
文件名    ：dbix_util.h
相关文件  ：
实现功能  ：数据库操作框架中的工具类, 包括连接操作类和SQL语句操作类
作者      ：huzhiyun
版权      ：<Copyright(C) 2003-2008 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2012/04/27         V1.0              huzhiyun                              新建文件
=========================================================================================*/
#ifndef _DBIX_UTIL_H_
#define _DBIX_UTIL_H_

#include "dbix_define.h"

DBIX_NS_BEGIN

struct TSqlString;
// 连接对象类
class CDBILayer;
class CDBConnection
{
public:
    enum ENExecOpt
    {
        enExecNoRet,   // 无需返回数据行
        enExecMustRet, // 必须返回数据行，不存在数据行也认为是错误
        enExecMayRet   // 返回数据行，但不是必须的，可能不存在数据行
    };
    enum ENMultiRsProcCode
    {
        enMultiRsProcFoundNewResultset, // 发现新的结果集, 可以获取新结果集的行数据
        enMultiRsProcNoNewResultset,    // 没有新的结果集
        enMultiRsProcFailed,            // 
    };

    CDBConnection(const TDBConnectCfg &tCfg);
    ~CDBConnection();
    bool CheckConnection(bool bTryExecTestSql = false); // 未连接时进行连接, 返回true表示连接建立
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

    /* 用于从数据库取数据 */
    const s8* GetString(const s8* field);  // 取完即刻使用
    CCMSTime GetCmsDate(const s8* field);
    s32 GetInt(const s8* field);
    bool GetBool(const s8* field);
    double GetDouble(const s8* field);

    bool MoveNext();   // 返回false表示移动时出错，注意，当前处于最后一行时再移动也是允许的
    ENMultiRsProcCode NextResultset(); // 获取下个结果集数据

private:
    TDBConnectCfg m_tDBConnectCfg; // 连接配置
    CDBILayer *m_pcDBILayer;       // dbi层对象
};

/* 连接动态管理, 注意——连接只允许在dbix框架线程上下文环境中直接使用 */
CDBConnection* AllocateDBConnection(const TDBConnectCfg* ptCfg = NULL); // 参数为NULL时使用初始化dbix时的数据库连接配置
void DeallocateDBConnection(const CDBConnection* pcDBConn);




// SQL字符串工具类
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
    u32 m_dwSqlLen; // 长度不包括结束符
};

// 转换C++数据至数据库数据；比如,字符串类数据自动在两端添加单引号并且自动转义需要转义的字符
std::string DBData(bool val);
std::string DBData(s32 val);
std::string DBData(double val);
std::string DBData(const std::string &val);  // 注意，用于like语境时请使用DBLikeString
std::string DBData(const s8* val);           // 注意，用于like语境时请使用DBLikeString
std::string DBData(const CCMSTime &val);

std::string DBData(const Variant &val, ENDBDataType enDBDataType); // 转换Variant形式的C++数据转换至数据库数据

/* SQL92标准常量 */

const s8 SQLEscapeChar = '\\'; // 转义符
const s8 SQLAnyBytes = '%';    // like通配符, 匹配多个(包括0个)任意字符
const s8 SQLSingleByte = '_';  // like通配符, 匹配一个任意字符

// like语境下额外特殊字符的支持，不同数据库可能不同；此函数对除“%,_,\”外的特殊字符自动转义
std::string DBLikeString(const std::string &val, ENDBType enDBType);
std::string DBLikeString(const std::string &val); // 相比上面的函数，它使用初始化库时连接的数据库类型作为enDBType

// 生成存储过程调用语句
void GenerateExecProcSql(string &strSql, const string &strProcName, const vector<TVariantWrapper> &tParms, ENDBType enDBType);

// 利用实体结构自动获取结果集, 结果保存于pResultData指向的地址
class CDBEntitySchema;
class CDBEntityValue;
ENDBResult AutoFetchResultset(CDBConnection *pcDBConnection, const CDBEntitySchema &cSchema, CDBEntityValue* &pResultData);

DBIX_NS_END
#endif // _DBIX_UTIL_H_
