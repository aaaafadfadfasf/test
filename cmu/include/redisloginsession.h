/*========================================================================================
模块名    ：cmu
文件名    ：redisloginsession.h
相关文件  ：
实现功能  ：实现基于redissdk的模块注册功能
作者      ：fanxg
版权      ：<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2012/02/06         V1.0              fanxg                                  新建文件
=========================================================================================*/

#ifndef _REDIS_LOGIN_SESSION_H
#define _REDIS_LOGIN_SESSION_H

#include "cms/ospsip/ospsip.h"
#include "cms/container/xset.h"
#include "cms/cms_struct.h"
#include "cms/uas//uas_struct.h"
#include "loginsession.h"
#include "redisssmng.h"

class CRedisLoginSessionMng;

class CRedisLoginSession : public CLoginSession
{
    friend class CRedisLoginSessionMng;
public:
    enum
    {
        Service
    };
protected:
    CRedisLoginSession(CInstExt *pcInst, CRedisLoginSessionMng *pcMng, const string &strSession);

public:
    virtual ~CRedisLoginSession();

public:
    
    virtual const char *GetObjName(void) const { return "CRedisLoginSession"; }

    virtual void InitStateMachine(void)
    { 
        NextState(Service); 
    }

    virtual const char* GetStateName(u32 dwState) const
    {
        switch (dwState)
        {
        case Service:
            return "Service";
        default:
            break;
        }
        return "Unknown State";
    }

    virtual bool IsService() const { return true; }
    virtual bool IsNotNeedPollState() { return true; }

private:
    CRedisLoginSessionMng *m_pcMng;
};

class CRedisLoginSessionMng
{
    friend class CRedisLoginSession;
public:
    CRedisLoginSessionMng();
    ~CRedisLoginSessionMng();

    static CRedisLoginSessionMng *GetInstance();

    CRedisLoginSession *GetLoginSession(const string &strSession);

    void PrintData() const;

private:
    map<string, CRedisLoginSession *> m_cLoginSessions;

    static CRedisLoginSessionMng *m_pcInst;
};

#endif  //#ifndef _REDIS_LOGIN_SESSION_H