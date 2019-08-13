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
// �������������Ԫ�Ĵ���InviteReq��ʱ���ͷ�ʽ
// ����inviteReq������ReceiverSwitchTask���������SenderSwitchTask
class CInviteReceiverSwitchTask : public CSwitchTask
{
public:
    enum TSwitchTaskState
    {
        Init,       //����״̬��������Dst��Task����InviteReq
        Starting,   //�Ѿ���SenderPin����InviteReq���ȴ�InviteRsp
        Ack,        //�ȴ�Dst��InviteAck
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
	//״̬����
	u32 OnState(CMessage *const pcMsg);
	u32 OnStateTimer();

public:
    //��Ϣ����
    CMS_RESULT ProcInviteReq();
    CMS_RESULT ProcInviteRsp(const COspSipMsg*const pcOspSipMsg);
    CMS_RESULT ProcInviteAck(const COspSipMsg*const pcOspSipMsg);
	//�ں��н���������£�ǰ�˵������ԣ��ֱ��ʣ����ʵȣ������ģ�����������Ļ���cmu������pui����invite�����ֱ��Ӧ��cui
	//����Ӧ����Ϣ�������Ի����޸�֮ǰ�ģ�������Ҫ��ʱ�滻������
	void ReSetSdpMediaInfo(TSdp& tSdp);

	//CMS_RESULT StartSwitch(const TChannel& tDstChn, const TChannel& tSrcChn, const TSdp& tSdp, TKDSIP_DIALOG_ID tDlgID);

private:
    //CInviteReq		m_cInviteReq;
    //TKDSIP_TRANS_ID m_tReqTransId;
};

#endif  //#ifndef _INVITE_RECEIVER_SWITCH_TASK_H