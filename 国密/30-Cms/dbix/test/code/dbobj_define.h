// 此文件最好使用工具生成

#ifndef _DBOBJ_DEFINE_H_
#define _DBOBJ_DEFINE_H_

#include "cms/dbix/dbix.h"
using namespace dbix;


#define T_ENTITYTEST "tblEntitytest"
#define F_ENTITYTEST_F_STRING "f_string"
#define F_ENTITYTEST_F_INT "f_int"
#define F_ENTITYTEST_F_BOOL "f_bool"
#define F_ENTITYTEST_F_DOUBLE "f_double"
#define F_ENTITYTEST_F_DATE "f_date"

/*

DROP TABLE IF EXISTS `test`.`tblEntitytest`;
CREATE TABLE  `test`.`tblEntitytest` (
`f_string` varchar(1024) NOT NULL,
`f_int` int(10) unsigned NOT NULL,
`f_bool` int(10) unsigned NOT NULL,
`f_double` double unsigned DEFAULT '0',
`f_date` bigint(20) unsigned NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

*/

class CEntitytestSchema: public CDBEntitySchema
{
public:
    CEntitytestSchema()
    {
        vector<TColInfo> cColInfo;
        TColInfo tCol;
        cColInfo.push_back(tCol.Assign(F_ENTITYTEST_F_STRING, enDBDataTypeString));
        cColInfo.push_back(tCol.Assign(F_ENTITYTEST_F_INT, enDBDataTypeS32));
        cColInfo.push_back(tCol.Assign(F_ENTITYTEST_F_BOOL, enDBDataTypeBool));
        cColInfo.push_back(tCol.Assign(F_ENTITYTEST_F_DOUBLE, enDBDataTypeDouble));
        cColInfo.push_back(tCol.Assign(F_ENTITYTEST_F_DATE, enDBDataTypeDate));
        InitSchema(T_ENTITYTEST, cColInfo);
    }
};
extern const CEntitytestSchema EntitytestSchema;


// 下面手工添加的内容
class CDBCmdDemo1ResultData: public CDbResultDataBase
{
public:
    string strResult;
};

class CDBCmdDemo2ResultData: public CDbResultDataBase
{
public:
    u32 dwResult;
};

#endif // _DBOBJ_DEFINE_H_
