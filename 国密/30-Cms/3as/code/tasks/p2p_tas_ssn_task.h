/*
��Ե�ͬ������˻Ự����
*/
#ifndef _P2P_TAS_SSN_TASK_H_
#define _P2P_TAS_SSN_TASK_H_
#include "common/ssn_taskbase.h"

class CSyncDataRecv;

class CP2PTasSsnTask : public CSsnTaskbase
{
public:
    enum
    {
        enStateInit = UNKNOWN_STATE + 1,
        enStateServer
    };
public:
    CP2PTasSsnTask(CInstExt *pcInst);
    ~CP2PTasSsnTask();
public:
    //������Ҫ��д�ķ���
    virtual const char* GetObjName() const{return "CP2PTasSsnTask";}
    virtual const char* GetStateName(u32 dwState) const;
    virtual void InitStateMachine();
private:
    virtual u32 InitStateProcMsg(CMessage *const pcMsg);
    virtual u32 ServerStateProcMsg(CMessage *const pcMsg);

private:

    CSyncDataRecv *m_pcSyncDataRecvTask; // ͬ�����ݽ�������
};

#endif // _P2P_TAS_SSN_TASK_H_
