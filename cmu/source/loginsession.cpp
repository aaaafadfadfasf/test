
#include "loginsession.h"
#include "cms/commonconfig/cmcfg.h"
#include "cmuconst.h"

CLoginSession::CLoginSession(CInstExt *pcInst) : CSipTask(pcInst)
{
    m_nCurDirectTransNum = 0;
    m_bIsNeedPunch = true;
}

CLoginSession::~CLoginSession()
{
}

void CLoginSession::PrintData() const
{
    CTask::PrintData();

    OspPrintf(TRUE, FALSE, "\n  login_dev[%s-type(%s)] - login_server[%s] - login_session[%s] - is_punch[%d]\n",
        m_strDevUri.c_str(), m_strDevType.c_str(),
        m_strServerUri.c_str(), m_strSession.c_str(), m_bIsNeedPunch);

    OspPrintf(TRUE, FALSE, "  dev addr list:\n");

    for (size_t i=0; i<m_tDevAddrList.size(); i++)
    {
        const TNetAddr& tAddr = m_tDevAddrList.at(i);
        OspPrintf(TRUE, FALSE, "    %u. %s:%d\n",i+1, tAddr.GetNetIp().c_str(), tAddr.GetNetPort());
    }

    OspPrintf(TRUE, FALSE, "  server addr list:\n");

    for (size_t i=0; i<m_tServerAddrList.size(); i++)
    {
        const TNetAddr& tAddr = m_tServerAddrList.at(i);
        OspPrintf(TRUE, FALSE, "    %u. %s:%d\n",i+1, tAddr.GetNetIp().c_str(), tAddr.GetNetPort());
    }

    OspPrintf(TRUE, FALSE, "\n");

    OspPrintf(TRUE, FALSE, "  son task list:\n");
    PrintChildTasks();
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
            //20130606 huangzhichun: 设备下线前先通知到子任务，便于子任务处理下线逻辑
            TDevOnOffCBMsg tDevOnOffCBMsg;
            tDevOnOffCBMsg.SetDevType(this->GetDevType());
            tDevOnOffCBMsg.SetDevUri(this->GetSession());

            CMessage cOspMsg;
            cOspMsg.event = CMU_DEV_OFFLINE_NTF;
            cOspMsg.content = (u8*)&tDevOnOffCBMsg;
            cOspMsg.length = sizeof(tDevOnOffCBMsg);
            pTask->ProcMsg(&cOspMsg);

            delete pTask;
            pTask = NULL;
        }
    }

    m_tTaskList.Empty();
}

bool CLoginSession::IterateChildTasks(Iterator& rNextPosition, CTask*& pTask) const
{
    return m_tTaskList.Iterate(rNextPosition, pTask);
}

void CLoginSession::CallbackToChildTask(CMessage *const pcMsg)
{
    CTask* pcTask = NULL;
    Iterator pPos;
    while (!pPos.End())
    {
        if (IterateChildTasks(pPos, pcTask) && pcTask != NULL)
        {
            if (pcTask->ProcMsg(pcMsg) == PROCMSG_DEL)
            {
                delete pcTask;
                pcTask = NULL;
            }
        }
    }
}

void CLoginSession::CallbackToChildTask(u16 event, const void* content, u16 length)
{
    CMessage cOspMsg;
    cOspMsg.event = event;
    cOspMsg.content = (u8*)content;
    cOspMsg.length = length;

    CallbackToChildTask(&cOspMsg);
}

set<u32> CLoginSession::GetChildTaskNO() const
{
    set<u32> cTasks;

    Iterator pPos;
    while(!pPos.End())
    {
        CTask* pcTask = NULL;
        if (m_tTaskList.Iterate(pPos, pcTask) && NULL != pcTask)
        {
            cTasks.insert(pcTask->GetTaskNO());
        }
    }

	return cTasks;
}

void CLoginSession::PrintChildTasks() const
{
    Iterator pPos;
    size_t i = 0;
    while(!pPos.End())
    {
        CTask* pTask = NULL;
        if (m_tTaskList.Iterate(pPos, pTask))
        {
            if (pTask != NULL)
            {
                i++;
                OspPrintf(TRUE, FALSE, "【%u】--- ", i);
                pTask->CTask::PrintData();
                OspPrintf(TRUE, FALSE, "\n");                
            }
        }
    }
}

TDeviceLogInfo CLoginSession::GetDevLogInfo(bool bOnOffState)
{
    TDeviceLogInfo tLogInfo;

    tLogInfo.devUri = GetDevUri();

    tLogInfo.devName = GetDevType();
    tLogInfo.devAlias = GetDevType();

    tLogInfo.happenedTime = CCMSTime().ToString();
    tLogInfo.logType = (bOnOffState ? enDeviceLogTypeOnline : enDeviceLogTypeOffline);

    if (GetDevAddrList().size() > 0)
    {
        //取第一个地址
        tLogInfo.netAddr = GetDevAddrList()[0].GetNetIp();
    } 
    else
    {
        tLogInfo.netAddr = "127.0.0.1";
    }

    if (bOnOffState)
    {
        tLogInfo.logDesc = GetLanDesc(LAN_DESC_ONLINE);
    }
    else
    {
        tLogInfo.logDesc = GetLanDesc(LAN_DESC_OFFLINE);
    }

    return tLogInfo;
}