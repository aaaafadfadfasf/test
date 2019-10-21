/*========================================================================================
模块名    ：dbix.lib
文件名    ：dbix.h
相关文件  ：
实现功能  ：实现数据库操作框架，采用异步请求+回调返回结果的编程模式；
            数据库中的3种对象——表，视图和存储过程，前二者被抽象成DBEntity, 后者被抽象成DBCmd。
作者      ：huzhiyun
版权      ：<Copyright(C) 2003-2008 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2012/04/25         V1.0              huzhiyun                               新建文件
=========================================================================================*/
#ifndef _DBIX_H_
#define _DBIX_H_
#include "dbix_define.h"
#include "dbix_util.h"
#include "dbix_criteria.h"

DBIX_NS_BEGIN

// 操作基类
class CDBTask
{
public:
    CDBTask(): m_pcDBConnection(NULL){m_tCBParm.qwParm = 0;}
    CDBTask(const CDBTask &cTask): m_pcDBConnection(cTask.m_pcDBConnection), m_tCBParm(cTask.m_tCBParm){}
    virtual ~CDBTask(){}

    void SetCBParm(const TCBParm &tParm)
    {
        m_tCBParm = tParm;
    }
    void SetCBParm(u32 val)
    {
        m_tCBParm.qwParm = 0;
        m_tCBParm.dwParm = val;
    }
    void SetCBParm(u64 val)
    {
        m_tCBParm.qwParm = val;
    }
    void SetCBParm(void* val)
    {
        m_tCBParm.qwParm = 0;
        m_tCBParm.pParm = val;
    }
    const TCBParm& GetCBParm() const
    {
        return m_tCBParm;
    }

    ENDBResult Exec(CDBConnection *pcDBConnection, CDbResultDataBase* &pcResultData)
    {
        m_pcDBConnection = pcDBConnection;
        return ExecImp(pcDBConnection, pcResultData);
    }
    CDBConnection* DBConnection() const
    {
        return m_pcDBConnection;
    }

protected:
    virtual ENDBResult ExecImp(CDBConnection *pcDBConnection, CDbResultDataBase* &pcResultData) = 0;

private:
    CDBConnection *m_pcDBConnection; // 任务使用的数据库连接
    TCBParm m_tCBParm;               // 回调参数, 请求时传入，回调时带回
};



// 实体列
struct TColInfo
{
    TColInfo(){}
    TColInfo(const TColName &colName_, ENDBDataType colDataType_):colName(colName_), colDataType(colDataType_){}
    TColInfo& Assign(const TColName &tColName, ENDBDataType dataType)
    {
        colName = tColName;
        colDataType = dataType;
        return *this;
    }
    TColName colName;
    ENDBDataType colDataType;
};
// 实体结构类
class CDBEntitySchema
{
public:
    CDBEntitySchema(){}
    CDBEntitySchema(const CDBEntitySchema &cSchema)
        : m_tTblName(cSchema.m_tTblName), m_tColInfos(cSchema.m_tColInfos){}
    virtual ~CDBEntitySchema(){}

    // 初始化实体信息，设置表名，列名和列数据类型; 建议在此类的派生类的构造函数中设置
    void InitSchema(const TblName &tblName, const vector<TColInfo> &tColInfos)
    {
        m_tTblName = tblName;
        m_tColInfos = tColInfos;
    }
    const TblName& TableName() const{return m_tTblName;}
    const vector<TColInfo>& ColumnInfos() const{return m_tColInfos;}
    bool ColumnDBDataType(const TColName &tColName, ENDBDataType &enDBDataType, u32 *pColIndex = NULL) const
    {
        for (u32 i = 0; i < m_tColInfos.size(); ++i)
        {
            if (m_tColInfos[i].colName == tColName)
            {
                enDBDataType = m_tColInfos[i].colDataType;
                if (NULL != pColIndex) *pColIndex = i;
                return true;
            }
        }

        return false;
    }
private:
    TblName m_tTblName;                  // 表名
    vector<TColInfo> m_tColInfos;        // 列名和列数据类型
};

