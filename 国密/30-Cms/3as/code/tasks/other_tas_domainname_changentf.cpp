#include "other_tas_domainname_changentf.h"
#include "cctrl.h"
#include "tasks/common/conn_taskbase.h"

void COtherTasDomainnameChangeNtf::InitStateMachine()
{
    NextState(enStateServer);

    CStateProc cProc;
    cProc.TimerPoll = &CTask::TimerPollSkip;

    cProc.ProcMsg = (CTask::PFProcMsg)&COtherTasDomainnameChangeNtf::OnServer;
    cProc.ProcErrMsg = cProc.ProcMsg;
    AddRuleProc(enStateServer, cProc);
}

u32 COtherTasDomainnameChangeNtf::OnServer( CMessage *const pcMsg )
{
    if (pcMsg->event != TAS_DOMAIN_RELATION_MOD_NTF_REQ)
    {
        return PROCMSG_FAIL;
    }

    COspSipMsg* const pcSipMsg = reinterpret_cast<COspSipMsg* const>(pcMsg->content);

    CDomainRelationModNtfReq cReq;
    pcSipMsg->GetMsgBody(cReq);

    CNtfRsp cRsp;
    cRsp.SetEvent(TAS_DOMAIN_RELATION_MOD_NTF_RSP);
    cRsp.SetSeqNum(cReq.GetSeqNum());
    cRsp.SetSession(cReq.GetSession());
    PostMsgRsp(pcSipMsg->GetSipTransID(), cRsp);
    
    // �������ӵ�Uri
    set<CConnTaskbase*> cConnTasks = static_cast<CCtrlInst*>(GetInstance())->GetAppData().GetTasConnTaskSet();
    CConnTaskbase* pcConnTask;
    for (set<CConnTaskbase*>::const_iterator it = cConnTasks.begin(); it != cConnTasks.end(); ++it)
    {
        pcConnTask = *it;
        if (pcConnTask->GetPeerDomainId() == cReq.GetDomainRelation().GetDomainID())
        {
            pcConnTask->SetPeerDomainname(cReq.GetDomainRelation().GetDomainName());
        }
    }

    // hzytodo �������ݿ��е�Ŀ�������ã��˿黹δ��. ����PMS���¼���PMS������ͬ���������̿���û��


    return PROCMSG_DEL;
}
