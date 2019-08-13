/*****************************************************************************
ģ����      : cmu 
�ļ���      : inviteswitchtask.h
����ļ�    : 
�ļ�ʵ�ֹ���: ����������
����        : ������
�汾        : vs_phoenix  Copyright(C) 2009-2015 KDDACOM, All rights reserved.
-----------------------------------------------------------------------------
�޸ļ�¼:
��  ��         �汾        �޸���      �޸�����
2011/11/04     1.0        ������        ����
******************************************************************************/

#ifndef _INVITE_SENDER_SWITCH_TASK_H
#define _INVITE_SENDER_SWITCH_TASK_H

#include "cms/ospsip/ospsip.h"
#include "cms/container/xmap.h"
#include "cms/cms_errorcode.h"

#include "cms/cmu/cmu_struct.h"
#include "cms/cmu/cmu_proto.h"
#include "cmudata.h"
#include "inviteswitchtask.h"
#include "3pctask.h"

/////////////////////////////////////////////////////////////////////////////////////////////////
//

class CInviteSenderSwitchTask : public CSwitchTask
{
public:
    enum TSwitchTaskState
    {
        Init,           //��״̬�������κ���Ϣ��������InviteReq��������SenderSwitchTask
        Starting,       //��RecverPin������InviteReq���ȴ�InviteRsp
        InviteSender,   //��SenderPin���͵�InviteReq���ȴ�InviteRsp
        Service
    };

public:
    CInviteSenderSwitchTask(CInstExt *pcInst);
    virtual ~CInviteSenderSwitchTask();
    //static CInviteSenderSwitchTask* Create(CInstExt *pcInst);

public:
    virtual const char* GetObjName() const
    {
        return "CInviteSenderSwitchTask";
    }

    virtual const char* GetStateName(u32 dwState) const
    {
        switch (dwState)
        {
        case Init:                      return "Init";
		case Starting:                  return "Starting";
		case InviteSender:              return "InviteSender";
        case Service:                   return "Service";
        default:                        return "UnknownState";
        }
    }

    virtual bool IsNotNeedPollState()
    {
        return (GetState() == Service);
    }

    virtual EmSwitchState GetSwitchState()
    {
        switch(GetState())
        {
        case Init:      return SWITCH_STATE_INIT;
        case Service:   return SWITCH_STATE_SWITCHED;
        default:        return SWITCH_STATE_SWITCHING;
        }
    }

    virtual TRedisModuleResourceId GetVtduResourceId() const
    {
        TRedisModuleResourceId tResourceId;
        if (!m_tOutputList.empty())
        {
            tResourceId = (*m_tOutputList.begin())->GetVtduResourceId();
        }
        else
        {
            tResourceId = m_tVtduResourceId;
        }

        return tResourceId;
    }

public:
    virtual void InitStateMachine();
    virtual void PrintData(ESwitchTrace eTrace=TRACE_NONE) const;

    virtual CMS_RESULT Start();

public:
    //״̬����
	u32 OnState(CMessage *const pcMsg);
	u32 OnStateTimer();

public:
    //��Ϣ����
    CMS_RESULT ProcInviteRsp(const COspSipMsg*const pcOspSipMsg);

    template<class CReq, class CRsp>
    u32 ProcInDlgNtfReq(CMessage *const pcMsg)
    {
        //������ǰ�˵�֪ͨ(���ȵ�)���ߵ��ú���
        u32 dwRet = PROCMSG_OK;
        COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

        //ֱ��Ӧ��pui
        CReq cReq;
        pcOspSipMsg->GetMsgBody(cReq);

        CRsp cRsp;
        cRsp.SetErrorCode(CMS_SUCCESS);
        cRsp.SetHeadFromReq(cReq);
        PostRsp(pcOspSipMsg->GetSipRsp(), pcOspSipMsg->GetSipTransID(), cRsp);

        //����ReceiveSwitchTask����3pcinvitetask����
        CTask *pcTask = NULL;
        if (NULL != m_pcCreatorTask)
        {
            pcTask = m_pcCreatorTask;
        }
        else
        {
            CSwitchTaskList& cTaskList = GetOutputList();
            pcTask = cTaskList.front();
            CMS_ASSERT(NULL != pcTask && "pcTask = NULL!!!");
        }

        if (pcTask != NULL)
        {
            pcTask->ProcMsg(pcMsg);
        }
        else
        {
            dwRet = PROCMSG_FAIL;
            TASKLOG(CMU_MOD_SWITCH, WARNING_LEV, "��ȡĿ�Ľ�������ʧ�� Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
        }

        return dwRet;
    }

	bool IsDelayPuAck() const
	{
		return m_bDelayPuAck;
	}

	void SendDelayPuAck()
	{
		if (m_bDelayPuAck)
		{
			PostInviteAck(m_cSender, TSdp());
			m_bDelayPuAck = false;				// ȷ��Դ�����е�Ackֻ����һ�Σ����ⷢ�Ͷ��
		}
	}

private:

	bool m_bDelayPuAck;
};


#endif  //#ifndef _INVITE_SENDER_SWITCH_TASK_H