// 实体值类
class CDBEntityValue: public CDbResultDataBase
{
public:
    typedef vector<Variant> TRowValue;
public:
    CDBEntityValue(const CDBEntitySchema &cSchema): m_cSchema(cSchema), m_dwCurrentRow(0xffffffff){}
    CDBEntityValue(const CDBEntityValue &cValue)
        : m_cSchema(cValue.m_cSchema), m_tRowValues(cValue.m_tRowValues), m_dwCurrentRow(cValue.m_dwCurrentRow){}

    /* 设置时先添加行，然后再依次添加行内的列值 */
    void Clear(){m_tRowValues.clear(); m_dwCurrentRow = 0xffffffff;}
    void AppendRow(){m_tRowValues.push_back(TRowValue()); m_dwCurrentRow = m_tRowValues.size()-1;}
    bool SetColumnVal(bool val)
    {
        return SetColumnVal<bool, enDBDataTypeBool>(val);
    }
    bool SetColumnVal(s32 val)
    {
        return SetColumnVal<s32, enDBDataTypeS32>(val);
    }
    bool SetColumnVal(double val)
    {
        return SetColumnVal<double, enDBDataTypeDouble>(val);
    }
    bool SetColumnVal(const string &val)
    {
        return SetColumnVal<string, enDBDataTypeString>(val);
    }
    bool SetColumnVal(const s8* val)
    {
        return SetColumnVal(string(val));
    }
    bool SetColumnVal(const CCMSTime &val)
    {
        return SetColumnVal<CCMSTime, enDBDataTypeDate>(val);
    }

    /* 获取时先根据行数设置当前行，然后再在当前行中按列名取列值 */
    u32 RowCount(){return static_cast<u32>(m_tRowValues.size());}
    bool NextRow(u32 dwIndex)
    { 
        if (m_tRowValues.size() > dwIndex)
        {
            m_dwCurrentRow = dwIndex;
            return true;
        }
        else
        {
            assert(false);
            return false;
        }
    }
    bool GetColumnVal(const TColName &colName, bool &val)
    {
        return GetColumnVal<enDBDataTypeBool, bool>(colName, val);
    }
    bool GetColumnVal(const TColName &colName, s32 &val)
    {
        return GetColumnVal<enDBDataTypeS32, s32>(colName, val);
    }
    bool GetColumnVal(const TColName &colName, double &val)
    {
        return GetColumnVal<enDBDataTypeDouble, double>(colName, val);
    }
    bool GetColumnVal(const TColName &colName, string &val)
    {
        return GetColumnVal<enDBDataTypeString, string>(colName, val);
    }
    bool GetColumnVal(const TColName &colName, CCMSTime &val)
    {
        return GetColumnVal<enDBDataTypeDate, CCMSTime>(colName, val);
    }

private:
    template<typename CppDataType, ENDBDataType DbDataType>
    bool SetColumnVal(const CppDataType &val)
    {
        u32 dwColumnValNum = m_tRowValues[m_dwCurrentRow].size();
        if (m_tRowValues.size() == (m_dwCurrentRow+1) && m_cSchema.ColumnInfos().size() > dwColumnValNum)
        {
            if (DbDataType == m_cSchema.ColumnInfos()[dwColumnValNum].colDataType)
            {
                m_tRowValues[m_dwCurrentRow].push_back(Variant(val));
                return true;
            }
            else
            {
                assert(false && "datatype not match");
                return false;
            }
        }
        else
        {
            assert(false);
            return false;
        }
    }
    template<ENDBDataType DbDataType, typename CppDataType>
    bool GetColumnVal(const TColName &colName, CppDataType &val)
    {
        ENDBDataType enDBDataType;
        u32 dwColIndex;
        if (m_cSchema.ColumnDBDataType(colName, enDBDataType, &dwColIndex) && enDBDataType == DbDataType)
        {
            if (m_tRowValues.size() > m_dwCurrentRow && 
                m_tRowValues[m_dwCurrentRow].size() == m_cSchema.ColumnInfos().size())
            {
                if (m_tRowValues[m_dwCurrentRow][dwColIndex].CopyTo<CppDataType>(val))
                    return true;
                else
                {
                    assert(false && "none value");
                    return false;
                }
            }
            else
            {
                assert(false && "without certain row and column's value");
                return false;
            }
        }
        else
        {
            assert(false && "without certain column name");
            return false;
        }
    }
private:
    CDBEntitySchema m_cSchema;           // 结构
    vector<TRowValue> m_tRowValues;      // 行值(类型由结构中的对应列类型决定)
    u32 m_dwCurrentRow;                  // 当前行号(-1表示无效)
};

