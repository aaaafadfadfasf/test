/*========================================================================================
模块名    ：pui
文件名    ：videncparamgettask.h
相关文件  ：
实现功能  ：视频编码参数获取事务
作者      ：fanxg
版权      ：<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2012/02/22         V1.0              fanxg                                  新建文件
=========================================================================================*/

#ifndef _PU_VID_ENC_PARAM_GET_TASK_H
#define _PU_VID_ENC_PARAM_GET_TASK_H

#include "cms/ospsip/ospsip.h"
#include "cms/cmu/cmu_proto.h"

struct TMTIVideoEncParam;

class CVidEncParamGetTask : public CSipTask
{
public:
    enum
    {
        Wait = UNKNOWN_STATE+1,
        WaitRsp,
    };

public:
    CVidEncParamGetTask(CInstExt *pcInst);
    virtual ~CVidEncParamGetTask();

public:
    virtual const char* GetObjName() const
    {
        return "CVidEncParamGetTask";
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
    //业务处理 
    
public:

    //数据获取
    const CPuVidEncParamGetReq& GetReq() const
    {
        return m_tReq;
    }
    CPuVidEncParamGetReq& GetReq()
    {
        return m_tReq;
    }
    void SetReq(const CPuVidEncParamGetReq& tReq)
    {
        m_tReq = tReq;
    }

private:
    CPuVidEncParamGetReq m_tReq;
};

#endif  //#ifndef _PU_VID_ENC_PARAM_GET_TASK_H