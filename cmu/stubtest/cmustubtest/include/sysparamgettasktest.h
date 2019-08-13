/*========================================================================================
模块名    ：cmu
文件名    ：sysparamgettask.h
相关文件  ：
实现功能  ：前端系统参数获取事务
作者      ：fanxg
版权      ：<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2012/02/22         V1.0              fanxg                                  新建文件
=========================================================================================*/

#ifndef _PU_SYS_PARAM_GET_TASK_H
#define _PU_SYS_PARAM_GET_TASK_H

#include "cms/ospsip/ospsip.h"
#include "cms/cmu/cmu_proto.h"

class CPuSysParamGetTask : public CSipTask
{
public:
    enum
    {
        Wait = UNKNOWN_STATE+1,
        WaitRsp,
    };

public:
    CPuSysParamGetTask(CInstExt *pcInst);
    virtual ~CPuSysParamGetTask();

public:
    virtual const char* GetObjName() const
    {
        return "CPuSysParamGetTask";
    }

    virtual const char* GetStateName(u32 dwState) const
    {
        switch (dwState)
        {
        case Wait:
            return "Wait";
        case WaitRsp:
            return "WaitRsp";
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

    u32 OnWaitRsp(CMessage *const pcMsg);
    u32 OnWaitRspTimer();
    
public:

    //数据获取
    const CPuSysParamGetReq& GetReq() const
    {
        return m_tReq;
    }
    CPuSysParamGetReq& GetReq()
    {
        return m_tReq;
    }
    void SetReq(const CPuSysParamGetReq& tReq)
    {
        m_tReq = tReq;
    }

private:
    CPuSysParamGetReq m_tReq;
};

#endif  //#ifndef _PU_SYS_PARAM_GET_TASK_H