/*
从TAS查询分组信息，再到DPS中校验分组下是否有设备
*/
#ifndef _DEV_GROUP_QUERY_TASK_H_
#define _DEV_GROUP_QUERY_TASK_H_

#include "cms/ospext/ospext.h"
#include "cms/ospsip/ospsip.h"
#include "cuidata.h"
#include "cms/commonconfig/cmcfg.h"
#include "culogintask.h"
#include "cms/utility/digital_string.h"

class CDevGroupQueryTask : public CSipTask
{
public:
    enum
    {
        NOT_QUERY_STATE = UNKNOWN_STATE + 1,        // 等待向3A发起查询
        WAIT_RESPONSE_STATE,                        // 等待查询Rsp
        WAIT_NOTIFY_STATE,                          // 等待查询Ntf
		WAIT_DPS_RSP_STATE                            // 等待DPS应答
    };
    enum EServerType
    {
        SERVER_3AS = 0,     //查询的目标服务器是3AS
        SERVER_CMU,
        SERVER_RCS,
        SERVER_TVWALL,
        SERVER_TYPE_NUM
    };
public:
    CDevGroupQueryTask(CInstExt *pcInst, CCuAgent *pcCuAgent, PTRecvNode ptRecvNode);
    ~CDevGroupQueryTask();

public:
    //子类需要重写的方法
    virtual const char* GetObjName() const //必须重载的方法
    {
        return "CDevGroupQueryTask";
    }
    virtual const char* GetStateName(u32 dwState) const //必须重载的方法
    {
        switch (dwState)
        {
        case NOT_QUERY_STATE:
            {
                return "Not Query State";
            }
            break;
        case WAIT_RESPONSE_STATE:
            {
                return "Wait Response State";
            }
            break;
        case WAIT_NOTIFY_STATE:
            {
                return "Wait Notify State";
            }
            break;
		case WAIT_DPS_RSP_STATE:
			{
				return "Wait DPS Rsp State";
			}
			break;
        default:
            break;
        }
        return "Unknown State";
    }
    void TerminateTask();
    virtual void InitStateMachine();
public:
    template <typename CReq>
    u32 FakePostMsgReq(CReq &cReq, const TSipURI &tDstURI)
    {
        TASKLOG(NORMAL_TASK, EVENT_LEV, "CDevGroupQueryTask[TaskNO %u]发送消息[%s--%d]给服务器[%s]\n", 
            GetTaskNO(), cReq.GetEventStr().c_str(), cReq.GetEventId(), tDstURI.GetURIString().c_str());

#ifdef _UNIT_TEST_
        g_cTaskNOQue.push(GetTaskNO());
        cout << "Post To " << tDstURI.GetURIString().c_str() << endl;
        cout << cReq.ToXml().c_str() << endl;
        return PROCMSG_OK;
#else
        return PostMsgReq<CReq>(cReq, tDstURI);
#endif
    }
    template <typename CReq>
    u32 PostMsgReqToCMU(CReq &cReq)
    {
        const TSipURI &tDstURI = m_pcCuAgent->m_tCmuURI;
        return FakePostMsgReq(cReq, tDstURI);
    }
    template <typename CReq>
    u32 PostMsgReqTo3AS(CReq &cReq)
    {
        if ( m_pcCuAgent->IsTasOnline() )
        {
            C3acAgent *pc3acAgent = (C3acAgent *)g_cCuiDataMgr.Get3asAgentTaskPtrBy3asName(m_pcCuAgent->m_str3asName, GetInstance());
            CMS_ASSERT( NULL != pc3acAgent );
            CMS_ASSERT( 0 == strcmp(pc3acAgent->GetObjName(), "C3acAgent") );
            if(pc3acAgent == NULL)
            {
                return PROCMSG_DEL;
            }
            const TSipURI &tDstURI = pc3acAgent->m_t3asURI;
            FakePostMsgReq(cReq, tDstURI);
            m_pc3acAgent = pc3acAgent;
        }
        else
        {
            return PROCMSG_DEL;
        }
        return PROCMSG_OK;
    }
public:
    //状态函数
    u32 NotQueryProcMsg(CMessage *const pcMsg);
    u32 WaitRspProcMsg(CMessage *const pcMsg);
    u32 WaitRspTimerPoll();
    u32 WaitNotifyProcMsg(CMessage *const pcMsg);
    u32 ProcNotifyMsg(CMessage *const pcMsg);
	u32 WaitNotifyTimerPoll();
	// 如果TAS成功取到消息，不影响CU取结果，因此超时、发送失败都返回给CU成功
	u32 WaitDpsRspProcMsg(CMessage *const pcMsg);
	u32 WaitDpsRspTimerPoll();

