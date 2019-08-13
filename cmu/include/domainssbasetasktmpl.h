/*========================================================================================
ģ����    ��cmu
�ļ���    ��domainssbasetasktmpl.h
����ļ�  ��
ʵ�ֹ���  �������б��ġ�����Ŀ¼���ĵĻ��࣬�ṩ�Ŷ�ʽ��ˢ�¶��Ļ���
����      ��xiongxh
��Ȩ      ��<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
�޸ļ�¼��
����               �汾              �޸���             �߶���              �޸ļ�¼
2014/05/27         V1.0              xiongxh                                �½��ļ�
=========================================================================================*/


#ifndef _DOMAIN_SS_BASE_TASK_TMPL_H
#define _DOMAIN_SS_BASE_TASK_TMPL_H

#include <queue>
using std::queue;

#include "cms/ospsip/ospsip.h"
#include "cmuconnecttask.h"
#include "cmuconst.h"
#include "cms/cms_errorcode.h"
#include "cms/cmu/cmu_event.h"
#include "cmudata.h"

enum CMU_CONNECT_STATE {CMU_CONNECT_STATE_UNKNOWN, CMU_CONNECT_STATE_CONNECT, CMU_CONNECT_STATE_DISCONNECT};

template <typename T>
class CSsUnit
{
public:
	CSsUnit()
	{

	}

    CSsUnit(const T &cObject, const TSsType &tSsType) : m_cObject(cObject), m_tSsType(tSsType)
    {

    }

    ~CSsUnit()
    {

    }

    const T &GetObject() const
    {
        return m_cObject;
    }
    T &GetObject()
    {
        return m_cObject;
    }
    void SetObject(const T &cObject)
    {
        m_cObject = cObject;
    }

    const TSsType &GetSsType() const
    {
        return m_tSsType;
    }
    TSsType &GetSsType()
    {
        return m_tSsType;
    }
    void SetSsType(const TSsType &tSsType)
    {
        m_tSsType = tSsType;
    }

public:
    bool operator<(const CSsUnit<T> &cUnit) const
    {
        if (m_cObject < cUnit.GetObject())
        {
            return true;
        }
        else
        {
            if (cUnit.GetObject() < m_cObject)
            {
                return false;
            }
            else
            {
                return m_tSsType < cUnit.GetSsType();
            }
        }
    }

private:
	T       m_cObject;
	TSsType m_tSsType;
};

template <typename T>
struct TSsOpInfo
{
public:
	TSsOpInfo()
	{
		tSsOpType  = SS_OP_TYPE_ADD;
	}

	TSsOpType        tSsOpType;					// ���Ĳ������ͣ�SS_OP_TYPE_ADD/SS_OP_TYPE_DEL
	set<CSsUnit<T> > cSsList;				    // ���ĵ�Ԫ����
};

template <typename T>
class CDomainSsBaseTaskTmpl : public CSipTask
{
public:
	enum
	{
		Idle = UNKNOWN_STATE + 1,
		WaitSs,
		Service,
	};

public:
	CDomainSsBaseTaskTmpl(CInstExt *pcInst, CCmuConnecterTask *pcLoginTask) : CSipTask(pcInst), m_pcConnectTask(pcLoginTask)
	{
		CMS_ASSERT(NULL != m_pcConnectTask);

		m_tSsDlgId   = INVALID_DIALOG_ID;
		m_dwTopSsNum = 0;
        m_strDomainName = m_pcConnectTask->GetDstUri().GetDomain();

		m_pcConnectTask->AddTask(this);
	}

	virtual ~CDomainSsBaseTaskTmpl()
	{
		ReleaseResource();

		m_pcConnectTask->DelTask(this);
	}

public:
	virtual const char* GetObjName() const
	{
		return "CDomainSsBaseTaskTmpl";
	}

	virtual const char* GetStateName(u32 dwState) const
	{
		switch (dwState)
		{
		case Idle:
			return "Idle";
		case WaitSs:
			return "WaitSs";
		case Service:
			return "Service";
		default:
			break;
		}
		return "Unknown State";
	}

public:
	virtual void InitStateMachine()
	{
		CStateProc cIdleProc;
		cIdleProc.ProcMsg = (CTask::PFProcMsg)&CDomainSsBaseTaskTmpl<T>::OnIdle;
		cIdleProc.ProcErrMsg = (CTask::PFProcMsg)&CDomainSsBaseTaskTmpl<T>::OnIdle;
		cIdleProc.TimerPoll = (CTask::PFTimerPoll)&CDomainSsBaseTaskTmpl<T>::OnIdleTimer;
		AddRuleProc(Idle, cIdleProc);

		CStateProc cWaitSsProc;
		cWaitSsProc.ProcMsg = (CTask::PFProcMsg)&CDomainSsBaseTaskTmpl<T>::OnWaitSs;
		cWaitSsProc.ProcErrMsg = (CTask::PFProcMsg)&CDomainSsBaseTaskTmpl<T>::OnWaitSs;
		cWaitSsProc.TimerPoll = (CTask::PFTimerPoll)&CDomainSsBaseTaskTmpl<T>::OnWaitSsTimer;
		AddRuleProc(WaitSs, cWaitSsProc);

		CStateProc cServiceProc;
		cServiceProc.ProcMsg = (CTask::PFProcMsg)&CDomainSsBaseTaskTmpl<T>::OnService;
		cServiceProc.ProcErrMsg = (CTask::PFProcMsg)&CDomainSsBaseTaskTmpl<T>::OnService;
		cServiceProc.TimerPoll = (CTask::PFTimerPoll)&CDomainSsBaseTaskTmpl<T>::OnServiceTimer;
		AddRuleProc(Service, cServiceProc);

		NextState(Idle);
	}

