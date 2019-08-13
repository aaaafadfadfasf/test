
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
            TASKLOG(CMU_MOD_SS, ERROR_LEV, "未知事务状态[%d]\n", GetState());
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

	CMS_ASSERT(pcMsg->content && "osp 消息为空\n");

	switch (pcMsg->event)
	{
	// 目前在WaitSs下只处理Sip消息，外层的switch用来扩展需要处理的非Sip消息
    // 注意：如果在该状态下会接收到其他类型的消息，则一定要加上相应的case，否则可能导致消息被错误的转换
	default:
		{
			COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

			switch (pcOspSipMsg->GetSipEventID())
			{
			case KDSIP_EVENT_SUBSCRIBE_REQ:
				{
					m_tSsDlgId = pcOspSipMsg->GetSipDlgID();			// 保存DlgId

					/*
					 * 这里在ProcSsReq前先进入Service状态，如果在ProcSsReq时需要发送初始通知，投递通知时会自动进入Notify的
					 * 如果ProcSsReq不需要发送初始通知，则进入Service状态也是对的
					 */
					NextState(Service);

					if (CMS_SUCCESS == ProcSsReq(pcMsg))				// 订阅成功
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

	CMS_ASSERT(pcMsg->content && "osp 消息为空\n");

	switch (pcMsg->event)
	{
	case OSP_SIP_MSG_PROC_FAIL:
		{
			COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

			TASKLOG(CMU_MOD_SS, WARNING_LEV, "收到SIP层错误应答, 等待超时重发, SipErrorCode[%d] SipDlg[%p] NtfSize[%u] TopNtf[%u] TotalNtf[%u]\n", 
				pcOspSipMsg->GetSipErrorCode(), m_tSsDlgId, m_tNtfQueue.size(), m_dwTopNtf, m_dwTotalNtf);

			dwRet = PROCMSG_OK;
		}
		break;
	case OSP_SIP_DIALOG_TERMINATE:
		{
			COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

			TASKLOG(CMU_MOD_SS, ERROR_LEV, "SIP异常终止, 退出订阅, SipErrorCode[%d] SipDlg[%p] NtfSize[%u] TopNtf[%u] TotalNtf[%u]\n", 
				pcOspSipMsg->GetSipErrorCode(), m_tSsDlgId, m_tNtfQueue.size(), m_dwTopNtf, m_dwTotalNtf);

			dwRet = PROCMSG_DEL;
		}
		break;
		/*
		 * 如果订阅者是本域模块，则断链时将收到OSP_SIP_DISCONNECT消息
		 */
	case OSP_SIP_DISCONNECT:
		{
			COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
			const s8 *pszDstUri = (const s8 *)(pcOspSipMsg->GetMsgBody());

			TASKLOG(CMU_MOD_SS, WARNING_LEV, "订阅过程中和对端[%s]断链，退出订阅，SipDlg[%p] NtfSize[%u] TopNtf[%u] TotalNtf[%u]\n", 
				pszDstUri, m_tSsDlgId, m_tNtfQueue.size(), m_dwTopNtf, m_dwTotalNtf);

			dwRet = PROCMSG_DEL;
		}
		break;
		/*
		 * 分析来看是不可能收到CMU_CMU_DISCONNECT消息的，暂时先断言
		 */
	case CMU_CMU_DISCONNECT:
		{
			CMS_ASSERT(false && "impossible!!!");
		}
		break;
		/*
		 * 如果订阅者是异域的cmu，则断链时将收到CMU_CMU_DELETE消息
		 */
	case CMU_CMU_DELETE:
		{
			COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;

			TASKLOG(CMU_MOD_SS, WARNING_LEV, "异域cmu连接Task由于断链被析构，订阅Task即将退出，CmuUri[%s]\n", 
				pcOspSipMsg->GetMsgBody());

			dwRet = PROCMSG_DEL;
		}
		break;
	case CMU_DEV_OFFLINE_NTF:
		{
			TDevOnOffCBMsg *ptOnOff = (TDevOnOffCBMsg *)pcMsg->content;

			TASKLOG(CMU_MOD_SS, ERROR_LEV, "模块[%s - %s]下线, 退出订阅, SipDlg[%p] NtfSize[%u] TopNtf[%u] TotalNtf[%u]\n", 
				ptOnOff->GetDevUri().c_str(), ptOnOff->GetDevType().c_str(), m_tSsDlgId, m_tNtfQueue.size(), m_dwTopNtf, m_dwTotalNtf);

			dwRet = PROCMSG_DEL;
		}
		break;
	default:
		{
			COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

			switch (pcOspSipMsg->GetSipEventID())
			{
			case KDSIP_EVENT_NOTIFY_RSP:							// 订阅通知应答
				{
					ProcNtfRsp(pcMsg);								// 将通知应答交由具体的业务Task来实现

					do 
					{
						CEventRsp cNtfRsp;
						pcOspSipMsg->GetMsgBody(cNtfRsp);

						if (CMS_SUCCESS == cNtfRsp.GetErrorCode())	// 对端成功收到通知才开始处理下一条通知，以实现通知的可靠性
						{
							m_strCurNtf.clear();					// 成功发送后，将当前缓存的这一条通知给清空
							if (!m_tNtfQueue.empty())				// 队列不为空，继续发送下一条
							{
								CMS_ASSERT(INVALID_DIALOG_ID != m_tSsDlgId);

								PostNtf();

								NextState(Notify);

								dwRet = PROCMSG_OK;
							}
							else
							{
								TASKLOG(CMU_MOD_SS, TIMER_LEV, "通知队列数量为0，停止发送，进入服务态，当前状态[%s]\n", 
									GetStateName(GetState()));

								NextState(Service);

								dwRet = PROCMSG_OK;
							}
						}
					} while (false);
				}
				break;
			case KDSIP_EVENT_SUBSCRIBE_REQ:						// 刷新订阅请求
				{
					TASKLOG(CMU_MOD_SS, TIMER_LEV, "发送Notify期间收到刷新订阅请求，NtfSize[%u] TopNtf[%u] TotalNtf[%u]\n", 
						m_tNtfQueue.size(), m_dwTopNtf, m_dwTotalNtf);

					if (CMS_SUCCESS == ProcRefreshReq(pcMsg))
					{
						dwRet = PROCMSG_OK;
					}
				}
				break;
			case KDSIP_EVENT_UNSUBSCRIBE_REQ:					// 取消订阅请求
				{
					TASKLOG(CMU_MOD_SS, EVENT_LEV, "发送Notify期间收到取消订阅请求，NtfSize[%u] TopNtf[%u] TotalNtf[%u]\n", 
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
	if (CMU_REQ_RSP_TASK_TIMEOUT < GetCurStateHoldTime())			// 超时重发
	{
		TASKLOG(CMU_MOD_SS, PROGRESS_LEV, "等待通知应答超时，重发该通知[%s] NtfSize[%u] TopNtf[%u] TotalNtf[%u]\n", 
			m_strCurNtf.c_str(), m_tNtfQueue.size(), m_dwTopNtf, m_dwTotalNtf);

		PostNtf();

		NextState(Notify);
	}

	return TIMERPOLL_DONE;
}


u32 CSsBaseTask::OnService(CMessage *const pcMsg)
{
    u32 dwRet = PROCMSG_FAIL;

	CMS_ASSERT(pcMsg->content && "osp 消息为空\n");

	switch (pcMsg->event)
	{
		/*
		 * 如果订阅者是本域模块，则断链时将收到OSP_SIP_DISCONNECT消息
		 */
	case OSP_SIP_DISCONNECT:
		{
			COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
			const s8 *pszDstUri = (const s8 *)(pcOspSipMsg->GetMsgBody());

			TASKLOG(CMU_MOD_SS, WARNING_LEV, "订阅过程中和对端[%s]断链，退出订阅，SipDlg[%p] NtfSize[%u] TopNtf[%u] TotalNtf[%u]\n", 
				pszDstUri, m_tSsDlgId, m_tNtfQueue.size(), m_dwTopNtf, m_dwTotalNtf);

			dwRet = PROCMSG_DEL;
		}
		break;
		/*
		 * 分析来看是不可能收到CMU_CMU_DISCONNECT消息的，暂时先断言
		 */
	case CMU_CMU_DISCONNECT:
		{
			COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

            TASKLOG(CMU_MOD_SS, WARNING_LEV, "异域cmu连接Task断链，订阅Task等待其重试，CmuUri[%s]\n", 
                pcOspSipMsg->GetMsgBody());

            dwRet = PROCMSG_OK;
		}
		break;
		/*
		 * 如果订阅者是异域的cmu，则断链时将收到CMU_CMU_DELETE消息
		 */
	case CMU_CMU_DELETE:
		{
			COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;

			TASKLOG(CMU_MOD_SS, WARNING_LEV, "异域cmu连接Task由于断链被析构，订阅Task即将退出，CmuUri[%s]\n", 
				pcOspSipMsg->GetMsgBody());

			dwRet = PROCMSG_DEL;
		}
		break;
	case OSP_SIP_DIALOG_TERMINATE:
		{
			COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
			TASKLOG(CMU_MOD_SS, ERROR_LEV, "SIP异常终止, 退出订阅, SipErrorCode[%d] SipDlg[%p] NtfSize[%u] TopNtf[%u] TotalNtf[%u]\n", 
				pcOspSipMsg->GetSipErrorCode(), m_tSsDlgId, m_tNtfQueue.size(), m_dwTopNtf, m_dwTotalNtf);

			dwRet = PROCMSG_DEL;
		}
		break;
	case CMU_DEV_OFFLINE_NTF:
		{
			TDevOnOffCBMsg *ptOnOff = (TDevOnOffCBMsg *)pcMsg->content;

			TASKLOG(CMU_MOD_SS, ERROR_LEV, "模块[%s - %s]下线, 退出订阅, SipDlg[%p] NtfSize[%u] TopNtf[%u] TotalNtf[%u]\n", 
				ptOnOff->GetDevUri().c_str(), ptOnOff->GetDevType().c_str(), m_tSsDlgId, m_tNtfQueue.size(), m_dwTopNtf, m_dwTotalNtf);

			dwRet = PROCMSG_DEL;
		}
		break;
	default:
		{
			COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;
			switch (pcOspSipMsg->GetSipEventID())
			{
			case KDSIP_EVENT_SUBSCRIBE_REQ:						// 刷新订阅请求
				{
					TASKLOG(CMU_MOD_SS, TIMER_LEV, "服务态下收到刷新订阅请求，NtfSize[%u]\n", m_tNtfQueue.size());

					if (CMS_SUCCESS == ProcRefreshReq(pcMsg))
					{
						dwRet = PROCMSG_OK;
					}
				}
				break;
			case KDSIP_EVENT_UNSUBSCRIBE_REQ:					// 取消订阅请求
				{
					TASKLOG(CMU_MOD_SS, PROGRESS_LEV, "服务态下收到取消订阅请求，NtfSize[%u]\n", m_tNtfQueue.size());

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

        // gpsdata是即时数据，在通知队列中只保存一份即可，保存多份过期的数据毫无意义，而且占用内存
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
	
	if (bEmptyQueue && Notify != GetState())		// 队列为空，并且当前不是发送Notify状态，则可以继续发送Notify
	{
		TASKLOG(CMU_MOD_SS, TIMER_LEV, "通知队列数量为1，开始发送通知!状态[%s]\n", GetStateName(GetState()));

		CMS_ASSERT(INVALID_DIALOG_ID != m_tSsDlgId);

		PostNtf();

		NextState(Notify);
	}

	// 计算收到的通知总数
	++m_dwTotalNtf;

	// 计算通知队列的峰值
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

	TASKLOG(CMU_MOD_SS, TIMER_LEV, "收到对端[%s]返回的通知应答[%d]，SipDlg[%p] Msg[%s - %d]\n", 
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

        // 已经生成好了，设置为直接发送
        tElement.bDirectSend = true;
    }
}

void CSsBaseTask::PostNtf()
{
	if (INVALID_DIALOG_ID != m_tSsDlgId)
	{
		if (m_strCurNtf.empty())								// 需要发送队列中的通知
		{
            if (!m_tNtfQueue.empty())
            {
                if (m_bRcvBatchNtf)                             // 接收批量通知
                {
                    CPuBatchNtyReq cReq;
                    u32 dwNtfSize = cReq.ToXml().size();

                    do 
                    {
                        TNtfElement &tElement = m_tNtfQueue.front();

                        bool bDirectSend = tElement.bDirectSend;
                        if (!bDirectSend)                       // 先生成通知内容，再发送通知
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

                                // 经过xml序列化以后，单条通知的长度会变长的（由于是CTransparentData，会加上﹤![CDATA[ ]]> 说明，还有vector序列化的附加长度）,
                                // 当批量通知中的数量多的时候，这也是一笔开销，姑且粗略估算为100字节，应该是够了
                                // 虽然看起来有点奇怪，但是这样组包的过程中不用经常性的ToXml()，可以加快组包速度
                                dwNtfSize += dwItemSize + 100;
                            }
                            else
                            {
                                break;
                            }
                        }
                        
                        if (tElement.cContent.empty())          // 该条通知中的所有数据发送完毕
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
                    if (!bDirectSend)                       // 先生成通知内容，再发送通知
                    {
                        GenNtfContent(tElement);
                    }

                    CMS_ASSERT(tElement.bDirectSend && "impossible...");

                    if (!tElement.cContent.empty())
                    {
                        m_strCurNtf = tElement.cContent.front();
                        tElement.cContent.pop_front();
                    }
                    
                    if (tElement.cContent.empty())          // 该条通知中的所有数据发送完毕
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
                // 所有通知都发送完毕
                return;
            }
		}

        if (!m_strCurNtf.empty())
        {
            if (NTF_BUF_MAX_SIZE < m_strCurNtf.size())
            {
                TASKLOG(CMU_MOD_SS, EVENT_LEV, "通知消息的内容太长[%u > %u]，该通知可能发送失败\n", 
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



