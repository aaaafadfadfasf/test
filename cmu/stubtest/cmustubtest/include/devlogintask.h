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

#include "cms/cmu/cmu_struct.h"

#include "loginsession.h"

#define    DEV_LOGIN_TIMEOUT     (10)  //s 

class CLoginTask : public CLoginSession
{
public:
    CLoginTask(CInstExt *pcInst) : CLoginSession(pcInst)
    {

    }
    virtual ~CLoginTask()
    {

    }

public:
    //数据操作
    void SetDevUri(const string& strDevUri)
    {
        m_strDevUri = strDevUri;
    }
    string& GetDevUri()
    {
        return m_strDevUri;
    }
    const string& GetDevUri() const
    {
        return m_strDevUri;
    }

    void SetDevType(const string& strDevType)
    {
        m_strDevType = strDevType;
    }
    string& GetDevType()
    {
        return m_strDevType;
    }
    const string& GetDevType() const
    {
        return m_strDevType;
    }

    void SetDevAddrList(const TNetAddrList& cDevAddrList)
    {
        m_tDevAddrList = cDevAddrList;
    }
    TNetAddrList& GetDevAddrList()
    {
        return m_tDevAddrList;
    }
    const TNetAddrList& GetDevAddrList() const
    {
        return m_tDevAddrList;
    }


    void SetServerUri(const string& strServerUri)
    {
        m_strServerUri = strServerUri;
    }
    string& GetServerUri()
    {
        return m_strServerUri;
    }
    const string& GetServerUri() const
    {
        return m_strServerUri;
    }

    //
    TSipURI GetDevSipUri() const
    {
        TSipURI tUri;
        tUri.SetURIString(GetDevUri());
        return tUri;
    }

    TSipURI GetServerSipUri() const
    {
        TSipURI tUri;
        tUri.SetURIString(GetServerUri());
        return tUri;
    }

    virtual void PrintData() const
    {
        CTask::PrintData();

        OspPrintf(TRUE, FALSE, "\n  登录服务器[%s]\n", 
            m_strServerUri.c_str());
        OspPrintf(TRUE, FALSE, "  登录设备[%s-%s]\n", 
           m_strDevType.c_str(), m_strDevUri.c_str());

        OspPrintf(TRUE, FALSE, "  登录设备地址列表：\n");
  
        u32 i = 0;

        typedef TNetAddrList::const_iterator   VI;
        for (VI it=m_tDevAddrList.begin(); it!=m_tDevAddrList.end(); ++it)
        {
            i++;
            OspPrintf(TRUE, FALSE, "  %4u. [%s-%d]\n", i, it->GetNetIp().c_str(), it->GetNetPort());
        }    
    }

private:
    //登录设备信息
    string           m_strDevUri;                   //设备URI
    string           m_strDevType;                  //设备类型
    TNetAddrList     m_tDevAddrList;                //设备地址列表

    //登录服务器信息
    string           m_strServerUri;                //服务器URI
};

//登录发起者事务
class CLoginSenderTask : public CLoginTask
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
class CLoginRecverTask : public CLoginTask
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

typedef CStrMap<CLoginTask*> CLoginTaskMap;

class CGeneralLoginList : public CLoginTaskMap
{
public:
    void PrintData();
};
extern CGeneralLoginList g_cGeneralLoginList;




#endif  //#ifndef _DEV_LOGIN_TASK_H