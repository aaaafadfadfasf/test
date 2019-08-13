/*========================================================================================
ģ����    ��als
�ļ���    ��alarmlinksettask.h
����ļ�  ��
ʵ�ֹ���  ���澯����������������
����      ��fanxg
��Ȩ      ��<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
�޸ļ�¼��
����               �汾              �޸���             �߶���              �޸ļ�¼
2012/06/15         V1.0              fanxg                                  �½��ļ�
=========================================================================================*/

#ifndef _ALARM_LINK_SET_TASK_H
#define _ALARM_LINK_SET_TASK_H

#include "cms/ospsip/ospsip.h"
#include "cms/cmu/cmu_proto.h"


class CAlarmLinkSetTask : public CSipTask
{
public:
    enum
    {
        Wait = UNKNOWN_STATE+1,
        Rsp,
    };

public:
    CAlarmLinkSetTask(CInstExt *pcInst);
    virtual ~CAlarmLinkSetTask();

public:
    virtual const char* GetObjName() const
    {
        return "CAlarmLinkSetTask";
    }

    virtual const char* GetStateName(u32 dwState) const
    {
        switch (dwState)
        {
        case Wait:
            return "Wait";
        case Rsp:
            return "Rsp";
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


public:
    //ҵ���� 
    bool SendRsp(u32 dwError);
    bool SendRsp(const CAlarmLinkCfgSetRsp& cRsp);

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

    const CAlarmLinkCfgSetReq& GetReq() const
    {
        return m_tReq;
    }
    CAlarmLinkCfgSetReq& GetReq()
    {
        return m_tReq;
    }
    void SetReq(const CAlarmLinkCfgSetReq& tReq)
    {
        m_tReq = tReq;
    }

private:
    TKDSIP_TRANS_ID m_tSipTransId;
    CAlarmLinkCfgSetReq m_tReq;
};

#endif  //#ifndef _ALARM_LINK_SET_TASK_H