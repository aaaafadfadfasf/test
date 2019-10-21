/*
    ���������NtfReq�Ĳ�ѯ����, �������ھ���ĻỰTask����.
    ���ݿ��ѯ��Ϊ���ȷ�֪ͨ�����Ӧ��
*/

#ifndef _QUERY_TASK_H_
#define _QUERY_TASK_H_
#include "inner_common.h"
class CSsnTaskbase;

class CQueryTask: public CSipTask
{
public:
    enum
    {
        enStateIdle  = UNKNOWN_STATE + 1,
        enStateQuery,    // ��ѯ״̬�����ݿ�Ľ����δ���ع���
        enStateTransRsp, // ���ݴ����״̬(����Rsp�׶�)
        enStateTransNtf, // ���ݴ����״̬(����Ntf�׶�)
    };

    CQueryTask(CSsnTaskbase* pcSsnTask);
    ~CQueryTask();
    virtual void PrintData() const;

public:
    virtual const char* GetObjName() const{return m_strObjName.c_str();}
    virtual const char* GetStateName(u32 dwState) const;
    virtual void InitStateMachine();
private:
    u32 OnIdle(CMessage *const pcMsg);
    u32 OnQuery(CMessage *const pcMsg);
    u32 QueryTimerPoll();
    u32 OnTransRsp(CMessage *const pcMsg);
    u32 TransRspTimerPoll();
    u32 OnTransNtf(CMessage *const pcMsg);
    u32 TransNtfTimerPoll();
    bool Query(CMessage *const pcMsg);
    bool DoTransfer(bool bRsp);

    void DoNextState(u32 enState);
private:
    list<const CTaskMsgTransferBase*> m_cQryNtfLst; // ��ѯ֪ͨ�б�������Ҫ˳���ͳ�ȥ
    CTaskMsgTransferBase* m_pcQryRsp; // ��ѯӦ��
    string m_strQueryTag;    // ��ѯ��ǩ(���ڱ�ʶ��ѯ֪ͨ�����Ĳ�ѯ)
    string m_strQueryReq;    // ��ѯ����
    TSipURI m_tSipUri;    // Ntf���͸���UA��ʶ
    CSsnTaskbase *m_pcSsnTask; // �����ĻỰ����
    string m_strObjName; // ��������(���ϲ�ѯ�¼�����)����Ҫ���ڴ�ӡ
};

#endif // _QUERY_TASK_H_
