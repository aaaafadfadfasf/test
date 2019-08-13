
#include <algorithm>

#include "cms/cms_errorcode.h"
#include "cms/cmu/cmu_event.h"
#include "cms/cmu/cmu_proto.h"

#include "pusstask.h"
#include "cmuconst.h"
#include "cmudata.h"
#include "culogintask.h"
#include "puregtask.h"

CPuSubscribeTask::CPuSubscribeTask(CInstExt *pcInst) : CSsBaseTask(pcInst)
{

}

CPuSubscribeTask::~CPuSubscribeTask()
{
    ReleaseResource();
}

void CPuSubscribeTask::ReleaseResource()
{
    if (!m_tSsTypes.empty())
    {
        TSsDevList tSsDevList;
        tSsDevList.insert(TSsDevList::value_type(m_strDevUri, m_tSsTypes));

        CCmuData::GetInstance()->DelSipSsTask(GetTaskNO(), tSsDevList);
        m_tSsTypes.clear();
    }

    CLoginSession* ptUserLoginTask = GetLoginSession(GetSession());
    if (ptUserLoginTask != NULL)
    {
        // CPuSubscribeTask�ڶ���ʧ������ʱҲ���ߵ�������������DelTask���ᵼ���κ����⣬����ֱ�Ӽ򵥵Ĵ����¼���
        ptUserLoginTask->DelTask(this);
    }
}

void CPuSubscribeTask::PrintData() const
{
    CSsBaseTask::PrintData();

    OspPrintf(TRUE, FALSE, "\n  Session[%s] UserUri[%s] DevUri[%s] SsTypes[%s]\n",
        GetSession().c_str(), GetUserUri().c_str(), m_strDevUri.c_str(), GetSsTypesStr(m_tSsTypes).c_str());
}

TSsMode CPuSubscribeTask::GetSsMode() const
{
	return SINGLE_SS;
}

