
#include <algorithm>

#include "cms/cms_errorcode.h"
#include "cms/cmu/cmu_event.h"
#include "cms/cmu/cmu_struct.h"
#include "cms/cmu/cmu_proto.h"

#include "pulistsstask.h"
#include "domainpulistssertask.h"
#include "cmuconst.h"
#include "cmudata.h"
#include "culogintask.h"
#include "puregtask.h"

CPuListSubscribeTask::CPuListSubscribeTask(CInstExt *pcInst) : CSsBaseTask(pcInst)
{
    
}

CPuListSubscribeTask::~CPuListSubscribeTask()
{
    ReleaseResource();
}


void CPuListSubscribeTask::ReleaseResource()
{
    if (!m_tSsDevList.empty())
    {
        CCmuData::GetInstance()->DelSipSsTask(GetTaskNO(), m_tSsDevList);
        m_tSsDevList.clear();
    }

    CLoginSession* ptUserLoginTask = GetLoginSession(GetSession());
    if (ptUserLoginTask != NULL)
    {
        ptUserLoginTask->DelTask(this);
    }
}

void CPuListSubscribeTask::PrintData() const
{
    CSsBaseTask::PrintData();

    OspPrintf(TRUE, FALSE, "\n  Session[%s]  UserUri[%s]\n", GetSession().c_str(), GetUserUri().c_str());

    // �������б�������з��࣬�Ա��ӡ
    map<string, TSsDevList> cDomainSsDevList;
    for (TSsDevList::const_iterator it = m_tSsDevList.begin(); m_tSsDevList.end() != it; ++it)
    {
        TSipURI tDevUri;
        tDevUri.SetURIString(it->first);

        cDomainSsDevList[tDevUri.GetDomain()].insert(TSsDevList::value_type(it->first, it->second));
    }
    
	if (g_bDebugOn && !cDomainSsDevList.empty())
	{
		for (map<string, TSsDevList>::const_iterator it = cDomainSsDevList.begin(); cDomainSsDevList.end() != it; ++it)
		{
			OspPrintf(TRUE, FALSE, "  ----------------------------Domain[%s]----------------------------\n", it->first.c_str());

			const TSsDevList &tSsList = it->second;
			for (map<string, TSsTypes>::const_iterator it4Dev = tSsList.begin(); tSsList.end() != it4Dev; ++it4Dev)
			{
				OspPrintf(TRUE, FALSE, "    DevUri[%s] SsTypes[%s]\n", it4Dev->first.c_str(), GetSsTypesStr(it4Dev->second).c_str());
			}

			OspPrintf(TRUE, FALSE, "  Domain[%s]�е��豸����[%u]\n", it->first.c_str(), tSsList.size());
		}
	}
}

TSsMode CPuListSubscribeTask::GetSsMode() const
{
	return LIST_SS;
}

