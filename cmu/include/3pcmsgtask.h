/*****************************************************************************
模块名      : cmu 
文件名      : 3pcmsgtask.h 
相关文件    : 
文件实现功能: 交换任务类
作者        : 黄至春
版本        : vs_phoenix  Copyright(C) 2009-2015 KDDACOM, All rights reserved.
-----------------------------------------------------------------------------
修改记录:
日  期         版本        修改人      修改内容
2012/08/02     1.0        黄至春        创建
******************************************************************************/

#ifndef _3PC_MSG_TASK_H
#define _3PC_MSG_TASK_H

#include "cms/ospsip/ospsip.h"
#include "cms/container/xmap.h"
#include "cms/cms_errorcode.h"

#include "cms/cmu/cmu_struct.h"
#include "cms/cmu/cmu_proto.h"
#include "cmudata.h"
#include "cmutask.h"
#include "3pctask.h"

///////////////////////////////////////////////////////////////////////////////////////
//C3PCMsgTask
///////////////////////////////////////////////////////////////////////////////////////
class C3PCMsgTask : public C3PCTask
{
public:
    enum
    {
        Idle = UNKNOWN_STATE+1,
        WaitRsp
    };

public:
    C3PCMsgTask(CInstExt *pcInst);

    virtual ~C3PCMsgTask();

    /////////////////////////////////////////////////////////////
    //重载函数
public:
    virtual const char* GetObjName() const
    {
        return "C3PCMsgTask";
    }

    virtual const char* GetStateName(u32 dwState) const
    {
        switch (dwState)
        {
        case Idle:
            return "Idle";
        case WaitRsp:
            return "WaitRsp";

        default:
            break;
        }
        return "Unknown State";
    }

    virtual void InitStateMachine();
    void ReleaseResource();

    /////////////////////////////////////////////////////////////
    //状态处理函数
public:
	u32 OnIdle(CMessage *const pcMsg);
	u32 OnIdleTimer();
	u32 OnWaitRsp(CMessage *const pcMsg);
	u32 OnWaitRspTimer();

    /////////////////////////////////////////////////////////////
    //业务逻辑
public:
    CMS_RESULT ProcStart3PCReq();
    PROCMSG_RESULT ProcStop3PCReq();

    /////////////////////////////////////////////////////////////
    //成员属性
public:
protected:
private:
	string m_strSrcChn;
};


#endif  //#ifndef _3PC_MSG_TASK_H