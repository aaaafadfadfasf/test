/*========================================================================================
模块名    ：tvs
文件名    ：twschemesavetasktest.h
相关文件  ：
实现功能  ：电视墙预案保存事务
作者      ：fanxg
版权      ：<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2012/08/01         V1.0              fanxg                                  新建文件
=========================================================================================*/

#ifndef _TW_SCHEME_SAVE_TASK_H
#define _TW_SCHEME_SAVE_TASK_H

#include "cms/ospsip/ospsip.h"
#include "cms/cmu/cmu_proto.h"


class CTwSchemeSaveTask : public CSipTask
{
public:
    enum
    {
        Wait = UNKNOWN_STATE+1,
        Rsp,
    };

public:
    CTwSchemeSaveTask(CInstExt *pcInst);
    virtual ~CTwSchemeSaveTask();

public:
    virtual const char* GetObjName() const
    {
        return "CTwSchemeSaveTask";
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

    //数据获取

private:
};

#endif  //#ifndef _TW_SCHEME_SAVE_TASK_H