/*========================================================================================
ģ����    ��dbix.lib
�ļ���    ��dbix_define.h
����ļ�  ��
ʵ�ֹ���  �����ݿ��������е���ض���
����      ��huzhiyun
��Ȩ      ��<Copyright(C) 2003-2008 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
�޸ļ�¼��
����               �汾              �޸���             �߶���              �޸ļ�¼
2012/04/27         V1.0              huzhiyun                              �½��ļ�
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
    string strDbName;      // �������ݿ����ƣ�Ҳ����ǰ����ʹ�õ����ݿ�
};

// �첽�������ݿ������pcTaskΪ�Ѷ���dwTaskQueueIndex��ʾͶ�����ĸ�������У�����falseʱ, �����߸����ͷ���
class CDBTask;
bool PostDBTask(CDBTask *pcTask, u32 dwTaskQueueIndex = 0);


// ���ݿ�����ص�������pcResultDataָ�벻ΪNULLʱ���ص�����ʵ���߸����ͷ���ָ�����Դ
class CDbResultDataBase // ���ݿ������ݻ��࣬���һ�������������ϲ�ʹ��ʱͳһ�ͷ���Դ
{
public:
    CDbResultDataBase(){}
    virtual ~CDbResultDataBase(){}
};
typedef void (*CBDBResult)(ENDBResult enDBResult, CDbResultDataBase *pcResultData, TCBParm tParm);

struct TTaskQueueConf
{
    TTaskQueueConf():dwThreadNum(0){}
    TDBConnectCfg tDBConnectCfg; // ���ݿ���������
    u32 dwThreadNum;             // �߳���Ŀ
};

struct TDBIXConf
{
    TDBIXConf():cbDBResult(NULL), dwThreadNum(0), dwConnectionMaxNum(0){}

    TDBConnectCfg tDBConnectCfg; // ���ݿ���������
    CBDBResult cbDBResult;       // ����ص�����
    u32 dwThreadNum;             // �߳���Ŀ
    u32 dwConnectionMaxNum;      // ע�⣺�ֶ�Ŀǰ��Ч
};
struct TDBIXConf2
{
    TDBIXConf2():cbDBResult(NULL){}

    CBDBResult cbDBResult;                   // ����ص�����
    vector<TTaskQueueConf> vecTaskQueueConf; // ����������� ע�⣺�������֮������ݿ�������Ҫ��ͬ
};

typedef std::string TblName; 
typedef std::string TColName;

enum ENDBDataType
{
    enDBDataTypeBool,   // ��Ӧbool  ���ݿ��ֶ�����Ϊ��ֵ����
    enDBDataTypeS32,    // ��Ӧs32
    enDBDataTypeDouble, // ��Ӧdouble
    enDBDataTypeString, // ��Ӧstring
    enDBDataTypeDate,   // ��ӦCCMSTime ���ݿ��ֶ�����Ϊ��ֵ����(��ʾ����utc 1970-1-1 0:0:0������)
};

// Variant��װ��
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
