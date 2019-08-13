/*========================================================================================
模块名    ：cms
文件名    ：devlogintask.h
相关文件  ：
实现功能  ：通用设备登录事务，分为两种事务: 登录发起者和登录接收者
作者      ：fanxg
版权      ：<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2012/07/06         V1.0              fanxg                                  新建文件
=========================================================================================*/

#ifndef _DEV_LOGIN_TASK_H
#define _DEV_LOGIN_TASK_H

#include "loginsession.h"

#define    DEV_LOGIN_TIMEOUT     (10)  //s 

//登录发起者事务
class CLoginSenderTask : public CLoginSession
{
public:
    enum
    {
        WaitLogin = UNKNOWN_STATE+1,
        Service,
        WaitLogout,     
    };

public:
    CLoginSenderTask(CInstExt *pcInst);
    virtual ~CLoginSenderTask();

public:
    virtual const char* GetObjName() const
    {
        #define   TASK_TYPE_LOGIN_SENDER    "CLoginSenderTask"

        return "CLoginSenderTask";
    }

    virtual const char* GetStateName(u32 dwState) const
    {
        switch (dwState)
        {
        case WaitLogin:
            return "WaitLogin";
        case Service:
            return "Service";
        case WaitLogout:
            return "WaitLogout";
        default:
            break;
        }
        return "Unknown State";
    }

    virtual bool IsNotNeedPollState()
    {
        return (GetState() == Service);
    }

public:
    virtual void InitStateMachine();

public:
    //状态处理
    u32 OnWaitLogin(CMessage *const pcMsg);
    u32 OnWaitLoginTimer();

    u32 OnService(CMessage *const pcMsg);
    u32 OnServiceTimer();

    u32 OnWaitLogout(CMessage *const pcMsg);
    u32 OnWaitLogoutTimer();

    virtual TDeviceLogInfo GetDevLogInfo(bool bOnOffState);

public:
    void ReleaseResource();

public:
    //业务处理

    //开启登录、登出
    void StartLogin();
    void StartLogout();

    bool IsService() const
    {
        return (GetState() == Service);
    }
};

//登录接收者事务
class CLoginRecverTask : public CLoginSession
{
public:
    enum
    {
        WaitLogin = UNKNOWN_STATE+1,
        Service, 
    };

public:
    CLoginRecverTask(CInstExt *pcInst);
    virtual ~CLoginRecverTask();

public:
    virtual const char* GetObjName() const
    {
        #define   TASK_TYPE_LOGIN_RECVER    "CLoginRecverTask"

        return "CLoginRecverTask";
    }

    virtual const char* GetStateName(u32 dwState) const
    {
        switch (dwState)
        {
        case WaitLogin:
            return "WaitLogin";
        case Service:
            return "Service";
        default:
            break;
        }
        return "Unknown State";
    }

    virtual bool IsNotNeedPollState()
    {
        return (GetState() == WaitLogin || GetState() == Service);
    }

public:
    virtual void InitStateMachine();

public:
    //状态处理
    u32 OnWaitLogin(CMessage *const pcMsg);
    u32 OnWaitLoginTimer();

    u32 OnService(CMessage *const pcMsg);
    u32 OnServiceTimer();

public:
    void ReleaseResource();

public:
    bool IsService() const
    {
        return (GetState() == Service);
    }
};

typedef CStrMap<CLoginSession*> CLoginTaskMap;

class CGeneralLoginList : public CLoginTaskMap
{
public:
    void PrintData();
};
extern CGeneralLoginList g_cGeneralLoginList;




#endif  //#ifndef _DEV_LOGIN_TASK_H