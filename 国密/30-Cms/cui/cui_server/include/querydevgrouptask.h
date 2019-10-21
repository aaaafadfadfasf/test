/*
��TAS��ѯ������Ϣ���ٵ�DPS��У��������Ƿ����豸
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
        NOT_QUERY_STATE = UNKNOWN_STATE + 1,        // �ȴ���3A�����ѯ
        WAIT_RESPONSE_STATE,                        // �ȴ���ѯRsp
        WAIT_NOTIFY_STATE,                          // �ȴ���ѯNtf
		WAIT_DPS_RSP_STATE                            // �ȴ�DPSӦ��
    };
    enum EServerType
    {
        SERVER_3AS = 0,     //��ѯ��Ŀ���������3AS
        SERVER_CMU,
        SERVER_RCS,
        SERVER_TVWALL,
        SERVER_TYPE_NUM
    };
public:
    CDevGroupQueryTask(CInstExt *pcInst, CCuAgent *pcCuAgent, PTRecvNode ptRecvNode);
    ~CDevGroupQueryTask();

public:
    //������Ҫ��д�ķ���
    virtual const char* GetObjName() const //�������صķ���
    {
        return "CDevGroupQueryTask";
    }
    virtual const char* GetStateName(u32 dwState) const //�������صķ���
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
        TASKLOG(NORMAL_TASK, EVENT_LEV, "CDevGroupQueryTask[TaskNO %u]������Ϣ[%s--%d]��������[%s]\n", 
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
    //״̬����
    u32 NotQueryProcMsg(CMessage *const pcMsg);
    u32 WaitRspProcMsg(CMessage *const pcMsg);
    u32 WaitRspTimerPoll();
    u32 WaitNotifyProcMsg(CMessage *const pcMsg);
    u32 ProcNotifyMsg(CMessage *const pcMsg);
	u32 WaitNotifyTimerPoll();
	// ���TAS�ɹ�ȡ����Ϣ����Ӱ��CUȡ�������˳�ʱ������ʧ�ܶ����ظ�CU�ɹ�
	u32 WaitDpsRspProcMsg(CMessage *const pcMsg);
	u32 WaitDpsRspTimerPoll();

    template<typename CReqToSvr, typename CRspToCu>
    u32 OnNotQueryMsg(CReqToSvr &cReq, EServerType eSendToDst = SERVER_3AS)
    {
        CMS_ASSERT(GetState() == NOT_QUERY_STATE);
        //����req��Ϣ�������wait rsp state�յ�ntf��Ϣ�ô���
        m_dwReqEvent = cReq.GetEventId();
        //1 ���������������, �������ͱ����Ƿ���������Ľӿ�����
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
            TASKLOG(NORMAL_TASK, EVENT_LEV, "CU�û�[%s]�Ự[%s]��������[%s--%d]��TAS�����ߣ�ֱ�Ӿܾ�����\n",
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

        //��QueryTag���뵽�Ự���У��ȴ�NTF
        g_cCuiDataMgr.m_mapSessToTaskMap.Insert(cReq.GetQueryTag(), GetTaskNO());
        m_strQueryTag = cReq.GetQueryTag();

        //2 ת��״̬���ȴ�Ӧ��
        NextState(WAIT_RESPONSE_STATE);

        //3 ��������������ݣ���ʱʱ���ô˽������
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
		{ //��ѯʧ�� ���� �޽��
			if (cRsp.GetErrorCode() != CMS_SUCCESS)
			{
				TASKLOG(NORMAL_TASK, ERROR_LEV, "CQueryTask[NO: %u]��ѯӦ��[%s--%d]����ʧ�ܣ�������[%d]\n",
					GetTaskNO(), cRsp.GetEventStr().c_str(), cRsp.GetEventId(), cRsp.GetErrorCode());
			}
			else
			{
				TASKLOG(NORMAL_TASK, EVENT_LEV, "CQueryTask[NO: %u]��ѯӦ��[%s--%d]�ɹ������Ϊ��\n",
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
    CCuAgent *m_pcCuAgent;      //��ָ��һ����Ϊ��
    C3acAgent *m_pc3acAgent;    //ֻ�е���ѯ�ɹ���(˵��TAS����)����ָ�����Ч��Ϊ��Ч�ʻ���
    string m_strRspOfErr;
	CCuUserDevGroupQryRsp *m_pRsp;
    string m_strQueryTag;       //���䲻Ϊ��ʱ��������m_mapSessToTaskMap�еǼǹ�
    std::list<string> m_VecNtfReqList; //������rsp��Ϣ�н��յ���ntf��Ϣ
	std::vector<string> m_devGroupIdList;
	u16 m_dwReqEvent;

	u32 m_dwTotalTime; // �յ�CU����ʼ��ʱ�������ʱ (ERR_CUI_OPR_TIME_OUT����3A���ݽ��ճɹ��Ϳ��Գɹ�����CU
private:
	void AddNtfForCu(const vector<CDeviceGroupInfo> &cCuDevGroupInfoList, const vector<bool> &hasDevList, vector<CCuUserDeviceGroupInfo> &devGroupIdList);

	void AddNtfForDps(const vector<CDeviceGroupInfo> &cCuDevGroupInfoList, std::vector<string> &devGroupIdList);
	
	void CompareNtfFromDps(vector<CCuUserDeviceGroupInfo> &cCuDevGroupInfoList, const set<string> &devGroupIdList);

	CPeAgentBase *GetPeAgentInServiceState(const string &strPeName);

	u32 SendReqToDps();
};

#endif // _DEV_GROUP_QUERY_TASK_H_
