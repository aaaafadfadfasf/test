
#include <algorithm>

#include "cms/cms_errorcode.h"
#include "cms/cmu/cmu_event.h"
#include "cms/cmu/cmu_struct.h"
#include "cms/cmu/cmu_proto.h"

#include "catalogsstask.h"
#include "cmuconst.h"
#include "cmudata.h"
#include "puregtask.h"
#include "domaincatalogssertask.h"

CCatalogSubscribeTask::CCatalogSubscribeTask(CInstExt *pcInst) : CSsBaseTask(pcInst)
{

}

CCatalogSubscribeTask::~CCatalogSubscribeTask()
{
    ReleaseResource();
}

void CCatalogSubscribeTask::ReleaseResource()
{
    if (!m_tSsTypes.empty())
    {
        CCmuData::GetInstance()->DelSipSsTask(GetTaskNO(), m_strDomain, m_tSsTypes);
        m_tSsTypes.clear();
    }

    CLoginSession* ptUserLoginTask = GetLoginSession(GetSession());
    if (ptUserLoginTask != NULL)
    {
        ptUserLoginTask->DelTask(this);
    }
}

void CCatalogSubscribeTask::PrintData() const
{
    CSsBaseTask::PrintData();

    OspPrintf(TRUE, FALSE, "\n  Session[%s] UserUri[%s]  Domain[%s] SsTypes[%s]\n",
        GetSession().c_str(), GetUserUri().c_str(), m_strDomain.c_str(), GetSsTypesStr(m_tSsTypes).c_str());
}

TSsMode CCatalogSubscribeTask::GetSsMode() const
{
	return CATALOG_SS;
}

u32  CCatalogSubscribeTask::ProcSsReq(CMessage * const pcMsg)
{
	CMS_ASSERT(CATALOG_STATUS_SS_REQ == pcMsg->event);

	COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;

	CCatalogStatusSsReq cReq;
	pcOspSipMsg->GetMsgBody(cReq);

	CCatalogStatusSsRsp cRsp;
	cRsp.SetHeadFromReq(cReq);

	SetSession(cReq.GetSession());
	SetUserUri(cReq.GetUserUri());
	SetRcvBatchNtf(cReq.GetBatchNotify());

	do 
	{
		//用户是否登录
		CLoginSession* ptUserLoginTask = GetLoginSession(GetSession());
		if (ptUserLoginTask == NULL)
		{
			TASKLOG(CMU_MOD_SS, CRITICAL_LEV, "用户[%s]登录session[%s]非法\n",
				GetUserUri().c_str(), GetSession().c_str());

			if (IsLocalDomainByDevUri(GetUserUri()))
			{
				cRsp.SetErrorCode(ERR_CMU_CU_LOGIN_SESS_ILLEGAL);
			}
			else
			{
				cRsp.SetErrorCode(ERR_CMU_CMU_NOT_CONN);
			}

			break;
		}

        s32 nRet = CCmuData::GetInstance()->AddSipSsTask(GetTaskNO(), cReq.GetDomain(), cReq.GetSsTypes());
        if (CMS_SUCCESS == nRet)
        {
            m_strDomain = cReq.GetDomain();
            m_tSsTypes  = cReq.GetSsTypes();

            ptUserLoginTask->AddTask(this);
        }
        else
        {
            cRsp.SetErrorCode(nRet);
        }

	} while (false);

	//订阅应答
	PostRsp(KDSIP_EVENT_SUBSCRIBE_RSP, pcOspSipMsg->GetSipTransID(), cRsp);

	if (CMS_SUCCESS == cRsp.GetErrorCode())
	{
		PublishInitialNotify(m_tSsTypes);
	}

	return cRsp.GetErrorCode();
}

void CCatalogSubscribeTask::PublishInitialNotify(const TSsTypes &tSsTypes)
{
    CDomainData *pcDomainData = CCmuData::GetInstance()->GetDomainDataByDomainName(m_strDomain);
    if (NULL != pcDomainData)
    {
        CPuDataMap &cPuList = pcDomainData->GetPuList().GetPuDataMap();
        for (CPuDataMap::iterator it = cPuList.begin(); cPuList.end() != it; ++it)
        {
            it->second.PublishCertainSipSsTypes(tSsTypes, ALL_SS, GetTaskNO(), false);
        }
    }
}

u32  CCatalogSubscribeTask::ProcRefreshReq(CMessage * const pcMsg)
{
	CMS_ASSERT(CATALOG_STATUS_REFRESH_SS_REQ == pcMsg->event);

	COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;

	CCatalogStatusRefreshSsReq cReq;
	pcOspSipMsg->GetMsgBody(cReq);

	CCatalogStatusRefreshSsRsp cRsp;
	cRsp.SetHeadFromReq(cReq);

	const TSsTypes &tSsTypes = cReq.GetSsTypes();

    if (SS_OP_TYPE_ADD == cReq.GetSsOpType())
    {
        s32 nRet = CCmuData::GetInstance()->AddSipSsTask(GetTaskNO(), m_strDomain, tSsTypes);
        if (CMS_SUCCESS == nRet)
        {
            m_tSsTypes.insert(tSsTypes.begin(), tSsTypes.end());
        }
        else
        {
            cRsp.SetErrorCode(nRet);
        }
    }
    else
    {
        CCmuData::GetInstance()->DelSipSsTask(GetTaskNO(), m_strDomain, tSsTypes);

        TSsTypes tDiffSsTypes;
        set_difference(m_tSsTypes.begin(), m_tSsTypes.end(), tSsTypes.begin(), tSsTypes.end(), 
            inserter(tDiffSsTypes, tDiffSsTypes.begin()));
        m_tSsTypes = tDiffSsTypes;
    }

    PostRsp(KDSIP_EVENT_SUBSCRIBE_RSP, pcOspSipMsg->GetSipTransID(), cRsp);

	if (CMS_SUCCESS == cRsp.GetErrorCode() && SS_OP_TYPE_ADD == cReq.GetSsOpType())
	{
		PublishInitialNotify(tSsTypes);
	}

	return cRsp.GetErrorCode();
}

void CCatalogSubscribeTask::ProcUnSsReq(CMessage * const pcMsg)
{
	CMS_ASSERT(CATALOG_STATUS_UNSS_REQ == pcMsg->event);

	COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;

	CCatalogStatusUnSsReq cReq;
	pcOspSipMsg->GetMsgBody(cReq);

	CCatalogStatusUnSsRsp cRsp;
	cRsp.SetHeadFromReq(cReq);	

    CCmuData::GetInstance()->DelSipSsTask(GetTaskNO(), m_strDomain, m_tSsTypes);
    m_tSsTypes.clear();

	PostRsp(KDSIP_EVENT_UNSUBSCRIBE_RSP, pcOspSipMsg->GetSipTransID(), cRsp);
}

void CCatalogSubscribeTask::ProcRemovePu(const string &strDevUri)
{

}

void CCatalogSubscribeTask::ProcRemoveDomain(const string &strDomainName)
{
    if (strDomainName == m_strDomain)
    {
        delete this;
    }
}
