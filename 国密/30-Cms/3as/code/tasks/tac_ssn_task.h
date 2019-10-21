/*
TAC模块服务端会话任务
*/
#ifndef _TAC_SSN_TASK_H_
#define _TAC_SSN_TASK_H_
#include "common/ssn_taskbase.h"

class CTacSsnTask : public CSsnTaskbase
{
public:
    enum
    {
        enStateInit  = UNKNOWN_STATE + 1,
        enStateLoginInProcess,
        enStateLogined
    };
public:
    CTacSsnTask(CInstExt *pcInst);
    ~CTacSsnTask();
public:
    virtual const char* GetObjName() const{return "CTacSsnTask";}
    virtual const char* GetStateName(u32 dwState) const;
    virtual void InitStateMachine();
private:
	bool ChangeNotifyFilter(u16 /*wMsgId*/, TChangeNtfItem &cItem);
    virtual u32 InitStateProcMsg(CMessage *const pcMsg);
    virtual u32 LoginInProcessStateProcMsg(CMessage *const pcMsg);
    virtual u32 LoginedStateProcMsg(CMessage *const pcMsg);
private:
    string m_strUserName;
};

#endif // _TAC_SSN_TASK_H_
