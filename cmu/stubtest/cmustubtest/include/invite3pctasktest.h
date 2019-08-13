/*========================================================================================
模块名    ：cmu
文件名    ：invite3pctasktest.h
相关文件  ：
实现功能  ：交换事务
作者      ：huangzhichun
版权      ：<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2012/08/10         V1.0              huangzhichun                         新建文件
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
    //业务处理
    u32 OnReq(CMessage *const pcMsg);

    u32 OnReqTimer();

    u32 OnWaitRsp(CMessage *const pcMsg);

    u32 OnWaitRspTimer();

    u32 OnAck(CMessage *const pcMsg);

    u32 OnAckTimer();

public:

    //数据获取
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