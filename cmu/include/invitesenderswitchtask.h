/*****************************************************************************
模块名      : cmu 
文件名      : inviteswitchtask.h
相关文件    : 
文件实现功能: 交换任务类
作者        : 黄至春
版本        : vs_phoenix  Copyright(C) 2009-2015 KDDACOM, All rights reserved.
-----------------------------------------------------------------------------
修改记录:
日  期         版本        修改人      修改内容
2011/11/04     1.0        黄至春        创建
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
        Init,           //此状态不处理任何消息，不会有InviteReq主动发给SenderSwitchTask
        Starting,       //向RecverPin发送了InviteReq，等待InviteRsp
        InviteSender,   //向SenderPin发送的InviteReq，等待InviteRsp
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
    //状态处理
	u32 OnState(CMessage *const pcMsg);
	u32 OnStateTimer();

public:
    //消息处理
    CMS_RESULT ProcInviteRsp(const COspSipMsg*const pcOspSipMsg);

    template<class CReq, class CRsp>
    u32 ProcInDlgNtfReq(CMessage *const pcMsg)
    {
        //放像中前端的通知(进度等)会走到该函数
        u32 dwRet = PROCMSG_OK;
        COspSipMsg* pcOspSipMsg = (COspSipMsg*)pcMsg->content;

        //直接应答pui
        CReq cReq;
        pcOspSipMsg->GetMsgBody(cReq);

        CRsp cRsp;
        cRsp.SetErrorCode(CMS_SUCCESS);
        cRsp.SetHeadFromReq(cReq);
        PostRsp(pcOspSipMsg->GetSipRsp(), pcOspSipMsg->GetSipTransID(), cRsp);

        //交给ReceiveSwitchTask或者3pcinvitetask处理
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
            TASKLOG(CMU_MOD_SWITCH, WARNING_LEV, "获取目的交换事务失败 Msg[%s-%d]\n", OspExtEventDesc(pcMsg->event).c_str(), pcMsg->event);
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
			m_bDelayPuAck = false;				// 确保源交换中的Ack只发送一次，避免发送多次
		}
	}

private:

	bool m_bDelayPuAck;
};


#endif  //#ifndef _INVITE_SENDER_SWITCH_TASK_H