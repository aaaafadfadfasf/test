/*========================================================================================
模块名    ：cmu
文件名    ：videncparamsettask.h
相关文件  ：
实现功能  ：视频编码参数设置事务
作者      ：fanxg
版权      ：<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2012/02/22         V1.0              fanxg                                  新建文件
=========================================================================================*/

#ifndef _PU_VID_ENC_PARAM_SET_TASK_H
#define _PU_VID_ENC_PARAM_SET_TASK_H

#include "cms/ospsip/ospsip.h"
#include "cms/cmu/cmu_proto.h"

struct TMTIVideoEncParam;

class CVidEncParamSetTask : public CSipTask
{
public:
    enum
    {
        Wait = UNKNOWN_STATE+1,
        WaitRsp,
    };

public:
    CVidEncParamSetTask(CInstExt *pcInst);
    virtual ~CVidEncParamSetTask();

public:
    virtual const char* GetObjName() const
    {
        return "CVidEncParamSetTask";
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
    const CPuVidEncParamSetReq& GetReq() const
    {
        return m_tReq;
    }
    CPuVidEncParamSetReq& GetReq()
    {
        return m_tReq;
    }
    void SetReq(const CPuVidEncParamSetReq& tReq)
    {
        m_tReq = tReq;
    }

private:
    CPuVidEncParamSetReq m_tReq;    //请求消息体
};

#endif  //#ifndef _PU_VID_ENC_PARAM_SET_TASK_H