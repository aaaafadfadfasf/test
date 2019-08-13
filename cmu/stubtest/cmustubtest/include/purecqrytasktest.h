/*========================================================================================
ģ����    ��cmu
�ļ���    ��purecqrytask.h
����ļ�  ��
ʵ�ֹ���  ��ǰ��¼���ѯ����
����      ��fanxg
��Ȩ      ��<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
�޸ļ�¼��
����               �汾              �޸���             �߶���              �޸ļ�¼
2012/09/18         V1.0              fanxg                                  �½��ļ�
=========================================================================================*/

#ifndef _PU_REC_QRY_TASK_H
#define _PU_REC_QRY_TASK_H

#include "cms/ospsip/ospsip.h"
#include "cms/cmu/cmu_proto.h"


class CPuRecQryTask : public CSipTask
{
public:
    enum
    {
        Wait = UNKNOWN_STATE+1,
        Rsp,
        Ntf,
        WaitRsp,
        Ack,
        WaitByeRsp,
    };

public:
    CPuRecQryTask(CInstExt *pcInst);
    virtual ~CPuRecQryTask();

public:
    virtual const char* GetObjName() const
    {
        return "CPuRecQryTask";
    }

    virtual const char* GetStateName(u32 dwState) const
    {
        switch (dwState)
        {
        case Wait:
            return "Wait";
        case Rsp:
            return "Rsp";
        case Ntf:
            return "Ntf";
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

public:
    virtual void InitStateMachine();
    virtual void PrintData();

public:
    //״̬����
    u32 OnWait(CMessage *const pcMsg);
    u32 OnWaitTimer();

    u32 OnRsp(CMessage *const pcMsg);
    u32 OnRspTimer();

    u32 OnNtf(CMessage *const pcMsg);
    u32 OnNtfTimer();

    u32 OnWaitRsp(CMessage *const pcMsg);
    u32 OnWaitRspTimer();

    u32 OnAck(CMessage *const pcMsg);
    u32 OnAckTimer();


public:
    //ҵ���� 

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

    const CPuRecQryReq& GetReq() const
    {
        return m_tReq;
    }
    CPuRecQryReq& GetReq()
    {
        return m_tReq;
    }
    void SetReq(const CPuRecQryReq& tReq)
    {
        m_tReq = tReq;
    }

private:
    TKDSIP_TRANS_ID m_tSipTransId;
    CPuRecQryReq m_tReq;
    string m_strQryTag;

    string m_strCuSession;
    CInviteReq m_tInviteReq;
    CPuRecInfoList m_cRecInfoList;
    TKDSIP_DIALOG_ID m_tSipDlgId;
    bool m_bIsQryProgress;
};

#endif  //#ifndef _PU_REC_QRY_TASK_H