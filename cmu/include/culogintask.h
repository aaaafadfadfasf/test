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
    //业务处理    
    bool IsService() const
    {
        return (GetState() == Service);
    }

    void LogoutProc(CMessage *const pcMsg);

    CCuiRegTask* GetCuiRegTask();      //获取cu途经的cui的注册事务
public:
    //数据操作
    const TSipURI GetUserName() const
    {
        return m_tUserName;
    }
    void SetUserName(const TSipURI& tSipUri)
    {
        m_tUserName = tSipUri;
    }

    const string GetCuVer() const
    {
        return m_strCuVer;
    }
    void SetCuVer(const string& strCuVer)
    {
        m_strCuVer = strCuVer;
    }

    const string& GetCuiIp() const
    {
        return m_strCuiIp;
    }
    string& GetCuiIp()
    {
        return m_strCuiIp;
    }
    void SetCuiIp(const string& strCuiIp)
    {
        m_strCuiIp = strCuiIp;
    }

    virtual size_t GetDevNetSegNO();
	virtual string GetJoinInterfaceIp(const string &strSession);

private:
    TSipURI m_tUserName;   //cu登录用户名
    string m_strCuVer;     //cu版本

    TSipURI m_tCuiUri;     //cu登录使用的cui
    string m_strCuiIp;     //cu登录使用的cui的IP，用于vtdu的码流地址选择
};

typedef CStrMap<CLoginSession*> CCuMap;

class CCuList : public CCuMap
{
public:
    void PrintData();
};
extern CCuList g_cCuList;




#endif  //#ifndef _CU_LOGIN_CMU_TASK_H