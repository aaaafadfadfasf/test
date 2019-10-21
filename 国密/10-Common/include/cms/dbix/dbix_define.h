/*========================================================================================
模块名    ：dbix.lib
文件名    ：dbix_define.h
相关文件  ：
实现功能  ：数据库操作框架中的相关定义
作者      ：huzhiyun
版权      ：<Copyright(C) 2003-2008 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2012/04/27         V1.0              huzhiyun                              新建文件
=========================================================================================*/
#ifndef _DBIX_DEFINE_H_
#define _DBIX_DEFINE_H_

#ifndef DBIX_NS_BEGIN
#define DBIX_NS_BEGIN namespace dbix {
#endif // DBIX_NS_BEGIN
#ifndef DBIX_NS_END
#define DBIX_NS_END }; // namespace dbix
#endif // DBIX_NS_END

#include <cassert>
#include <cstdarg>
#include <cstring>
#include <cstdio>
#include <string>
using std::string;
#include <vector>
using std::vector;
#include <map>
using std::map;
#include "kdvtype.h"
#include "variant.h"
#include "cms/utility/cmstime.h"

DBIX_NS_BEGIN

enum ENDBResult
{
    enDBResultSuccess = 0,
    enDBResultFail = 1,
};

union TCBParm
{
    u32 dwParm;
    u64 qwParm;
    void *pParm;
};

enum ENDBType
{
    enDBTypeSybase = 1,
    enDBTypeMysql = 2,
    enDBTypeOracle = 3
};

struct TDBConnectCfg
{
    TDBConnectCfg():enDbType(enDBTypeMysql), wDbServerPort(0){}

    bool operator == (const TDBConnectCfg &rh) const
    {
        return (enDbType == rh.enDbType) && (strDbServerIP == rh.strDbServerIP)
            && (wDbServerPort == rh.wDbServerPort) && (strDbUserName == rh.strDbUserName)
            && (strDbUserPassword == rh.strDbUserPassword) && (strDbName == rh.strDbName);
    }

    ENDBType enDbType;
    string strDbServerIP;
    u16 wDbServerPort;
    string strDbUserName;
    string strDbUserPassword;
    string strDbName;      // 工作数据库名称，也即当前正在使用的数据库
};

// 异步请求数据库操作；pcTask为堆对象，dwTaskQueueIndex表示投递至哪个任务队列；返回false时, 调用者负责释放它
class CDBTask;
bool PostDBTask(CDBTask *pcTask, u32 dwTaskQueueIndex = 0);


// 数据库操作回调函数；pcResultData指针不为NULL时，回调函数实现者负责释放它指针的资源
class CDbResultDataBase // 数据库结果数据基类，设计一个基类有利于上层使用时统一释放资源
{
public:
    CDbResultDataBase(){}
    virtual ~CDbResultDataBase(){}
};
typedef void (*CBDBResult)(ENDBResult enDBResult, CDbResultDataBase *pcResultData, TCBParm tParm);

struct TTaskQueueConf
{
    TTaskQueueConf():dwThreadNum(0){}
    TDBConnectCfg tDBConnectCfg; // 数据库连接配置
    u32 dwThreadNum;             // 线程数目
};

struct TDBIXConf
{
    TDBIXConf():cbDBResult(NULL), dwThreadNum(0), dwConnectionMaxNum(0){}

    TDBConnectCfg tDBConnectCfg; // 数据库连接配置
    CBDBResult cbDBResult;       // 结果回调函数
    u32 dwThreadNum;             // 线程数目
    u32 dwConnectionMaxNum;      // 注意：字段目前无效
};
struct TDBIXConf2
{
    TDBIXConf2():cbDBResult(NULL){}

    CBDBResult cbDBResult;                   // 结果回调函数
    vector<TTaskQueueConf> vecTaskQueueConf; // 任务队列配置 注意：任务队列之间的数据库类型需要相同
};

typedef std::string TblName; 
typedef std::string TColName;

enum ENDBDataType
{
    enDBDataTypeBool,   // 对应bool  数据库字段类型为数值类型
    enDBDataTypeS32,    // 对应s32
    enDBDataTypeDouble, // 对应double
    enDBDataTypeString, // 对应string
    enDBDataTypeDate,   // 对应CCMSTime 数据库字段类型为数值类型(表示距离utc 1970-1-1 0:0:0的秒数)
};

// Variant包装器
struct TVariantWrapper
{
public:
    TVariantWrapper(bool val): m_enDBDataType(enDBDataTypeBool), m_enValue(val){}
    TVariantWrapper(s32 val): m_enDBDataType(enDBDataTypeS32), m_enValue(val){}
    TVariantWrapper(double val):m_enDBDataType(enDBDataTypeDouble), m_enValue(val){}
    TVariantWrapper(const string &val): m_enDBDataType(enDBDataTypeString), m_enValue(val){}
    TVariantWrapper(const s8 *val): m_enDBDataType(enDBDataTypeString), m_enValue(string(val)){}
    TVariantWrapper(const CCMSTime &val): m_enDBDataType(enDBDataTypeDate), m_enValue(val){}

    ENDBDataType DataType() const{return m_enDBDataType;}
    const Variant& DataVal() const{return m_enValue;}

private:
    ENDBDataType m_enDBDataType;
    Variant m_enValue;
};

DBIX_NS_END
#endif // _DBIX_H_
