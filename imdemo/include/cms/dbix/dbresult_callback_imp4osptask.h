/*
功能：提供一个回调函数的实现版本，用于osp与task应用场景。
算法：该版本的回调函数收到dbix的回调后将结果以消息的形式投递给回调参数指定的OspIID；
     具体的消息为DBIX_RESULT_CB， 消息体为TDbixResult。
*/

#ifndef _DBRESULT_CALLBACK_IMP4OSPTASK_H_
#define _DBRESULT_CALLBACK_IMP4OSPTASK_H_

#include "cms/ospext/osptask.h"
#include "cms/dbix/dbix.h"
using namespace dbix;
#include "cms/cms_eventid.h"

// 数据库操作结果回调实现
void cbDbResultHandler(ENDBResult enDBResult, CDbResultDataBase* pcResultData, TCBParm tParm);

// 产生数据库操作的回调参数
inline TCBParm GenerateDbResultCbParm(u32 dwOspIID, u32 dwTaskId)
{
    TCBParm tCbParm;
    tCbParm.qwParm = dwOspIID;
    tCbParm.qwParm = tCbParm.qwParm<<32;
    tCbParm.qwParm += dwTaskId;
    return tCbParm;
}

/* 回调事件的消息号和消息体 */
const u16 DBIX_RESULT_CB = DBIX_EVENT_BEGIN; // 消息体 TDbixResult
struct TDbixResult // dbix操作结果消息体
{
    TDbixResult()
    {
        dwTaskId = INVALID_TASKNO;
        enDBResult = enDBResultFail;
        pcDBData = NULL;
    }

    u32 GetOspTaskNO() const
    {
        return dwTaskId;
    }
    void SetOspTaskNO(u32 dwTaskId_)
    {
        dwTaskId = dwTaskId_;
    }

    ENDBResult GetDBResult() const
    {
        return enDBResult;
    }
    void SetDBResult(ENDBResult enDBResult_)
    {
        enDBResult = enDBResult_;
    }

    CDbResultDataBase* GetDBData()
    {
        return pcDBData;
    }
    void SetDBData(CDbResultDataBase* pcDBData_)
    {
        pcDBData = pcDBData_;
    }

    u32               dwTaskId;
    ENDBResult        enDBResult;
    CDbResultDataBase*  pcDBData;    
};

// 注册回调事件的消息描述
void InitDbixResultCbMsgDesc();









/* 下面的接口不建议使用, 它们为了使现有的代码改动尽量少而保留的接口 */
inline u64 GetOspTransId_u64(u32 dwIID, u32 dwTaskId)
{
    u64 lwOspTransId = dwIID;
    lwOspTransId = lwOspTransId<<32;
    lwOspTransId += dwTaskId;
    return lwOspTransId;
}
typedef TDbixResult CDbixMsg;


#endif // _DBRESULT_CALLBACK_IMP4OSPTASK_H_