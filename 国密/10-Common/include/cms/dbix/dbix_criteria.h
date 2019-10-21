/*========================================================================================
模块名    ：dbix.lib
文件名    ：dbix_criteria.h
相关文件  ：
实现功能  ：实现数据库操作框架的过滤条件类。支持如下运算符：
           eq, ne, like, like_raw, gt, lt, le, ge, between, in;
           sql_and, sql_or, sql_not。它们的具体含义参见具体函数注释。
作者      ：huzhiyun
版权      ：<Copyright(C) 2003-2008 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2012/05/04         V1.0              huzhiyun                               新建文件
=========================================================================================*/
#ifndef _DBIX_CRITERIA_H_
#define _DBIX_CRITERIA_H_
#include "dbix_define.h"

DBIX_NS_BEGIN

// 过滤条件类
typedef std::string CDBCriteria; 

/**
* Apply an "equal" constraint to the named column
* @param columnName
* @param val
* @return CDBCriteria
*/
template<typename T> 
CDBCriteria eq(const TColName& tColName, T val) 
{
    return tColName + " = " + DBData(val);
}
/**
* Apply a "not equal" constraint to the named column
* @param columnName
* @param val
* @return CDBCriteria
*/
template<typename T>
CDBCriteria ne(const TColName& tColName, T val) 
{
    return tColName + " != " + DBData(val);
}
/**
* Apply a "like" constraint to the named column
* @param columnName
* @param val
* @return CDBCriteria
*/
inline CDBCriteria like(const TColName& tColName, const string &val) 
{
    return tColName + " like " + DBLikeString(val);
}
inline CDBCriteria like(const TColName& tColName, const s8* val)
{
    return tColName + " like " + DBLikeString(val);
}
inline CDBCriteria like_raw(const TColName& tColName, const string &val) 
{
    // relative to like, it will not escape the especial word automacally
    return tColName + " like " + DBData(val);
}
inline CDBCriteria like_raw(const TColName& tColName, const s8* val) 
{
    // relative to like, it will not escape the especial word automacally
    return tColName + " like " + DBData(val);
}
/**
* Apply a "greater than" constraint to the named column
* @param columnName
* @param val
* @return CDBCriteria
*/
template<typename T>
CDBCriteria gt(const TColName& tColName, T val) 
{
    return tColName + " > " + DBData(val);
}
/**
* Apply a "less than" constraint to the named column
* @param columnName
* @param val
* @return CDBCriteria
*/
template<typename T>
CDBCriteria lt(const TColName& tColName, T val) 
{
    return tColName + " < " + DBData(val);
}
/**
* Apply a "less than or equal" constraint to the named column
* @param columnName
* @param val
* @return CDBCriteria
*/
template<typename T>
CDBCriteria le(const TColName& tColName, T val) 
{
    return tColName + " <= " + DBData(val);
}
/**
* Apply a "greater than or equal" constraint to the named column
* @param columnName
* @param val
* @return CDBCriteria
*/
template<typename T>
CDBCriteria ge(const TColName& tColName, T val) 
{
    return tColName + " >= " + DBData(val);
}
/**
* Apply a "between" constraint to the named column
* @param columnName
* @param lo value
* @param hi value
* @return CDBCriteria
*/
template<typename T>
CDBCriteria between(const TColName& tColName, T lo, T hi) 
{
    return tColName + " between " + DBData(lo) + " and " + DBData(hi);
}

/**
* Apply an "in" constraint to the named column
* @param columnName
* @param strDBDataList, prepared database data list which is splitted with comma.
* @return CDBCriteria
*/
inline CDBCriteria in(const TColName& tColName, const string& strDBDataList) 
{
    return tColName + " in (" + strDBDataList + ")";
}

/**
* Return the conjuction of two expressions
*
* @param lhs
* @param rhs
* @return CDBCriteria
*/
inline CDBCriteria sql_and(const CDBCriteria& lhs, const CDBCriteria& rhs) 
{
    return "(" + lhs + ") and (" + rhs + ")";
}

/**
* Return the disjuction of two expressions
*
* @param lhs
* @param rhs
* @return CDBCriteria
*/
inline CDBCriteria sql_or(const CDBCriteria& lhs, const CDBCriteria& rhs) 
{
    return "(" + lhs + ") or (" + rhs + ")";
}
/**
* Return the negation of an expression
*
* @param expression
* @return CDBCriteria
*/
inline CDBCriteria sql_not(const CDBCriteria& val) 
{
    return "not(" + val + ")";
}

DBIX_NS_END
#endif // _DBIX_CRITERIA_H_
