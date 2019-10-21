/*****************************************************************************
   模块名      : cui_fcgi 
   文件名      : clienttask.h
   相关文件    : 
   文件实现功能: client task定义
   作者        : liangli
   版本        : vs_phoenix  Copyright(C) 2009-2015 KDC, All rights reserved.
-----------------------------------------------------------------------------
   修改记录:
   日  期      版本        修改人      修改内容
   2011/10/10   1.0         liangli        创建
******************************************************************************/
#ifndef _CLIENT_TASK_H_
#define _CLIENT_TASK_H_
#include "cms/ospext/ospext.h"
#include "cms/ospsip/ospsip.h"

class CClientTask : public CSipTask
{
public:
    enum
    {
        IDLE_STATE = UNKNOWN_STATE + 1,  //初始状态, 可以发送注册请求
        WAIT_REG_RSP_STATE,                  //等待接收RegRsp
        SERVICE_STATE,                  //服务态, 可以发送消息，订阅，以及接收广播
    };
public:
    CClientTask(CInstExt *pcInst);
    ~CClientTask();
public:
    //子类需要重写的方法
    virtual const char* GetObjName() const
    {
        return "CClientTask";
    }
    virtual const char* GetStateName(u32 dwState);
    virtual void InitStateMachine();
    virtual void TerminateTask();
public:
    u32 WaitRegRspProcMsg(CMessage *const pcMsg);
    u32 WaitRegRspTimerPoll();
    u32 ServiceProcMsg(CMessage *const pcMsg);
public:
    //子类自己的方法
    u32 StartReg();
    u32 StartUnReg();
    u32 SendMsg(const string strMsg);
    u32 StartSubscribe();
    u32 StopSubscribe();


	u32 StartInvite();
	u32 StopInvite();
    u32 SendInfoMsg();
    u32 SendDlgMsg();
public:
    //子类自己的数据成员
    string m_strUserName;
    string m_strSession;
    TSipURI m_tServerUri;
    TKDSIP_DIALOG_ID m_tSubscribeID;
	TKDSIP_DIALOG_ID m_tInviteID;
};

#endif

