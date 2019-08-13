
#include "cms/cms_proto.h"
#include "cms/cms_errorcode.h"
#include "ssbasetask.h"
#include "cmuconst.h"
#include "cms/cmu/cmu_struct.h"
#include "cms/cmu/cmu_proto.h"
#include "loginsession.h"
#include "cmudata.h"

extern bool g_bDebugOn;

CSsBaseTask::CSsBaseTask(CInstExt *pcInst) : CSipTask(pcInst)
{
    m_tSsDlgId = INVALID_DIALOG_ID;
	m_dwTopNtf = 0;
	m_dwTotalNtf = 0;
    m_dwSkipNtf = 0;
	m_bRcvBatchNtf = false;
}

CSsBaseTask::~CSsBaseTask()
{
    ReleaseResource();
}

void CSsBaseTask::InitStateMachine()
{
    CStateProc cWaitSsProc;
    cWaitSsProc.ProcMsg = (CTask::PFProcMsg)&CSsBaseTask::OnWaitSs;
    cWaitSsProc.ProcErrMsg = (CTask::PFProcMsg)&CSsBaseTask::OnWaitSs;
    cWaitSsProc.TimerPoll = (CTask::PFTimerPoll)&CSsBaseTask::OnWaitSsTimer;
    AddRuleProc(WaitSs, cWaitSsProc);

	CStateProc cNotifyProc;
	cNotifyProc.ProcMsg = (CTask::PFProcMsg)&CSsBaseTask::OnNotify;
	cNotifyProc.ProcErrMsg = (CTask::PFProcMsg)&CSsBaseTask::OnNotify;
	cNotifyProc.TimerPoll = (CTask::PFTimerPoll)&CSsBaseTask::OnNotifyTimer;
	AddRuleProc(Notify, cNotifyProc);

    CStateProc cServiceProc;
    cServiceProc.ProcMsg = (CTask::PFProcMsg)&CSsBaseTask::OnService;
    cServiceProc.ProcErrMsg = (CTask::PFProcMsg)&CSsBaseTask::OnService;
    cServiceProc.TimerPoll = (CTask::PFTimerPoll)&CSsBaseTask::OnServiceTimer;
    AddRuleProc(Service, cServiceProc);

    NextState(WaitSs);
}

void CSsBaseTask::ReleaseResource()
{
    switch (GetState())
    {
    case WaitSs:
	case Notify:
    case Service:
        {
            if (INVALID_DIALOG_ID != m_tSsDlgId)
			{
				OspSipReleaseDlg(m_tSsDlgId);
				m_tSsDlgId = INVALID_DIALOG_ID;
			}
        }
        break;
    default:
        {
            TASKLOG(CMU_MOD_SS, ERROR_LEV, "δ֪����״̬[%d]\n", GetState());
        }
        break;
    }
}

void CSsBaseTask::PrintData() const
{
    CSipTask::PrintData();

	OspPrintf(TRUE, FALSE, "\n  SipDlg[%p] RcvBatchNtf[%d] NtfSize[%u] TopNtf[%u] TotalNtf[%u] SkipNtf[%u]\n",
		m_tSsDlgId, m_bRcvBatchNtf, m_tNtfQueue.size(), m_dwTopNtf, m_dwTotalNtf, m_dwSkipNtf);

	if (g_bDebugOn)
	{
        OspPrintf(TRUE, FALSE, "  GpsDataList[%u]\n", m_cGpsDataList.size());
        OspPrintf(TRUE, FALSE, "  CurNtf[%s]\n", m_strCurNtf.c_str());
	}
}

