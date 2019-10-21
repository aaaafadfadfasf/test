/*
    ָ���û�ָ���豸���µݹ��ѯ������ƵԴ����Rsp��Ntf����֧�ַ�ҳ��
*/

#ifndef _DEVGRP_VIDSRC_TOTALNUM_GET_TASK_H_
#define _DEVGRP_VIDSRC_TOTALNUM_GET_TASK_H_
#include "inner_common.h"
class CSsnTaskbase;

class CDevGrpVidsrcTotalnumGetTask: public CSipTask
{
public:
    enum
    {
        enStateIdle  = UNKNOWN_STATE + 1,
		enStateGet,
    };

    CDevGrpVidsrcTotalnumGetTask(CSsnTaskbase* pcSsnTask);
    ~CDevGrpVidsrcTotalnumGetTask();
public:
    virtual const char* GetObjName() const{return m_strObjName.c_str();}
    virtual const char* GetStateName(u32 dwState) const;
    virtual void InitStateMachine();
private:
    u32 OnIdle(CMessage *const pcMsg);
    u32 OnGet(CMessage *const pcMsg);
    u32 OnGetTimerPoll();
    bool Get(CMessage *const pcMsg);
    void DoNextState(u32 enState);

private:
    CSsnTaskbase *m_pcSsnTask; // �����ĻỰ����
    string m_strObjName; // ��������(���ϲ�ѯ�¼�����)����Ҫ���ڴ�ӡ
};

#endif // _DEVGRP_VIDSRC_TOTALNUM_GET_TASK_H_
