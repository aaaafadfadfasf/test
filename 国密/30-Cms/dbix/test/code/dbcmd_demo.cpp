#include "inner_common.h"

class CDBCmdDemo1: public CDBCmd
{
public:
    CDBCmdDemo1(): CDBCmd(){}
private:
    virtual ENDBResult ExecImp(CDBConnection *pcDBConnection, CDbResultDataBase* &pcResultData)
    {
        pcResultData = new CDBCmdDemo1ResultData();

        if (pcDBConnection->ExecSQL("create table tbltest(f1 varchar(32))", CDBConnection::enExecNoRet))
        {
            static_cast<CDBCmdDemo1ResultData*>(pcResultData)->strResult.assign("CmdDemo1 success data");

            return dbix::enDBResultSuccess;
        }
        else
        {
            static_cast<CDBCmdDemo1ResultData*>(pcResultData)->strResult.assign("CmdDemo1 fail data");

            return dbix::enDBResultFail;
        }
    }
};

class CDBCmdDemo2: public CDBCmd
{
public:
    CDBCmdDemo2(): CDBCmd(){}
private:
    virtual ENDBResult ExecImp(CDBConnection *pcDBConnection, CDbResultDataBase* &pcResultData)
    {
        pcResultData = new CDBCmdDemo2ResultData;

        if (pcDBConnection->ExecSQL("select count(*) count from tbltest", CDBConnection::enExecMustRet))
        {
            static_cast<CDBCmdDemo2ResultData*>(pcResultData)->dwResult = pcDBConnection->GetInt("count");

            return dbix::enDBResultSuccess;
        }
        else
        {
            return dbix::enDBResultFail;
        }
    }
};

void DbCmdDemo1Test(u32 dwQueueIndex)
{
    CDBCmdDemo1 cCmd;
    cCmd.SetCBParm((u32)1);
    ExecDBCmd(cCmd, dwQueueIndex);
}

void DbCmdDemo2Test()
{
    CDBCmdDemo2 cCmd;
    cCmd.SetCBParm((u32)2);
    ExecDBCmd(cCmd);
}

void DbCmdDemo3Test()
{
    vector<TParmVal> parms;
    parms.push_back(TParmVal("autotest"));
    parms.push_back(TParmVal(123));

    CDBCmd cCmd("procDbCmdtest", parms);
    cCmd.SetCBParm((u32)3);
    ExecDBCmd(cCmd);

    /*
    DELIMITER $$

    DROP PROCEDURE IF EXISTS `test`.`procDbCmdtest` $$
    CREATE PROCEDURE `test`.`procDbCmdtest` (v1 varchar(1024), v2 int)
    BEGIN
    create table if not exists tblDbCmdtest(f1 varchar(1024), f2 int);
    insert into tblDbCmdtest values(v1, v2);
    END $$

    DELIMITER ;
    */
}