    virtual void PrintData() const
	{
		CSipTask::PrintData();

		OspPrintf(TRUE, FALSE, "SsDlgId[%ld] CmuUri[%s] ConnectTask[%u] SsOpQueue[%u] Top[%u] SsInfoMap[%u]\n", 
			m_tSsDlgId, m_pcConnectTask->GetServerUri().c_str(), m_pcConnectTask->GetTaskNO(), 
			m_cSsOpQueue.size(), m_dwTopSsNum, m_cSsList.size());

		if (g_bDebugOn && !m_cSsOpQueue.empty())
		{
			const TSsOpInfo<T> &tOpInfo = m_cSsOpQueue.front();
			if (!tOpInfo.cSsList.empty())
			{
				OspPrintf(TRUE, FALSE, "---------------------------------SsOpQueue-------------------------------------\n");
				for (typename set<CSsUnit<T> >::const_iterator it = tOpInfo.cSsList.begin(); tOpInfo.cSsList.end() != it; ++it)
				{
					OspPrintf(TRUE, FALSE, "  OpType[%s] SsUnit[%s]\n", 
						tOpInfo.tSsOpType.c_str(), SsUnit2String(*it).c_str());
				}
			}
		}

		if (g_bDebugOn && !m_cSsList.empty())
		{
			OspPrintf(TRUE, FALSE, "---------------------------------SsInfoMap-------------------------------------\n");
			for (typename set<CSsUnit<T> >::const_iterator it = m_cSsList.begin(); m_cSsList.end() != it; ++it)
			{
				OspPrintf(TRUE, FALSE, "  SsUnit[%s]\n", SsUnit2String(*it).c_str());
			}
		}
	}

