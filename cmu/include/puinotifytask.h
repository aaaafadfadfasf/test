
#ifndef _PUI_NOTIFY_TASK_H
#define _PUI_NOTIFY_TASK_H

#include <set>
#include <map>
using std::set;
using std::map;

#include "cms/ospext/osptask.h"
#include "cms/ospsip/ospsip.h"
#include "cmuconst.h"
#include "slidingwindowtmpl.h"

class CPuiNotifyList;

class CPuiNotifyTask : public CTask
{
public:
    typedef string ElementIdType;
public:
    enum
    {
        Working = UNKNOWN_STATE + 1
    };

public:
    CPuiNotifyTask(CInstExt* pcInst, const TSipURI &tUri, CPuiNotifyList *pcNotifyList = NULL);
    virtual ~CPuiNotifyTask();

public:
    virtual const char* GetObjName() const { return "CPuiNotifyTask"; }

    virtual const char* GetStateName(u32 dwState) const
    {
        switch (dwState)
        {
        case Working:
            return "Working";
        default:
            break;
        }
        return "Unknown State";
    }

    virtual void InitStateMachine();
    virtual void PrintData() const;
    virtual bool IsNotNeedPollState() { return false; }

    u32 OnWorking(CMessage *const pcMsg);
    u32 OnWorkingTimer();

public:
    void Add(const set<ElementIdType> &cIdSet);
    void Add(const vector<ElementIdType> &cIds);
    bool IsFinished() const;

    bool PuOfflineProc(const vector<ElementIdType> &cIds);

private:
    TSipURI m_tPuiUri;
    CSlidingWindowTask<CPuiNotifyTask> *m_pcSlidingWindowTask; 

    CPuiNotifyList *m_pcNotifyList;
};

class CPuiNotifyList
{
    friend class CPuiNotifyTask;
public:
    CPuiNotifyList() {}

    bool IsEmpty() const { return m_cList.empty(); }
    bool IsExistPuiNotifyTask(const TSipURI &tUri);
    CPuiNotifyTask *GetPuiNotifyTask(const TSipURI &tUri);

    void PrintData() const;
private:
    map<string, CPuiNotifyTask *> m_cList;
};

extern CPuiNotifyList g_cPuiNotifyList;

#endif  //#ifndef _PUI_NOTIFY_TASK_H
