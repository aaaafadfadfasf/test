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

#ifndef _INVITE_RECEIVER_SWITCH_TASK_H
#define _INVITE_RECEIVER_SWITCH_TASK_H

#include "cms/ospsip/ospsip.h"
#include "cms/container/xmap.h"
#include "cms/cms_errorcode.h"

#include "cms/cmu/cmu_struct.h"
#include "cms/cmu/cmu_proto.h"
#include "cmudata.h"
#include "inviteswitchtask.h"

/////////////////////////////////////////////////////////////////////////////////////////////////
// 根据这个交换单元的处理InviteReq的时机和方式
// 接收inviteReq请求是ReceiverSwitchTask，否则就是SenderSwitchTask
class CInviteReceiverSwitchTask : public CSwitchTask
{
public:
    enum TSwitchTaskState
    {
        Init,       //空闲状态，可以有Dst向本Task发起InviteReq
        Starting,   //已经向SenderPin发送InviteReq，等待InviteRsp
        Ack,        //等待Dst的InviteAck
        Service
    };

public:
    CInviteReceiverSwitchTask(CInstExt *pcInst);
    virtual ~CInviteReceiverSwitchTask();
    //static CInviteReceiverSwitchTask* Create(CInstExt *pcInst);

public:
    virtual const char* GetObjName() const
    {
        return "CInviteReceiverSwitchTask";
    }

    virtual const char* GetStateName(u32 dwState) const
    {
        switch (dwState)
        {
        case Init:				return "Init";
        case Starting:		    return "Starting";
        case Ack:				return "Ack";
        case Service:			return "Service";
        default:				return "UnknownState";
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

public:
    virtual void InitStateMachine();
    //void ReleaseResource();
    virtual void PrintData(ESwitchTrace eTrace=TRACE_NONE) const;

    virtual CMS_RESULT Start();

public:
	//状态处理
	u32 OnState(CMessage *const pcMsg);
	u32 OnStateTimer();

public:
    //消息处理
    CMS_RESULT ProcInviteReq();
    CMS_RESULT ProcInviteRsp(const COspSipMsg*const pcOspSipMsg);
    CMS_RESULT ProcInviteAck(const COspSipMsg*const pcOspSipMsg);
	//在含有交换的情况下，前端的流属性（分辨率，码率等）被更改，这是再浏览的话，cmu不会像pui发送invite请求就直接应答cui
	//这是应答消息中流属性还是修改之前的，所以需要适时替换成最新
	void ReSetSdpMediaInfo(TSdp& tSdp);

	//CMS_RESULT StartSwitch(const TChannel& tDstChn, const TChannel& tSrcChn, const TSdp& tSdp, TKDSIP_DIALOG_ID tDlgID);

private:
    //CInviteReq		m_cInviteReq;
    //TKDSIP_TRANS_ID m_tReqTransId;
};

#endif  //#ifndef _INVITE_RECEIVER_SWITCH_TASK_H