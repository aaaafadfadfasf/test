#include "hb_detect_multi_task.h"
#include "cctrl.h"

static CHbDetectMultiTask *s_pcHbDetectMultiTask = NULL;

bool SetMultiHb( const TSipURI &tDstURI, u32 dwTaskNo, time_t tTaskCreateTm )
{
    if (NULL != s_pcHbDetectMultiTask)
    {
        return s_pcHbDetectMultiTask->SetMultiHb(tDstURI, dwTaskNo, tTaskCreateTm);
    }
    
    return false;
}

bool StopMultiHb( const TSipURI &tDstURI, u32 dwTaskNo, time_t tTaskCreateTm )
{
    if (NULL != s_pcHbDetectMultiTask)
    {
        return s_pcHbDetectMultiTask->StopMultiHb(tDstURI, dwTaskNo, tTaskCreateTm);
    }
    
    return false;
}


bool CHbDetectMultiTask::SetMultiHb( const TSipURI &tDstURI, u32 dwTaskNo, time_t tTaskCreateTm )
{
    string strDstUri = tDstURI.GetURIString();
    Log(CRITICAL_LEV, "SetMultiHb, the DstUri:%s\n", strDstUri.c_str());

    map<string, list<u32> >::iterator it = m_cPeerUri2TasknoList.find(strDstUri);

    if (m_cPeerUri2TasknoList.end() == it)
    {
        list<u32> lst;
        lst.push_back(dwTaskNo);
        if (SetHBParam(tDstURI))
        {
            m_cPeerUri2TasknoList.insert(map<string, list<u32> >::value_type(strDstUri, lst));
            return true;
        }
        else
        {
            Log(ERROR_LEV, "SetHBParam failed in CHbDetectMultiTask::SetMultiHb\n");
            return false;
        }
    }
    else
    {
        it->second.push_back(dwTaskNo);
        return true;
    }
}

bool CHbDetectMultiTask::StopMultiHb( const TSipURI &tDstURI, u32 dwTaskNo, time_t tTaskCreateTm )
{
    string strDstUri = tDstURI.GetURIString();
    Log(CRITICAL_LEV, "StopMultiHb, the DstUri:%s\n", strDstUri.c_str());

    map<string, list<u32> >::iterator it = m_cPeerUri2TasknoList.find(strDstUri);

    if (m_cPeerUri2TasknoList.end() != it)
    {
        it->second.remove(dwTaskNo);
        if (it->second.size() == 0)
        {
            m_cPeerUri2TasknoList.erase(it);
            return StopHB(tDstURI);
        }
        
        return true;
    }
    else
    {
        return false;
    }
}

CHbDetectMultiTask::CHbDetectMultiTask( CInstExt *pcInst ): CSipTask(pcInst)
{
    CMS_ASSERT(s_pcHbDetectMultiTask == NULL);
    s_pcHbDetectMultiTask = this;
}

CHbDetectMultiTask::~CHbDetectMultiTask()
{
    s_pcHbDetectMultiTask = NULL;
}

void CHbDetectMultiTask::InitStateMachine()
{
    NextState(enStateServer);

    CStateProc cProc;
    cProc.TimerPoll = &CTask::TimerPollSkip;

    cProc.ProcMsg = (CTask::PFProcMsg)&CHbDetectMultiTask::OnServer;
    cProc.ProcErrMsg = cProc.ProcMsg;
    AddRuleProc(enStateServer, cProc);
}

u32 CHbDetectMultiTask::OnServer( CMessage *const pcMsg )
{
    if (pcMsg->event == OSP_SIP_DISCONNECT)
    {
        COspSipMsg *pcSipMsg = reinterpret_cast<COspSipMsg*>(pcMsg->content);
        string strPeerUri = pcSipMsg->GetMsgBody();
        Log(CRITICAL_LEV, "detected Disconnection in CHbDetectMultiTask, the PeerUri:%s\n", strPeerUri.c_str());

        map<string, list<u32> >::iterator it = m_cPeerUri2TasknoList.find(strPeerUri);
        if (it != m_cPeerUri2TasknoList.end())
        {
            // 此处不要使用引用，因为相应任务析构时停止心跳检测会m_cPeerUri2TasknoList中的value部分
            list<u32> tasknoList = it->second;

            for (list<u32>::iterator it2 = tasknoList.begin(); it2 != tasknoList.end(); ++it2)
            {
                CTask *pcTask = GetInstance()->FindTask(*it2);
                u32 dwProcRet = pcTask->ProcMsg(pcMsg);

                if (PROCMSG_DEL == dwProcRet)
                {
                    delete pcTask;
                }
            }

            // 重新查找，因为前面删除任务的过程中可能影响m_cPeerUri2TasknoList中的key部分
            it = m_cPeerUri2TasknoList.find(strPeerUri);
            if (it != m_cPeerUri2TasknoList.end())
                m_cPeerUri2TasknoList.erase(it);
        }
    }

    return PROCMSG_OK;
}

void CHbDetectMultiTask::PrintData() const
{
    for (map<string, list<u32> >::const_iterator it = m_cPeerUri2TasknoList.begin(); it != m_cPeerUri2TasknoList.end(); ++it)
    {
        TelePrint("DstUri:%s's taskno list:", it->first.c_str());
        const list<u32> &tasklist = it->second;
        for (list<u32>::const_iterator it2 = tasklist.begin(); it2 != tasklist.end(); ++it2)
        {
            OspPrintf(TRUE, FALSE, "%u ", *it2);
        }
        TelePrint("");
    }
}

