/*========================================================================================
ģ����    ��cmu
�ļ���    ��culogintask.h
����ļ�  ��
ʵ�ֹ���  ��cu��¼����
����      ��fanxg
��Ȩ      ��<Copyright(C) 2003-2011 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
�޸ļ�¼��
����               �汾              �޸���             �߶���              �޸ļ�¼
2012/01/08         V1.0              fanxg                                  �½��ļ�
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
    //״̬����
    u32 OnWaitLogin(CMessage *const pcMsg);
    u32 OnWaitLoginTimer();

    u32 OnService(CMessage *const pcMsg);
    u32 OnServiceTimer();

public:
    virtual void TerminateTask();

public:
    //ҵ����    
    void DisProc(CMessage *const pcMsg);
    void LogoutProc(CMessage *const pcMsg);
    void DestroyAllTasks();

public:
    //���ݲ���
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
    string m_strUserName;   //cu��¼�û���
    string m_strCuVer;     //cu�汾
    string m_strCmuVer;    //cmu�汾
    string m_strSession;   //��¼session
};

typedef CStrMap<CLoginSession*> CCuMap;

class CCuList : public CCuMap
{
public:
    void PrintData();
};
extern CCuList g_cCuList;




#endif  //#ifndef _CU_LOGIN_CMU_TASK_H