u32 CSsBaseTask::OnWaitSs(CMessage *const pcMsg)
{
	u32 dwRet = PROCMSG_FAIL;

	CMS_ASSERT(pcMsg->content && "osp ��ϢΪ��\n");

	switch (pcMsg->event)
	{
	// Ŀǰ��WaitSs��ֻ����Sip��Ϣ������switch������չ��Ҫ����ķ�Sip��Ϣ
    // ע�⣺����ڸ�״̬�»���յ��������͵���Ϣ����һ��Ҫ������Ӧ��case��������ܵ�����Ϣ�������ת��
	default:
		{
			COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

			switch (pcOspSipMsg->GetSipEventID())
			{
			case KDSIP_EVENT_SUBSCRIBE_REQ:
				{
					m_tSsDlgId = pcOspSipMsg->GetSipDlgID();			// ����DlgId

					/*
					 * ������ProcSsReqǰ�Ƚ���Service״̬�������ProcSsReqʱ��Ҫ���ͳ�ʼ֪ͨ��Ͷ��֪ͨʱ���Զ�����Notify��
					 * ���ProcSsReq����Ҫ���ͳ�ʼ֪ͨ�������Service״̬Ҳ�ǶԵ�
					 */
					NextState(Service);

					if (CMS_SUCCESS == ProcSsReq(pcMsg))				// ���ĳɹ�
					{
						dwRet = PROCMSG_OK;
					}
					else
					{
						dwRet = PROCMSG_DEL;
					}
				}
				break;
			default:
				{
					TASKLOG(CMU_MOD_SS, WARNING_LEV, "Receive unknown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
				}
				break;
			}
		}
		break;
	}

	return dwRet;
}

u32 CSsBaseTask::OnWaitSsTimer()
{
	return TIMERPOLL_DONE;
}

u32 CSsBaseTask::OnNotify(CMessage *const pcMsg)
{
	u32 dwRet = PROCMSG_FAIL;

	CMS_ASSERT(pcMsg->content && "osp ��ϢΪ��\n");

	switch (pcMsg->event)
	{
	case OSP_SIP_MSG_PROC_FAIL:
		{
			COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

			TASKLOG(CMU_MOD_SS, WARNING_LEV, "�յ�SIP�����Ӧ��, �ȴ���ʱ�ط�, SipErrorCode[%d] SipDlg[%p] NtfSize[%u] TopNtf[%u] TotalNtf[%u]\n", 
				pcOspSipMsg->GetSipErrorCode(), m_tSsDlgId, m_tNtfQueue.size(), m_dwTopNtf, m_dwTotalNtf);

			dwRet = PROCMSG_OK;
		}
		break;
	case OSP_SIP_DIALOG_TERMINATE:
		{
			COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

			TASKLOG(CMU_MOD_SS, ERROR_LEV, "SIP�쳣��ֹ, �˳�����, SipErrorCode[%d] SipDlg[%p] NtfSize[%u] TopNtf[%u] TotalNtf[%u]\n", 
				pcOspSipMsg->GetSipErrorCode(), m_tSsDlgId, m_tNtfQueue.size(), m_dwTopNtf, m_dwTotalNtf);

			dwRet = PROCMSG_DEL;
		}
		break;
		/*
		 * ����������Ǳ���ģ�飬�����ʱ���յ�OSP_SIP_DISCONNECT��Ϣ
		 */
	case OSP_SIP_DISCONNECT:
		{
			COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
			const s8 *pszDstUri = (const s8 *)(pcOspSipMsg->GetMsgBody());

			TASKLOG(CMU_MOD_SS, WARNING_LEV, "���Ĺ����кͶԶ�[%s]�������˳����ģ�SipDlg[%p] NtfSize[%u] TopNtf[%u] TotalNtf[%u]\n", 
				pszDstUri, m_tSsDlgId, m_tNtfQueue.size(), m_dwTopNtf, m_dwTotalNtf);

			dwRet = PROCMSG_DEL;
		}
		break;
		/*
		 * ���������ǲ������յ�CMU_CMU_DISCONNECT��Ϣ�ģ���ʱ�ȶ���
		 */
	case CMU_CMU_DISCONNECT:
		{
			CMS_ASSERT(false && "impossible!!!");
		}
		break;
		/*
		 * ����������������cmu�������ʱ���յ�CMU_CMU_DELETE��Ϣ
		 */
	case CMU_CMU_DELETE:
		{
			COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;

			TASKLOG(CMU_MOD_SS, WARNING_LEV, "����cmu����Task���ڶ���������������Task�����˳���CmuUri[%s]\n", 
				pcOspSipMsg->GetMsgBody());

			dwRet = PROCMSG_DEL;
		}
		break;
	case CMU_DEV_OFFLINE_NTF:
		{
			TDevOnOffCBMsg *ptOnOff = (TDevOnOffCBMsg *)pcMsg->content;

			TASKLOG(CMU_MOD_SS, ERROR_LEV, "ģ��[%s - %s]����, �˳�����, SipDlg[%p] NtfSize[%u] TopNtf[%u] TotalNtf[%u]\n", 
				ptOnOff->GetDevUri().c_str(), ptOnOff->GetDevType().c_str(), m_tSsDlgId, m_tNtfQueue.size(), m_dwTopNtf, m_dwTotalNtf);

			dwRet = PROCMSG_DEL;
		}
		break;
	default:
		{
			COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

			switch (pcOspSipMsg->GetSipEventID())
			{
			case KDSIP_EVENT_NOTIFY_RSP:							// ����֪ͨӦ��
				{
					ProcNtfRsp(pcMsg);								// ��֪ͨӦ���ɾ����ҵ��Task��ʵ��

					do 
					{
						CEventRsp cNtfRsp;
						pcOspSipMsg->GetMsgBody(cNtfRsp);

						if (CMS_SUCCESS == cNtfRsp.GetErrorCode())	// �Զ˳ɹ��յ�֪ͨ�ſ�ʼ������һ��֪ͨ����ʵ��֪ͨ�Ŀɿ���
						{
							m_strCurNtf.clear();					// �ɹ����ͺ󣬽���ǰ�������һ��֪ͨ�����
							if (!m_tNtfQueue.empty())				// ���в�Ϊ�գ�����������һ��
							{
								CMS_ASSERT(INVALID_DIALOG_ID != m_tSsDlgId);

								PostNtf();

								NextState(Notify);

								dwRet = PROCMSG_OK;
							}
							else
							{
								TASKLOG(CMU_MOD_SS, TIMER_LEV, "֪ͨ��������Ϊ0��ֹͣ���ͣ��������̬����ǰ״̬[%s]\n", 
									GetStateName(GetState()));

								NextState(Service);

								dwRet = PROCMSG_OK;
							}
						}
					} while (false);
				}
				break;
			case KDSIP_EVENT_SUBSCRIBE_REQ:						// ˢ�¶�������
				{
					TASKLOG(CMU_MOD_SS, TIMER_LEV, "����Notify�ڼ��յ�ˢ�¶�������NtfSize[%u] TopNtf[%u] TotalNtf[%u]\n", 
						m_tNtfQueue.size(), m_dwTopNtf, m_dwTotalNtf);

					if (CMS_SUCCESS == ProcRefreshReq(pcMsg))
					{
						dwRet = PROCMSG_OK;
					}
				}
				break;
			case KDSIP_EVENT_UNSUBSCRIBE_REQ:					// ȡ����������
				{
					TASKLOG(CMU_MOD_SS, EVENT_LEV, "����Notify�ڼ��յ�ȡ����������NtfSize[%u] TopNtf[%u] TotalNtf[%u]\n", 
						m_tNtfQueue.size(), m_dwTopNtf, m_dwTotalNtf);

					ProcUnSsReq(pcMsg);

					dwRet = PROCMSG_DEL;
				}
				break;
			default:
				{
					TASKLOG(CMU_MOD_SS, WARNING_LEV, "Receive unknown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
				}
				break;
			}
		}
		break;
	}

	return dwRet;
}

u32 CSsBaseTask::OnNotifyTimer()
{
	if (CMU_REQ_RSP_TASK_TIMEOUT < GetCurStateHoldTime())			// ��ʱ�ط�
	{
		TASKLOG(CMU_MOD_SS, PROGRESS_LEV, "�ȴ�֪ͨӦ��ʱ���ط���֪ͨ[%s] NtfSize[%u] TopNtf[%u] TotalNtf[%u]\n", 
			m_strCurNtf.c_str(), m_tNtfQueue.size(), m_dwTopNtf, m_dwTotalNtf);

		PostNtf();

		NextState(Notify);
	}

	return TIMERPOLL_DONE;
}


u32 CSsBaseTask::OnService(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_FAIL;

	CMS_ASSERT(pcMsg->content && "osp ��ϢΪ��\n");

	switch (pcMsg->event)
	{
		/*
		 * ����������Ǳ���ģ�飬�����ʱ���յ�OSP_SIP_DISCONNECT��Ϣ
		 */
	case OSP_SIP_DISCONNECT:
		{
			COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
			const s8 *pszDstUri = (const s8 *)(pcOspSipMsg->GetMsgBody());

			TASKLOG(CMU_MOD_SS, WARNING_LEV, "���Ĺ����кͶԶ�[%s]�������˳����ģ�SipDlg[%p] NtfSize[%u] TopNtf[%u] TotalNtf[%u]\n", 
				pszDstUri, m_tSsDlgId, m_tNtfQueue.size(), m_dwTopNtf, m_dwTotalNtf);

			dwRet = PROCMSG_DEL;
		}
		break;
		/*
		 * ���������ǲ������յ�CMU_CMU_DISCONNECT��Ϣ�ģ���ʱ�ȶ���
		 */
	case CMU_CMU_DISCONNECT:
		{
			COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

            TASKLOG(CMU_MOD_SS, WARNING_LEV, "����cmu����Task����������Task�ȴ������ԣ�CmuUri[%s]\n", 
                pcOspSipMsg->GetMsgBody());

            dwRet = PROCMSG_OK;
		}
		break;
		/*
		 * ����������������cmu�������ʱ���յ�CMU_CMU_DELETE��Ϣ
		 */
	case CMU_CMU_DELETE:
		{
			COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;

			TASKLOG(CMU_MOD_SS, WARNING_LEV, "����cmu����Task���ڶ���������������Task�����˳���CmuUri[%s]\n", 
				pcOspSipMsg->GetMsgBody());

			dwRet = PROCMSG_DEL;
		}
		break;
	case OSP_SIP_DIALOG_TERMINATE:
		{
			COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
			TASKLOG(CMU_MOD_SS, ERROR_LEV, "SIP�쳣��ֹ, �˳�����, SipErrorCode[%d] SipDlg[%p] NtfSize[%u] TopNtf[%u] TotalNtf[%u]\n", 
				pcOspSipMsg->GetSipErrorCode(), m_tSsDlgId, m_tNtfQueue.size(), m_dwTopNtf, m_dwTotalNtf);

			dwRet = PROCMSG_DEL;
		}
		break;
	case CMU_DEV_OFFLINE_NTF:
		{
			TDevOnOffCBMsg *ptOnOff = (TDevOnOffCBMsg *)pcMsg->content;

			TASKLOG(CMU_MOD_SS, ERROR_LEV, "ģ��[%s - %s]����, �˳�����, SipDlg[%p] NtfSize[%u] TopNtf[%u] TotalNtf[%u]\n", 
				ptOnOff->GetDevUri().c_str(), ptOnOff->GetDevType().c_str(), m_tSsDlgId, m_tNtfQueue.size(), m_dwTopNtf, m_dwTotalNtf);

			dwRet = PROCMSG_DEL;
		}
		break;
	default:
		{
			COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
			switch (pcOspSipMsg->GetSipEventID())
			{
			case KDSIP_EVENT_SUBSCRIBE_REQ:						// ˢ�¶�������
				{
					TASKLOG(CMU_MOD_SS, TIMER_LEV, "����̬���յ�ˢ�¶�������NtfSize[%u]\n", m_tNtfQueue.size());

					if (CMS_SUCCESS == ProcRefreshReq(pcMsg))
					{
						dwRet = PROCMSG_OK;
					}
				}
				break;
			case KDSIP_EVENT_UNSUBSCRIBE_REQ:					// ȡ����������
				{
					TASKLOG(CMU_MOD_SS, PROGRESS_LEV, "����̬���յ�ȡ����������NtfSize[%u]\n", m_tNtfQueue.size());

					ProcUnSsReq(pcMsg);

					dwRet = PROCMSG_DEL;
				}
				break;
			default:
				{
					TASKLOG(CMU_MOD_SS, WARNING_LEV, "Receive unknown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
				}
				break;
			}
		}
		break;
	}

	return dwRet;
}

u32 CSsBaseTask::OnServiceTimer()
{
    return TIMERPOLL_DONE;
}

void CSsBaseTask::PostNotify(const string &strDevId, const TSsType &tSsType, const CEventReq &cNtfReq)
{
	TNtfElement tElement;
	tElement.tSsType = tSsType;

    if (!cNtfReq.GetEventStr().empty())
    {
        tElement.bDirectSend = true;
        tElement.cContent.push_back(cNtfReq.ToXml());
    }
    else
    {
        tElement.bDirectSend = false;
        tElement.strExtra1   = strDevId;
        tElement.strExtra2   = cNtfReq.GetSession();
    }

    bool bEmptyQueue = m_tNtfQueue.empty();	

    if (!tElement.bDirectSend)
    {
        if (!bEmptyQueue)
        {
            const TNtfElement &tRearNtf = m_tNtfQueue.back();
            if (!tRearNtf.bDirectSend
            && tRearNtf.strExtra1 == tElement.strExtra1
            && tRearNtf.tSsType == tElement.tSsType)
            {
                ++m_dwSkipNtf;

                return;
            }
        }

        // gpsdata�Ǽ�ʱ���ݣ���֪ͨ������ֻ����һ�ݼ��ɣ������ݹ��ڵ����ݺ������壬����ռ���ڴ�
        if (PU_GPS_STATUS == tSsType)
        {
            if (m_cGpsDataList.end() != m_cGpsDataList.find(strDevId))
            {
                ++m_dwSkipNtf;

                return;
            }
        }
    }
    
    m_tNtfQueue.push(tElement);

    if (!tElement.bDirectSend)
    {
        if (PU_GPS_STATUS == tSsType)
        {
            m_cGpsDataList.insert(strDevId);
        }
    }
	
	if (bEmptyQueue && Notify != GetState())		// ����Ϊ�գ����ҵ�ǰ���Ƿ���Notify״̬������Լ�������Notify
	{
		TASKLOG(CMU_MOD_SS, TIMER_LEV, "֪ͨ��������Ϊ1����ʼ����֪ͨ!״̬[%s]\n", GetStateName(GetState()));

		CMS_ASSERT(INVALID_DIALOG_ID != m_tSsDlgId);

		PostNtf();

		NextState(Notify);
	}

	// �����յ���֪ͨ����
	++m_dwTotalNtf;

	// ����֪ͨ���еķ�ֵ
	const u32 dwNtfSize = m_tNtfQueue.size();
	if (dwNtfSize > m_dwTopNtf)
	{
		m_dwTopNtf = dwNtfSize;
	}
}

void CSsBaseTask::ProcNtfRsp(CMessage * const pcMsg)
{
	COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;

	CEventRsp cNtfRsp;
	pcOspSipMsg->GetMsgBody(cNtfRsp);

	TASKLOG(CMU_MOD_SS, TIMER_LEV, "�յ��Զ�[%s]���ص�֪ͨӦ��[%d]��SipDlg[%p] Msg[%s - %d]\n", 
		GetUserUri().c_str(), cNtfRsp.GetErrorCode(), m_tSsDlgId, cNtfRsp.GetEventStr().c_str(), cNtfRsp.GetEventId());
}

void CSsBaseTask::GenNtfContent(TNtfElement &tElement)
{
    if (!tElement.bDirectSend && tElement.cContent.empty())
    {
        if (PU_ON_OFF_STATUS == tElement.tSsType)
        {
            CPuData *pcPuData = GetPuDataByUri(tElement.strExtra1);
            if (NULL != pcPuData)
            {
                CPuStatusNtyReq cNtfReq;
                cNtfReq.SetDevURI(tElement.strExtra1);
                cNtfReq.SetSession(tElement.strExtra2);
                cNtfReq.SetOnOffStatus(pcPuData->GetPuStatus());
                tElement.cContent.push_back(cNtfReq.ToXml());
            }
        }
        else if (PU_CONFIG_STATUS == tElement.tSsType)
        {
            CPuData *pcPuData = GetPuDataByUri(tElement.strExtra1);
            if (NULL != pcPuData)
            {
                TPuDevConfig tPuDevConfig = pcPuData->GetPuDevConfig();
                vector<TPuDevConfig> vctConfig;
                CPuData::SplitPuConfig2ConfigNtyReq(tPuDevConfig, vctConfig);

                CPuConfigNtyReq cNtfReq;
                cNtfReq.SetDevURI(tElement.strExtra1);
                cNtfReq.SetSession(tElement.strExtra2);
                for (vector<TPuDevConfig>::const_iterator it = vctConfig.begin(); vctConfig.end() != it; ++it)
                {
                    cNtfReq.SetConfig(*it);
                    tElement.cContent.push_back(cNtfReq.ToXml());
                }
            }
        }
        else if (PU_SERVICE_STATUS == tElement.tSsType)
        {
            CPuData *pcPuData = GetPuDataByUri(tElement.strExtra1);
            if (NULL != pcPuData)
            {
                CPuServiceStatusNtyReq cNtfReq;
                cNtfReq.SetDevURI(tElement.strExtra1);
                cNtfReq.SetSession(tElement.strExtra2);
                cNtfReq.SetServiceStatus(pcPuData->GetPuServiceStatus());
                tElement.cContent.push_back(cNtfReq.ToXml());
            }
        }
        else if (PU_ALARM_STATUS == tElement.tSsType)
        {
            CPuData *pcPuData = GetPuDataByUri(tElement.strExtra1);
            if (NULL != pcPuData)
            {
                CPuAlarmStatusNtyReq cNtfReq;
                cNtfReq.SetDevURI(tElement.strExtra1);
                cNtfReq.SetSession(tElement.strExtra2);
                cNtfReq.SetAlarmStatus(pcPuData->GetPuAlarmStatus());
                tElement.cContent.push_back(cNtfReq.ToXml());
            }
        }
        else if (PU_GPS_STATUS == tElement.tSsType)
        {
            CPuData *pcPuData = GetPuDataByUri(tElement.strExtra1);
            if (NULL != pcPuData)
            {
                CPuGpsStatusNtyReq cNtfReq;
                cNtfReq.SetDevURI(tElement.strExtra1);
                cNtfReq.SetSession(tElement.strExtra2);
                cNtfReq.SetGpsData(pcPuData->GetPuGpsData());
                tElement.cContent.push_back(cNtfReq.ToXml());
            }
        }
        else if (PU_TRANSDATA_STATUS == tElement.tSsType)
        {
            CPuData *pcPuData = GetPuDataByUri(tElement.strExtra1);
            if (NULL != pcPuData)
            {
                CPuTransDataStatusNtyReq cNtfReq;
                cNtfReq.SetDevURI(tElement.strExtra1);
                cNtfReq.SetSession(tElement.strExtra2);
                cNtfReq.SetData(pcPuData->GetPuTransparentData());
                tElement.cContent.push_back(cNtfReq.ToXml());
            }
        }
        else
        {
            CMS_ASSERT(false && "unreachable...");
        }

        // �Ѿ����ɺ��ˣ�����Ϊֱ�ӷ���
        tElement.bDirectSend = true;
    }
}

void CSsBaseTask::PostNtf()
{
	if (INVALID_DIALOG_ID != m_tSsDlgId)
	{
		if (m_strCurNtf.empty())								// ��Ҫ���Ͷ����е�֪ͨ
		{
            if (!m_tNtfQueue.empty())
            {
                if (m_bRcvBatchNtf)                             // ��������֪ͨ
                {
                    CPuBatchNtyReq cReq;
                    u32 dwNtfSize = cReq.ToXml().size();

                    do 
                    {
                        TNtfElement &tElement = m_tNtfQueue.front();

                        bool bDirectSend = tElement.bDirectSend;
                        if (!bDirectSend)                       // ������֪ͨ���ݣ��ٷ���֪ͨ
                        {
                            GenNtfContent(tElement);
                        }

                        CMS_ASSERT(tElement.bDirectSend && "impossible...");

                        if (!tElement.cContent.empty())
                        {
                            TGeneralData tDataItem;
                            tDataItem.type    = tElement.tSsType;
                            tDataItem.content = tElement.cContent.front();

                            u32 dwItemSize    = tDataItem.ToXml().size();
                            if (dwNtfSize + dwItemSize + 100 <= NTF_BUF_MAX_SIZE || cReq.GetData().empty())
                            {
                                cReq.GetData().push_back(tDataItem);
                                tElement.cContent.pop_front();

                                // ����xml���л��Ժ󣬵���֪ͨ�ĳ��Ȼ�䳤�ģ�������CTransparentData������ϩ�![CDATA[ ]]> ˵��������vector���л��ĸ��ӳ��ȣ�,
                                // ������֪ͨ�е��������ʱ����Ҳ��һ�ʿ��������Ҵ��Թ���Ϊ100�ֽڣ�Ӧ���ǹ���
                                // ��Ȼ�������е���֣�������������Ĺ����в��þ����Ե�ToXml()�����Լӿ�����ٶ�
                                dwNtfSize += dwItemSize + 100;
                            }
                            else
                            {
                                break;
                            }
                        }
                        
                        if (tElement.cContent.empty())          // ����֪ͨ�е��������ݷ������
                        {
                            if (!bDirectSend)
                            {
                                if (PU_GPS_STATUS == tElement.tSsType)
                                {
                                    m_cGpsDataList.erase(tElement.strExtra1);
                                }
                            }

                            m_tNtfQueue.pop();
                        }

                    } while (!m_tNtfQueue.empty());

                    if (!cReq.GetData().empty())
                    {
                        m_strCurNtf = cReq.ToXml();
                    }
                }
                else
                {
                    TNtfElement &tElement = m_tNtfQueue.front();

                    bool bDirectSend = tElement.bDirectSend;
                    if (!bDirectSend)                       // ������֪ͨ���ݣ��ٷ���֪ͨ
                    {
                        GenNtfContent(tElement);
                    }

                    CMS_ASSERT(tElement.bDirectSend && "impossible...");

                    if (!tElement.cContent.empty())
                    {
                        m_strCurNtf = tElement.cContent.front();
                        tElement.cContent.pop_front();
                    }
                    
                    if (tElement.cContent.empty())          // ����֪ͨ�е��������ݷ������
                    {
                        if (!bDirectSend)
                        {
                            if (PU_GPS_STATUS == tElement.tSsType)
                            {
                                m_cGpsDataList.erase(tElement.strExtra1);
                            }
                        }

                        m_tNtfQueue.pop();
                    }
                }
            }
            else
            {
                // ����֪ͨ���������
                return;
            }
		}

        if (!m_strCurNtf.empty())
        {
            if (NTF_BUF_MAX_SIZE < m_strCurNtf.size())
            {
                TASKLOG(CMU_MOD_SS, EVENT_LEV, "֪ͨ��Ϣ������̫��[%u > %u]����֪ͨ���ܷ���ʧ��\n", 
                    m_strCurNtf.size(), NTF_BUF_MAX_SIZE);
            }

            PostInDlgReq(KDSIP_EVENT_NOTIFY_REQ, m_strCurNtf, m_tSsDlgId);
        }
        else
        {
            PostNtf();
        }
	}
}



