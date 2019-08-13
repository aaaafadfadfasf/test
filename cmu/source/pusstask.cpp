
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
        // CPuSubscribeTask在订阅失败析构时也会走到这里来，但是DelTask不会导致任何问题，这里直接简单的处理下即可
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

	TASKLOG(CMU_MOD_SS, EVENT_LEV, "用户[%s]订阅PU[%s]信息，订阅类型:[%s]\n",
		GetUserUri().c_str(), cReq.GetDevUri().c_str(), GetSsTypesStr(cReq.GetSsTypes()).c_str());

	CPuStatusSsRsp cRsp;
	cRsp.SetHeadFromReq(cReq);

	do 
	{
		// 用户是否登录
		CLoginSession* ptUserLoginTask = GetLoginSession(cReq.GetSession());
		if (NULL == ptUserLoginTask)
		{
			TASKLOG(CMU_MOD_SS, CRITICAL_LEV, "用户[%s]登录session[%s]非法\n",
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

		// 目标设备是否合法
		if (!tDevUri.IsValidURI())
		{
			TASKLOG(CMU_MOD_SS, ERROR_LEV, "pu[%s]-Uri非法, 订阅失败\n", cReq.GetDevUri().c_str());

			cRsp.SetErrorCode(ERR_CMU_PU_NOT_IN_TOP);
			break;
		}

        // 单点订阅视为列表订阅的特殊情况来处理（即列表中只有一个设备的列表订阅）
        TSsDevList tSsDevList, tSsSuccessList;
        tSsDevList.insert(TSsDevList::value_type(cReq.GetDevUri(), cReq.GetSsTypes()));
        TSsErrorList tSsErrorList;
        CCmuData::GetInstance()->AddSipSsTask(GetTaskNO(), tSsDevList, tSsSuccessList, tSsErrorList);
        if (tSsSuccessList.end() != tSsSuccessList.find(cReq.GetDevUri()))      // 订阅成功
        {
            m_strDevUri = cReq.GetDevUri();
            m_tSsTypes  = cReq.GetSsTypes();

            // 将订阅Task加到登录Task中
            ptUserLoginTask->AddTask(this);
        }
        else if (tSsErrorList.end() != tSsErrorList.find(cReq.GetDevUri()))     // 订阅失败
        {
            TASKLOG(CMU_MOD_SS, ERROR_LEV, "Dev[%s]订阅失败，错误码[%d], Msg[%s-%d]请求失败\n",
                cReq.GetDevUri().c_str(), tSsErrorList[cReq.GetDevUri()], cReq.GetEventStr().c_str(), cReq.GetEventId());

            cRsp.SetErrorCode(tSsErrorList[cReq.GetDevUri()]);
            break;
        }
        else
        {
            CMS_ASSERT(false && "unreachable...");
        }

	} while (false);

	//应答订阅者
	PostRsp(KDSIP_EVENT_SUBSCRIBE_RSP, pcOspSipMsg->GetSipTransID(), cRsp);

	// 订阅成功后发送初始的通知
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

    if (SS_OP_TYPE_ADD == cReq.GetSsOpType())				// 新增订阅类型
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

	// 对于新增的订阅类型，也先发送一次初始状态通知
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
        // 订阅的设备退网了，删除该订阅会话
		delete this;
	}
}

void CPuSubscribeTask::ProcRemoveDomain(const string &strDomainName)
{

}
