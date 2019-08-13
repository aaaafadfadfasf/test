
#include "cms/cms_errorcode.h"
#include "cms/cmu/cmu_event.h"
#include "cms/cmu/cmu_struct.h"
#include "cms/cmu/cmu_proto.h"

#include "domainpulistssertask.h"
#include "cmuconnecttask.h"
#include "pulistsstask.h"
#include "cmuconst.h"
#include "cmudata.h"
#include "cmucoreapp.h"

CDomainPuListSserTask::CDomainPuListSserTask(CInstExt *pcInst, CCmuConnecterTask * const pcConnectTask) : CDomainSsBaseTaskTmpl<string>(pcInst, pcConnectTask)
{

}

CDomainPuListSserTask::~CDomainPuListSserTask()
{
    ReleaseResource();

    CCmuConnecterTask *pcConnectTask = dynamic_cast<CCmuConnecterTask *>(GetConnectTask());
    if (NULL != pcConnectTask)
    {
        pcConnectTask->ExitDomainPuListSserTask(this);
    }
}


void CDomainPuListSserTask::ReleaseResource()
{

}

void CDomainPuListSserTask::PrintData() const
{
	CDomainSsBaseTaskTmpl<string>::PrintData();
}

void CDomainPuListSserTask::TUnitList2TSsList(const set<CSsUnit<string> > &tUnitList, map<string, TSsTypes> &tSsList)
{
    for (set<CSsUnit<string> >::const_iterator it = tUnitList.begin(); tUnitList.end() != it; ++it)
    {
        tSsList[it->GetObject()].insert(it->GetSsType());
    }
}

string  CDomainPuListSserTask::GetSsReqBySsOpInfo(const TSsOpInfo<string> &tOpInfo)
{
	const CCmuConnecterTask *pcConnectTask = (CCmuConnecterTask *)GetConnectTask();
	CMS_ASSERT(NULL != pcConnectTask);

	CPuListStatusSsReq cReq;
	cReq.SetSession(GetLocalUri().GetURIString());
	cReq.SetUserUri(GetLocalUri().GetURIString());
	cReq.SetBatchNotify(true);
	TUnitList2TSsList(tOpInfo.cSsList, cReq.GetSsDevList());

	return cReq.ToXml();
}

string  CDomainPuListSserTask::GetRefreshSsReqBySsOpInfo(const TSsOpInfo<string> &tOpInfo)
{
	const CCmuConnecterTask *pcConnectTask = (CCmuConnecterTask *)GetConnectTask();
	CMS_ASSERT(NULL != pcConnectTask);

	CPuListStatusRefreshSsReq cReq;
	cReq.SetSession(GetLocalUri().GetURIString());
	cReq.SetSsOpType(tOpInfo.tSsOpType);
	TUnitList2TSsList(tOpInfo.cSsList, cReq.GetSsDevList());

	return cReq.ToXml();
}

void CDomainPuListSserTask::ProcSsRsp(const TSsOpInfo<string> &tOpInfo, const CMessage *const pcMsg, set<CSsUnit<string> > &cErrorList)
{
	COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;

	CPuListStatusSsRsp cRsp;
	pcOspSipMsg->GetMsgBody(cRsp);

	DealSsOpRsp(tOpInfo, cRsp.GetSsErrorList(), cErrorList);
}

void CDomainPuListSserTask::ProcRefreshSsRsp(const TSsOpInfo<string> &tOpInfo, const CMessage *const pcMsg, set<CSsUnit<string> > &cErrorList)
{
	COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;

	CPuListStatusRefreshSsRsp cRsp;
	pcOspSipMsg->GetMsgBody(cRsp);

	DealSsOpRsp(tOpInfo, cRsp.GetSsErrorList(), cErrorList);
}

void CDomainPuListSserTask::DealSsOpRsp(const TSsOpInfo<string> &tOpInfo, const TSsErrorList &tErrorList, set<CSsUnit<string> > &cErrorList)
{
    const set<CSsUnit<string> > &cSsList = tOpInfo.cSsList;
	for (set<CSsUnit<string> >::const_iterator it = cSsList.begin(); cSsList.end() != it; ++it)
	{
		if (tErrorList.end() != tErrorList.find(it->GetObject()))
        {
            cErrorList.insert(*it);
        }
	}
}

