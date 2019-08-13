/*========================================================================================
ģ����    ��cmu
�ļ���    ��puptzctrltask.h
����ļ�  ��
ʵ�ֹ���  ��pu����ͷ��������
����      ��fanxg
��Ȩ      ��<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
�޸ļ�¼��
����               �汾              �޸���             �߶���              �޸ļ�¼
2012/02/13         V1.0              fanxg                                  �½��ļ�
=========================================================================================*/
#ifndef _PU_PTZ_CTRL_TASK_H
#define _PU_PTZ_CTRL_TASK_H

#include "cms/cmu/cmu_proto.h"
#include "cms/ospsip/ospsip.h"

class CPuPtzCtrlTask : public CSipTask
{
public:
    enum
    {
        Req = UNKNOWN_STATE+1,
        WaitRsp,
        Hold,
    };

    typedef CPuPtzCtrlReq CReq;
    typedef CPuPtzCtrlRsp CRsp;

public:
	CPuPtzCtrlTask(CInstExt* pInst);	
	~CPuPtzCtrlTask();

    void ReleaseResource();

public:
	virtual const char* GetObjName() const
	{
		return "CPuPtzCtrlTask";
	}

    virtual const char* GetStateName(u32 dwState) const
    {
        switch (dwState)
        {
        case Req:
            return "Req";
        case WaitRsp:
            return "WaitRsp";
        case Hold:
            return "Hold";
        default:
            break;
        }
        return "Unknown State";
    }


    virtual void InitStateMachine();
    virtual void PrintData() const;

public:
    //ҵ����
    u32 OnReq(CMessage *const pcMsg);

    u32 OnReqTimer();

    u32 OnWaitRsp(CMessage *const pcMsg);

    u32 OnWaitRspTimer();

    u32 OnHold(CMessage *const pcMsg);

    u32 OnHoldTimer();

    void SendReq(const CReq& cReq, const TSipURI& tDstUri);

    bool SendRsp(u32 dwError);

    bool SendRsp(const CPuPtzCtrlRsp& cRsp);

public:

    //���ݻ�ȡ
    const TKDSIP_TRANS_ID GetSipTransID() const
    {
        return m_tSipTransId;
    }
    void SetSipTransID(const TKDSIP_TRANS_ID tSipTransID)
    {
        m_tSipTransId = tSipTransID;
    }

    const CPuPtzCtrlReq& GetReq() const
    {
        return m_tReq;
    }
    CPuPtzCtrlReq& GetReq()
    {
        return m_tReq;
    }
    void SetReq(const CPuPtzCtrlReq& tReq)
    {
        m_tReq = tReq;
    }
	
private:
    TKDSIP_TRANS_ID m_tSipTransId;
	CPuPtzCtrlReq m_tReq;
};

#endif  //#ifndef _PU_PTZ_CTRL_TASK_H