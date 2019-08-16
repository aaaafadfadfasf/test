/*
���ܣ��ṩһ���ص�������ʵ�ְ汾������osp��taskӦ�ó�����
�㷨���ð汾�Ļص������յ�dbix�Ļص��󽫽������Ϣ����ʽͶ�ݸ��ص�����ָ����OspIID��
     �������ϢΪDBIX_RESULT_CB�� ��Ϣ��ΪTDbixResult��
*/

#ifndef _DBRESULT_CALLBACK_IMP4OSPTASK_H_
#define _DBRESULT_CALLBACK_IMP4OSPTASK_H_

#include "cms/ospext/osptask.h"
#include "cms/dbix/dbix.h"
using namespace dbix;
#include "cms/cms_eventid.h"

// ���ݿ��������ص�ʵ��
void cbDbResultHandler(ENDBResult enDBResult, CDbResultDataBase* pcResultData, TCBParm tParm);

// �������ݿ�����Ļص�����
inline TCBParm GenerateDbResultCbParm(u32 dwOspIID, u32 dwTaskId)
{
    TCBParm tCbParm;
    tCbParm.qwParm = dwOspIID;
    tCbParm.qwParm = tCbParm.qwParm<<32;
    tCbParm.qwParm += dwTaskId;
    return tCbParm;
}

/* �ص��¼�����Ϣ�ź���Ϣ�� */
const u16 DBIX_RESULT_CB = DBIX_EVENT_BEGIN; // ��Ϣ�� TDbixResult
struct TDbixResult // dbix���������Ϣ��
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

// ע��ص��¼�����Ϣ����
void InitDbixResultCbMsgDesc();









/* ����Ľӿڲ�����ʹ��, ����Ϊ��ʹ���еĴ���Ķ������ٶ������Ľӿ� */
inline u64 GetOspTransId_u64(u32 dwIID, u32 dwTaskId)
{
    u64 lwOspTransId = dwIID;
    lwOspTransId = lwOspTransId<<32;
    lwOspTransId += dwTaskId;
    return lwOspTransId;
}
typedef TDbixResult CDbixMsg;


#endif // _DBRESULT_CALLBACK_IMP4OSPTASK_H_