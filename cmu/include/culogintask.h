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

    virtual bool IsNotNeedPollState()
    {
        return (GetState() == WaitLogin || GetState() == Service);
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
    void ReleaseResource();

public:
    //ҵ����    
    bool IsService() const
    {
        return (GetState() == Service);
    }

    void LogoutProc(CMessage *const pcMsg);

    CCuiRegTask* GetCuiRegTask();      //��ȡcu;����cui��ע������
public:
    //���ݲ���
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
    TSipURI m_tUserName;   //cu��¼�û���
    string m_strCuVer;     //cu�汾

    TSipURI m_tCuiUri;     //cu��¼ʹ�õ�cui
    string m_strCuiIp;     //cu��¼ʹ�õ�cui��IP������vtdu��������ַѡ��
};

typedef CStrMap<CLoginSession*> CCuMap;

class CCuList : public CCuMap
{
public:
    void PrintData();
};
extern CCuList g_cCuList;




#endif  //#ifndef _CU_LOGIN_CMU_TASK_H