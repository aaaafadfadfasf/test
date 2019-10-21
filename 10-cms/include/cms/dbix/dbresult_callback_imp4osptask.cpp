/* 此文件不是DBIX库必须的，为了方便上层使用而添加的. 本来是由上层使用者编译连接的。
   为了解决公司linux common make脚本源文件无法跨目录的问题，所以包含于dbix里面编译 */

#include "dbresult_callback_imp4osptask.h"

/*lint -esym(1746,tParm)*/
void cbDbResultHandler(ENDBResult enDBResult, CDbResultDataBase* pcResultData, TCBParm tParm)
{
    u64 qwOspTransId = tParm.qwParm;
    u32 dwOspIID = qwOspTransId >> 32;
    u32 dwOspTaskId = (u32)qwOspTransId;

    TDbixResult cMsg;
    cMsg.dwTaskId = dwOspTaskId;
    cMsg.enDBResult = enDBResult;
    cMsg.pcDBData = pcResultData;

    GLOBELOG(EVENT_LEV, "dbix回调处理，目的事务：OspIID[%u], TaskId[%u]\n", dwOspIID, dwOspTaskId);

    if (0 != ::OspPost(dwOspIID, DBIX_RESULT_CB, &cMsg, sizeof(cMsg)))
    {
        GLOBELOG(EVENT_LEV, "dbix回调处理，目的事务：OspIID[%u], TaskId[%u], OspPost投递失败\n", dwOspIID, dwOspTaskId);

        if (cMsg.pcDBData != NULL)delete cMsg.pcDBData;
    }
}
/*lint +esym(1746,tParm)*/

void InitDbixResultCbMsgDesc()
{
    OSP_ADD_EVENT_DESC(DBIX_RESULT_CB);
}
