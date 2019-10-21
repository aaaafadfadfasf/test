/*
����TAS����˻Ự����
*/

#ifndef _CHILD_TAS_SSN_TASK_H_
#define _CHILD_TAS_SSN_TASK_H_
#include "common/ssn_taskbase.h"

class CSyncDataRecv;

class CChildTasSsnTask : public CSsnTaskbase
{
public:
    enum
    {
        enStateInit = UNKNOWN_STATE + 1,
        enStateLogining,
        enStateLogined
    };
public:
    CChildTasSsnTask(CInstExt *pcInst);
    ~CChildTasSsnTask();
public:
    //������Ҫ��д�ķ���
    virtual const char* GetObjName() const{return "CChildTasSsnTask";}
    virtual const char* GetStateName(u32 dwState) const;
    virtual void InitStateMachine();
private:
    virtual u32 InitStateProcMsg(CMessage *const pcMsg);
    virtual u32 LoginingStateProcMsg(CMessage *const pcMsg);
    virtual u32 LoginedStateProcMsg(CMessage *const pcMsg);

    void EnterInitState();    // �����ʼ��״̬
    void EnterLoginedState(); // �����Ѿ���¼״̬

private:
    TSipURI m_tPeerTasUri; // �Զ�TAS��URI
    CSyncDataRecv *m_pcSyncDataRecvTask; // ͬ�����ݽ�������
};

#endif // _CHILD_TAS_SSN_TASK_H_