// 排序控制类
struct TOrderbyVal
{
    TOrderbyVal(){}
    TOrderbyVal(const TColName &tCol, bool bAsc): colName(tCol), asc(bAsc){}
    TOrderbyVal& Assign(const TColName &tCol, bool bAsc){colName = tCol; asc = bAsc;return *this;}

    TColName colName;
    bool asc; // 升序
};
typedef vector<TOrderbyVal> TOrderby;

// 实体操作类, 自动生成代码时查询结果利用CDBEntityValue返回(记录数为0时也一个这样的无数据的对象)
class CDBEntity: public CDBTask
{
public:
    enum ENOperate
    {
        enOperateNone,
        enOperateAdd,
        enOperateDel,
        enOperateMod,
        enOperateQry
    };
    
public:
    CDBEntity(const CDBEntitySchema &cSchema): m_cSchema(cSchema), m_enOperate(enOperateNone) {}
    CDBEntity(const CDBEntity &cDBEntity)
        : CDBTask(cDBEntity), m_cSchema(cDBEntity.m_cSchema), m_enOperate(cDBEntity.m_enOperate),
          m_tColValues(cDBEntity.m_tColValues), m_cConditions(cDBEntity.m_cConditions),
          m_tOrderby(cDBEntity.m_tOrderby), m_dwEntityIndex(cDBEntity.m_dwEntityIndex),
          m_dwEntityNum(cDBEntity.m_dwEntityNum) {}

    // 指定条件时先删除符合条件的记录再进行添加，否则直接进行添加；注意:此处如果要指定条件则条件的内容不能为空
    void SetAddFlag(const CDBCriteria *pcCondition = NULL)
    { 
        m_enOperate = enOperateAdd;
        if (NULL != pcCondition)
        {
            m_cConditions = *pcCondition;
        }
        else
        {
            m_cConditions = CDBCriteria();
        }
    }
    void SetDelFlag(const CDBCriteria *pcCondition = NULL)
    {
        m_enOperate = enOperateDel;
        if (NULL != pcCondition)
        {
            m_cConditions = *pcCondition;
        }
        else
        {
            m_cConditions = CDBCriteria();
        }
    }
    void SetModFlag(const CDBCriteria *pcCondition = NULL)
    {
        m_enOperate = enOperateMod;
        if (NULL != pcCondition)
        {
            m_cConditions = *pcCondition;
        }
        else
        {
            m_cConditions = CDBCriteria();
        }
    }
    // dwEntityNum为0时表示不限制, 服务器端实现时一般内部会存在一个限制的
    void SetQryFlag(u32 dwEntityIndex=0, u32 dwEntityNum=10000, 
                    const CDBCriteria *pcCondition = NULL, const TOrderby *ptOrderby=NULL)
    {
        m_enOperate = enOperateQry;
        m_dwEntityIndex = dwEntityIndex;
        m_dwEntityNum = (dwEntityNum > 0? dwEntityNum:0xffffffff);
        if (NULL != pcCondition)
        {
            m_cConditions = *pcCondition;
        }
        else
        {
            m_cConditions = CDBCriteria();
        }
        if (NULL != ptOrderby)
        {
            m_tOrderby = *ptOrderby;

#ifdef DEBUG  // debug版本时校验一下参数的合法性
            ENDBDataType enDBDataType;
            for (u32 i = 0; i < m_tOrderby.size(); ++i)
            {
                assert(m_cSchema.ColumnDBDataType(m_tOrderby[i].colName, enDBDataType));
            }
#endif // DEBUG
        }
        else
        {
            m_tOrderby = TOrderby();
        }
    }

