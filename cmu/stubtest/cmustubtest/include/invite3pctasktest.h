/*========================================================================================
ģ����    ��cmu
�ļ���    ��invite3pctasktest.h
����ļ�  ��
ʵ�ֹ���  ����������
����      ��huangzhichun
��Ȩ      ��<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
�޸ļ�¼��
����               �汾              �޸���             �߶���              �޸ļ�¼
2012/08/10         V1.0              huangzhichun                         �½��ļ�
=========================================================================================*/
#ifndef _INVITE_3PC_TASK_TEST_H
#define _INVITE_3PC_TASK_TEST_H

#include "cms/cmu/cmu_proto.h"
#include "cms/ospsip/ospsip.h"

class CInvite3PCTask : public CSipTask
{
public:
    enum
    {
        Req = UNKNOWN_STATE+1,
        WaitRsp,
        Ack,
		WaitByeRsp,
    };

    typedef CInviteReq CReq;
    typedef CInviteRsp CRsp;

public:
	CInvite3PCTask(CInstExt* pInst) : CSipTask(pInst)
	{
        //m_tSipTransId = INVALID_TRANS_ID;
        m_tSipDlgId = INVALID_DIALOG_ID;
	}
	
	~CInvite3PCTask()
	{	
	}

public:
	virtual const char* GetObjName() const
	{
		return "CInvite3PCTask";
	}

    virtual const char* GetStateName(u32 dwState) const
    {
        switch (dwState)
        {
        case Req:
            return "Req";
        case WaitRsp:
            return "WaitRsp";
        case Ack:
            return "Ack";
        case WaitByeRsp:
            return "WaitByeRsp";
        default:
            break;
        }
        return "Unknown State";
    }


    virtual void InitStateMachine();

public:
    //ҵ����
    u32 OnReq(CMessage *const pcMsg);

    u32 OnReqTimer();

    u32 OnWaitRsp(CMessage *const pcMsg);

    u32 OnWaitRspTimer();

    u32 OnAck(CMessage *const pcMsg);

    u32 OnAckTimer();

public:

    //���ݻ�ȡ
    //const TKDSIP_TRANS_ID GetSipTransID() const
    //{
    //    return m_tSipTransId;
    //}
    //void SetSipTransID(const TKDSIP_TRANS_ID tSipTransID)
    //{
    //    m_tSipTransId = tSipTransID;
    //}

    const CInviteThirdPartyCallReq& GetReq() const
    {
        return m_tReq;
    }
    CInviteThirdPartyCallReq& GetReq()
    {
        return m_tReq;
    }
    void SetReq(const CInviteThirdPartyCallReq& tReq)
    {
        m_tReq = tReq;
    }
	
private:
    //TKDSIP_TRANS_ID m_tSipTransId;
	TKDSIP_DIALOG_ID m_tSipDlgId;
	CInviteThirdPartyCallReq m_tReq;
};

#endif  //#ifndef _INVITE_3PC_TASK_TEST_H