u32 CPuListSubscribeTask::ProcSsReq(CMessage * const pcMsg)
{
	CMS_ASSERT(PULIST_STATUS_SS_REQ == pcMsg->event);

	COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;

	CPuListStatusSsReq cReq;
	pcOspSipMsg->GetMsgBody(cReq);

	CPuListStatusSsRsp cRsp;
	cRsp.SetHeadFromReq(cReq);

	SetSession(cReq.GetSession());
	SetUserUri(cReq.GetUserUri());
	SetRcvBatchNtf(cReq.GetBatchNotify());

	TASKLOG(CMU_MOD_SS, EVENT_LEV, "�û�[%s]������Ϣ\n", GetUserUri().c_str());

	map<string, TSsDevList> cDevGroupMap;

	do 
	{
		//�û��Ƿ��¼
		CLoginSession* ptUserLoginTask = GetLoginSession(GetSession());
		if (ptUserLoginTask == NULL)
		{
			TASKLOG(CMU_MOD_SS, CRITICAL_LEV, "�û�[%s]��¼session[%s]�Ƿ�\n",
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

        TSsDevList   tSsSuccessList;
        TSsErrorList tSsErrorList;
        CCmuData::GetInstance()->AddSipSsTask(GetTaskNO(), cReq.GetSsDevList(), tSsSuccessList, tSsErrorList);

        m_tSsDevList = tSsSuccessList;
        ptUserLoginTask->AddTask(this);

        cRsp.SetSsErrorList(tSsErrorList);

        TASKLOG(CMU_MOD_SS, CRITICAL_LEV, "�б��Ĵ����������������[%u]���ɹ�����[%u]��ʧ������[%u]\n",
            cReq.GetSsDevList().size(), tSsSuccessList.size(), tSsErrorList.size());

	} while (false);

	// ���ĳɹ����ͳ�ʼ��֪ͨ
	PostRsp(KDSIP_EVENT_SUBSCRIBE_RSP, pcOspSipMsg->GetSipTransID(), cRsp);

	if (CMS_SUCCESS == cRsp.GetErrorCode())
	{
		PublishInitialNotify(m_tSsDevList);
	}

	return cRsp.GetErrorCode();
}

void CPuListSubscribeTask::PublishInitialNotify(const TSsDevList &tSsDevList)
{
    for (TSsDevList::const_iterator it = tSsDevList.begin(); tSsDevList.end() != it; ++it)
	{
        CPuData *pcPuData = CCmuData::GetInstance()->GetPuDataByUri(it->first);
        if (NULL != pcPuData)
        {
            pcPuData->PublishCertainSipSsTypes(it->second, ALL_SS, GetTaskNO(), false);
        }
	}
}

u32 CPuListSubscribeTask::ProcRefreshReq(CMessage * const pcMsg)
{
	CMS_ASSERT(PULIST_STATUS_REFRESH_SS_REQ == pcMsg->event);

	COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;

	CPuListStatusRefreshSsReq cReq;
	pcOspSipMsg->GetMsgBody(cReq);

	CPuListStatusRefreshSsRsp cRsp;
	cRsp.SetHeadFromReq(cReq);

    if (SS_OP_TYPE_ADD == cReq.GetSsOpType())
    {
        TSsDevList   tSsSuccessList;
        TSsErrorList tSsErrorList;
        CCmuData::GetInstance()->AddSipSsTask(GetTaskNO(), cReq.GetSsDevList(), tSsSuccessList, tSsErrorList);
        
        // ��ˢ�¶��ĳɹ��б�ϲ���ԭ�����б���
        for (TSsDevList::const_iterator it = tSsSuccessList.begin(); tSsSuccessList.end() != it; ++it)
        {
            m_tSsDevList[it->first].insert(it->second.begin(), it->second.end());
        }

        cRsp.SetSsErrorList(tSsErrorList);
    }
    else
    {
        CCmuData::GetInstance()->DelSipSsTask(GetTaskNO(), cReq.GetSsDevList());

        for (TSsDevList::const_iterator it = cReq.GetSsDevList().begin(); cReq.GetSsDevList().end() != it; ++it)
        {
            TSsDevList::iterator it4Dev = m_tSsDevList.find(it->first);
            if (m_tSsDevList.end() != it4Dev)
            {
                TSsTypes tDiffSsTypes;
                set_difference(it4Dev->second.begin(), it4Dev->second.end(), it->second.begin(), it->second.end(), 
                    inserter(tDiffSsTypes, tDiffSsTypes.begin()));
                it4Dev->second = tDiffSsTypes;
                if (it4Dev->second.empty())
                {
                    m_tSsDevList.erase(it4Dev);
                }
            }
        }
    }

    // Ӧ��
    PostRsp(KDSIP_EVENT_SUBSCRIBE_RSP, pcOspSipMsg->GetSipTransID(), cRsp);

	if (CMS_SUCCESS == cRsp.GetErrorCode() && SS_OP_TYPE_ADD == cReq.GetSsOpType())
	{
		PublishInitialNotify(cReq.GetSsDevList());
	}

	return cRsp.GetErrorCode();
}

void CPuListSubscribeTask::ProcUnSsReq(CMessage * const pcMsg)
{
	CMS_ASSERT(PULIST_STATUS_UNSS_REQ == pcMsg->event);

	COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;

	CPuListStatusUnSsReq cReq;
	pcOspSipMsg->GetMsgBody(cReq);

	CPuListStatusUnSsRsp cRsp;
	cRsp.SetHeadFromReq(cReq);

    CCmuData::GetInstance()->DelSipSsTask(GetTaskNO(), m_tSsDevList);
    m_tSsDevList.clear();

	PostRsp(KDSIP_EVENT_UNSUBSCRIBE_RSP, pcOspSipMsg->GetSipTransID(), cRsp);
}

void CPuListSubscribeTask::ProcRemovePu(const string &strDevUri)
{
	// �����б��ģ���ĳ��DevData��ɾ��ʱ��ֻҪ�����豸�Ӷ����б�ȥ�����ɣ������˳������������߿��Լ����ڸö��ĻỰ�ڴ��������豸
    m_tSsDevList.erase(strDevUri);
}

void CPuListSubscribeTask::ProcRemoveDomain(const string &strDomainName)
{
    // CPuListSubscribeTaskֻ��Ҫ����ProcRemovePu����
}