	void ReleaseResource()
	{
		if (INVALID_DIALOG_ID != m_tSsDlgId)
		{
			// ����Task��Ϊ���ĵ����󷽣��������OspSipReleaseDlg�ͷű���Sip��Դ��
			// ͬʱ�Զ˵ײ��յ�Terminate���͵�Notify��ҵ����յ�OSP_SIP_DIALOG_TERMINATE
			OspSipReleaseDlg(m_tSsDlgId);
			m_tSsDlgId = INVALID_DIALOG_ID;
		}
	}

public:
	//״̬����
	u32 OnIdle(CMessage *const pcMsg)
	{
		u32 dwRet = PROCMSG_OK;

		switch(pcMsg->event)
		{
		case CMU_CMU_CONNECT_SUCCESS:
			{
				COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
				CMS_ASSERT(NULL != pcOspSipMsg);

				const s8 *pszDstUri = (const s8 *)(pcOspSipMsg->GetMsgBody());

				TASKLOG(CMU_MOD_SS, CRITICAL_LEV, "connect to cmu[%s] succeed!\n", pszDstUri);

				ProcSsQueue();
			}
			break;
		case CMU_CMU_CONNECT_FAIL:
			{
				COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
				CMS_ASSERT(NULL != pcOspSipMsg);

				const s8 *pszDstUri = (const s8 *)(pcOspSipMsg->GetMsgBody());

				TASKLOG(CMU_MOD_SS, CRITICAL_LEV, "connect to cmu[%s] failed!\n", pszDstUri);
			}
			break;
		default:
			{
				TASKLOG(CMU_MOD_SS, WARNING_LEV, "Receive unknown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

				dwRet = PROCMSG_FAIL;
			}
			break;
		}

		return dwRet;
	}
	u32 OnIdleTimer()
	{
		if (!m_cSsOpQueue.empty() && GetCurStateHoldTime() > CMU_SS_PU_STATUS_TIMEOUT)
		{
			TASKLOG(CMU_MOD_SS, WARNING_LEV, "�ȴ���¼cmu[%s]��ʱ�����³��Դ�������\n", 
				m_pcConnectTask->GetDstUri().GetURIString().c_str(), m_cSsOpQueue.front().tSsOpType.c_str());

			ProcSsQueue();
		}

		return TIMERPOLL_DONE;
	}

	u32 OnWaitSs(CMessage *const pcMsg)
	{
		u32 dwRet = PROCMSG_OK;

		switch (pcMsg->event)
		{
		case OSP_SIP_MSG_PROC_FAIL:
			{
				COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;

				TASKLOG(CMU_MOD_SS, WARNING_LEV, "���Ͷ�������ʧ�ܣ��ȴ���ʱ���ԣ�SipErrorCode[%d]\n", pcOspSipMsg->GetSipErrorCode());
			}
			break;
		case OSP_SIP_DIALOG_TERMINATE:
			{
				Reload();
			}
			break;
			/*
			 * �����¼�cmu����ʱ����Ӧ������cmu����Task�ᷢ��CMU_CMU_DISCONNECT��Ϣ������
			 */
		case CMU_CMU_DISCONNECT:
			{
				Reload();
			}
			break;
			/*
			 * ���¼���ɾ��ʱ����Ӧ������cmu����Task����ɾ�����ʶ����յ�CMU_CMU_DELETE��Ϣ��������Task��CCmuConnecterTask����ɾ����
			 */
		case CMU_CMU_DELETE:
			{
				COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;

				TASKLOG(CMU_MOD_SS, WARNING_LEV, "����cmu����Task������ɾ��������������Task�����˳���CmuUri[%s]\n", 
					pcOspSipMsg->GetMsgBody());
			}
			break;
		default:
			{
				COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
				CMS_ASSERT(NULL != pcOspSipMsg);

				switch (pcOspSipMsg->GetSipEventID())
				{
				case KDSIP_EVENT_SUBSCRIBE_RSP:												// ���Ļ�ˢ�¶���Ӧ��
					{
						CMS_ASSERT(!m_cSsOpQueue.empty() && "m_cSsOpQueue.empty()!!!");

						COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

						bool bSsRsp = false;												// ��־��Ӧ���Ƕ���Ӧ����ˢ�¶���Ӧ��
						if (INVALID_DIALOG_ID == m_tSsDlgId)
						{
							m_tSsDlgId = pcOspSipMsg->GetSipDlgID();						// �����sip�ỰID
							bSsRsp     = true;
						}

						CEventRsp cRsp;
						pcOspSipMsg->GetMsgBody(cRsp);

						/*
						 * Ŀǰ�������ǣ�������Ĳ����ɹ���������ʧ���б�����˴β������������ɹ��Ĳ��ֵļ���m_cSsInfoMap����������
						 *               ������Ĳ���ʧ�ܣ������´���
						 */
						if (CMS_SUCCESS == cRsp.GetErrorCode())
						{
							set<CSsUnit<T> > cErrorList;
							if (bSsRsp)
							{
								ProcSsRsp(m_cSsOpQueue.front(), pcMsg, cErrorList);
							}
							else
							{
								ProcRefreshSsRsp(m_cSsOpQueue.front(), pcMsg, cErrorList);
							}

							// �˴β������������������m_cSsInfoMap
							FinishOpItem(cErrorList);

							ProcSsQueue();
						}
						else
						{
							TASKLOG(CMU_MOD_SS, WARNING_LEV, "�յ�����ʧ��Ӧ�𣬵ȴ���ʱ���ԣ�������[%d]\n", cRsp.GetErrorCode());

							if (bSsRsp)															//����ʧ����Ҫ�ͷ���Դ
							{
								OspSipReleaseDlg(m_tSsDlgId);
								m_tSsDlgId = INVALID_DIALOG_ID;
							}
						}
					}
					break;
				case KDSIP_EVENT_NOTIFY_REQ:													// ����֪ͨ����
					{
						ProcNotify(pcMsg);
					}
					break;
				default:
					{
						TASKLOG(CMU_MOD_SS, WARNING_LEV, "Receive unknown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
						dwRet = PROCMSG_FAIL;
					}
					break;
				}
			}
			break;
		}

		return dwRet;
	}
	u32 OnWaitSsTimer()
	{
		if (GetCurStateHoldTime() > CMU_SS_PU_STATUS_TIMEOUT)
		{
			TASKLOG(CMU_MOD_SS, WARNING_LEV, "���Ĳ���[%s]��ʱ�����´��������\n", m_cSsOpQueue.front().tSsOpType.c_str());

			ProcSsQueue();
		}

		return TIMERPOLL_DONE;
	}

	u32 OnService(CMessage *const pcMsg)
	{
		u32 dwRet = PROCMSG_OK;

		switch (pcMsg->event)
		{
		case OSP_SIP_DIALOG_TERMINATE:
			{
				Reload();
			}
			break;
			/*
			 * �����¼�cmu����ʱ����Ӧ������cmu����Task�ᷢ��CMU_CMU_DISCONNECT��Ϣ������
			 */
		case CMU_CMU_DISCONNECT:
			{
				Reload();
			}
			break;
			/*
			 * ���¼���ɾ��ʱ����Ӧ������cmu����Task����ɾ�����ʶ����յ�CMU_CMU_DELETE��Ϣ��������Task��CCmuConnecterTask����ɾ����
			 */
		case CMU_CMU_DELETE:
			{
				COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;

				TASKLOG(CMU_MOD_SS, WARNING_LEV, "����cmu����Task������ɾ��������������Task�����˳���CmuUri[%s]\n", 
					pcOspSipMsg->GetMsgBody());
			}
			break;
		default:
			{
				COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
				CMS_ASSERT(NULL != pcOspSipMsg);

				switch (pcOspSipMsg->GetSipEventID())
				{
					case KDSIP_EVENT_NOTIFY_REQ:												// ����֪ͨ����
					{
						ProcNotify(pcMsg);
					}
				   break;
				default:
					{
						TASKLOG(CMU_MOD_SS, WARNING_LEV, "Receive unknown Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);

						dwRet = PROCMSG_FAIL;
					}
				   break;
				}
			}
			break;
		}

		return dwRet;
	}
	u32 OnServiceTimer()
	{
		return TIMERPOLL_DONE;
	}

	virtual bool IsNotNeedPollState()
	{
		return (Service == GetState());
	}

public:
	// ��Ҫ�Ķ���ӿ�
    void AddSs(const T &cObject, const TSsTypes &tSsTypes)
    {
        set<CSsUnit<T> > cSsUnits;
        for (TSsTypes::const_iterator it = tSsTypes.begin(); tSsTypes.end() != it; ++it)
        {
            cSsUnits.insert(CSsUnit<string>(cObject, *it));
        }

        AddSs(cSsUnits);
    }
    void AddSs(const set<CSsUnit<T> > &cSsUnits)
    {
        TSsOpInfo<T> tOpInfo;
        tOpInfo.tSsOpType = SS_OP_TYPE_ADD;
        tOpInfo.cSsList   = cSsUnits;

        PostSsOpReq(tOpInfo);
    }

    void DelSs(const T &cObject, const TSsTypes &tSsTypes)
    {
        set<CSsUnit<T> > cSsUnits;
        for (TSsTypes::const_iterator it = tSsTypes.begin(); tSsTypes.end() != it; ++it)
        {
            cSsUnits.insert(CSsUnit<string>(cObject, *it));
        }

        DelSs(cSsUnits);
    }
    void DelSs(const vector<CSsUnit<T> > &cSsUnits)
    {
        TSsOpInfo<T> tOpInfo;
        tOpInfo.tSsOpType = SS_OP_TYPE_DEL;
        tOpInfo.cSsList   = cSsUnits;

        PostSsOpReq(tOpInfo);
    }

protected:

	void PostSsOpReq(const TSsOpInfo<T> &tOpInfo, bool bAutoDeal = true)
	{
        if (!tOpInfo.cSsList.empty())
        {
            if (tOpInfo.cSsList.size() > m_dwMaxSsUnitNumInSsOpInfo)            // ��������̫�࣬��Ҫ���
            {
                TSsOpInfo<T>  tSingleOp;
                tSingleOp.tSsOpType = tOpInfo.tSsOpType;

                u32 dwCnt = 0;
                for (typename set<CSsUnit<T> >::const_iterator it = tOpInfo.cSsList.begin(); 
                    tOpInfo.cSsList.end() != it; ++it)
                {
                    tSingleOp.cSsList.insert(*it);
                    if (0 == ++dwCnt % m_dwMaxSsUnitNumInSsOpInfo)
                    {
                        m_cSsOpQueue.push(tSingleOp);
                        tSingleOp.cSsList.clear();
                    }
                }

                if (!tSingleOp.cSsList.empty())				// ���һ��
                {
                    m_cSsOpQueue.push(tSingleOp);
                    tSingleOp.cSsList.clear();
                }
            }
            else
            {
                m_cSsOpQueue.push(tOpInfo);
            }

            u32 dwCurSize = m_cSsOpQueue.size();		    // �����ֵ
            if (dwCurSize > m_dwTopSsNum)
            {
                m_dwTopSsNum = dwCurSize;
            }

            u32 dwCurState = GetState();
            if (bAutoDeal && (Idle == dwCurState || Service == dwCurState))
            {
                ProcSsQueue();
            }
        }
	}

public:

	// ���˶��ĵ�Ԫ�б�
	void FilterSsOpInfo(TSsOpInfo<T> &tOpInfo)
	{
        const TSsOpType &tSsOpType = tOpInfo.tSsOpType;
        set<CSsUnit<T> > &cSsList  = tOpInfo.cSsList;

        CMS_ASSERT((SS_OP_TYPE_ADD == tSsOpType || SS_OP_TYPE_DEL == tSsOpType));

		if (SS_OP_TYPE_ADD == tSsOpType)
		{
			for (typename set<CSsUnit<T> >::iterator it = cSsList.begin(); cSsList.end() != it;)
			{
				typename set<CSsUnit<T> >::const_iterator it4Unit = m_cSsList.find(*it);
				if (m_cSsList.end() != it4Unit)		                            // ��SsUnit�Ѿ����Ĺ�
				{
                    cSsList.erase(it++);
				}
				else																// ��SsUnit��δ���Ĺ�����Ҫ��������
				{
					++it;
				}
			}
		}
		else
		{
            for (typename set<CSsUnit<T> >::iterator it = cSsList.begin(); cSsList.end() != it;)
			{
				typename set<CSsUnit<T> >::const_iterator it4Unit = m_cSsList.find(*it);
				if (m_cSsList.end() == it4Unit)
				{
					cSsList.erase(it++);
				}
				else				// ȥ����δ���ĵĶ��ĵ�Ԫ
				{
					++it;
				}
			}
		}
	}

	void ProcSsQueue()
	{
		if (CCmuConnecterTask::Service != m_pcConnectTask->GetState())
		{
			TASKLOG(CMU_MOD_SS, CRITICAL_LEV, "��δ��¼��ģ��[%s]����ͣ��������!\n", m_pcConnectTask->GetServerUri().c_str());

			return;
		}

		if (m_cSsOpQueue.empty())
		{
			TASKLOG(CMU_MOD_SS, TIMER_LEV, "�������Ĳ�������Ϊ�գ�ֹͣ��������\n");

			if (WaitSs == GetState())
			{
				NextState(Service);
			}

			return;
		}

		TSsOpInfo<T> &tOpInfo = m_cSsOpQueue.front();

		// �������ʱ���ȹ���һ�£�ȥ��û�б�Ҫ���ͳ�ȥ�Ķ��ĵ�Ԫ
		FilterSsOpInfo(tOpInfo);

        if (tOpInfo.cSsList.empty())
        {
            // ����������һ������
            m_cSsOpQueue.pop();
            ProcSsQueue();
            return;
        }

        const TSsOpType &tOpType  = tOpInfo.tSsOpType;
		u32 dwCurState = GetState();
		switch (dwCurState)
		{
		case Idle:			// ��ʼʱ����������
			{
				CMS_ASSERT(m_tSsDlgId == INVALID_DIALOG_ID && "m_tSsDlgId != INVALID_DIALOG_ID!!!");

				if (SS_OP_TYPE_ADD == tOpType)
				{
					PostReq(KDSIP_EVENT_SUBSCRIBE_REQ, GetSsReqBySsOpInfo(tOpInfo), m_pcConnectTask->GetDstUri());

					NextState(WaitSs);

					TASKLOG(CMU_MOD_SS, EVENT_LEV, "��ʼ���Ͷ�������\n");
				}
				else
				{
					TASKLOG(CMU_MOD_SS, ERROR_LEV, "��ʼ״̬���յ�ɾ��[%s]���Ĳ��������Ը�����!\n", tOpType.c_str());

					// ���Ե�ǰ���󣬼���������һ������
					m_cSsOpQueue.pop();
					ProcSsQueue();
				}			
			}
			break;
		case WaitSs:		// �յ����Ĳ���Ӧ���ʱ���������
			{
				if (INVALID_DIALOG_ID == m_tSsDlgId)			// ���Ĵ���
				{
					PostReq(KDSIP_EVENT_SUBSCRIBE_REQ, GetSsReqBySsOpInfo(tOpInfo), m_pcConnectTask->GetDstUri());
				}
				else											// ˢ�¶��Ĵ���
				{
					PostInDlgReq(KDSIP_EVENT_SUBSCRIBE_REQ, GetRefreshSsReqBySsOpInfo(tOpInfo), m_tSsDlgId);
				}

				NextState(WaitSs);
			}
			break;
		case Service:		// ����̬�´���ˢ�¶�������
			{
				CMS_ASSERT(m_tSsDlgId != INVALID_DIALOG_ID && "m_tSsDlgId == INVALID_DIALOG_ID!!!");

                PostInDlgReq(KDSIP_EVENT_SUBSCRIBE_REQ, GetRefreshSsReqBySsOpInfo(tOpInfo), m_tSsDlgId);

                NextState(WaitSs);

                TASKLOG(CMU_MOD_SS, EVENT_LEV, "��ʼ����ˢ�¶�������\n");
			}
			break;
		default:
			{
				CMS_ASSERT(false && "Task״̬�쳣!!!");
			}
			break;
		}
	}

	void FinishOpItem(const set<CSsUnit<T> > &cErrorList)
	{
        TSsOpInfo<T> &tOpInfo           = m_cSsOpQueue.front();
        const TSsType &tOpType          = tOpInfo.tSsOpType;
		const set<CSsUnit<T> > &tSsList = tOpInfo.cSsList;

		for (typename set<CSsUnit<T> >::const_iterator it = tSsList.begin(); tSsList.end() != it; ++it)
		{
			typename set<CSsUnit<T> >::const_iterator it4Unit = cErrorList.find(*it);
			if (cErrorList.end() == it4Unit)					    // ���Ĳ����ɹ�
			{
				if (SS_OP_TYPE_ADD == tOpType)
				{
					m_cSsList.insert(*it);
				}
				else
				{
					m_cSsList.erase(*it);
				}
			}
		}

		// �����������������Ԫ�س�����
		m_cSsOpQueue.pop();
	}

	void Reload()
	{
        // 1������ԭDialog��Դ
		if (INVALID_DIALOG_ID != m_tSsDlgId)
		{
			OspSipReleaseDlg(m_tSsDlgId);
			m_tSsDlgId = INVALID_DIALOG_ID;
		}

		// 2�����������Ŷӵ�Ԫ�ز����ԭ����
		vector<TSsOpInfo<T> > cTmpSsOpQueue;
		while (!m_cSsOpQueue.empty())
		{
			cTmpSsOpQueue.push_back(m_cSsOpQueue.front());
			m_cSsOpQueue.pop();
		}

        // 3���Ƚ�ԭ���ĳɹ��ļ��붩�Ķ��в�����Ѷ����б�
        TSsOpInfo<T> tOpInfo;
        tOpInfo.tSsOpType = SS_OP_TYPE_ADD;
        tOpInfo.cSsList   = m_cSsList;
        PostSsOpReq(tOpInfo, false);
		m_cSsList.clear();

		// 4����֮ǰ�Ŷӵ�Ԫ�غϲ�����
		for (typename vector<TSsOpInfo<T> >::const_iterator it = cTmpSsOpQueue.begin(); cTmpSsOpQueue.end() != it; ++it)
		{
			m_cSsOpQueue.push(*it);
		}

        NextState(Idle);

		ProcSsQueue();
	}

    virtual string  SsUnit2String(const CSsUnit<T> &tUnit) const
    {
        ostringstream oss;
        oss << tUnit.GetObject() << " " << tUnit.GetSsType();
        return oss.str();
    }

protected:

    // ��������Ҫʵ�ֵĴ��麯��
    virtual TSsMode GetSsMode() const = 0;
	virtual string  GetSsReqBySsOpInfo(const TSsOpInfo<T> &tOpInfo) = 0;
	virtual string  GetRefreshSsReqBySsOpInfo(const TSsOpInfo<T> &tOpInfo) = 0;
    virtual void    ProcSsRsp(const TSsOpInfo<T> &tOpInfo, const CMessage *const pcMsg, set<CSsUnit<T> > &cSucceedList) = 0;
	virtual void    ProcRefreshSsRsp(const TSsOpInfo<T> &tOpInfo, const CMessage *const pcMsg, set<CSsUnit<T> > &cSucceedList) = 0;
    virtual void    ProcNotifyDevNotExist(const TSsType &tSsType, const CEventReq *pcNtfReq) { };
	
    void ProcNotify(CMessage *const pcMsg)
    {
        COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
        CMS_ASSERT(NULL != pcOspSipMsg);

        switch (pcMsg->event)
        {
        case PU_STATUS_NTF_REQ:
            {
                CPuStatusNtyReq cNtfReq;
                pcOspSipMsg->GetMsgBody(cNtfReq);

                CPuStatusNtyRsp cNtfRsp;
                cNtfRsp.SetEvent(PU_STATUS_NTF_RSP);
                cNtfRsp.SetHeadFromReq(cNtfReq);
                PostRsp(KDSIP_EVENT_NOTIFY_RSP, pcOspSipMsg->GetSipTransID(), cNtfRsp);

                PuStatusNtf(cNtfReq);
            }
            break;
        case PU_CONFIG_NTF_REQ:
            {
                CPuConfigNtyReq cNtfReq;
                pcOspSipMsg->GetMsgBody(cNtfReq);

                CPuConfigNtyRsp cNtfRsp;
                cNtfRsp.SetEvent(PU_CONFIG_NTF_RSP);
                cNtfRsp.SetHeadFromReq(cNtfReq);
                PostRsp(KDSIP_EVENT_NOTIFY_RSP, pcOspSipMsg->GetSipTransID(), cNtfRsp);

                PuConfigNtf(cNtfReq);
            }
            break;
        case PU_SERVICE_STATUS_NTF_REQ:
            {
                CPuServiceStatusNtyReq cNtfReq;
                pcOspSipMsg->GetMsgBody(cNtfReq);

                CPuServiceStatusNtyRsp cNtfRsp;
                cNtfRsp.SetEvent(PU_SERVICE_STATUS_NTF_RSP);
                cNtfRsp.SetHeadFromReq(cNtfReq);
                PostRsp(KDSIP_EVENT_NOTIFY_RSP, pcOspSipMsg->GetSipTransID(), cNtfRsp);

                PuServiceNtf(cNtfReq);
            }
            break;
        case PU_ALARM_STATUS_NTF_REQ:
            {
                CPuAlarmStatusNtyReq cNtfReq;
                pcOspSipMsg->GetMsgBody(cNtfReq);

                CPuAlarmStatusNtyRsp cNtfRsp;
                cNtfRsp.SetEvent(PU_ALARM_STATUS_NTF_RSP);
                cNtfRsp.SetHeadFromReq(cNtfReq);
                PostRsp(KDSIP_EVENT_NOTIFY_RSP, pcOspSipMsg->GetSipTransID(), cNtfRsp);

                PuAlarmNtf(cNtfReq);
            }
            break;
        case PU_GPS_STATUS_NTF_REQ:
            {
                CPuGpsStatusNtyReq cNtfReq;
                pcOspSipMsg->GetMsgBody(cNtfReq);

                CPuGpsStatusNtyRsp cNtfRsp;
                cNtfRsp.SetEvent(PU_GPS_STATUS_NTF_RSP);
                cNtfRsp.SetHeadFromReq(cNtfReq);
                PostRsp(KDSIP_EVENT_NOTIFY_RSP, pcOspSipMsg->GetSipTransID(), cNtfRsp);

                PuGpsNtf(cNtfReq);
            }
            break;
        case PU_TRANSDATA_STATUS_NTF_REQ:
            {
                CPuTransDataStatusNtyReq cNtfReq;
                pcOspSipMsg->GetMsgBody(cNtfReq);

                CPuTransDataStatusNtyRsp cNtfRsp;
                cNtfRsp.SetEvent(PU_TRANSDATA_STATUS_NTF_RSP);
                cNtfRsp.SetHeadFromReq(cNtfReq);
                PostRsp(KDSIP_EVENT_NOTIFY_RSP, pcOspSipMsg->GetSipTransID(), cNtfRsp);

                PuTransDataNtf(cNtfReq);
            }
            break;
        case PU_BATCH_NTF_REQ:
            {
                CPuBatchNtyReq cNtfReq;
                pcOspSipMsg->GetMsgBody(cNtfReq);

                CPuBatchNtyRsp cNtfRsp;
                cNtfRsp.SetHeadFromReq(cNtfReq);
                PostRsp(KDSIP_EVENT_NOTIFY_RSP, pcOspSipMsg->GetSipTransID(), cNtfRsp);

                const vector<TGeneralData> &vctData = cNtfReq.GetData();
                for (vector<TGeneralData>::const_iterator it = vctData.begin(); vctData.end() != it; ++it)
                {
                    TASKLOG(CMU_MOD_SS, TIMER_LEV, "batchNtf[%s]\n", it->content.c_str());

                    if (PU_ON_OFF_STATUS == it->type)
                    {
                        CPuStatusNtyReq cReq;
                        cReq.FromXml(it->content);

                        PuStatusNtf(cReq);
                    }
                    else if (PU_CONFIG_STATUS == it->type)
                    {
                        CPuConfigNtyReq cReq;
                        cReq.FromXml(it->content);

                        PuConfigNtf(cReq);
                    }
                    else if (PU_SERVICE_STATUS == it->type)
                    {
                        CPuServiceStatusNtyReq cReq;
                        cReq.FromXml(it->content);

                        PuServiceNtf(cReq);
                    }
                    else if (PU_ALARM_STATUS == it->type)
                    {
                        CPuAlarmStatusNtyReq cReq;
                        cReq.FromXml(it->content);

                        PuAlarmNtf(cReq);
                    }
                    else if (PU_GPS_STATUS == it->type)
                    {
                        CPuGpsStatusNtyReq cReq;
                        cReq.FromXml(it->content);

                        PuGpsNtf(cReq);
                    }
                    else if (PU_TRANSDATA_STATUS == it->type)
                    {
                        CPuTransDataStatusNtyReq cReq;
                        cReq.FromXml(it->content);

                        PuTransDataNtf(cReq);
                    }
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

    void PuStatusNtf(CPuStatusNtyReq &cReq)
    {
        CPuData *pcPuData = GetPuDataByUri(cReq.GetDevURI());
        if (NULL != pcPuData)
        {
            pcPuData->SetPuStatus(cReq.GetOnOffStatus());

            pcPuData->PublishOnOff(cReq.GetOnOffStatus());
        }
        else
        {
            ProcNotifyDevNotExist(PU_ON_OFF_STATUS, &cReq);
        }
    }

    void PuConfigNtf(CPuConfigNtyReq &cReq)
    {
        CPuData *pcPuData = GetPuDataByUri(cReq.GetDevURI());
        if (NULL != pcPuData)
        {
            pcPuData->MergePuConfig(cReq.GetConfig());

            pcPuData->PublishConfig(cReq.GetConfig());
        }
        else
        {
            ProcNotifyDevNotExist(PU_CONFIG_STATUS, &cReq);
        }
    }

    void PuServiceNtf(CPuServiceStatusNtyReq &cReq)
    {
        CPuData *pcPuData = GetPuDataByUri(cReq.GetDevURI());
        if (NULL != pcPuData)
        {
            pcPuData->SetPuServiceStatus(cReq.GetServiceStatus());

            pcPuData->PublishService(cReq.GetServiceStatus());
        }
        else
        {
            ProcNotifyDevNotExist(PU_SERVICE_STATUS, &cReq);
        }
    }

    void PuAlarmNtf(CPuAlarmStatusNtyReq &cReq)
    {
        CPuData *pcPuData = GetPuDataByUri(cReq.GetDevURI());
        if (NULL != pcPuData)
        {
            pcPuData->UpdateAlarmStatus(cReq.GetAlarmStatus());

            pcPuData->PublishAlarm(cReq.GetAlarmStatus());
        }
        else
        {
            ProcNotifyDevNotExist(PU_ALARM_STATUS, &cReq);
        }
    }

    void PuGpsNtf(CPuGpsStatusNtyReq &cReq)
    {
        CPuData *pcPuData = GetPuDataByUri(cReq.GetDevURI());
        if (NULL != pcPuData)
        {
            pcPuData->SetPuGpsData(cReq.GetGpsData());

            pcPuData->PublishGpsData(cReq.GetGpsData());
        }
        else
        {
            ProcNotifyDevNotExist(PU_GPS_STATUS, &cReq);
        }
    }

    void PuTransDataNtf(CPuTransDataStatusNtyReq &cReq)
    {
        CPuData *pcPuData = GetPuDataByUri(cReq.GetDevURI());
        if (NULL != pcPuData)
        {
            pcPuData->SetPuTransparentData(cReq.GetData());

            pcPuData->PublishTransData(cReq.GetData());
        }
        else
        {
            ProcNotifyDevNotExist(PU_TRANSDATA_STATUS, &cReq);
        }
    }

	CLoginSession *GetConnectTask()
	{
		return m_pcConnectTask;
	}

	bool IsCertainUnitSSed(const T &tUnit) const
	{
		return (m_cSsList.end() != m_cSsList.find(tUnit));
	}

	// ��ȡָ�����ĵ�Ԫ�����ж�������Ϣ
	void GetSsersBySsUnit(const T &tUnit, set<u32> &cTasks) const
	{
		typename map<T, set<u32> >::const_iterator it = m_cSsList.find(tUnit);
		if (m_cSsList.end() != it)
		{
			cTasks.insert(it->second.begin(), it->second.end());
		}
	}

	// ��ȡָ�������ߵ����ж��ĵ�Ԫ��Ϣ
	void GetSsUnitListBySser(u32 dwTaskNO, set<T> &tSsUnitList)
	{
		for (typename map<T, set<u32> >::const_iterator it = m_cSsList.begin(); m_cSsList.end() != it; ++it)
		{
			set<u32>::const_iterator it4Task = it->second.find(dwTaskNO);
			if (it->second.end() != it4Task)
			{
				tSsUnitList.insert(it->first);
			}
		}
	}

    const string &GetDomainName() const 
    {
        return m_strDomainName;
    }

private:
    KDSIP_DIALOG_ID            m_tSsDlgId;						    // ���ĶԻ�SIP ID����COspSipMsg�л�ȡ
    string                     m_strDomainName;                     // �����ĵ�DomainName

    set<CSsUnit<T> >           m_cSsList;						    // ���ĳɹ��󱣴����Ϣ
	CCmuConnecterTask         *m_pcConnectTask;					    // ��¼����cmu��Taskָ��
	queue<TSsOpInfo<T> >	   m_cSsOpQueue;						// ���Ĳ��������/ɾ�����Ķ��У��������Ѿ�����sip��Ϣ����С���зְ�

	u32						   m_dwTopSsNum;						// ���Ĳ��������еķ�ֵ

	static const u32           m_dwMaxSsUnitNumInSsOpInfo = 100;	// ÿһ������������Я���Ķ��ĵ�Ԫ�������������ʵ�ּ򵥲��
};


#endif  //#ifndef _DOMAIN_SS_BASE_TASK_TMPL_H

