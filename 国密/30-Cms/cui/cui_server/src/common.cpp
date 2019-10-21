#include "common.h"
#include "cms/caps/caps_event.h"

CCallBackTask::CCallBackTask(CInstExt *pcInst, const string &strCuSession, const string &strPeUri) : CSipTask(pcInst)
{
    //µÇÂ¼Ê§°ÜÊ±sessionÎª¿Õ
    //CMS_ASSERT(!strCuSession.empty());
    CMS_ASSERT(!strPeUri.empty());

    m_tPeUri.SetNOString(strPeUri);
    m_strCuSession = strCuSession;
}

CCallBackTask::~CCallBackTask()
{
    //    CPeAgent *pcPeAgent = (CPeAgent *)g_cCuiDataMgr.GetPeAgentPtrByPeName(m_tPeUri.GetURIString(), GetInstance());
    //    if(NULL != pcPeAgent)
    //    {
    //        if(!pcPeAgent->m_tCallBackTaskList.empty())
    //        {
    //            for ( TCallBackTaskList::const_iterator pTask = pcPeAgent->m_tCallBackTaskList.begin();pTask != pcPeAgent->m_tCallBackTaskList.end(); pTask++ )
    //            {
    //                if(*pTask == GetTaskNO())
    //                {
    //                    pcPeAgent->m_tCallBackTaskList.erase(pTask);
    //                }
    //            }
    //        }
    //    }
}

bool IsNormalNotOpenPeEevent( u16 wEvent )
{
	if (wEvent>CAPS_EVENT_BEGIN&&wEvent<CAPS_CURRENT_EVENT_END)
	{
		return true;
	}

	return false;
}