    /* 此组方法只对添加和修改操作有效；添加时表示添加的列值，修改时表示新的列值 */
    void ClearColumnVal(const TColName &colName=TColName()) // 默认删除所有列值
    {
        if (colName.empty())
        {
            m_tColValues.clear();
        }
        else
        {
            m_tColValues.erase(colName);
        }
    }
    bool SetColumnVal(const TColName &colName, bool val)
    {
        return SetColumnVal<bool, enDBDataTypeBool>(colName, val);
    }
    bool SetColumnVal(const TColName &colName, s32 val)
    {
        return SetColumnVal<s32, enDBDataTypeS32>(colName, val);
    }
    bool SetColumnVal(const TColName &colName, double val)
    {
        return SetColumnVal<double, enDBDataTypeDouble>(colName, val);
    }
    bool SetColumnVal(const TColName &colName, const string &val)
    {
        return SetColumnVal<string, enDBDataTypeString>(colName, val);
    }
    bool SetColumnVal(const TColName &colName, const s8 *val)
    {
        return SetColumnVal<string, enDBDataTypeString>(colName, string(val));
    }
    bool SetColumnVal(const TColName &colName, const CCMSTime &val)
    {
        return SetColumnVal<CCMSTime, enDBDataTypeDate>(colName, val);
    }

    bool GetColumnVal(const TColName &colName, bool &val)
    {
        return GetColumnVal<bool, enDBDataTypeBool>(colName, val);
    }
    bool GetColumnVal(const TColName &colName, s32 &val)
    {
        return GetColumnVal<s32, enDBDataTypeS32>(colName, val);
    }
    bool GetColumnVal(const TColName &colName, double &val)
    {
        return GetColumnVal<double, enDBDataTypeDouble>(colName, val);
    }
    bool GetColumnVal(const TColName &colName, string &val)
    {
        return GetColumnVal<string, enDBDataTypeString>(colName, val);
    }
    bool GetColumnVal(const TColName &colName, CCMSTime &val)
    {
        return GetColumnVal<CCMSTime, enDBDataTypeDate>(colName, val);
    }


protected:
    // 实现算法：根据实体结构、操作类型以及相关参数自动生成操作代码并且执行, 操作类型为查询时自动获取查询内容
    virtual ENDBResult ExecImp(CDBConnection *pcDBConnection, CDbResultDataBase* &pcResultData);

private:
    template<typename CppDataType, ENDBDataType DbDataType>
    bool SetColumnVal(const TColName &colName, const CppDataType &val)
    {
        assert(m_enOperate == enOperateAdd || m_enOperate == enOperateMod);
        ENDBDataType enDbDataType;
        if (m_cSchema.ColumnDBDataType(colName, enDbDataType) && enDbDataType == DbDataType)
        {
            std::pair<map<TColName, Variant>::iterator, bool> pr = m_tColValues.insert(map<TColName, Variant>::value_type(colName, val));
            if (!pr.second)
            {
                pr.first->second = val;
            }
            return true;
        }
        else
        {
            assert(false && "not found certain column");
            return false;
        }
    }
    template<typename CppDataType, ENDBDataType DbDataType>
    bool GetColumnVal(const TColName &colName, CppDataType &val)
    {
        assert(m_enOperate == enOperateAdd || m_enOperate == enOperateMod);
        ENDBDataType enDbDataType;
        if (m_cSchema.ColumnDBDataType(colName, enDbDataType) && enDbDataType == DbDataType)
        {
            map<TColName, Variant>::const_iterator it = m_tColValues.find(colName);
            if (it != m_tColValues.end())
            {
                return it->second.CopyTo(val);
            }
            else
            {
                return false;
            }
        }
        else
        {
            assert(false && "not found certain column");
            return false;
        }
    }

protected:
    CDBEntitySchema m_cSchema;           // 实体结构
    ENOperate m_enOperate;               // 操作类型

