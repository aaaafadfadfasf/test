
#include "loginsession.h"

CLoginSession::CLoginSession(CInstExt *pcInst) : CSipTask(pcInst)
{
}

CLoginSession::~CLoginSession()
{
}

void CLoginSession::AddTask(CTask* pTask)
{
    if (pTask == NULL)
    {
        TASKLOG(ERROR_LEV, "pTask == NULL");
        return;
    }

    m_tTaskList.Insert(pTask);
}

void CLoginSession::DelTask(CTask* pTask)
{
    if (pTask == NULL)
    {
        TASKLOG(ERROR_LEV, "pTask == NULL");
        return;
    }

    m_tTaskList.Erase(pTask);
}

void CLoginSession::DestroyChildTasks()
{
    Iterator pPos;
    CTask* pTask = NULL;
    while(!pPos.End())
    {
        if (m_tTaskList.Iterate(pPos, pTask) && pTask != NULL)
        {
            delete pTask;
            pTask = NULL;
        }
    }
}

bool CLoginSession::IterateChildTasks(Iterator& rNextPosition, CTask*& pTask) const
{
    return m_tTaskList.Iterate(rNextPosition, pTask);
}