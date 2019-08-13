/*========================================================================================
模块名    ：cmu
文件名    ：regtastask.h
相关文件  ：
实现功能  ：登录TAS事务
作者      ：fanxg
版权      ：<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2011/12/18         V1.0              fanxg                                  新建文件
=========================================================================================*/

#ifndef _CMU_REG_TAS_TASK_H
#define _CMU_REG_TAS_TASK_H

#include "loginsession.h"

class CCapAliasQryTask;
class CDomainMapQryTask;

class CRegTasTask : public CLoginSession
{
public:
    enum
    {
        Idle = UNKNOWN_STATE + 1,
        WaitRegTas,
		WaitQryRsp,
        Service,
    };

public:
    CRegTasTask(CInstExt *pcInst);
    virtual ~CRegTasTask();

public:
    virtual const char* GetObjName() const
    {
        return "CRegTasTask";
    }

    virtual const char* GetStateName(u32 dwState) const
    {
        switch (dwState)
        {
        case Idle:
            return "Idle";
        case WaitRegTas:
            return "WaitRegTas";
		case WaitQryRsp:
			return "WaitQryRsp";
        case Service:
            return "Service";
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
    u32 OnWaitRegTas(CMessage *const pcMsg);
    u32 OnWaitRegTasTimer();

	u32 OnWaitQryRsp(CMessage *const pcMsg);
	u32 OnWaitQryRspTimer();
	
    u32 OnService(CMessage *const pcMsg);
    u32 OnServiceTimer();

    virtual TDeviceLogInfo GetDevLogInfo(bool bOnOffState);

public:
    //业务处理
    u32 StartRegTas();
    u32 GetDevInfo();
    u32 StartQryCapAlias();
    u32 StartQryDomainMap();
    u32 StartQryNetAddr();
    void TasDisProc(CMessage *const pcMsg);

    bool IsService() const
    {
        return (GetState() == Service);
    }

public:
    //数据操作
    const string& GetUserName() const
    {
        return m_strUserName;
    }
    void SetUserName(const string& strUserName)
    {
        m_strUserName = strUserName;
    }
    const string& GetPassWord() const
    {
        return m_strPassWord;
    }
    void SetPassWord(const string& strPassWord)
    {
        m_strPassWord = strPassWord;
    }

    const TSipURI& GetTasUri() const
    {
        return m_tTasURI;
    }
    void SetTasUri(const TSipURI& tSipUri)
    {
        m_tTasURI = tSipUri;
    }

private:
    string m_strUserName;    //CMU登录3AS的用户名
    string m_strPassWord;    //CMU登录3AS的密码
    TSipURI m_tTasURI;       //3AS的URI
};


#endif  //#ifndef _CMU_REG_TAS_TASK_H