/*========================================================================================
ģ����    ��cmu
�ļ���    ��invitetasktest.h
����ļ�  ��
ʵ�ֹ���  ����������
����      ��huangzhichun
��Ȩ      ��<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
�޸ļ�¼��
����               �汾              �޸���             �߶���              �޸ļ�¼
2012/02/13         V1.0              huangzhichun                         �½��ļ�
=========================================================================================*/
#ifndef _INVITE_TASK_TEST_H
#define _INVITE_TASK_TEST_H

#include "cms/cmu/cmu_proto.h"
#include "cms/ospsip/ospsip.h"
#include "cbb/dataswitch/datadispatch.h"

class CInviteTask : public CSipTask
{
public:
    enum
    {
        Req = UNKNOWN_STATE+1,
        QryAddr,
        WaitRsp,
        Service,
		WaitByeRsp,
    };

    typedef CInviteReq CReq;
    typedef CInviteRsp CRsp;

public:
	CInviteTask(CInstExt* pInst) : CSipTask(pInst)
	{
        //m_tSipTransId = INVALID_TRANS_ID;
        m_tSipDlgId = INVALID_DIALOG_ID;
	}

    ~CInviteTask();

public:
	virtual const char* GetObjName() const
	{
		return "CInviteTask";
	}

    virtual const char* GetStateName(u32 dwState) const
    {
        switch (dwState)
        {
        case Req:
            return "Req";
        case QryAddr:
            return "QryAddr";
        case WaitRsp:
            return "WaitRsp";
        case Service:
            return "Service";
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

    u32 OnQryAddr(CMessage *const pcMsg);
    u32 OnQryAddrTimer();

    u32 OnWaitRsp(CMessage *const pcMsg);
    u32 OnWaitRspTimer();

    u32 OnService(CMessage *const pcMsg);
    u32 OnServiceTimer();

    u32 OnWaitByeRsp(CMessage *const pcMsg);
    u32 OnWaitByeRspTimer();

    bool CreateInviteReq(const string& strUserSess, const TChannel& tSrcChn, const TChannel& tDstChn);
    bool SendInviteReq();
    bool SendKeyFrameReq();

    void AddDsDump();
    void DelDsDump();
    void SetDsFilter(FilterFunc pfDsFilter);

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

    const CInviteReq& GetReq() const
    {
        return m_tReq;
    }
    CInviteReq& GetReq()
    {
        return m_tReq;
    }
    void SetReq(const CInviteReq& tReq)
    {
        m_tReq = tReq;
    }

    u16 GetVidPort() const
    {
        return m_tNetAddr.GetNetPort();
    }

    u16 GetAudPort() const
    {
        return (GetVidPort()+2);
    }
	
private:
    //TKDSIP_TRANS_ID m_tSipTransId;
	TKDSIP_DIALOG_ID m_tSipDlgId;
	CInviteReq m_tReq;

    TNetAddr m_tNetAddr;
};

#endif  //#ifndef _INVITE_TASK_TEST_H