    template<typename CReqToSvr, typename CRspToCu>
    u32 OnNotQueryMsg(CReqToSvr &cReq, EServerType eSendToDst = SERVER_3AS)
    {
        CMS_ASSERT(GetState() == NOT_QUERY_STATE);
        //缓存req消息，如果在wait rsp state收到ntf消息好处理
        m_dwReqEvent = cReq.GetEventId();
        //1 向服务器发送请求, 请求类型必须是服务器定义的接口类型
        u32 dwRet = PROCMSG_OK;
        switch (eSendToDst)
        {
        case SERVER_3AS:
            {
                dwRet = PostMsgReqTo3AS(cReq);
            }
            break;
        case SERVER_CMU:
            {
                PostMsgReqToCMU(cReq);
            }
            break;
        default:
            {
                return PROCMSG_FAIL;
            }
            break;
        }
        if ( PROCMSG_DEL == dwRet )
        {
            TASKLOG(NORMAL_TASK, EVENT_LEV, "CU用户[%s]会话[%s]发送请求[%s--%d]，TAS不在线，直接拒绝请求\n",
                m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
                cReq.GetEventStr().c_str(), cReq.GetEventId());
            CEventRsp cRsp;
            cRsp.SetSeqNum(cReq.GetSeqNum());
            cRsp.SetSession(cReq.GetSession());
            cRsp.SetErrorCode(ERR_CUI_TAS_OFFLINE);
            m_strRspOfErr = cRsp.ToXml();
			if ( m_ptRecvNode != NULL )
			{
				SendRspToCu(m_strRspOfErr, m_ptRecvNode);
				m_ptRecvNode = NULL;
			}
            NextState(NOT_QUERY_STATE);
            delete this;
            return dwRet;
        }

        //将QueryTag插入到会话表中，等待NTF
        g_cCuiDataMgr.m_mapSessToTaskMap.Insert(cReq.GetQueryTag(), GetTaskNO());
        m_strQueryTag = cReq.GetQueryTag();

        //2 转换状态，等待应答
        NextState(WAIT_RESPONSE_STATE);

        //3 创建结果保存数据，超时时复用此结果数据
        m_pRsp = new CRspToCu;
        m_pRsp->SetSeqNum(cReq.GetSeqNum());
        m_pRsp->SetSession(cReq.GetSession());
        return PROCMSG_OK;
    }
    template<typename CRspFromSvr, typename CRspToCu>
    u32 OnWaitRspMsg(CMessage *const pcMsg)
    {
		CMS_ASSERT(GetState() == WAIT_RESPONSE_STATE);

		COspSipMsg *pcOspSipMsg = (COspSipMsg *)pcMsg->content;
		CRspFromSvr cRsp;
		pcOspSipMsg->GetMsgBody(cRsp);
		if (cRsp.GetErrorCode() != CMS_SUCCESS || cRsp.GetEntryNum() == 0)
		{ //查询失败 或者 无结果
			if (cRsp.GetErrorCode() != CMS_SUCCESS)
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "CQueryTask[NO: %u]查询应答[%s--%d]返回失败，错误码[%d]\n",
					GetTaskNO(), cRsp.GetEventStr().c_str(), cRsp.GetEventId(), cRsp.GetErrorCode());
			}
			else
			{
				TASKLOG(NORMAL_TASK, EVENT_LEV, "CQueryTask[NO: %u]查询应答[%s--%d]成功，结果为空\n",
					GetTaskNO(), cRsp.GetEventStr().c_str(), cRsp.GetEventId());
			}

			m_pRsp->SetErrorCode(cRsp.GetErrorCode());
			m_strRspOfErr = m_pRsp->ToXml();
			if (m_ptRecvNode != NULL)
			{
				SendRspToCu(m_strRspOfErr, m_ptRecvNode);
				m_ptRecvNode = NULL;
			}
			NextState(NOT_QUERY_STATE);
			delete this;
			return PROCMSG_OK;
		}

		CRspToCu *pcRspToCu = (CRspToCu *)m_pRsp;
		pcRspToCu->SetTotalEntryNum(cRsp.GetTotalEntryNum());
		NextState(WAIT_NOTIFY_STATE);
		return PROCMSG_OK;
    }

public:
    PTRecvNode m_ptRecvNode;
    CCuAgent *m_pcCuAgent;      //该指针一定不为空
    C3acAgent *m_pc3acAgent;    //只有当查询成功后(说明TAS在线)，该指针才有效，为了效率缓存
    string m_strRspOfErr;
	CCuUserDevGroupQryRsp *m_pRsp;
    string m_strQueryTag;       //当其不为空时，表面在m_mapSessToTaskMap中登记过
    std::list<string> m_VecNtfReqList; //缓存在rsp消息中接收到的ntf消息
	std::vector<string> m_devGroupIdList;
	u16 m_dwReqEvent;

	u32 m_dwTotalTime; // 收到CU请求开始计时，如果超时 (ERR_CUI_OPR_TIME_OUT）但3A数据接收成功就可以成功返回CU
private:
	void AddNtfForCu(const vector<CDeviceGroupInfo> &cCuDevGroupInfoList, const vector<bool> &hasDevList, vector<CCuUserDeviceGroupInfo> &devGroupIdList);

	void AddNtfForDps(const vector<CDeviceGroupInfo> &cCuDevGroupInfoList, std::vector<string> &devGroupIdList);
	
	void CompareNtfFromDps(vector<CCuUserDeviceGroupInfo> &cCuDevGroupInfoList, const set<string> &devGroupIdList);

	CPeAgentBase *GetPeAgentInServiceState(const string &strPeName);

	u32 SendReqToDps();
};

#endif // _DEV_GROUP_QUERY_TASK_H_
