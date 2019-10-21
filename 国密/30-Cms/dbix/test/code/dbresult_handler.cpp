#include "inner_common.h"

void DBResultHandler(ENDBResult enDBResult, CDbResultDataBase *pcResultData, TCBParm tParm)
{
    switch (tParm.dwParm)
    {
    case 1:
        {
            CDBCmdDemo1ResultData *pcData = static_cast<CDBCmdDemo1ResultData*>(pcResultData);
            printf("CDBCmdDemo1 return result:%d, return data:%s\n", (s32)enDBResult, pcData->strResult.c_str());
            delete pcData;

            break;
        }
    case 2:
        {
            CDBCmdDemo2ResultData *pcData = static_cast<CDBCmdDemo2ResultData*>(pcResultData);
            printf("CDBCmdDemo2 return result:%d, return data:%d\n", (s32)enDBResult, pcData->dwResult);
            delete pcData;

            break;
        }
    case 3:
        {
            printf("CDBCmdDemo3 return result:%d\n", (s32)enDBResult);

            break;
        }

    case 10:
        {
            printf("DbEntityAdd return result:%d\n", (s32)enDBResult);
            break;
        }
    case 11:
        {
            printf("DbEntityDel return result:%d\n", (s32)enDBResult);
            break;
        }
    case 12:
        {
            printf("DbEntityMod return result:%d\n", (s32)enDBResult);
            break;
        }
    case 13:
        {
            printf("DbEntityQry return result:%d\n", (s32)enDBResult);
            if (NULL != pcResultData)
            {
                CDBEntityValue *pcEntityValue = static_cast<CDBEntityValue*>(pcResultData);
                string strVal;
                s32 nVal;
                bool bVal;
                double dblVal;
                CCMSTime cVal;

                for (u32 i = 0; i < pcEntityValue->RowCount(); ++i)
                {
                    pcEntityValue->NextRow(i);
                    pcEntityValue->GetColumnVal(F_ENTITYTEST_F_STRING, strVal);
                    pcEntityValue->GetColumnVal(F_ENTITYTEST_F_INT, nVal);
                    pcEntityValue->GetColumnVal(F_ENTITYTEST_F_BOOL, bVal);
                    pcEntityValue->GetColumnVal(F_ENTITYTEST_F_DOUBLE, dblVal);
                    pcEntityValue->GetColumnVal(F_ENTITYTEST_F_DATE, cVal);

                    printf("DbEntity value:%s %d %s %f %s\n", 
                        strVal.c_str(), nVal, bVal?"true":"false", dblVal, cVal.ToString().c_str());
                }

                delete pcResultData;
            }

            break;
        }
    default:
        {
            printf("unhanded dbresult, cbParm=%u\n", tParm.dwParm);
            break;
        }
    }
}
