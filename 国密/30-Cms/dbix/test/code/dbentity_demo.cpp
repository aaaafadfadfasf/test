#include "inner_common.h"

void DbEntityAdd()
{
    CDBEntity cEntity(EntitytestSchema);
    cEntity.SetCBParm((u32)10);

    cEntity.SetAddFlag();
    cEntity.SetColumnVal(F_ENTITYTEST_F_STRING, "string value");
    cEntity.SetColumnVal(F_ENTITYTEST_F_INT, 123);
    cEntity.SetColumnVal(F_ENTITYTEST_F_BOOL, true);
    cEntity.SetColumnVal(F_ENTITYTEST_F_DOUBLE, 3.333333);
    cEntity.SetColumnVal(F_ENTITYTEST_F_DATE, CCMSTime());

    OperateDBEntity(cEntity);



    cEntity.SetColumnVal(F_ENTITYTEST_F_STRING, "string value");
    cEntity.SetColumnVal(F_ENTITYTEST_F_INT, 1234);
    s32 nVal;
    cEntity.GetColumnVal(F_ENTITYTEST_F_INT, nVal);
    printf("cEntity F_ENTITYTEST_F_INT:%d\n", nVal);
    cEntity.SetColumnVal(F_ENTITYTEST_F_BOOL, true);
    cEntity.SetColumnVal(F_ENTITYTEST_F_DOUBLE, 3.33333333);
    cEntity.SetColumnVal(F_ENTITYTEST_F_DATE, CCMSTime());

    OperateDBEntity(cEntity);


    cEntity.ClearColumnVal();
    cEntity.SetColumnVal(F_ENTITYTEST_F_STRING, "string value");
    cEntity.SetColumnVal(F_ENTITYTEST_F_INT, 12345);
    cEntity.SetColumnVal(F_ENTITYTEST_F_BOOL, true);
    cEntity.SetColumnVal(F_ENTITYTEST_F_DATE, CCMSTime());

    OperateDBEntity(cEntity);
}

void DbEntityDel()
{
    CDBEntity cEntity(EntitytestSchema);
    cEntity.SetCBParm((u32)11);

    CDBCriteria cDBCriteria = eq(F_ENTITYTEST_F_STRING, "string value");
    cEntity.SetDelFlag(&cDBCriteria);

    OperateDBEntity(cEntity);
}

void DbEntityMod()
{
    CDBEntity cEntity(EntitytestSchema);
    cEntity.SetCBParm((u32)12);

    CDBCriteria cDBCriteria = eq(F_ENTITYTEST_F_STRING, "string value");
    cEntity.SetModFlag(&cDBCriteria);

    cEntity.SetColumnVal(F_ENTITYTEST_F_INT, 456);
    cEntity.SetColumnVal(F_ENTITYTEST_F_DOUBLE, 0.333);

    OperateDBEntity(cEntity);
}

void DbEntityQry()
{
    CDBEntity cEntity(EntitytestSchema);
    cEntity.SetCBParm((u32)13);

    CDBCriteria cDBCriteria;
    TOrderby tOrderby;
    tOrderby.push_back(TOrderbyVal(F_ENTITYTEST_F_STRING, true));

    cEntity.SetQryFlag(0, 20, &cDBCriteria, &tOrderby);

    OperateDBEntity(cEntity);
}
