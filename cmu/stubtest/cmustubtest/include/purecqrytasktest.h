/*========================================================================================
模块名    ：cmu
文件名    ：purecqrytask.h
相关文件  ：
实现功能  ：前端录像查询事务
作者      ：fanxg
版权      ：<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2012/09/18         V1.0              fanxg                                  新建文件
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
    //状态处理
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
    //业务处理 

public:

    //数据获取
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