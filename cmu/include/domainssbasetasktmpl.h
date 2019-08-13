/*========================================================================================
模块名    ：cmu
文件名    ：domainssbasetasktmpl.h
相关文件  ：
实现功能  ：跨域列表订阅、跨域目录订阅的基类，提供排队式的刷新订阅机制
作者      ：xiongxh
版权      ：<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2014/05/27         V1.0              xiongxh                                新建文件
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

	TSsOpType        tSsOpType;					// 订阅操作类型，SS_OP_TYPE_ADD/SS_OP_TYPE_DEL
	set<CSsUnit<T> > cSsList;				    // 订阅单元集合
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
			// 域订阅Task作为订阅的请求方，这里调用OspSipReleaseDlg释放本地Sip资源，
			// 同时对端底层收到Terminate类型的Notify，业务层收到OSP_SIP_DIALOG_TERMINATE
			OspSipReleaseDlg(m_tSsDlgId);
			m_tSsDlgId = INVALID_DIALOG_ID;
		}
	}

public:
	//状态处理
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
			TASKLOG(CMU_MOD_SS, WARNING_LEV, "等待登录cmu[%s]超时，重新尝试处理请求\n", 
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

				TASKLOG(CMU_MOD_SS, WARNING_LEV, "发送订阅请求失败，等待超时重试，SipErrorCode[%d]\n", pcOspSipMsg->GetSipErrorCode());
			}
			break;
		case OSP_SIP_DIALOG_TERMINATE:
			{
				Reload();
			}
			break;
			/*
			 * 当和下级cmu断链时，对应的异域cmu连接Task会发出CMU_CMU_DISCONNECT消息后重连
			 */
		case CMU_CMU_DISCONNECT:
			{
				Reload();
			}
			break;
			/*
			 * 当下级域被删除时，对应的异域cmu连接Task将被删除，故而会收到CMU_CMU_DELETE消息（异域订阅Task由CCmuConnecterTask负责删除）
			 */
		case CMU_CMU_DELETE:
			{
				COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;

				TASKLOG(CMU_MOD_SS, WARNING_LEV, "异域cmu连接Task由于域删除被析构，域订阅Task即将退出，CmuUri[%s]\n", 
					pcOspSipMsg->GetMsgBody());
			}
			break;
		default:
			{
				COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
				CMS_ASSERT(NULL != pcOspSipMsg);

				switch (pcOspSipMsg->GetSipEventID())
				{
				case KDSIP_EVENT_SUBSCRIBE_RSP:												// 订阅或刷新订阅应答
					{
						CMS_ASSERT(!m_cSsOpQueue.empty() && "m_cSsOpQueue.empty()!!!");

						COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

						bool bSsRsp = false;												// 标志该应答是订阅应答还是刷新订阅应答
						if (INVALID_DIALOG_ID == m_tSsDlgId)
						{
							m_tSsDlgId = pcOspSipMsg->GetSipDlgID();						// 保存好sip会话ID
							bSsRsp     = true;
						}

						CEventRsp cRsp;
						pcOspSipMsg->GetMsgBody(cRsp);

						/*
						 * 目前的做法是：如果订阅操作成功（可能有失败列表），则此次操作结束，将成功的部分的加入m_cSsInfoMap保存起来，
						 *               如果订阅操作失败，则将重新处理
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

							// 此次操作结束，将结果存入m_cSsInfoMap
							FinishOpItem(cErrorList);

							ProcSsQueue();
						}
						else
						{
							TASKLOG(CMU_MOD_SS, WARNING_LEV, "收到订阅失败应答，等待超时重试，错误码[%d]\n", cRsp.GetErrorCode());

							if (bSsRsp)															//订阅失败需要释放资源
							{
								OspSipReleaseDlg(m_tSsDlgId);
								m_tSsDlgId = INVALID_DIALOG_ID;
							}
						}
					}
					break;
				case KDSIP_EVENT_NOTIFY_REQ:													// 订阅通知处理
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
			TASKLOG(CMU_MOD_SS, WARNING_LEV, "订阅操作[%s]超时，重新处理该请求\n", m_cSsOpQueue.front().tSsOpType.c_str());

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
			 * 当和下级cmu断链时，对应的异域cmu连接Task会发出CMU_CMU_DISCONNECT消息后重连
			 */
		case CMU_CMU_DISCONNECT:
			{
				Reload();
			}
			break;
			/*
			 * 当下级域被删除时，对应的异域cmu连接Task将被删除，故而会收到CMU_CMU_DELETE消息（异域订阅Task由CCmuConnecterTask负责删除）
			 */
		case CMU_CMU_DELETE:
			{
				COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;

				TASKLOG(CMU_MOD_SS, WARNING_LEV, "异域cmu连接Task由于域删除被析构，域订阅Task即将退出，CmuUri[%s]\n", 
					pcOspSipMsg->GetMsgBody());
			}
			break;
		default:
			{
				COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
				CMS_ASSERT(NULL != pcOspSipMsg);

				switch (pcOspSipMsg->GetSipEventID())
				{
					case KDSIP_EVENT_NOTIFY_REQ:												// 订阅通知处理
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
	// 主要的对外接口
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
            if (tOpInfo.cSsList.size() > m_dwMaxSsUnitNumInSsOpInfo)            // 单次请求太多，需要拆包
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

                if (!tSingleOp.cSsList.empty())				// 最后一组
                {
                    m_cSsOpQueue.push(tSingleOp);
                    tSingleOp.cSsList.clear();
                }
            }
            else
            {
                m_cSsOpQueue.push(tOpInfo);
            }

            u32 dwCurSize = m_cSsOpQueue.size();		    // 计算峰值
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

	// 过滤订阅单元列表
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
				if (m_cSsList.end() != it4Unit)		                            // 该SsUnit已经订阅过
				{
                    cSsList.erase(it++);
				}
				else																// 该SsUnit尚未订阅过，需要保留下来
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
				else				// 去除尚未订阅的订阅单元
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
			TASKLOG(CMU_MOD_SS, CRITICAL_LEV, "尚未登录到模块[%s]，暂停处理请求!\n", m_pcConnectTask->GetServerUri().c_str());

			return;
		}

		if (m_cSsOpQueue.empty())
		{
			TASKLOG(CMU_MOD_SS, TIMER_LEV, "待处理订阅操作队列为空，停止处理请求\n");

			if (WaitSs == GetState())
			{
				NextState(Service);
			}

			return;
		}

		TSsOpInfo<T> &tOpInfo = m_cSsOpQueue.front();

		// 处理队列时，先过滤一下，去除没有必要发送出去的订阅单元
		FilterSsOpInfo(tOpInfo);

        if (tOpInfo.cSsList.empty())
        {
            // 继续处理下一条请求
            m_cSsOpQueue.pop();
            ProcSsQueue();
            return;
        }

        const TSsOpType &tOpType  = tOpInfo.tSsOpType;
		u32 dwCurState = GetState();
		switch (dwCurState)
		{
		case Idle:			// 初始时处理订阅请求
			{
				CMS_ASSERT(m_tSsDlgId == INVALID_DIALOG_ID && "m_tSsDlgId != INVALID_DIALOG_ID!!!");

				if (SS_OP_TYPE_ADD == tOpType)
				{
					PostReq(KDSIP_EVENT_SUBSCRIBE_REQ, GetSsReqBySsOpInfo(tOpInfo), m_pcConnectTask->GetDstUri());

					NextState(WaitSs);

					TASKLOG(CMU_MOD_SS, EVENT_LEV, "开始发送订阅请求\n");
				}
				else
				{
					TASKLOG(CMU_MOD_SS, ERROR_LEV, "初始状态下收到删除[%s]订阅操作，忽略该请求!\n", tOpType.c_str());

					// 忽略当前请求，继续处理下一条请求
					m_cSsOpQueue.pop();
					ProcSsQueue();
				}			
			}
			break;
		case WaitSs:		// 收到订阅操作应答或超时后继续处理
			{
				if (INVALID_DIALOG_ID == m_tSsDlgId)			// 订阅处理
				{
					PostReq(KDSIP_EVENT_SUBSCRIBE_REQ, GetSsReqBySsOpInfo(tOpInfo), m_pcConnectTask->GetDstUri());
				}
				else											// 刷新订阅处理
				{
					PostInDlgReq(KDSIP_EVENT_SUBSCRIBE_REQ, GetRefreshSsReqBySsOpInfo(tOpInfo), m_tSsDlgId);
				}

				NextState(WaitSs);
			}
			break;
		case Service:		// 服务态下处理刷新订阅请求
			{
				CMS_ASSERT(m_tSsDlgId != INVALID_DIALOG_ID && "m_tSsDlgId == INVALID_DIALOG_ID!!!");

                PostInDlgReq(KDSIP_EVENT_SUBSCRIBE_REQ, GetRefreshSsReqBySsOpInfo(tOpInfo), m_tSsDlgId);

                NextState(WaitSs);

                TASKLOG(CMU_MOD_SS, EVENT_LEV, "开始发送刷新订阅请求\n");
			}
			break;
		default:
			{
				CMS_ASSERT(false && "Task状态异常!!!");
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
			if (cErrorList.end() == it4Unit)					    // 订阅操作成功
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

		// 处理结束，将处理后的元素出队列
		m_cSsOpQueue.pop();
	}

	void Reload()
	{
        // 1、清理原Dialog资源
		if (INVALID_DIALOG_ID != m_tSsDlgId)
		{
			OspSipReleaseDlg(m_tSsDlgId);
			m_tSsDlgId = INVALID_DIALOG_ID;
		}

		// 2、备份正在排队的元素并清空原队列
		vector<TSsOpInfo<T> > cTmpSsOpQueue;
		while (!m_cSsOpQueue.empty())
		{
			cTmpSsOpQueue.push_back(m_cSsOpQueue.front());
			m_cSsOpQueue.pop();
		}

        // 3、先将原订阅成功的加入订阅队列并清空已订阅列表
        TSsOpInfo<T> tOpInfo;
        tOpInfo.tSsOpType = SS_OP_TYPE_ADD;
        tOpInfo.cSsList   = m_cSsList;
        PostSsOpReq(tOpInfo, false);
		m_cSsList.clear();

		// 4、将之前排队的元素合并进来
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

    // 子类中需要实现的纯虚函数
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

	// 获取指定订阅单元的所有订阅者信息
	void GetSsersBySsUnit(const T &tUnit, set<u32> &cTasks) const
	{
		typename map<T, set<u32> >::const_iterator it = m_cSsList.find(tUnit);
		if (m_cSsList.end() != it)
		{
			cTasks.insert(it->second.begin(), it->second.end());
		}
	}

	// 获取指定订阅者的所有订阅单元信息
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
    KDSIP_DIALOG_ID            m_tSsDlgId;						    // 订阅对话SIP ID，从COspSipMsg中获取
    string                     m_strDomainName;                     // 所订阅的DomainName

    set<CSsUnit<T> >           m_cSsList;						    // 订阅成功后保存的信息
	CCmuConnecterTask         *m_pcConnectTask;					    // 登录异域cmu的Task指针
	queue<TSsOpInfo<T> >	   m_cSsOpQueue;						// 订阅操作（添加/删除）的队列，队列中已经根据sip消息包大小进行分包

	u32						   m_dwTopSsNum;						// 订阅操作队列中的峰值

	static const u32           m_dwMaxSsUnitNumInSsOpInfo = 100;	// 每一个请求中所能携带的订阅单元的最大数量，以实现简单拆包
};


#endif  //#ifndef _DOMAIN_SS_BASE_TASK_TMPL_H

