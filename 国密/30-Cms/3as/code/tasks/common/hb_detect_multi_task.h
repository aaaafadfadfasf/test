/*
���ܣ���������עͬһ��URI��������⡣
Ӧ�ó���������
1. ��Ϊ�ͻ���(�¼�)����ͬʱ�����¼��Ϳ�����Ȩ������ݵ�¼ͬһ��TAS����������������⡣
2. ��Ϊ�����(�ϼ�)����ͬʱ�����¼�TAS�ĵ�¼����ͬһ���¼�TAS����ȫ��ͨ����ͬ��...

������URI��TaskNo��ӳ�䣬Task�����˳�ʱ�����ͷ��������
*/

#ifndef _HB_DETECT_MULTI_TASK_H_
#define _HB_DETECT_MULTI_TASK_H_

#include "inner_common.h"

// ����һ������ʱ���ֶ����ڷ�ֹTASK���ظ�ʱ���µ�����, ע��:ֻ����cctrl app��TASK��ʹ��
bool SetMultiHb(const TSipURI &tDstURI, u32 dwTaskNo, time_t tTaskCreateTm);
bool StopMultiHb(const TSipURI &tDstURI, u32 dwTaskNo, time_t tTaskCreateTm);

class CHbDetectMultiTask: public CSipTask
{
public:
    enum
    {
        enStateServer = UNKNOWN_STATE + 1
    };

    CHbDetectMultiTask(CInstExt *pcInst);
    ~CHbDetectMultiTask();
    virtual void PrintData() const;

    virtual const char* GetObjName() const{return "CHbDetectMultiTask";}
    virtual const char* GetStateName(u32 dwState) const {return "ServerState";}
    virtual void InitStateMachine();

    bool SetMultiHb(const TSipURI &tDstURI, u32 dwTaskNo, time_t tTaskCreateTm);
    bool StopMultiHb(const TSipURI &tDstURI, u32 dwTaskNo, time_t tTaskCreateTm);

private:
    u32 OnServer(CMessage *const pcMsg);

private:
    map<string, list<u32> > m_cPeerUri2TasknoList;
};

#endif // _HB_DETECT_MULTI_TASK_H_
