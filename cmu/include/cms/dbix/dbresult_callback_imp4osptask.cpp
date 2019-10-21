/* ���ļ�����DBIX�����ģ�Ϊ�˷����ϲ�ʹ�ö���ӵ�. ���������ϲ�ʹ���߱������ӵġ�
   Ϊ�˽����˾linux common make�ű�Դ�ļ��޷���Ŀ¼�����⣬���԰�����dbix������� */

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

    GLOBELOG(EVENT_LEV, "dbix�ص�����Ŀ������OspIID[%u], TaskId[%u]\n", dwOspIID, dwOspTaskId);

    if (0 != ::OspPost(dwOspIID, DBIX_RESULT_CB, &cMsg, sizeof(cMsg)))
    {
        GLOBELOG(EVENT_LEV, "dbix�ص�����Ŀ������OspIID[%u], TaskId[%u], OspPostͶ��ʧ��\n", dwOspIID, dwOspTaskId);

        if (cMsg.pcDBData != NULL)delete cMsg.pcDBData;
    }
}
/*lint +esym(1746,tParm)*/

void InitDbixResultCbMsgDesc()
{
    OSP_ADD_EVENT_DESC(DBIX_RESULT_CB);
}