    map<TColName, Variant> m_tColValues; // 列值(添加,修改操作使用)
    CDBCriteria m_cConditions;           // 条件(添加,删除，修改，查询三种操作使用)
    TOrderby m_tOrderby;                 // 排序(查询操作使用)
    u32 m_dwEntityIndex, m_dwEntityNum;  // 分页控制(查询操作使用, m_dwEntityIndex为-1时表示不启用分页)
};



// 命令参数类
typedef TVariantWrapper TParmVal;

// 命令操作类
class CDBCmd: public CDBTask
{
public:
    CDBCmd(){}
    CDBCmd(const string &strCmdName, const vector<TParmVal> &tParms)
        : m_strCmdName(strCmdName), m_tParmValues(tParms) {}
    CDBCmd(const CDBCmd &cCmd)
        : CDBTask(cCmd), m_strCmdName(cCmd.m_strCmdName), m_tParmValues(cCmd.m_tParmValues){}
    void InitCmdInfo(const string &strCmdName, const vector<TParmVal> *ptParms = NULL)
    {
        m_strCmdName = strCmdName;
        if (NULL != ptParms) 
        {
            m_tParmValues = *ptParms;
        }
        else
        {
            m_tParmValues = vector<TParmVal>();
        }
    }


protected:
    // 实现算法：以m_strCmdName为存储过程名字和m_tParmValues为调用参数，自动生成存储过程调用语句并且执行
    virtual ENDBResult ExecImp(CDBConnection *pcDBConnection, CDbResultDataBase* &pcResultData);

protected:
    string m_strCmdName;              // 命令名称
    vector<TParmVal> m_tParmValues;   // 命名参数, 顺序与数组索引一致
};



/* 初始化与反初始化DBIX库 */
bool DBIXInit(const TDBIXConf &tConf);
bool DBIXInit(const TDBIXConf2 &tConf);
void DBIXQuit();

/* 异步操作数据库对象，结果通过初始化库时设置的回调函数返回 */
template<typename DBEntityType>
bool OperateDBEntity(const DBEntityType &cDBEntity, u32 dwTaskQueueIndex = 0)
{
    CDBTask *pcDBTask = new DBEntityType(cDBEntity);
    bool bRet = PostDBTask(pcDBTask, dwTaskQueueIndex);
    if (bRet)
    {
        return true;
    }
    else
    {
        delete pcDBTask;
        return false;
    }
}
template<typename DBCmdType>
bool ExecDBCmd(const DBCmdType &cDBCmd, u32 dwTaskQueueIndex = 0)
{
    CDBTask *pcDBTask = new DBCmdType(cDBCmd);
    bool bRet = PostDBTask(pcDBTask, dwTaskQueueIndex);
    if (bRet)
    {
        return true;
    }
    else
    {
        delete pcDBTask;
        return false;
    }
}


/* dbix日志级别控制 */
enum TLogLvl
{
    enLOGLVL_ERROR = 1,         // 重要错误，可能会影响系统正常运行的
    enLOGLVL_CRITICAL_PATH = 3, // 关键流程，包括初始化流程与关键业务流程等
    enLOGLVL_WARN = 4,          // 警告错误，系统内部能够正确地处理掉

    enLOGLVL_DEBUG = 7,         // 调试信息，打印详细信息
};
void DbixLoglvl(TLogLvl tLvl);

DBIX_NS_END
#endif // _DBIX_H_
