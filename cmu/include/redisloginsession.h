/*========================================================================================
ģ����    ��cmu
�ļ���    ��redisloginsession.h
����ļ�  ��
ʵ�ֹ���  ��ʵ�ֻ���redissdk��ģ��ע�Ṧ��
����      ��fanxg
��Ȩ      ��<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
�޸ļ�¼��
����               �汾              �޸���             �߶���              �޸ļ�¼
2012/02/06         V1.0              fanxg                                  �½��ļ�
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