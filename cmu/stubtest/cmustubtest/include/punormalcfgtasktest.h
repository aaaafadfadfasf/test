/*========================================================================================
模块名    ：pui
文件名    ：punormalcfgtask.h
相关文件  ：
实现功能  ：pu通用的参数操作事务，包括设置和获取
作者      ：fanxg
版权      ：<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2012/08/16         V1.0              fanxg                                  新建文件
=========================================================================================*/

#ifndef _PU_NORMAL_CFG_TASK_H
#define _PU_NORMAL_CFG_TASK_H

#include "cms/ospsip/ospsip.h"
#include "cms/cms_proto.h"

class CPuNormalCfgTask : public CSipTask
{
public:
    enum
    {
        Wait = UNKNOWN_STATE+1,
        Rsp,
    };

public:
    CPuNormalCfgTask(CInstExt *pcInst);
    virtual ~CPuNormalCfgTask();

public:
    virtual const char* GetObjName() const
    {
        return "CPuNormalCfgTask";
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

public:


private:
};

#endif  //#ifndef _PU_NORMAL_CFG_TASK_H