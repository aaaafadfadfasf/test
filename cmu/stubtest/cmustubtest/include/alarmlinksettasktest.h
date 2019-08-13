/*========================================================================================
模块名    ：als
文件名    ：alarmlinksettask.h
相关文件  ：
实现功能  ：告警联动参数设置事务
作者      ：fanxg
版权      ：<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2012/06/15         V1.0              fanxg                                  新建文件
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
    //状态处理
    u32 OnWait(CMessage *const pcMsg);
    u32 OnWaitTimer();

    u32 OnRsp(CMessage *const pcMsg);
    u32 OnRspTimer();


public:
    //业务处理 
    bool SendRsp(u32 dwError);
    bool SendRsp(const CAlarmLinkCfgSetRsp& cRsp);

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