u32 CPuSubscribeTask::ProcSsReq(CMessage * const pcMsg)
{
	CMS_ASSERT(PU_STATUS_SS_REQ == pcMsg->event);

	COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;

	CPuStatusSsReq cReq;
	pcOspSipMsg->GetMsgBody(cReq);

	SetSession(cReq.GetSession());
	SetUserUri(cReq.GetUserUri());
	SetRcvBatchNtf(cReq.GetBatchNotify());

    TSipURI tDevUri(cReq.GetDevUri());

	TASKLOG(CMU_MOD_SS, EVENT_LEV, "�û�[%s]����PU[%s]��Ϣ����������:[%s]\n",
		GetUserUri().c_str(), cReq.GetDevUri().c_str(), GetSsTypesStr(cReq.GetSsTypes()).c_str());

	CPuStatusSsRsp cRsp;
	cRsp.SetHeadFromReq(cReq);

	do 
	{
		// �û��Ƿ��¼
		CLoginSession* ptUserLoginTask = GetLoginSession(cReq.GetSession());
		if (NULL == ptUserLoginTask)
		{
			TASKLOG(CMU_MOD_SS, CRITICAL_LEV, "�û�[%s]��¼session[%s]�Ƿ�\n",
				GetUserUri().c_str(), cReq.GetSession().c_str());

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

		// Ŀ���豸�Ƿ�Ϸ�
		if (!tDevUri.IsValidURI())
		{
			TASKLOG(CMU_MOD_SS, ERROR_LEV, "pu[%s]-Uri�Ƿ�, ����ʧ��\n", cReq.GetDevUri().c_str());

			cRsp.SetErrorCode(ERR_CMU_PU_NOT_IN_TOP);
			break;
		}

        // ���㶩����Ϊ�б��ĵ�����������������б���ֻ��һ���豸���б��ģ�
        TSsDevList tSsDevList, tSsSuccessList;
        tSsDevList.insert(TSsDevList::value_type(cReq.GetDevUri(), cReq.GetSsTypes()));
        TSsErrorList tSsErrorList;
        CCmuData::GetInstance()->AddSipSsTask(GetTaskNO(), tSsDevList, tSsSuccessList, tSsErrorList);
        if (tSsSuccessList.end() != tSsSuccessList.find(cReq.GetDevUri()))      // ���ĳɹ�
        {
            m_strDevUri = cReq.GetDevUri();
            m_tSsTypes  = cReq.GetSsTypes();

            // ������Task�ӵ���¼Task��
            ptUserLoginTask->AddTask(this);
        }
        else if (tSsErrorList.end() != tSsErrorList.find(cReq.GetDevUri()))     // ����ʧ��
        {
            TASKLOG(CMU_MOD_SS, ERROR_LEV, "Dev[%s]����ʧ�ܣ�������[%d], Msg[%s-%d]����ʧ��\n",
                cReq.GetDevUri().c_str(), tSsErrorList[cReq.GetDevUri()], cReq.GetEventStr().c_str(), cReq.GetEventId());

            cRsp.SetErrorCode(tSsErrorList[cReq.GetDevUri()]);
            break;
        }
        else
        {
            CMS_ASSERT(false && "unreachable...");
        }

	} while (false);

	//Ӧ������
	PostRsp(KDSIP_EVENT_SUBSCRIBE_RSP, pcOspSipMsg->GetSipTransID(), cRsp);

	// ���ĳɹ����ͳ�ʼ��֪ͨ
	if (CMS_SUCCESS == cRsp.GetErrorCode())
	{
		PublishInitialNotify(cReq.GetSsTypes());
	}

	return cRsp.GetErrorCode();
}

void CPuSubscribeTask::PublishInitialNotify(const TSsTypes &tSsTypes)
{
    CPuData *pcPuData = CCmuData::GetInstance()->GetPuDataByUri(m_strDevUri);
    if (NULL != pcPuData)
    {
        pcPuData->PublishCertainSipSsTypes(tSsTypes, ALL_SS, GetTaskNO(), false);
    }
}

u32 CPuSubscribeTask::ProcRefreshReq(CMessage * const pcMsg)
{
	CMS_ASSERT(PU_STATUS_REFRESH_SS_REQ == pcMsg->event);

	COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;

	CPuStatusRefreshSsReq cReq;
	pcOspSipMsg->GetMsgBody(cReq);

	CPuStatusRefreshSsRsp cRsp;
	cRsp.SetHeadFromReq(cReq);

    const TSsTypes &tSsTypes = cReq.GetSsTypes();

    TSsDevList tSsDevList;
    tSsDevList.insert(TSsDevList::value_type(m_strDevUri, tSsTypes));

    if (SS_OP_TYPE_ADD == cReq.GetSsOpType())				// ������������
    {
        TSsDevList   tSsSuccessList;
        TSsErrorList tSsErrorList;
        CCmuData::GetInstance()->AddSipSsTask(GetTaskNO(), tSsDevList, tSsSuccessList, tSsErrorList);
        if (tSsSuccessList.end() != tSsSuccessList.find(m_strDevUri))
        {
            m_tSsTypes.insert(tSsTypes.begin(), tSsTypes.end());
        }
        else if (tSsErrorList.end() != tSsErrorList.find(m_strDevUri))
        {
            cRsp.SetErrorCode(tSsErrorList[m_strDevUri]);
        }
    }
    else
    {
        CCmuData::GetInstance()->DelSipSsTask(GetTaskNO(), tSsDevList);

        TSsTypes tDiffSsTypes;
        set_difference(m_tSsTypes.begin(), m_tSsTypes.end(), tSsTypes.begin(), tSsTypes.end(), 
            inserter(tDiffSsTypes, tDiffSsTypes.begin()));
        m_tSsTypes = tDiffSsTypes;
    }

	PostRsp(KDSIP_EVENT_SUBSCRIBE_RSP, pcOspSipMsg->GetSipTransID(), cRsp);

	// ���������Ķ������ͣ�Ҳ�ȷ���һ�γ�ʼ״̬֪ͨ
	if (CMS_SUCCESS == cRsp.GetErrorCode() && SS_OP_TYPE_ADD == cReq.GetSsOpType())
	{
		PublishInitialNotify(cReq.GetSsTypes());
	}

	return cRsp.GetErrorCode();
}

void CPuSubscribeTask::ProcUnSsReq(CMessage * const pcMsg)
{
	CMS_ASSERT(PU_STATUS_UNSS_REQ == pcMsg->event);

	COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;

	CPuStatusUnSsReq cReq;
	pcOspSipMsg->GetMsgBody(cReq);

    CPuStatusUnSsRsp cRsp;
    cRsp.SetHeadFromReq(cReq);

    TSsDevList tSsDevList;
    tSsDevList.insert(TSsDevList::value_type(m_strDevUri, m_tSsTypes));

    CCmuData::GetInstance()->DelSipSsTask(GetTaskNO(), tSsDevList);
    m_tSsTypes.clear();

	PostRsp(KDSIP_EVENT_UNSUBSCRIBE_RSP, pcOspSipMsg->GetSipTransID(), cRsp);
}

void CPuSubscribeTask::ProcRemovePu(const string &strDevUri)
{
	if (strDevUri == m_strDevUri)
	{
        // ���ĵ��豸�����ˣ�ɾ���ö��ĻỰ
		delete this;
	}
}

void CPuSubscribeTask::ProcRemoveDomain(const string &strDomainName)
{

}
