/*
��ѯPTZ����Ϣ�е��û������û������豸��PTZ���Ƽ���
*/
#ifndef _GET_PTZ_LOCK_USER_INFO_TASK_H_
#define _GET_PTZ_LOCK_USER_INFO_TASK_H_

#include "cms/ospext/ospext.h"
#include "cms/ospsip/ospsip.h"
#include "cuidata.h"
#include "cms/commonconfig/cmcfg.h"
#include "culogintask.h"
#include "cms/utility/digital_string.h"
#include "normalcutask.h"

class CGetPuPtzLockUserInfoTask : public CSipTask
{
public:
	enum
	{
		NOT_QUERY_STATE = UNKNOWN_STATE + 1,        //����״̬��δ���Ͳ�ѯ����
		WAIT_RESPONSE_STATE,                        //�ȴ���ѯRsp
		WAIT_NOTIFY_STATE,                          //�ȴ���ѯNtf
	};
public:
	CGetPuPtzLockUserInfoTask(CInstExt *pcInst, CCuAgent *pcCuAgent, PTRecvNode ptRecvNode);
	~CGetPuPtzLockUserInfoTask();

	void SetPuPtzCtrlRsp(const CPuPtzCtrlRsp& cRsp);
	void SetPuTransOperateRsp(const CCuPuTransparentOperateRsp& cRsp);
public:
	//������Ҫ��д�ķ���
	virtual const char* GetObjName() const //�������صķ���
	{
		return "CGetPuPtzLockUserInfoTask";
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
		TASKLOG(NORMAL_TASK, EVENT_LEV, "CGetPuPtzLockUserInfoTask[TaskNO %u]������Ϣ[%s--%d]��������[%s]\n",
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
public:
	//״̬����
	u32 NotQueryProcMsg(CMessage *const pcMsg);
	u32 WaitRspProcMsg(CMessage *const pcMsg);
	u32 WaitRspTimerPoll();
	u32 WaitNotifyProcMsg(CMessage *const pcMsg);
	u32 ProcNotifyMsg(CMessage *const pcMsg);
	u32 WaitNotifyTimerPoll();

	bool GetUserNameInfo(const string &strUserId, const TChannel &tDevVsChn);
	bool GetUserDevVsPtzPri();

	//NotReq״̬�µķ��ʹ���
	//��ΪCReq����͸����CUI��Ҫת��CU��3AS֮������ݣ��������ﲻֱ�Ӵ���CMessage
	//����ת���ŵ��������
	/*====================================================================
	������      : OnNotQueryMsg
	����        : NotReq״̬�µķ��ʹ���
	��ΪCReq����͸����CUI��Ҫת��CU��3AS֮������ݣ��������ﲻֱ�Ӵ���CMessage
	����ת���ŵ��������
	�㷨ʵ��    :
	����ȫ�ֱ���:
	�������˵��: CReqToSvr &cReq            �������ͱ����Ƿ���������Ľӿ�����
	EServerType eSendToDst     ��������3AS����CMU���Ժ��п�����RecMgr
	����ֵ˵��  : �ɹ�:true��ʧ��:false
	----------------------------------------------------------------------
	�޸ļ�¼    :
	��  ��      �汾        �޸���        �޸�����
	2012/2/3/             liangli
	====================================================================*/
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
		if (PROCMSG_DEL == dwRet)
		{
			TASKLOG(NORMAL_TASK, EVENT_LEV, "CU�û�[%s]�Ự[%s]��������[%s--%d]��TAS�����ߣ�ֱ�Ӿܾ�����\n",
				m_pcCuAgent->m_strUserName.c_str(), m_pcCuAgent->m_strSession.c_str(),
				cReq.GetEventStr().c_str(), cReq.GetEventId());
			CEventRsp cRsp;
			cRsp.SetSeqNum(cReq.GetSeqNum());
			cRsp.SetSession(cReq.GetSession());
			cRsp.SetErrorCode(ERR_CUI_TAS_OFFLINE);
			m_strRspOfErr = cRsp.ToXml();
			SendRspToCu(m_strRspOfErr, m_ptRecvNode);
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
				TASKLOG(NORMAL_TASK, ERROR_LEV, "CGetPuPtzLockUserInfoTask[NO: %u]��ѯӦ��[%s--%d]����ʧ�ܣ�������[%d]\n",
					GetTaskNO(), cRsp.GetEventStr().c_str(), cRsp.GetEventId(), cRsp.GetErrorCode());
			}
			else
			{
				TASKLOG(NORMAL_TASK, EVENT_LEV, "CGetPuPtzLockUserInfoTask[NO: %u]��ѯӦ��[%s--%d]�ɹ������Ϊ��\n",
					GetTaskNO(), cRsp.GetEventStr().c_str(), cRsp.GetEventId());
			}

			m_pRsp->SetErrorCode(cRsp.GetErrorCode());
			m_strRspOfErr = m_pRsp->ToXml();
			SendRspToCu(m_strRspOfErr, m_ptRecvNode);
			NextState(NOT_QUERY_STATE);
			delete this;
			return PROCMSG_OK;
		}
		NextState(WAIT_NOTIFY_STATE);
		return PROCMSG_OK;
	}

public:
	PTRecvNode m_ptRecvNode;
	CCuAgent *m_pcCuAgent;      //��ָ��һ����Ϊ��
	C3acAgent *m_pc3acAgent;    //ֻ�е���ѯ�ɹ���(˵��TAS����)����ָ�����Ч��Ϊ��Ч�ʻ���
	string m_strRspOfErr;
	CPuPtzCtrlWithLockRsp *m_pRsp;
	u32 m_dwRspEventId;
	string m_strQueryTag;       //���䲻Ϊ��ʱ��������m_mapSessToTaskMap�еǼǹ�
	std::list<string> m_VecNtfReqList; //������rsp��Ϣ�н��ܵ���ntf��Ϣ
	u16 m_dwReqEvent;
	string m_strUserId;
	TChannel m_tVsChn;
	bool m_bIsSuperAdmin;
};

#endif // _GET_PTZ_LOCK_USER_INFO_TASK_H_
