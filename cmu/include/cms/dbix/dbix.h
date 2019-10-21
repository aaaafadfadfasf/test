/*========================================================================================
ģ����    ��dbix.lib
�ļ���    ��dbix.h
����ļ�  ��
ʵ�ֹ���  ��ʵ�����ݿ������ܣ������첽����+�ص����ؽ���ı��ģʽ��
            ���ݿ��е�3�ֶ��󡪡�����ͼ�ʹ洢���̣�ǰ���߱������DBEntity, ���߱������DBCmd��
����      ��huzhiyun
��Ȩ      ��<Copyright(C) 2003-2008 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
�޸ļ�¼��
����               �汾              �޸���             �߶���              �޸ļ�¼
2012/04/25         V1.0              huzhiyun                               �½��ļ�
=========================================================================================*/
#ifndef _DBIX_H_
#define _DBIX_H_
#include "dbix_define.h"
#include "dbix_util.h"
#include "dbix_criteria.h"

DBIX_NS_BEGIN

// ��������
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
    CDBConnection *m_pcDBConnection; // ����ʹ�õ����ݿ�����
    TCBParm m_tCBParm;               // �ص�����, ����ʱ���룬�ص�ʱ����
};



// ʵ����
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
// ʵ��ṹ��
class CDBEntitySchema
{
public:
    CDBEntitySchema(){}
    CDBEntitySchema(const CDBEntitySchema &cSchema)
        : m_tTblName(cSchema.m_tTblName), m_tColInfos(cSchema.m_tColInfos){}
    virtual ~CDBEntitySchema(){}

    // ��ʼ��ʵ����Ϣ�����ñ���������������������; �����ڴ����������Ĺ��캯��������
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
    TblName m_tTblName;                  // ����
    vector<TColInfo> m_tColInfos;        // ����������������
};

// ʵ��ֵ��
class CDBEntityValue: public CDbResultDataBase
{
public:
    typedef vector<Variant> TRowValue;
public:
    CDBEntityValue(const CDBEntitySchema &cSchema): m_cSchema(cSchema), m_dwCurrentRow(0xffffffff){}
    CDBEntityValue(const CDBEntityValue &cValue)
        : m_cSchema(cValue.m_cSchema), m_tRowValues(cValue.m_tRowValues), m_dwCurrentRow(cValue.m_dwCurrentRow){}

    /* ����ʱ������У�Ȼ��������������ڵ���ֵ */
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

    /* ��ȡʱ�ȸ����������õ�ǰ�У�Ȼ�����ڵ�ǰ���а�����ȡ��ֵ */
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
    CDBEntitySchema m_cSchema;           // �ṹ
    vector<TRowValue> m_tRowValues;      // ��ֵ(�����ɽṹ�еĶ�Ӧ�����;���)
    u32 m_dwCurrentRow;                  // ��ǰ�к�(-1��ʾ��Ч)
};

// ���������
struct TOrderbyVal
{
    TOrderbyVal(){}
    TOrderbyVal(const TColName &tCol, bool bAsc): colName(tCol), asc(bAsc){}
    TOrderbyVal& Assign(const TColName &tCol, bool bAsc){colName = tCol; asc = bAsc;return *this;}

    TColName colName;
    bool asc; // ����
};
typedef vector<TOrderbyVal> TOrderby;

// ʵ�������, �Զ����ɴ���ʱ��ѯ�������CDBEntityValue����(��¼��Ϊ0ʱҲһ�������������ݵĶ���)
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

    // ָ������ʱ��ɾ�����������ļ�¼�ٽ�����ӣ�����ֱ�ӽ�����ӣ�ע��:�˴����Ҫָ�����������������ݲ���Ϊ��
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
    // dwEntityNumΪ0ʱ��ʾ������, ��������ʵ��ʱһ���ڲ������һ�����Ƶ�
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

#ifdef DEBUG  // debug�汾ʱУ��һ�²����ĺϷ���
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

    /* ���鷽��ֻ����Ӻ��޸Ĳ�����Ч�����ʱ��ʾ��ӵ���ֵ���޸�ʱ��ʾ�µ���ֵ */
    void ClearColumnVal(const TColName &colName=TColName()) // Ĭ��ɾ��������ֵ
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
    // ʵ���㷨������ʵ��ṹ�����������Լ���ز����Զ����ɲ������벢��ִ��, ��������Ϊ��ѯʱ�Զ���ȡ��ѯ����
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
    CDBEntitySchema m_cSchema;           // ʵ��ṹ
    ENOperate m_enOperate;               // ��������

    map<TColName, Variant> m_tColValues; // ��ֵ(���,�޸Ĳ���ʹ��)
    CDBCriteria m_cConditions;           // ����(���,ɾ�����޸ģ���ѯ���ֲ���ʹ��)
    TOrderby m_tOrderby;                 // ����(��ѯ����ʹ��)
    u32 m_dwEntityIndex, m_dwEntityNum;  // ��ҳ����(��ѯ����ʹ��, m_dwEntityIndexΪ-1ʱ��ʾ�����÷�ҳ)
};



// ���������
typedef TVariantWrapper TParmVal;

// ���������
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
    // ʵ���㷨����m_strCmdNameΪ�洢�������ֺ�m_tParmValuesΪ���ò������Զ����ɴ洢���̵�����䲢��ִ��
    virtual ENDBResult ExecImp(CDBConnection *pcDBConnection, CDbResultDataBase* &pcResultData);

protected:
    string m_strCmdName;              // ��������
    vector<TParmVal> m_tParmValues;   // ��������, ˳������������һ��
};



/* ��ʼ���뷴��ʼ��DBIX�� */
bool DBIXInit(const TDBIXConf &tConf);
bool DBIXInit(const TDBIXConf2 &tConf);
void DBIXQuit();

/* �첽�������ݿ���󣬽��ͨ����ʼ����ʱ���õĻص��������� */
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


/* dbix��־������� */
enum TLogLvl
{
    enLOGLVL_ERROR = 1,         // ��Ҫ���󣬿��ܻ�Ӱ��ϵͳ�������е�
    enLOGLVL_CRITICAL_PATH = 3, // �ؼ����̣�������ʼ��������ؼ�ҵ�����̵�
    enLOGLVL_WARN = 4,          // �������ϵͳ�ڲ��ܹ���ȷ�ش����

    enLOGLVL_DEBUG = 7,         // ������Ϣ����ӡ��ϸ��Ϣ
};
void DbixLoglvl(TLogLvl tLvl);

DBIX_NS_END
#endif // _DBIX_H_
