/*========================================================================================
模块名    ：cmu
文件名    ：culogintask.h
相关文件  ：
实现功能  ：cu登录事务
作者      ：fanxg
版权      ：<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2012/01/08         V1.0              fanxg                                  新建文件
=========================================================================================*/

#ifndef _CU_LOGIN_CMU_TASK_H
#define _CU_LOGIN_CMU_TASK_H

#include "loginsession.h"

class CCuiRegTask;

class CCuLoginTask : public CLoginSession
{
public:
    enum
    {
        WaitLogin = UNKNOWN_STATE+1,
        Service,
    };

public:
    CCuLoginTask(CInstExt *pcInst);
    virtual ~CCuLoginTask();

public:
    virtual const char* GetObjName() const
    {
        return "CCuLoginTask";
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

public:
    virtual void InitStateMachine();

public:
    //状态处理
    u32 OnWaitLogin(CMessage *const pcMsg);
    u32 OnWaitLoginTimer();

    u32 OnService(CMessage *const pcMsg);
    u32 OnServiceTimer();

public:
    virtual void TerminateTask();

public:
    //业务处理    
    void DisProc(CMessage *const pcMsg);
    void LogoutProc(CMessage *const pcMsg);
    void DestroyAllTasks();

public:
    //数据操作
    const string GetUserName() const
    {
        return m_strUserName;
    }
    void SetUserName(const string& tSipUri)
    {
        m_strUserName = tSipUri;
    }

    const string GetCuVer() const
    {
        return m_strCuVer;
    }
    void SetCuVer(const string& strCuVer)
    {
        m_strCuVer = strCuVer;
    }

    const string GetCmuVer() const
    {
        return m_strCmuVer;
    }
    void SetCmuVer(const string& strCmuVer)
    {
        m_strCmuVer = strCmuVer;
    }

    const string GetSession() const
    {
        return m_strSession;
    }
    void SetSession(const string& strSession)
    {
        m_strSession = strSession;
    }

private:
    string m_strUserName;   //cu登录用户名
    string m_strCuVer;     //cu版本
    string m_strCmuVer;    //cmu版本
    string m_strSession;   //登录session
};

typedef CStrMap<CLoginSession*> CCuMap;

class CCuList : public CCuMap
{
public:
    void PrintData();
};
extern CCuList g_cCuList;




#endif  //#ifndef _CU_LOGIN_CMU_TASK_H